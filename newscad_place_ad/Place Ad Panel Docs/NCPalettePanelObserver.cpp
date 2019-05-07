/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	NCPalettePanelObserver.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	5/30/06 ca. 9:22 PM
	
	ab	Abstract:
	ab	Implementation of NCPalettePanelObserver class.  This 
	ab	object observes the "Make ad" button in the widget list.
	
	cl	Change Log:
	cl	5/30/06	New.
------------------------------------------------------------------------------*/
#include "VCPlugInHeaders.h"

// Implementation includes
#include "WidgetID.h"

// Interface includes
#include "ISubject.h"
#include "IBooleanControlData.h"
#include "IControlView.h"
#include "IPanelControlData.h"
#include "IListBoxController.h"
#include "Utils.h"
#include "IWidgetUtils.h"
#include "IDocument.h"
#include "IBlackBoxCommands.h"

// Implem includes
#include "CAlert.h"
#include "CObserver.h"
#include "NCPaletteID.h"
#include "IListControlData.h"

// General includes
#include "LayoutUIUtils.h"

#include "MyListBoxHelper.h"
#include "PlaceAdPanelDisplayAd.h"
#include "tokenizer.hpp"

typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
const char cStartDelim = '<';
const char cEndDelim = '>';

#define		WANT_DEBUG_ALERTS		0

class NCPalettePanelObserver : public CObserver {
	public:
		NCPalettePanelObserver (IPMUnknown *boss);
		~NCPalettePanelObserver ();
		
		virtual void AutoAttach ();
		virtual void AutoDetach ();

		virtual void Update (const ClassID& theChange, 
								ISubject* theSubject, 
								const PMIID &protocol, 
								void* changedBy);

	protected:
	
	private:
		void DoAttachDetach (const WidgetID& widgetID, 
								const PMIID& pmiid, bool16 attaching);
		int32 GetListBoxSelectedItemIndex_();
		void UpdateUnplacedAdsDocumentData_(int32 selItemIndex);
		void RemoveListboxElementAtIndex_(int32 selItemIndex);
};

CREATE_PMINTERFACE(NCPalettePanelObserver, kNCPalettePanelObserverImpl)

//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and D E S T R U C T I O N
//----------------------------------------------------------------------------------
NCPalettePanelObserver::NCPalettePanelObserver (IPMUnknown *boss)
	:	CObserver (boss)
{

}


NCPalettePanelObserver::~NCPalettePanelObserver ()
{

}


//----------------------------------------------------------------------------------
// AutoAttach is only called for registered observers
// of widgets.  This method is called by the application
// core when the widget is shown.
//----------------------------------------------------------------------------------
void NCPalettePanelObserver::AutoAttach ()
{
	this->DoAttachDetach (kNCPalettePlaceAdButtonWidgetID, 
							IBooleanControlData::kDefaultIID, kTrue);
	this->DoAttachDetach (kNCPaletteRemoveButtonWidgetID, 
							IBooleanControlData::kDefaultIID, kTrue);
}


//----------------------------------------------------------------------------------
// AutoAttach is only called for registered observers
// of widgets.  This method is called by the application
// core when the widget is hidden.
//----------------------------------------------------------------------------------
void NCPalettePanelObserver::AutoDetach ()
{
	this->DoAttachDetach (kNCPalettePlaceAdButtonWidgetID, 
							IBooleanControlData::kDefaultIID, kFalse);
	this->DoAttachDetach (kNCPaletteRemoveButtonWidgetID, 
							IBooleanControlData::kDefaultIID, kFalse);
}



