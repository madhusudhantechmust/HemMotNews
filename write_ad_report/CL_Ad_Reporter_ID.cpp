/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	CL_Ad_Reporter_ID.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	1/17/07 ca. 8:47 PM
	
	ab	Abstract:
	ab	Implementation of CL_Ad_Reporter_ID class.
	ab	
	
	cl	Change Log:
	cl	1/17/07	New.
------------------------------------------------------------------------------*/

#include "IDocument.h"
#include "IDataBase.h"
#include "IHierarchy.h"
#include "LayoutUIUtils.h"
#include "ISpread.h"
#include "ISpreadList.h"
#include "IPathUtils.h"
#include "SplineID.h"
#include "IFrameType.h"
#include "IPageItemTypeUtils.h"
#include "IBlackBoxCommands.h"
#include "IGeometry.h"
#include "TransformUtils.h"		// for ComputeAndStoreAdGeometry_()
#include "LayoutUIUtils.h"
#include "IPageList.h"
#include "IMasterPage.h"
#include "SDKFileHelper.h"

#include "WFPID.h"
#include "CL_Ad_Reporter_ID.h"
#include "CL_Log_File.h"
#include "global_data.h"

//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and D E S T R U C T I O N
//----------------------------------------------------------------------------------

CL_Ad_Reporter_ID::CL_Ad_Reporter_ID ()
{
	placedAdsVector_.reserve (200);
}

CL_Ad_Reporter_ID::~CL_Ad_Reporter_ID ()
{

}


//----------------------------------------------------------------------------------
// C O P Y  and  A S S I G N
//----------------------------------------------------------------------------------
																		// Copy
CL_Ad_Reporter_ID::CL_Ad_Reporter_ID (const CL_Ad_Reporter_ID & inObj)
{

}

																		// Assign
CL_Ad_Reporter_ID & CL_Ad_Reporter_ID::operator= (const CL_Ad_Reporter_ID & inObj)				
{
	if (this != &inObj)									// Beware of "a = a"
	{
		// put some code here
	}

	return *this;
}

