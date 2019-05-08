/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	Basic_Page.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	7/12/06 ca. 8:52 PM
	
	ab	Abstract:
	ab	Implementation of Basic_Page class.
	ab	
	
	cl	Change Log:
	cl	7/12/06	New.
------------------------------------------------------------------------------*/

using namespace std;
#include "Basic_Page.h"
#include "Basic_Display_Ad.h"
#include "IDocument.h"
#include "IControlView.h"
#include "ILayoutUIUtils.h"
#include "IHierarchy.h"
#include "IPageList.h"
#include "IMargins.h"
#include "IColumns.h"
#include "PMReal.h"
#include "IGeometry.h"
#include "ISpread.h"
#include "ISpreadList.h"
#include "ISpreadLayer.h"
#include "IFrameType.h"
#include "SplineID.h"
#include "TransformUtils.h"

//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and  D E S T R U C T I O N
//----------------------------------------------------------------------------------
Basic_Page::Basic_Page ()											// Create
	:	pageIndex_(0)
	,	columnCount_(0)
{
	pgRect_.Left (0);
	pgRect_.Right (0);
	pgRect_.Top (0);
	pgRect_.Bottom (0);
}
/*
Basic_Page::Basic_Page (something)									// Create
{

}
*/
Basic_Page::~Basic_Page ()											// Destroy
{

}

Basic_Page::Basic_Page (const Basic_Page & inObj)					// Copy
{

}

Basic_Page & Basic_Page::operator= (const Basic_Page & inObj)		// Assign
{
	if (this != &inObj)										// Beware of "a = a"
	{
		// put some code here
	}

	return *this;
}


/*================================================================================*/
void Basic_Page::ComputeAndStorePageMeasurements_() throw (PMString) {
/*----------------------------------------------------------------------------------
	Abstract:
		This is similar to HM_Page::SetupForAdPlacement_() in NewsCAD_ID.mcp.  Its 
		job is to discover the widths of this page's layout elements like margins, 
		columns, and gutters.
		
	Parameters and modes:
		None.  Uses member variable uidRef_.

	Modifies:
		columnCount_		gutterWidth_		columnWidth_		
		width_				height_				pgRect_
		left_				top_				right_				bottom_

	Returns:

	Caveats:
		Caller must catch exceptions!
		
	Change Log:
----------------------------------------------------------------------------------*/
	// Get front document
	InterfacePtr<IDocument> frontDocument (
			Utils<ILayoutUIUtils>()->GetFrontDocument (), UseDefaultIID ());
	if ( !frontDocument ) throw "could not get pointer to front document";
	
	// Get page list
	InterfacePtr<IPageList> pageList (frontDocument, UseDefaultIID());
	if (!pageList) throw ("could not get IPageList");
	
	// Now that we have the document's page list and we've stored the page's 
	// UID, we can use the 2 together to get interesting facts about the page.
	// Get a pointer to the page's margin data, and extract what's interesting.
	IMargins* iMargins = pageList->QueryPageMargins (uid_);
	if (!iMargins) throw ("could not get IMargins");
	PMReal insideMargin, outsideMargin, topMargin, bottomMargin;
	iMargins->GetMargins (&insideMargin, &topMargin, &outsideMargin, &bottomMargin);

	// We'll need this for later when we place ads -- InDesign applies a 
	// transform to the same geometry to place things on left and right pages.
	insideMargin_ = insideMargin;
	
	// Get a pointer to the page column data, and extract the count of page 
	// cols, the width of a single gutter, and measure consumed by all gutters.
	IColumns* iColumns = pageList->QueryPageColumns (uid_);
	if (!iColumns) throw ("could not get IColumns");
	columnCount_ = iColumns->GetNumberColumns ();	// Number of page cols
	gutterWidth_ = iColumns->GetGutter ();			// Width of a single gutter
	PMReal totalGutterWidth = (columnCount_ - 1) * 
									gutterWidth_;	// Measure consumed by gutters
	
	// Get the left X and right X of each page column, compute the width of 
	// each one and accumulate the total.  InDesign expresses these values as 
	// DISPLACEMENTS from the page margins.  They are not absolute.
	PMRealList outputColumns;
	iColumns->GetColumns (&outputColumns);
	PMRealList::iterator p = outputColumns.begin ();
	PMReal x1, x2;
	int i = 1;
	PMReal totColWidth = 0;						// Measure consumed by columns
	while ( p < outputColumns.end () )
	{
		x1 = *p;
		++p;
		x2 = *p;
		totColWidth += x2 - x1;
		++p;
	}
	
	PMReal pmrColCount (columnCount_);
	columnWidth_ = totColWidth / pmrColCount;	// Width of a single column

	// Get the page rectangle.
	InterfacePtr<IGeometry> iGeometry (UIDRef_(), UseDefaultIID ());
	if (!iGeometry) throw ("could not get IGeometry");
	PMRect tmpPMRect (insideMargin, topMargin, outsideMargin, bottomMargin);
//		iColumns->GetNthColumnBox (iGeometry, 0, tmpPMRect);
	tmpPMRect = iGeometry->GetStrokeBoundingBox (::InnerToParentMatrix (iGeometry));
	
	// Copy the page rect
	pgRect_.Left (tmpPMRect.Left ());
	pgRect_.Right (tmpPMRect.Right ());
	pgRect_.Top (tmpPMRect.Top ());
	pgRect_.Bottom (tmpPMRect.Bottom ());
	
	// Set page LIVE AREA height & width
	height_ = tmpPMRect.Bottom () - tmpPMRect.Top () - 
				topMargin - bottomMargin;
	width_ = tmpPMRect.Right () - tmpPMRect.Left () - 
				insideMargin - outsideMargin;
	
	// Live area coordinates are dependent on whether page is on the left 
	// or right (or neither, in the case of single-sided documents).
	// enum PageType { kLeftPage = 0, kUnisexPage, kRightPage };
	pageType_ = pageList->GetPageType (uid_);
	left_ = pageType_ == kRightPage ? insideMargin : outsideMargin;
	right_ = left_ + width_;
	top_ = topMargin;							// Top margin y-position
	bottom_ = topMargin + height_;				// Bottom margin y-position

}


