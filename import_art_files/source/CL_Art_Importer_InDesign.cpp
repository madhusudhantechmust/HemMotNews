/* 
	CL_Art_Importer_InDesign.cpp

	Author:			
	Description:	<describe the CL_Art_Importer_InDesign class here>
*/

#include "IDocument.h"
#include "IDataBase.h"
#include "IHierarchy.h"
#include "IPathUtils.h"
#include "IGroupItemUtils.h"
#include "IBlackBoxCommands.h"
#include "IImportExportFacade.h"
#include "SDKFileHelper.h"

#include "IFrameType.h"
#include "GroupID.h"

#include "IPageItemTypeUtils.h"

#include "PageItemScrapID.h"		// kDeleteCmdBoss for DeleteTextFrame_()
#include "CmdUtils.h"
#include "ErrorUtils.h"

#include "IReplaceCmdData.h"
#include "IBoolData.h"
#include "IUIDData.h"
#include "IPlaceGun.h"
#include "CAlert.h"					// For debugging only
#include "IImportResourceCmdData.h"	// Experimental

using namespace std;
#include <string>

#include "CL_NewsCAD_Prefs.h"
#include "global_data.h"
#include "CL_Display_Ad.h"
#include "CL_Art_Importer_InDesign.h"
#include "ImportArtID.h"

#include "IURIUtils.h"				// Experimental

CL_Art_Importer_InDesign::CL_Art_Importer_InDesign()
	:	graphicsPath_(NULL)
	,	graphicsFileName_(NULL)
{
}


CL_Art_Importer_InDesign::~CL_Art_Importer_InDesign()
{
	delete [] graphicsPath_;
	delete [] graphicsFileName_;
}


/*================================================================================*/
void CL_Art_Importer_InDesign::ProcessPageItems_(UIDList & inPageItemList) {
/*----------------------------------------------------------------------------------
	Abstract:
		This method serves as a sort of "traffic director."  It takes a list of 
		items on a spread and goes through them looking for frames into which 
		graphics are to be imported.
		
	Parameters and modes:
		inPageItemList	An InDesign data structure that contains an InDesign 	IN
						Unique IDentifier for each item on the spread.
						
	Returns:
		Nothing.
		
	Change Log:
		31-Oct-06	New.  Happy Halloween!
	
	pageItemList			inPageItemList
	pageItemLen				countPageItemsToProcess
	
----------------------------------------------------------------------------------*/
	int32 countPageItemsToProcess = inPageItemList.Length ();
	IDataBase* db = inPageItemList.GetDataBase ();
	UIDRef pageItemUIDRef = UIDRef::gNull;
	bool status = false;
	
	LOG_BEGIN_FUNCTION;
	
    for (int32 k = countPageItemsToProcess - 1; k >= 0; k--)
    {
    	pageItemUIDRef = inPageItemList.GetRef (k);
    	
		LOG ("Got pageItemUIDRef");
		
		// Get the hierarchy so we can check if this is a group
        InterfacePtr<IHierarchy> groupNode (pageItemUIDRef, IID_IHIERARCHY);
        if (groupNode && ::GetClass (groupNode) == kGroupItemBoss)
        {
        	// Drill down through the group and import graphics where needed
        	status = ProcessGroup_(db, pageItemUIDRef, groupNode);
			if (status)
			{
				// Hmmm ... I don't know, maybe nothing needed here.
			}
        }
        else
        {
        	// See if it's a picture frame
			InterfacePtr<IFrameType> frameType (pageItemUIDRef, UseDefaultIID());
			if ( frameType && frameType->IsGraphicFrame () )
			{
				status = Import1Picture_(pageItemUIDRef, db);
	        	if (status)
	        	{
					// Hmmm ... I don't know, maybe nothing needed here.
	        	}
        	}
        }
    }

	LOG_END_FUNCTION;
}


