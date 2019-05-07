/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	NCPaletteListBoxObserver.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	5/23/06 ca. 9:51 PM
	
	ab	Abstract:
	ab	Implementation of NCPaletteListBoxObserver class.
	ab	
	
	cl	Change Log:
	cl	5/23/06	New.
----------------------------------------------------------------------------------*/
using namespace std;
#include <string>
#include "tokenizer.hpp"

#include "VCPlugInHeaders.h"

// Implementation includes
#include "WidgetID.h"

// Interface includes
#include "ISubject.h"
#include "IControlView.h"
#include "IPanelControlData.h"
#include "IListControlData.h"
#include "IListBoxController.h"
#include "IDocument.h" //###Added for DCN40953
#include "IActiveContext.h" //###Added for DCN40953
#include "IControlView.h" //###Added for DCN40953
#include "ILayoutControlData.h"
#include "IDocument.h"
#include "IBlackBoxCommands.h"
#include "IWidgetUtils.h"

// Implem includes
#include "CAlert.h"
#include "CObserver.h"
#include "NCPaletteID.h"
#include "MyListBoxHelper.h"

// General includes
#include "LayoutUIUtils.h"

#define		WANT_DEBUG_ALERTS		0

class NCPaletteListBoxObserver : public CObserver {
	public:
		NCPaletteListBoxObserver (IPMUnknown *boss);
		~NCPaletteListBoxObserver ();

		// AutoAttach is only called for registered observers
		// of widgets.  This method is called by the application
		// core when the widget is shown.
		virtual void AutoAttach ();
	
		// AutoDetach is only called for registered observers
		// of widgets. Called when widget hidden.
		virtual void AutoDetach ();

		// This class is interested in changes along IID_ILISTCONTROLDATA 
		// protocol with classID of kListSelectionChangedByUserMessage. 
		// This message is sent when a user clicks on an element in the list-box.
		virtual void Update (const ClassID& theChange, ISubject* theSubject, 
								const PMIID &protocol, void* changedBy);

	private:
		void UpdateListBox_(bool16 isAttaching);
		void PutBlackBoxDataIntoListBox_(char* inCStr);
		void AttachTo_(IControlView *);
		void DetachFrom_(IControlView *);
		bool16 ActiveDocChange_(IActiveContext* context);

		IDocument*		fCurrentDocument;
		IControlView*	fCurrentLayout;
};


CREATE_PMINTERFACE(NCPaletteListBoxObserver, kNCPaletteListBoxObserverImpl)


//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and D E S T R U C T I O N
//----------------------------------------------------------------------------------
NCPaletteListBoxObserver::NCPaletteListBoxObserver (IPMUnknown* boss)
	: CObserver (boss)
	, fCurrentDocument (nil)
	, fCurrentLayout (nil)
{
	// EMPTY
}


NCPaletteListBoxObserver::~NCPaletteListBoxObserver ()
{
	// EMPTY
}


/*================================================================================*/
void NCPaletteListBoxObserver::AutoAttach () {
/*----------------------------------------------------------------------------------
	Abstract:
		AutoAttach is only called for registered observers of widgets.  This method 
		is called by the application core when the widget is shown.
		
	Parameters and modes:
		None.

	Returns:
		Nothing.

	Change Log:
		24-May-06	New.
----------------------------------------------------------------------------------*/
	InterfacePtr<ISubject> subject(this, UseDefaultIID());
	if (subject != nil)
	{
		subject->AttachObserver (this, IID_ILISTCONTROLDATA);
	}
	UpdateListBox_(kTrue);

	//### Added for DCN40953 Attach for document change
	IActiveContext *context = gSession->GetActiveContext ();
    //this->ActiveDocChange(context);

	if (context)
	{
        InterfacePtr<ISubject> contextSubject (context, IID_ISUBJECT);

        if (contextSubject)
            contextSubject->AttachObserver(this, IID_IACTIVECONTEXT);

        IControlView* activeView = context->GetContextView();
        InterfacePtr<ILayoutControlData> isLayoutView( activeView, UseDefaultIID());

        if (isLayoutView && (activeView != fCurrentLayout))
        {
            if (fCurrentLayout)
                DetachFrom_(fCurrentLayout);

            AttachTo_(activeView);
        }
    }
}


