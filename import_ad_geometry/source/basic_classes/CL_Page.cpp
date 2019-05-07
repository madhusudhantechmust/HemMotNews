/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	CL_Page.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	9/03/02 ca. 10:29 AM
	
	ab	Abstract:
	ab	Implementation of CL_Page class.
	
	cl	Change Log:
	cl	10-Sep-05	Began port to InDesign.
	cl	9/03/02	New.
------------------------------------------------------------------------------*/

using namespace std;

#include "IDocument.h"
#include "IDataBase.h"
#include "IHierarchy.h"
#include "ILayoutUIUtils.h"
#include "IPageList.h"
#include "IMargins.h"
#include "IColumns.h"
#include "PMReal.h"
#include "IGeometry.h"
#include "ISpread.h"
#include "ISpreadList.h"
#include "IFrameType.h"
#include "SplineID.h"
#include "TransformUtils.h"
#include "GroupID.h"
#include "TextID.h"
#include "IPathUtils.h"
#include "IPageItemTypeUtils.h"
#include "IGraphicFrameData.h"
#include "CAlert.h"
#include "CL_Display_Ad.h"
#include "CL_Page.h"
#include "CL_Document.h"
#include "global_data.h"
#include "CL_Log_File.h"
#include "CL_NewsCadRect_ID.h"
#include "CL_Geometry_Importer.h"

void dump_newsCAD_rect (CL_NewsCadRect & ncr);

#if PRODUCT == QUARKXTENSION					// QuarkXPress only
	void dump_mac_rect (Rect r);
	void dump_fixed_rect (fixedrect r);
#endif

#if VERSION == IN_HOUSE
	#define		LOG_PIC_FULL_PATH											\
				{															\
					char* logPath = new char [pathCharCount + 1];			\
					P2CSTRCPY ((uchar*)logPath, fullPath);					\
					char* logstr = new char [::strlen (logPath) + 60];		\
					::strcpy (logstr, "<ImportPic path=\"");				\
					::strcat (logstr, logPath);								\
					delete [] logPath;										\
					::strcat (logstr, "\" />");								\
					LOG (logstr);											\
					delete [] logstr;										\
				}
	#define		LOG_ERROR_XTGETTEXTPIC										\
				{															\
					char *logstr = new char [256];							\
					::sprintf (logstr, "Error xtgettextpict(): %d", iErr);	\
					LOG (logstr);											\
					delete [] logstr;										\
				}
#else
	#define		LOG_PIC_FULL_PATH
	#define		LOG_ERROR_XTGETTEXTPIC
#endif


//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and  D E S T R U C T I O N
//----------------------------------------------------------------------------------

CL_Page::CL_Page ()
{
//	Initialize_();
}


CL_Page::CL_Page (CL_Geometry_Importer* inImporter)
{
	Initialize_();
	pGeometryImporter = inImporter;
}


void CL_Page::Initialize_()
{
	folio_ = 0;
	spread_ = 0;
	width_ = 0;
	height_ = 0;
	columnWidth_ = 0;
	columnCount_ = 0;
	gutterWidth_ = 0;
	pgRect_.Left (0);
	pgRect_.Top (0);
	pgRect_.Right (0);
	pgRect_.Bottom (0);
	spreadIndex_ = -1;						// Very bad -- using for both flag and data.
	pageType_ = static_cast<PageType> (-1);	// Ditto
	bottom_ = 0;
	left_ = 0;
	right_ = 0;
	top_ = 0;
	pGeometryImporter = NULL;
	isColorPage_ = false;
	narrowestAdWidth_ = 0;
	shallowestAdHeight_ = 0;
}


CL_Page::~CL_Page ()
{

}

/*
void CL_Page::AppendCurrentAd_(CL_Display_Ad* inAd)
{
	adsVector_.push_back (*inAd);
}
*/


void 
CL_Page::RecomputeCandidateRects_(vector<CL_NewsCadRect> & newsCadRects, 
							int adIndex, Fixed adYPos, Fixed adHeight, 
							vector<CL_Display_Ad> & adsToPlaceVector)
{
	LOG_BEGIN_FUNCTION;
	LOG ("Derived classes MUST override!");
	LOG_END_FUNCTION;
}


void 
CL_Page::SortCandidateRects_(vector<CL_NewsCadRect> & newsCadRects)
{
	LOG_BEGIN_FUNCTION;
	LOG ("Derived classes MUST override!");
	LOG_END_FUNCTION;
}


/*================================================================================*/
void CL_Page::SortAdsByDepthWithinWidth_(vector<CL_Display_Ad> & v) {
/*----------------------------------------------------------------------------------
	Abstract:

	Assumptions:
		The vector has already been sorted by columns wide!
----------------------------------------------------------------------------------*/
	if (v.size () < 2)
		return;
	
	vector<CL_Display_Ad>::iterator p, q, v_end;
	p = q = v.begin ();
	v_end = v.end ();
	int32 pcc, qcc;
	while ( p < v_end && q < v_end )
	{
		pcc = p->GetColumnCount_();
		qcc = q->GetColumnCount_();
		
		// Look for "control break".
		while (qcc == pcc && q < v_end)
		{
			++q;
			if ( q < v_end )		// Careful, careful!!  Check that pointer!
			{
				qcc = q->GetColumnCount_();
			}
		}
				
		if (p != q)					// Don't bother to call sort for just 1 element.
		{
			// Sort elements from p to q-1 by height descending.
			sort ( p, q, Compare_Ads_Depths );
		}

		p = q;						// Prepare to start over.
	}
}

bool Compare_Ads_ColumnsWide (CL_Display_Ad a, CL_Display_Ad b)
{
	return (a.GetColumnCount_() > b.GetColumnCount_());
}


bool Compare_Ads_Depths (CL_Display_Ad a, CL_Display_Ad b)
{
	return (a.GetHeight_() > b.GetHeight_());
}


bool Compare_Ads_Area (CL_Display_Ad a, CL_Display_Ad b)
{
	PMReal width = a.Width_();
	PMReal depth = a.GetHeight_();
	PMReal areaA = width * depth;
	width = b.Width_();
	depth = b.GetHeight_();
	PMReal areaB = width * depth;

	return ( areaA > areaB);
}

void dump_newsCAD_rect (CL_NewsCadRect & ncr)
{
#if VERSION == IN_HOUSE
	ncr.Log_();
#endif
}


/*================================================================================*/
void CL_Page::StorePageInfo_() {
/*----------------------------------------------------------------------------------
	Abstract:
		Get information about the document page from QuarkXPress and store in 
		member variables.
		
	Parameters and modes:

	Returns:

	Modifies:
		uid_	columnCount_	gutterWidth_	columnWidth_	height_		width_
		
	Change Log:
		13-Sep-05	Began port to InDesign.
		20-Feb-05	Began port to 6.x.
		03-Sep-02	Adapted from NewsCAD 68K code.
		14-Aug-99	Made common to all versions of the program.
		11-JAN-97	New.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	
	try {
		char logstr[256];

		// Get front document
		InterfacePtr<IDocument> frontDocument (
				Utils<ILayoutUIUtils>()->GetFrontDocument (), UseDefaultIID ());
		if ( !frontDocument ) throw "could not get pointer to front document";
	
		// Get page list
		InterfacePtr<IPageList> pageList (frontDocument, UseDefaultIID());
		if (!pageList) throw ("could not get IPageList");
	
		// We need the page's record number in the document database.
		UID uidPage1 = pageList->GetNthPageUID (0);			// Page 1
		SetUID_(uidPage1);
		
		ComputeAndStorePageMeasurements_();
#if 0	
		// Get a pointer to the page's margin data, and extract what's interesting.
		IMargins* iMargins = iPageList->QueryPageMargins (uidPage1);
		if (!iMargins) throw ("iMargins");
		PMReal insideMargin, outsideMargin, topMargin, bottomMargin;
		iMargins->GetMargins (&insideMargin, &topMargin, &outsideMargin, 
								&bottomMargin/*, bool16 wantMaster=kTrue*/);
// 		sprintf (logstr, "Margins (l,t,r,b): %8.3f    %8.3f    %8.3f    %8.3f",
// 				ToFloat (insideMargin), ToFloat (topMargin), 
// 				ToFloat (outsideMargin), ToFloat (bottomMargin));
// 		LOG (logstr);
	
		// Get a pointer to the page column data, and extract what's interesting.
		IColumns* iColumns = iPageList->QueryPageColumns (uidPage1);
		if (!iColumns) throw ("iColumns");
		columnCount_ = iColumns->GetNumberColumns (/*bool16 wantMaster=kTrue*/);
		gutterWidth_ = iColumns->GetGutter (/*bool16 wantMaster=kTrue*/);
		PMReal totalGutterWidth = (columnCount_ - 1) * gutterWidth_;

		// Get the left X and right X of each page column.  InDesign expresses these 
		// values as DISPLACEMENTS from the page margins.  They are not absolute.
		PMRealList outputColumns;
		iColumns->GetColumns (&outputColumns /*, bool16 wantMaster=kTrue*/);
		PMRealList::iterator p = outputColumns.begin ();
		PMReal x1, x2;
		int i = 1;
		PMReal colWidth = 0;
// 		LOG ("Page columns returned by GetColumns ():");
		while ( p < outputColumns.end () )
		{
			x1 = *p;
			++p;
			x2 = *p;
			colWidth += x2 - x1;
// 			sprintf (logstr, "x1=%8.3f    x2=%8.3f", x1, x2);
// 			LOG (logstr);
			++p;
		}
	
