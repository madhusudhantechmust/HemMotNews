/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	CreateAdOnTheFly_DialogController.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	12/4/07 ca. 10:01 PM
	
	ab	Abstract:
	ab	Declaration and implementation of CreateAdOnTheFly_DialogController class.
	ab	This began life as the spawn of the Adobe DollyXS code generator.
	
	cl	Change Log:
	cl	12/4/07	New.
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
//#include "PlaceAdPanelDisplayAd.h"
//#include "Basic_Page.h"
#include <string>
// Includes needed for the non-UI plugins that actually do the work.
#include "IAbstractAd.h"
#include "CrAdID.h"

using namespace std;

/** CreateAdOnTheFly_DialogController
	Methods allow for the initialization, validation, and application of dialog 
	widget values.  Implements IDialogController based on the partial 
	implementation CDialogController.
	
	@ingroup newscad_create_ad
*/
class CreateAdOnTheFly_DialogController : public CDialogController {
public:
	/** Constructor.
	@param boss interface ptr from boss object on which 
	this interface is aggregated.
	*/
	CreateAdOnTheFly_DialogController(IPMUnknown* boss)
		:	CDialogController(boss)
	{
	}
	
	/** Destructor.
	*/
	virtual		~CreateAdOnTheFly_DialogController() {}
	
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

	int32	columnsWide_;
	double	depthInPoints_;
	PMReal	pmrDepthInPoints_;
	string	stlString;
};


CREATE_PMINTERFACE(CreateAdOnTheFly_DialogController, kCreateAdOnTheFly_DialogControllerImpl)

//----------------------------------------------------------------------------------
//	InitializeDialogFields
//
//	Put code to initialize widget values here.
//----------------------------------------------------------------------------------
void CreateAdOnTheFly_DialogController::InitializeDialogFields (
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
WidgetID CreateAdOnTheFly_DialogController::ValidateDialogFields (IActiveContext* myContext)
{
	WidgetID result = CDialogController::ValidateDialogFields(myContext);
	
	// Validate whatever user typed for width in columns
	PMString pmstr = this->GetTextControlData (kColumnCountEditText_WidgetID);
	PMString::ConversionError err;
	int32 itmp = pmstr.GetAsNumber (&err);
	if (err != PMString::kNoError)
	{
		// Return WidgetID of invalid value
		return kColumnCountEditText_WidgetID;
	}
	else
	{
		columnsWide_ = itmp;
	}
	
	// Check value for depth
/*
	pmstr = this->GetTextControlData (kDepthEditTextWidgetID);
	double idbl = pmstr.GetAsDouble (&err);
	if (err != PMString::kNoError)
	{
		// Return WidgetID of invalid value
		return kDepthEditTextWidgetID;
	}
	else
	{
		depthInPoints_ = itmp;
	}
*/
	pmrDepthInPoints_ = 0.0;
	
	InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
	if (!panelControlData) {
		return kDepthEditText_WidgetID;
	}
	IControlView* controlView = panelControlData->FindWidget(kDepthEditText_WidgetID);
	if (!controlView) {
		return kDepthEditText_WidgetID;
	}
	InterfacePtr<ITextValue> textValue(controlView, UseDefaultIID());
	if (!textValue) {
		return kDepthEditText_WidgetID;
	}

	pmrDepthInPoints_ = textValue->GetTextAsValue();
	
	// If we get this far, everything passed muster.
	return result;
}


/*================================================================================*/
void CreateAdOnTheFly_DialogController::ApplyDialogFields (
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
	PMString outString;
	
	// Sample ad record:
	// 1|342|KAISER-WILLYS AUTO SUPPLY|KAISER-WIL-1x4.75-210121|Dave Nutter||||0|0|
	// BW| Willys Parts (//) |Willys Parts
	// Width in columns
	PMString pmstr = this->GetTextControlData (kColumnCountEditText_WidgetID);
	outString = pmstr;
	outString.Append ('|');
	
	// Depth
/*
	pmstr = this->GetTextControlData (kDepthEditTextWidgetID);
	outString += pmstr;
	outString.Append ('|');
*/
	pmstr = "";
	pmstr.AppendNumber (pmrDepthInPoints_);
	outString += pmstr;
	outString.Append ('|');

	// Client
	pmstr = this->GetTextControlData (kClientEditText_WidgetID);
	outString += pmstr;
	outString.Append ('|');
	
	// Art file
	pmstr = this->GetTextControlData (kArtFileNameEditText_WidgetID);
	outString += pmstr;
	outString.Append ('|');
	
	// Salesperson
	pmstr = this->GetTextControlData (kSalespersonNameEditText_WidgetID);
	outString += pmstr;
	outString.Append ('|');
	
	// The rest of the string is stuff we don't care about in a "basic" ad, 
	// so we're just throwing in some hard coded values:
	outString += "|||0|0|BW|No section|No notes";
/*
	// Dialog has closed -- make an ad if necessary
	CAlert::ModalAlert
	(
		outString,	// Alert string
		kOKString, 					// OK button
		kNullString, 				// No second button
		kNullString, 				// No third button
		1,							// Set OK button to default
		CAlert::eInformationIcon	// Information icon.
	);
*/

	InterfacePtr<IAbstractAd> createAdObj (
				CreateObject2<IAbstractAd>(kCrAdBoss));
	if ( createAdObj )
	{
/* test code:
		pmstr = "Create Ad On The Fly Plugin called with parameter string passed in.  ";
		pmstr += outString;
		createAdObj->Make_(pmstr);*/
		createAdObj->Make_(outString);
	}
#if CS2
	const char* pCstr = outString.GrabCString ();
	string adRec = pCstr;
	try {
		// Realize the ad
		PlaceAdPanelDisplayAd* pAd = new PlaceAdPanelDisplayAd (adRec);

		// The ad gets pasted up on the current page, so instantiate a new 
		// page object and set it up for ad placement.
		auto_ptr<Basic_Page> curPage ( new Basic_Page );
		curPage->ComputeCurrentPageUID_();						// Also sets UIDRef
		curPage->ComputeAndStorePageMeasurements_();

		// We now know enough about the page to convert our ad's 
		// width in columns to a width expressed in points.
		pAd->ComputeWidth_(curPage->ColumnWidth_(), curPage->GutterWidth_());

		// Now we have enough information to paste up the ad (this).  Note 
		// that Basic_Page's PasteUp_ can throw.
		curPage->PasteUp_(pAd);

		// Return memory to free store
		delete pAd;
	}
	catch (PMString exceptionString) {
		CAlert::ErrorAlert (exceptionString);
	}	
#endif
}