/*================================================================================*/
void NCPalettePanelObserver::Update (const ClassID& theChange, 
					ISubject* theSubject, const PMIID &protocol, void* changedBy) {
/*----------------------------------------------------------------------------------
	Abstract:
		Respond to mouse clicks in the "Place Ad" and "Remove From List" buttons.

	Parameters and modes:
		I'm not sure.  I'm just aping some stuff Adobe tech support (India) sent.

	Returns:
		Nothing.

	Change Log:
		30-May-06	New.
----------------------------------------------------------------------------------*/
/*
	PMString dbgInfoString("NCPaletteListBoxObserver::Update");
	dbgInfoString.SetTranslatable(kFalse);	// only for debug- not real code
	CAlert::InformationAlert(dbgInfoString);
*/
	int32 selItemIndex;
	do
	{
		InterfacePtr<IControlView> controlView (theSubject, UseDefaultIID());
		ASSERT (controlView);
		if (!controlView)
		{
			break;
		}

		// Get the button ID from the view.
		WidgetID widgetID = controlView->GetWidgetID ();
		bool shouldDisableButtons = false;
		
		switch (widgetID.Get())
		{
			case kNCPalettePlaceAdButtonWidgetID:
			{
				if (theChange != kTrueStateMessage) {
					break;
				}

				selItemIndex = GetListBoxSelectedItemIndex_();

#if WANT_DEBUG_ALERTS
				PMString dbgInfoString (
					"Update() : Make Ad pressed! Item selected = ");
				dbgInfoString.SetTranslatable(kFalse);	// only for debug- not real code
				dbgInfoString.AppendNumber(selItemIndex);
				CAlert::InformationAlert(dbgInfoString);
#endif

				if (selItemIndex >= 0)
				{
					// Make an ad now!  (Will also clean up doc slug.)
					PlaceAdPanelDisplayAd ad (selItemIndex);
					ad.Realize_();
					
					// Remove this item from the doc slug!
					//UpdateUnplacedAdsDocumentData_(selItemIndex);
					
					// Remove this item from the listbox now!
					RemoveListboxElementAtIndex_(selItemIndex);
					
					// TBD: DISABLE THE BUTTONS, AS NOTHING IS SELECTED.
					shouldDisableButtons = true;
				}
			}
			break;

			case kNCPaletteRemoveButtonWidgetID:
			{
				if (theChange != kTrueStateMessage) {
					break;
				}

				selItemIndex = GetListBoxSelectedItemIndex_();

#if WANT_DEBUG_ALERTS
				PMString dbgInfoString (
					"Update() : Remove From List pressed!  Item selected = ");
				dbgInfoString.SetTranslatable(kFalse);	// only for debug- not real code
				dbgInfoString.AppendNumber(selItemIndex);
				CAlert::InformationAlert(dbgInfoString);
#endif

				if (selItemIndex >= 0)
				{
					// Remove this item from the list now!
					UpdateUnplacedAdsDocumentData_(selItemIndex);
					
					// Remove this item from the listbox now!
					RemoveListboxElementAtIndex_(selItemIndex);
					
					// TBD: DISABLE THE BUTTONS, AS NOTHING IS SELECTED.
					shouldDisableButtons = true;
				}
			}
			break;
			
			default:
				break;
		}
#if 1
		if (shouldDisableButtons)
		{
			// Handle enabling/disabling the buttons
			InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID ());
			IControlView* makeAdCView = 
						panelControlData->FindWidget (kNCPalettePlaceAdButtonWidgetID);
			IControlView* putBackCView =
						panelControlData->FindWidget(kNCPaletteRemoveButtonWidgetID);
			makeAdCView->Enable (kFalse);
			putBackCView->Enable (kFalse);
		}
#endif

	} while (kFalse);
}


void NCPalettePanelObserver::DoAttachDetach(const WidgetID& widgetID, const PMIID& pmiid, bool16 attaching)
{
	do
	{
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		ASSERT(panelControlData);
		if (!panelControlData)
		{
			break;
		}
		IControlView* view = panelControlData->FindWidget(widgetID);
		ASSERT(view);
		if (!view)
		{
			break;
		}
		InterfacePtr<ISubject> subject(view, UseDefaultIID());
		ASSERT(subject);
		if (!subject)
		{
			break;
		}

		if (attaching)
		{
			if (!subject->IsAttached(this, pmiid, IID_IOBSERVER))
			{
				subject->AttachObserver(this, pmiid, IID_IOBSERVER);
			}
		}
		else
		{
			if (subject->IsAttached(this, pmiid, IID_IOBSERVER))
			{
				subject->DetachObserver(this, pmiid, IID_IOBSERVER);
			}
		}
	} while (kFalse);
}