/*================================================================================*/
bool CL_Art_Importer_InDesign::ProcessGroup_(IDataBase* pIDataBase, 
						UIDRef & inGroupUIDRef, IHierarchy* inGroupNode) {
/*----------------------------------------------------------------------------------
	Abstract:
		Examine a group and determine whether a picture should be imported.
		
	Parameters and modes:
		pIDataBase
		inGroupNode

	Returns:
		true if something was imported.
		
	Change Log:
		06-Nov-06	New.
----------------------------------------------------------------------------------*/
	char*	buffer = new char [512];
	int32	len = 0;
	bool	status = false;

	LOG_BEGIN_FUNCTION;
	
	if (!buffer)
	{
		LOG ("Out of memory!");
		LOG_END_FUNCTION;
		return status;
	}
	
	// Was this group created by NewsCAD?
	InterfacePtr<IBlackBoxCommands>blackBoxCmds (
								Utils<IBlackBoxCommands>().QueryUtilInterface());
	len = blackBoxCmds->GetData (inGroupUIDRef, kNC_PluginID, buffer, 511);
	if ( len )
	{
		// Group has NewsCAD's black box data -- it's one of ours.
		LOG ("Found a NewsCAD group");
		
		// Drill down through the group.  Look for a graphic frame.
		UIDList leafnodes (pIDataBase);
		Utils<IPathUtils>()->GetPathSelectableUIDList (&leafnodes, inGroupNode);
		int32 leafLength = leafnodes.Length();

		UIDRef textFrameRef;			// Safe -- CTOR inits UID to kInvalidUID
		UIDRef graphicFrameRef;			// Ditto -- UID is member of UIDRef
		int32 i;
		for (i = leafLength - 1; i >= 0; i--)
		{
			// Ross, this is where you could inspect for your BlackBoxData to 
			// determine if this is a graphic or text frame and set things like 
			// the path to the graphic that you are going to place. You should 
			// probably set the text and graphic frame refs here so you know which
			// page item to delete and which one to place the image in.
			// I am using a less reliable way to set up my graphic and text 
			// frame references by assuming that if it's not my text frame then 
			// it must be my graphic frame.  This is basically something that I am 
			// using in the context of my sample document.
			if(Utils<IPageItemTypeUtils>()->IsTextFrame(leafnodes.GetRef(i)))
			{
				textFrameRef = leafnodes.GetRef(i);
				LOG ("Found a text frame");
			}
			else
			{
				graphicFrameRef = leafnodes.GetRef(i);
				LOG ("Found a pic frame");
			}
		}
		if (graphicFrameRef.GetUID () == kInvalidUID)
		{
			status = false;		// Couldn't get at a frame to import graphic into.
			LOG ("Could not get graphic frame!");
		}
		else
		{
			status = Import1Picture_(graphicFrameRef, pIDataBase);
		}
		
		// If a picture was successfully imported, ungroup the group 
		// and then attempt to delete the text frame.
		if (status)
		{
			status = Ungroup_(inGroupUIDRef);
			if (status)									// Successfully ungrouped?
			{
				DeleteTextFrame_(textFrameRef);
			}
		}
	}
	LOG_END_FUNCTION;
	return status;
}


//------------------------------------------------------------------------------
//	SetGraphicsFilename_()
//
//	Gets the name of the file to import into this picture box and stores it in 
//	member variable graphicsFileName_.  Constructs an ad object from the 
//	passed in black box data attached to the picture box to load the name.
//------------------------------------------------------------------------------
void CL_Art_Importer_InDesign::SetGraphicsFilename_(char* blackBoxData) {
	LOG_BEGIN_FUNCTION;
	delete [] graphicsFileName_;							// Should be NULL!
	std::string stlStr = blackBoxData;
	CL_Display_Ad* ad = new CL_Display_Ad (stlStr);
	if (ad)
	{
		const char* fname = ad->GetArtFileNameCStr_();
		if (fname)
		{
			int32 fnamlen = ::strlen (fname);
			if (fnamlen)
			{
				graphicsFileName_ = new char [fnamlen];
				if (graphicsFileName_)
				{
					::strcpy (graphicsFileName_, fname);
				}
				else
				{
					LOG ("Couldn\'t allocate graphicsFileName_!");
				}
			}
			else
			{
				LOG ("Graphics filename length is 0!");
			}
		}
		else
		{
			LOG ("No file name in slug!");
		}
	
		delete ad;				// Return to free store
	}
	else
	{
		LOG ("Ad ptr NULL!");
	}
	
	LOG_END_FUNCTION;
}


