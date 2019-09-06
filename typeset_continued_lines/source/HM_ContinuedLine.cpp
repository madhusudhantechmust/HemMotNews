/* 
	HM_ContinuedLine.cpp

	Author:			
	Description:	<describe the HM_ContinuedLine class here>
*/


//#include "ITextFrame.h"
#include "IParcel.h"
#include "IBoolData.h"
#include "ITextModel.h"
#include "ITextModelCmds.h"
#include "CL_NewsCadRect_ID.h"
#include "HM_Page.h"
#include "CL_Log_File.h"
#include "global_data.h"
#include "HM_ContinuedLine.h"
#include "SnpTextModelHelper.h"

#include "CL_InDesignStyleSheetHelper.h"

using namespace std;

HM_ContinuedLine::HM_ContinuedLine ()
	:	storyUidRef_(UIDRef::gNull)
{
}


HM_ContinuedLine::~HM_ContinuedLine ()
{
}


HM_ContinuedLine::HM_ContinuedLine (UIDRef storyUidRef, PMString & text)
	:	storyUidRef_(storyUidRef)
{
	text_ = text;
}


bool HM_ContinuedLine::Typeset_()
{
	LOG_BEGIN_FUNCTION;
	ASSERT (storyUidRef_ != UIDRef::gNull);
	bool x = false;
	
	// Set the type.  (See CL_Display_Ad::InsertAdDescriptiveText_.)
	InterfacePtr<ITextModel> textModel (storyUidRef_, UseDefaultIID());
	if (textModel)
	{
		// Use ITextModelCmds to create a command *** New in 3.0 ***
		InterfacePtr<ITextModelCmds> textModelCmds (
											textModel, UseDefaultIID());
		if (textModelCmds)
		{
			PMString pmstr = text_ + "\r";
            boost::shared_ptr<WideString> wideString (
								new WideString (pmstr.GetPlatformString ().c_str()));
            InterfacePtr<ICommand> insertTextCmd (
           							textModelCmds->InsertCmd (
           												0, wideString));
			if (insertTextCmd)
			{
				CmdUtils::ProcessCommand (insertTextCmd);

				// Finally, apply the stylesheet.
				auto_ptr<CL_InDesignStyleSheetHelper> styleSheetHelper (
											new CL_InDesignStyleSheetHelper);
				PMString head2 ("Head2");
				UID head2StyleSheetUID = styleSheetHelper->FindByName_(head2);
				if (head2StyleSheetUID == kInvalidUID)
				{
					// Needed style sheet wasn't found -- create it now.
					PMString head1 ("Head1");
					head2StyleSheetUID = 
								styleSheetHelper->DuplicateParagraphStyle_(
																head1, head2);
				}
				styleSheetHelper->ApplyStyleSheet_(textModel, head2StyleSheetUID);
				
				// Append the paragraph "continued" and style that, too.
                boost::shared_ptr<WideString> continuedWideString (
										new WideString ("continued"));
				int32 storyLen = textModel->TotalLength ();
				InterfacePtr<ICommand> insertTextCmd2 (
           								textModelCmds->InsertCmd (
											storyLen - 1, continuedWideString));
	           	if (insertTextCmd2)
	           	{
					CmdUtils::ProcessCommand (insertTextCmd2);
					
					// Style it.
					PMString pmstrContinued ("Continued");
					UID continuedSSUID = styleSheetHelper->FindByName_(
																	pmstrContinued);
					if (continuedSSUID == kInvalidUID)
					{
						// Needed style sheet wasn't found -- create it now.
						continuedSSUID = 
									styleSheetHelper->CreateContinuedStyleSheet_();
					}
					styleSheetHelper->ApplyStyleSheet_(
											textModel, continuedSSUID, storyLen, 1);
	           	}
				
			}
		}
	}

	LOG_END_FUNCTION;	
	return x;
}

//----------------------------------------------------------------------------------
//	SetBoxDepthToTextDepth_
//
// Set the depth of the text box equal to the depth of its text.
// See the old QXT code in /qxpdev/QXP4/NewsCAD/XTension/Source/HM_Page.cpp.
//----------------------------------------------------------------------------------
void HM_ContinuedLine::SetBoxDepthToTextDepth_(UIDRef & textBoxUidRef)
{
	LOG_BEGIN_FUNCTION;
	InterfacePtr<ICommand> cmd (
						CmdUtils::CreateCommand (kFitFrameToContentCmdBoss));
	UIDList newList (textBoxUidRef);
	InterfacePtr<IBoolData> adjustWidth ( cmd, UseDefaultIID () );
	adjustWidth->Set (kFalse);
	cmd->SetItemList ( newList );
	ErrorCode result = CmdUtils::ProcessCommand (cmd);
	LOG_END_FUNCTION;
}
