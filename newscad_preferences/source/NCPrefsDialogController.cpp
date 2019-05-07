//========================================================================================
//  
//  $File: $
//  
//  Owner: Ross Edgar
//  
//  $Author: $
//  
//  $DateTime: $
//  
//  $Revision: $
//  
//  $Change: $
//  
//  Copyright 1997-2008 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#include "VCPlugInHeaders.h"

// Interface includes:
#include "IActiveContext.h"
#include "IPanelControlData.h"
#include "IDropDownListController.h"

// General includes:
#include "CDialogController.h"

// Project includes:
#include "NCPrefsID.h"
#include "CL_Log_File.h"
#include "CL_NewsCAD_Prefs.h"
#include "global_data.h"

#include "CAlert.h"

// Configure developer debug/info messages here:
#define	DEVELOPER_ALERTS_WANTED		0

/** NCPrefsDialogController
	Methods allow for the initialization, validation, and application of dialog widget
	values.
	Implements IDialogController based on the partial implementation CDialogController.

	
	@ingroup newscadpreferences
*/
class NCPrefsDialogController : public CDialogController
{
	public:
		/** Constructor.
			@param boss interface ptr from boss object on which this interface is aggregated.
		*/
		NCPrefsDialogController(IPMUnknown* boss) : CDialogController(boss) {}

		/** Destructor.
		*/
		virtual ~NCPrefsDialogController() {}

		/** Initialize each widget in the dialog with its default value.
			Called when the dialog is opened.
		*/
	       virtual void InitializeDialogFields(IActiveContext* dlgContext);

		/** Validate the values in the widgets.
			By default, the widget with ID kOKButtonWidgetID causes
			ValidateFields to be called. When all widgets are valid,
			ApplyFields will be called.
			@return kDefaultWidgetId if all widget values are valid, WidgetID of the widget to select otherwise.

		*/
	       virtual WidgetID ValidateDialogFields(IActiveContext* myContext);


		/** Retrieve the values from the widgets and act on them.
			@param widgetId identifies the widget on which to act.
		*/
		virtual void ApplyDialogFields(IActiveContext* myContext, const WidgetID& widgetId);
};

CREATE_PMINTERFACE(NCPrefsDialogController, kNCPrefsDialogControllerImpl)

// We'll need to be able to read and write the preferences file.
// We handle the preferences file in the Import Ad Geometry plugin.
// See CL_Geometry_Importer::Run_() at line 194 for loading preferences.
// The preferences loading code handles the possibility that the prefs 
// file does not exist, in which case it will create a new one and 
// write out the default values to it.  If the file does exist, it 
// will read it in and store the values in the object.