/*================================================================================*/
void NCPaletteListBoxObserver::AutoDetach () {
/*----------------------------------------------------------------------------------
	Abstract:
		AutoDetach is only called for registered observers
		of widgets. Called when widget hidden.
		
	Parameters and modes:
		None.
		
	Returns:
		Nothing.
		
	Change Log:
		25-May-06	New.
----------------------------------------------------------------------------------*/
	UpdateListBox_(kFalse);
	InterfacePtr<ISubject> subject ( this, UseDefaultIID() );
	if (subject != nil)
	{
		subject->DetachObserver (this);
	}
	
	//###Added
	
	IActiveContext *context = gSession->GetActiveContext();
    if (context)
    {
        InterfacePtr<ISubject> contextSubject (context, IID_ISUBJECT);

        if (contextSubject && contextSubject->IsAttached (this, IID_IACTIVECONTEXT))
            contextSubject->DetachObserver (this, IID_IACTIVECONTEXT);
    }

    fCurrentDocument = nil;
    fCurrentLayout = nil;
}


/*================================================================================*/
void NCPaletteListBoxObserver::Update (const ClassID& theChange, 
					ISubject* theSubject, const PMIID &protocol, void* changedBy) {
/*----------------------------------------------------------------------------------
	Abstract:
		
	Parameters and modes:
		theChange	specified by the agent of change; it can be the 
					class ID of the agent, or it may be some specialised 
					message ID.
		theSubject	provides reference to the object which has changed; here 
					the button widget boss object that is being observed.
		protocol	the protocol along which the change occurred.
		changedBy	this can be used to provide additional information about 
					the change or a reference to the boss object that 
					caused the change.

	Returns:
		Nothing.
		
	Change Log:
		25-May-06	New.
----------------------------------------------------------------------------------*/
	if (protocol == IID_IACTIVECONTEXT)
	{
		const PMIID& id = *((const PMIID*)changedBy);
//		if (id == IID_ICONTROLVIEW)
//			CAlert::InformationAlert (PMString ("Something", -1, 
//										PMString::kNoTranslate));
		if (id == IID_IDOCUMENT)
//			CAlert::InformationAlert (PMString ("Document changed", -1, 
//										PMString::kNoTranslate));
		{
			MyListBoxHelper listHelper (this, kNCPalettePluginID, 
							kUnplacedAdsListBoxWidgetID, kNCPalettePanelWidgetID);
			listHelper.EmptyCurrentListBox ();
			UpdateListBox_(kTrue);
		}
	}

	if ((protocol == IID_ILISTCONTROLDATA) && 
			(theChange == kListSelectionChangedByUserMessage) )
	{
		do {
			MyListBoxHelper listHelper (this, kNCPalettePluginID, 
							kUnplacedAdsListBoxWidgetID, kNCPalettePanelWidgetID);
			IControlView* listBox = listHelper.FindCurrentListBox ();
			if (listBox == nil) {
				break;
			}
			
			InterfacePtr<IListBoxController> 
				listCntl (listBox, IID_ILISTBOXCONTROLLER);
								// useDefaultIID() not defined for this interface
//			ASSERT_MSG (listCntl != nil, "listCntl nil");
			if (listCntl == nil) {
				break;
			}
			
			// Get the first item in the selection (should be only one if the 
			// list box has been set up correctly in the framework resource file)
			K2Vector<int32> multipleSelection ;
			listCntl->GetSelected ( multipleSelection ) ;
			const int kSelectionLength =  multipleSelection.size () ;
			if ( kSelectionLength > 0 )
			{
#if WANT_DEBUG_ALERTS
				PMString dbgInfoString ("Selected item(s): ");
				for (int i = 0; i < multipleSelection.size () ; i++)
				{
					int indexSelected = multipleSelection[i];
					dbgInfoString.AppendNumber(indexSelected);
					if (i < kSelectionLength - 1) {
						dbgInfoString += ", ";
					}
				}
				dbgInfoString.SetTranslatable (kFalse);			// only for debug
				CAlert::InformationAlert (dbgInfoString);
#endif
#if 1				
				// TBD:  ENABLE THE BUTTONS, AS THE USER HAS SELECTED SOMETHING.
				InterfacePtr<IControlView> panelWidget (
					static_cast<IControlView*>(Utils<IWidgetUtils>()->QueryRelatedWidget (
										listBox, kNCPalettePanelWidgetID, IID_ICONTROLVIEW)));
				InterfacePtr<IPanelControlData> 
											panelControlData (panelWidget, UseDefaultIID ());

				IControlView* makeAdCView =	
							panelControlData->FindWidget (kNCPalettePlaceAdButtonWidgetID);
				IControlView* putBackCView = 
							panelControlData->FindWidget (kNCPaletteRemoveButtonWidgetID);
				
				// Enable the Place Ad and Put Back buttons
				makeAdCView->Enable (kTrue);
				putBackCView->Enable (kTrue);
#endif
			}
			
		} while (0);
	}
}


