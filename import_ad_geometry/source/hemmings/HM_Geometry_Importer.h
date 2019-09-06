/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	HM_Geometry_Importer.hpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	6/04/03 ca. 12:20 PM
	
	ab	Abstract:
	ab	Declarations for HM_Geometry_Importer.hpp class.
	
	cl	Change Log:
	cl	15-Jun-05	Began port to InDesign.
	cl	04-Jun-03	New.
------------------------------------------------------------------------------*/
#ifndef _HM_GEOMETRY_IMPORTER_H_
#define	_HM_GEOMETRY_IMPORTER_H_

#include "TextStoryAccessTypes.h"
#include "CL_Geometry_Importer.h"
#include "newscad.h"
//#include "HM_Display_Ad.h"

#if HNM_PRODUCT == INDESIGN_PLUGIN
	class IDataBase;
	class ITextModel;
	class IDocument;
	class ISpread;
	class IFrameList;
#endif

class HM_Geometry_Importer : public CL_Geometry_Importer {
public:
	// Construction and destruction
			HM_Geometry_Importer ();
	virtual ~HM_Geometry_Importer ();
	
	// Called from HM_Page::PasteUp_() as soon as it has finished pasting up an ad.
	virtual void HandleAdPastedUp_(int inIdx);

protected:
	virtual void	BuildInDesignDocument_();						// Override
	virtual void	ProcessStory_(UIDRef inStoryUIDRef) throw (char*);
	virtual bool	PasteUpDisplayAdsForClassification_(PMString & inClassification, 
						ISpread* inSpread, UID inPageUID);

private:
	HM_Geometry_Importer (const HM_Geometry_Importer & inObj)
	{}														// Prevent copying
	HM_Geometry_Importer & operator=
		(const HM_Geometry_Importer & inObj)
	{return *this;}											// Prevent assignment
	
	IDocument*	frontDoc_;
	IDataBase*	frontDocDB_;
	IFrameList*	currentStoryFrameList_;

protected:
////////////////////////////////////////////
#if 0
QUARK CODE:

	virtual void	BuildQuarkXPressDocument_();					// Override
	virtual bool	PasteUpDisplayAdsForClassification_(
						char* inClassification, int16 folio);

	

#endif
////////////////////////////////////////////
	
	// Page makeup
	virtual void	SelectAdsInCurrentClassification_(const char* inClassification);

	// Data management
	virtual bool	UpdateAdVectorsAfterPagePasteup_();

	// Logging
	void	LogDocumentStories_();
	void	LogCurrentStoryData_(int32 inStoryCounter, ITextModel* ipCurTextModel, 
									UIDRef inStoryUIDRef, int32 inStoryCharCount, 
									TextStory_StoryAccess inStoryAccess);
	void	LogTextRunData_(const TextIndex position, 
							const int32 length, const char* name);
	void	LogClassStartPageData_(const UID pageUID);
};

#endif

// END OF FILE