/*================================================================================*/
void Basic_Page::ComputeCurrentPageUID_() throw (PMString) {
/*----------------------------------------------------------------------------------
	Abstract:
		Figure out the Adobe UID of the page currently on the user screen 
		and store it in a member for use later.
		
	Parameters and modes:
		None.
		
	Returns:
		Nothing.
		
	Caveats:
		The caller has to catch exceptions!
		
	Change Log:
		24-Jul-06	New.
----------------------------------------------------------------------------------*/
	PMString exceptionString = "Basic_Page::ComputeCurrentPageUID_() ";
	
	try {
		// Gets the IControlView of the front document
		InterfacePtr<IControlView> dControlView (
									Utils<ILayoutUIUtils>()->QueryFrontView () );
		if (!dControlView)
		{
			exceptionString += "could not get IControlView of front doc.";
			throw exceptionString;
		}

		// Get Spreadlist for parameter in ILayoutUIUtils->GetVisibleMasterSpreadUID()
		// Get the frontDocument first so that we can get the spreadlist
		InterfacePtr<IDocument> frontDocument (
					Utils<ILayoutUIUtils>()->GetFrontDocument (), UseDefaultIID ());
		if (!frontDocument)
		{
			exceptionString += "could not get pointer to front document.";
			throw exceptionString;
		}
		
		// Get Spreadlist
		InterfacePtr<ISpreadList> iSpreadList ( frontDocument, UseDefaultIID () );
		if (!iSpreadList)
		{
			exceptionString += "could not get pointer to spread list.";
			throw exceptionString;
		}
		
		// Find the spreadUID that is currently being viewed in a layout view
		const UID spreadUID = Utils<ILayoutUIUtils>()->GetVisibleSpreadUID (
														dControlView, iSpreadList);
		
		//InterfacePtr<ILayoutControlData> layoutControlData(dControlView, UseDefaultIID());
		//ASSERT(layoutControlData != nil);
		//if (layoutControlData == nil) {	break;	}
		
		// Get the IGeometry of the spread since we now have 
		// the spreadUID being viewed
		InterfacePtr<IGeometry> viewSpreadGeometry ( iSpreadList->QueryNthSpread (
										iSpreadList->GetSpreadIndex (spreadUID) ) );
		if ( !viewSpreadGeometry )
		{
			exceptionString += "could not get pointer to view spread geometry.";
			throw exceptionString;
		}

		// Get the IHierarchy of the spread so that we can go down 
		// the Hierarchy and have access to the spreadlayer.
		InterfacePtr<IHierarchy> viewSpreadHierarchy (
									viewSpreadGeometry, UseDefaultIID () );
		if ( !viewSpreadHierarchy )
		{
			exceptionString += "could not get pointer to view spread hierarchy.";
			throw exceptionString;
		}

		// Get access to the spreadlayer where the spline items are created. 
		// Reference: programming_guide.pdf page. 152 Figure 34
		InterfacePtr<IHierarchy> viewSpreadLayerHierarchy ( 
									viewSpreadHierarchy->QueryChild (2) );
		if ( !viewSpreadLayerHierarchy )
		{
			exceptionString += "could not get pointer to view spread layer hierarchy.";
			throw exceptionString;
		}

		UID visibleSpreadLayerUID = viewSpreadLayerHierarchy->GetLayerUID();
		if (visibleSpreadLayerUID == kInvalidUID)
		{
			exceptionString += "got invalid value for visible spread layer UID.";
			throw exceptionString;
		}

		InterfacePtr<ISpreadLayer> spreadLayer (::GetDataBase (viewSpreadHierarchy), 
										visibleSpreadLayerUID, IID_ISPREADLAYER);

		UIDRef parentUIDRef = ::GetUIDRef (spreadLayer);
		parentUidRef_ = parentUIDRef;

		// Get the page currently being viewed
		UID visiblePage = Utils<ILayoutUIUtils>()->GetVisiblePageUID (
																dControlView, nil);
		if (visiblePage == kInvalidUID)
		{
			exceptionString += "got invalid value for visible page UID.";
			throw exceptionString;
		}
		
		SetUID_(visiblePage);

//SDKLayoutHelper.cpp and .h are located at source\sdksamples\common
//SDKLayoutHelper layoutHelper;

		UIDRef pageUIDRef (parentUIDRef.GetDataBase(), visiblePage);
		SetUIDRef_(pageUIDRef);

/*
THIS CODE (OR SOMETHING LIKE IT) WILL MOVE TO THE METHOD THAT REALIZES THE AD.
// Get geometry to calculate where to place the new page item
InterfacePtr<IGeometry> pageGeometry(pageUIDRef, UseDefaultIID());
ASSERT(pageGeometry);
if (pageGeometry == nil) {		break;	}

PMRect pageBox = pageGeometry->GetStrokeBoundingBox(::InnerToParentMatrix(pageGeometry));

// Places the frame in the middle of the page with size 200.0 x 200.0
PMRect boundsInParentCoords(pageBox.GetHCenter()-100.0, pageBox.GetVCenter()-100.0, 
							pageBox.GetHCenter()+100.0, pageBox.GetVCenter()+100.0 );

// Create the frame.
result = layoutHelper.CreateTextFrame(parentUIDRef, boundsInParentCoords, 1, kFalse);
*/
	}
	catch (...) {
	}
}

