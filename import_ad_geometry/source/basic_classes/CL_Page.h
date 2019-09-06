/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	CL_Page.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	ca. 9/03/02 10:28 AM
	
	ab	Abstract:
	ab	Declarations for CL_Page class.  This object provides information 
	ab	about a page in the QuarkXPress document.
	
	cl	Change Log:
	cl	23-Jan-11	Began port to CS4.
	cl	10-Sep-05	Began port to InDesign.
	cl	03-Sep-02	New.
----------------------------------------------------------------------------------*/
#ifndef	CL_PAGE_H_
#define	CL_PAGE_H_

//using namespace std;
#include <vector>
#include "ILayoutUtils.h"
#include "CL_NewsCadRect_ID.h"
#include "Basic_Page.h"
#include "CL_Display_Ad.h"

class CL_Geometry_Importer;
class CL_NewsCadRect;
class CL_Display_Ad;
class IPageList;
class IGeometry;


class CL_Page : public Basic_Page {
public:
			CL_Page ();
			CL_Page (CL_Geometry_Importer* inImporter);
	virtual ~CL_Page ();

	// Queries QXP and stores info for pages needed during processing of 
	// the input geometry file -- page column width, column count, gutter width.
	virtual void	StorePageInfo_();
	
	// Query QXP and store information for a page needed for placing ads.
	// This was the C function "SetupPage()" in 68K NewsCAD.
	virtual void	SetupForAdPlacement_();
	virtual int		FindOpenRects_();
	virtual void	ProcessPageItem_(UID inItemUID, 
						UIDRef inPageItemUIDRef, IHierarchy* inPageItemIHierarchy);

	virtual void 	ConvertPageFRectToSpread_(CL_NewsCadRect* ioFixedRect);
														// replace pagefrect2sprd()

	// Page makeup -- determine ad geometry and place ads
					// Big, ugly containing loop, does it all
    using Basic_Page::PasteUp_;
	virtual void	PasteUp_(std::vector<CL_Display_Ad> & adsToPlaceVector);
	
	// This just places ads -- it calls methods in derived classes for creating 
	// and sorting rectangles to reflect the changing geometry of the page as ads 
	// get pasted up.  The idea is to put the basic stuff (finding ads to fit 
	// rectangles, creating text boxes to contain ads, etc.) in 1 place, and 
	// allow derived classes to provide code to customize the how and where.

	virtual void	PlaceAds_(std::vector<CL_Display_Ad> & adsToPlaceVector, 
								std::vector<CL_NewsCadRect> & newsCadRects);

	virtual void	InitializeOpenRects_(
							std::vector<CL_NewsCadRect> & inVecNewsCadRects);
	
	// Art import
//	void			PruneTextBox_(boxid inGroupBoxId, boxid inPicBoxId);

	// Find an ad to fit a rectangle
	virtual int		FindBestFit_(CL_NewsCadRect& ncr, 
								std::vector<CL_Display_Ad> & adsToPlaceVector);

	// Ad reporting
	virtual void	FindAdsPlaced_(std::vector<CL_Display_Ad> & placedAdsVector);
	
	// Art import
	virtual void	ImportArtFiles_(uchar* inGraphicsPath);
/*
	// Safe wrappers for Quark functions
	virtual bool	GoToFolio_();			// gotopage					OBSOLETE
	virtual XTError RedrawPage_();			// redrawpage				OBSOLETE
*/
	// Getters
	virtual PMReal	GetColumnWidth_()		{return columnWidth_;}
	virtual	int		GetColumnCount_()		{return columnCount_;}
	virtual PMReal	GetGutterWidth_()		{return gutterWidth_;}

	PMReal			GetLeft_()				{return left_;}
	PMReal			GetTop_()				{return top_;}
	PMReal			GetRight_()				{return right_;}
	PMReal			GetBottom_()			{return bottom_;}
	PMReal			LiveAreaLeft_()			{return pgRect_.Left ();}
	PMReal			LiveAreaTop_()			{return pgRect_.Top ();}
	PMReal			GetWidth_()				{return width_;}
	PMReal			GetHeight_()			{return height_;}
	PageType		PageType_()				{return pageType_;}
	PMReal			InsideMargin_()			{return insideMargin_;}
	UID				UID_()					{return uid_;}
	UIDRef			UIDRef_()				{return uidRef_;}
	UIDRef			SpreadUIDRef_()			{return spreadUIDRef_;}
	int16			GetFolio_()				{return folio_;}

	// Setters
	virtual void	SetFolio_(int16 pgnum)	{ folio_ = pgnum; }
	virtual void	SetIsColorPage_(bool b)	{ isColorPage_ = b; }
//	virtual void	AppendCurrentAd_(CL_Display_Ad* inAd);

	virtual void	InitializeInDesignData_();
	virtual int32	GetSpreadIndex_(int32 inPageIndex);


protected:
	void			Initialize_();
	void			OLD_GetGraphicFrameUIDRefVector_();
	void			GetGraphicFrameUIDRefVector_();
	
//OBSOLETE	virtual bool	IsFolioInRange_();

//	virtual bool	PageHasRoomForAd_(CL_Display_Ad* pAd);
	virtual bool	PageHasRoomForAd_(std::vector<CL_Display_Ad>::iterator p);
	virtual bool	PageHasRoomForAd_(CL_Display_Ad& inAd);


	// Page makeup
					// Derived classes must override these next 2 methods!
	virtual void	SortCandidateRects_(std::vector<CL_NewsCadRect> & newsCadRects);
	virtual void	RecomputeCandidateRects_(std::vector<CL_NewsCadRect> & newsCadRects, 
						int adIndex, Fixed adYPos, Fixed adHeight, 
						std::vector<CL_Display_Ad> & adsToPlaceVector);

