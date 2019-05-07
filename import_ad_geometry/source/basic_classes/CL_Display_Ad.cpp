/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	CL_Display_Ad.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date:
	da	ca. 02-September-2002 10:07 PM
	
	ab	Abstract:
	ab	Implementation of CL_Display_Ad class.
	
	cl	Change Log:
	cl	30-Nov-10	Began port to CS4.
	cl	10-Sep-05	Began port to InDesign.
	cl	19-Feb-05	Began port to QXP 6.x.
	cl	13-Jun-03	Revamped the class to allow variable length strings in 
	cl				data members.
	cl	02-Sep-02	New.
------------------------------------------------------------------------------*/

using namespace std;

#include <cstring>
#include <string>

	#include "CL_NewsCadRect_ID.h"
	#include "PMString.h"
//	#include "ILayoutUtils.h"
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
// TBD:  FIND THIS FILE	#include "WFPID.h"
//#include "CL_NewsCAD_XT.h"
// TBD:  FIND THIS FILE #include "NewsCAD_Prefs.h"
//#include "CL_Document.h"
#include "CL_Display_Ad.h"
#include "CL_Page.h"
#include "Basic_Page.h"
#include "CL_Log_File.h"
#include "global_data.h"


// Macro for logging information in debug file.
#if VERSION==IN_HOUSE
	#define	LOG_PASTEUP		LogPasteup_()
#else
	#define	LOG_PASTEUP		NULL
#endif

//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and D E S T R U C T I O N
//----------------------------------------------------------------------------------
CL_Display_Ad::CL_Display_Ad ()
{
	Clear_();
}


//----------------------------------------------------------------------------------
// Create an ad object from an STL string (possibly from a geometry file record 
// or a string extracted from the document slug).
//----------------------------------------------------------------------------------
CL_Display_Ad::CL_Display_Ad (string & inputRec)	// Create from ASCII
	: Basic_Display_Ad (inputRec)
{
	LOG_BEGIN_FUNCTION;
	pageNum_ = 0;
	position_ = 0;
	placed_ = false;
	ComputeAdWidth_();
	LOG_END_FUNCTION;
}


CL_Display_Ad::CL_Display_Ad (const CL_Display_Ad & inObj)				// Copy
{
	CopyAllMembers_(inObj);
}


CL_Display_Ad & CL_Display_Ad::operator= (const CL_Display_Ad & inObj)	// Assign
{
	if (this != &inObj)									// Beware of "a = a"
	{
		CopyAllMembers_(inObj);
	}

	return *this;
}


CL_Display_Ad::~CL_Display_Ad ()
{

}


//----------------------------------------------------------------------------------
//	Clear_()
//	Make the object "tabula rasa."
//----------------------------------------------------------------------------------
void CL_Display_Ad::Clear_()
{
	Basic_Display_Ad::Clear_();
	pageNum_ = 0;
	position_ = 0;
	placed_ = false;
}


//----------------------------------------------------------------------------------
//	CopyAllMembers_
//
//	This method is called by the copy ctor and the assignment operator to set all 
//	data members.  Note my assumption that the STL takes care of deallocating the 
//	data in this's string objects when they get assigned.
//----------------------------------------------------------------------------------
void CL_Display_Ad::CopyAllMembers_(const CL_Display_Ad & inObj)
{
	Basic_Display_Ad::CopyAllMembers_(Basic_Display_Ad (inObj));
/*	inputRecord_ = inObj.inputRecord_;
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
*/
	pageNum_ = inObj.pageNum_;
	position_ = inObj.position_;
	placed_ = inObj.placed_;
}


//----------------------------------------------------------------------------------
// ComputeAdWidth_()
//
// Figure ad width in points from the number of columns wide and the 
// current page's column and gutter widths.
//----------------------------------------------------------------------------------
void CL_Display_Ad::ComputeAdWidth_()
{
	LOG_BEGIN_FUNCTION;
	auto_ptr<Basic_Page> curPage ( new Basic_Page );
	curPage->ComputeCurrentPageUID_();						// Also sets UIDRef
	curPage->ComputeAndStorePageMeasurements_();

	// We now know enough about the page to convert our ad's 
	// width in columns to a width expressed in points.  This will 
	// be stored in the width_ member.  Call the inherited method 
	// to accomplish this:
	ComputeWidth_(curPage->ColumnWidth_(), curPage->GutterWidth_());
	char logStr[256];
	sprintf (logStr,
			"Column width = %f  Gutter width = %f", 
			ToFloat (curPage->ColumnWidth_()), 
			ToFloat (curPage->GutterWidth_()) );
	LOG (logStr);
	LOG_END_FUNCTION;
}


