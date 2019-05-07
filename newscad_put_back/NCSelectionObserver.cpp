/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	NCSelectionObserver.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	3/9/06 ca. 9:30 PM
	
	ab	Abstract:
	ab	Implementation of NCSelectionObserver class.
	ab	This is the class referred to in the NCPutBackPanel.fr 
	ab	file as kNCPutBackSelectionObserverImpl.
	
	cl	Change Log:
	cl	3/9/06	New.
----------------------------------------------------------------------------------*/

#include "VCPlugInHeaders.h"

// Interface includes
#include "ITextControlData.h"
#include "IWidgetParent.h"
#include "IPanelControlData.h"
#include "IControlView.h"
#include "ICounterControlData.h"
#include "IPanelControlData.h"
#include "ISubject.h"
#include "IDocument.h"
#include "IBlackBoxCommands.h"
#include "ITextValue.h"
#include "IHierarchy.h"
#include "GroupID.h" // for 'kGroupItemBoss'

// General includes
#include "CAlert.h"
#include "SelectionObserver.h"
#include "ILayoutUIUtils.h"
#include "ISelectionUtils.h"
#include "IActiveContext.h"

#include "NCSelectionSuite.h"

// Project includes
#include "NCPutBackPanelID.h"
#include "IBlackBoxData.h"

// My stuff
#include "Basic_Display_Ad.h"
extern "C" char* uitoa (unsigned int ui, char* s);
const char cStartDelim = '<';
const char cEndDelim = '>';

// A handy macro for turning on and turning off debugging alerts
#define NEWSCAD_DEBUG 0

// Note the absence of the include file ... the class is declared and defined in 
// the cpp file, because it hooks into Adobe's stuff via the NCPutBackPanel.fr.

class NCSelectionObserver : public ActiveSelectionObserver
{
	public:
		NCSelectionObserver (IPMUnknown* boss);
		virtual ~NCSelectionObserver ();
		virtual void AutoAttach () ;
		virtual void AutoDetach () ;
		virtual void Update ( const ClassID& theChange, ISubject* theSubject, 
								const PMIID& protocol, void* changedBy ) ;
								
		// toms - stuff missing from ActiveSelectionObserver
//		virtual void	HandleFrequentSelectionChanged (void) {}
//		virtual void	HandleSelectionAttributeChanged (const ISelectionMessage* m) {}

	protected:
		virtual void HandleSelectionChanged(const ISelectionMessage* message);
//		virtual void HandleSelectionAttributeChanged (
//												const ISelectionMessage* message);
		void		UpdatePanel ();
		
		bool		HaveValidSelection ();
		UIDList*	GetSelection ();
		virtual void DeleteAd_();
		virtual void UpdateStaticTextFields_();
		virtual void ClearStaticTextFields_();
		virtual void DeleteAd_(UIDRef inAdGroupItemUIDRef);
		virtual void UpdateUnplacedAdsDocumentData_(char* inAdData);

		void DebugAlert_(const char* inStr);
		
		// toms - stuff missing from ActiveSelectionObserver
//		virtual void 	HandleSelectionUpdate(const ClassID& bossClassID, ISubject*s, const PMIID&p, void*v) {}
//		virtual void 	AttachToSelectionSubject(ISubject* selectionSubject) {}
//		virtual void 	DetachFromSelectionSubject(ISubject* selectionSubject) {}
//		virtual void	HandleActiveSelectionChanged (void) {}
};


//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and D E S T R U C T I O N
//----------------------------------------------------------------------------------
CREATE_PMINTERFACE (NCSelectionObserver, kNCPutBackSelectionObserverImpl)

void NCSelectionObserver::DebugAlert_(const char* inStr)
{
#if NEWSCAD_DEBUG
	PMString pmstr (inStr);
	CAlert::InformationAlert (pmstr);
#endif
}


NCSelectionObserver::NCSelectionObserver (IPMUnknown* boss)
	:	ActiveSelectionObserver(boss)
{
				// EMPTY
}

NCSelectionObserver::~NCSelectionObserver ()
{
				// EMPTY
}

