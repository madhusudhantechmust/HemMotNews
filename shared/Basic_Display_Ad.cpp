/* 
	Basic_Display_Ad.cpp

	Author:			
	Description:	<describe the Basic_Display_Ad class here>

	cl	9/12/10	Re-examined as part of porting to CS4; no porting changes 
	cl			were required to compile this file successfully.
*/

//	Fields in input record for display ads::
//
//	Field						Macro in old			
//	Number	Contents			CL_Display_Ad		New Macro
//	------	----------------	-------------		---------
//	 1		Columns             xPos				COLCOUNT
//	 2		Depth in Points     yPos				DEPTH
//	 3		Customer Name       clientPos			CUSTOMER
//	 4		Ad File Name        filePos				FILENAME
//	 5		Sales Rep Name      initPos				SALESPERSON
//	 6		Absolute Page       bPos				ABSOLUTE_PAGE
//	 7		Begin Page          pagePos				LOWER_PAGE
//	 8		End Page            lowPos				UPPER_PAGE
//	 9		Odd Page Flag       UpPos				ODD_PAGE
//	10		Camera Ready Flag   OddPos				CAMERA_READY
//	11		Color               LocalPos			COLOR
//	12		Notes               colorPos			NOTES
//	13		Section             Not in old code		SECTION
//	14		Placement Category	Not in old code		DA_CATEGORY
using namespace std;
#include "Basic_Display_Ad.h"
#include "PMRect.h"
#include "Basic_Page.h"
	#include "ILayoutUIUtils.h"
	#include "ILayoutControlData.h"
	#include "IHierarchy.h"
	#include "IStandOff.h"
	#include "SDKLayoutHelper.h"
	#include "IBlackBoxCommands.h"
	#include "IBlackBoxCmdData.h"
 	#include "ILayoutUIUtils.h"
// 	#include "IMultiColumnItemData.h"
 	#include "IFrameUtils.h"
	#include "ITextWrapFacade.h"
	#include "ITextModel.h"
	#include "ITextModelCmds.h"
	#include "ITextAttrAlign.h"
	#include "ITextAttrUtils.h"
	#include "SnpTextModelHelper.h"
	#include "ITextAttrRealNumber.h"
	#include "IFrameContentFacade.h"
	#include "SnpGraphicHelper.h"
	#include "IIntData.h"
	#include "CTextEnum.h"
	#include "IGroupCmdData.h"
#include "CAlert.h"
#include "newscad.h"
#include "global_data.h"
#include "tokenizer.hpp"
#include "CL_Log_File.h"
typedef boost::tokenizer<boost::char_separator<char> > tokenizer;

// Some handy macros.
#if CLIENT == HEMMINGS || CLIENT == MOUNTAIN_ENTERPRISE
	#define	POINT_SIZE_AND_LEADING	10.0
	#define	AD_TEXTBOX_SHADE		0L
	#define	AD_TEXTBOX_COLOR		COLOR_WHITE
#elif CLIENT == THRIFTY_NICKEL
	#define	POINT_SIZE_AND_LEADING	10L
	#define	AD_TEXTBOX_SHADE		13107L
	#define	AD_TEXTBOX_COLOR_0		COLOR_BLACK
	#define	AD_TEXTBOX_COLOR_1		COLOR_YELLOW
	#define	AD_TEXTBOX_COLOR_2		COLOR_MAGENTA
#else
	// CAPE GAZETTE and REMINDER
	#define	POINT_SIZE_AND_LEADING	24L
	#define	AD_TEXTBOX_SHADE		13107L
	#define	AD_TEXTBOX_COLOR		COLOR_BLACK
#endif

#define		WANT_DEBUG_ALERTS		0


//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and D E S T R U C T I O N
//----------------------------------------------------------------------------------