// 		sprintf (logstr, "Col width = %8.3f    Gutter width = %8.3f", 
// 					colWidth, totalGutterWidth);
// 		LOG (logstr);
		
		PMReal pmrColCount (columnCount_);
		columnWidth_ = colWidth / pmrColCount;
	
// 		sprintf (logstr, "columnCount= %d  widths:  column=%8.3f    gutter=%8.3f",
// 							columnCount_, columnWidth_, gutterWidth_);
// 		LOG (logstr);

		// Get the page rectangle.
		IDataBase* database = ::GetDataBase (iDocument);
		if (!database) throw ("database");
		UIDRef page1UIDRef (database, uidPage1);
		InterfacePtr<IGeometry> iGeometry (page1UIDRef, UseDefaultIID ());
		if (!iGeometry) throw ("iGeometry");
		PMRect tmpPMRect (insideMargin, topMargin, outsideMargin, bottomMargin);
		iColumns->GetNthColumnBox (iGeometry, 0, tmpPMRect);
		tmpPMRect = iGeometry->GetStrokeBoundingBox ();
// 		sprintf (logstr, "tmpPMRect (l,t,r,b)=%4.3f  %4.3f   %4.3f   %4.3f", 
// 				tmpPMRect.Left(), tmpPMRect.Top(), tmpPMRect.Right(), tmpPMRect.Bottom());
// 		LOG (logstr);

		// Set page height & width
		height_ = tmpPMRect.Bottom () - tmpPMRect.Top ();
		width_ = tmpPMRect.Right () - tmpPMRect.Left ();
// 		sprintf (logstr, "height_=%8.3f    width_=%8.3f", height_, width_);
// 		LOG (logstr);
		sprintf (logstr, 
			"columnCount_=%d  gutterWidth_=%4.3f  columnWidth_=%4.3f  height_=%4.3f  width_=%4.3f",
			columnCount_, ToFloat (gutterWidth_), 
			ToFloat (columnWidth_), ToFloat (height_), ToFloat (width_));
		LOG (logstr);
#endif
	}
	catch (char* s) {
		LOG (s);
	}
	LOG_END_FUNCTION;
}

/*
 virtual PBPMRect GetNthColumnBox 	( 	IGeometry * 	pageGeom,
		const int32 	n,
		const PMRect & 	marginRect
	) 	const [pure virtual]
		if (i & 1)						// We're looking at a column
		{
			colWidth += x2 - x1;
		}
		else							// We're looking at a gutter
		{
			totalGutterWidth += x2 - x1;
		}
*/

void CL_Page::SetupForAdPlacement_()	
{
	
}


#define LOG_FIND_OPEN_RECTS		1
#if VERSION == IN_HOUSE && LOG_FIND_OPEN_RECTS
	#define	LOG_FOLIO		::sprintf (logstr,									   \
						"<!-- CL_Page::FindOpenRects_(page %d, pageIndex %d) -->", \
							folio_, pageIndex_);  \
							LOG (logstr);
#else
	#define	LOG_FOLIO					NULL
#endif
/*================================================================================*/
int CL_Page::FindOpenRects_() {
/*----------------------------------------------------------------------------------
	Abstract:
		This routine looks at a Quark page and determines all the rectangles that 
		it thinks exist on the page.
		
	Parameters and modes:
		
	Change Log:
		02-Jan-07	Create a list of UIDRefs for the graphic boxes on the page, 
					then process that list.  This will save repeatedly 
					reconstructing it.
		20-Dec-06	Program is not detecting graphics when they're part of a group.
					Function needs a complete overhaul.
		13-Sep-05	Began port to InDesign.
		20-Feb-05	Ported to 6.x.
		09-Sep-02	Adapted from 68K NewsCAD C function FindOpenRectsOnThisPage().
		11-Aug-99	Don't blow up processing doublesided documents (spreads).
		23-May-99	This is the code that was in the 1994 version of the XTension.
					I've made 1 change today:  eliminate references to 
					thePage.colWidth and replace them with thePage.gutterWidth,
					which is what I believe the original programmer intended (but 
					who knows).  Also cleaned up the source a little bit (indents, 
					etc.) to make it a little more readable.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	DECLARE_logstr;
	LOG_FOLIO;	

	// Init rectangles vector, then create a default rectangle -- the page live 
	// area.  Program deletes it later if it finds a box on the page, and creates 
	// rectangles around that box.
	rectanglesVector_.clear ();
	rectanglesVector_.reserve (100);
	CL_NewsCadRect theFRect (left_, top_, right_, bottom_);
	rectanglesVector_.push_back (theFRect);

	// Get a list of the UIDRefs of the graphic frames on the page.
	GetGraphicFrameUIDRefVector_();
	
	InterfacePtr<ISpread> ipSpread (spreadUIDRef_, UseDefaultIID());

	// Get front document
	InterfacePtr<IDocument> iDocument (
			Utils<ILayoutUIUtils>()->GetFrontDocument (), UseDefaultIID ());
	if ( !iDocument ) throw "could not get pointer to front document";
	IDataBase* database = ::GetDataBase (iDocument);
	if (!database) throw ("IDataBase");
	UIDList pageItemList (database);
	ipSpread->GetItemsOnPage (pagePosOnSpread_, &pageItemList, 
								kFalse);	// don't include the page object itself
/*
	// PASS 1 -- Iterate the list of graphic frames and compute surrounding rects
	LOG ("<!-- PASS 1 -->");
	int32 countPageItemsToProcess = pageItemList.Length ();
	LOG_TAG_AND_VALUE ("PageItemsCount", countPageItemsToProcess);
*/
	// PASS 1 -- Iterate the list of graphic frames and compute surrounding rects
	LOG ("<!-- PASS 1 -->");
	int32 graphicFrameCount = picBoxUIDRefsVector_.size ();
	LOG_TAG_AND_VALUE ("GraphicFrameCount", graphicFrameCount);
	int n = 0;
	bool firstround = true;
	UIDRef pageItemUIDRef = UIDRef::gNull;
	vector<UIDRef>::iterator picFramesIter = picBoxUIDRefsVector_.begin ();
    while ( picFramesIter < picBoxUIDRefsVector_.end () )
    {
		UIDRef graphicFrameRef = *picFramesIter;
		ComputeRectanglesSurrounding_(graphicFrameRef, firstround);
    	++picFramesIter;
    }

#	if VERSION == IN_HOUSE && LOG_FIND_OPEN_RECTS
		LOG ("<!-- DUMP OF rectanglesVector_ after first pass: -->");
		for_each (rectanglesVector_.begin (), 
				rectanglesVector_.end (), dump_newsCAD_rect );
#	endif

	// PASS 2 -- check for intersections
	// For each existing pic box, examine each candidate rectangle and 
	// if the candidate rect intersects with the pic box, adjust 
	// the candidate rect's dimensions so there's no overlap.
	LOG ("<!-- PASS 2 -->");
	if (rectanglesVector_.size () == 0)					// Anything to do?
		return 0;										// Nope
	n = 0;												// ???
	vector<CL_NewsCadRect>::iterator p;
/*
	for (int32 j = 0; j < pageItemList.Length(); j++)
	{
		if (database->GetClass(pageItemList[j]) == kSplineItemBoss)
		{
			UIDRef objectUIDRef = pageItemList.GetRef (j);
			InterfacePtr<IFrameType> frameType (objectUIDRef, UseDefaultIID());
			if ( frameType->IsGraphicFrame () )
			{
				InterfacePtr<IGeometry> objectGeometry (
													objectUIDRef, UseDefaultIID());
				if (!objectGeometry) {LOG ("NO OBJECT GEOMETRY"); goto cleanup;}
				CL_NewsCadRect itemRect;
				ObjectRectToPageRect_(objectGeometry, itemRect);
				// For each candidate rectangle, see if it collides with 
				// the graphic box that's already on the page.
				// theFrect == candidate rectangle
				// itemRect == pic box rectangle
				p = rectanglesVector_.begin ();
				while (p < rectanglesVector_.end ())
				{
					CL_NewsCadRect candidateRect (p->Left (), p->Top (), 
													p->Right (), p->Bottom ());
					if ( IntersectOrAbut (candidateRect, itemRect) )
					{
						ProcessIntersection_(candidateRect, itemRect);
					}
					
					++p;
				}
			}
		}
	}
*/
	picFramesIter = picBoxUIDRefsVector_.begin ();
    while ( picFramesIter < picBoxUIDRefsVector_.end () )
    {
		UIDRef graphicFrameRef = *picFramesIter;
		InterfacePtr<IGeometry> objectGeometry (
											graphicFrameRef, UseDefaultIID());
		if (!objectGeometry) {LOG ("NO OBJECT GEOMETRY"); goto cleanup;}
		CL_NewsCadRect itemRect;
		ObjectRectToPageRect_(objectGeometry, itemRect);
		// For each candidate rectangle, see if it collides with 
		// the graphic box that's already on the page.
		// theFrect == candidate rectangle
		// itemRect == pic box rectangle
		p = rectanglesVector_.begin ();
		while (p < rectanglesVector_.end ())
		{
			CL_NewsCadRect candidateRect (p->Left (), p->Top (), 
											p->Right (), p->Bottom ());
			if ( IntersectOrAbut (candidateRect, itemRect) )
			{
				ProcessIntersection_(candidateRect, itemRect);
			}
			
			++p;
		}
    	++picFramesIter;
    }

#	if VERSION == IN_HOUSE && LOG_FIND_OPEN_RECTS
	LOG ("<!-- DUMP OF rectanglesVector_ after second pass: -->");
	for_each (rectanglesVector_.begin (), 
				rectanglesVector_.end (), dump_newsCAD_rect );
#	endif

	// PASS 3 -- Just delete any intersections that are left over from pass 2.
	LOG ("<!-- PASS 3 -->");
/*
	for (int32 j = 0; j < pageItemList.Length(); j++)
	{
		if (database->GetClass(pageItemList[j]) == kSplineItemBoss)
		{
			UIDRef objectUIDRef = pageItemList.GetRef (j);
			InterfacePtr<IFrameType> frameType (objectUIDRef, UseDefaultIID());
			if ( frameType->IsGraphicFrame () )
			{
				InterfacePtr<IGeometry> objectGeometry (
													objectUIDRef, UseDefaultIID());
				if (!objectGeometry) {LOG ("NO OBJECT GEOMETRY"); goto cleanup;}
				CL_NewsCadRect itemRect;
				ObjectRectToPageRect_(objectGeometry, itemRect);
				// For each candidate rectangle, see if it collides with 
				// the graphic box that's already on the page.
				// theFrect == candidate rectangle
				// itemRect == pic box rectangle
				p = rectanglesVector_.begin ();
				while (p < rectanglesVector_.end ())
				{
					CL_NewsCadRect candidateRect (p->Left (), p->Top (), 
													p->Right (), p->Bottom ());
					if ( IntersectOrAbut (candidateRect, itemRect) 			|| 
						 candidateRect.IsEmpty ()							||
						(candidateRect.Left () >= candidateRect.Right ())	||
						(candidateRect.Top () >= candidateRect.Bottom ())	||
						(candidateRect.Left () < left_)						||
						(candidateRect.Top () < top_)						||
						(candidateRect.Left () >= right_)					||
						(candidateRect.Right () <= left_)					||
						(candidateRect.Top () >= bottom_)					||
						(candidateRect.Bottom () <= top_)						)
					{
						// "Bad" rectangle -- remove it -- No need to close up 
						// the array, the STL will do that with the erase ()
						rectanglesVector_.erase (p);
					}
					else
					{
						++p;
					}
					// Exit now if we've eliminated all candidate rectangles.
					if ( rectanglesVector_.size () == 0 )
					{
						break;
					}
				}
			}
		}
	}
*/
	picFramesIter = picBoxUIDRefsVector_.begin ();
    while ( picFramesIter < picBoxUIDRefsVector_.end () )
    {
		UIDRef graphicFrameRef = *picFramesIter;
		InterfacePtr<IGeometry> objectGeometry (
											graphicFrameRef, UseDefaultIID());
		if (!objectGeometry) {LOG ("NO OBJECT GEOMETRY"); goto cleanup;}
		CL_NewsCadRect itemRect;
		ObjectRectToPageRect_(objectGeometry, itemRect);
		// For each candidate rectangle, see if it collides with 
		// the graphic box that's already on the page.
		// theFrect == candidate rectangle
		// itemRect == pic box rectangle
		p = rectanglesVector_.begin ();
		while (p < rectanglesVector_.end ())
		{
			CL_NewsCadRect candidateRect (p->Left (), p->Top (), 
											p->Right (), p->Bottom ());
			if ( IntersectOrAbut (candidateRect, itemRect) 			|| 
				 candidateRect.IsEmpty ()							||
				(candidateRect.Left () >= candidateRect.Right ())	||
				(candidateRect.Top () >= candidateRect.Bottom ())	||
				(candidateRect.Left () < left_)						||
				(candidateRect.Top () < top_)						||
				(candidateRect.Left () >= right_)					||
				(candidateRect.Right () <= left_)					||
				(candidateRect.Top () >= bottom_)					||
				(candidateRect.Bottom () <= top_)						)
			{
				// "Bad" rectangle -- remove it -- No need to close up 
				// the array, the STL will do that with the erase ()
				rectanglesVector_.erase (p);
			}
			else
			{
				++p;
			}
			// Exit now if we've eliminated all candidate rectangles.
			if ( rectanglesVector_.size () == 0 )
			{
				break;
			}
		}
    	++picFramesIter;
    }

#	if VERSION == IN_HOUSE && LOG_FIND_OPEN_RECTS
	::sprintf (logstr, "<!-- Found %d rects. -->", rectanglesVector_.size ());
	LOG (logstr);
	LOG ("<!-- DUMP OF rectanglesVector_ after third pass: -->");
	for_each (rectanglesVector_.begin (), 
				rectanglesVector_.end (), dump_newsCAD_rect );
#	endif

cleanup:
	LOG ("<!-- END OF CL_Page::FindOpenRects_() -->");
	LOG_END_FUNCTION;
	return rectanglesVector_.size ();		
}
#undef LOG_FOLIO


