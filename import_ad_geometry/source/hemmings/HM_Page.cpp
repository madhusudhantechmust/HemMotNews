/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	HM_Page.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	6/18/03 ca. 12:19 AM
	
	ab	Abstract:
	ab	Implementation of HM_Page class.  This class extends the basic page 
	ab	class CL_Page for functionality required by Hemmings Motor News.
	
	cl	Change Log:
	cl	1/25/11	Began port to InDesign CS4.
	cl	6/18/03	New.
------------------------------------------------------------------------------*/
#include "IDocument.h"
#include "IPageList.h"
#include "IColumns.h"
#include "IMargins.h"
#include "ILayoutUIUtils.h"
#include "ILayoutCmdData.h"
#include "IUIDData.h"
#include "ILayoutUIUtils.h"		// debugging only
#include "IGraphicsContext.h"
#include "SnpTextModelHelper.h"
#include "IParcel.h"
#include "CAlert.h"

#include "CL_Page.h"
#include "CL_Log_File.h"
#include "HM_Geometry_Importer.h"
#include "CL_NewsCadRect_ID.h"
#include "HM_Page.h"
#include "CL_Display_Ad.h"
#include "text_frame_options.h"

#include "global_data.h"

using namespace std;

// Fixed VerifyBoxDepth (boxid bxid, Fixed leadingIncrement);

//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and  D E S T R U C T I O N
//----------------------------------------------------------------------------------
HM_Page::HM_Page ()
{
	LOG ("<ctor class=\"HM_Page\"/>");
//	Initialize_();
}


HM_Page::HM_Page (HM_Geometry_Importer* inImporter)
{
	LOG ("<ctor class=\"HM_Page\" variant=\"CL_Geometry_Importer\"/>");
//	Initialize_();
	pHMGeometryImporter_ = inImporter;
	pGeometryImporter = inImporter;
}

HM_Page::HM_Page (HM_Geometry_Importer* inImporter, IPageList* inPageList, 
					UID inUid, int32 inPgIdx, int16 inFolio)
	:	pHMGeometryImporter_(inImporter)
{
	LOG ("<ctor class=\"HM_Page\" variant=\"CL_Geometry_Importer, UID, int32, int16\"/>");
	uid_ = inUid;
	pageIndex_ = inPgIdx;
	iPageList_ = inPageList;
	folio_ = inFolio;
	pGeometryImporter = inImporter;
}


// Call this variant to construct an HM_Page for placing a continued line.
HM_Page::HM_Page (IPageList* inPageList, UID inUid)
{
	LOG ("<ctor class=\"HM_Page\" variant=\"UID, UIDList\"/>");
	uid_ = inUid;
	iPageList_ = inPageList;
}


HM_Page::~HM_Page ()												// Destroy
{
	LOG ("<dtor class=\"HM_Page\"/>");
}


void HM_Page::SetupForAdPlacement_()
{
	LOG_BEGIN_FUNCTION;

	// Make sure this object has been constructed correctly, or initialized.
	ASSERT (pHMGeometryImporter_);
	ASSERT (uid_.Get());
	ASSERT (pageIndex_ >= 0);
	ASSERT (folio_);
	ASSERT (iPageList_);

	ComputePageGeometry_();

	LOG_END_FUNCTION;
}