// attach to our button (the checkbox)
void NCSelectionObserver::AutoAttach ()
{
	DebugAlert_("AutoAttach()");
	ActiveSelectionObserver::AutoAttach ();
	do
	{	
		// Get the IPanelControlData, needed to get the widgets
		InterfacePtr<IPanelControlData> iPanel ( this, UseDefaultIID () );
		if (iPanel == nil) break;

		IControlView* iView = iPanel->FindWidget ( kOKButtonWidgetID );
		if (iView == nil) break;

		InterfacePtr<ISubject> iSubject( iView, UseDefaultIID () );
		if (iSubject == nil) break;

		iSubject->AttachObserver (this, IID_IBOOLEANCONTROLDATA);
		
		// Now, whenever the user clicks the OK button, our 
		// NCSelectionObserver::Update () will be automagically called.
	} 
	while (false);
}

//
// detach from our button
void NCSelectionObserver::AutoDetach ()
{
	DebugAlert_("AutoDetach()");
	ActiveSelectionObserver::AutoDetach ();
	
	do
	{
		InterfacePtr<IPanelControlData> iPanel (this, UseDefaultIID ());
		if (iPanel == nil) break;
	
		IControlView* iView = iPanel->FindWidget( kOKButtonWidgetID );
		if (iView == nil) break;

		InterfacePtr<ISubject> iSubject( iView, UseDefaultIID() );
		if (iSubject == nil) break;

		iSubject->DetachObserver (this, IID_IBOOLEANCONTROLDATA);
	} 
	while (false);
}


void NCSelectionObserver::Update ( 
									const ClassID& theChange, 
									ISubject* theSubject, 
									const PMIID& protocol, 
									void* changedBy )
{			
	DebugAlert_("Update()");
	
	// Call parent's Update so our HandleSelectionChanged () will 
	// be called, when necessary
	ActiveSelectionObserver::Update (theChange, theSubject, protocol, changedBy);

	if ( theChange.Get() == kTrueStateMessage )
	{
		// Click on a control
		// The 'ISubject' argument is an interface on this particular 
		// widget's boss class so we can get the widget using that ISubject
		InterfacePtr<IControlView> button ( theSubject, UseDefaultIID () );
		if ( button && button->GetWidgetID () == kOKButtonWidgetID )
		{
			// Button is our "Put Back" button
			DeleteAd_();
		}
	}
}

void NCSelectionObserver::HandleSelectionChanged (
										const ISelectionMessage* iSelectionMsg)
{
	DebugAlert_("HandleSelectionChanged()");
	this->UpdatePanel();
}

/*
void DCNObserver::HandleSelectionAttributeChanged( const ISelectionMessage* iSelectionMsg)
{
	this->UpdatePanel();
}
*/

void NCSelectionObserver::UpdatePanel()
{
	do 
	{
		InterfacePtr<IPanelControlData> panelControlData(this, UseDefaultIID());
		if (!panelControlData) break;

		// Get a pointer to our "Put Back" button
		IControlView* myControlView = 
								panelControlData->FindWidget (kOKButtonWidgetID);
		if (!myControlView) break;
		
		if ( HaveValidSelection() )
		{
			// Enable the "Put Back" button and fill the static text fields.
			myControlView->Enable();
			UpdateStaticTextFields_();
		}
		else
		{
			myControlView->Disable();
			ClearStaticTextFields_();
		}
	}
	while (false);
}

