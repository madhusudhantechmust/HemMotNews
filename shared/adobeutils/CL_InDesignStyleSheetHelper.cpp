/* 
	CL_InDesignStyleSheetHelper.cpp

	Author:			
	Description:	<describe the CL_InDesignStyleSheetHelper class here>
*/

#include "ITextModelCmds.h"
#include "ITextAttributes.h"
#include "ITextAttrAlign.h"
#include "IFontMgr.h"
#include "IPMFont.h"
#include "IDocFontMgr.h"
#include "ITextAttrUID.h"
#include "ITextAttrBoolean.h"
#include "ITextAttrRealNumber.h"
#include "IStyleNameTable.h"
#include "IStyleGroupManager.h"
#include "CL_Document.h"
#include "CL_InDesignStyleSheetHelper.h"
#include "CL_Log_File.h"
#include "global_data.h"

using namespace std;

CL_InDesignStyleSheetHelper::CL_InDesignStyleSheetHelper()
{
}


CL_InDesignStyleSheetHelper::~CL_InDesignStyleSheetHelper()
{
}


UID CL_InDesignStyleSheetHelper::FindByName_(PMString & inSSName) const
{
	LOG_BEGIN_FUNCTION;
	DECLARE_logstr;
	::sprintf (logstr, "<!-- Trying to find para style sheet named %s -->", 
		inSSName.GetPlatformString().c_str());
	LOG (logstr);
	UID retval = kInvalidUID;

//	IDocument* document = ::GetFrontDocument ();
	auto_ptr<CL_Document> apCurrentDocument (new CL_Document);
	CL_Document* pCurDoc = apCurrentDocument.get ();
	IDocument* document = pCurDoc->IDocument_();
	InterfacePtr<IWorkspace> iWorkSpace (
								document->GetDocWorkSpace(), UseDefaultIID());
//?	InterfacePtr<IStyleNameTable> styleTable (iWorkSpace, IID_IPARASTYLENAMETABLE);
	InterfacePtr<IStyleGroupManager> styleTable(iWorkSpace, IID_IPARASTYLEGROUPMANAGER);
	retval = styleTable->FindByName (inSSName);

	if (retval == kInvalidUID)
	{
		LOG ("<!-- Could not find style sheet. -->");
	}
	LOG_END_FUNCTION;

	return retval;
}


UID CL_InDesignStyleSheetHelper::DuplicateParagraphStyle_(
					PMString & inSourceStyleName, PMString & inNewStyleName) const
{
	LOG_BEGIN_FUNCTION;
	DECLARE_logstr;
	::sprintf (logstr, 
		"<!-- Duplicate style sheet named %s; name new style sheet %s. -->",
		inSourceStyleName.GetPlatformString().c_str (), inNewStyleName.GetPlatformString ().c_str() );

	UID retval = kInvalidUID;
	UID sourceStyleSheetUid = kInvalidUID;

	sourceStyleSheetUid = FindByName_(inSourceStyleName);
	
	if (sourceStyleSheetUid == kInvalidUID)			// Exit immediately on failure
	{
		LOG ("<!-- Could not find style sheet to duplicate (source style) -->");
		LOG_END_FUNCTION;
		return retval;
	}
	
	// Source style will be the 'based-on' style for the new style
	UID baseOnStyle = sourceStyleSheetUid;
	InterfacePtr<ICommand> cmdNewStyle (
								CmdUtils::CreateCommand (kCreateParaStyleCmdBoss));
	InterfacePtr<IStyleInfo> pInfo ( cmdNewStyle, UseDefaultIID () );
//	IDocument* document = ::GetFrontDocument ();
	auto_ptr<CL_Document> apCurrentDocument (new CL_Document);
	CL_Document* pCurDoc = apCurrentDocument.get ();
	IDocument* document = pCurDoc->IDocument_();
	InterfacePtr<IWorkspace> iWorkSpace (document->GetDocWorkSpace(), 
																UseDefaultIID());
	cmdNewStyle->SetItemList ( UIDList (iWorkSpace) );

	// Make the source style the 'based-on' of the new style
	// so it will adopt all of its attributes
	pInfo->SetBasedOn (baseOnStyle);
/*
	// Add custom override(s)
	InterfacePtr<ITextAttributes> attrList ( cmdNewStyle,UseDefaultIID () );
	if ( attrList )
	{
		// We add align-center attribute
		InterfacePtr<ITextAttrAlign> alignAttr (
				(ITextAttrAlign*)::CreateObject (
				kTextAttrAlignmentBoss, IID_ITEXTATTRALIGN) );
		alignAttr->Set ( ICompositionStyle::kTextAlignRight );
		attrList->ApplyAttribute ( alignAttr );
	}
*/
	pInfo->SetName (inNewStyleName);
	pInfo->SetIsStyleImported (kFalse);
	if ( CmdUtils::ProcessCommand (cmdNewStyle) == kSuccess )
	{
		// Return UID of new style
//		InterfacePtr<IStyleNameTable> styleTable ( 
//					iWorkSpace, IID_IPARASTYLENAMETABLE );
		retval = FindByName_(inNewStyleName);
	}
	
	LOG_END_FUNCTION;
	return retval;
}

