/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	ImportArtFiles.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	3/3/11 ca. 9:13 PM
	
	ab	Abstract:
	ab	This file declares, implements, and makes callable by InDesign a class 
	ab	whose purpose is to construct a CL_ArtImporter object and call a 
	ab	method in that class that will start the art file import.
	
	cl	Change Log:
	cl	3/3/11	New.
----------------------------------------------------------------------------------*/
#include "IDocument.h"
#include "ILayoutUIUtils.h"
#include "ISpread.h"
#include "ISpreadList.h"
#include "CAlert.h"

#include "newscad.h"
#include "ImportArtID.h"
#include "IAbstractArtImporter.h"
#include "CL_Art_Importer_InDesign.h"
#include "CL_NewsCAD_Prefs.h"
#include "CL_Log_File.h"
#include "global_data.h"

using namespace std;

class CL_ArtImportWorkhorse : public CPMUnknown<IAbstractArtImporter> {
public:
	CL_ArtImportWorkhorse (IPMUnknown* boss) 
		: CPMUnknown<IAbstractArtImporter>(boss)
	{}
	virtual void	ImportArtFiles_();
};

// This is what enables our code to be called by InDesign:
CREATE_PMINTERFACE (CL_ArtImportWorkhorse, kImportArtImpl)

//------------------------------------------------------------------------------
//	ImportArtFiles_() runs the graphics files import.
//	This function is called when the user chooses NewsCAD->Import Art Files
//	from the menu bar.
//------------------------------------------------------------------------------
void CL_ArtImportWorkhorse::ImportArtFiles_()
{
	// Put code here to instantiate the importer and fire it up.
	try 
	{
		// Start the log.
		OPEN_LOG_FILE;
		LOG_BEGIN_FUNCTION;

		IDocument *iDocument = 
			Utils<ILayoutUIUtils>()->GetFrontDocument();
		if (iDocument == nil)
			throw "From NewsCAD - ERROR: No front document!";
		
		InterfacePtr<ISpreadList> spreadList ( iDocument, UseDefaultIID () );
		if (spreadList == nil)
			throw "From NewsCAD - ERROR: No spread list!";
		
		int32 spreadCount = spreadList->GetSpreadCount();
		if (spreadCount <= 0)
		{
			// This shouldn't ever happen if there is an open doc
			throw "Spread count is 0?";
		}

		// Read in the preferences file and store in global object		
		gpPrefsObj = new CL_NewsCAD_Prefs;
		gpPrefsObj->LoadPreferencesFromFile_("NewsCAD_Prefs.XML");

		// Start big, scary loop to process every spread in the document:
		for (int spreadNum = 0; spreadNum < spreadCount; spreadNum++)
		{
			InterfacePtr<ISpread> curSpread (::GetDataBase (iDocument), 
										spreadList->GetNthSpreadUID (spreadNum), 
										IID_ISPREAD);
			if (curSpread == nil)
				throw ("Could not get current spread!");
			
			int32 pageCount = curSpread->GetNumPages ();

			// Every spread should have at least one page, but you
			// never know what could happen
			if (pageCount > 0)
			{
				// iterate pages in this spread and gather/process page items
				for (int32 pageNum = 0; pageNum < pageCount; pageNum++)
				{
					// Get the list of items on the page
					UIDList itemList (iDocument);
					curSpread->GetItemsOnPage (pageNum, &itemList, kFalse, kFalse);

					// Here's where all the action is, baby!
					CL_Art_Importer_InDesign* artImporter = 
													new CL_Art_Importer_InDesign;
					artImporter->ProcessPageItems_(itemList);
					delete artImporter;
				}
			}
		}
		delete gpPrefsObj;
		gpPrefsObj = NULL;
		LOG_END_FUNCTION;
		CLOSE_LOG_FILE;			// Closes file and deletes, sets gpLogFile = NULL;
	}
	catch (char* exceptStr)
	{
		CAlert::ModalAlert
		(
			exceptStr,
			kOKString, 					// OK button
			kNullString, 				// No second button
			kNullString, 				// No third button
			1,							// Set OK button to default
			CAlert::eInformationIcon	// Information icon.
		);
		delete gpPrefsObj;
		gpPrefsObj = NULL;
		LOG (exceptStr);
		LOG_END_FUNCTION;
		CLOSE_LOG_FILE;			// Closes file and deletes, sets gpLogFile = NULL;
	}

	// What follows is the old 2008 code, commented out:
/*
	try 
	{
		IDocument *iDocument = ::GetFrontDocument();
		if (iDocument == nil)
			throw "No front document!";
		
		InterfacePtr<ISpreadList> spreadList ( iDocument, UseDefaultIID () );
		if (spreadList == nil)
			throw "No spread list!";
		
		int32 spreadCount = spreadList->GetSpreadCount();
		if (spreadCount <= 0)
		{
			// This shouldn't ever happen if there is an open doc
			throw "Spread count is 0?";
		}

		// For each spread in the document
		for (int spreadNum = 0; spreadNum < spreadCount; spreadNum++)
		{
			InterfacePtr<ISpread> curSpread (::GetDataBase (iDocument), 
										spreadList->GetNthSpreadUID (spreadNum), 
										IID_ISPREAD);
			if (curSpread == nil)
				throw ("Could not get current spread!");
			
			int32 pageCount = curSpread->GetNumPages ();

			// Every spread should have at least one page, but you
			// never know what could happen
			if (pageCount > 0)
			{
				// iterate pages in this spread and gather/process page items
				for (int32 pageNum = 0; pageNum < pageCount; pageNum++)
				{
					// Get the list of items on the page
					UIDList itemList (iDocument);
					curSpread->GetItemsOnPage (pageNum, &itemList, kFalse, kFalse);
					CL_Art_Importer_InDesign* artImporter = 
													new CL_Art_Importer_InDesign;
					artImporter->ProcessPageItems_(itemList);
					delete artImporter;
				}
			}
		}
	}
	catch (...)
	{
		
	}
*/
	// End of CL_ArtImportWorkhorse::ImportArtFiles_().
}


// END OF FILE