/*
bool NCSelectionObserver::HaveValidSelection()
{
	bool	valid = false;
	
	// get list of selected object from our selection suite
	UIDList* list = GetSelection();
	
	if ( list )
	{
		for ( int i=0 ; i<list->Length() ; i++ )
		{
			InterfacePtr<IBlackBoxData> obj( list->GetRef(i),IID_IBLACKBOXDATA );
			if ( obj )
			{
				int datalen = obj->GetDataLength (kWFPPluginID);
				if ( datalen )
				{
					PMString pmstr ("Have a page item with our custom data on it");
					CAlert::InformationAlert (pmstr);
					char logstr [256];
					::sprintf (logstr, "Data Length = %d", datalen);
					pmstr = logstr;
					CAlert::InformationAlert (pmstr);
					obj->ReadData (kWFPPluginID, (void*)logstr, datalen < 250 ? datalen : 250);
					logstr[datalen < 250 ? datalen : 250] = '\0';
					pmstr = logstr;
					CAlert::InformationAlert (pmstr);
				}
			}
		}
		
		// we don't enable if more than one page item selected
		if ( list->Length() != 1 ) valid = false;
		
		delete list;
	}
	
	return valid;
}
*/
bool NCSelectionObserver::HaveValidSelection () 
{
	char 	buffer[256];
	int32	len = 0;
	bool	valid = false;				// Default to "selection not valid"

	DebugAlert_("HaveValidSelection()");
	InterfacePtr<IBlackBoxCommands>blackBoxCmds (
				Utils<IBlackBoxCommands>().QueryUtilInterface());

	// Get list of selected objects from our selection suite.
	UIDList* list = GetSelection();

	if ( list )
	{
		// If more than 1 item selected, we know we aren't going to enable the 
		// Put Back button, irrespective of whether the items have our custom data.
		if ( list->Length() != 1 )
		{
			delete list;
			DebugAlert_("ERROR:  List has more than 1 item -- illegal");
			return valid;					// ... which was initted to false
		}

		for ( int32 i=0 ; i < list->Length () ; i++ )
		{
			len = blackBoxCmds->GetData (list->GetRef(i), 
										kNC_PluginID, buffer, 256);
			if ( len )
			{
				DebugAlert_("Valid selection, enable Put Back button");
				valid = true;
			}
			else
			{
				DebugAlert_("ERROR:  Item black box data is 0 length");
				// EMPTY
			}
		}
		delete list;
	}
	else
	{
		//DebugAlert_("ERROR:  No selection list");
		// EMPTY -- and valid has already been defaulted to false.
	}

	return valid;
}


//
// we get access to our custom selection suite
//  from the currently active context
// we then call this suite to get a list of the currently-selected page items

UIDList* NCSelectionObserver::GetSelection()
{
	DebugAlert_("NCSelectionObserver::GetSelection()");
	
	UIDList*	list = NULL;
	
	do
	{
		IDocument* idoc = Utils<ILayoutUIUtils>()->GetFrontDocument();
		if ( idoc == NULL ) break;
		
		IActiveContext* context = GetExecutionContextSession()->GetActiveContext();
		if ( context == NULL ) break;
		
		ISelectionManager* iSelMgr = context->GetContextSelection();
		InterfacePtr<INCPutBackSelSuite> mySelectionSuite( iSelMgr, UseDefaultIID());
		if ( mySelectionSuite == NULL ) break;
		
		mySelectionSuite->GetTheSelection( &list );
	}
	while ( false );
	
	return list;
}


