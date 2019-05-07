//------------------------------------------------------------------------------
//	Source name:
//		CL_Geometry_Importer.h
//
//	Abstract:
//		Declaration of NewsCAD class that reads the database's export file and 
//		builds a QuarkXPress document from it.  We're not doing much with C++ 
//		in NewsCAD beyond encapsulating the XTension's code and data and using 
//		information hiding.
//
//	History:
//
//		01-Sep-02	New.
//------------------------------------------------------------------------------
#ifndef _CL_GEOMETRY_IMPORTER_H_
#define	_CL_GEOMETRY_IMPORTER_H_

#include <vector>
#include <string>
#include "newscad.h"
#include "CL_Display_Ad.h"
#include "CL_Document.h"
#include "global_data.h"
#include "CL_Log_File.h"

class CL_Geometry_Importer {
public:
	// Construction and destruction
			CL_Geometry_Importer ();
	virtual ~CL_Geometry_Importer ();

	// The big, ugly, main routine
	virtual void	Run_();

	// Compute and tell caller area of a page already covered by ads.
	PMReal	GetAreaCoveredByAds_(int16 pgidx);
	Fixed	GetAdCoverageThreshhold_()		{ return adCoverageThreshhold_; }


protected:
	// Put up a file selection dialog for user to indicate which file to import.
	void			ChooseFile_();

	virtual void	CreateAdObjectsFromInputFile_() throw (std::string);
	virtual bool	VerifyFile_(std::string & s);
	virtual bool	StoreColorPages_(std::string & s);
	virtual void	ProcessColorPageToken_(std::string & tok);
	virtual void	BuildInDesignDocument_();

	void			ComputeAdCoverageThreshhold_();
	virtual void	ResizeAdCoverageVectorIfNeeded_(
									int16 inMinimumSize);
	void			UpdateAdVectorsAfterPasteUp_();
	void			StoreUnplacedAdsInDocSlug_();

	CL_Document*	CurrentDocument_()		{ return currentDocument_; }
	bool			IsColorPage_(int16 pageNum);
	virtual void	StoreArea_(PMReal adWidth, PMReal adHeight, int16 folio);
	
	
	char*			inputFilePath_;
	Fixed			adCoverageThreshhold_;	// Max percentage of area 
													// on any page that can be 
													// covered by ads.
	OSStatus		xtErr_;
	
	// Folios of pages in color signatures
	std::vector<long> colorPagesVector_;
	
	// Array to store the amount of each page covered by ads.
	std::vector<PMReal>	adCoverageVector_;
	
	// Objects that store "cooked" geometry file
	std::vector<CL_Display_Ad> unplacedAdsVector_;		
	
	// Set true if we need to deal with master pages
	bool masterPagesRecFound_;
	
	// Stores the record that begins with "MP"
	std::string masterPageCmd_;

	CL_Document*	currentDocument_;

	std::vector<CL_Display_Ad>	adsToPlaceVector_;		// Objects destined for current page
	std::vector<CL_Display_Ad>	placedAdsVector_;
};

#endif

// END OF FILE

////////////////////////////////////////////
#if 0
	// Keep track of area on each page consumed by ads.
#if PRODUCT == QUARKXTENSION
	virtual void StoreArea_(long adWidth, long adHeight, int16 folio);
#elif PRODUCT == INDESIGN_PLUGIN
#else
	#error	PRODUCT not defined!
#endif


	virtual void	Parse_();
	virtual bool	Read_1_Record_(int32 cntCharsProcessed, 
								const int32 dataSize, string & s);
	virtual void	BuildQuarkXPressDocument_();
	virtual void	ResizeAdCoverageVectorIfNeeded_(
									int16 inMinimumSize);
	FSSpec					inputFileSpec_;
	short					inputFileRefNum_;
#endif
////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////// ++
#if 0
	
	string					basicMasterPageName_;
	vector<string>			masterPageCmdTokens_;	// Individual pieces of the MP|...


private:
//	CL_Page*				currPageObjPtr;			// Page in process
#endif
/////////////////////////////////////////////////////////////////////////////// --