/* --------------------- Default CTOR, not very interesting --------------------- */
Basic_Display_Ad::Basic_Display_Ad ()
	:	cols_(0)
	,	height_(0)
	,	absolutePage_(0)
	,	lowerPage_(0)
	,	upperPage_(0)
	,	oddPageFlag_(false)
	,	cameraReadyFlag_(false)
	,	category_(0)
	,	width_(0)
	,	boxX_(0)
	,	boxY_(0)
	,	pMyPage_(NULL)
{
	Initialize_();
}


/* ------------------------------ Copy Constructor ------------------------------ */
Basic_Display_Ad::Basic_Display_Ad (const Basic_Display_Ad & inObj)
{
	CopyAllMembers_(inObj);
}


/* ---------------------------- Assignment Operator ----------------------------- */
Basic_Display_Ad & Basic_Display_Ad::operator= (const Basic_Display_Ad & inObj)
{
	if (this != &inObj)									// Beware of "a = a"
	{
		CopyAllMembers_(inObj);
	}

	return *this;
}


/* --------------------------------- Destructor --------------------------------- */
Basic_Display_Ad::~Basic_Display_Ad ()
{
}


/* ------------------------- Construct from STL string -------------------------- */
Basic_Display_Ad::Basic_Display_Ad (string & inputRec)
	: cols_(0)
	, height_(0)
	, absolutePage_(0)
	, lowerPage_(0)
	, upperPage_(0)
	, oddPageFlag_(false)
	, cameraReadyFlag_(false)
	, category_(0)
	, width_(0)
	, boxX_(0)
	, boxY_(0)
	, pMyPage_(NULL)
{
	Initialize_();								// To finish construction
	SetMemberVariablesFromSTLString_( inputRec );

/*
	// Save the input for storage in a QXP slug later on.
	inputRecord_ = inputRec;

	// Convert and store input into members.
	char ch;
	int fldnum = COLCOUNT;
	string curtok;
	string::iterator p = inputRecord_.begin ();
	
	while (p < inputRecord_.end () && fldnum <= SECTION)
	{
		ch = *p;
		switch (ch)
		{
			case '\r':								// End of field & record
			case '|':								// End of field
				ProcessField_(static_cast<FieldNumber>(fldnum), curtok);
				++fldnum;
				curtok.resize (0);
				break;
			default:								// Append character to token.
				curtok += ch;
				break;
		}		
		++p;
	}
*/

}


/*================================================================================*/
void Basic_Display_Ad::SetMemberVariablesFromSTLString_(string& inAdRecord) {
/*----------------------------------------------------------------------------------
	Abstract:
		Parse the input record into fields, encode, and store.
		
	Parameters and modes:
		inAdRecord	A string that contains all the data for an ad.		IN

	Returns:
		Nothing.
		
	Change Log:
		25-Nov-10	Adapted from PlaceAdPanelDisplayAd class.
----------------------------------------------------------------------------------*/
	// Save the input for storage in a QXP slug later on.
	inputRecord_ = inAdRecord;

	unsigned cntTokensProcessed = 0;
	boost::char_separator<char> sep("<>|\n\r", "<>", boost::keep_empty_tokens);
	tokenizer tokens (inAdRecord, sep);
	tokenizer::iterator tok_iter;
	for (tok_iter = tokens.begin (); tok_iter != tokens.end (); ++tok_iter)
	{
		string s = *tok_iter;
		++cntTokensProcessed;
		ProcessField_(static_cast<FieldNumber> (cntTokensProcessed), s);
	}
}


/* ---------------------------------- Clear_() ---------------------------------- */
//	Make an instance "tabula rasa."
//----------------------------------------------------------------------------------
void Basic_Display_Ad::Clear_()
{
	inputRecord_.resize (0);
	cols_ = 0;
	height_ = 0;
	customer_.resize (0);
	fileName_.resize (0);
	salesRepName_.resize (0);
	absolutePage_ = 0;
	lowerPage_ = 0;
	upperPage_ = 0;
	oddPageFlag_ = false;
	cameraReadyFlag_ = false;
	color_.resize (0);
	notes_.resize (0);
	section_.resize (0);
	category_ = 0;
	width_ = 0;
	boxX_ = 0;
	boxY_ = 0;
	picBoxUidRef_ = UIDRef::gNull;
	textBoxUidRef_ = UIDRef::gNull;
	pMyPage_ = NULL;
}

