//========================================================================================
//  
//  $File: $
//  
//  Owner: Ross L. Edgar
//  
//  $Author: $
//  
//  $DateTime: $
//  
//  $Revision: $
//  
//  $Change: $
//  
//  Copyright 1997-2005 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#include "VCPlugInHeaders.h"

// Interface includes:
#include "IControlView.h"		// UpdateActionStates ()
#include "IWidgetUtils.h"		// UpdateActionStates ()
#include "IActionStateList.h"	// UpdateActionStates ()

// General includes:
#include "CActionComponent.h"
#include "CAlert.h"
#include "PalettePanelUtils.h"		// UpdateActionStates ()
#include "MyListBoxHelper.h"		// UpdateActionStates ()

// Project includes:
#include "NCPaletteID.h"

/** Implements IActionComponent; performs the actions that are executed when the plug-in's
	menu items are selected.

	
	@ingroup newscad_ui

*/
class NCPaletteActionComponent : public CActionComponent {
public:
	NCPaletteActionComponent (IPMUnknown* boss);
	virtual void DoAction (IActiveContext* ac, ActionID actionID, 
								GSysPoint mousePoint, IPMUnknown* widget);
	virtual void UpdateActionStates(IActiveContext *myContext, IActionStateList *listToUpdate, GSysPoint mousePoint, IPMUnknown *widget);

private:
	void DoAbout ();
//	void DoRemoveItemFromList_(IPMUnknown *invokedWidget);
};

/* CREATE_PMINTERFACE
 Binds the C++ implementation class onto its
 ImplementationID making the C++ code callable by the
 application.
*/
CREATE_PMINTERFACE(NCPaletteActionComponent, kNCPaletteActionComponentImpl)

/* NCPaletteActionComponent Constructor
*/
/**
Constructor.
@param boss interface ptr from boss object on which this 
interface is aggregated.
 */
NCPaletteActionComponent::NCPaletteActionComponent(IPMUnknown* boss)
: CActionComponent(boss)
{
}

/* DoAction
*/
/** The action component should perform the requested action.
	This is where the menu item's action is taken.
	When a menu item is selected, the Menu Manager determines
	which plug-in is responsible for it, and calls its DoAction
	with the ID for the menu item chosen.

	@param actionID identifies the menu item that was selected.
	@param ac active context
	@param mousePoint contains the global mouse location at time of event causing action (e.g. context menus). kInvalidMousePoint if not relevant.
	@param widget contains the widget that invoked this action. May be nil. 
	*/
void NCPaletteActionComponent::DoAction (IActiveContext* ac, ActionID actionID, 
										GSysPoint mousePoint, IPMUnknown* widget)
{
	switch (actionID.Get())
	{
		case kNCPalettePopupAboutThisActionID:
		case kNCPaletteAboutActionID:
		{
			this->DoAbout();
			break;
		}

		default:
		{
			break;
		}
	}
}

/* DoAbout
*/
/** Encapsulates functionality for the about menu item. */
void NCPaletteActionComponent::DoAbout ()
{
	CAlert::ModalAlert
	(
		kNCPaletteAboutBoxStringKey,		// Alert string
		kOKString, 							// OK button
		kNullString, 						// No second button
		kNullString, 						// No third button
		1,									// Set OK button to default
		CAlert::eInformationIcon			// Information icon.
	);
}


/* UpdateActionStates

*/

void NCPaletteActionComponent::UpdateActionStates (
				IActiveContext *myContext, IActionStateList *listToUpdate, 
				GSysPoint mousePoint, IPMUnknown *widget)
{
	do {
		InterfacePtr<IControlView> listBox ( static_cast<IControlView*>(
						Utils<IWidgetUtils>()->QueryRelatedWidget (
						widget, kUnplacedAdsListBoxWidgetID, IID_ICONTROLVIEW)));

		int listElementCount = 0;
		if(listBox != nil) {
			MyListBoxHelper listHelper (listBox, kNCPalettePluginID, 
							kUnplacedAdsListBoxWidgetID, kNCPalettePanelWidgetID);
			listElementCount = listHelper.GetElementCount ();
		}
			
		for (int i = 0; i < listToUpdate->Length (); i++) {

			ActionID action = listToUpdate->GetNthAction (i);
			switch (action.Get ())
			{
				case kNCPalettePopupRemoveFromListActionID:
					if(listElementCount>0) {
						listToUpdate->SetNthActionState(i,kEnabledAction);
					}
					else {
						listToUpdate->SetNthActionState(i,kDisabled_Unselected);
					}
					break;
				
				case kNCPalettePopupPlaceAdActionID:
					if(listElementCount>0) {
						listToUpdate->SetNthActionState(i,kEnabledAction);
					}
					else {
						listToUpdate->SetNthActionState(i,kDisabled_Unselected);
					}
					break;
			}
		}
	} while (0);
}


/* DoRemoveItemFromList_
*/
/** Encapsulates functionality for the RemoveItem menu item. 

	@param invokedWidget Widget that initiated the event. (The panel menu.)
*/
/*
void NCPaletteActionComponent::DoRemoveItemFromList_(IPMUnknown *invokedWidget)
{
	InterfacePtr<IControlView> listBox (static_cast<IControlView*>(Utils<IWidgetUtils>()->QueryRelatedWidget(invokedWidget, kUnplacedAdsListBoxWidgetID, IID_ICONTROLVIEW)));

	if(listBox != nil) {
		MyListBoxHelper listHelper(listBox, kNCPalettePluginID, kUnplacedAdsListBoxWidgetID, kNCPalettePanelWidgetID);
		listHelper.RemoveElementAt (0);
	}
}
*/

// END OF FILE