void NCPaletteListBoxObserver::UpdateListBox_(bool16 isAttaching)
{
/*
	PMString dbgInfoString("NCPaletteListBoxObserver::UpdateListBox_");
	dbgInfoString.AppendNumber(isAttaching);
	dbgInfoString.SetTranslatable(kFalse);	// only for debug- not real code
	CAlert::InformationAlert(dbgInfoString);
*/
/*
	MyListBoxHelper listHelper (this, kNCPalettePluginID, 
							kUnplacedAdsListBoxWidgetID, kNCPalettePanelWidgetID);
*/
//	CAlert::InformationAlert("Before listHelper.EmptyCurrentListBox()");
/*
	listHelper.EmptyCurrentListBox();
*/
//	CAlert::InformationAlert("After listHelper.EmptyCurrentListBox()");

	// See if we can find the listbox
	// If we can then populate the listbox in brain-dead way
	if(isAttaching)
	{
		do {
			// Get our custom data from the current document; exit on failure.
			IDocument* doc = ::GetFrontDocument();
			if ( !doc )
			{
				SysBeep (9);
				break;
			}
			InterfacePtr<IBlackBoxData> pIBlackBoxData ( doc, UseDefaultIID() );
			if ( !pIBlackBoxData ) 
			{
				SysBeep (9);
				break;
			}

			// Read in the current document slug.
			int32 docSlugDataLen = pIBlackBoxData->GetDataLength (kWFPPluginID);
			char* dataStr = new char [docSlugDataLen + 1];
			if (!dataStr) break;
			pIBlackBoxData->ReadData (kWFPPluginID, (void*)dataStr, docSlugDataLen);
			dataStr[docSlugDataLen] = '\0';
			PutBlackBoxDataIntoListBox_(dataStr);
			delete [] dataStr;
/*			
			const int targetDisplay1WidgetId = kNCPaletteText1WidgetID;
			const int targetDisplay2WidgetId = kNCPaletteText2WidgetID;
			const int targetDisplay3WidgetId = kNCPaletteText3WidgetID;
			const int targetDisplay4WidgetId = kNCPaletteText4WidgetID;
			const int targetDisplay5WidgetId = kNCPaletteText5WidgetID;
			const int targetDisplay6WidgetId = kNCPaletteText6WidgetID;

			const int kNumberItems = 12;
			for (int i = 0; i < kNumberItems; i++)
			{
				PMString name1 ("CLIENT ");
				name1.AppendNumber(i+1);
				NCListBoxCell clientCell (
								name1.GrabCString (), kNCPaletteText1WidgetID);

				PMString name2 ("FILE NAME ");
				name2.AppendNumber(i+1);
				NCListBoxCell fileNameCell (
								name2.GrabCString (), kNCPaletteText2WidgetID);

				PMString name3 ("AD SIZE ");
				name3.AppendNumber(i+1);
				NCListBoxCell adSizeCell (
								name3.GrabCString (), kNCPaletteText3WidgetID);

				PMString name4 ("4/C ");
				name4.AppendNumber(i+1);
				NCListBoxCell colorCell (
								name4.GrabCString (), kNCPaletteText4WidgetID);

				PMString name5 ("SECTION ");
				name5.AppendNumber(i+1);
				NCListBoxCell sectionCell (
								name5.GrabCString (), kNCPaletteText5WidgetID);

				PMString name6 ("PAGE RANGE ");
				name6.AppendNumber(i+1);
				NCListBoxCell pageRangeCell (
								name6.GrabCString (), kNCPaletteText6WidgetID);

				listHelper.AddRow_(clientCell, fileNameCell, adSizeCell, 
									colorCell, sectionCell, pageRangeCell);

			}
*/
		} while (0);
	}
}