// THIS FUNCTION SHOULD WORK ... BUT WHO KNOWS ... NEEDS TO BE SENT TO ADOBE.
void Basic_Page::ComputePageUIDRef_()
{
// PORTING CHANGE
/*
	IDataBase* database = ::GetDataBase (::GetFrontDocument ());
	IDataBase* database = ::GetDataBase (
							Utils<ILayoutUIUtils>()->GetFrontDocument() );
	UIDRef uidref (database, uid_);
	uidRef_ = uidref;
*/
	IDocument* doc = Utils<ILayoutUIUtils>()->GetFrontDocument();
	IDataBase* database = ::GetDataBase (doc);
	UIDRef uidref (database, uid_);
	uidRef_ = uidref;
}

// See UIDRef CL_Display_Ad::CreatePictureAndTextBoxes_ID_()
// in NewsCAD_ID.mcp
// The basic behavior is simply to plunk the ad down in the middle of the page.
void Basic_Page::PasteUp_(Basic_Display_Ad* ad) throw (PMString)
{
	// Starting in the center of the page, outset by half the ad width and depth.
	PMReal halfWidth = ad->Width_() / 2.0;
	PMReal halfDepth = ad->GetHeight_() / 2.0;
	PMRect boundsInParentCoords (
			pgRect_.GetHCenter () - halfWidth, pgRect_.GetVCenter () - halfDepth, 
			pgRect_.GetHCenter () + halfWidth, pgRect_.GetVCenter () + halfDepth );
	ad->SetBoxX_(pgRect_.GetHCenter () - halfWidth);
	ad->SetBoxY_(pgRect_.GetVCenter () - halfDepth);
	ad->SetPointerToParentPage_(this);
	ad->PasteUp_(ParentUIDRef_(), boundsInParentCoords);
}


// END OF FILE