void CL_Display_Ad::LogInputRec_()
{
#if VERSION == IN_HOUSE
	string ds = "<AdInput>[";
	ds += inputRecord_;
	ds += "]";
	const char *cstr = ds.c_str ();
	LOG (cstr);
	LOG ("</AdInput>");
#endif
}


void CL_Display_Ad::LogPasteup_()
{
#if VERSION == IN_HOUSE
	LOG_BEGIN_FUNCTION;

	string ds;
	const char *cstr;
	char logStr[256];
 
	ds = "<results fileName_=\"";
	ds += fileName_;
	ds += "\"  ";
	sprintf (logStr, "boxX_=\"%8.3f\"  ", ToFloat (boxX_) );
	ds += logStr;

	sprintf (logStr, "boxY_=\"%8.3f\"  ", ToFloat (boxY_) );
	ds += logStr;
	
	sprintf (logStr, "width_=\"%8.3f\"  ", ToFloat (width_) );
	ds += logStr;

	sprintf (logStr, "height_ =\"%8.3f\"  ", ToFloat (height_) );
	ds += logStr;

	sprintf (logStr, "category_=\"%d\"  ", category_);
	ds += logStr;

	sprintf (logStr, "pageNum_=\"%d\"", pageNum_);
	ds += logStr;

	ds += "/>";
	
	cstr = ds.c_str ();
	LOG (cstr);

	LOG_END_FUNCTION;
#endif
}


void CL_Display_Ad::Log_()
{
#if VERSION == IN_HOUSE
	LOG ("<CLDisplayAdObject>");

	Basic_Display_Ad::Log_();

	string ds;
	const char *cstr;
	char logStr[256];

	sprintf (logStr, "\t<member name=\"pageNum_\">%d</member>", pageNum_);
	LOG (logStr);

	sprintf (logStr, "\t<member name=\"position_\">%d</member>", position_);
	LOG (logStr);

	sprintf (logStr, "\t<member name=\"placed_\">%s</member>", 
				placed_ == true ? "TRUE" : "FALSE");
	LOG (logStr);
	
	LOG ("</CLDisplayAdObject>");
#endif
}

void CL_Display_Ad::LogNameAndPage_()
{
#if VERSION == IN_HOUSE
	LOG ("Place on absolute page:");
	char logStr[256];
	::strcpy (logStr, "\tfileName_ = ");
	::strcat ( logStr, fileName_.c_str() );
	LOG (logStr);
	::sprintf (logStr, "\tabsolutePage_ = %d", absolutePage_);
	LOG (logStr);
#endif
}

void CL_Display_Ad::LogNameAndPageLimits_()
{
#if VERSION == IN_HOUSE
	char logStr[256];
	::sprintf (logStr, "Place ad [%s] between pages: %d and %d", 
				fileName_.c_str(), lowerPage_, upperPage_);
	LOG (logStr);
#endif
}

void CL_Display_Ad::GetQuarkRect_(CL_NewsCadRect* inRect)
{
	inRect->Left (XPosition());
	inRect->Top (YPosition());
	inRect->Right (XPosition() + Width_());
	inRect->Bottom (YPosition() + Height_());
}

/*================================================================================*/
void CL_Display_Ad::PasteUp_(CL_Page* inPageObjPtr) throw (OSStatus) {
/*----------------------------------------------------------------------------------
	Abstract:
		Create a picture and text box on the page to represent this ad.
		This overloads the function in the base class.
		
	Parameters and modes:

	Returns:
		
	Change Log:
		28-Jan-11	Began port to CS4.
		07-Nov-05	Began port to InDesign.
		07-Jan-04	Output notes field for Reminder.
		07-Oct-02	Began rewrite based on the old 68K code.
----------------------------------------------------------------------------------*/
#if CLIENT == HEMMINGS
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

	LOG_BEGIN_FUNCTION;

	int16 adPageNumber = inPageObjPtr->GetFolio_();
	SetPageNumber_(adPageNumber);
	LOG_PASTEUP;
	SetPointerToParentPage_(inPageObjPtr);
	CreatePictureAndTextBoxes_ID_();
	SetPlaced_(true);

	LOG_END_FUNCTION;
}