/*================================================================================*/
void HM_Page::ComputePageGeometry_() {
/*----------------------------------------------------------------------------------
	Abstract:
		Gather data from InDesign about the page on which ads are to be placed.
		In NewsCAD 68K, this was the C function SetupPage().

	Parameters and modes:

	Modifies:
		columnCount_		gutterWidth_		columnWidth_		
		width_				height_				pgRect_
		left_				top_				right_				bottom_
		
	Returns:

	Change Log:
		28-Jan-11	Most of this function's processing was moved to the Basic_Page
					class during development of some of the other plugins.  Most 
					of what remains is code to do the logging, since the Basic_Page 
					class cannot do any logging.
		13-Sep-05	Began port to InDesign.
		20-Feb-05	Port to 6.x.
		08-Sep-02	Port from old NewsCAD.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;

	// Make sure this object has been constructed correctly, or initialized.
// 	ASSERT (pHMGeometryImporter_);
// 	ASSERT (uid_.Get());
// 	ASSERT (pageIndex_ >= 0);
// 	ASSERT (folio_);
// 	ASSERT (iPageList_);

	ASSERT (uid_.Get());
	ASSERT (iPageList_);

	DECLARE_logstr;
	::sprintf (logstr, "<p>Page %d  UID 0x%x</p>", folio_, uid_.Get());
	LOG (logstr);
	
	try {
		// Be sure uid_ and uidRef_ members have been set.  In this class, the uid_ has 
		// been set in the CTOR, but we have to ensure uidRef_ is ready, too.
		ComputePageUIDRef_();					// Basic_Page
		ComputeAndStorePageMeasurements_();		// Basic_Page
		
	// 	Get a pointer to the page's margin data, and extract what's interesting.
	// 	IMargins* iMargins = iPageList_->QueryPageMargins (uid_);
	// 	if (!iMargins) throw ("iMargins");
	// 	PMReal insideMargin, outsideMargin, topMargin, bottomMargin;
	// 	iMargins->GetMargins (&insideMargin, &topMargin, &outsideMargin, &bottomMargin);
	// 	
	// 	We'll need this for later when we place ads -- InDesign applies a transform 
	// 	to the same geometry to place things on left and right pages.
	// 	insideMargin_ = insideMargin;
	
// 		::sprintf (logstr, "<Margins left=\"%8.3f\" top=\"%8.3f\" right=\"%8.3f\" bottom=\"%8.3f\"/>",
// 				ToFloat (insideMargin), ToFloat (topMargin), 
// 				ToFloat (outsideMargin), ToFloat (bottomMargin));
// 		LOG (logstr);
		
		// Get a pointer to the page column data, and extract what's interesting.
	// 	IColumns* iColumns = iPageList_->QueryPageColumns (uid_);
	// 	if (!iColumns) throw ("iColumns");
	// 	columnCount_ = iColumns->GetNumberColumns ();	// Number of page cols
	// 	gutterWidth_ = iColumns->GetGutter ();			// Width of a single gutter
	// 	PMReal totalGutterWidth = (columnCount_ - 1) * 	// Measure consumed by gutters
	// 								gutterWidth_;
		
		// Get the left X and right X of each page column, compute the width of each 
		// one and accumulate the total.  InDesign expresses these values as 
		// DISPLACEMENTS from the page margins.  They are not absolute.
	// 	PMRealList outputColumns;
	// 	iColumns->GetColumns (&outputColumns);
	// 	PMRealList::iterator p = outputColumns.begin ();
	// 	PMReal x1, x2;
	// 	int i = 1;
	// 	PMReal totColWidth = 0;							// Measure consumed by columns
	// 	while ( p < outputColumns.end () )
	// 	{
	// 		x1 = *p;
	// 		++p;
	// 		x2 = *p;
	// 		totColWidth += x2 - x1;
	// 		++p;
	// 	}
	// 	
	// 	PMReal pmrColCount (columnCount_);
	// 	columnWidth_ = totColWidth / pmrColCount;		// Width of a single column
	/*
		width_ = totColWidth +  totalGutterWidth;		// Live area width
		height_ = pgRect_.bottom - pgRect_.top - 
					topMargin - bottomMargin;		// Live area height
	*/
		// Get the page rectangle.
	// 	IDocument* iDocument = ::GetFrontDocument ();			// LayoutUIUtils.h
	// 	IDataBase* database = ::GetDataBase (iDocument);
	// 	if (!database) throw ("database");
	// 	UIDRef page1UIDRef (database, uid_);
	// 	InterfacePtr<IGeometry> iGeometry (page1UIDRef, UseDefaultIID ());
	// 	if (!iGeometry) throw ("iGeometry");
	// 	PMRect tmpPMRect (insideMargin, topMargin, outsideMargin, bottomMargin);
	// 	iColumns->GetNthColumnBox (iGeometry, 0, tmpPMRect);
	// 	tmpPMRect = iGeometry->GetStrokeBoundingBox ();
		
		// Copy the page rect
	// 	pgRect_.Left (tmpPMRect.Left ());
	// 	pgRect_.Right (tmpPMRect.Right ());
	// 	pgRect_.Top (tmpPMRect.Top ());
	// 	pgRect_.Bottom (tmpPMRect.Bottom ());
		
		// Set page LIVE AREA height & width
	// 	height_ = tmpPMRect.Bottom () - tmpPMRect.Top () - topMargin - bottomMargin;
	// 	width_ = tmpPMRect.Right () - tmpPMRect.Left () - insideMargin - outsideMargin;
	
	/*
			bool doubleSided;							// Right margin x-position
			bool rightMasterPageApplied;
			XTError xtErr = pCurDoc->IsDoubleSided_(&doubleSided);
			if (doubleSided)
			{
	//			left_ = (rightmaster (folio_)) ? insideMargin : outsideMargin;
				pCurDoc->IsRightMaster_(folio_, &rightMasterPageApplied);
				left_ = rightMasterPageApplied ? insideMargin : outsideMargin;
				right_ = left_ + width_;
			}
			else
			{
				left_ = insideMargin;
				right_ = insideMargin + width_;
			}
	
			top_ = topMargin;							// Top margin y-position
			bottom_ = topMargin + height_;				// Bottom margin y-position
	*/
	/*
		virtual PageType 	GetPageType (UID pageUID)=0
	*/
		// Live area coordinates are dependent on whether page is on the left 
		// or right (or neither, in the case of single-sided documents).
		// enum PageType { kLeftPage = 0, kUnisexPage, kRightPage };
	// 	pageType_ = iPageList_->GetPageType (uid_);
	// 	left_ = pageType_ == kRightPage ? insideMargin : outsideMargin;
	// 	right_ = left_ + width_;
	// 	top_ = topMargin;							// Top margin y-position
	// 	bottom_ = topMargin + height_;				// Bottom margin y-position
		
		::sprintf (logstr, "<p>Stats columnCount_=\"%d\" gutterWidth_=\"%4.3f\""
							"columnWidth_=\"%4.3f\" width_=\"%4.3f\" height_=\"%4.3f\"</p>",
							columnCount_, ToFloat (gutterWidth_), 
							ToFloat (columnWidth_), ToFloat (width_), ToFloat (height_));
		LOG (logstr);
		::sprintf (logstr, "<p>PageRect x1=\"%4.3f\" y1=\"%4.3f\" x2=\"%4.3f\" y2=\"%4.3f\"</p>", 
							ToFloat (pgRect_.Left ()), ToFloat (pgRect_.Top()), 
							ToFloat (pgRect_.Right()), ToFloat (pgRect_.Bottom()));
		LOG (logstr);
		::sprintf (logstr, "<p>LiveAreaRect left_=\"%4.3f\" top_=\"%4.3f\""
							" right_=\"%4.3f\" bottom_=\"%4.3f\"</p>", 
							ToFloat (left_), ToFloat (top_), 
							ToFloat (right_), ToFloat (bottom_) );
		LOG (logstr);
	}
	catch (PMString exceptionString) {
		CAlert::ErrorAlert (exceptionString);
		const char* pCstr = exceptionString.GrabCString ();
		LOG (pCstr);
	}	
	LOG_END_FUNCTION;
}