/* ------------------------------- Initialize_() -------------------------------- */
// Call this function only from a CTOR -- it clears only the members that 
// aren't dealt with by the initializer list in the CTOR.
//----------------------------------------------------------------------------------
void Basic_Display_Ad::Initialize_()
{
	inputRecord_.resize (0);
	customer_.resize (0);
	fileName_.resize (0);
	salesRepName_.resize (0);
	color_.resize (0);
	notes_.resize (0);
	section_.resize (0);
	pMyPage_ = NULL;
	picBoxUidRef_ = UIDRef::gNull;
	textBoxUidRef_ = UIDRef::gNull;
}


/* ------------------------------ CopyAllMembers_ ------------------------------- */
//	This method is called by the copy ctor and the assignment operator to set all 
//	data members.  Note my assumption that the STL takes care of deallocating the 
//	data in this's string objects when they get assigned.
//----------------------------------------------------------------------------------
void Basic_Display_Ad::CopyAllMembers_(const Basic_Display_Ad & inObj)
{
	inputRecord_ = inObj.inputRecord_;
	cols_ = inObj.cols_;
	height_ = inObj.height_;
	customer_ = inObj.customer_;
	fileName_ = inObj.fileName_;
	salesRepName_ = inObj.salesRepName_;
	absolutePage_ = inObj.absolutePage_;
	lowerPage_ = inObj.lowerPage_;
	upperPage_ = inObj.upperPage_;
	oddPageFlag_ = inObj.oddPageFlag_;
	cameraReadyFlag_ = inObj.cameraReadyFlag_;
	color_ = inObj.color_;
	notes_ = inObj.notes_;
	section_ = inObj.section_;
	category_ = inObj.category_;
	width_ = inObj.width_;
	boxX_ = inObj.boxX_;
	boxY_ = inObj.boxY_;
	picBoxUidRef_ = inObj.picBoxUidRef_;
	textBoxUidRef_ = inObj.textBoxUidRef_;
	pMyPage_ = inObj.pMyPage_;
}


//----------------------------------------------------------------------------------
// ProcessField_()
//
// Store a field value in the ad record into a member variable.  Convert text 
// to binary representation as needed.
//----------------------------------------------------------------------------------
void Basic_Display_Ad::ProcessField_(FieldNumber inFieldNumber, string& inString)
{
	// Strip leading spaces from the input
	string::iterator p;
	p = inString.begin ();
	while (*p == ' ')
	{
		inString.erase (p);
	}
	
	// Strip trailing spaces from the input
	size_t len = inString.length ();
	string::reverse_iterator q;
	q = inString.rbegin ();
	while (q != inString.rend ())
	{
		if (*q != ' ')
			break;

		--len;
		++q;
	}
	inString.resize (len);

 	PMReal fTemp;

 	const char *pInStr = inString.c_str ();
	PMString pmstr (pInStr);
	PMString::ConversionError convErr;
	CharCounter lenNotConverted;

	switch (inFieldNumber) {
	case COLCOUNT:							// Ad width in columns
 		// Get number of columns spanned by this ad, store in member var
 		long cols;
 		cols_ = cols = ::atol (pInStr);
		break;

 	case DEPTH:												// Ad height in points
		height_ = pmstr.GetAsDouble ( &convErr, &lenNotConverted);
 		break;
 
	case CUSTOMER:
		customer_ = inString;
		break;

	case FILENAME:
		fileName_ = inString;
		break;

	case SALESPERSON:
		salesRepName_ = inString;
		break;
		
	case ABSOLUTE_PAGE:
		absolutePage_ = ::atol (pInStr);
		break;

	case LOWER_PAGE:
		lowerPage_ = ::atol (pInStr);
		break;

	case UPPER_PAGE:
		upperPage_ = ::atol (pInStr);
		break;

	case ODD_PAGE:
		if (::atol (pInStr) != 0)
		{
			oddPageFlag_ = true;
		}
		break;

	case CAMERA_READY:
		if (::atol (pInStr) != 0)
		{
			cameraReadyFlag_ = true;
		}
		break;

	case COLOR:
		color_ = inString;
		break;

	case NOTES:
		notes_ = inString;
		break;

	case SECTION:
		section_ = inString;
		break;
	
	// The DA Placement Category is an integer value assigned to each ad record.
	// It's used to group ads that are in related categories but different headings.
	// E.g., we might have "Garbage for Sale" and "Real Junk for Sale" in cat 7.
	case DA_CATEGORY:
		category_ = ::atol (pInStr);
		break;

	default:
		// i don't know yet
		break;
	}	
}

