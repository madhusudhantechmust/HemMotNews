/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	Preferences_DialogController.cpp — SDS_Plugins.bbprojectd
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	8/7/11 ca. 5:00 PM
	
	ab	Abstract:
	ab	Implementation of Preferences_DialogController class.
	ab	
	
	cl	Change Log:
	cl	8/7/11	New.
----------------------------------------------------------------------------------*/
#include "VCPlugInHeaders.h"

// Interface includes:
#include "IActiveContext.h"
#include "PMString.h"
#include "IPanelControlData.h"
// General includes:
#include "CDialogController.h"
#include "CAlert.h"
// Project includes:
#include "NC_ID.h"
// Standard includes:
#include <string>
// Includes needed for the non-UI plugins that actually do the work:
// TBD

using namespace std;

class Preferences_DialogController : public CDialogController {
public:
	//------------------------------------------------------------------------------
	// C O N S T R U C T I O N  and D E S T R U C T I O N
	//------------------------------------------------------------------------------
	/** Constructor.
	@param boss interface ptr from boss object on which 
	this interface is aggregated.
	*/
	Preferences_DialogController(IPMUnknown* boss)
		:	CDialogController(boss)
	{
	}

	/** Destructor.
	*/
	virtual		~Preferences_DialogController()
	{
	}
	
	/** Initialize each widget in the dialog with its default value.
	Called when the dialog is opened.
	*/
	virtual void	InitializeDialogFields(IActiveContext* dlgContext);
	
	/** Validate the values in the widgets.
	By default, the widget with ID kOKButtonWidgetID causes
	ValidateFields to be called. When all widgets are valid,
	ApplyFields will be called.
	@return kDefaultWidgetId if all widget values are valid, 
	WidgetID of the widget to select otherwise.
	*/
	virtual
	WidgetID	ValidateDialogFields(IActiveContext* myContext);
	
	/** Retrieve the values from the widgets and act on them.
	@param widgetId identifies the widget on which to act.
	*/
	virtual void	ApplyDialogFields (
							IActiveContext* myContext, const WidgetID& widgetId);
	
	//------------------------------------------------------------------------------
	// M e m b e r   V a r i a b l e s
	//------------------------------------------------------------------------------
	
	
	
	// END OF Preferences_DialogController class declaration
};

CREATE_PMINTERFACE(Preferences_DialogController, kPreferences_DialogControllerImpl)

//----------------------------------------------------------------------------------
//	InitializeDialogFields
//
//	Put code to initialize widget values here.
//----------------------------------------------------------------------------------
void Preferences_DialogController::InitializeDialogFields (
												IActiveContext* dlgContext)
{
	CDialogController::InitializeDialogFields (dlgContext);
}


//----------------------------------------------------------------------------------
//	ValidateDialogFields
//
//	Put code to validate widget values here.  When the OK button is clicked, 
//	this method is called to give the plugin a chance to validate the input.
//	If a field contains an invalid value, return its WidgetID; else just return
//	the result of calling the inherited method.
//----------------------------------------------------------------------------------
WidgetID Preferences_DialogController::ValidateDialogFields (IActiveContext* myContext)
{
	// Always call the inherited method.  Like most things in InDesign 
	// development, don't ask questions, just do it.
	WidgetID result = CDialogController::ValidateDialogFields(myContext);
	
	// Validate whatever user typed for width in columns

	// PUT LOTSA CODE HERE

	// If we get this far, everything passed muster.
	return result;
}


/*================================================================================*/
void Preferences_DialogController::ApplyDialogFields (
						IActiveContext* inContext, const WidgetID& inWidgetId) {
/*----------------------------------------------------------------------------------
	Abstract:
		The user has clicked the OK button and the plugin has validated the input.  
		It's time to gather the widget values and store them for the plugin 
		to process.
		
	Parameters and modes:
		inContext		I don't know; not documented by Adobe.				IN
		inWidgetId		Widget that caused this function to be called.		IN
						Generally, the OK button -- kOKButtonWidgetID.

	Returns:
		Nothing.
		
	Change Log:
----------------------------------------------------------------------------------*/
	// Dialog has closed -- make an ad if necessary
	CAlert::ModalAlert
	(
		"NewsCAD Preferences dialog has closed without blowing up.",	// Alert string
		kOKString, 					// OK button
		kNullString, 				// No second button
		kNullString, 				// No third button
		1,							// Set OK button to default
		CAlert::eInformationIcon	// Information icon.
	);
}

/*
REGISTER_PMINTERFACE(NC_ActionComponent, kNC_ActionComponentImpl)
REGISTER_PMINTERFACE(CreateAdOnTheFly_DialogController, kCreateAdOnTheFly_DialogControllerImpl)
REGISTER_PMINTERFACE(CreateAdOnTheFly_DialogObserver, kCreateAdOnTheFly_DialogObserverImpl)
REGISTER_PMINTERFACE(Preferences_DialogController, kPreferences_DialogControllerImpl)
REGISTER_PMINTERFACE(Preferences_DialogObserver, kPreferences_DialogObserverImpl)
*/


// END OF FILE

