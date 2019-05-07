/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	Basic_Page.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	7/12/06 ca. 8:38 PM
	
	ab	Abstract:
	ab	Declarations for Basic_Page class.  Eventual plan is to derive all page 
	ab	classes for all InDesign plugins from this class.
	
	cl	Change Log:
	cl	9/12/10	Re-examined as part of porting to CS4; no porting changes 
	cl			were required to compile this file successfully.
	cl	7/12/06	New.
----------------------------------------------------------------------------------*/

#pragma once

#include "PMRect.h"
#include "PMString.h"
#include "ILayoutUtils.h"

class PlaceAdPanelDisplayAd;

class Basic_Page {
	public:
		Basic_Page ();													// Create
		Basic_Page (const Basic_Page & inObj);							// Copy
		Basic_Page & operator= (const Basic_Page & inObj);				// Assign
		virtual	~Basic_Page ();											// Destroy

		/* -------------------- Accessors:  SETTERS --------------------- */
		void SetPageIndex_(int32 inPageIdx)		{ pageIndex_ = inPageIdx; }
		void SetUID_(UID inUid)					{ uid_ = inUid; }
		void SetUIDRef_(UIDRef inUidRef)		{ uidRef_ = inUidRef; }
		
		/* -------------------- Accessors:  GETTERS --------------------- */
		int32	PageIndex_()					{ return pageIndex_; }
		int32	ColumnCount_()					{ return columnCount_; }
		void	PagePMRect_(PMRect& inPMRect)	{ inPMRect = pgRect_; }
		PMReal	GutterWidth_()					{ return gutterWidth_; }
		PMReal	ColumnWidth_()					{ return columnWidth_; }
		PMReal	LiveAreaWidth_()				{ return width_; }
		PMReal	LiveAreaHeight_()				{ return height_; }
		PMReal	Left_()							{ return left_; }
		PMReal	Top_()							{ return top_; }
		PMReal	Right_()						{ return right_; }
		PMReal	Bottom_()						{ return bottom_; }
		UID		UID_()							{ return uid_; }
		UIDRef	UIDRef_()						{ return uidRef_; }
		UIDRef	ParentUIDRef_()					{ return parentUidRef_; }
		int32	PagePositionOnSpread_()			{ return pagePosOnSpread_; }
		PageType
				PageType_()						{ return pageType_; }
		PMReal			InsideMargin_()			{return insideMargin_;}
		

		// Chase a gazillion pointers to get Adobe's unique id's for the page.
		void	ComputeCurrentPageUID_() throw (PMString);
		void	ComputePageUIDRef_();
		
		// Determine column/gutter widths, margins, etc.
		virtual
		void	ComputeAndStorePageMeasurements_() throw (PMString);
		
		// Realize an ad -- bring it to life on some page.
		virtual
		void	PasteUp_(PlaceAdPanelDisplayAd* ad) throw (PMString);
		
	protected:
/*
		int16	folio_;						// Index of page where it appears in 
											// the document, as opposed to the page 
											// number that appears on the page
*/
		int32	pageIndex_;					// Zero-based position of page in doc
		int32	columnCount_;				// Number of page columns
		PMRect	pgRect_;
		PMReal	gutterWidth_;				// Page gutter width
		PMReal	columnWidth_;				// Page column width
		PMReal	width_;						// Live area width
		PMReal	height_;					// Live area height
		PMReal	left_;						// Left margin x-position
		PMReal	top_;						// Top margin y-position
		PMReal	right_;						// Right margin x-position
		PMReal	bottom_;					// Bottom margin y-position
		PMReal	insideMargin_;				// At the spine of the book
		UID		uid_;						// Record number of this page in ID db.
		UIDRef	uidRef_;					// This's UIDRef
		UIDRef	parentUidRef_;				// The containing spread's UIDRef
		int32	pagePosOnSpread_;			// Index of page on containing spread.
		PageType
				pageType_;					// enum PageType { kLeftPage = 0, 
											// kUnisexPage, kRightPage };
		
	private:
	
};