//------------------------------------------------------------------------------
//	SetGraphicsPath_
//
//	Get the path to the graphics files from the preferences XML file.
//------------------------------------------------------------------------------
void CL_Art_Importer_InDesign::SetGraphicsPath_() {
	LOG_BEGIN_FUNCTION;
	delete [] graphicsPath_;				// Shouldn't be anything there, NULL!
	CL_NewsCAD_Prefs*	pPrefs = gpPrefsObj;	// Ugly, because this has to have been loaded
	if (gpPrefsObj)
	{
		char* path = pPrefs->GraphicsPath_();
		if (path)
		{
			int32 pathlen = ::strlen (path);
			graphicsPath_ = new char [pathlen + 1];
			if (graphicsPath_)
			{
				::strcpy (graphicsPath_, path);
				LOG (graphicsPath_);
			}
			else
			{
				LOG ("281 No memory for graphics path!");
			}
		}
		else
		{
			LOG ("286 No memory for graphics path!");
		}
	}
	else
	{
		LOG ("291 Couldn\'t get ptr to global prefs object!");
	}

	LOG_END_FUNCTION;
}


/*================================================================================*/
bool CL_Art_Importer_InDesign::Import1Picture_(UIDRef & inUidRef, 
												IDataBase* inIDataBasePtr) {
/*----------------------------------------------------------------------------------
	Abstract:
		
	Parameters and modes:

	Returns:

	Change Log:
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	char*	buffer = new char [512];
	int32	len = 0;
	bool	status = false;

	if (!buffer)
	{
		// Put up an alert to tell the user we couldn't allocate memory and exit.
		return status;
	}
	
	// Was this graphic frame created by NewsCAD?
	InterfacePtr<IBlackBoxCommands>blackBoxCmds (
								Utils<IBlackBoxCommands>().QueryUtilInterface());
	len = blackBoxCmds->GetData (inUidRef, kNC_PluginID, buffer, 511);

	if ( len ) {
		LOG ("Our pic frame");
		*(buffer + len) = '\0';
		LOG (buffer);
		SetGraphicsPath_();					// Get path from prefs
		if (!graphicsPath_)
		{
			delete [] buffer;
			return status;					// Failed, error already in log
		}
		
		SetGraphicsFilename_(buffer);		// Get filename from black box data
		if (!graphicsFileName_)
		{
			delete [] buffer;
			return status;					// Failed, error already in log
		}
		
		// Place the graphic file
		PMString graphicsFullPath (graphicsPath_);
		graphicsFullPath.Append(':');
		graphicsFullPath.Append (graphicsFileName_);
		SDKFileHelper sdkFileHelperObj (graphicsFullPath);
		if (sdkFileHelperObj.IsExisting ())
		{
			LOG ( "Graphics file exists");
			LOG ( graphicsFullPath.GetPlatformString().c_str());
			IDFile idFile (graphicsFullPath);

			// Start a command sequence. This is a stack based command sequence 
			// pointer.  I set the state to "failed". If we make it to the end, 
			// then it is set to success.  If it isn't set to success when the 
			// destructor is called, then the command sequence will roll back.
			// (Actually, for NewsCAD, we'll never roll anything back -- this 
			// Adobe sample shows how you'd handle a command interactively, 
			// one-at-a-time.)
		    CmdUtils::SequencePtr cmdSeq;

			// Set the name that shows on the Undo menu
			PMString seqName ("Place Ad");
			seqName.SetTranslatable (kFalse);
			cmdSeq->SetName (seqName);
//			cmdSeq.SetState (kFailure);		// This is "typical" processing.

			// Load the place gun.
            URI tmpURI;
            bool16 bHasClipping = kFalse;
            Utils<IURIUtils>()->IDFileToURI (idFile, tmpURI);
            Utils<Facade::IImportExportFacade> oImportExportFacade;
            ErrorCode adobeStatus = oImportExportFacade->ImportAndLoadPlaceGun (
                                                        inIDataBasePtr, tmpURI, kSuppressUI, kTrue, kFalse, kFalse, bHasClipping, IPlaceGun::kAddToBack);
/*
			InterfacePtr<ICommand> importCmd (
						CmdUtils::CreateCommand (kImportAndLoadPlaceGunCmdBoss));
			if (!importCmd)
			{
				ErrorUtils::PMSetGlobalErrorCode(kFailure);
				status = false;
				CAlert::ModalAlert
				(
					"Failed to get pointer to ICommand -- Unable to import art file.",
					kOKString, 					// OK button
					kNullString, 				// No second button
					kNullString, 				// No third button
					1,							// Set OK button to default
					CAlert::eInformationIcon	// Information icon.
				);
				goto cleanup;
			}
#if 0
			// NEXT LINE FAILS AND PROGRAM GRACEFULLY GIVES UP.
			InterfacePtr<IImportFileCmdData> data (
												importCmd, IID_IIMPORTFILECMDDATA);
			if (!data)
			{
				// PROGRAM COMES HERE.
				ErrorUtils::PMSetGlobalErrorCode(kFailure);
				status = false;
				CAlert::ModalAlert
				(
					"Failed to get pointer to IImportFileCmdData -- Cannot import art file.",
					kOKString, 					// OK button
					kNullString, 				// No second button
					kNullString, 				// No third button
					1,							// Set OK button to default
					CAlert::eInformationIcon	// Information icon.
				);
				goto cleanup;
			}
#endif
// BEGIN 2011 REWRITE
			InterfacePtr<IImportResourceCmdData> importResourceCmdData (
						importCmd, IID_IIMPORTRESOURCECMDDATA); // no DefaultIID for this
			if (!importResourceCmdData)
			{
				ErrorUtils::PMSetGlobalErrorCode(kFailure);
				status = false;
				CAlert::ModalAlert
				(
					"Failed to get pointer to IImportResourceCmdData -- Cannot import art file.",
					kOKString, 					// OK button
					kNullString, 				// No second button
					kNullString, 				// No third button
					1,							// Set OK button to default
					CAlert::eInformationIcon	// Information icon.
				);
				goto cleanup;
			}

			importResourceCmdData->Set (inIDataBasePtr, tmpURI, kSuppressUI);
			ErrorCode err = CmdUtils::ProcessCommand (importCmd);
// END 2011 REWRITE
#if 0
2008 CODE
			data->Set (inIDataBasePtr, idFile, kSuppressUI);
#endif
// This next block is the old CS2 code that no longer compiles.
// 			if (CmdUtils::ProcessCommand (importCmd) != kSuccess)
// 			{
// 				status = false;
// 				goto cleanup;				// Just exit on error
// 			}
			ErrorCode adobeStatus = CmdUtils::ProcessCommand (importCmd);
 */
			if (adobeStatus == kSuccess)
			{
				status = true;
			}
			else
			{
				ErrorUtils::PMSetGlobalErrorCode (adobeStatus);
				status = false;
				goto cleanup;
			}
			
			// Now place the imported file into the graphic frame passed in.
			InterfacePtr<ICommand> replaceCmd (
									CmdUtils::CreateCommand (kReplaceCmdBoss) );
			InterfacePtr<IReplaceCmdData> replaceData (
									replaceCmd, IID_IREPLACECMDDATA);
//			InterfacePtr<IBoolData> hasClippingFrame (importCmd, UseDefaultIID());
			
			// Need to get the place gun here so that we can set 
			// the item that has been loaded for placing
			InterfacePtr<IPlaceGun> placeGun (inIDataBasePtr, 
								inIDataBasePtr->GetRootUID(), UseDefaultIID());
			replaceData->Set (inIDataBasePtr, inUidRef.GetUID (), 
								placeGun->GetFirstPlaceGunItemUID (), // was GetItemUID() in CS2
								bHasClipping);
/*
Old CS2 code
			if (CmdUtils::ProcessCommand(replaceCmd) != kSuccess)
			{
				status = false;
				goto cleanup;				// Just exit on error
			}
*/
			adobeStatus = CmdUtils::ProcessCommand (replaceCmd);
			if (adobeStatus == kSuccess)
			{
				status = true;
			}
			else
			{
				ErrorUtils::PMSetGlobalErrorCode (adobeStatus);
				status = false;
				goto cleanup;
			}
			
			//The place gun might still be loaded, clear the place gun.
			if ( placeGun->IsLoaded () )
			{
				InterfacePtr<ICommand> clearPlaceGun (
								CmdUtils::CreateCommand ( kClearPlaceGunCmdBoss) );
				InterfacePtr <IUIDData> uidData (clearPlaceGun, UseDefaultIID () );
				uidData->Set (placeGun);
				if (CmdUtils::ProcessCommand (clearPlaceGun) != kSuccess)
				{
					status = false;
					goto cleanup;				// Just exit on error
				}
			}
			
// ARCHAIC 2008			cmdSeq.SetState (kSuccess);

			status = true;					// Indicate to caller that all went well
		}
		else
		{
			LOG ( "Requested file does not exist");
			LOG ( graphicsFullPath.GetPlatformString().c_str() );
			status = false;					// Failed to import anything
		}
	}
	else {
		LOG ("Not NewsCAD\'s pic frame");
		status = false;						// Nothing to do -- no slug (black box)
	}

