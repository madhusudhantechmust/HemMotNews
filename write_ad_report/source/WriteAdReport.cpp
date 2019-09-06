/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	WriteAdReport.cpp — SDS_Plugins.bbprojectd
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	5/26/11 ca. 10:41 PM
	
	ab	Abstract:
	ab	Implementation of CL_WriteAdReportWorkhorse class.
	ab	
	
	cl	Change Log:
	cl	5/26/11	New.
----------------------------------------------------------------------------------*/
#include "CAlert.h"
#include "IDocument.h"
#include "IDataBase.h"
#include "IHierarchy.h"
#include "ILayoutUIUtils.h"
#include "ISpread.h"
#include "ISpreadList.h"
#include "IFrameType.h"
#include "IPageItemTypeUtils.h"
#include "IBlackBoxCommands.h"
#include "IGeometry.h"
#include "TransformUtils.h"		// for ComputeAndStoreAdGeometry_()
#include "IPageList.h"
#include "IPathUtils.h"
#include "SplineID.h"
#include "IMasterPage.h"
#include "LayerID.h"
#include "ImageID.h"
#include "PDFID.h"
#include "EPSID.h"
#include "GroupID.h"
#include "TextOnPathID.h"
#include "GuideID.h"
#if 0
#include "LayoutUIUtils.h"
#endif
#include "SDKFileHelper.h"

#include "newscad.h"
#include "WriteAdReportID.h"
#include "CL_Document.h"
#include "IAbstractAdReportWriter.h"
#include "CL_Log_File.h"
#include "global_data.h"

#include <vector>

// Set to 1 to get extra debugging output in WriteReport_()
#define		DUMP_PLACED_ADS		0

using namespace std;

class CL_WriteAdReportWorkhorse : 
			public CPMUnknown<IAbstractAdReportWriter> {
	public:
		CL_WriteAdReportWorkhorse (IPMUnknown* boss) 
			: CPMUnknown<IAbstractAdReportWriter>(boss)
		{}
		virtual void	WriteAdReport_();
		virtual void	ScanDocument_();
		virtual void	WriteReport_();

	protected:
		virtual void ScanSpread_(IDocument* inIDocumentPtr, 
								ISpreadList* inSpreadListPtr, int inSpreadNum)
								throw (char*);
		virtual void ProcessPageItems_(UIDList & inPageItemList);
		virtual void ProcessPageItem_(
						UID inItemUID, 
						UIDRef inPageItemUIDRef, 
						IHierarchy* inPageItemIHierarchy);
		virtual void GetBlackBoxData_(IPMUnknown* inObj, char** ioBuffer);
		virtual void ComputeAndStorePageNumber_(UID inUID, UIDRef inUIDRef, 
									CL_Display_Ad* ioAd, UID & adPageUID) const;
		virtual PMPoint ComputeAndStoreAdGeometry_(UIDRef inUIDRef, UID & inAdPageUID,
                                    PBPMRect bBoxPasteboard) const
								throw (char*);
		virtual bool ChooseOutputFile_(std::string & ioFullPath);
		virtual bool ChooseOutputFile_(bool & ioDoesFileExist);

		// Use this for putting string exceptions on the screen, or just for errors.
		inline void GiveAlert_(const char* except)
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
		void Log1Object_(IPMUnknown* inObj, 
							bool callerWantsBlackBoxDataLogged = true);

	private:
		vector<CL_Display_Ad> placedAdsVector_;
};

// This is what enables our code to be called by InDesign:
CREATE_PMINTERFACE(CL_WriteAdReportWorkhorse, kWriteAdReportImpl);

#pragma mark -