/*--------------------------------------------------------------------------------*/

void CL_InDesignStyleSheetHelper::ApplyStyleSheet_(
					ITextModel* inITextModel, UID inStyleUid, 
					TextIndex inStartOffset, int32 inLength) const
{
	InterfacePtr<ITextModelCmds> cmds ( inITextModel, UseDefaultIID() );

	// In this case the story consists of only one sentence so we can apply the 
	// style to position 1 to have it apply to the entire (one line) paragraph
	InterfacePtr<ICommand> cmd (cmds->ApplyStyleCmd (inStartOffset, inLength, 
												inStyleUid, kParaAttrStrandBoss) );
	if ( cmd )
	{
		CmdUtils::ProcessCommand (cmd);
	}
}

/*--------------------------------------------------------------------------------*/
// SEE SnpManipulateTextStyle::CreateStyle (...)
UID CL_InDesignStyleSheetHelper::CreateContinuedStyleSheet_()
{
	LOG_BEGIN_FUNCTION;
	UID retval = kInvalidUID;

//	IDocument* document = ::GetFrontDocument ();
	auto_ptr<CL_Document> apCurrentDocument (new CL_Document);
	CL_Document* pCurDoc = apCurrentDocument.get ();
	IDocument* document = pCurDoc->IDocument_();
	InterfacePtr<IWorkspace> iWorkSpace (document->GetDocWorkSpace(),
																UseDefaultIID());
	// For character styles, replace IID_IPARASTYLENAMETABLE with 
	// IID_ICHARSTYLENAMETABLE in the next line.
//?	InterfacePtr<IStyleNameTable> pStyleTable (iWorkSpace, IID_IPARASTYLENAMETABLE);
	InterfacePtr<IStyleGroupManager> pStyleTable(iWorkSpace, IID_IPARASTYLEGROUPMANAGER);


	// For char styles, replace kCreateParaStyleCmdBoss 
	// with kCreateCharStyleCmdBoss in the next line.
	InterfacePtr<ICommand> cmdNewStyle (
								CmdUtils::CreateCommand (kCreateParaStyleCmdBoss));
	InterfacePtr<IStyleInfo> pInfo ( cmdNewStyle, UseDefaultIID () );
	cmdNewStyle->SetItemList ( UIDList (iWorkSpace) );

	pInfo->SetBasedOn (kInvalidUID);						// Based on root style

	// Add custom overrides
	InterfacePtr<ITextAttributes> attrList( cmdNewStyle, UseDefaultIID());
	if ( attrList )
	{
		// Alignment
		InterfacePtr<ITextAttrAlign> 
				alignAttr ((ITextAttrAlign*)::CreateObject (
							kTextAttrAlignmentBoss,IID_ITEXTATTRALIGN));
		alignAttr->Set( ICompositionStyle::kTextAlignCenter );
		attrList->ApplyAttribute ( alignAttr );
		
		// Font UID
//		PMString myFont ("OptimaLTStd", -1, PMString::kNoTranslate);
		PMString myFont ("OptimaLTStd", PMString::kUnknownEncoding);
		
//		InterfacePtr<IFontMgr> appFontMgr ( gSession, UseDefaultIID () );
		InterfacePtr<IFontMgr> appFontMgr ( GetExecutionContextSession (), UseDefaultIID () );
		InterfacePtr<IPMFont> ipmFont ( appFontMgr->QueryFont (myFont) );
		if ( ipmFont )
		{
			InterfacePtr<IDocFontMgr> fontMgr ( iWorkSpace, IID_IDOCFONTMGR );
			UID fontUID = fontMgr->GetFontFamilyUID ( ipmFont );
			if ( fontUID != kInvalidUID )
			{
				InterfacePtr<ITextAttrUID> 
						fontAttr((ITextAttrUID*)::CreateObject (
										kTextAttrFontUIDBoss, IID_ITEXTATTRUID) );
				fontAttr->SetUIDData (fontUID);
				attrList->ApplyAttribute ( fontAttr );
			}
		}
		
		// Point size:
		// Create a container for the point size character attribute to be applied.
		InterfacePtr<ITextAttrRealNumber> textAttrPointSize (
					CreateObject2<ITextAttrRealNumber> (kTextAttrPointSizeBoss));
		if (!textAttrPointSize)
		{
			LOG ("Could not create textAttrPointSize");
			goto cleanup;
		}
		textAttrPointSize->SetRealNumber (10.0);
		attrList->ApplyAttribute ( textAttrPointSize );

		// Leading:
		// Create a container for the leading attribute.
		InterfacePtr<ITextAttrRealNumber> textAttrLeading (
							CreateObject2<ITextAttrRealNumber> (kTextAttrLeadBoss));
		if (!textAttrLeading)
		{
			LOG ("Could not create textAttrLeading");
			goto cleanup;
		}
		textAttrLeading->SetRealNumber (10.5);
		attrList->ApplyAttribute ( textAttrLeading );
/*
		// para rule below
		InterfacePtr<ITextAttrBoolean> praAttr ((ITextAttrBoolean*)::CreateObject (
									kTextAttrPRARuleOnBoss, IID_ITEXTATTRBOOLEAN));
		if (!textAttrLeading)
		{
			LOG ("Could not create praAttr");
			goto cleanup;
		}
		praAttr->SetFlag (kTrue);
		attrList->ApplyAttribute ( praAttr );
*/
	}

cleanup:	
	PMString pmstrContinued ("Continued");
	pInfo->SetName (pmstrContinued);
	pInfo->SetIsStyleImported (kFalse);

	if ( CmdUtils::ProcessCommand (cmdNewStyle) == kSuccess )
	{
		// Return UID of new style
		retval = FindByName_(pmstrContinued);
	}
	
	LOG_END_FUNCTION;

	return retval;
}