UIDRef CL_Display_Ad::CreateBox_ID_(ContentType inContentType)
{
	UIDRef retval = UIDRef::gNull;
//	inContentType;
	
	return retval;
}


UIDRef CL_Display_Ad::CreatePictureAndTextBoxes_ID_()
{
#if CLIENT == MOUNTAIN_ENTERPRISE
	extern ErrorCode SetRunaround_(const UIDRef& graphicFrameUIDRef, 
						const PMReal &topMargin, 
						const PMReal &leftMargin, 
						const PMReal &bottomMargin, 
						const PMReal &rightMargin,
						IStandOff::mode mode = IStandOff::kBothSides);
#endif
	extern ErrorCode SetRunaround_(const UIDRef& graphicFrameUIDRef, 
						const PMReal inAmount = 0, 
						IStandOff::mode mode = IStandOff::kBothSides);
	extern ErrorCode SetIgnoreRunarounds_(
						const UIDRef& inTextFrameUIDRef, bool inIgnore);
						
	UIDRef result = UIDRef::gNull;

	// Get layout control data for current document window
	ILayoutControlData* pILayoutControlData = 
		Utils<ILayoutUIUtils>()->QueryFrontLayoutData ();
	ASSERT (pILayoutControlData);
	if (pILayoutControlData)
	{
		// Parent the frame on the active spread layer.
		InterfacePtr<IHierarchy> activeSpreadLayerHierarchy (
									pILayoutControlData->QueryActiveLayer());
		ASSERT (activeSpreadLayerHierarchy);
		if (activeSpreadLayerHierarchy)
		{
			UIDRef parentUIDRef = ::GetUIDRef (activeSpreadLayerHierarchy);

			// Transform the bounds of the frame from page co-ordinates
			// into the parent co-ordinates, i.e. the spread.
			UID pageUID = pILayoutControlData->GetPage ();
			ASSERT (pageUID != kInvalidUID);
			if (pageUID != kInvalidUID)
			{
				SDKLayoutHelper layoutHelper;
				UIDRef pageUIDRef (parentUIDRef.GetDataBase (), pageUID);
				
				// For whatever reason, those fine folks at Adobe have decided that 
				// 0,0 is the top left of the page column (live) area rather than 
				// the trim.  This despite the fact that their palette shows item 
				// positions displaced from the trim!
				PMRect boundsInPageCoords (
					XPosition () - pMyPage_->InsideMargin_(),
					YPosition () - pMyPage_->Top_(),
					XPosition () - pMyPage_->InsideMargin_() + width_,
					YPosition () - pMyPage_->Top_() + height_);
				gpLogFile->LogPMRect_("boundsInPageCoords", boundsInPageCoords);

				PMRect boundsInParentCoords = layoutHelper.PageToSpread (
													pageUIDRef, boundsInPageCoords);
				gpLogFile->LogPMRect_("boundsInParentCoords", boundsInParentCoords);

				// Create the frame.
				result = layoutHelper.CreateRectangleFrame (
												parentUIDRef, boundsInParentCoords);
				picBoxUidRef_ = result;
#if CLIENT == MOUNTAIN_ENTERPRISE
				// Customized behavior:
				ErrorCode e = SetRunaround_(result, 
											2.16, 5.04, 2.16, 5.04);
#else
				// Typical behavior:
				ErrorCode e = SetRunaround_(result, pMyPage_->GutterWidth_());
#endif
				// Add black box (slug) data.
				StoreInBoxSlug_(picBoxUidRef_);
				
				// Create the text box -- 1 column, horizontal text direction.
				UIDRef storyUIDRef;
				result = layoutHelper.CreateTextFrame (
											parentUIDRef, boundsInParentCoords, 1, 
											kFalse, &storyUIDRef);
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
				UID swatchUID = graphicsHelper.GetNamedSwatch (PMString("Paper"), 
														txtBoxUidLst.GetDataBase());
				graphicsHelper.AddFillRendering (swatchUID);
				graphicsHelper.ApplyAttributes ();
/*
				// Make the frame 4 pt weight.
				graphicsHelper.AddStrokeWeight (4.0);
				UID swatchUID = graphicsHelper.GetNamedSwatch (PMString("Black"), 
														txtBoxUidLst.GetDataBase());
				graphicsHelper.AddStrokeRendering (swatchUID);
				graphicsHelper.ApplyAttributes ();
*/
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
						LOG ("CANNOT CENTER CONTENT IN FRAME");
					}
				}
				else
				{
					LOG ("CANNOT EVEN CREATE A COMMAND TO CENTER CONTENT IN FRAME");
				}
				
				// Group the text and pic frames.
				InterfacePtr<ICommand> groupItemsCmd (
							CmdUtils::CreateCommand (kGroupCmdBoss));
				if (groupItemsCmd)
				{
/*
					InterfacePtr<IIntData> groupItemsValue (groupItemsCmd, 
														UseDefaultIID());
					groupItemsValue->Set (2);*/
/*					UIDList* itemsToGroup = new UIDList;
					itemsToGroup->Insert ( textBoxUidRef_.GetUID () );
					itemsToGroup->Insert ( picBoxUidRef_.GetUID () );
*/
					UIDList* itemsToGroup = new UIDList(textBoxUidRef_);
					itemsToGroup->Append ( picBoxUidRef_.GetUID () );
					InterfacePtr<IGroupCmdData> groupCmdData (groupItemsCmd, 
															UseDefaultIID());
					groupCmdData->Set (itemsToGroup);
					if (CmdUtils::ProcessCommand (groupItemsCmd) != kSuccess)
					{
						LOG ("CANNOT GROUP THE ITEMS");
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
									LOG ("TEXT FRAME PARENT IS NOT A GROUP");
								}
							}
							else
							{
								LOG ("QueryParent() RETURNED NULL FOR TEXT BOX IHierarchy");
							}
						}
						else 
						{
							LOG ("hier IS NULL FOR TEXT FRAME");
						}
					}
				}
			}
		}
	}

	return result;
}