void HM_Page::GoTo_()
{
	LOG_BEGIN_FUNCTION;
	DECLARE_logstr;
	
	ErrorCode status = kFailure;

	// Create the command -- Set page to view -- kSetPageCmdBoss
	InterfacePtr<ICommand> gotopageCmd (CmdUtils::CreateCommand (kSetPageCmdBoss));
	if (!gotopageCmd) throw "ICommand";
	
	// Specify arguments:
	// Item list: UID of spread which contains the page
	::sprintf (logstr, "pageIndex_=%d", pageIndex_);
	LOG (logstr);
	
	InitializeInDesignData_();
	gotopageCmd->SetItemList ( UIDList (spreadUIDRef_) );
	
	::sprintf (logstr, "spread uid=0x%x", spreadUIDRef_.GetUID().Get());
	LOG (logstr);
	
	InterfacePtr<ILayoutCmdData> iLayoutCmdData (gotopageCmd, UseDefaultIID());
	if (!iLayoutCmdData) throw "ILayoutCmdData";

	// ILayoutCmdData: IDocument and ILayoutControlData of layout view
// This is the CS2 code; won't compile because of changes to Utils interface.
//	iLayoutCmdData->Set(::GetUIDRef(::GetFrontDocument ()), ::QueryFrontLayoutData());
	// Get front document and front layout (CS4):
	InterfacePtr<IDocument> frontDocument (
			Utils<ILayoutUIUtils>()->GetFrontDocument (), UseDefaultIID ());
	InterfacePtr<ILayoutControlData> frontLayoutData (
			Utils<ILayoutUIUtils>()->QueryFrontLayoutData (), UseDefaultIID ());
	iLayoutCmdData->Set(::GetUIDRef(frontDocument), frontLayoutData);


	// IUIDData: UID of page to put in center of view
	InterfacePtr<IUIDData> gotopageCmdUIDData (gotopageCmd, UseDefaultIID()); 
	gotopageCmdUIDData->Set (uidRef_);
	
	::sprintf (logstr, "page uid=0x%x", uidRef_.GetUID().Get());
	LOG (logstr);

	// process the command
	status = CmdUtils::ProcessCommand (gotopageCmd);
	::sprintf (logstr, "status=%d", status);
	LOG (logstr);

	::sprintf (logstr, "visible page uid=0x%x", 
		Utils<ILayoutUIUtils>()->GetVisiblePageUID (
				Utils<ILayoutUIUtils>()->QueryFrontView()).Get());
		
	LOG (logstr);
/***
	::sprintf (logstr, "Begin pasting up page index_=%d", pageIndex_);
	CAlert::InformationAlert (logstr);
***/
	LOG_END_FUNCTION;
}


