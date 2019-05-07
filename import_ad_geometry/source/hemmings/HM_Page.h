/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	HM_Page.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	6/18/03 ca. 12:13 AM
	
	ab	Abstract:
	ab	Declarations for HM_Page class.  This class extends the basic page 
	ab	class CL_Page for functionality required by Hemmings Motor News.
	
	cl	Change Log:
	cl	1/25/11	Began port to InDesign CS4.
	cl	6/18/03	New.
------------------------------------------------------------------------------*/
#ifndef _HM_PAGE_H_
#define _HM_PAGE_H_

//#include <map>
#include <vector>
#include "HM_Teaser_Ad_Classes.h"
#include "CL_Page.h"
#include "CL_NewsCadRect_ID.h"


class IPageList;
class HM_Geometry_Importer;

class HM_Page : public CL_Page {
public:
	// Construction / destruction
			HM_Page ();
			HM_Page (HM_Geometry_Importer* inImporter);
			HM_Page (HM_Geometry_Importer* inImporter, IPageList* inPageList,
						UID inUid, int32 inPgIdx, int16 inFolio);
			HM_Page (IPageList* inPageList, UID inUid);
	virtual ~HM_Page ();

	
	virtual void	SetupForAdPlacement_();
	virtual void	SetupForClassifiedContinuedLinePlacement_();
	virtual void	GoTo_();

	// Page makeup -- determine ad geometry and place ads
	virtual void	PasteUp_(std::vector<CL_Display_Ad> & adsToPlaceVector);

	// Page makeup -- classified ad continued lines
	virtual void	TypesetAndPasteupContinuedLine_(PMString & inHeadingText);
	virtual UIDRef	CreateTextFrameForContinuedLine_(
						CL_NewsCadRect* inNewsCadRectPtr, 
						UIDRef & outContinuedLineStoryUIDRef);

	/* TO BE IMPLEMENTED!!
	virtual void	TypesetAndPasteUpContinuedLine_(
						char *inHeadingText, Rect *inNextHeadRect);
	virtual void	TypesetContinuedLine_(char *inHeadingText);
	
// "Teaser" ad processing
	virtual void	FindAdNumbersPlaced_(vector<CL_Display_Ad> & v);


*/
protected:
	HM_Geometry_Importer* pHMGeometryImporter_;

	virtual void	FindTopmost_(std::vector<CL_NewsCadRect> & newsCadRects, 
													CL_NewsCadRect & outTopmost);
	virtual void	ComputePageGeometry_();
 	virtual void	FilterCandidateRectsForContinuedLine_(
 											std::vector<CL_NewsCadRect> & newsCadRects);

private:
	HM_Page (const HM_Page & inObj)				{}					// Prevent copying
	HM_Page & operator=(const HM_Page & inObj)	{return *this;}		// Prevent assignment
};

#endif
// END OF FILE