void NCSelectionObserver::UpdateStaticTextFields_()
{
	extern ErrorCode GetFrameGeometryInPageCoordinates_(
						const UIDRef& inFrameUIDRef, PMRect & outFrameRect);
	extern bool GetPageBoundsForItem_( IPMUnknown* pageItem, 
										PMRect& pageItemBounds );

	InterfacePtr<IBlackBoxCommands>blackBoxCmds (
				Utils<IBlackBoxCommands>().QueryUtilInterface());

	// get list of selected object from our selection suite
	UIDList* list = GetSelection();

	if ( list && list->Length() == 1)
	{
		InterfacePtr<IBlackBoxData> bbd (list->GetRef(0), UseDefaultIID());
		if (bbd)
		{
			int32 datalen = bbd->GetDataLength (kNC_PluginID);
			if (datalen)
			{
				char* buffer = new char [datalen + 1];
				int32 len = blackBoxCmds->GetData (list->GetRef(0), 
										kNC_PluginID, buffer, datalen);
				if (datalen != len)
				{
					DebugAlert_("ERROR:  Did not read the right amount of data!"
								"  Cannot update static text fields.");
				}
				else
				{
					// Construct a display ad from a STL string.
					buffer[datalen] = 0;
					string s = buffer;
					Basic_Display_Ad theAd (s);
					
					// Now extract the info we want.
					theAd.GetSalesRepName_(s);
					theAd.GetArtFileName_(s);
					int32 colcnt = theAd.GetColumnCount_();
					uitoa (static_cast<uint32> (colcnt), buffer);

					// Look for and set values of static text widgets.
					InterfacePtr<IPanelControlData> panelControlData (
														this, UseDefaultIID());
					if (panelControlData)
					{
						PMString pmstr;
						
						// Ad size
						IControlView* textView = 
								panelControlData->FindWidget (kStaticTextWidgetID_AdSize);
						if (textView)
						{
							InterfacePtr<ITextControlData> tcData (
													textView, UseDefaultIID () );
							if ( tcData )
							{
								pmstr = buffer;
								pmstr += " columns wide  x  ";
								::sprintf (buffer, "%6.3f", ToFloat (theAd.GetHeight_()));
								pmstr += buffer;
								pmstr += " pts deep";
								tcData->SetString ( pmstr );
							}
						}
						
						// Customer
						textView = panelControlData->FindWidget (
													kStaticTextWidgetID_Client);
						if (textView)
						{
							InterfacePtr<ITextControlData> tcData (
													textView, UseDefaultIID () );
							if ( tcData )
							{
								theAd.GetCustomer_(s);
								pmstr = s.c_str();
								tcData->SetString ( pmstr );
							}
						}

						// Sales Rep
						textView = panelControlData->FindWidget (
													kStaticTextWidgetID_SalesRep);
						if (textView)
						{
							InterfacePtr<ITextControlData> tcData (
													textView, UseDefaultIID () );
							if ( tcData )
							{
								theAd.GetSalesRepName_(s);
								pmstr = s.c_str();
								tcData->SetString ( pmstr );
							}
						}
						
						// Art File
						textView = panelControlData->FindWidget (
													kStaticTextWidgetID_FileName);
						if (textView)
						{
							InterfacePtr<ITextControlData> tcData (
													textView, UseDefaultIID () );
							if ( tcData )
							{
								theAd.GetArtFileName_(s);
								pmstr = s.c_str();
								tcData->SetString ( pmstr );
							}
						}
						
						// Ad geometry (as placed -- maybe user moved it)
						InterfacePtr<IHierarchy> hier ( list->GetRef(0),IID_IHIERARCHY );
						if ( hier /*&& ::GetClass (hier) == kGroupItemBoss*/ )
						{
							PMRect pageItemBounds;
							bool gotGeometry = GetPageBoundsForItem_( hier, pageItemBounds );
							if (gotGeometry)
							{
								float ftmp;
								char stmp[32];
										
								// Left (X-Position)
								textView = panelControlData->FindWidget (
														kStaticTextWidgetID_StartX);
								if (textView)
								{
									InterfacePtr<ITextControlData> tcData (
															textView, UseDefaultIID () );
									if ( tcData )
									{
										ftmp = ToFloat ( pageItemBounds.Left() );
										::sprintf (stmp, "%6.3f", ftmp);
										pmstr = stmp;
										tcData->SetString ( pmstr );
									}
								}

								// Top (Y-Position)
								textView = panelControlData->FindWidget (
														kStaticTextWidgetID_StartY);
								if (textView)
								{
									InterfacePtr<ITextControlData> tcData (
															textView, UseDefaultIID () );
									if ( tcData )
									{
										ftmp = ToFloat ( pageItemBounds.Top() );
										::sprintf (stmp, "%6.3f", ftmp);
										pmstr = stmp;
										tcData->SetString ( pmstr );
									}
								}
							}
						}
						else
						{
							DebugAlert_("FAILED to get panelControlData.");
						}
					}
				}
				
				delete [] buffer;
			 }
		}
	}
}


void NCSelectionObserver::ClearStaticTextFields_()
{
	PMString emptyString ("");
	
	// Look for and set values of static text widgets.
	InterfacePtr<IPanelControlData> panelControlData (this, UseDefaultIID());
	if (panelControlData)
	{
		// Ad size
		IControlView* textView = 
					panelControlData->FindWidget (kStaticTextWidgetID_AdSize);
		if (textView)
		{
			InterfacePtr<ITextControlData> tcData (
									textView, UseDefaultIID () );
			if ( tcData )
			{
				tcData->SetString ( emptyString );
			}
		}

		// Customer
		textView = panelControlData->FindWidget (
									kStaticTextWidgetID_Client);
		if (textView)
		{
			InterfacePtr<ITextControlData> tcData (
									textView, UseDefaultIID () );
			if ( tcData )
			{
				tcData->SetString ( emptyString );
			}
		}

		// Sales Rep
		textView = panelControlData->FindWidget (
									kStaticTextWidgetID_SalesRep);
		if (textView)
		{
			InterfacePtr<ITextControlData> tcData (
									textView, UseDefaultIID () );
			if ( tcData )
			{
				tcData->SetString ( emptyString );
			}
		}

		// Art File
		textView = panelControlData->FindWidget (
									kStaticTextWidgetID_FileName);
		if (textView)
		{
			InterfacePtr<ITextControlData> tcData (
									textView, UseDefaultIID () );
			if ( tcData )
			{
				tcData->SetString ( emptyString );
			}
		}
		
		// Start X
		textView = panelControlData->FindWidget (
									kStaticTextWidgetID_StartX);
		if (textView)
		{
			InterfacePtr<ITextControlData> tcData (
									textView, UseDefaultIID () );
			if ( tcData )
			{
				tcData->SetString ( emptyString );
			}
		}
		
		// Start Y
		textView = panelControlData->FindWidget (
									kStaticTextWidgetID_StartY);
		if (textView)
		{
			InterfacePtr<ITextControlData> tcData (
									textView, UseDefaultIID () );
			if ( tcData )
			{
				tcData->SetString ( emptyString );
			}
		}
	}
}