void CL_Display_Ad::FormatForAdReport_(std::string & adRec)
{
	char tmp[256];
	uchar utmp[256];
	PMString pmstr;
	const int32 kDigitsPrecision = 2;
	const bool16 kRound = true;
	const bool16 kSuppressTrailingZeroes = true;
		
	pmstr.Append ('\t');							// Tab before any data
	pmstr.AppendNumber (GetPageNumber_());			// Page number
	pmstr.Append ('\t');
	pmstr.AppendNumber (XPosition (), kDigitsPrecision, 
				kRound, kSuppressTrailingZeroes);	// X-position
	pmstr.Append ('\t');
	pmstr.AppendNumber (YPosition (), kDigitsPrecision, 
				kRound, kSuppressTrailingZeroes);	// Y-position
	pmstr.Append ('\t');
	pmstr.Append (customer_.c_str ());				// Customer name
	pmstr.Append ('\t');
	pmstr.Append (fileName_.c_str ());				// Art file name
	pmstr.Append ('\t');
	pmstr.Append (salesRepName_.c_str ());			// Salesperson
	pmstr.Append ('\r');							// Mark end of record
	
	// Set output value for caller.
	adRec = pmstr.GrabCString ();
}



// END OF FILE

#pragma mark // END OF FILE
#pragma mark -

#if 0
void ReadAd (int theVol, Ads *theAd, long thePos) 