void Basic_Display_Ad::ComputeWidth_(PMReal inColumnWidth, PMReal inGutterWidth)
{
	// Since an ad can't be any narrower than 1 column, we'll initialize to 
	// that width, then accumulate a column width for each additional column.
	PMReal width = inColumnWidth;
	int32 i;
	
	// Init the counter to 1 -- we've already tossed in the width of the 1st column.
	for (i = 1; i < GetColumnCount_(); i++)
	{
		width += inGutterWidth;
		width += inColumnWidth;
	}

	SetWidth_(width);
}

#include "SDKLayoutHelper.h"
#include "IStandOff.h"
#include "SnpGraphicHelper.h"

void Basic_Display_Ad::PasteUp_(UIDRef destinationSpreadUIDRef, 
									PMRect & boundsInParentCoords) throw (PMString)
{
	extern ErrorCode SetRunaround_(const UIDRef& graphicFrameUIDRef, 
						const PMReal inAmount = 0, 
						IStandOff::mode mode = IStandOff::kBothSides);
	extern ErrorCode SetIgnoreRunarounds_(
						const UIDRef& inTextFrameUIDRef, bool inIgnore);
#if CLIENT == MOUNTAIN_ENTERPRISE
	extern ErrorCode SetRunaround_(const UIDRef& graphicFrameUIDRef, 
						const PMReal &topMargin, 
						const PMReal &leftMargin, 
						const PMReal &bottomMargin, 
						const PMReal &rightMargin,
						IStandOff::mode mode = IStandOff::kBothSides);
#endif
						
	SDKLayoutHelper layoutHelper;
	UIDRef result = UIDRef::gNull;

	// Create the frame.
	result = layoutHelper.CreateRectangleFrame (
								destinationSpreadUIDRef, boundsInParentCoords);
	picBoxUidRef_ = result;
#if CLIENT == MOUNTAIN_ENTERPRISE
	// Customized behavior:
	ErrorCode e = SetRunaround_(result, 2.16, 5.04, 2.16, 5.04);
#else
	// Typical behavior:
	ErrorCode e = SetRunaround_(result, pMyPage_->GutterWidth_());
#endif
	
	// Add black box (slug) data.
	StoreInBoxSlug_(picBoxUidRef_);
	
	// Create the text box -- 1 column, horizontal text direction.
	UIDRef storyUIDRef;
	result = layoutHelper.CreateTextFrame (
								destinationSpreadUIDRef, boundsInParentCoords, 
								1, kFalse, &storyUIDRef);
	textBoxUidRef_ = result;

	// Add black box (slug) data.
	StoreInBoxSlug_(textBoxUidRef_);
	
	// Creating the text box using the Adobe-supplied SDKLayoutHelper 
	// makes a box with the user's default preferences.  We need to 
	// make sure the new box ignores text wrapping (runarounds), or 
	// the text won't show because of the picture box's runaround.
	e = SetIgnoreRunarounds_(textBoxUidRef_, true);

	// Make the text box white.
	UIDList txtBoxUidLst (textBoxUidRef_);
	SnpGraphicHelper graphicsHelper (txtBoxUidLst);
/*  ADOBE SAMPLE CODE:
	graphicHelper.AddFillRendering (
				graphicHelper.GetNamedSwatch(PMString("C=100 M=0 Y=0 K=0"), 
				frameUIDRef.GetDataBase()));
*/
	UID swatchUID = graphicsHelper.GetNamedSwatch (PMString("Paper"), 
											txtBoxUidLst.GetDataBase());
	graphicsHelper.AddFillRendering (swatchUID);
	graphicsHelper.ApplyAttributes ();

	// Add the text that describes the ad.
	InsertAdDescriptiveText_(storyUIDRef);

	// Center content vertically in the frame.
	InterfacePtr<ICommand> setFrameVertJustifyCmd (
				CmdUtils::CreateCommand (kSetFrameVertJustifyCmdBoss));
	if (setFrameVertJustifyCmd)
	{
		InterfacePtr<IIntData> vertJustValue (setFrameVertJustifyCmd, 
											UseDefaultIID());
		vertJustValue->Set (Text::kVJCenter);
		setFrameVertJustifyCmd->SetItemList (txtBoxUidLst);
		if (CmdUtils::ProcessCommand (setFrameVertJustifyCmd) != kSuccess)
		{
			throw "CANNOT CENTER CONTENT IN FRAME";
		}
	}
	else
	{
		throw "CANNOT EVEN CREATE A COMMAND TO CENTER CONTENT IN FRAME";
	}
	
	// Group the text and pic frames.
	InterfacePtr<ICommand> groupItemsCmd (
				CmdUtils::CreateCommand (kGroupCmdBoss));
	if (groupItemsCmd)
	{
		UIDList* itemsToGroup = new UIDList(textBoxUidRef_);
		itemsToGroup->Append ( picBoxUidRef_.GetUID () );
		InterfacePtr<IGroupCmdData> groupCmdData (groupItemsCmd, 
												UseDefaultIID());
		groupCmdData->Set (itemsToGroup);
		if (CmdUtils::ProcessCommand (groupItemsCmd) != kSuccess)
		{
			throw "CANNOT GROUP THE ITEMS";
		}
		else
		{
			// Add our black box data to the group item
			InterfacePtr<IHierarchy> hier (textBoxUidRef_,IID_IHIERARCHY );
			if ( hier )
			{
				InterfacePtr<IHierarchy> groupItem (hier->QueryParent());
				if (groupItem)
				{
					if (::GetClass (groupItem) == kGroupItemBoss)
					{
						UIDRef groupUidRef = ::GetUIDRef (groupItem);
						StoreInBoxSlug_(groupUidRef);
					}
					else
					{
						throw "TEXT FRAME PARENT IS NOT A GROUP";
					}
				}
				else
				{
					throw "QueryParent() RETURNED NULL FOR TEXT BOX IHierarchy";
				}
			}
			else 
			{
				throw "hier IS NULL FOR TEXT FRAME";
			}
		}
	}
}