#pragma mark -
/*================================================================================*/
void CL_Ad_Reporter_ID::ScanDocument_() {
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
		IDocument *iDocument = ::GetFrontDocument();
		if (iDocument == nil)
			throw "ScanDocument_(): No front document!";
		
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
void CL_Ad_Reporter_ID::WriteReport_() {
/*----------------------------------------------------------------------------------
	Abstract:
		After the document database scan, this function writes the stored data 
		about the ad objects to a user-specified output file.
		
	Parameters and modes:
		None.
		
	Returns:
		None.
		
	Change Log:
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;

	// Debugging code
#define		DUMP_PLACED_ADS		0
#if VERSION == IN_HOUSE && DUMP_PLACED_ADS
	extern void dump_ad (CL_Display_Ad ad);		// CL_Geometry_Importer.cpp
	LOG ("<DumpPlacedAdsVector>");
	char logstr[256];
	sprintf (logstr, "<!-- Placed ads vector contains: %d elements. -->", 
				placedAdsVector_.size ());
	LOG (logstr);
	for_each (placedAdsVector_.begin (), 
				placedAdsVector_.end (), dump_ad );
	LOG ("</DumpPlacedAdsVector>");
#endif
	// End debugging code
	
	// Have the user specify the output file.  ChooseOutputFile_ puts up the 
	// dialog and if the user OKs, stores the file info in the reportFile_ 
	// member and returns true.  If he Cancels, it just returns false.
	bool doesFileExist;
	if (ChooseOutputFile_(doesFileExist))
	{
		// Try to create (or if it already exists, open) the file.
		OSErr err = noErr;
		if (!doesFileExist)										// Already on disk?
		{														// No, so create
			err = ::FSpCreate (&outputFileMacFSSpec_, 'R*ch', 	// BBEdit file
								'TEXT', smSystemScript);
			if (err)
			{
				char errorStr[256];
				::sprintf (errorStr, "WriteReport_(): Could not create file.  "
										"Error = %d", err);
				GiveAlert_(errorStr);
				return;
			}
		}
		
		if (!err)
		{
			// No problems -- open the file.
			short reportFileRefNum;
			err = ::FSpOpenDF (&outputFileMacFSSpec_, 
									fsRdWrPerm, &reportFileRefNum);
			if (err)
			{
				char errorStr[256];
				::sprintf (errorStr, "WriteReport_(): Could not open file.  "
										"Error = %d", err);
				GiveAlert_(errorStr);
				return;
			}
			err = ::SetEOF (reportFileRefNum, 0);
			if (err)
			{
				char errorStr[256];
				::sprintf (errorStr, "WriteReport_(): Could not set file EOF "
										"pointer.  Error = %d", err);
				GiveAlert_(errorStr);
				return;
			}

			long byteCount = 0;

			// Loop over the ads and write them out
			string adRecord;
			const char *cstr;
			vector<CL_Display_Ad>::iterator p = placedAdsVector_.begin ();
			while (p < placedAdsVector_.end ())
			{
				p->FormatForAdReport_(adRecord);
				cstr = adRecord.c_str ();
				byteCount = adRecord.length ();
				::FSWrite (reportFileRefNum, &byteCount, cstr);
				++p;
			}

			placedAdsVector_.clear ();		// Return borrowed memory to free store.

			err = ::FSClose (reportFileRefNum);
			if (err)
			{
				char errorStr[256];
				::sprintf (errorStr, "WriteReport_(): File did not close normally. "
										" Error = %d", err);
				GiveAlert_(errorStr);
				return;
			}
		}
	}
	
	LOG_END_FUNCTION;
}

bool CL_Ad_Reporter_ID::ChooseOutputFile_(bool & ioDoesFileExist)
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
		reportFile_ = outputFileChooser.GetIDFile ();
		outputFileMacFSSpec_.vRefNum = reportFile_.GetVRefNum ();
		outputFileMacFSSpec_.parID = reportFile_.GetParId ();
		PMString pmsFileName = reportFile_.GetFSSpecName ();
		pmsFileName.GetPString (outputFileMacFSSpec_.name, 64);
		ioDoesFileExist = outputFileChooser.IsExisting ();
		return true;
	}

	return false;
}


void CL_Ad_Reporter_ID::ScanSpread_(IDocument* inIDocumentPtr, 
					ISpreadList* inSpreadListPtr, int inSpreadNum)
					throw (char*)
{
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
void CL_Ad_Reporter_ID::ProcessPageItems_(UIDList & inPageItemList) {
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
//    	ProcessPageItem_( inPageItemList[k], pageItemUIDRef );
		InterfacePtr<IHierarchy> pageItemHierarchy (pageItemUIDRef, UseDefaultIID());
		ProcessPageItem_(inPageItemList[k], pageItemUIDRef, pageItemHierarchy);
    }
	LOG_END_FUNCTION;
}


void CL_Ad_Reporter_ID::ProcessPageItem_(UID inItemUID, 
						UIDRef inPageItemUIDRef, IHierarchy* inPageItemIHierarchy)
{
	LOG_BEGIN_FUNCTION;

	if (!inPageItemIHierarchy)
		return;
	
//	TESTING ONLY	Log1Object_(inPageItemIHierarchy);
	
	if ( ::GetClass (inPageItemIHierarchy) == kSplineItemBoss )
    {
		if (Utils<IPageItemTypeUtils>()->IsTextFrame (inPageItemIHierarchy))
		{
			LOG ("<!-- Object is a TEXT frame. IGNORE. -->");
		}
		else
		{
			LOG ("<!-- Object is a PICTURE frame -->");
			
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
//					CL_Display_Ad* ad = new CL_Display_Ad (adData);
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
					LOG ("<!-- NO OBJECT GEOMETRY -->");
					LOG_END_FUNCTION;
					return;
				}
				
				UID adPageUID = kInvalidUID;
				ComputeAndStorePageNumber_(inItemUID, inPageItemUIDRef, 
														ad.get(), adPageUID);
				ComputeAndStoreAdGeometry_(inPageItemUIDRef, adPageUID, 
													objectGeometry, ad.get());
				
				placedAdsVector_.push_back (*ad);		// Copy ad into vector
			}
			else
			{
				LOG ("<!-- No BlackBoxData present. IGNORE. -->");
			}
		}
    }
    
	int32 childCount = inPageItemIHierarchy->GetChildCount ();
	for (int32 childIndex = 0; childIndex < childCount; childIndex++)
	{
		UID childUID = inPageItemIHierarchy->GetChildUID (childIndex);
		UIDRef childUIDRef (::GetDataBase (::GetFrontDocument ()), childUID );
		InterfacePtr<IHierarchy> child (
								inPageItemIHierarchy->QueryChild (childIndex));
		ProcessPageItem_(childUID, childUIDRef, child);
	}

	LOG_END_FUNCTION;
}


void CL_Ad_Reporter_ID::ProcessPageItem_(UID inItemUID, UIDRef inPageItemUIDRef)
{
	LOG_BEGIN_FUNCTION;
	InterfacePtr<IHierarchy> pageItemHierarchy (inPageItemUIDRef, UseDefaultIID());
	if (pageItemHierarchy)
	{
		LOG ("<!-- Page item has a hierarchy -->");
		Log1Object_(pageItemHierarchy, true);
		VisitChildren_(pageItemHierarchy);
	}
	else
	{
		LOG ("<!-- Page item has no hierarchy IGNORE IT -->");
	}
	LOG_END_FUNCTION;
}


/*================================================================================*/
void CL_Ad_Reporter_ID::VisitChildren_(IHierarchy* parent) {
/*----------------------------------------------------------------------------------
	Abstract:
		This thing is called by the top-level item in a hierarchy as well as 
		by itself recursively to examine the children in the hierarchy.
		
	Parameters and modes:
		The parent of the children to examine or process.
		
	Returns:
		Nothing.
		
	Change Log:
----------------------------------------------------------------------------------*/
	int32 childCount = parent->GetChildCount ();
	if ( !childCount )
		return;
	
	LOG_BEGIN_FUNCTION;
	DECLARE_logstr;
	UID parentUid = ::GetUID (parent);
	for (int32 childIndex = 0; childIndex < childCount; childIndex++)
	{
		InterfacePtr<IHierarchy> child (parent->QueryChild (childIndex));
		UID childUid = ::GetUID (child);
		::sprintf (logstr, "<ParentUID children=%d>0x%x</ParentUID>", 
						childCount, parentUid);
		LOG (logstr);
		Log1Object_(child, true);
		if ( ::GetClass (child) == kSplineItemBoss )
        {
			UIDRef childUidRef = ::GetUIDRef (child);
			if (Utils<IPageItemTypeUtils>()->IsTextFrame (childUidRef))
			{
				::sprintf (logstr, 
						"<ChildUID type=\"text_frame\">0x%x</ChildUID>", childUid);
				LOG (logstr);
			}
			else
			{
				// Might be a graphic frame -- no way to tell for certain because 
				// unless it has a picture placed in it, it won't have children.  
				// If it does contain a picture, it has a kImageItem child.  So 
				// we wind up just checking for our black box data.
				::sprintf (logstr, 
						"<ChildUID type=\"spline_item\">0x%x</ChildUID>", childUid);
				LOG (logstr);

				// Was this graphic frame created by NewsCAD?
				char* buffer = new char [512];
				if (!buffer)
				{
					LOG ("Out of memory!");
					LOG_END_FUNCTION;
				}
				int32 len = 0;
				InterfacePtr<IBlackBoxCommands>blackBoxCmds (
								Utils<IBlackBoxCommands>().QueryUtilInterface());
				len = blackBoxCmds->GetData (childUidRef, kWFPPluginID, 
												buffer, 511);
				if ( len )
				{
					*(buffer + len) = '\0';
					LOG_TAG_AND_VALUE ("PictureBox", buffer);
					string adData = buffer;
					
					// Use std::auto_ptr just in case ComputeAndStorePageNumber_()
					// or ComputeAndStoreAdGeometry_() throws an exception.
//					CL_Display_Ad* ad = new CL_Display_Ad (adData);
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
													childUidRef, UseDefaultIID());
					if (!objectGeometry)
					{
						LOG ("<!-- NO OBJECT GEOMETRY -->");
						LOG_END_FUNCTION;
						return;
					}
					
					UID adPageUID = kInvalidUID;
					ComputeAndStorePageNumber_(childUid, childUidRef, 
															ad.get(), adPageUID);
					ComputeAndStoreAdGeometry_(childUidRef, adPageUID, 
														objectGeometry, ad.get());
					
					placedAdsVector_.push_back (*ad);		// Copy ad into vector
				}
			}
        }

		// Drill down through the hierarchy.
		VisitChildren_(child);
	}
	LOG_END_FUNCTION;
}


//----------------------------------------------------------------------------------
//	ComputeAndStoreAdGeometry_
//----------------------------------------------------------------------------------
void CL_Ad_Reporter_ID::ComputeAndStoreAdGeometry_(UIDRef inUIDRef, 
									UID & inAdPageUID, IGeometry* pIGeometry, 
									CL_Display_Ad* ioAd) const throw (char*)
{
	LOG_BEGIN_FUNCTION;
	
	// Ad geometry:
	PBPMRect bBoxPasteboard = pIGeometry->GetStrokeBoundingBox (
							::InnerToPasteboardMatrix (pIGeometry));
	
	// Page geometry:
	IDocument* iDocument = ::GetFrontDocument ();			// LayoutUIUtils.h
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

	PBPMRect bBoxPagePasteboard = pageGeometry->GetStrokeBoundingBox (
										::InnerToPasteboardMatrix (pageGeometry));

	// Translate object's x1,y1
	PMReal left = bBoxPasteboard.Left () - bBoxPagePasteboard.Left ();
	PMReal top  = bBoxPasteboard.Top () - bBoxPagePasteboard.Top ();
	
	// Compute width and height of object even though we don't need them
	PMReal width = bBoxPasteboard.Right () - bBoxPasteboard.Left ();
	PMReal height = bBoxPasteboard.Bottom () - bBoxPasteboard.Top ();
	
	// Set x1,y1
	ioAd->SetBoxx_( left );
	ioAd->SetBoxy_( top );
	
	LOG_END_FUNCTION;
}


//----------------------------------------------------------------------------------
//	ComputeAndStorePageNumber_
//----------------------------------------------------------------------------------
void CL_Ad_Reporter_ID::ComputeAndStorePageNumber_(UID inUID, UIDRef inUIDRef, 
									CL_Display_Ad* ioAd, UID & outAdPageUID) const
{
	LOG_BEGIN_FUNCTION;
	
	// Figure out which page and spread contains the ad.  We need 
	// only the page's UID to perform subsequent operations.  The rest of 
	// the code is to put the page number into the log file.
	IDocument *iDocument = ::GetFrontDocument ();
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


#include "ImageID.h"
#include "IDocument.h"
#include "IDocumentLayer.h"
#include "PDFID.h"
#include "EPSID.h"
#include "GroupID.h"
#include "TextOnPathID.h"
#include "GuideID.h"

//----------------------------------------------------------------------------------
//	Log1Object_
//----------------------------------------------------------------------------------
void CL_Ad_Reporter_ID::Log1Object_(IPMUnknown* inObj, 
										bool callerWantsBlackBoxDataLogged)
{
	LOG_BEGIN_FUNCTION;

	// Compute and log item's UID
	char cstr[256];
	UID uid = ::GetUID (inObj);
	::sprintf (cstr, "0x%x", uid);
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


/*================================================================================*/
void CL_Ad_Reporter_ID::GetBlackBoxData_(IPMUnknown* inObj, char** ioBuffer) {
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
	int32 len = blackBoxCmds->GetData (inObjUidRef, kWFPPluginID, 
									buffer, 511);
	if ( len )
	{
		*(buffer + len) = '\0';
	}
	
	*ioBuffer = buffer;
	
	LOG_END_FUNCTION;
}



// END OF FILE