void NCPaletteListBoxObserver::PutBlackBoxDataIntoListBox_(char* inCStr)
{
	MyListBoxHelper listHelper (this, kNCPalettePluginID, 
							kUnplacedAdsListBoxWidgetID, kNCPalettePanelWidgetID);
	listHelper.EmptyCurrentListBox ();
	typedef boost::tokenizer<boost::char_separator<char> > tokenizer;
	bool insideRecord = false;
	unsigned cntTokensProcessed = 0;
	boost::char_separator<char> sep("<>|\n\r", "<>", boost::keep_empty_tokens);
	string str = inCStr;
	tokenizer tokens (str, sep);
	tokenizer::iterator tok_iter;
	string adSizeStr, clientStr, fileNameStr, pageRangeStr, sectionStr, colorStr;
	unsigned countRowsAdded = 0;
	for (tok_iter = tokens.begin (); tok_iter != tokens.end (); ++tok_iter)
	{
		std::string s = *tok_iter;
		if ( insideRecord )
		{
			if (*tok_iter == ">")							// End of record
			{
				insideRecord = false;						// Outside the "<...>"
				cntTokensProcessed = 0;						// Reset for next rec

				// Start output
				NCListBoxCell clientCell (
									clientStr.c_str (), kNCPaletteText1WidgetID);
				NCListBoxCell fileNameCell (
									fileNameStr.c_str (), kNCPaletteText2WidgetID);
				NCListBoxCell adSizeCell (
									adSizeStr.c_str (), kNCPaletteText3WidgetID);
				NCListBoxCell colorCell (
									colorStr.c_str (), kNCPaletteText4WidgetID);
				NCListBoxCell sectionCell (
									sectionStr.c_str (), kNCPaletteText5WidgetID);
				NCListBoxCell pageRangeCell (
									pageRangeStr.c_str (), kNCPaletteText6WidgetID);
				listHelper.AddRow_(clientCell, fileNameCell, adSizeCell, 
									colorCell, sectionCell, pageRangeCell);
				++countRowsAdded;
			}
			else
			{
				++cntTokensProcessed;
				switch (cntTokensProcessed) {
					case 1:									// Ad Size -- columns wide
						adSizeStr = *tok_iter + " cl. x ";
						break;
					case 2:									// Ad Size -- points deep
						adSizeStr += *tok_iter + " pt";
						break;
					case 3:									// Client name
						clientStr = *tok_iter;
						break;
					case 4:									// Art filename
						fileNameStr = *tok_iter;
						break;
					case 7:									// "Lower" page
						if (tok_iter->length())
						{
							pageRangeStr = *tok_iter;
							pageRangeStr += '-';
						}
						break;
					case 8:									// "Upper" page
						if (tok_iter->length())
						{
							pageRangeStr += *tok_iter;
						}
						break;
					case 11:								// BW, 2/C, 4/C
						colorStr = *tok_iter;
						break;
					case 13:
						sectionStr = *tok_iter;
						break;
					default:
						break;
				}
			}
		}
		else
		{
			if (*tok_iter == "<")
			{
				adSizeStr.resize (0);
				clientStr.resize (0);
				fileNameStr.resize (0);
				pageRangeStr.resize (0);
				sectionStr.resize (0);
				colorStr.resize (0);
				insideRecord = true;
			}
		}
	}

#if 0	
	IControlView* listBox = listHelper.FindCurrentListBox();
	InterfacePtr<IControlView> panelWidget (
		static_cast<IControlView*>(Utils<IWidgetUtils>()->QueryRelatedWidget (
							listBox, kNCPalettePanelWidgetID, IID_ICONTROLVIEW)));
	InterfacePtr<IPanelControlData> 
								panelControlData (panelWidget, UseDefaultIID ());

	IControlView* makeAdCView =	
				panelControlData->FindWidget (kNCPalettePlaceAdButtonWidgetID);
	IControlView* putBackCView = 
				panelControlData->FindWidget (kNCPaletteRemoveButtonWidgetID);

	if (countRowsAdded)				// Anything in the palette?
	{
		// Enable the Place Ad and Put Back buttons
		makeAdCView->Enable (kTrue);
		putBackCView->Enable (kTrue);
	}
	else
	{
		// Disable the Place Ad and Put Back buttons
		makeAdCView->Enable (kFalse);
		putBackCView->Enable (kFalse);
	}
#endif

}


void NCPaletteListBoxObserver::AttachTo_(IControlView* inLayout)
{
	//    Trace("Attaching to layout %x\n",layout);
	//
	// Note: layout == nil is legal
	//
	if (fCurrentLayout != inLayout)
	{
		InterfacePtr<ISubject> subject (inLayout, IID_ISUBJECT);
		
		if (subject)
		{
			subject->AttachObserver (this, IID_ILAYOUTCONTROLDATA);
		}
		
		fCurrentLayout = inLayout;
	}
}


void NCPaletteListBoxObserver::DetachFrom_(IControlView* inLayout)
{
	//    Trace("Detaching from layout %x\n",layout);
	if (inLayout)
	{
		InterfacePtr<ISubject> subject (inLayout, IID_ISUBJECT);
	
		if (subject)
			subject->DetachObserver(this, IID_ILAYOUTCONTROLDATA);
	}
}


bool16 NCPaletteListBoxObserver::ActiveDocChange_(IActiveContext* context)
{
    IDocument* theDocument = context->GetContextDocument();
    bool16 bReallyChanged = kFalse;

    //if (theDocument != fCurrentDocument)
    //    {
    //    if (fCurrentDocument)
    //        this->DetachFrom(fCurrentDocument);

    //    if (theDocument)
    //        this->AttachTo(theDocument);

    //    bReallyChanged = kTrue;
    //    }

    return(bReallyChanged);
}


// END OF FILE