//----------------------------------------------------------------------------------
//	OLD_GetGraphicFrameUIDRefVector_
//
//	Fill the member variable with UIDRefs of the graphic frames on the page.
//	This is just for InDesign ... QuarkXPress constructs this list for XTensions.
//	In a plugin, you have to roll your own.
//----------------------------------------------------------------------------------
void CL_Page::OLD_GetGraphicFrameUIDRefVector_()
{
	LOG_BEGIN_FUNCTION;
	picBoxUIDRefsVector_.clear ();		// Init storage for graphic frames' UIDRef
	picBoxUIDRefsVector_.reserve (32);
	InterfacePtr<ISpread> ipSpread (spreadUIDRef_, UseDefaultIID());
	// Get front document
	InterfacePtr<IDocument> iDocument (
			Utils<ILayoutUIUtils>()->GetFrontDocument (), UseDefaultIID ());
	if ( !iDocument ) throw "could not get pointer to front document";
	IDataBase* database = ::GetDataBase (iDocument);
	if (!database) throw ("IDataBase");
	UIDList pageItemList (database);
	ipSpread->GetItemsOnPage (pagePosOnSpread_, &pageItemList, 
								kFalse);	// don't include the page object itself

	// Iterate the list of page items we just got...
	int32 countPageItemsToProcess = pageItemList.Length ();
	LOG_TAG_AND_VALUE ("PageItemsCount", countPageItemsToProcess);
	int32 graphicFrameCount = 0;
	UIDRef pageItemUIDRef = UIDRef::gNull;
    for (int32 j = countPageItemsToProcess - 1; j >= 0; j--)
    {
    	pageItemUIDRef = pageItemList.GetRef (j);

		// Get the hierarchy so we can check if this is a group
        InterfacePtr<IHierarchy> groupNode (pageItemUIDRef, IID_IHIERARCHY);
        if (groupNode && ::GetClass (groupNode) == kGroupItemBoss)
        {
        	// Drill down through the group and search for graphic frame(s).
        	LOG ("<!-- Start group -->");
        	LOG_TAG_AND_VALUE ("FrameType", "group");
			UIDRef graphicFrameRef;			// Safe -- CTOR inits UID to kInvalidUID
			UIDList leafnodes (database);
			Utils<IPathUtils>()->GetPathSelectableUIDList (&leafnodes, groupNode);
			int32 leafLength = leafnodes.Length ();
			LOG_TAG_AND_VALUE ("GroupItemCount", leafLength);
			for (int i = leafLength - 1; i >= 0; i--)
			{
				LOG ("<!-- +++ Processing item +++ -->");
				if ( Utils<IPageItemTypeUtils>()->IsTextFrame (
						leafnodes.GetRef (i) ) )
				{
					LOG_TAG_AND_VALUE ("ItemType", "text frame");
				}
				else if (database->GetClass (leafnodes[i]) == kSplineItemBoss)
				{
					LOG_TAG_AND_VALUE ("ItemType", "kSplineItemBoss");
					InterfacePtr<IHierarchy> splineHier (
										leafnodes.GetRef (i), IID_IHIERARCHY);
					if (splineHier)
					{
						int ii = 0;
						int childCount = splineHier->GetChildCount();
						if (childCount == 0)
						{
							LOG ("<!-- Spline item has no children -->");

							// We're going to treat as graphic frame:
							LOG_TAG_AND_VALUE ("ItemType", "graphic frame");
							++graphicFrameCount;
							graphicFrameRef = leafnodes.GetRef (i);
							picBoxUIDRefsVector_.push_back (graphicFrameRef);
						}
						else
						{
							while (ii < childCount)
							{
								InterfacePtr<IHierarchy> splineChildHier (
													splineHier->QueryChild(ii));
								if (splineChildHier)
								{
									if (::GetClass (splineChildHier) == 
															kMultiColumnItemBoss)
									{
										LOG_TAG_AND_VALUE ("ItemType", 
															"kMultiColumnItemBoss");
									}
									else
									{
										InterfacePtr<IGraphicFrameData> 
											graphicFrameData (splineChildHier, 
											IID_IGRAPHICFRAMEDATA);
										if (graphicFrameData && 
												graphicFrameData->IsGraphicFrame())
										{
											LOG_TAG_AND_VALUE ("ItemType",
																"graphic frame");
											// We're going to treat as graphic:
											LOG_TAG_AND_VALUE ("ItemType", 
																"graphic frame");
											++graphicFrameCount;
											graphicFrameRef = 
															leafnodes.GetRef (i);
											picBoxUIDRefsVector_.push_back (
																graphicFrameRef);
										}
									}
								}
								++ii;
							}
						}
					}
				}
				else
				{
					// EMPTY
				}
				LOG ("<!-- *** Finished processing item *** -->");
			}
        	LOG ("<!-- End group -->");
        }
        else
        {
        	// See if it's a graphic frame not in a group
        	// TBD: THIS LOOKS BUGGY
			InterfacePtr<IFrameType> frameType (pageItemUIDRef, UseDefaultIID());
			if ( frameType && frameType->IsGraphicFrame () )
			{
				LOG_TAG_AND_VALUE ("FrameType", "graphic");
				++graphicFrameCount;
				picBoxUIDRefsVector_.push_back (pageItemUIDRef);
        	}
        }
	}
	LOG_TAG_AND_VALUE ("GraphicFrameCount", graphicFrameCount);

	LOG_END_FUNCTION;
	// END OF OLD_GetGraphicFrameUIDRefVector_()
}