// END OF FILE

/*
The style attributes you have listed all apply to paragraph styles.
You can create a style that is not derived from an existing style by just passing kInvalidUID as the based-on style UID (this actually bases the style on the root style, which defines defaults for all of the attributes).  The following example does this.  It also adds a font UID and paragraph-rule-below attributes.  Note that you'll need to replace 'TheFontPostscriptName' with the postscript name of the font you want to use.  Note that character styles are created the same way with the changes noted..

*InterfacePtr<IWorkspace> iWorkSpace (document->GetDocWorkSpace(), UseDefaultIID());
*InterfacePtr<IStyleNameTable> pStyleTable(iWorkSpace, IID_IPARASTYLENAMETABLE);  // *replace with IID_ICHARSTYLENAMETABLE for character styles

*InterfacePtr<ICommand> cmdNewStyle(CmdUtils::CreateCommand(kCreateParaStyleCmdBoss)); // replace with kCreateCharStyleCmdBoss for character styles
*InterfacePtr<IStyleInfo> pInfo(cmdNewStyle, UseDefaultIID());
*cmdNewStyle->SetItemList( UIDList(iWorkSpace));

*pInfo->SetBasedOn(kInvalidUID); // based on root style

// add custom overrides
InterfacePtr<ITextAttributes> attrList( cmdNewStyle,UseDefaultIID());
if ( attrList )
{
// alignment
InterfacePtr<ITextAttrAlign> alignAttr((ITextAttrAlign*)::CreateObject(kTextAttrAlignmentBoss,IID_ITEXTATTRALIGN));
alignAttr->Set( ICompositionStyle::kTextAlignCenter );
attrList->ApplyAttribute( alignAttr );

// font UID
PMString myFont("TheFontPostscriptName",-1,PMString::kNoTranslate);

InterfacePtr<IFontMgr> appFontMgr(gSession,UseDefaultIID());
InterfacePtr<IPMFont> ipmFont(appFontMgr->QueryFont(myFont));
if ( ipmFont )
{
InterfacePtr<IDocFontMgr> fontMgr(iWorkSpace, IID_IDOCFONTMGR);
UID fontUID = fontMgr->GetFontFamilyUID( ipmFont );
if ( fontUID != kInvalidUID )
{
InterfacePtr<ITextAttrUID> fontAttr((ITextAttrUID*)::CreateObject(kTextAttrFontUIDBoss,IID_ITEXTATTRUID));
fontAttr->SetUIDData(fontUID);
attrList->ApplyAttribute( fontAttr );
}
}

// para rule below
InterfacePtr<ITextAttrBoolean> prbAttr((ITextAttrBoolean*)::CreateObject(kTextAttrPRBRuleOnBoss,IID_ITEXTATTRBOOLEAN));
prbAttr->SetFlag(kTrue);
attrList->ApplyAttribute( prbAttr );
}

pInfo->SetName(newStyleName);
pInfo->SetIsStyleImported(kFalse);

CmdUtils::ProcessCommand(cmdNewStyle)
*/
