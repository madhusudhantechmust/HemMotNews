/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	PrefsWorkhorse.cpp — SDS_Plugins.bbprojectd
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	8/11/11 ca. 8:59 PM
	
	ab	Abstract:
	ab	Implementation of the class that manages the preferences dialog and 
	ab	then reads from and writes to the preferences file.
	ab	
	
	cl	Change Log:
	cl	8/11/11	New.
----------------------------------------------------------------------------------*/
#include "sds_trace.h"

// Interface includes:
#include "ISession.h"
#include "IApplication.h"
#include "IDialogMgr.h"
#include "IDialog.h"

// Dialog-specific resource includes:
#include "CoreResTypes.h"
#include "LocaleSetting.h"
#include "RsrcSpec.h"

// The SDS stuff:
#include "newscad.h"
#include "CL_NewsCAD_Prefs.h"
#include "CL_Log_File.h"
#include "global_data.h"
#include "NCPrefsID.h"
#include "IAbstractPrefsHandler.h"

using namespace std;

//----------------------------------------------------------------------------------

class PrefsWorkhorse : 
			public CPMUnknown<IAbstractPrefsHandler> {
	public:
		PrefsWorkhorse (IPMUnknown* boss) 
			: CPMUnknown<IAbstractPrefsHandler>(boss)
		{}
		
		virtual void	DoPreferences_();	// Pure virtual in ABC

	protected:
	
	private:
};

// This is what enables our code to be called by InDesign:
CREATE_PMINTERFACE(PrefsWorkhorse, kNCPrefsImpl);

#pragma mark -

/*================================================================================*/
void PrefsWorkhorse::DoPreferences_() {
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
	SDS_TRACE ("Successfully called PrefsWorkhorse::DoPreferences_()!");
	try {
		// Get the application interface and the DialogMgr.	
		InterfacePtr<IApplication> application (
								GetExecutionContextSession()->QueryApplication());
		ASSERT(application);
		if (application == nil) {	
			throw "ERROR: Could not get pointer to application.";
		}
		InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
		ASSERT(dialogMgr);
		if (dialogMgr == nil) {
			throw "ERROR: Could not get pointer to dialog manager component.";
		}

		// Load the plug-in's resource.
		PMLocaleId nLocale = LocaleSetting::GetLocale();
		RsrcSpec dialogSpec
		(
			nLocale,					// Locale index from PMLocaleIDs.h. 
			kNCPrefsPluginID,			// Our Plug-in ID  
			kViewRsrcType,				// This is the kViewRsrcType.
			kSDKDefDialogResourceID,	// Resource ID for our dialog.
			kTrue						// Initially visible.
		);

		// CreateNewDialog takes the dialogSpec created above, and also
		// the type of dialog being created (kMovableModal).
		IDialog* dialog = dialogMgr->CreateNewDialog (dialogSpec, 
												IDialog::kMovableModal);
		ASSERT(dialog);
		if (dialog == nil) {
			throw "ERROR: Could not construct preferences dialog.";
		}

		// Here is where we open the dialog.  The Open() method that's called 
		// here is in Adobe's code.  Although the program returns here 
		// eventually, the OK button is handled by another method in another 
		// class, the dialog controller's ApplyDialogFields().
		dialog->Open(); 
		SDS_TRACE ("Right after dialog->Open()");
	}
	catch (char* exceptStr) {
		PMString pmsException (exceptStr);
		pmsException += "  Contact Software Design Services for help.";
		CAlert::ModalAlert
		(
			exceptStr,
			kOKString, 					// OK button
			kNullString, 				// No second button
			kNullString, 				// No third button
			1,							// Set OK button to default
			CAlert::eErrorIcon			// Error icon.
		);
	}
/*	do
	{
		// Get the application interface and the DialogMgr.	
		InterfacePtr<IApplication> application (
								GetExecutionContextSession()->QueryApplication());
		ASSERT(application);
		if (application == nil) {	
			break;
		}
		InterfacePtr<IDialogMgr> dialogMgr(application, UseDefaultIID());
		ASSERT(dialogMgr);
		if (dialogMgr == nil) {
			break;
		}

		// Load the plug-in's resource.
		PMLocaleId nLocale = LocaleSetting::GetLocale();
		RsrcSpec dialogSpec
		(
			nLocale,					// Locale index from PMLocaleIDs.h. 
			kNCPrefsPluginID,			// Our Plug-in ID  
			kViewRsrcType,				// This is the kViewRsrcType.
			kSDKDefDialogResourceID,	// Resource ID for our dialog.
			kTrue						// Initially visible.
		);

		// CreateNewDialog takes the dialogSpec created above, and also
		// the type of dialog being created (kMovableModal).
		IDialog* dialog = dialogMgr->CreateNewDialog (dialogSpec, 
												IDialog::kMovableModal);
		ASSERT(dialog);
		if (dialog == nil) {
			break;
		}

		// Here is where we open the dialog.  The Open() method that's called 
		// here is in Adobe's code.  Although the program returns here 
		// eventually, the OK button is handled by another method in another 
		// class, the dialog controller's ApplyDialogFields().
		dialog->Open(); 
	
	} while (false);			
*/
}