// this ad reads an ad from the specified volume
// and file (thePos) into theAd.
{
	int				fcount;
	long			fOffset;
	long			fRead;
	long			vTemp;
	long			fPos;
	Fixed			fTemp;
	docsysrec		theDoc;
	Fixed			colWidth;
//?	long			lcolWidth;
	long			theTempVal;
	int				Counter;
	unsigned char	tempStr[256];
	
	getdocinfo(&theDoc);
	colWidth=theDoc.pagewidth-theDoc.marleft-theDoc.marright;
	for (Counter=1;Counter<theDoc.colcount;Counter++)
		colWidth-=theDoc.gutter;
	colWidth = FixDiv (colWidth, FIX (theDoc.colcount));
		
	fPos=thePos;
	theAd->Position=12;
	theAd->Skip[0] = 1;			/* BUG FIX */
	theAd->Skip[1] = 'N';

	for(fcount=1; fcount <=  advars; fcount++)
	{
		SetFPos(theVol,1,fPos);
		fPos+=adVarLen[fcount];
		fRead=adVarLen[fcount];
		FSRead(FileVol,&fRead,&tempStr[1]);
		tempStr[0]=fRead;
		StripTrailingSpaces(tempStr);
		switch (fcount)
		{		// this switch is used to provide flexibility in 
				// reading ads in any pre-determined order
				// which is specified through the varPos values
				// which are fairly self - descriptive
			case xPos:
			{
				/* The old NewsCAD code tried to figure the number of columns 
				** from the width of the ad in points that was supplied by the 
				** database.  The database now supplies the ad's column count.  
				** As part of the 11-Aug-99 revisions, we're replacing the old 
				** processing with the following block.  It computes the ad width 
				** in points from the column count.
				*/
				Fixed pageColumnWidth;
				int pageColumnCount;
				Fixed pageGutterWidth;
				Fixed ftemp;
				long count;
				
				GetPageWidths (&pageColumnWidth, &pageColumnCount, &pageGutterWidth);
				
				/* Get number of columns spanned by this ad, store in ad struct */
				pstrcpy (tmpstr, tempStr);
				StringToNum (tempStr, &theTempVal);
				theAd->cols = count = theTempVal;
				
				/* Compute total column width spanned */
				ftemp = FixMul (FIX (count), pageColumnWidth);
				
				/* Compute number of gutters spanned; accumulate gutter width */
				--count;
				if (count)
					ftemp += FixMul (FIX (count), pageGutterWidth);
				theAd->Width = ftemp;
				break;
			}
			case yPos:
				pstrcpy(tmpstr,tempStr);
				str2val(NUMBER,0,&fTemp);
				theAd->Height=fTemp;
				break;
				
			case clientPos:
				pstrcpy(theAd->client,tempStr);
				break;
				
			case filePos:
				pstrcpy(theAd->name,tempStr);
				break;
				
			case initPos:
				pstrcpy(theAd->initials,tempStr);
				break;
				
			case bPos:
				pstrcpy(tmpstr,tempStr);
				str2val(NUMBER,0,&fTemp);
				theAd->border=fTemp;
				break;
				
			case pagePos: 
				StringToNum(tempStr,&vTemp);
				theAd->PageNo=(int)vTemp;
				break;
								 
			case lowPos:
				StringToNum(tempStr,&vTemp);
				theAd->LowerPage=(int)vTemp;
				break;
				
			case UpPos:
				StringToNum(tempStr,&vTemp);
				theAd->UpperPage=(int)vTemp;
				break;
				
			case OddPos:
				StringToNum(tempStr,&vTemp);
				theAd->OddPage=(int)vTemp;
				break;
				
			case LocalPos:
				StringToNum(tempStr,&vTemp);
				theAd->Local=(int)vTemp;
				break;
				
			case colorPos:
				pstrcpy(theAd->Color,tempStr);
				break;			
		}
		
	}
	
}

//----------------------------------------------------------------------------------
// ProcessField_()
// Store a field value in the ad record into a member variable.  Convert text 
// to binary representation as needed.
// Code is adapted from old NewsCAD's ReadInputFile.c

// Macros for field numbers lifted from old NewsCAD.h
#define		xPos		1
#define		yPos		2
#define		clientPos	3
#define 	filePos		4
#define		initPos		5
#define		bPos		6
#define		pagePos		7
#define		lowPos		8
#define		UpPos		9
#define 	OddPos 		10
#define		LocalPos	11
#define		colorPos	12