//----------------------------------------------------------------------------------
//	GetGraphicFrameUIDRefVector_
//
//	Fill the member variable with UIDRefs of the graphic frames on the page.
//	This is just for InDesign ... QuarkXPress constructs this list for XTensions.
//	In a plugin, you have to roll your own.
//----------------------------------------------------------------------------------
void CL_Page::GetGraphicFrameUIDRefVector_()
{
	LOG_BEGIN_FUNCTION;

	picBoxUIDRefsVector_.clear ();		// Init storage for graphic frames' UIDRef
	picBoxUIDRefsVector_.reserve (32);
	InterfacePtr<ISpread> ipSpread (spreadUIDRef_, UseDefaultIID());
	// Get front document
	InterfacePtr<IDocument> iDocument (
			Utils<ILayoutUIUtils>()->GetFrontDocument (), UseDefaultIID ());
	if ( !iDocument ) throw "could not get pointer to front document";
	IDataBase* database = ::GetDataBase (iDocument);
	if (!database) throw ("IDataBase");
	UIDList pageItemList (database);
	ipSpread->GetItemsOnPage (pagePosOnSpread_, &pageItemList, 
								kFalse);	// don't include the page object itself

	// Iterate the list of page items we just got...
	int32 countPageItemsToProcess = pageItemList.Length ();
	LOG_TAG_AND_VALUE ("PageItemsCount", countPageItemsToProcess);
	int32 graphicFrameCount = 0;
	UIDRef pageItemUIDRef = UIDRef::gNull;
	for (int32 j = countPageItemsToProcess - 1; j >= 0; j--)
	{
		pageItemUIDRef = pageItemList.GetRef (j);
		InterfacePtr<IHierarchy> pageItemHierarchy (
											pageItemUIDRef, UseDefaultIID());
		ProcessPageItem_(pageItemList[j], pageItemUIDRef, pageItemHierarchy);
	}

	LOG ("<!-- END OF GetGraphicFrameUIDRefVector_() -->");
	LOG_END_FUNCTION;
	// END OF GetGraphicFrameUIDRefVector_()
}




/*================================================================================*/
void CL_Page::ProcessPageItem_(UID inItemUID, UIDRef inPageItemUIDRef, 
											IHierarchy* inPageItemIHierarchy) {
/*----------------------------------------------------------------------------------
	Abstract:
		Given a page item, check it and its children and discover whether it's 
		an item that the program is allowed to overprint with an ad.  This function 
		calls itself recursively for each child of the passed-in item.
		
	Parameters and modes:
		inItemUID			An InDesign Unique IDentifer for some page item.	IN
		inPageItemUIDRef	Combination of UID and document database ID.		IN
		inPageItemIHierarchy
							Pointer to children of the item.					IN

	Returns:

	Change Log:
		05-JUN-07	New.  Ripped off from CL_Ad_Reporter.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;

	if (!inPageItemIHierarchy)
		return;
	
	DECLARE_logstr;
	::sprintf (logstr, "<PageItemUid>0x%x</PageItemUid>", inItemUID.Get());
	LOG (logstr);
	
	// We're interested only in what the InDesign people call "spline" items -- 
	// we don't care about group items or whatever else they have.
	if ( ::GetClass (inPageItemIHierarchy) == kSplineItemBoss )
    {
		if (Utils<IPageItemTypeUtils>()->IsTextFrame (inPageItemIHierarchy))
		{
			// It's OK to overprint this item with an ad.
			LOG ("<!-- Object is a TEXT frame. IGNORE. -->");
		}
		else
		{
			// We cannot overprint this object (whatever it is) with an ad.
			LOG ("<!-- Object is NOT a TEXT frame -->");
			picBoxUIDRefsVector_.push_back (inPageItemUIDRef);
		}
    }
    
    // Recursively process the passed-in item's children.
	int32 childCount = inPageItemIHierarchy->GetChildCount ();
	for (int32 childIndex = 0; childIndex < childCount; childIndex++)
	{
		UID childUID = inPageItemIHierarchy->GetChildUID (childIndex);
		// Get front document
		InterfacePtr<IDocument> iDocument (
				Utils<ILayoutUIUtils>()->GetFrontDocument (), UseDefaultIID ());
		if ( !iDocument ) throw "could not get pointer to front document";
		UIDRef childUIDRef (::GetDataBase (iDocument), childUID );
		InterfacePtr<IHierarchy> child (
								inPageItemIHierarchy->QueryChild (childIndex));
		ProcessPageItem_(childUID, childUIDRef, child);
	}

	LOG_END_FUNCTION;
}


//----------------------------------------------------------------------------------
//	ObjectRectToPageRect
//
//	Given a geometry interface for an object, compute its location on the page 
//	where the top, left of the trim is at 0,0.
//----------------------------------------------------------------------------------
void CL_Page::ObjectRectToPageRect_(
				const IGeometry* inObjectGeometry, CL_NewsCadRect & outResult) const
{
/*
	PMMatrix identity;
	PMRect bBoxInner = inObjectGeometry->GetStrokeBoundingBox (identity);
	gpLogFile->LogPMRect_("Inner", bBoxInner);
	
	PMRect bBoxParent = inObjectGeometry->GetStrokeBoundingBox (
							::InnerToParentMatrix(inObjectGeometry));
	gpLogFile->LogPMRect_("Parent", bBoxParent);
*/
	// A PBPMRect is nothing more than a typedef PMRect -- represents a rect in 
	// "PasteBoard" coordinates.
	PBPMRect bBoxPasteboard = inObjectGeometry->GetStrokeBoundingBox (
							::InnerToPasteboardMatrix (inObjectGeometry));
/*	gpLogFile->LogPMRect_("Pasteboard", bBoxPasteboard);*/
	
	InterfacePtr<IGeometry> pageGeometry (uidRef_, UseDefaultIID());
	if (!pageGeometry) {LOG ("NO PAGE GEOMETRY"); return;}
/*
	PMRect bBoxPage = pageGeometry->GetStrokeBoundingBox (identity);
	gpLogFile->LogPMRect_("Page Inner", bBoxPage);
*/
	PBPMRect bBoxPagePasteboard = pageGeometry->GetStrokeBoundingBox (
										::InnerToPasteboardMatrix (pageGeometry));
/*	gpLogFile->LogPMRect_("Page Pasteboard", bBoxPagePasteboard);*/
	
	// Translate object's x1,y1
	PMReal left = bBoxPasteboard.Left () - bBoxPagePasteboard.Left ();
	PMReal top  = bBoxPasteboard.Top () - bBoxPagePasteboard.Top ();
	
	// Compute width and height of object
	PMReal width = bBoxPasteboard.Right () - bBoxPasteboard.Left ();
	PMReal height = bBoxPasteboard.Bottom () - bBoxPasteboard.Top ();
	
	// Set x1,y1 and x2,y2
	outResult.Left   ( left );
	outResult.Top    ( top );
	outResult.Right  ( left + width );
	outResult.Bottom ( top + height );

	gpLogFile->LogPMRect_("Item Page Rect", outResult);
}

//----------------------------------------------------------------------------------
//	ComputeAndStoreRectangleToRightOf_
//
//	Given a bounding rectangle for some object in page coordinates, compute the 
//	page rectangle that lies to the right of it, presumably as a candidate for 
//	containing an ad.  Store the results in the rectanglesVector_ member.
//----------------------------------------------------------------------------------
void CL_Page::ComputeAndStoreRectanglesToRightOf_(CL_NewsCadRect & inItemsPageRect)
{
	// Create rectangles to RIGHT of found box.
	if ( (right_ - inItemsPageRect.Right ()) > gutterWidth_ )
	{
		// Set height to HEIGHT OF LIVE AREA, 
		// Left == ITEM'S RIGHT + gutter
		PMReal left = inItemsPageRect.Right () + gutterWidth_;
		CL_NewsCadRect r1 (left, top_, right_, bottom_);
		rectanglesVector_.push_back (r1);
		
		// Set top to top of ITEM, right & bottom 
		// to right and bottom of LIVE AREA.
		CL_NewsCadRect r2 (left, inItemsPageRect.Top (), right_, bottom_);
		rectanglesVector_.push_back (r2);
	}
}

//----------------------------------------------------------------------------------
//	ComputeAndStoreRectangleAbove_
//
//	Given a bounding rectangle for some object in page coordinates, compute the 
//	page rectangle that lies above it, presumably as a candidate for 
//	containing an ad.  Store the results in the rectanglesVector_ member.
//----------------------------------------------------------------------------------
void CL_Page::ComputeAndStoreRectanglesAbove_(CL_NewsCadRect & inItemsPageRect)
{
	if ( ((inItemsPageRect.Top ()) - top_) > gutterWidth_)
	{
		// Left == FOUND BOX LEFT, top == TOP OF LIVE AREA,
		// Right == FOUND BOX RIGHT, bottom == FOUND BOX TOP - gutter
		CL_NewsCadRect r1 (inItemsPageRect.Left (), top_, 
							inItemsPageRect.Right ()/*????+thePage.gutterWidth*/, 
							inItemsPageRect.Top () - gutterWidth_);
		/*????if (theFRect.right == thePage.right)
		 theFRect.right = theFRect.right+thePage.gutterWidth;*/
		rectanglesVector_.push_back (r1);

		// Left == LIVE AREA LEFT, top == LIVE AREA TOP, 
		// Right == LIVE AREA RIGHT, bottom == FOUND BOX TOP - gutter
		CL_NewsCadRect r2 (left_, top_, right_, 
							inItemsPageRect.Top () - gutterWidth_);
		rectanglesVector_.push_back (r2);
	}
}