cleanup:
	LOG_END_FUNCTION;
	delete [] buffer;
	return status;
}


/*================================================================================*/
bool CL_Art_Importer_InDesign::Ungroup_(UIDRef & inGroupUIDRef) {
/*----------------------------------------------------------------------------------
	Abstract:
		This will ungroup the picture and text box that comprise the ad after the 
		picture has been imported.
		
	Parameters and modes:
		inGroupUIDRef	InDesign reference to unique ID of group to ungroup.	IN
		
	Returns:
		true, if ungrouping was successful.
		
	Change Log:
----------------------------------------------------------------------------------*/
// To learn about processing a command sequence, which has changed since CS2,
// see the doc solutions.pdf starting on page 273.  Read also the URL 
// file:///Users/ross/Documents/sdkdocs/html/classSnpManipulateInline.html#1184545d4c709d722b3de6232961b3fe
// From Adobe's BaseType.h:
// enum { 
// 	kSuccess = 0, 	/** OK */
// 	kFailure = 1, 	/** error occurred */
// 	kCancel = 2 	/** operation has been cancelled or aborted */
// };
	LOG_BEGIN_FUNCTION;
	bool ungroupStatus = false;			// Meaning, "operation failed"

	// Start a command sequence.  When its destructor is called, if the 
	// global error code is set, then the command sequence will rollback.
	CmdUtils::SequencePtr cmdSeq;

	// Set the name that shows on the Undo menu
	PMString seqName ("Ungroup Ad");
	seqName.SetTranslatable (kFalse);
	cmdSeq->SetName (seqName);
	
	// To ungroup the items, create an ungroup command and process it.
	InterfacePtr<ICommand> ungroupCmd (CmdUtils::CreateCommand (kUngroupCmdBoss) );
	ungroupCmd->SetItemList (inGroupUIDRef);
	ErrorCode adobeStatus = CmdUtils::ProcessCommand (ungroupCmd);
	if (adobeStatus == kSuccess)
	{
		ungroupStatus = true;
	}

	// See Adobe docs -- set the global error code if op failed
	if (adobeStatus != kSuccess)
	{
		ErrorUtils::PMSetGlobalErrorCode (adobeStatus);
	}

	LOG_END_FUNCTION;
	return ungroupStatus;
}


/* ----------------------------- DeleteTextFrame_() ----------------------------- */

bool CL_Art_Importer_InDesign::DeleteTextFrame_(UIDRef & inUidRef)
{
	LOG_BEGIN_FUNCTION;
	bool deleteTextFrameStatus = false;

	// Now it's time to delete the text frame
	InterfacePtr<ICommand> deleteCmd(CmdUtils::CreateCommand (kDeleteCmdBoss) );
	deleteCmd->SetItemList (UIDList (inUidRef) );
	if (CmdUtils::ProcessCommand (deleteCmd) == kSuccess)
	{
		deleteTextFrameStatus = true;
	}

	LOG_END_FUNCTION;
	return deleteTextFrameStatus;
}


// END OF FILE