/*================================================================================*/
void HM_Page::PasteUp_(std::vector<CL_Display_Ad> & adsToPlaceVector) {
/*----------------------------------------------------------------------------------
	Abstract:
		This method places ads on this page.  It replaces the C function 
		FitAdsToPage() in the old product (old source: sd.c).

	Parameters and modes:
		NOT SURE YET

	Returns:
		NOT SURE YET

	Change Log:
		18-Jun-03	New.
----------------------------------------------------------------------------------*/
	extern void dump_ad (CL_Display_Ad ad);
	extern void dump_newsCAD_rect (CL_NewsCadRect & ncr);

	LOG_BEGIN_FUNCTION;

	// Be sure there are some rectangles open on the page
	if (rectanglesVector_.size () == 0)
	{
		LOG ("No rectangles found.");
		return;												// Nothing to do
	}

	ASSERT (pHMGeometryImporter_);							// Yes, this IS necessary

	// The program places the ads starting with the largest ad and working 
	// through the array of ads to place down to the smallest.
	PMReal Shortest, Thinest;
	SortAdsBySquareArea_(adsToPlaceVector, &Shortest, &Thinest);

#if VERSION == IN_HOUSE
	LOG ("*** ADS TO PLACE (SORTED BY AREA DESCENDING):");
	for_each (adsToPlaceVector.begin (), adsToPlaceVector.end (), dump_ad );
#endif

	// Process the open rectangles, store in another vector
	vector<CL_NewsCadRect> newsCadRects;
	InitializeOpenRects_(newsCadRects);

	// Set up big, scary loop to place as many ads as possible
	int countAdsToPlace = adsToPlaceVector.size ();
	while (newsCadRects.size () > 0 && countAdsToPlace > 0)
	{
		// Important note about STL sort() algorithm -- if we ask to sort a vector  
		// containing just 1 element, the program won't crash.  The STL is smart 
		// enough not to call our comparison function with an empty parameter.

		// First, sort candidate rectangles by the y value.  Those that are 
		// closer to the bottom receive the higher priority
		sort ( newsCadRects.begin (), newsCadRects.end (), Compare_NewsCadRects_Y );
#if VERSION == IN_HOUSE
		LOG ("<!-- DUMP OF newsCadRects VECTOR after sorting on y2: -->");
		for_each (newsCadRects.begin (), newsCadRects.end (), dump_newsCAD_rect );
#endif
		// Second, sort candidate rectangles by x-position.  Sort order depends 
		// on which side of the spread we're on.  On a left-hand page, sort 
		// rects from left to right; on an odd page, sort them from right to left.
		ASSERT (pageType_ != -1);	// ENSURE HM_Page::SetupForAdPlacement_() CALLED
		sort ( newsCadRects.begin (), newsCadRects.end (), 
				pageType_ == kRightPage ? Compare_NewsCadRects_X : 
				Compare_NewsCadRects_X_LeftPage);
#if VERSION == IN_HOUSE
		LOG ("<!-- DUMP OF newsCadRects VECTOR after sorting on x1: -->");
		for_each (newsCadRects.begin (), newsCadRects.end (), dump_newsCAD_rect );
#endif

		// now grab the width and height of the current rectangle 
		// as well as its position on the page
		PMReal theWidth = newsCadRects[0].Width ();
		PMReal theHeight = newsCadRects[0].Height ();
		PMReal placex = newsCadRects[0].Left ();
		PMReal placey = newsCadRects[0].Bottom ();
		
		// Find an ad to fit the current rectangle -- if an ad is found, it 
		// is returned as an array element number for the ad array
		int theAd = FindBestFit_(newsCadRects[0], adsToPlaceVector);

		// Store InDesign rect -- if  an ad is found to fit this rectangle, 
		// we'll need the Quark rectangle later to compute new rects based on 
		// the newly-placed ad's geometry.
		CL_NewsCadRect currNewsCadRect = newsCadRects[0];

		// Forget about the rectangle in the array
		newsCadRects.erase ( newsCadRects.begin () );
		if (theAd >= 0)
		{
			// original ads is the array that contains all of the ads passed
			// to the routine. Set the position to 12 to flag PasteUp_() that it 
			// should use the computed position in boxx_ and boxy_.
			adsToPlaceVector[theAd].SetPosition_(12);

			// On a right-hand page, push ad flush right
			if (pageType_ == kRightPage)
			{
				placex += (theWidth - adsToPlaceVector[theAd].Width_());
			}

			adsToPlaceVector[theAd].SetBoxX_(placex);
			adsToPlaceVector[theAd].SetBoxY_(
									placey - adsToPlaceVector[theAd].GetHeight_());

			// Go ahead and create the ad on the page (old MakeAd routine)
			adsToPlaceVector[theAd].PasteUp_(this);
			--countAdsToPlace;

			// Now that we've placed an ad, notify our parent so (s)he can 
			// take whatever action is necessary (like re-H&Jing the story, 
			// accumulating and storing the area consumed by the ad, etc.).
			pHMGeometryImporter_->HandleAdPastedUp_(theAd);

			// Cycle through the remaining rectangles on the page to 
			// invalidate / extend those that were intersected, thereby 
			// creating a new set extending from the boundaries of the ad.
			UpdateRectanglesVector_(theAd, newsCadRects, adsToPlaceVector);
			
			// Generate new rects ...
			// Create a rectangle that extends directly above the placed ad.
			CL_NewsCadRect r;
			r.Left   (adsToPlaceVector[theAd].GetAdLeft_());
			r.Right  (adsToPlaceVector[theAd].GetAdRight_());
			r.Bottom (adsToPlaceVector[theAd].GetAdTop_() - gutterWidth_);
			r.Top    (placey - theHeight);

			// if the width ends up being to narrow or
			// the height is too small, just forget about it.
			// Otherwise, add it to the rectangles array.
			if ( (r.Width () >= Thinest) && (r.Height () >= Shortest) )
			{
				newsCadRects.push_back (r);
			}

			// Create a rectangle to the left of the placed ad 
			// which is as high as the heightVar (could be height of page)
			r.Left (left_);										// Page left margin
			r.Right (adsToPlaceVector[theAd].GetAdLeft_() - gutterWidth_);
			if (newsCadRects.size () > 0)
			{
				r.Top (currNewsCadRect.Top ());
				r.Bottom (currNewsCadRect.Bottom ());
			}
			else
			{
				// No open rectangles currently on the page -- use entire live area.
				r.Top (top_);
				r.Bottom (bottom_);
			}

			// once again, if the width is to thin or height to small, 
			// forget it, otherwise add it to the rectangles array.
			if ( (r.Width () >= Thinest) && (r.Height () >= Shortest) )
			{
				newsCadRects.push_back (r);
			}
		}
	}
	LOG_END_FUNCTION;
}