//----------------------------------------------------------------------------------
//	ComputeAndStoreRectangleToLeftOf_
//
//	Given a bounding rectangle for some object in page coordinates, compute the 
//	page rectangle that lies to the left of it, presumably as a candidate for 
//	containing an ad.  Store the results in the rectanglesVector_ member.
//----------------------------------------------------------------------------------
void CL_Page::ComputeAndStoreRectanglesToLeftOf_(CL_NewsCadRect & inItemsPageRect)
{
	if ( (inItemsPageRect.Left () - left_) > gutterWidth_ )
	{
		// Left == LIVE AREA LEFT, top == LIVE AREA TOP, 
		// Right == FOUND BOX LEFT - gutter, bottom == LIVE AREA BOTTOM
		CL_NewsCadRect r1 (left_, top_, 
							inItemsPageRect.Left () - gutterWidth_, bottom_);
		rectanglesVector_.push_back (r1);
		
		// Left == LIVE AREA LEFT, top == FOUND BOX TOP, 
		// Right == FOUND BOX LEFT - gutter, bottom == LIVE AREA BOTTOM
		CL_NewsCadRect r2 (left_, inItemsPageRect.Top (), 
							inItemsPageRect.Left () - gutterWidth_, bottom_);
		rectanglesVector_.push_back (r2);
	}
}


//----------------------------------------------------------------------------------
//	ComputeAndStoreRectanglesBelow_
//
//	Given a bounding rectangle for some object in page coordinates, compute the 
//	page rectangle that lies below it, presumably as a candidate for 
//	containing an ad.  Store the results in the rectanglesVector_ member.
//----------------------------------------------------------------------------------
void CL_Page::ComputeAndStoreRectanglesBelow_(CL_NewsCadRect & inItemsPageRect)
{
	// If the item's rectangle is less than or equal to a page gutter width 
	// away from the bottom of the page, there's no room for an ad, so exit.
	if (bottom_ - inItemsPageRect.Bottom () <= gutterWidth_)
		return;
	
	// Create rectangle to the RIGHT of the found box (what, again)?
	if (right_ - inItemsPageRect.Right () > gutterWidth_)
	{
		// Left == FOUND BOX RIGHT + gutter, top == TOP OF LIVE AREA,
		// Right == RIGHT OF LIVE AREA, bottom == FOUND BOX BOTTOM
		CL_NewsCadRect r1 ( inItemsPageRect.Right () + gutterWidth_,
							top_, right_, inItemsPageRect.Bottom () );
		rectanglesVector_.push_back (r1);
	}
	
	// Create rectangle BELOW the found box.
	// Left == LIVE AREA LEFT, top == FOUND BOX BOTTOM + gutter,
	// Right == LIVE AREA RIGHT, bottom == LIVE AREA BOTTOM
	CL_NewsCadRect r2 ( left_, inItemsPageRect.Bottom () + gutterWidth_, 
						right_, bottom_);
	rectanglesVector_.push_back (r2);
	
	// Create yet another rectangle BELOW the found box.
	if ( left_ != inItemsPageRect.Left () )
	{
		// Left == FOUND BOX LEFT, top == FOUND BOX BOTTOM + gutter,
		// Right == FOUND BOX RIGHT, bottom == LIVE AREA BOTTOM
		CL_NewsCadRect r3 (
					inItemsPageRect.Left (), 
					inItemsPageRect.Bottom () + gutterWidth_, 
					inItemsPageRect.Right () /*????+thePage.gutterWidth*/, 
					bottom_);
		rectanglesVector_.push_back (r3);
	}
}


//----------------------------------------------------------------------------------
//	ProcessIntersection_
//
//	This function adjusts a candidate (for ad placement) rectangle so that it 
//	does not collide with the rect of an existing item.
//----------------------------------------------------------------------------------
void CL_Page::ProcessIntersection_(CL_NewsCadRect & candidateRect, 
									CL_NewsCadRect & itemRect)
{
	if ( (candidateRect.Bottom () == bottom_) && (candidateRect.Top () == top_))
	{
		// The candidate rectangle's top and bottom are 
		// at the page live area top and bottom ...
		if ((candidateRect.Bottom () - itemRect.Bottom ()) <
					(itemRect.Top () - candidateRect.Top () ) )
			// The ad is closer to the bottom of the page
			// Adjust candidate rectangle so it's above the ad
			candidateRect.Bottom (itemRect.Top () - gutterWidth_);
		else
			// Adjust candidate rectangle so it's below the ad
			candidateRect.Top (itemRect.Bottom () + gutterWidth_);
	}
	else if ((candidateRect.Left () == left_) && (candidateRect.Right () >= right_))
	{
		// The candidate rectangle's left and right are 
		// at the page live area left and right ...
		if ((candidateRect.Right () - itemRect.Right ()) <
				(itemRect.Left () - candidateRect.Left () ))
			// The ad is closer to the right of the page
			// Adjust candidate rectangle so it's to the left of the ad
			candidateRect.Right (itemRect.Left () - gutterWidth_);
		else if (itemRect.Left () <= candidateRect.Left ())
			// Adjust candidate rectangle so it's to the right of the ad
			candidateRect.Left (itemRect.Right () + gutterWidth_);
	}
	else if (   (candidateRect.Left () == left_)			&& 
				(itemRect.Right () < candidateRect.Right ())	&&
				(candidateRect.Right () <= right_))
	{
		// The candidate rectangle's left and right are at the page live area left 
		// and right.  Move the candidate's left edge to the right of the ad.
		candidateRect.Left (itemRect.Right () + gutterWidth_);
	}
	else if ((candidateRect.Right () >= right_) &&
				(itemRect.Left () > candidateRect.Left ()))
	{
		// Right side of candidate rectangle is beyond the right side of page live 
		// area and the ad's left edge is to the right of the candidate rectangle's 
		// left edge.  Adjust candidate rectangle so it's at left of ad.
		candidateRect.Right (itemRect.Left () - gutterWidth_);
	}
	else if (candidateRect.Top () == top_)
	{
		// Candidate rectangle's top is at page top -- Move its top below the ad
		candidateRect.Top (itemRect.Bottom () + gutterWidth_);
	}
	else if (candidateRect.Bottom () == bottom_)
	{
		// Candidate rectangle's bottom is at page bottom
		// Move its bottom above the ad
		candidateRect.Bottom (itemRect.Top () - gutterWidth_);
	}
	else if ((candidateRect.Bottom () - itemRect.Bottom ()) < 
					(itemRect.Top () - candidateRect.Top ()))
	{
		// Distance between candidate rectangle's bottom and ad's bottom is less 
		// than distance between ad's top and candidate rectangle's top.  
		// Put candidate rectangle above the ad.
		candidateRect.Bottom (itemRect.Top () - gutterWidth_);
	}
	else
	{
		// Put candidate rectangle below the ad.
		candidateRect.Top (itemRect.Bottom () + gutterWidth_);
	}
}