/* InitializeDialogFields
The NCPrefsDialogController::InitializeDialogFields() method initializes the
widgets on the dialog. This method is called by the parent class when
the dialog is opened, and when the dialog's Reset button is clicked
(Cancel changes to Reset when you hold the Alt or Option key), if you
have not overridden the CDialogController::ResetDialogFields() method.
This method first needs to call the
CDialogController::InitializeDialogFields() method in the parent class.
*/
/*================================================================================*/
void NCPrefsDialogController::InitializeDialogFields (
										IActiveContext* dlgContext) {
/*----------------------------------------------------------------------------------
	Abstract:
		
	Parameters and modes:

	Returns:

	Change Log:
----------------------------------------------------------------------------------*/
#if DEVELOPER_ALERTS_WANTED != 0
	CAlert::ModalAlert
	(
		"Successfully called NCPrefsDialogController::InitializeDialogFields()!",
		kOKString, 					// OK button
		kNullString, 				// No second button
		kNullString, 				// No third button
		1,							// Set OK button to default
		CAlert::eInformationIcon	// Information icon.
	);
#endif
	CDialogController::InitializeDialogFields(dlgContext);
	// Put code to initialize widget values here.
	// See the CS2 code in
	// /Volumes/developer_documents/adobe/FirstPlugin copy/
	//		source/WFPDialogController.cpp starting around line 85
	// for how to initialize the widgets.
	try {
		// Get current panel control data.  Because Adobe says to do it this way.
		InterfacePtr<IPanelControlData> pPanelData (QueryIfNilElseAddRef(nil));
		if (pPanelData == nil) {
			throw "Could not get pointer to PanelControlData.";
		}

		// Find dropdown list menu control view from panel data.
		IControlView* pDropDownListControlView = 
							pPanelData->FindWidget (kNCPrefsDropDownWidgetID);
		if ( pDropDownListControlView == nil ) {	// Is the widget on the dialog?
			throw "Could not get pointer to rule weight editable drop down.";
		}
	
		// Get IDropDownListController interface pointer.
		InterfacePtr<IDropDownListController> pDropDownListController (
									pDropDownListControlView, UseDefaultIID());
		if (pDropDownListController == nil) {		// Is the controller available?
			throw "Could not get pointer to IDropDownListController.";
		}

		// Create and initialize a new preferences global object (Oh,
		// how I wish it weren't global) -- reads in the preferences 
		// file and stores in global object:
		gpPrefsObj = new CL_NewsCAD_Prefs;
		gpPrefsObj->LoadPreferencesFromFile_("NewsCAD_Prefs.XML");

		// Retrieve the values from the object and stuff them into the widgets:
		char cstr[512];
		CL_NewsCAD_Prefs*	pPrefs = gpPrefsObj;
		pPrefs->GetRuleWeightStr_(cstr);						// Rule weight
		PMString pmsInitialString (cstr);
		SetTextControlData (kNCPrefsDropDownWidgetID, 
									pmsInitialString);
		SetTriStateControlData (kNCPrefsGrayAdOutlineCheckBoxWidgetID, // Gray Ad Outline
			pPrefs->GetGrayOutline_() ? 
			ITriStateControlData::kSelected : 
			ITriStateControlData::kUnselected);
		pPrefs->GetGraphicsPath_(cstr);							// Graphics folder
		pmsInitialString = cstr;
		SetTextControlData (kNCPrefsChangeFolderEditTextWidgetID,
									pmsInitialString);
		pPrefs->GetAdCoveragePercent_(cstr);					// Ad coverage pct.
		pmsInitialString = cstr;
		SetTextControlData (kNCPrefsAdCoverageEditTextWidgetID,
									pmsInitialString);
	
	// Return preferences object to free store and do any other clean up:
		delete gpPrefsObj;
		gpPrefsObj = NULL;
	}
	catch (char* exceptStr) {
		PMString exceptionPMString (
			"Exception in NCPrefsDialogController::InitializeDialogFields(): ");
		exceptionPMString += exceptStr;
		exceptionPMString += "  Contact Software Design Services for help.";
		CAlert::ModalAlert
		(
			exceptionPMString,
			kOKString, 					// OK button
			kNullString, 				// No second button
			kNullString, 				// No third button
			1,							// Set OK button to default
			CAlert::eErrorIcon
		);
	}
}


/* ValidateFields
The WFPDialogController::ValidateDialogFields() method validates the
fields on the dialog box. When the OK button is clicked, this method is
called before the ApplyDialogFields() method is called. Again, we first
call the CDialogController::ValidateDialogFields() method in the parent
class. If there is even one field that has an invalid value, you can
return the WidgetID to be selected. If all fields have valid values,
then you can return kDefaultWidgetId, which will allow the parent class
to call our ApplyDialogFields() method.
*/
WidgetID NCPrefsDialogController::ValidateDialogFields(IActiveContext* myContext)
{
#if DEVELOPER_ALERTS_WANTED != 0
	CAlert::ModalAlert
	(
		"Successfully called NCPrefsDialogController::ValidateDialogFields()!",
		kOKString, 					// OK button
		kNullString, 				// No second button
		kNullString, 				// No third button
		1,							// Set OK button to default
		CAlert::eInformationIcon	// Information icon.
	);
#endif
	WidgetID result = CDialogController::ValidateDialogFields(myContext);

	// Put code to validate widget values here.

	return result;
}