/*================================================================================*/
void CL_WriteAdReportWorkhorse::WriteAdReport_() {
/*----------------------------------------------------------------------------------
	Abstract:
		This is the main entry point from the UI plugin.
		
	Parameters and modes:
		None.
		
	Returns:
		Nothing.
	
	Exception handling:
		
	Change Log:
----------------------------------------------------------------------------------*/
// 	CAlert::ModalAlert
// 	(
// 		"BEGIN writing the report of placed ads ...",
// 		kOKString, 					// OK button
// 		kNullString, 				// No second button
// 		kNullString, 				// No third button
// 		1,							// Set OK button to default
// 		CAlert::eInformationIcon	// Information icon.
// 	);
// 
	OPEN_LOG_FILE;
	LOG_BEGIN_FUNCTION;
	placedAdsVector_.reserve (512);
	ScanDocument_();
	WriteReport_();
	LOG_END_FUNCTION;
	CLOSE_LOG_FILE;
}

#pragma mark ----- SCAN DOCUMENT AND LOG PAGE ITEMS -----
/*================================================================================*/
void CL_WriteAdReportWorkhorse::ScanDocument_() {
/*----------------------------------------------------------------------------------
	Abstract:
		This is a "top-level" function that iterates over the document database 
		and gathers and stores information about the page items it contains.
		
	Parameters and modes:
		None.
		
	Returns:
		Nothing.
	
	Exception handling:
		This method will catch exceptions thrown during scanning, so that if 
		the scan has gathered at least some information, it will be written out 
		to the file.  These exceptions are pretty unlikely; if one of them is 
		thrown, the user's computer is probably on fire.
		
	Change Log:
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;

	try {
		IDocument* iDocument = Utils<ILayoutUIUtils>()->GetFrontDocument();
		if (!iDocument) throw ("Could not get IDocument*.");
		
		InterfacePtr<ISpreadList> spreadList ( iDocument, UseDefaultIID () );
		if (spreadList == nil)
			throw "ScanDocument_(): No spread list!";
		
		int32 spreadCount = spreadList->GetSpreadCount();
		if (spreadCount <= 0)
		{
			// This shouldn't ever happen if there is an open doc
			throw "ScanDocument_(): Spread count is 0?";
		}

		// For each spread in the document
		for (int spreadNum = 0; spreadNum < spreadCount; spreadNum++)
		{
			ScanSpread_(iDocument, spreadList, spreadNum);
		}
 	}
	catch (char* except) {
		GiveAlert_(except);			// Try to put up a dialog
		LOG (except);
	}
	catch (...) {
		char* unknownException = 
					"Unknown exception in ScanDocument_() creating ad report.";
		GiveAlert_(unknownException);
		LOG (unknownException);
	}
	
	LOG_END_FUNCTION;
}


/*================================================================================*/
void CL_WriteAdReportWorkhorse::ScanSpread_(IDocument* inIDocumentPtr, 
					ISpreadList* inSpreadListPtr, int inSpreadNum)
					throw (char*) {
/*----------------------------------------------------------------------------------
	Abstract:
		
	Parameters and modes:

	Returns:

	Change Log:
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	LOG_TAG_AND_VALUE ("ScanSpread", inSpreadNum);

	InterfacePtr<ISpread> curSpread (::GetDataBase (inIDocumentPtr), 
							inSpreadListPtr->GetNthSpreadUID (inSpreadNum), 
							IID_ISPREAD);
	if (curSpread == nil)
	{
		// This would be a serious failure.  The exception will be caught in 
		// ScanDocument_(), and will stop execution of the scanning loop.
		LOG_END_FUNCTION;
		throw ("ScanSpread_(): Could not get current spread!");
	}
	
	int32 pageCount = curSpread->GetNumPages ();

	// Every spread should have at least one page, but you
	// never know what could happen
	if (pageCount > 0)
	{
		// Iterate pages in this spread and gather/process page items
		for (int32 pageNum = 0; pageNum < pageCount; pageNum++)
		{
			// Get the list of items on the page
			LOG_TAG_AND_VALUE ("ScanPage", pageNum);
 			UIDList itemList (inIDocumentPtr);
 			curSpread->GetItemsOnPage (pageNum, &itemList, kFalse, kFalse);
 			ProcessPageItems_(itemList);
		}
	}
	LOG_END_FUNCTION;
}


/*================================================================================*/
void CL_WriteAdReportWorkhorse::ProcessPageItems_(UIDList & inPageItemList) {
/*----------------------------------------------------------------------------------
	Abstract:
		This method serves as a sort of "traffic director."
		
	Parameters and modes:
		inPageItemList	An InDesign data structure that contains an InDesign 	IN
						Unique IDentifier for each item on the spread.
						
	Returns:
		Nothing.
		
	Change Log:
		19-Jan-07	New.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	int32 countPageItemsToProcess = inPageItemList.Length ();
	UIDRef pageItemUIDRef = UIDRef::gNull;
    for (int32 k = countPageItemsToProcess - 1; k >= 0; k--)
    {
    	pageItemUIDRef = inPageItemList.GetRef (k);
		InterfacePtr<IHierarchy> pageItemHierarchy (pageItemUIDRef, UseDefaultIID());
		ProcessPageItem_(inPageItemList[k], pageItemUIDRef, pageItemHierarchy);
    }
	LOG_END_FUNCTION;
}


/*================================================================================*/
void CL_WriteAdReportWorkhorse::ProcessPageItem_(UID inItemUID, 
					UIDRef inPageItemUIDRef, IHierarchy* inPageItemIHierarchy) {
/*----------------------------------------------------------------------------------
	Abstract:
		
	Parameters and modes:

	Returns:

	Change Log:
		08-JUN-11	Began port to CS4.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;

	if (!inPageItemIHierarchy)
		return;
	
	Log1Object_(inPageItemIHierarchy);		// TESTING ONLY
	
	if ( ::GetClass (inPageItemIHierarchy) == kSplineItemBoss )
    {
		if (Utils<IPageItemTypeUtils>()->IsTextFrame (inPageItemIHierarchy))
		{
			LOG ("Object is a TEXT frame. IGNORE.<br/>");
		}
		else
		{
			LOG ("Object is a PICTURE frame<br/>");
			
			// Check for presence of NewsCAD BlackBoxData.
			char* buffer;
			GetBlackBoxData_(inPageItemIHierarchy, &buffer);
			if ( ::strlen (buffer) )
			{
				// We have a graphic frame that NewsCAD created.
				LOG_TAG_AND_VALUE ("BlackBoxData", buffer);

				string adData = buffer;
				
				// Use std::auto_ptr just in case ComputeAndStorePageNumber_()
				// or ComputeAndStoreAdGeometry_() throws an exception.
				auto_ptr<CL_Display_Ad> ad (new CL_Display_Ad (adData));
				if (!ad.get())
				{
					LOG ("OUT OF MEMORY");
					LOG_END_FUNCTION;
					return;
				}
				
				// Discover and save x-position, y-position, and page number.
				// Store this information in the ad object.
				InterfacePtr<IGeometry> objectGeometry (
											inPageItemUIDRef, UseDefaultIID());
				if (!objectGeometry)
				{
					LOG ("<p>NO OBJECT GEOMETRY</p>");
					LOG_END_FUNCTION;
					return;
				}

				UID adPageUID = kInvalidUID;
				ComputeAndStorePageNumber_(inItemUID, inPageItemUIDRef, 
														ad.get(), adPageUID);

                PBPMRect oItemBoundingBox = objectGeometry->GetStrokeBoundingBox(::InnerToPasteboardMatrix(objectGeometry));
                try
                {
                    PMPoint oOrigin = ComputeAndStoreAdGeometry_(inPageItemUIDRef, adPageUID, oItemBoundingBox);
                    ad->SetBoxX_(oOrigin.X());
                    ad->SetBoxY_(oOrigin.Y());
                }
                catch (char *poError)
                {
                    LOG(poError);
                }
				placedAdsVector_.push_back (*ad);		// Copy ad into vector
			}
			else
			{
				LOG ("<p>No BlackBoxData present. IGNORE.</p>");
			}
		}
    }
    
	int32 childCount = inPageItemIHierarchy->GetChildCount ();
	if (childCount > 0)
	{
		IDocument* iDocument = Utils<ILayoutUIUtils>()->GetFrontDocument();
		if (!iDocument) throw ("Could not get IDocument*.");
		for (int32 childIndex = 0; childIndex < childCount; childIndex++)
		{
			UID childUID = inPageItemIHierarchy->GetChildUID (childIndex);
			UIDRef childUIDRef (::GetDataBase (iDocument), childUID );
			InterfacePtr<IHierarchy> child (
								inPageItemIHierarchy->QueryChild (childIndex));
			ProcessPageItem_(childUID, childUIDRef, child);
		}
	}

	LOG_END_FUNCTION;
}


/*================================================================================*/
void CL_WriteAdReportWorkhorse::GetBlackBoxData_(
										IPMUnknown* inObj, char** ioBuffer) {
/*----------------------------------------------------------------------------------
	Abstract:
		Given an IHierarchy ptr, return any NewsCAD black box data attached to 
		the object it fingers.
		
	Parameters and modes:
		inObj		Object whose black bax data caller wants.				IN
		ioBuffer	Ptr to buffer to allocate and fill with data.  			IN/OUT
					On input, this should be a pointer only.  This 
					method allocates the memory and fills it with data, 
					if any is present.
	Returns:
		Nothing -- sets ioBuffer.
		
	Change Log:
		08-JUN-11	Began port to CS4.
		08-May-07	New.
	
	Caveats:
		Caller must dispose of the memory pointed to by the passed-in ioBuffer.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	
	UIDRef inObjUidRef = ::GetUIDRef (inObj);
	InterfacePtr<IBlackBoxCommands>blackBoxCmds (
					Utils<IBlackBoxCommands>().QueryUtilInterface());
	char* buffer = new char [512];
	*buffer = '\0';
	int32 len = blackBoxCmds->GetData (inObjUidRef, kNC_PrefixNumber, 
									buffer, 511);
	if ( len )
	{
		*(buffer + len) = '\0';
	}
	
	*ioBuffer = buffer;
	
	LOG_END_FUNCTION;
}


//----------------------------------------------------------------------------------
//	ComputeAndStorePageNumber_
//----------------------------------------------------------------------------------
void CL_WriteAdReportWorkhorse::ComputeAndStorePageNumber_(
		UID inUID, UIDRef inUIDRef, CL_Display_Ad* ioAd, UID & outAdPageUID) const
{
	LOG_BEGIN_FUNCTION;
	
	// Figure out which page and spread contains the ad.  We need 
	// only the page's UID to perform subsequent operations.  The rest of 
	// the code is to put the page number into the log file.
	IDocument* iDocument = Utils<ILayoutUIUtils>()->GetFrontDocument();
	if (!iDocument) throw (
		"ComputeAndStorePageNumber_(): Could not get IDocument*.");
 	IDataBase* frontDocDatabase = ::GetDataBase (iDocument);
 	InterfacePtr<IPMUnknown> unknown (
 								frontDocDatabase, inUID, IID_IUNKNOWN);
 	InterfacePtr<IHierarchy> hierarchy (unknown, UseDefaultIID());
	if (hierarchy)
	{
		outAdPageUID = Utils<ILayoutUtils>()->GetOwnerPageUID (hierarchy);
		InterfacePtr<IPMUnknown> unknown2 (frontDocDatabase, outAdPageUID, 
															IID_IUNKNOWN);
		InterfacePtr<IMasterPage> masterPage (unknown2, IID_IMASTERPAGE);
		if (masterPage)
		{
			// Add a string describing the page.
			InterfacePtr<IPageList> pageList (iDocument, UseDefaultIID());
			if (pageList)
			{
				PMString pageStr;
				pageList->GetPageString (outAdPageUID, &pageStr);
				int32 folio = pageStr.GetAsNumber ();
				ioAd->SetPageNumber_(folio);
			}
		}
	}
	else
	{
		// This is bad, but not fatal.
		LOG ("<error>Could not get IHierarchy to compute page number.</error>");
	}

	LOG_END_FUNCTION;
}


//----------------------------------------------------------------------------------
//	ComputeAndStoreAdGeometry_
//----------------------------------------------------------------------------------
PMPoint CL_WriteAdReportWorkhorse::ComputeAndStoreAdGeometry_(UIDRef inUIDRef,
									UID & inAdPageUID, PBPMRect bBoxPasteboard) const throw (char*)
{
	LOG_BEGIN_FUNCTION;
	
// 	// Page geometry:
	IDocument* iDocument = Utils<ILayoutUIUtils>()->GetFrontDocument();
	if (!iDocument) 
	{
		// Exception will be caught way up in ScanDocument_() which 
		// will stop the document scan and put up an alert.
		throw ("ComputeAndStoreAdGeometry_(): "
				"Could not get front document (IDocument).");
	}
	
	IDataBase* database = ::GetDataBase (iDocument);
	if (!database) 
	{
		// Exception will be caught way up in ScanDocument_() which 
		// will stop the document scan and put up an alert.
		throw ("ComputeAndStoreAdGeometry_(): "
				"Could not get document database (IDataBase).");
	}

 	UIDRef pageUidRef (database, inAdPageUID);
 	InterfacePtr<IGeometry> pageGeometry (pageUidRef, UseDefaultIID());
	if (!pageGeometry)
	{
		const char* errorStr = "ComputeAndStoreAdGeometry_(): "
								"Could not get page geometry.";
		LOG (errorStr);
		throw errorStr;
	}

    PBPMRect bBoxPagePasteboard = pageGeometry->GetStrokeBoundingBox (::InnerToPasteboardMatrix(pageGeometry));

	// Translate object's x1,y1
	PMReal left = bBoxPasteboard.Left () - bBoxPagePasteboard.Left ();
	PMReal top  = bBoxPasteboard.Top () - bBoxPagePasteboard.Top ();
	
	// Compute width and height of object even though we don't need them
//	PMReal width = bBoxPasteboard.Right () - bBoxPasteboard.Left ();
//	PMReal height = bBoxPasteboard.Bottom () - bBoxPasteboard.Top ();
	
	// Set x1,y1
//	ioAd->SetBoxX_( left );
//	ioAd->SetBoxY_( top );

	LOG_END_FUNCTION;
    return PMPoint (left, top);
}


//----------------------------------------------------------------------------------
//	Log1Object_
//----------------------------------------------------------------------------------
void CL_WriteAdReportWorkhorse::Log1Object_(IPMUnknown* inObj, 
										bool callerWantsBlackBoxDataLogged)
{
	LOG_BEGIN_FUNCTION;

	// Compute and log item's UID
	char cstr[256];
	UID uid = ::GetUID (inObj);
	::sprintf (cstr, "0x%x", uid.Get());
	LOG_TAG_AND_VALUE ("ItemUID", cstr);

	// Compute and log item's class
	ClassID unknownClassID = ::GetClass (inObj);
	if (unknownClassID == kDocBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kDocBoss");
	}
	else if (unknownClassID == kSpreadBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kSpreadBoss");
	}
	else if (unknownClassID == kPageBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kPageBoss");
	}
	else if (unknownClassID == kMasterPagesBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kMasterPagesBoss");
	}
	else if (unknownClassID == kDocumentLayerBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kDocumentLayerBoss");
	}
	else if (unknownClassID == kSpreadLayerBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kSpreadLayerBoss");
	}
	else if (unknownClassID == kTextStoryBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kTextStoryBoss");
	}
	else if (unknownClassID == kPageItemBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kPageItemBoss");
	}
	else if (unknownClassID == kMultiColumnItemBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kMultiColumnItemBoss");
	}
	else if (unknownClassID == kFrameItemBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kFrameItemBoss");
	}
	else if (unknownClassID == kDrawablePageItemBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kDrawablePageItemBoss");
	}
	else if (unknownClassID == kSplineItemBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kSplineItemBoss");
	}
	else if (unknownClassID == kImageBaseItem)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kImageBaseItem");
	}
	else if (unknownClassID == kImageItem)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kImageItem");
	}
	else if (unknownClassID == kPlacedPDFItemBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kPlacedPDFItemBoss");
	}
	else if (unknownClassID == kEPSTextItemBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kEPSTextItemBoss");
	}
	else if (unknownClassID == kDisplayListPageItemBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kDisplayListPageItemBoss");
	}
	else if (unknownClassID == kEPSItem)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kEPSItem");
	}
	else if (unknownClassID == kPICTItem)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kPICTItem");
	}
	else if (unknownClassID == kWMFItem)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kWMFItem");
	}
	else if (unknownClassID == kDCSItemBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kDCSItemBoss");
	}
	else if (unknownClassID == kGroupItemBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kGroupItemBoss");
	}
	else if (unknownClassID == kTOPSplineItemBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kTOPSplineItemBoss");
	}
	else if (unknownClassID == kGuideItemBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kGuideItemBoss");
	}
	else if (unknownClassID == kInlineBoss)
	{
		LOG_TAG_AND_VALUE ("ItemType", "kInlineBoss");
	}
	else
	{
		LOG ("<!-- Unknown item type -->");
		::sprintf (cstr, "0x%x", unknownClassID.Get());
		LOG_TAG_AND_VALUE ("ItemUID", cstr);
	}

	if (callerWantsBlackBoxDataLogged)
	{
		char* buffer;
		GetBlackBoxData_(inObj, &buffer);
		if ( ::strlen (buffer) )
		{
			LOG_TAG_AND_VALUE ("BlackBoxData", buffer);
		}
		else
		{
			LOG ("<!-- No BlackBoxData present. -->");
		}

		delete [] buffer;
	}
	
	LOG_END_FUNCTION;
}

#pragma mark --------------- WRITE OUTPUT FILE ---------------
/*================================================================================*/
void CL_WriteAdReportWorkhorse::WriteReport_() {
/*----------------------------------------------------------------------------------
	Abstract:
		After the document database scan, this function writes the stored data 
		about the ad objects to a user-specified output file.
		
	Parameters and modes:
		None.
		
	Returns:
		None.
		
	Change Log:
		30-JUN-11	Change file handling from Mac Classic file objects and I/O 
					to standard C++ file objects and I/O.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;

	// Debugging code
#if VERSION == IN_HOUSE && DUMP_PLACED_ADS
	extern void dump_ad (CL_Display_Ad ad);		// CL_Geometry_Importer.cpp
	LOG ("<DumpPlacedAdsVector>");
	char logstr[256];
	sprintf (logstr, "<!-- Placed ads vector contains: %d elements. -->", 
				placedAdsVector_.size ());
	LOG (logstr);
	for_each (placedAdsVector_.begin (), placedAdsVector_.end (), dump_ad );
	LOG ("</DumpPlacedAdsVector>");
#endif
	// End debugging code

// OLD CS2 CODE:
	// Have the user specify the output file.  ChooseOutputFile_ puts up the 
	// dialog and if the user OKs, stores the file info in the reportFile_ 
	// member and returns true.  If he Cancels, it just returns false.
// 	bool doesFileExist;
// 	if (ChooseOutputFile_(doesFileExist))
// 	{
// 		// Try to create (or if it already exists, open) the file.
// 		OSErr err = noErr;
// 		if (!doesFileExist)										// Already on disk?
// 		{														// No, so create
// 			err = ::FSpCreate (&outputFileMacFSSpec_, 'R*ch', 	// BBEdit file
// 								'TEXT', smSystemScript);
// 			if (err)
// 			{
// 				char errorStr[256];
// 				::sprintf (errorStr, "WriteReport_(): Could not create file.  "
// 										"Error = %d", err);
// 				GiveAlert_(errorStr);
// 				return;
// 			}
// 		}
// 		
// 		if (!err)
// 		{
// 			// No problems -- open the file.
// 			short reportFileRefNum;
// 			err = ::FSpOpenDF (&outputFileMacFSSpec_, 
// 									fsRdWrPerm, &reportFileRefNum);
// 			if (err)
// 			{
// 				char errorStr[256];
// 				::sprintf (errorStr, "WriteReport_(): Could not open file.  "
// 										"Error = %d", err);
// 				GiveAlert_(errorStr);
// 				return;
// 			}
// 			err = ::SetEOF (reportFileRefNum, 0);
// 			if (err)
// 			{
// 				char errorStr[256];
// 				::sprintf (errorStr, "WriteReport_(): Could not set file EOF "
// 										"pointer.  Error = %d", err);
// 				GiveAlert_(errorStr);
// 				return;
// 			}
// 
// 			long byteCount = 0;
// 
// 			// Loop over the ads and write them out
// 			string adRecord;
// 			const char *cstr;
// 			vector<CL_Display_Ad>::iterator p = placedAdsVector_.begin ();
// 			while (p < placedAdsVector_.end ())
// 			{
// 				p->FormatForAdReport_(adRecord);
// 				cstr = adRecord.c_str ();
// 				byteCount = adRecord.length ();
// 				::FSWrite (reportFileRefNum, &byteCount, cstr);
// 				++p;
// 			}
// 
// 			placedAdsVector_.clear ();		// Return borrowed memory to free store.
// 
// 			err = ::FSClose (reportFileRefNum);
// 			if (err)
// 			{
// 				char errorStr[256];
// 				::sprintf (errorStr, "WriteReport_(): File did not close normally. "
// 										" Error = %d", err);
// 				GiveAlert_(errorStr);
// 				return;
// 			}
// 		}
// 	}
 	
 	std::string fullPath;
 	if (ChooseOutputFile_(fullPath))
 	{
 		LOG_TAG_AND_VALUE ("InDesignFullPath", fullPath.c_str());
 		
 		// Change Mac Classic path to UNIX
 		fullPath = "/Volumes/" + fullPath;
 		std::string::iterator iter;
 		for (iter = fullPath.begin (); iter < fullPath.end (); iter++)
 		{
 			if (*iter == ':')
 				*iter = '/';
 		}
 		LOG_TAG_AND_VALUE ("UNIXFullPath", fullPath.c_str());
 		
 		// Try to open the file and if successful, write the ad objects into it.
 		std::fstream reportFile;
		reportFile.open (fullPath.c_str(), fstream::out);
		if (reportFile.is_open ())
		{
			std::string adRecord;
			const char *cstr;
			std::vector<CL_Display_Ad>::iterator p = placedAdsVector_.begin ();
			while (p < placedAdsVector_.end ())
			{
				p->FormatForAdReport_(adRecord);
				cstr = adRecord.c_str ();
				reportFile << cstr << endl;
				reportFile.flush ();
				++p;
			}
			reportFile.close ();
		}
		else
		{
			std::string errorMsg = "Unable to open report file: ";
			errorMsg += fullPath;
			GiveAlert_( errorMsg.c_str () );
		}
 	}
 	// No need for else {} because user cancelled.
 	
	// Loop over the ads and write them out to the log file in any case
	// (i.e., even if the user cancelled the Save File dialog, write out 
	// whatever data we gathered from the document):
	LOG ("<!-- Contents of ad report follows. -->");
	LOG ("<PlacedAdsList>");
	std::string adRecord;
	const char *cstr;
	std::vector<CL_Display_Ad>::iterator p = placedAdsVector_.begin ();
	while (p < placedAdsVector_.end ())
	{
		p->FormatForAdReport_(adRecord);
		cstr = adRecord.c_str ();
		LOG_TAG_AND_VALUE ("PlacedAd", cstr);
		++p;
	}
	LOG ("</PlacedAdsList>");
	placedAdsVector_.clear ();		// Return borrowed memory to free store.
	
	LOG_END_FUNCTION;
}


/*================================================================================*/
bool CL_WriteAdReportWorkhorse::ChooseOutputFile_(std::string & ioFullPath) {
/*----------------------------------------------------------------------------------
	Abstract:
		Display an InDesign "Save File" dialog for the user to specify 
		the ad report (output) file.
		
	Parameters and modes:
		ioFullPath	The path to the output file specified.  This path is	IN/OUT
					in Mac classic format (uses colon for separator).
	Returns:
		TRUE if the user specified and file and clicked OK; FALSE if the user 
		chose the Cancel button in the dialog.
		
	Change Log:
		30-JUN-11	Ported to CS4.
----------------------------------------------------------------------------------*/
	ioFullPath.clear ();
	SDKFileSaveChooser outputFileChooser;
	PMString pmsTitle ("Save Report As:");					// Setup dialog title
	pmsTitle.SetTranslatable (kFalse);
//	PMString pmsFilterTxtFiles ("TEXT files (txt)");		// Setup file filter
//	pmsFilterTxtFiles.SetTranslatable (kFalse);
//	outputFileChooser.AddFilter ('TEXT', "txt", pmsFilterTxtFiles);
	PMString pmsFilterTextFiles ("TEXT files (text)");
	pmsFilterTextFiles.SetTranslatable (kFalse);
	outputFileChooser.AddFilter (kTEXTFileTypeInfoID, pmsFilterTextFiles);
	
	// Add filter that allows the user to turn off filtering and show all files.
//	outputFileChooser.AddAllFiles ();

	outputFileChooser.SetTitle (pmsTitle);
	outputFileChooser.SetFilename ("Untitled Ad Report");
	
	// Now put the dialog on the screen.  Runs until user OKs or Cancels.
	outputFileChooser.ShowDialog ();

	if ( outputFileChooser.IsChosen () )
	{
		// Save the InDesign file object and fill in a Mac FSSpec for the file.
// 		reportFile_ = outputFileChooser.GetIDFile ();
// 		outputFileMacFSSpec_.vRefNum = reportFile_.GetVRefNum ();
// 		outputFileMacFSSpec_.parID = reportFile_.GetParId ();
// 		PMString pmsFileName = reportFile_.GetFSSpecName ();
// 		pmsFileName.GetPString (outputFileMacFSSpec_.name, 64);
		PMString filePath = outputFileChooser.GetPath ();
		LOG_TAG_AND_VALUE ("outputFileChooser", 
			filePath.GetPlatformString().c_str());
		// Next line is a porting change for CS4 and shows the new, preferred 
		// way of getting a C pointer to the string in the PMString.
		ioFullPath = filePath.GetPlatformString().c_str();
//		ioDoesFileExist = outputFileChooser.IsExisting ();	DON'T BOTHER ANYMORE
		return true;
	}

	return false;
}


// This is really just dead CS2 code.  I could delete it but who knows whether 
// I might need it again.
bool CL_WriteAdReportWorkhorse::ChooseOutputFile_(
					bool & ioDoesFileExist)
{
	SDKFileSaveChooser outputFileChooser;
	PMString pmsTitle ("Save Report As:");					// Setup dialog title
	pmsTitle.SetTranslatable (kFalse);
//	PMString pmsFilterTxtFiles ("TEXT files (txt)");		// Setup file filter
//	pmsFilterTxtFiles.SetTranslatable (kFalse);
//	outputFileChooser.AddFilter ('TEXT', "txt", pmsFilterTxtFiles);
	PMString pmsFilterTextFiles ("TEXT files (text)");
	pmsFilterTextFiles.SetTranslatable (kFalse);
	outputFileChooser.AddFilter (kTEXTFileTypeInfoID, pmsFilterTextFiles);
	
	// Add filter that allows the user to turn off filtering and show all files.
//	outputFileChooser.AddAllFiles ();

	outputFileChooser.SetTitle (pmsTitle);
	outputFileChooser.SetFilename ("Untitled Ad Report");
	
	// Now put the dialog on the screen.  Runs until user OKs or Cancels.
	outputFileChooser.ShowDialog ();

	if ( outputFileChooser.IsChosen () )
	{
		// Save the InDesign file object and fill in a Mac FSSpec for the file.
// 		reportFile_ = outputFileChooser.GetIDFile ();
// 		outputFileMacFSSpec_.vRefNum = reportFile_.GetVRefNum ();
// 		outputFileMacFSSpec_.parID = reportFile_.GetParId ();
// 		PMString pmsFileName = reportFile_.GetFSSpecName ();
// 		pmsFileName.GetPString (outputFileMacFSSpec_.name, 64);
		PMString filePath = outputFileChooser.GetPath ();
		LOG_TAG_AND_VALUE ("outputFileChooser", 
			filePath.GetPlatformString().c_str());
		ioDoesFileExist = outputFileChooser.IsExisting ();
		return true;
	}

	return false;
}

// END OF FILE