/*================================================================================*/
void CL_Page::PlaceAds_(vector<CL_Display_Ad> & adsToPlaceVector, 
						vector<CL_NewsCadRect> & newsCadRects) {
/*----------------------------------------------------------------------------------
	Abstract:
		This method is a generalized version of PasteUp_().  That method is a "do 
		it all" kind of thing -- it prioritizes rectangles, finds ads to fit them, 
		and computes new rectangles in a big loop.  This method just places ads -- 
		it calls methods in derived classes for creating and sorting rectangles to 
		reflect the changing geometry of the page as ads get pasted up.  The idea 
		is to put the basic stuff (finding ads to fit rectangles, creating text 
		boxes to contain ads, etc.) in 1 place, and allow derived classes to provide 
		code to customize the how and where.
		
	Parameters and modes:
		adsToPlaceVector	Array of ads allowed to go on this page.		IN/OUT
		newCadRects			Open rects on the page in which the ads 		IN/OUT
							can be placed.
	Returns:

	Change Log:
		13-Sep-05	Began port to InDesign.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	LOG_END_FUNCTION;
}

/*================================================================================*/
void CL_Page::PasteUp_(vector<CL_Display_Ad> & adsToPlaceVector) {
/*----------------------------------------------------------------------------------
	Abstract:
		This method places ads on this page.  It replaces the C function 
		FitAdsToPage() in the old product (old source: sd.c).

	Parameters and modes:
		An array of candidate ads to attempt to place on this page.

	Returns:
		NOT SURE YET

	Change Log:
		25-Oct-05	Port to InDesign.
		15-Sep-02	New.
----------------------------------------------------------------------------------*/
// 	extern bool Compare_NewsCadRects_Area (
// 					const CL_NewsCadRect a, const CL_NewsCadRect b);
// 	extern void dump_ad (CL_Display_Ad ad);

	// There is no code in this function right now, because Hemmings Motor News is 
	// our only InDesign client.  This function would provide basic functionality 
	// for all clients, but since HMN is the only client, and HMN requires special 
	// processing, I haven't bothered to port the basic functionality.  HMN pages 
	// are made up in HM_Page::PasteUp_(), which obviously overrides this function.

	LOG_BEGIN_FUNCTION;
	LOG ("<Error>You MUST override this function for Hemmings!</Error>");
	LOG_END_FUNCTION;
}


//------------------------------------------------------------------------------
//	InitializeOpenRects_
//
//	Copy all the rectangles in the member variable rectanglesVector_ into the 
//	passed-in vector referenced by inVecNewsCadRects.  Then sort 
//	inVecNewsCadRects.
//------------------------------------------------------------------------------
void CL_Page::InitializeOpenRects_(vector<CL_NewsCadRect> & inVecNewsCadRects)
{
	LOG_BEGIN_FUNCTION;
	inVecNewsCadRects.reserve (128);
	vector<CL_NewsCadRect>::iterator p = rectanglesVector_.begin ();
	CL_NewsCadRect r;
	while (p < rectanglesVector_.end ())
	{
		r = *p;
		inVecNewsCadRects.push_back (r);
		++p;
	}

	//-- Sort the recursives (rectangles available) by the square space.
	//-- since this is the first sort, it is the highest priority.
	sort ( inVecNewsCadRects.begin (), inVecNewsCadRects.end (), 
			Compare_NewsCadRects_Area );
#if VERSION == IN_HOUSE
	LOG ("<!-- DUMP OF newsCadRects VECTOR after sorting on area: -->");
	for_each (inVecNewsCadRects.begin (), 
					inVecNewsCadRects.end (), dump_newsCAD_rect );
#endif

	LOG_END_FUNCTION;
}


#if 0
/*================================================================================*/
int CL_Page::FindBestFit_(CL_NewsCadRect & candidateRectangle, 
								vector<CL_Display_Ad> & adsToPlaceVector) {
/*----------------------------------------------------------------------------------
	Abstract:
		Search the adsToPlaceVector for an ad that can fit into the 
		candidateRectangle.  The caller probably passed in the largest rectangle 
		in the vector as the candidate, but it doesn't matter.  Return index into 
		ad vector of the ad that fits.  Return -1 if no ad found to fit the 
		hole in the page.
		
	Parameters and modes:
		candidateRectangle	A piece of page real estate that's not currently 	IN
							occupied by an ad.
		adsToPlaceVector	STL container of ads, any of which are allowed 		IN
							in the open real estate.
	Returns:
		Index into adsToPlaceVector of ad that's to be pasted up in the 
		candidateRectangle, otherwise, -1 if no ad can go there.
		
	Change Log:
		22-Oct-05	Began port to InDesign.
----------------------------------------------------------------------------------*/
#define TRACE_FindBestFit	1
#if VERSION == IN_HOUSE && TRACE_FindBestFit
	#define	LOG_FOLIO															\
		::sprintf (logstr, "<page folio=\"%d\" index=\"%d\"/>", 				\
														folio_, pageIndex_);	\
		LOG (logstr);
	#define	LOG_CANDIDATE_RECT	if (gpLogFile)									\
								{												\
									gpLogFile->LogPMRect_(						\
									"Candidate Rect", candidateRectangle);		\
								}

#else
	#define	LOG_FOLIO					NULL
	#define	LOG_CANDIDATE_RECT			NULL
#endif
	LOG_BEGIN_FUNCTION;
	DECLARE_logstr;
	LOG_FOLIO;
	LOG_CANDIDATE_RECT;

	bool foundAnAdThatFits = false;
	vector<CL_Display_Ad>::iterator p = adsToPlaceVector.begin ();
	int i = 0;
	string adColor;

	while ( p < adsToPlaceVector.end () )
	{
		// If this is a color page and this ad is color OR this is not a
		// color page then proceed to see if it will fit
		p->GetColor_(adColor);
		if ( adColor == "BW" || adColor == "" ||
			(isColorPage_ && adColor == "SC") ||  
			(isColorPage_ && adColor == "4C") ) 
		{ 
			if (p->GetWidth_()  <= (candidateRectangle.Width ()/*  + 16384*/) 	&& 
				p->GetHeight_() <= (candidateRectangle.Height ()/* + 16384*/)	&&
				!p->GetPlaced_() && PageHasRoomForAd_(p) )
			{
				// Ad fits
				foundAnAdThatFits = true;
				break;
			}
			else
			{
#if VERSION == IN_HOUSE && TRACE_FindBestFit == 1
//				RecordReasonAdRejected_(p, candidateRectangle, false);
string aString = "frank";
p->GetArtFileName_(aString);
LOG_TAG_AND_VALUE ("ArtFileNameOfAdRejected", aString.c_str ());
#endif			
				// Next!
				++i;
				++p;
			}
		}
		else
		{
			// Rejected for color
#if VERSION == IN_HOUSE && TRACE_FindBestFit == 1
//			RecordReasonAdRejected_(p, candidateRectangle, true);
string aString = "frank";
p->GetArtFileName_(aString);
LOG_TAG_AND_VALUE ("ArtFileNameOfAdRejected", aString.c_str ());
#endif			
			// Next!
			++i;
			++p;
		}
	}
	
	if (!foundAnAdThatFits)
		i = -1;						// Gross little flag value

	LOG_TAG_AND_VALUE ("FindBestFit_returned", i);

	LOG_END_FUNCTION;
	return i;
}
#endif
/*================================================================================*/
int CL_Page::FindBestFit_(CL_NewsCadRect & candidateRectangle, 
								vector<CL_Display_Ad> & adsToPlaceVector) {
/*----------------------------------------------------------------------------------
	Abstract:
		Search the adsToPlaceVector for an ad that can fit into the 
		candidateRectangle.  The caller probably passed in the largest rectangle 
		in the vector as the candidate, but it doesn't matter.  Return index into 
		ad vector of the ad that fits.  Return -1 if no ad found to fit the 
		hole in the page.
		
	Parameters and modes:
		candidateRectangle	A piece of page real estate that's not currently 	IN
							occupied by an ad.
		adsToPlaceVector	STL container of ads, any of which are allowed 		IN
							in the open real estate.
	Returns:
		Index into adsToPlaceVector of ad that's to be pasted up in the 
		candidateRectangle, otherwise, -1 if no ad can go there.
		
	Change Log:
		22-Oct-05	Began port to InDesign.
----------------------------------------------------------------------------------*/
#define TRACE_FindBestFit	1
#if VERSION == IN_HOUSE && TRACE_FindBestFit
	#define	LOG_FOLIO															\
		::sprintf (logstr, "<page folio=\"%d\" index=\"%d\"/>", 				\
														folio_, pageIndex_);	\
		LOG (logstr);
	#define	LOG_CANDIDATE_RECT	if (gpLogFile)									\
								{												\
									gpLogFile->LogPMRect_(						\
									"Candidate Rect", candidateRectangle);		\
								}

#else
	#define	LOG_FOLIO					NULL
	#define	LOG_CANDIDATE_RECT			NULL
#endif
	LOG_BEGIN_FUNCTION;
	DECLARE_logstr;
	LOG_FOLIO;
	LOG_CANDIDATE_RECT;

	bool foundAnAdThatFits = false;
	vector<CL_Display_Ad>::iterator p = adsToPlaceVector.begin ();
	int i = 0;
	string adColor;
	const char* artFileNameCStr;

	while ( p < adsToPlaceVector.end () )
	{
		LOG_TAG_AND_VALUE ("AdName", p->GetArtFileNameCStr_());

		// If this is a color page and this ad is color OR this is not a
		// color page then proceed to see if it will fit
		p->GetColor_(adColor);
		if ( adColor == "BW" || adColor == "" ||
			(isColorPage_ && adColor == "SC") ||  
			(isColorPage_ && adColor == "4C") ) 
		{
			// Ad passes for color.
			if ( p->Width_() <= candidateRectangle.Width () )
			{
				// Ad's width is OK.
				if ( p->GetHeight_() <= candidateRectangle.Height () )
				{
					// Ad's height is OK.
					if (!p->GetPlaced_())
					{
						// Ad hasn't been placed yet.
						// See if it can fit on the current page.
						CL_Display_Ad ad = *p;
						if (PageHasRoomForAd_(ad))
						{
							// Ad fits
							foundAnAdThatFits = true;
							break;
						}
						else
						{
							#if VERSION == IN_HOUSE && TRACE_FindBestFit == 1
							LOG_TAG_AND_VALUE ("ReasonAdRejected", 
													"Would exceed coverage");
							#endif			
							// Next!
							++i;
							++p;
						}
					}
					else
					{
						#if VERSION == IN_HOUSE && TRACE_FindBestFit == 1
						LOG_TAG_AND_VALUE ("ReasonAdRejected", "Already placed");
						#endif			
						// Next!
						++i;
						++p;
					}
				}
				else
				{
					#if VERSION == IN_HOUSE && TRACE_FindBestFit == 1
					LOG_TAG_AND_VALUE ("ReasonAdRejected", "Height");
					#endif			
					// Next!
					++i;
					++p;
				}
			}
			else
			{
				#if VERSION == IN_HOUSE && TRACE_FindBestFit == 1
				LOG_TAG_AND_VALUE ("ReasonAdRejected", "Width");
				#endif			
				// Next!
				++i;
				++p;
			}
		}
		else
		{
			// Rejected for color
			#if VERSION == IN_HOUSE && TRACE_FindBestFit == 1
			LOG_TAG_AND_VALUE ("ReasonAdRejected", 
							"Ad is SC or 4C and this page is BW.");
			LOG_TAG_AND_VALUE ("AdName", p->GetArtFileNameCStr_());
			#endif			
			// Next!
			++i;
			++p;
		}
	}
	
	if (!foundAnAdThatFits)
		i = -1;						// Gross little flag value

	LOG_TAG_AND_VALUE ("FindBestFit_returned", i);

	LOG_END_FUNCTION;
	return i;
}
#undef	LOG_FOLIO
#undef	LOG_CANDIDATE_RECT


//----------------------------------------------------------------------------------
//	RecordReasonAdRejected_
//----------------------------------------------------------------------------------
void CL_Page::RecordReasonAdRejected_(
					vector<CL_Display_Ad>::iterator p, 
					CL_NewsCadRect & ncr, bool rejectedForColor)
{
#if VERSION == IN_HOUSE
	LOG_BEGIN_FUNCTION;
	const char* artFileName = p->GetArtFileNameCStr_();
/*
	p->GetArtFileName_(*pArtFileName);
*/
	LOG_TAG_AND_VALUE ("ArtFileName", artFileName);
/*
	if (p->GetPlaced_())
	{
		LOG_TAG_AND_VALUE ("ReasonAdRejected", "Ad has already been placed.");
		LOG_END_FUNCTION;
		return;
	}
//DebugStr ("\pOK  p->GetPlaced_()");
	if (rejectedForColor)
	{
		LOG_TAG_AND_VALUE ("ReasonAdRejected", 
							"Ad is SC or 4C and this page is BW.");
		LOG_END_FUNCTION;
		return;
	}
//DebugStr ("\pOK  rejectedForColor");
	if ( ! (PageHasRoomForAd_(p)) )
	{
		LOG_TAG_AND_VALUE ("ReasonAdRejected", 
							"Ad coverage threshhold would be exceeded.");
		LOG_END_FUNCTION;
		return;
	}
//DebugStr ("\pOK  PageHasRoomForAd_(p)");
	if ( ! (p->GetWidth_() <= ncr.Width ()) )
	{
		LOG_TAG_AND_VALUE ("ReasonAdRejected", 
							"Width of ad is greater than width of open rectangle.");
	}
//DebugStr ("\pOK  (p->GetWidth_() <= ncr.Width ())");
	if ( ! (p->GetHeight_() <= ncr.Height ()) )
	{
		LOG_TAG_AND_VALUE ("ReasonAdRejected", 
							"Depth of ad is greater than depth of open rectangle.");
	}
//DebugStr ("\pOK  (p->GetHeight_() <= ncr.Height ())");
	LOG_END_FUNCTION;
*/
#endif
}


/*================================================================================*/
void CL_Page::UpdateRectanglesVector_(int inAdIndex, 
										vector<CL_NewsCadRect> & newsCadRects, 
										vector<CL_Display_Ad> & adsToPlaceVector) {
/*----------------------------------------------------------------------------------
	Abstract:
		Immediately after an ad has been placed, PasteUp_() calls this method to 
		cycle through the remaining rectangles on the page to invalidate / extend 
		those that were intersected, thereby creating a new set extending from 
		the boundaries of the ad.

	Parameters and modes:
		newsCadRects		Ref to a vector of rects which can hold an ad.	IN/OUT
		adsToPlaceVector	Vector of ads to paste up on this page.			IN
	Returns:

	Change Log:
		25-Oct-05	Began port to InDesign.
----------------------------------------------------------------------------------*/
	bool eraseCurrNewsCadRect = false;

	// Fetch and store rectangle of the just-placed ad.
	CL_NewsCadRect adRect;
	adsToPlaceVector[inAdIndex].GetQuarkRect_(&adRect);

	vector<CL_NewsCadRect>::iterator p = newsCadRects.begin ();
	while (p < newsCadRects.end ())
	{
		CL_NewsCadRect candidateRect = *p;		// Fetch next NewsCadRect in vector

		// does the ad intersect with the current rectangle?
		// NOTE--COMPUTERWORLD CODE CHECKS FOR WHETHER RECTANGLE IS CLOSER 
		// THAN A PAGE GUTTER WIDTH TO THE JUST-PLACED AD RATHER THAN 
		// JUST SIMPLE INTERSECTION, SO MORE CHANGES MAY BE ON THE WAY HERE.
		if ( CL_NewsCadRect::FRectinFRect_(candidateRect, adRect) )
		{
			// Adjust current rectangle by removing space consumed by ad.
			candidateRect.Bottom (adRect.Top () - gutterWidth_);
			*p = candidateRect;

			// If the rectangle's new height is less than the gutter width of 
			// the page, then it won't hold an ad, so remove it from our vector.
			if (p->Height () <= gutterWidth_)
			{
				eraseCurrNewsCadRect = true;
			}
		}

		if (eraseCurrNewsCadRect)
		{
			newsCadRects.erase (p);
			eraseCurrNewsCadRect = false;
		}
		else
		{
			++p;
		}
	}
}


/*================================================================================*/
bool CL_Page::RectangleOverprints_(CL_NewsCadRect* inCandidateFRectPtr) {
/*----------------------------------------------------------------------------------
	Abstract:
		See if the candidate rectangle intersects any boxes on the page.  
		If it does, then placing an ad in that rectangle would cause the 
		ad to overprint another ad already on the page.
----------------------------------------------------------------------------------*/
	bool			retval = false;
	LOG_BEGIN_FUNCTION;
	LOG_END_FUNCTION;
	return retval;
}


/*================================================================================*/
bool CL_Page::PageHasRoomForAd_(vector<CL_Display_Ad>::iterator pAd) {
/*----------------------------------------------------------------------------------
	Abstract:
		Determine whether pAd can fit on this page without exceeding the 
		threshhold for area occupied by ads as defined in the preferences file.  
		This is part of the Cape Gazette enhancements.
				
	Parameters and modes:
		pAd		Pointer to the ad caller is trying to fit on the page.			IN
		
	Returns:
		If the ad in question can possibly fit on the page without exceeding 
		the threshhold for area occupied by ads, return true, else false.

	Change Log:
		22-Oct-05	Began port to InDesign.
		03-Oct-02	Began rewrite from the old 68K code.
		21-Apr-01	New.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	ASSERT (pGeometryImporter);

	// If no ad has been placed on the page yet, pass this one through.  This 
	// is to ensure that even if the first candidate ad exceeds the threshhold, 
	// it will be placed.  How else would the program place a full page ad, if 
	// the maximum coverage were, say, 50%?
	PMReal pageAreaConsumedByAds = 
							pGeometryImporter->GetAreaCoveredByAds_(pageIndex_);
//CAlert::InformationAlert ("After pGeometryImporter->GetAreaCoveredByAds_(pageIndex_)");
	if (pageAreaConsumedByAds == 0)
	{
		LOG_END_FUNCTION;
		return true;
	}

	// Compute area of ad to check.
	PMReal adW = pAd->Width_();
	PMReal adH = pAd->GetHeight_();
	PMReal adA = adW * adH;

	// Compute square points of page that will be covered if the ad were placed.
	PMReal potentialCoverage = pageAreaConsumedByAds + adA;

	// Compute page live area.
	PMReal pageA = width_ * height_;

	// Compute percentage of page live area that would be covered if ad were placed.
	PMReal potentialCoveragePercent = potentialCoverage / pageA;
	
	// Convert value for maximum amount of allowable coverage -- our member that 
	// stores this is a Fixed number -- a QuarkXPress legacy.
	PMReal adCoverageThreshhold = 
				pGeometryImporter->GetAdCoverageThreshhold_() / 65536.0;
	
#if VERSION == IN_HOUSE
	DECLARE_logstr;
	sprintf (logstr, "<PageHasRoomForAd folio_ = \"%d\"  "
						"adCoverageThreshhold = \"%f\"  "
						"potentialCoveragePercent = \"%f\"",
						folio_, ToFloat (adCoverageThreshhold), 
						ToFloat (potentialCoveragePercent));
	LOG (logstr);
#endif
	LOG_END_FUNCTION

	// The ad is deemed to fit if placing it won't cause the page's maximum 
	// allowed amount of coverage to be exceeded.
	return (potentialCoveragePercent <= adCoverageThreshhold);
}


/*================================================================================*/
bool CL_Page::PageHasRoomForAd_(CL_Display_Ad& inAd) {
/*----------------------------------------------------------------------------------
	Abstract:
		Determine whether pAd can fit on this page without exceeding the 
		threshhold for area occupied by ads as defined in the preferences file.  
		This is part of the Cape Gazette enhancements.
				
	Parameters and modes:
		pAd		Pointer to the ad caller is trying to fit on the page.			IN
		
	Returns:
		If the ad in question can possibly fit on the page without exceeding 
		the threshhold for area occupied by ads, return true, else false.

	Change Log:
		22-Oct-05	Began port to InDesign.
		03-Oct-02	Began rewrite from the old 68K code.
		21-Apr-01	New.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	ASSERT (pGeometryImporter);

	// If no ad has been placed on the page yet, pass this one through.  This 
	// is to ensure that even if the first candidate ad exceeds the threshhold, 
	// it will be placed.  How else would the program place a full page ad, if 
	// the maximum coverage were, say, 50%?
	PMReal pageAreaConsumedByAds = 
							pGeometryImporter->GetAreaCoveredByAds_(pageIndex_);
//CAlert::InformationAlert ("After pGeometryImporter->GetAreaCoveredByAds_(pageIndex_)");
	if (pageAreaConsumedByAds == 0)
	{
		LOG_END_FUNCTION;
		return true;
	}

	// Compute area of ad to check.
	PMReal adW = inAd.Width_();
	PMReal adH = inAd.GetHeight_();
	PMReal adA = adW * adH;

	// Compute square points of page that will be covered if the ad were placed.
	PMReal potentialCoverage = pageAreaConsumedByAds + adA;

	// Compute page live area.
	PMReal pageA = width_ * height_;

	// Compute percentage of page live area that would be covered if ad were placed.
	PMReal potentialCoveragePercent = potentialCoverage / pageA;
	
	// Convert value for maximum amount of allowable coverage -- our member that 
	// stores this is a Fixed number -- a QuarkXPress legacy.
	PMReal adCoverageThreshhold = 
				pGeometryImporter->GetAdCoverageThreshhold_() / 65536.0;
	
#if VERSION == IN_HOUSE
	DECLARE_logstr;
	sprintf (logstr, "<PageHasRoomForAd folio_ = \"%d\"  "
						"adCoverageThreshhold = \"%f\"  "
						"potentialCoveragePercent = \"%f\"",
						folio_, ToFloat (adCoverageThreshhold), 
						ToFloat (potentialCoveragePercent));
	LOG (logstr);
#endif
	LOG_END_FUNCTION

	// The ad is deemed to fit if placing it won't cause the page's maximum 
	// allowed amount of coverage to be exceeded.
	return (potentialCoveragePercent <= adCoverageThreshhold);
}


// Finds all our ad boxes on this page and creates display ad objects for each.
// Data is used for creating the ad report.
void CL_Page::FindAdsPlaced_(vector<CL_Display_Ad> & v)
{
}

#if 0
// Ungroup the text and pic box, then delete 
// the now unneeded text box.
void CL_Page::PruneTextBox_(boxid inGroupBoxId, boxid inPicBoxId)
{
}
#endif

void CL_Page::ConvertPageFRectToSpread_(CL_NewsCadRect* ioFixedRect)
{
}

#if 0
Handle CL_Page::GetBoxSlug_(boxid inBoxID) const
{
	Handle 				h = NULL;
	return h;
}
#endif

void CL_Page::ImportArtFiles_(uchar* inGraphicsPath)
{
	LOG_BEGIN_FUNCTION;
	LOG_END_FUNCTION;
}

#if 0
OSErr CL_Page::ImportOneArtFile_(uchar* inGraphicsPath, 
										CL_Display_Ad* pAd, const boxid currentBox)
{
	return noErr;
}
#endif

OSStatus CL_Page::Redraw_()
{
	return noErr;
}


//------------------------------------------------------------------------------
//	FindNarrowestAndShallowestAd
//------------------------------------------------------------------------------
void CL_Page::FindNarrowestAndShallowestAd (vector<CL_Display_Ad> & adsvec, 
												PMReal* shallowestAdHeight, 
												PMReal* narrowestAdWidth)
{
	*shallowestAdHeight = *narrowestAdWidth = 7920;			// 110 inches
	PMReal w, h;

	vector<CL_Display_Ad>::iterator p = adsvec.begin ();
	while (p < adsvec.end ())
	{
		w = p->Width_();
		h = p->GetHeight_();
		
		if (w < *narrowestAdWidth)		// Narrower than narrowest seen so far?
		{
			*narrowestAdWidth = w;
		}
		
		if (h < *shallowestAdHeight)	// Shallower than shallowest seen so far?
		{
			*shallowestAdHeight = h;
		}
		
		++p;
	}
}

//------------------------------------------------------------------------------
//	SortAdsBySquareArea_
//------------------------------------------------------------------------------
void CL_Page::SortAdsBySquareArea_(vector<CL_Display_Ad> & adsToPlaceVector, 
							PMReal* shallowestAdHeight, PMReal* narrowestAdWidth)
{
	// Sort ads by area descending.
	sort ( adsToPlaceVector.begin (), adsToPlaceVector.end (), Compare_Ads_Area );
	FindNarrowestAndShallowestAd (adsToPlaceVector, shallowestAdHeight, 
														narrowestAdWidth);
}


void CL_Page::SortAdsByColumnsWide_(vector<CL_Display_Ad> & adsToPlaceVector, 
							PMReal* shallowestAdHeight, PMReal* narrowestAdWidth)
{
	// Sort ads by columns wide descending.
	sort ( adsToPlaceVector.begin (), adsToPlaceVector.end (), 
										Compare_Ads_ColumnsWide );
	FindNarrowestAndShallowestAd (adsToPlaceVector, shallowestAdHeight, 
														narrowestAdWidth);
}


int32 CL_Page::GetSpreadIndex_(int32 inPageIndex)
{
	if (spreadIndex_ < 0)
	{
		InitializeInDesignData_();
	}
	return spreadIndex_;
}


/*================================================================================*/
void CL_Page::InitializeInDesignData_() {
/*----------------------------------------------------------------------------------
	Abstract:
		Sets up various members (see below) required for InDesign ops.
		
	Parameters and modes:

	Returns:
		Nothing.
	
	Modifies Members:
		uidRef_			spreadIndex_			spreadUID_			spreadUIDRef_
		pagePosOnSpread_
		
	Assumptions:
		Members uid_ and pageIndex_ must be setup before calling this function.
		
	Change Log:
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	DECLARE_logstr;
	
::sprintf (logstr, "pageIndex_=%d", pageIndex_);
LOG (logstr);

	// Get front document
	InterfacePtr<IDocument> frontDocument (
			Utils<ILayoutUIUtils>()->GetFrontDocument (), UseDefaultIID ());
	if ( !frontDocument ) throw "could not get pointer to front document";
	IDataBase* database = ::GetDataBase (frontDocument);
	if (!database) throw ("IDataBase");
 	UIDRef uidRef (database, uid_);
 	uidRef_ = uidRef;

	InterfacePtr<ISpreadList> ipSpreadList ( frontDocument, UseDefaultIID() );
	if (!ipSpreadList) throw "ISpreadList";
	
	// Walk thru the spread list, and find which spread contains this page.  Also 
	// compute this page's position on that spread.
	int32 i, pageCount, numPagesOnCurrentSpread;
	UID spreadUID;
	UIDRef spreadUIDRef;
	for (i = 0, pageCount = 0; i < ipSpreadList->GetSpreadCount (); i++)
	{
		spreadUID = ipSpreadList->GetNthSpreadUID (i);

::sprintf (logstr, "spreadUID=0x%x", spreadUID.Get());
LOG (logstr);

		InterfacePtr<IGeometry> ipSpreadGeometry (
									ipSpreadList->QueryNthSpread (i));
		spreadUIDRef = ::GetUIDRef (ipSpreadGeometry);

::sprintf (logstr, "spreadUID computed from spreadUIDRef=0x%x", spreadUIDRef.GetUID().Get());
LOG (logstr);

		InterfacePtr<ISpread> ipSpread (spreadUIDRef, UseDefaultIID());
		numPagesOnCurrentSpread = ipSpread->GetNumPages ();
		pageCount += numPagesOnCurrentSpread;

::sprintf (logstr, "pageCount=%d", pageCount);
LOG (logstr);

		if (pageCount > pageIndex_) break;
	}
	
	pagePosOnSpread_ = pageIndex_ - pageCount + numPagesOnCurrentSpread;
	spreadIndex_ = i;
	spreadUID_ = spreadUID;
	spreadUIDRef_ = spreadUIDRef;

::sprintf (logstr, "Finally:  spreadIndex_=%d", spreadIndex_);
LOG (logstr);
::sprintf (logstr, "spreadUID_=0x%x", spreadUID_.Get());
LOG (logstr);
::sprintf (logstr, "spreadUID computed from spreadUIDRef_=0x%x", spreadUIDRef_.GetUID().Get());
LOG (logstr);
::sprintf (logstr, "page pos on spread = %d", pagePosOnSpread_);
LOG (logstr);
InterfacePtr<ISpread> ipSpread (spreadUIDRef_, UseDefaultIID());
pagePosOnSpread_ = ipSpread->GetPageIndex (uid_);
::sprintf (logstr, "COMPUTED page pos on spread = %d", pagePosOnSpread_);
LOG (logstr);

	LOG_END_FUNCTION;
}

//----------------------------------------------------------------------------------
//	ComputeRectanglesSurrounding_
//
//	Given a page item of interest, get its geometry and build rectangles above, 
//	below, to the left, and to the right of that page item.  Store them in the 
//	rectanglesVector_.  The vector gets optimized later in the program.
//----------------------------------------------------------------------------------
void CL_Page::ComputeRectanglesSurrounding_(
				UIDRef & inPageItemUIDRef, bool & isFirstItemInProcess)
{
	LOG_BEGIN_FUNCTION;
	
	// First, make sure whatever was passed in is valid.	
	if (inPageItemUIDRef.GetUID () == kInvalidUID)
	{
		LOG ("<!-- Invalid UIDRef was passed in! -->");
		LOG_END_FUNCTION;
		return;
	}
	
	InterfacePtr<IGeometry> objectGeometry (inPageItemUIDRef, UseDefaultIID());
	if (!objectGeometry)
	{
		LOG ("<!-- NO OBJECT GEOMETRY -->");
		LOG_END_FUNCTION;
		return;
	}
	
	CL_NewsCadRect itemPageRect;
	ObjectRectToPageRect_(objectGeometry, itemPageRect);
	if ( isFirstItemInProcess )
	{
		// Program has created a default rect (the whole page), but since a 
		// valid box has been found on the page, throw that rect away.
		rectanglesVector_.clear ();
		isFirstItemInProcess = false;
	}
	
	// Create and store rectangles in rectanglesVector_ that lie to the RIGHT, 
	// ABOVE, LEFT, and BELOW that of the found box.  There are certain conditions 
	// the program considers to ensure these computed rectangles make sense.
	ComputeAndStoreRectanglesToRightOf_(itemPageRect);
	ComputeAndStoreRectanglesAbove_(itemPageRect);
	ComputeAndStoreRectanglesToLeftOf_(itemPageRect);
	ComputeAndStoreRectanglesBelow_(itemPageRect);

	LOG_END_FUNCTION;
}


// END OF FILE