#pragma mark -
//----------------------------------------------------------------------------------
//	SetupForClassifiedContinuedLinePlacement_
//
//	Make sure the page object has been initialized.  Then discover all the 
//	interesting stuff about it (margins, gutters, columns, etc.).
//----------------------------------------------------------------------------------
void HM_Page::SetupForClassifiedContinuedLinePlacement_()
{
	LOG_BEGIN_FUNCTION;

	// Make sure this object has been constructed correctly, or initialized.
	ASSERT (uid_.Get());
	ASSERT (iPageList_);

	ComputePageGeometry_();

	LOG_END_FUNCTION;
}

//----------------------------------------------------------------------------------
//	Look through the vector referenced by inNewsCadRects and find the topmost one.
//	Send it back in outTopmost.
//----------------------------------------------------------------------------------
void HM_Page::FindTopmost_(std::vector<CL_NewsCadRect> & inNewsCadRects, 
									CL_NewsCadRect & outTopmost)
{
	PMReal top = bottom_;
	PMReal curFRectTop;
	vector<CL_NewsCadRect>::iterator p = inNewsCadRects.begin ();
	while (p < inNewsCadRects.end ())
	{
		curFRectTop = p->Top ();
		if (curFRectTop < top)
		{
			outTopmost = *p;
			top = curFRectTop;
		}
		++p;
	}
}


//----------------------------------------------------------------------------------
//	FilterCandidateRectsForContinuedLine_
//
//	Walk through the rects in newsCadRects and remove any that are not at least 
//	24 point deep.
//----------------------------------------------------------------------------------
void HM_Page::FilterCandidateRectsForContinuedLine_(
											vector<CL_NewsCadRect> & newsCadRects)
{
	vector<CL_NewsCadRect>::iterator p = newsCadRects.begin ();
	while (p < newsCadRects.end ())
	{
		if (p->Height () < 24.0)
		{
			newsCadRects.erase (p);
		}
		else
		{
			++p;
		}
	}
}


#pragma mark -
#include "IHierarchy.h"
#include "IStandOff.h"
#include "SDKLayoutHelper.h"
#include "SnpGraphicHelper.h"
#include "ITextModelCmds.h"
#include "HM_ContinuedLine.h"

UIDRef HM_Page::CreateTextFrameForContinuedLine_(
CL_NewsCadRect* inNewsCadRectPtr, 
UIDRef & outContinuedLineStoryUIDRef)
{
	extern ErrorCode SetRunaround_(const UIDRef& graphicFrameUIDRef, 
						const PMReal inAmount = 0, 
						IStandOff::mode mode = IStandOff::kBothSides);
	extern ErrorCode SetRunaround_(const UIDRef& graphicFrameUIDRef, 
						const PMReal &topMargin, 
						const PMReal &leftMargin, 
						const PMReal &bottomMargin, 
						const PMReal &rightMargin,
						IStandOff::mode mode = IStandOff::kBothSides);
	extern ErrorCode SetIgnoreRunarounds_(
						const UIDRef& inTextFrameUIDRef, bool inIgnore);
	LOG_BEGIN_FUNCTION;						
	DECLARE_logstr;
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
				
				// For whatever reason, those fine folks at Adobe have decided 
				// that 0,0 is the top left of the page column (live) area 
				// rather than the trim.  This despite the fact that their 
				// palette shows item positions displaced from the trim!
				PMRect boundsInPageCoords (
					inNewsCadRectPtr->Left () - InsideMargin_(),
					inNewsCadRectPtr->Top () - GetTop_(),
					inNewsCadRectPtr->Right () - InsideMargin_(),
					inNewsCadRectPtr->Bottom () - GetTop_());
				gpLogFile->LogPMRect_(
								"boundsInPageCoords", boundsInPageCoords);

				PMRect boundsInParentCoords = layoutHelper.PageToSpread (
											pageUIDRef, boundsInPageCoords);
				gpLogFile->LogPMRect_(
							"boundsInParentCoords", boundsInParentCoords);

				// Create the frame.
				// Create the text box -- 1 column, horizontal text direction.
				UIDRef storyUIDRef;
				result = layoutHelper.CreateTextFrame (
									parentUIDRef, boundsInParentCoords, 1, 
									kFalse, &storyUIDRef);
				outContinuedLineStoryUIDRef = storyUIDRef;
				::sprintf (logstr, 
						"<ContinuedLineBoxUid>0x%x</ContinuedLineBoxUid>", 
						result.GetUID().Get());
				LOG (logstr);

				// Add a runaround at bottom of continued line equal to 
				// the page gutter width.
				ErrorCode e = SetRunaround_(result, 
											0.0, 0.0, GetGutterWidth_(), 0.0);

				// Set a text inset of .045" (3.25 pt)
				InDesignTextFrameOptionsManipulator* tfm = 
										new InDesignTextFrameOptionsManipulator;
				tfm->SetAndApplyTextInset_(result, 0.0, 3.25, 0.0, 0.0);
				delete tfm;

				// Make the text box white.
				UIDList txtBoxUidLst (result);
				SnpGraphicHelper graphicsHelper (txtBoxUidLst);
				UID swatchUID = graphicsHelper.GetNamedSwatch (
												PMString("Paper"), // "Yellow"
												txtBoxUidLst.GetDataBase());
				graphicsHelper.AddFillRendering (swatchUID);
				graphicsHelper.ApplyAttributes ();
			}
		}
	}

	LOG_END_FUNCTION;
	return result;
}


