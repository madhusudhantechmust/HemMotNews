/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	frame_geometry.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	4/6/06 ca. 8:29 PM
	
	ab	Abstract:
	ab	Some utility functions to handle frame geometry in InDesign plugins.
	ab	
	
	cl	Change Log:
	cl	4/6/06	New.
----------------------------------------------------------------------------------*/

#include "UIDList.h"
#include "IGeometry.h"
#include "TransformUtils.h"
#include "IHierarchy.h"
#include "ISpread.h"
#include "IDataBase.h"
#include "IPasteboardUtils.h"
#include "TransformUtils.h"

/* ---------------------------- P R O T O T Y P E S ----------------------------- */

ErrorCode GetFrameGeometryInPageCoordinates_(const UIDRef& inFrameUIDRef, 
														PMRect & outFrameRect);
ErrorCode ObjectRectToPageRect_(const UIDRef & inUIDRef, 
				const IGeometry* inObjectGeometry, PMRect & outResult);
bool GetPageBoundsForItem_( IPMUnknown* pageItem,PMRect& pageItemBounds );

#pragma mark -

/*================================================================================*/
bool GetPageBoundsForItem_( IPMUnknown* pageItem,PMRect& pageItemBounds ) {
/*----------------------------------------------------------------------------------
	Abstract:
		
	Parameters and modes:

	Returns:
		true	on success, else false.  Notice this is exactly opposite from 
				the way most functions written for the Mac return error info.
				
	Change Log:
		13-Apr-06	New.  This is Adobe code, supplied as a response to DCN40731.
----------------------------------------------------------------------------------*/
	pageItemBounds = kZeroRect;

	do
	{
		// get page item's IHierarchy
		InterfacePtr<IHierarchy> iHierarchy(pageItem,UseDefaultIID());
		if ( iHierarchy == NULL ) break;

		// get the spread the item is on
        InterfacePtr<ISpread> parentSpread(Utils<IPasteboardUtils>()->QuerySpread(iHierarchy));
		if ( parentSpread == NULL ) break;

		InterfacePtr<IGeometry> iGeometry(pageItem,UseDefaultIID());
		if ( iGeometry == NULL ) break;

		// page item bounds in pasteboard coordinates
		PMRect pathBox = iGeometry->GetStrokeBoundingBox();
		TransformInnerRectToPasteboard(iGeometry,&pathBox );

		// we use center of page item to determine which page this item is on
		PMPoint center = pathBox.GetCenter();

		// pages are on a separate layer
		InterfacePtr<IHierarchy> pagesLayer(parentSpread->QueryPagesLayer());
		if ( pagesLayer == NULL || pagesLayer->GetChildCount() < 1) break;

		IDataBase * db = ::GetDataBase(pageItem);

		// iterate through each page on this spread until we find the one
		//  that contains the center of our page item
		for (int32 i = 0 ; i < pagesLayer->GetChildCount() ; i++)
		{
			// page bounds in pasteboard coordinates
			InterfacePtr<IGeometry> pageItem(db, 
										pagesLayer->GetChildUID(i), IID_IGEOMETRY);
			PMRect pageBounds = pageItem->GetStrokeBoundingBox (
									InnerToPasteboardMatrix (pageItem) );

			if ( pageBounds.PointIn(center) )
			{
				// center of page item is on this page
				//  adjust page item bounds relative to page bounds
				pageItemBounds = pathBox.MoveRel ( 
										-pageBounds.Left(),-pageBounds.Top() );
				return true;
			}
		}
	}
	while ( false );

	return false;
}


/*================================================================================*/
ErrorCode GetFrameGeometryInPageCoordinates_(const UIDRef & inFrameUIDRef, 
													PMRect & outFrameRect) {
/*----------------------------------------------------------------------------------
	Abstract:
		Compute an item's page geometry from its UIDRef.  A UIDRef is 	 		IN
		a datum in InDesign that uniquely identifies an item and its 
		document.  Hence it's unique irrepsective of the number of 
		documents open, which one is the frontmost, etc.
		
	Parameters and modes:

	Returns:

	Change Log:
		06-Apr-06	New.
----------------------------------------------------------------------------------*/
	ErrorCode errCode = -1;									// Default to error
	
/*	InterfacePtr<IGeometry> objectGeometry (inFrameUIDRef, UseDefaultIID());
	if (objectGeometry)
	{
		errCode = ObjectRectToPageRect_(inFrameUIDRef, objectGeometry, outFrameRect);
	}*/


	InterfacePtr<IGeometry> iGeometry (inFrameUIDRef, UseDefaultIID());
	if ( iGeometry != NULL )
	{
		PMRect pathBox = iGeometry->GetPathBoundingBox();
		PMRect strokeBox = iGeometry->GetStrokeBoundingBox();

		// convert path bounds to pasteboard coords
		TransformInnerRectToPasteboard(iGeometry, &pathBox );

		// convert stroke bounds to pasteboard coords
		TransformInnerRectToPasteboard ( iGeometry, &strokeBox );
		
		// Give the caller what he wants
		outFrameRect = strokeBox;

		errCode = noErr;
	}

	return errCode;
}


/*================================================================================*/
ErrorCode ObjectRectToPageRect_(const UIDRef & inUIDRef, 
				const IGeometry* inObjectGeometry, PMRect & outResult) {
/*----------------------------------------------------------------------------------
	Abstract:
		Given a geometry interface for an object, compute its location on the page 
		where the top, left of the trim is at 0,0.
		
	Parameters and modes:

	Returns:

	Change Log:
		06-Apr-06	New.
----------------------------------------------------------------------------------*/
	// A PBPMRect is nothing more than a typedef PMRect -- represents a rect in 
	// "PasteBoard" coordinates.
	PBPMRect bBoxPasteboard = inObjectGeometry->GetStrokeBoundingBox (
							::InnerToPasteboardMatrix (inObjectGeometry));
	
	InterfacePtr<IGeometry> pageGeometry (inUIDRef, UseDefaultIID());
	if (!pageGeometry)
		return -1;

	PBPMRect bBoxPagePasteboard = pageGeometry->GetStrokeBoundingBox (
										::InnerToPasteboardMatrix (pageGeometry));
	
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

	return noErr;
}


// END OF FILE