#include "IBlackBoxCommands.h"
#include "IBlackBoxCmdData.h"
//#include "NewsCADPlaceAdID.h"

void Basic_Display_Ad::StoreInBoxSlug_(UIDRef & inUIDRef) throw (PMString)
{
	// Create the command that sets the IBPIData.
	InterfacePtr<ICommand> blackboxSetDataCmd (
		CmdUtils::CreateCommand (kBlackBoxAddDataCmdBoss));

DebugAlert_("StoreInBoxSlug_() called");

	if (blackboxSetDataCmd)
	{
		// Pass the attribute value to be applied.
		InterfacePtr<IBlackBoxCmdData> blackboxData (blackboxSetDataCmd, 
														UseDefaultIID());
		if (blackboxData)
		{
			// Set the value of the data.
			blackboxData->Set (inUIDRef, kNC_PrefixNumber, 
								(void*)inputRecord_.c_str (), 
								inputRecord_.length ());

			// Process the command to set the attribute for these objects.
			blackboxSetDataCmd->SetItemList ( UIDList (inUIDRef) );
			ErrorCode e = CmdUtils::ProcessCommand (blackboxSetDataCmd);

if (e) DebugAlert_("Error in CmdUtils::ProcessCommand (blackboxSetDataCmd)");

			// This is just "double check" code to see if the black 
			// box data really worked.
			InterfacePtr<IBlackBoxData> bbd (inUIDRef, UseDefaultIID());
			if (bbd)
			{
//				::sprintf (logstr, "Data Length = %d", 
//									bbd->GetDataLength (kWFPPluginID));
//				LOG (logstr);
if (bbd->GetDataLength (kNC_PrefixNumber) == 0)
DebugAlert_("Black box data length is 0!!");
				char logstr[64];
				bbd->ReadData (kNC_PrefixNumber, (void*)logstr, 32);
				logstr[32] = '\0';
DebugAlert_(logstr);
//				LOG (logstr);
			}
			else
			{
				throw "LOOKS LIKE BLACK BOX DATA WASN'T REALLY ADDED.  NUTS.";
			}
	 	}
		else
		{
			throw "COULD NOT SET THE COMMAND DATA";
		}
	}
	else
	{
		throw "COULD NOT CREATE COMMAND";
	}
}