/*================================================================================*/
void HM_Page::TypesetAndPasteupContinuedLine_(PMString & inHeadingText) {
/*----------------------------------------------------------------------------------
	Abstract:
		
	Parameters and modes:
		inHeadingText	Data for the continued line.							IN

	Returns:
	
	Caller:
		CL_TypesetContinuedLinesWorkhorse::TypesetAndPasteUpContinuedLine_().

	Change Log:
		19-Jun-07	New.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	DECLARE_logstr;
	LOG_TAG_AND_VALUE ("ClassifiedHeading", inHeadingText.GrabCString ());

	// Be sure there are some rectangles open on the page
	if (rectanglesVector_.size () == 0)
	{
		LOG ("<!-- No rectangles found. -->");
		return;												// Nothing to do
	}

	// Process the open rectangles, store in another vector
	vector<CL_NewsCadRect> newsCadRects;
	InitializeOpenRects_(newsCadRects);

	// Remove any candidate rectangles that aren't at least 24 pts deep.
	FilterCandidateRectsForContinuedLine_(newsCadRects);

	// Sort new vector of rects by their x-position.  This should 
	// put the leftmost rectangle first in the vector.
	sort ( newsCadRects.begin (), newsCadRects.end (), 
			Compare_NewsCadRects_X_LeftPage );

	// Now look through the vector and find the topmost, which makes it 
	// the "leftmost topmost" rectangle.
	CL_NewsCadRect topmost;
	FindTopmost_(newsCadRects, topmost);

	// If the next classification head is on the current page, check for an 
	// intersection between it and the candidate rectangle for the continued line.
	bool continuedLineWouldOverprint = false;
	
	// Find a way to get from the text range indices to an IWaxLine, which 
	// contains the width, height, x-position, y-position, etc. of the type.
	// See SnpInspectTextModel::ReportLines().
	// Also see lines 340-362 in Quark 4 code HM_Page.cpp.

	// If continued line will fit and not intersect with a classified heading,
	// create a text box for it and typeset the text passed in inHeadingText.
	if (topmost.Height () >= 24.0 && ! continuedLineWouldOverprint)
	{
		// Continued line should overlay top and left of rect "topmost".
		CL_NewsCadRect *pContinuedLineRect = new CL_NewsCadRect;
		*pContinuedLineRect = topmost;
		pContinuedLineRect->SetHeight (27.216);				// Arbitrary for now
		pContinuedLineRect->SetWidth (GetColumnWidth_());
		
		// Create the text box.
		// See CL_Display_Ad::CreatePictureAndTextBoxes_ID_().
		/* UIDRef result = UIDRef::gNull; */
		UIDRef continuedLineStoryUIDRef;
		UIDRef result = CreateTextFrameForContinuedLine_(
									pContinuedLineRect, continuedLineStoryUIDRef);
		delete pContinuedLineRect;
		if (result != UIDRef::gNull)
		{
			HM_ContinuedLine* continuedLineObj = 
				new HM_ContinuedLine (continuedLineStoryUIDRef, inHeadingText);
			continuedLineObj->Typeset_();
			continuedLineObj->SetBoxDepthToTextDepth_(result);
			delete continuedLineObj;
		}
	}

	LOG ("<!-- END OF HM_Page::TypesetAndPasteupContinuedLine_(PMString &) -->");
	LOG_END_FUNCTION;
}


#pragma mark ========== UNIMPLEMENTED ==========
#if 0
void HM_Page::FindAdNumbersPlaced_(vector<CL_Display_Ad> & v)
{
	// Initialize some stuff
	setmode (CONTENTSMODE);
	int16 curdoc;
	xtget_curdoc (&curdoc);
	if (curdoc >= 0)
	{
/*	22-Oct-03
		int16 curpage;
		xtget_curpage (&curpage);

		pagenumdata page;
		xtseq2page (curpage, FALSE, &page);
*/
		InitializePagenumdata_();
/*
		folio_ = page.pagenum;			// QXP sequential page number
		spread_ = page.spreadnum;		// QXP spread number on which page appears
*/
		folio_ = pagenumdata_.pagenum;		// QXP sequential page number
		spread_ = pagenumdata_.spreadnum;	// QXP spread number on which page appears
	}

	ASSERT (spread_ >= 1);
	
	// Priming read
	boxid currentBox = firstbox (spread_, TRUE);
	while (currentBox != NULL)
	{
		if (BoxOnPage (currentBox) && isapicture (currentBox))
		{
			Handle h = getslug (currentBox, OBJECTSLUG, 'ANZ1');
			if (h)
			{
				::HLock (h);
				CL_Display_Ad* ad = new CL_Display_Ad;
				ad->SetInputRecord_((char*) *h);
				ad->SetPageNumber (folio_);
				v.push_back (*ad);				// Copy ad into the vector
				delete ad;						// Return object to free store

				::HUnlock (h);
				::DisposeHandle (h);
			}			// end if our object slug found
		}				// end if box is on page and is a picture box
		currentBox = nextbox (currentBox, TRUE);
	}					// end while (currentBox != NULL)

}