/* ApplyDialogFields
*/
// CS2 code is at 
// /Volumes/developer_documents/adobe/FirstPlugin copy/
//		source/WFPDialogController.cpp 
// starting around line 188
/*================================================================================*/
void NCPrefsDialogController::ApplyDialogFields (
				IActiveContext* myContext, const WidgetID& widgetId) {
/*----------------------------------------------------------------------------------
	Abstract:
		Retrieve the values from the dialog fields and act on them.  Here is 
		where we obtain the values from the widgets on the dialog, and do 
		appropriate processing.  The widgetId from the parameter list contains 
		the widget ID that caused this method to be called. By default, this 
		parameter contains kOKButtonWidgetID -- the OK button.
		
	Parameters and modes:
		myContext	
		widgetId	Reference to the widget that caused this method 			IN
					to be called.

	Returns:
		Nothing.
		
	Change Log:
		01-Sep-11	While porting from CS2, discovered that this method is called 
					by InDesign long after my method that originally opened the 
					dialog has returned.  That method is 
					PrefsWorkhorse::DoPreferences_().
----------------------------------------------------------------------------------*/
#if DEVELOPER_ALERTS_WANTED != 0
 	CAlert::ModalAlert
 	(
 		"Successfully called NCPrefsDialogController::ApplyDialogFields()!",
 		kOKString, 					// OK button
 		kNullString, 				// No second button
 		kNullString, 				// No third button
 		1,							// Set OK button to default
 		CAlert::eInformationIcon	// Information icon.
 	);
#endif
	try {
		// Start the log.
		OPEN_LOG_FILE;
		LOG_BEGIN_FUNCTION;
		
		char cstr[256];
		gpPrefsObj = new CL_NewsCAD_Prefs;
		if ( !gpPrefsObj )
			throw "Could not allocate memory to write preferences.";
		
		// Get text of selected item in our rule weight combo box.  The string that 
		// we get back is not the string that we see on our dialog, but actually the 
		// string key that we defined in the string table resource.
		PMString resultString;
		resultString = this->GetTextControlData (kNCPrefsDropDownWidgetID);
	
		// Look up our string and replace.
		resultString.Translate ();
		resultString.GetCString (cstr, 255);
		gpPrefsObj->SetRuleWeightCStr_(cstr);
		
		// Process "Gray Ad Outline" checkbox
		int iGrayOutline = (int) this->GetTriStateControlData (
									kNCPrefsGrayAdOutlineCheckBoxWidgetID);
		bool bGrayOutline = iGrayOutline == ITriStateControlData::kSelected;
		gpPrefsObj->SetGrayOutline_(bGrayOutline);
										
		// Path to graphics folder
		resultString = this->GetTextControlData (
									kNCPrefsChangeFolderEditTextWidgetID);
		resultString.GetCString (cstr, 255);
		gpPrefsObj->SetGraphicsPath_(cstr);
		
		// Ad Coverage Percentage
		resultString = this->GetTextControlData (
									kNCPrefsAdCoverageEditTextWidgetID);
		resultString.GetCString (cstr, 255);
		gpPrefsObj->SetAdCoveragePercentCStr_(cstr);
		
		// Write the dialog data to the file.
		gpPrefsObj->StorePreferencesDialogInput_("NewsCAD_Prefs.XML");
		
		// Return preferences object to free store and do any other clean up:
		delete gpPrefsObj;
		gpPrefsObj = NULL;


		LOG_END_FUNCTION;
		CLOSE_LOG_FILE;			// Closes file and deletes, sets gpLogFile = NULL;
	}
	catch (char* exceptStr) {
		PMString exceptionPMString (
			"Exception in NCPrefsDialogController::ApplyDialogFields(): ");
		exceptionPMString += exceptStr;
		exceptionPMString += "  Contact Software Design Services for help.";
		CAlert::ModalAlert
		(
			exceptionPMString,
			kOKString, 					// OK button
			kNullString, 				// No second button
			kNullString, 				// No third button
			1,							// Set OK button to default
			CAlert::eErrorIcon
		);

		LOG_END_FUNCTION;
		CLOSE_LOG_FILE;			// Closes file and deletes, sets gpLogFile = NULL;
	}
	catch (...) {
		PMString pmstr (
			"Unknown exception in NCPrefsDialogController::ApplyDialogFields(): ");
		pmstr += "Contact Software Design Services for help.";
		CAlert::ModalAlert
		(
			pmstr,
			kOKString, 					// OK button
			kNullString, 				// No second button
			kNullString, 				// No third button
			1,							// Set OK button to default
			CAlert::eErrorIcon
		);

		LOG_END_FUNCTION;
		CLOSE_LOG_FILE;			// Closes file and deletes, sets gpLogFile = NULL;
	}

	// TODO add code that gathers widget values and applies them.


}

//  Code generated by DollyXs code generator

/*
		delete gpPrefsObj;
		gpPrefsObj = NULL;
		LOG (exceptStr);
		LOG_END_FUNCTION;
		CLOSE_LOG_FILE;			// Closes file and deletes, sets gpLogFile = NULL;
*/