#include "ITextModelCmds.h"
#include "ITextAttrAlign.h"
#include "ITextAttrUtils.h"
#include "SnpTextModelHelper.h"

void Basic_Display_Ad::InsertAdDescriptiveText_(UIDRef inStoryUidRef)
{
	InterfacePtr<ITextModel> textModel (inStoryUidRef, UseDefaultIID());
	if (textModel)
	{
		// Use ITextModelCmds to create a command *** New in 3.0 ***
		InterfacePtr<ITextModelCmds> textModelCmds (textModel, UseDefaultIID());
		if (textModelCmds)
		{
			string str;
			ComputeAdDescriptionText_(str);
			K2::shared_ptr<WideString> wideString (new WideString (str.c_str()));
            InterfacePtr<ICommand> insertTextCmd (
           								textModelCmds->InsertCmd (0, wideString));
			if (insertTextCmd)
			{
				CmdUtils::ProcessCommand (insertTextCmd);
				ApplyTextAttributeOverrides_(textModel, 
									0, textModel->TotalLength ());
			}
		}
	}
}

void Basic_Display_Ad::ComputeAdDescriptionText_(string & ioDescText)
{
		ioDescText = customer_;
		ioDescText += '\r';
		ioDescText += fileName_;
		ioDescText += '\r';
		ioDescText += color_;
#if CLIENT == HEMMINGS || CLIENT == MOUNTAIN_ENTERPRISE
		if (lowerPage_)
		{
			char pPageStr[256];
			sprintf (pPageStr, "%d", lowerPage_);
			ioDescText += '\r';
			ioDescText += pPageStr;
		}
		if (upperPage_)
		{
			char pPageStr[256];
			sprintf (pPageStr, "%d", upperPage_);
			ioDescText += '\r';
			ioDescText += pPageStr;
		}
#endif
#if CLIENT == HEMMINGS || CLIENT == MOUNTAIN_ENTERPRISE || CLIENT == REMINDER
		if (notes_.length ())
		{
			ioDescText += '\r';
			ioDescText += notes_;
		}
#endif
}


	#include "ITextAttrRealNumber.h"
	#include "IFrameContentFacade.h"
	#include "SnpGraphicHelper.h"
	#include "IIntData.h"
	#include "CTextEnum.h"
	#include "IGroupCmdData.h"