/*================================================================================*/
void HM_Page::TypesetAndPasteUpContinuedLine_(
									char *inHeadingText, Rect *inNextHeadRect) {
/*----------------------------------------------------------------------------------
	Abstract:
		Typesets the text of the classification heading and the word "continued" 
		and places it at the top and leftmost point of the ad live area that 
		doesn't contain a display ad.
		
	Parameters and modes:

	Returns:

	Change Log:
----------------------------------------------------------------------------------*/
	LOG ("HM_Page::TypesetAndPasteUpContinuedLine_()");

	// Be sure there are some rectangles open on the page
	if (rectanglesVector_.size () == 0)
	{
		LOG ("No rectangles found.");
		return;												// Nothing to do
	}

	// Process the open rectangles, store in another vector
	vector<CL_NewsCadRect> newsCadRects;
	InitializeOpenRects_(newsCadRects);

	// Sort new vector of fixedrects by their x-position.  This should 
	// put the leftmost rectangle first in the vector.
	std::sort ( newsCadRects.begin (), newsCadRects.end (), 
			Compare_NewsCadRects_X_LeftPage );

	// Look at the candidate rectangles and remove any that 
	// aren't at least 24 pts deep.
	vector<CL_NewsCadRect>::iterator p = newsCadRects.begin ();
	while (p < newsCadRects.end ())
	{
		if (p->GetQuarkRectHeight_() < FIX (24L))
		{
			newsCadRects.erase (p);
		}
		else
		{
			++p;
		}
	}
	
	// Now look through the vector and find the topmost, which makes it 
	// the "leftmost topmost" rectangle.
	CL_NewsCadRect topmost;
	Fixed top = bottom_;
	Fixed curFRectTop;
	p = newsCadRects.begin ();
	while (p < newsCadRects.end ())
	{
		curFRectTop = p->GetQuarkRectTop_();
		if (curFRectTop < top)
		{
			topmost = *p;
			top = curFRectTop;
		}
		++p;
	}

	// If the next classification head is on the current page, check for an 
	// intersection between it and the candidate rectangle for the continued line.
	bool continuedLineWouldOverprint = false;
	if (inNextHeadRect)
	{
		// Adjust bounds of topmost leftmost to the correct measure for typesetting.
		// Give it an arbitrary depth just for the moment.
		fixedrect *continuedLineFR = new fixedrect;
		topmost.GetQuarkRect_(continuedLineFR);
		continuedLineFR->right = continuedLineFR->left + columnWidth_;
		continuedLineFR->bottom = continuedLineFR->top + gutterWidth_;
		
		CL_NewsCadRect* ncr = new CL_NewsCadRect;
		ncr->Initialize_(inNextHeadRect);
		fixedrect* nextHeadFR = new fixedrect;
		ncr->GetQuarkRect_(nextHeadFR);
		delete ncr;
		sprdfrect2page (nextHeadFR);
		continuedLineWouldOverprint = CL_NewsCadRect::FRectinFRect (
												nextHeadFR, continuedLineFR);
		delete nextHeadFR;
		delete continuedLineFR;
	}
	
	// Be sure a rectangle was found that would accommodate the heading.
	if (topmost.GetQuarkRectHeight_() > FIX (24L) && ! continuedLineWouldOverprint)
	{
char ds[256];
sprintf (ds, "Output continued line on pg = %d  heading = %s", folio_, inHeadingText);
LOG (ds);
LOG ("RECTANGLE = ");
topmost.Log_();
		// Create the text box.
		try {
			xtboxptr bp = newxtbox (CT_TEXT, SH_RECT, FALSE);
			if (!bp) throw MemError ();
			bp->t.box.shade = 13107L;
			bp->t.box.color = COLOR_WHITE;
			topmost.GetQuarkRect_(&bp->t.box.s.r.fr);
			bp->t.textalign = TEXTTOP;
			bp->t.box.runaround.type = RUN_BOX;
			bp->t.box.runaround.outset.bottom = gutterWidth_;
			bp->t.textinset[0] = 0L;
			bp->t.textinset[1] = 0L;
			bp->t.textinset[2] = 0L;
			bp->t.textinset[3] = 0L;
			
/*	22-Oct-03
			int16 curDocPage;
			// TBD:  Make member func to do this
			XTError xtErr = xtget_curpage (&curDocPage);
			if (xtErr) throw (xtErr);
			pagenumdata page;
			xtErr = xtseq2page (curDocPage, false, &page);
			if (xtErr) throw (xtErr);
*/
			InitializePagenumdata_();
/*
			pagefrect2sprd (&page, &bp->t.box.s.r.fr);
*/
			pagefrect2sprd (&pagenumdata_, &bp->t.box.s.r.fr);
			uchar* s = new uchar [255];
			Fixed fixedValue;
			STRCPY (s, "\p27.25pt");
			str2valbuf (POINTUNITS, 0, &fixedValue, s);
			delete [] s;
			bp->t.box.s.r.fr.bottom = bp->t.box.s.r.fr.top + fixedValue;
			bp->t.box.s.r.fr.right = bp->t.box.s.r.fr.left + columnWidth_;

			boxid textBoxID = xtinstallbox (bp, 0, NULL, TRUE);
			disposextbox (bp, TRUE);
			
			TypesetContinuedLine_(inHeadingText);

			// Adjust depth of box so that all text shows.
			VerifyBoxDepth (textBoxID, FIX (1));
		}
		catch (OSErr err) {
			giveerror (err);
		}
	}
}