	virtual bool 	RectangleOverprints_(CL_NewsCadRect* inCandidateFRect);
	void 			PlaceAdsBotToTopOutsideToInside_(
							std::vector<CL_Display_Ad> & adsToPlaceVector, 
							std::vector<CL_NewsCadRect> & newsCadRects, 
							Fixed narrowestAdWidth, Fixed shallowestAdHeight);
	virtual void 	UpdateRectanglesVector_(int inAdIndex, 
							std::vector<CL_NewsCadRect> & newsCadRects, 
							std::vector<CL_Display_Ad> & adsToPlaceVector);
	virtual void	SortAdsByDepthWithinWidth_(std::vector<CL_Display_Ad> & adsToPlaceVector);
	virtual void	SortAdsBySquareArea_(std::vector<CL_Display_Ad> & adsToPlaceVector,
						PMReal* shallowestAdHeight, PMReal* narrowestAdWidth);
	virtual void	SortAdsByColumnsWide_(std::vector<CL_Display_Ad> & adsToPlaceVector, 
						PMReal* shallowestAdHeight, PMReal* narrowestAdWidth);
	virtual void 	FindNarrowestAndShallowestAd (std::vector<CL_Display_Ad> & adsvec, 
												PMReal* shallowestAdHeight, 
												PMReal* narrowestAdWidth);
	virtual void	ObjectRectToPageRect_(const IGeometry* inObjectGeometry, 
											CL_NewsCadRect & outResult) const;
	virtual void	ComputeAndStoreRectanglesToRightOf_(
											CL_NewsCadRect & inItemsPageRect);
	virtual void	ComputeAndStoreRectanglesAbove_(
											CL_NewsCadRect & inItemsPageRect);
	virtual void	ComputeAndStoreRectanglesToLeftOf_(
											CL_NewsCadRect & inItemsPageRect);
	virtual void	ComputeAndStoreRectanglesBelow_(
											CL_NewsCadRect & inItemsPageRect);
	virtual void	ProcessIntersection_(CL_NewsCadRect & candidateRect, 
											CL_NewsCadRect & itemRect);

//	virtual OSErr	ImportOneArtFile_(uchar* inGraphicsPath, 
//								CL_Display_Ad* pAd, const boxid currentBox);

	virtual OSStatus	Redraw_();


	// Logging
	virtual void 	RecordReasonAdRejected_(
							std::vector<CL_Display_Ad>::iterator p, 
							CL_NewsCadRect & ncr, 
							bool rejectedForColor);

	//-------------------------------------------------------------------------- 

	int16			folio_;					// QXP sequential page number where 
											// it appears in the document, as 
											// opposed to the page number that 
											// appears on the page
	bool			isColorPage_;			// true if geometry input file 
											// specifies this folio_ as color
// in Basic_Page	int32			columnCount_;			// Number of page columns

	int16			spread_;				// QXP spread number on which 
											// page appears
// in Basic_Page	CL_NewsCadRect	pgRect_;
// in Basic_Page	PMReal			gutterWidth_;			// Page gutter width
// in Basic_Page	PMReal			columnWidth_;			// Page column width
// in Basic_Page	PMReal			width_;					// Live area width
// in Basic_Page	PMReal			height_;				// Live area height
// in Basic_Page	PMReal			left_;					// Left margin x-position
// in Basic_Page	PMReal			top_;					// Top margin y-position
// in Basic_Page	PMReal			right_;					// Right margin x-position
// in Basic_Page	PMReal			bottom_;				// Bottom margin y-position
	PMReal 			narrowestAdWidth_;
	PMReal			shallowestAdHeight_;

// in Basic_Page	UID				uid_;				// Record number of this page in ID database.
// in Basic_Page	UIDRef			uidRef_;
// in Basic_Page	int32			pageIndex_;			// Zero-based position of this page in document
// in Basic_Page	int32			pagePosOnSpread_;	// Index of this page on containing spread.
// in Basic_Page	PageType		pageType_;			// enum PageType { kLeftPage = 0, 
										// kUnisexPage, kRightPage };
// in Basic_Page	PMReal			insideMargin_;
	IPageList*		iPageList_;			// InDesign ptr to page list to which the 
										// page represented by this object belongs
	int32			spreadIndex_;		// Zero-based index of containing sprd.
	UID				spreadUID_;			// InDesign database record num
	UIDRef			spreadUIDRef_;		// InDesign database + record num

	std::vector<CL_Display_Ad>
					adsVector_;				// Ads to attempt to place on the page
	std::vector<CL_NewsCadRect>
					rectanglesVector_;		// Use PMRect for InDesign
	std::vector<UIDRef>	picBoxUIDRefsVector_;	// UIDRefs of items on top of which we 
											//		must not place ads.
	CL_Geometry_Importer* 
					pGeometryImporter;

	void ComputeRectanglesSurrounding_(UIDRef & inPageItemUIDRef, bool & isFirstItemInProcess);
//	virtual Handle GetBoxSlug_(boxid inBoxID) const;

private:
	CL_Page (const CL_Page & inObj)				{}					// Prevent copying
	CL_Page & operator=(const CL_Page & inObj)	{return *this;}		// Prevent assignment
};

bool Compare_Ads_Area (CL_Display_Ad a, CL_Display_Ad b);
bool Compare_Ads_ColumnsWide (CL_Display_Ad a, CL_Display_Ad b);
bool Compare_Ads_Depths (CL_Display_Ad a, CL_Display_Ad b);

#endif

// END OF FILE