ErrorCode Basic_Display_Ad::ApplyTextAttributeOverrides_(ITextModel* textModel, 
									const TextIndex position, const int32 length)
									throw (PMString)
{
	ErrorCode status = kFailure;

	do
	{
		//--------------------------------------------------------------------------
		// First set the paragraph alignment to "center".
		//--------------------------------------------------------------------------
		// Create a container for the paragraph attribute to be applied.
		InterfacePtr<ITextAttrAlign> textAttrAlign (
							CreateObject2<ITextAttrAlign> (kTextAttrAlignmentBoss));
		if (!textAttrAlign) break;
		textAttrAlign->SetAlignment (ICompositionStyle::kTextAlignCenter);

		// Next step is to create a command to apply the attribute.
		Utils<ITextAttrUtils>()->BuildApplyTextAttrCmd (
				textModel, position, length, textAttrAlign, kParaAttrStrandBoss);
				
		// Then add that to a list of paragraph attributes -- probably dumb, but 
		// that's the only way I've gotten it to work.
		K2::shared_ptr<AttributeBossList> paraAttributeBossList (
														new AttributeBossList ());
		paraAttributeBossList->ApplyAttribute (textAttrAlign);

		// Use SnpTextModelHelper::ApplyOverrides to apply the attribute. We are 
		// applying a paragraph attribute, so we indicate kParaAttributeStrand.
		SnpTextModelHelper textCmdHelper;
		status = textCmdHelper.ApplyOverrides (textModel, position, length-1, 
										paraAttributeBossList, kParaAttrStrandBoss);
		if (status == kFailure) break;

		//--------------------------------------------------------------------------
		// Now set the point size and leading for the characters.
		//--------------------------------------------------------------------------
		status = kFailure;

		// Create a container for the point size character attribute to be applied.
		InterfacePtr<ITextAttrRealNumber> textAttrPointSize (
						CreateObject2<ITextAttrRealNumber> (kTextAttrPointSizeBoss));
		if (!textAttrPointSize) break;
		textAttrPointSize->SetRealNumber (POINT_SIZE_AND_LEADING);

		// Next step is to create a command to apply the attribute.
		Utils<ITextAttrUtils>()->BuildApplyTextAttrCmd (
				textModel, position, length, textAttrPointSize, kCharAttrStrandBoss);

		// Then add that to a list of text attributes -- probably dumb, but 
		// that's the only way I've gotten it to work.
		K2::shared_ptr<AttributeBossList> charAttributeBossList (
															new AttributeBossList ());
		if (!charAttributeBossList) throw "No charAttributeBossList";
		charAttributeBossList->ApplyAttribute (textAttrPointSize);
		
		// Create a container for the leading attribute.
		InterfacePtr<ITextAttrRealNumber> textAttrLeading (
							CreateObject2<ITextAttrRealNumber> (kTextAttrLeadBoss));
		if (!textAttrLeading) throw "NO textAttrLeading";
		textAttrLeading->SetRealNumber (POINT_SIZE_AND_LEADING);
		
		// Next step is to create a command to apply the attribute.
		Utils<ITextAttrUtils>()->BuildApplyTextAttrCmd (
				textModel, position, length, textAttrLeading, kCharAttrStrandBoss);

		// Add the leading to the attribute list.
		charAttributeBossList->ApplyAttribute (textAttrLeading);
/*
		InterfacePtr<IAttrReport> textAttrUnderlineMode(SnpTextAttrHelper::CreateTextAttribute<ITextAttrUnderlineMode, ITextAttrUnderlineMode::ValueType>(kTextAttrUnderlineBoss, IDrawingStyle::kUnderlineSingle));
		ASSERT(textAttrUnderlineMode);
		if (!textAttrUnderlineMode) {
			break;
		}
		charAttributeBossList->ApplyAttribute(textAttrUnderlineMode);
		}
*/
		//--------------------------------------------------------------------------
		// Apply the text formatting commands.
		//--------------------------------------------------------------------------
		// Use SnpTextModelHelper::ApplyOverrides to apply the attribute. We are 
		// applying a paragraph attribute, so we indicate kCharAttrStrandBoss.
		status = textCmdHelper.ApplyOverrides (textModel, position, length-1, 
										charAttributeBossList, kCharAttrStrandBoss);
		if (status == kFailure) throw "Could not format the text";
	} while (false);

	return status;
}