int32 NCPalettePanelObserver::GetListBoxSelectedItemIndex_()
{
	MyListBoxHelper listHelper (this, kNCPalettePluginID, 
					kUnplacedAdsListBoxWidgetID, kNCPalettePanelWidgetID);
	IControlView* listBox = listHelper.FindCurrentListBox ();
	if (listBox == nil) {
		return -1;
	}
	
	InterfacePtr<IListBoxController> 
		listCntl (listBox, IID_ILISTBOXCONTROLLER);	// useDefaultIID() not 
													// defined for this interface
	if (listCntl == nil) {
		return -2;
	}
	
	// Get the first item in the selection (should be only one if the 
	// list box has been set up correctly in the framework resource file)
	K2Vector<int32> multipleSelection ;
	listCntl->GetSelected ( multipleSelection ) ;
	
	// Archie added this check (I think it's irrelevant if the buttons/menus 
	// are being enabled and disabled correctly, but WTF):
	if ( multipleSelection.IsEmpty () )
	{
		return 0;
	}

	return multipleSelection[0];
}


void NCPalettePanelObserver::UpdateUnplacedAdsDocumentData_(int32 selItemIndex) 
{
	if (selItemIndex < 1)							// Valid index?
		return;										// No, exit
	
	try 
	{
		IDocument* doc = ::GetFrontDocument();
		if ( !doc ) throw "Could not get front document.";

		InterfacePtr<IBlackBoxData> pIBlackBoxData ( doc, UseDefaultIID() );
		if ( !pIBlackBoxData ) throw "Could not get document black box data.";

		// Read in the current document slug.
		int32 docSlugDataLen = pIBlackBoxData->GetDataLength (kWFPPluginID);
		char* dataStr = new char [docSlugDataLen + 1];
		if (!dataStr) throw "Out of memory.";
		pIBlackBoxData->ReadData (kWFPPluginID, (void*)dataStr, docSlugDataLen);
		dataStr[docSlugDataLen] = '\0';
		string blackBoxData = dataStr;
		
		// Copy the input to the output but skip the item to be deleted.
		string newBlackBoxData;
		newBlackBoxData.clear ();
		boost::char_separator<char> sep("<>\n\r");
		tokenizer tokens (blackBoxData, sep);
		tokenizer::iterator tok_iter;
		int32 i = 0;
		for (tok_iter = tokens.begin (); tok_iter != tokens.end (); ++tok_iter)
		{
			if (i != selItemIndex)
			{
				newBlackBoxData += cStartDelim;
				newBlackBoxData += *tok_iter;
				newBlackBoxData += cEndDelim;
				newBlackBoxData += '\r';
			}
			++i;
		}
		
		// Set new document black box data.
		pIBlackBoxData->RemoveData (kWFPPluginID);
		pIBlackBoxData->WriteData ( kWFPPluginID, (void*)newBlackBoxData.c_str(), 
													newBlackBoxData.length());

		// Return to free store.
		delete [] dataStr;
	}
	catch (PMString exceptionString)
	{
		CAlert::ErrorAlert (exceptionString);
	}
}


void NCPalettePanelObserver::RemoveListboxElementAtIndex_(int32 selItemIndex)
{
	MyListBoxHelper listHelper (this, kNCPalettePluginID, 
					kUnplacedAdsListBoxWidgetID, kNCPalettePanelWidgetID);
	IControlView* listBox = listHelper.FindCurrentListBox ();

	if (listBox != nil)
	{
		MyListBoxHelper listHelper (listBox, kNCPalettePluginID, 
							kUnplacedAdsListBoxWidgetID, kNCPalettePanelWidgetID);
		listHelper.RemoveElementAt (selItemIndex);
	}

#if 0	
	// If user has just removed the last item, disable the buttons now.
	InterfacePtr<IListControlData> listControlData (listBox, UseDefaultIID());
	if (listControlData == nil || listControlData->Length () < 1)
	{
		// Get the kWLBCmpPanelWidgetBoss
		InterfacePtr<IControlView> panelWidget (this, IID_ICONTROLVIEW);
		InterfacePtr<IPanelControlData> panelControlData (
											panelWidget, UseDefaultIID ());

		IControlView* makeAdCView =	panelControlData->FindWidget (
												kNCPalettePlaceAdButtonWidgetID);
		IControlView* putBackCView = panelControlData->FindWidget (
												kNCPaletteRemoveButtonWidgetID);
		makeAdCView->Enable (kFalse);
		putBackCView->Enable (kFalse);
	}
#endif

}


// END OF FILE
