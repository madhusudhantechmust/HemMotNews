/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	CL_Ad_Reporter_ID.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	1/17/07 ca. 8:45 PM
	
	ab	Abstract:
	ab	Declarations for CL_Ad_Reporter_ID.h class.
	
	cl	Change Log:
	cl	1/17/07	New.
----------------------------------------------------------------------------------*/
#pragma once

using namespace std;

#include <vector>
#include "CL_Display_Ad.h"
#include "IDocument.h"
#include "ISpreadList.h"
#include "IGeometry.h"
#include "IDFile.h"
#include "IHierarchy.h"
#include "CAlert.h"




class CL_Ad_Reporter_ID {
public:
	CL_Ad_Reporter_ID ();												// Create
	virtual	~CL_Ad_Reporter_ID ();										// Destroy
	virtual void ScanDocument_();
	virtual void WriteReport_();

private:
	// Prevent copying and assignment
	CL_Ad_Reporter_ID (const CL_Ad_Reporter_ID & inObj);				// Copy
	CL_Ad_Reporter_ID & operator= (const CL_Ad_Reporter_ID & inObj);	// Assign

protected:
	vector<CL_Display_Ad> placedAdsVector_;
	IDFile reportFile_;
	FSSpec outputFileMacFSSpec_;

	virtual bool ChooseOutputFile_(bool & ioDoesFileExist);

	virtual void ScanSpread_(IDocument* inIDocumentPtr, 
								ISpreadList* inSpreadListPtr, int inSpreadNum)
								throw (char*);
	virtual void ProcessPageItems_(UIDList & inPageItemList);
	virtual void ProcessPageItem_(UID inPageItemUID, UIDRef inPageItemUIDRef);
	virtual void ProcessPageItem_(
						UID inItemUID, 
						UIDRef inPageItemUIDRef, 
						IHierarchy* inPageItemIHierarchy);
	virtual void VisitChildren_(IHierarchy* parent);
	virtual void ComputeAndStorePageNumber_(UID inUID, UIDRef inUIDRef, 
									CL_Display_Ad* ioAd, UID & adPageUID) const;
	virtual void ComputeAndStoreAdGeometry_(UIDRef inUIDRef, UID & inAdPageUID, 
								IGeometry* pIGeometry, CL_Display_Ad* ioAd) const
								throw (char*);
	virtual void GetBlackBoxData_(IPMUnknown* inObj, char** ioBuffer);
	
	
	// Use this for putting string exceptions on the screen, or just for errors.
	inline void GiveAlert_(char* except)
	{
		PMString alertString (except);
		CAlert::ModalAlert
		(
			alertString,		// Text to display
			kOKString,			// OK button
			kNullString, 		// No second button
			kNullString, 		// No third button
			1,					// Set OK button to default
			CAlert::eErrorIcon
		);

	}
	
	// Use these for debugging:
	void Log1Object_(IPMUnknown* inObj, bool callerWantsBlackBoxDataLogged = true);
};