void CL_Display_Ad::ProcessField_(int inFieldNumber, string& inString)
{
	CL_Page pageObj;
	Fixed fTemp;
	
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
	
	const char *pInStr = inString.c_str ();
	uchar val[256];
	::strcpy ( (char*) val, pInStr );
	
	switch (inFieldNumber) {
	case xPos:						// Ad width in columns
		// Init some stuff (it's old NewsCAD stuff, I'm not sure yet 
		// how it's used
		position_ = 12;
		skip_[0] = 1;
		skip_[1] = 'N';

		// Now convert "columns wide" into points wide
		pageObj.StorePageInfo_();
		Fixed pageColumnWidth = pageObj.GetColumnWidth_();
		int pageColumnCount = pageObj.GetColumnCount_();
		Fixed pageGutterWidth = pageObj.GetGutterWidth_();
		long cols;

		// Get number of columns spanned by this ad, store in member var
		cols_ = cols = ::atol (pInStr);
		
		// Compute total column width spanned
		fTemp = FixMul (FIX (cols), pageColumnWidth);
		
		// Compute number of gutters spanned; accumulate gutter width
		--cols;
		if (cols)
			fTemp += FixMul (FIX (cols), pageGutterWidth);
		width_ = fTemp;
		break;

	case yPos:						// Ad height in points
		CtoPstr ( (char*) val );
		str2valbuf (NUMBER, 0, &fTemp, val);
		height_ = fTemp;
		break;
		
	case clientPos:
		CtoPstr ( (char*) val );
		STRCPY (client_, val);
		break;

	case filePos:
		CtoPstr ( (char*) val );
		STRCPY (name_, val);
		break;

	case initPos:
		CtoPstr ( (char*) val );
		STRCPY (initials_, val);
		break;

	case bPos:
		CtoPstr ( (char*) val );
		str2valbuf (NUMBER, 0, &fTemp, val);
		border_ = fTemp;
		break;

	case pagePos:
		pageNum_ = ::atol (pInStr);
		break;

	case lowPos:
		lowerPage_ = ::atol (pInStr);
		break;

	case UpPos:
		upperPage_ = ::atol (pInStr);
		break;

	case OddPos:
		oddPage_ = ::atol (pInStr);
		break;

	case LocalPos:
		local_ = ::atol (pInStr);
		break;

	case colorPos:
		CtoPstr ( (char*) val );
		STRCPY (color_, val);
		break;
	}
}

void CL_Display_Ad::Log_()
{
#if VERSION == IN_HOUSE
	LOG ("*** Ad Object:");

	uchar tmp[256];
	char logStr[256];

	::strcpy (logStr, "\tname_ = ");
	ptocstrcpy (tmp, name_);
	::strcat (logStr, (char*) tmp);
	LOG (logStr);
	
	::strcpy (logStr, "\tclient_ = ");
	ptocstrcpy (tmp, client_);
	::strcat (logStr, (char*) tmp);
	LOG (logStr);
	
	::strcpy (logStr, "\tinitials_ = ");
	ptocstrcpy (tmp, initials_);
	::strcat (logStr, (char*) tmp);
	LOG (logStr);

	sprintf (logStr, "\tboxx_ = %10.3f", float (float (boxx_) / 65536.0) );
	LOG (logStr);

	sprintf (logStr, "\tboxy_ = %10.3f", float (float (boxy_) / 65536.0) );
	LOG (logStr);

	sprintf (logStr, "\theight_ = %10.3f", float (float (height_) / 65536.0) );
	LOG (logStr);

	sprintf (logStr, "\twidth_ = %10.3f", float (float (width_) / 65536.0) );
	LOG (logStr);

	sprintf (logStr, "\tpriority_ = %d", priority_);
	LOG (logStr);

	sprintf (logStr, "\tborder_ = %d", border_);
	LOG (logStr);

	sprintf (logStr, "\tpageNum_ = %d", pageNum_);
	LOG (logStr);

	sprintf (logStr, "\tposition_ = %d", position_);
	LOG (logStr);

	sprintf (logStr, "\tlowerPage_ = %d", lowerPage_);
	LOG (logStr);

	sprintf (logStr, "\tupperPage_ = %d", upperPage_);
	LOG (logStr);

	sprintf (logStr, "\toddPage_ = %d", oddPage_);
	LOG (logStr);

	sprintf (logStr, "\tlocal_ = %d", local_);
	LOG (logStr);

	::strcpy (logStr, "\tcolor_ = ");
	ptocstrcpy (tmp, color_);
	::strcat (logStr, (char*) tmp);
	LOG (logStr);

	sprintf (logStr, "\tcols_ = %d", cols_);
	LOG (logStr);

	::strcpy (logStr, "\tskip_ = ");
	ptocstrcpy (tmp, skip_);
	::strcat (logStr, (char*) tmp);
	LOG (logStr);
#endif
}

#endif

///////////////////////////////////////////////////////////////////////////////
#if 0
HANDLED BY BASE CLASS