void NCSelectionObserver::DeleteAd_()
{
	char* 	buffer = NULL;
	int32	len = 0;

//	::SysBeep (9);
	DebugAlert_("DeleteAd_()");

	InterfacePtr<IBlackBoxCommands>blackBoxCmds (
				Utils<IBlackBoxCommands>().QueryUtilInterface());

	// get list of selected object from our selection suite
	UIDList* list = GetSelection ();

	if ( list )
	{
		for ( int32 i=0 ; i<list->Length() ; i++ )
		{
			buffer = new char [1024];
			len = blackBoxCmds->GetData (list->GetRef(i), 
										kNC_PluginID, buffer, 1024);
			if (len == 1024)
			{
				delete [] buffer;
				buffer = new char [2048];
				len = blackBoxCmds->GetData (list->GetRef(i), 
										kNC_PluginID, buffer, 2048);
			}

			if ( len )
			{
				DebugAlert_("NCSelectionObserver::DeleteAd_()\n Have a "
								"page item with our custom data on it");
				buffer[len < 2048 ? len : 2048] = '\0';
				DebugAlert_(buffer);
				DeleteAd_(list->GetRef(i));
				UpdateUnplacedAdsDocumentData_(buffer);
			}
			else
			{
				PMString pmstr3 ("NCSelectionObserver::DeleteAd_()\n Page item doesn't have our data -- should never happen!");
				CAlert::InformationAlert (pmstr3);
			}
		}
	}

}


void NCSelectionObserver::DeleteAd_(UIDRef inAdGroupItemUIDRef)
{
	extern void DeletePageItem ( UIDRef pageItemUID );

	DebugAlert_("DeleteAd_(UIDRef inAdGroupItemUIDRef)");
	
	// Try deleting the group item.
	DeletePageItem (inAdGroupItemUIDRef);
}


void NCSelectionObserver::UpdateUnplacedAdsDocumentData_(char* inAdData)
{
//	DebugAlert_("UpdateUnplacedAdsDocumentData_() called.");

	// get our custom data from the current document
	IDocument* doc = Utils<ILayoutUIUtils>()->GetFrontDocument();
	if ( doc )
	{
		InterfacePtr<IBlackBoxData> pIBlackBoxData ( doc, UseDefaultIID() );
		if ( pIBlackBoxData )
		{
			// Read in the current document slug.
			int32 docSlugDataLen = pIBlackBoxData->GetDataLength (kNC_PluginID);
			char* dataStr = new char [docSlugDataLen + 1];
			pIBlackBoxData->ReadData (kNC_PluginID, (void*)dataStr, docSlugDataLen);
			dataStr[docSlugDataLen] = '\0';
//			DebugAlert_(dataStr);
			
			// Append the deleted ad's slug data to the document slug data.
			int32 adSlugDataLen = ::strlen (inAdData);
			char* newDocSlug = new char [docSlugDataLen + adSlugDataLen + 4];
			::strcpy (newDocSlug, dataStr);
			newDocSlug[docSlugDataLen] = cStartDelim;
			newDocSlug[docSlugDataLen + 1] = '\0';
			::strcat (newDocSlug, inAdData);
			newDocSlug[docSlugDataLen + adSlugDataLen] = cEndDelim;
			newDocSlug[docSlugDataLen + adSlugDataLen + 1] = '\0';
//			DebugAlert_(newDocSlug);
			delete [] dataStr;
			
			// Rewrite the document slug with the new data.
			pIBlackBoxData->RemoveData (kNC_PluginID);
			pIBlackBoxData->WriteData ( kNC_PluginID, newDocSlug, 
														strlen (newDocSlug));
			// To free store
			delete [] newDocSlug;
		}
	}
}


// END OF FILE