void Basic_Display_Ad::DebugAlert_(const char* inStr)
{
#if WANT_DEBUG_ALERTS
	PMString pmstr (inStr);
	CAlert::InformationAlert (pmstr);
#endif
}

void Basic_Display_Ad::Log_()
{
#if VERSION == IN_HOUSE
	LOG ("<BasicDisplayAdObject>");

	string ds;
	const char *cstr;
	
	ds = "\t<member name=\"inputRecord_\">";
	ds += inputRecord_;
	ds += "</member>";
	cstr = ds.c_str ();
	LOG (cstr);
	ds.resize (0);

	char logStr[256];

	sprintf (logStr, "\t<member name=\"cols_\">%d</member>", cols_);
	LOG (logStr);

	sprintf (logStr, "\t<member name=\"height_\">%10.3f</member>", 
					ToFloat (height_) );
	LOG (logStr);

	ds = "\t<member name=\"customer_\">";
	ds += customer_;
	ds += "</member>";
	cstr = ds.c_str ();
	LOG (cstr);
	ds.resize (0);

	ds = "\t<member name=\"fileName_\">";
	ds += fileName_;
	ds += "</member>";
	cstr = ds.c_str ();
	LOG (cstr);
	ds.resize (0);

	ds = "\t<member name=\"salesRepName_\">";
	ds += salesRepName_;
	ds += "</member>";
	cstr = ds.c_str ();
	LOG (cstr);
	ds.resize (0);

	sprintf (logStr, "\t<member name=\"absolutePage_\">%d"
				"</member>", absolutePage_);
	LOG (logStr);

	sprintf (logStr, "\t<member name=\"lowerPage_\">%d"
				"</member>", lowerPage_);
	LOG (logStr);

	sprintf (logStr, "\t<member name=\"upperPage_\">%d"
				"</member>", upperPage_);
	LOG (logStr);

	sprintf (logStr, "\t<member name=\"oddPageFlag_\">%s</member>", 
				oddPageFlag_ == true ? "TRUE" : "FALSE");
	LOG (logStr);

	sprintf (logStr, "\t<member name=\"cameraReadyFlag_\">%s</member>", 
	cameraReadyFlag_ == true ? "TRUE" : "FALSE");
	LOG (logStr);

	ds = "\t<member name=\"color_\">";
	ds += color_;
	ds += "</member>";
	cstr = ds.c_str ();
	LOG (cstr);
	ds.resize (0);

	ds = "\t<member name=\"notes_\">";
	ds += notes_;
	ds += "</member>";
	cstr = ds.c_str ();
	LOG (cstr);
	ds.resize (0);

	ds = "\t<member name=\"section\">";
	ds += section_;
	ds += "</member>";
	cstr = ds.c_str ();
	LOG (cstr);
	ds.resize (0);

	sprintf (logStr, "\t<member name=\"category_\">%d</member>", 
				category_);
	LOG (logStr);
	
	sprintf (logStr, "\t<member name=\"width_\">%10.3f</member>", 
				ToFloat (width_) );
	LOG (logStr);
///////////////////////////////////////////////////////////////////////////////
#if 0
	sprintf (logStr, "\tpageNum_ = %d", pageNum_);
	LOG (logStr);

	sprintf (logStr, "\tposition_ = %d", position_);
	LOG (logStr);

	sprintf (logStr, "\tplaced_ = %s", placed_ == true ? "TRUE" : "FALSE");
	LOG (logStr);

	sprintf (logStr, "\tboxx_ = %10.3f", ToFloat (boxx_) );
	LOG (logStr);

	sprintf (logStr, "\tboxy_ = %10.3f", ToFloat (boxy_) );
	LOG (logStr);
#endif
///////////////////////////////////////////////////////////////////////////////

	LOG ("</BasicDisplayAdObject>");
#endif
}

// END OF FILE