void HM_Page::TypesetContinuedLine_(char *inHeadingText) {

	xesetsel (0, 0, TRUE);

	// Flow in the heading text.
	long charCount = ::strlen (inHeadingText);
	xeputchars (0, charCount, (unsigned char *) inHeadingText);
	
	// Apply the classified heading (continued) stylesheet.
	int16 index;
	parastylerec theParaStyle;
	xesetsel (0, charCount, TRUE);
	if (getparastylebyname (STRCONST ("Continued Head1"), &theParaStyle, &index))
	{
		setparastyleontext (index, TRUE);
	}

	// Flow in "continued".
	long prevCharCount = charCount;
	char* continuedStr = "\rcontinued";
	charCount = ::strlen (continuedStr);
	xeputchars (prevCharCount, charCount, (unsigned char *) continuedStr);
	xecalc (TRUE);
	
	// Apply that stylesheet.
	xesetsel (prevCharCount + 1, prevCharCount + 2 + charCount, TRUE);
	if (getparastylebyname (STRCONST ("Continued"), &theParaStyle, &index))
	{
		setparastyleontext (index, TRUE);
	}
}


// TBD:  THIS SHOULD BE IN A "BOX" CLASS
/*================================================================================*/
Fixed VerifyBoxDepth (boxid bxid, Fixed leadingIncrement) {
/*----------------------------------------------------------------------------------
	Abstract:
		
	Parameters and modes:
	
	Modifies:
		
	Change Log:
		11-Aug-03	Lifted from the Amway project BoxUtils.c and tweaked a little.
----------------------------------------------------------------------------------*/
	int32		startch;
	fixedpoint	fixedoffset;
	loshandle	losHdl;
	int16		loslen;
	Fixed		overflowtest;
	xehandle	xehndl;
	int32		start, end, textlen;
	boxid		thebox;
	xtboxptr	bp;
	Fixed		finalBoxDepth = FIX (2L);
	boxid		curBox;
	XTError		xtErr;
	
	ASSERT (leadingIncrement > 0);
	
	/* Make sure the box we're to operate on is the current box.  Select if nec. */
	xtErr = xtget_curbox (&curBox);
	if (xtErr) giveerror (xtErr);
	if (curBox != bxid) {
		xtErr = xtset_curbox (bxid);
		if (xtErr) giveerror (xtErr);
	}

	thebox = bxid;

	/* This text box must not be linked to any others */
	if (prevtextbox (thebox) || nexttextbox (thebox))
	{
		ASSERT (0);
		return (finalBoxDepth);
	}

	/* Get original (current) box depth */
	bp = getxtbox (NULL, thebox, false);
	finalBoxDepth = bp->t.box.s.r.fr.bottom - bp->t.box.s.r.fr.top;
	disposextbox (bp, true);

	xegetinfo (thebox, &xehndl, &start, &end, &textlen);

	/* Nothing to do if there's no text in the box */
	if (textlen == 0L)
		return (finalBoxDepth);

	xesetsel (0L, 0L, TRUE);
	xesetsel (0L, textlen, FALSE);

	/* Get los information for the text in the text box */
	startch = 0L;
	getlos (&thebox, &startch, FALSE, &fixedoffset, &losHdl);
	loslen = GetHandleSize ((Handle) losHdl) / sizeof (losrec);
	
	while ((overflowtest = (**losHdl)[loslen-1].ascent) == TXTOVFL)
	{
		/* Text overflows the box--add 1 line of lead to it */
		bp = getxtbox (NULL, thebox, false);
		bp->t.box.s.r.fr.bottom += leadingIncrement;
		finalBoxDepth = bp->t.box.s.r.fr.bottom - bp->t.box.s.r.fr.top;

		if (setxtbox (bp, thebox, false, true))
		{
			/* setbox failed -- box is probably as big as it can be */
			disposextbox (bp, true);
			break;
		}

		disposextbox (bp, true);
		bp = NULL;
		getlos (&thebox, &startch, FALSE, &fixedoffset, &losHdl);
		loslen = GetHandleSize ((Handle) losHdl) / sizeof (losrec);
	}

	return (finalBoxDepth);
}
#endif
#pragma mark END_UNIMPLEMENTED

// END OF FILE