//----------------------------------------------------------------------------------
//	Choose your level of logging:  0 for none, 1 for function entry and exit, 2 
//	for value returned by InDesign's ProcessCommand(), 3 for really detailed.
#define		StoreInBoxSlug_LogLevel		3
//----------------------------------------------------------------------------------
void CL_Display_Ad::StoreInBoxSlug_(UIDRef & inUIDRef)
{
#if StoreInBoxSlug_LogLevel
	LOG_BEGIN_FUNCTION;
#endif
#if StoreInBoxSlug_LogLevel > 2
	LOG ("TRY TO ADD SLUG DATA");
#endif

	// Create the command that sets the IBPIData.
	InterfacePtr<ICommand> blackboxSetDataCmd (
		CmdUtils::CreateCommand (kBlackBoxAddDataCmdBoss));
	ASSERT (blackboxSetDataCmd);

	if (blackboxSetDataCmd)
	{
#if StoreInBoxSlug_LogLevel > 2
		LOG ("CREATED COMMAND");
#endif
		// Pass the attribute value to be applied.
		InterfacePtr<IBlackBoxCmdData> blackboxData (blackboxSetDataCmd, 
														UseDefaultIID());
		ASSERT (blackboxData);
		if (blackboxData)
		{
			// Set the value of the data.
			blackboxData->Set (inUIDRef, kWFPPluginID, 
								(void*)inputRecord_.c_str (), 
								inputRecord_.length ());
#if StoreInBoxSlug_LogLevel > 2
			LOG ("VALUE FOR DATA SET SUCCESSFULLY");
#endif
			// Process the command to set the attribute for these objects.
			blackboxSetDataCmd->SetItemList ( UIDList (inUIDRef) );
			ErrorCode e = CmdUtils::ProcessCommand (blackboxSetDataCmd);
#if StoreInBoxSlug_LogLevel > 1
			DECLARE_logstr;
			::sprintf (logstr, "ProcessCommand() returned %d for graphic frame", e);
			LOG (logstr);
#endif
#if StoreInBoxSlug_LogLevel > 2
			// This is just "double check" code to see if the black 
			// box data really worked.
			InterfacePtr<IBlackBoxData> bbd (inUIDRef, UseDefaultIID());
			if (bbd)
			{
				::sprintf (logstr, "Data Length = %d", 
									bbd->GetDataLength (kWFPPluginID));
				LOG (logstr);
				bbd->ReadData (kWFPPluginID, (void*)logstr, 32);
				logstr[32] = '\0';
				LOG (logstr);
			}
			else
			{
				LOG ("LOOKS LIKE BLACK BOX DATA WASN'T REALLY ADDED.  NUTS.");
			}
#endif
	 	}
		else
		{
#if StoreInBoxSlug_LogLevel > 2
			LOG ("COULD NOT SET THE COMMAND DATA");
#endif
		}
	}
	else
	{
#if StoreInBoxSlug_LogLevel > 2
		LOG ("COULD NOT CREATE COMMAND");
#endif
	}
#if StoreInBoxSlug_LogLevel
	LOG_END_FUNCTION;
#endif
}
#undef StoreInBoxSlug_LogLevel


void CL_Display_Ad::InsertAdDescriptiveText_(UIDRef inStoryUidRef)
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
				/*ErrorCode*/ApplyTextAttributeOverrides_(textModel, 
									0, textModel->TotalLength ());
			}
		}
	}
}

void CL_Display_Ad::ComputeAdDescriptionText_(string & ioDescText)
{
		ioDescText = customer_;
		ioDescText += '\r';
		ioDescText += fileName_;
		ioDescText += '\r';
		ioDescText += color_;
#if CLIENT == HEMMINGS || CLIENT == THRIFTY_NICKEL
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
#if CLIENT == HEMMINGS || CLIENT == THRIFTY_NICKEL || CLIENT == REMINDER
		if (notes_.length ())
		{
			ioDescText += '\r';
			ioDescText += notes_;
		}
#endif
}


ErrorCode CL_Display_Ad::ApplyTextAttributeOverrides_(ITextModel* textModel, 
									const TextIndex position, const int32 length)
{
	LOG_BEGIN_FUNCTION;
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
		if (!charAttributeBossList) LOG ("No charAttributeBossList");
		charAttributeBossList->ApplyAttribute (textAttrPointSize);
		
		// Create a container for the leading attribute.
		InterfacePtr<ITextAttrRealNumber> textAttrLeading (
							CreateObject2<ITextAttrRealNumber> (kTextAttrLeadBoss));
		if (!textAttrLeading) LOG ("NO textAttrLeading");
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
		if (status == kFailure) LOG ("Could not format the text");
	} while (false);

	LOG_END_FUNCTION;
	return status;
}
#endif
///////////////////////////////////////////////////////////////////////////////
