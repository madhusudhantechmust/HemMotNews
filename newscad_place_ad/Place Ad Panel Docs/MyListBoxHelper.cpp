//========================================================================================
//  
//  $File: MyListBoxHelper.cpp $
//  
//  Owner: Adobe Developer Technologies
//  
//  $Author: pmbuilder $
//  
//  $DateTime: 2005/03/08 13:31:35 $
//  
//  $Revision: #1 $
//  
//  $Change: 323509 $
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
// Interface includes

#include "IListControlDataOf.h"
#include "IPanelControlData.h"
#include "ITextControlData.h"
#include "IWidgetParent.h"
#include "IWidgetUtils.h"
#include "IListBoxAttributes.h"
#include "IControlView.h"
#include "IApplication.h"
// implem includes
#include "PersistUtils.h" // GetDatabase
#include "PalettePanelUtils.h"

#include "CreateObject.h"
#include "CoreResTypes.h"
#include "LocaleSetting.h"
#include "RsrcSpec.h"

#include "MyListBoxHelper.h"

MyListBoxHelper::MyListBoxHelper(IPMUnknown* owner, const PluginID& pluginID, 
								   const WidgetID& listBoxID, const WidgetID& owningWidgetID) : fOwner(owner), 
													fOwningPluginID(pluginID), fListboxID(listBoxID),
													fOwningWidgetID(owningWidgetID)
{

}

MyListBoxHelper::~MyListBoxHelper()
{
	fOwner=nil;
}




IControlView * MyListBoxHelper ::FindCurrentListBox()
{
	if(!verifyState())
		return nil;

	IControlView * listBoxControlView = nil;
	do {
		InterfacePtr<IPanelControlData> iPanelControlData(fOwner,UseDefaultIID());
		ASSERT_MSG(iPanelControlData != nil, "MyListBoxHelper ::FindCurrentListBox() iPanelControlData nil");
		if(iPanelControlData == nil) {
			break;
		}
		listBoxControlView = 	iPanelControlData->FindWidget(fListboxID);
		ASSERT_MSG(listBoxControlView != nil, "MyListBoxHelper ::FindCurrentListBox() no listbox");
		if(listBoxControlView == nil) {
			break;
		}
	
	} while(0);

	return listBoxControlView;
}

 

void MyListBoxHelper::AddElement (	const PMString & displayName, 
									WidgetID updateWidgetId, 
									const PMString & displayName2, 
									WidgetID updateWidget2Id, 
									int atIndex)
{
	if(!verifyState())
		return;

	do			// false loop
	{
		IControlView * listBox = this->FindCurrentListBox();
		if(listBox == nil) {
			break;
		}
		// Create an instance of a list element
		InterfacePtr<IListBoxAttributes> listAttr(listBox, UseDefaultIID());
		if(listAttr == nil) {
			break;	
		}
		RsrcID widgetRsrcID = listAttr->GetItemWidgetRsrcID();
		if (widgetRsrcID == 0)
				return;
		RsrcSpec elementResSpec(LocaleSetting::GetLocale(), fOwningPluginID, kViewRsrcType, widgetRsrcID);
		// Create an instance of the list element type
		InterfacePtr<IControlView> newElView( (IControlView*) ::CreateObject(::GetDataBase(listBox), elementResSpec, IID_ICONTROLVIEW));
		ASSERT_MSG(newElView != nil, "MyListBoxHelper::AddElement() Cannot create element");
		if(newElView == nil) {
			break;
		}
		this->AddListElementWidget (newElView, displayName, updateWidgetId, 
											displayName2, updateWidget2Id, atIndex);
	
	}
	while (false);			// false loop
}


/*================================================================================*/
void MyListBoxHelper::AddRow_(NCListBoxCell& clientCell, NCListBoxCell& fileNameCell, 
	NCListBoxCell& adSizeCell, NCListBoxCell& colorCell, NCListBoxCell& sectionCell, 
	NCListBoxCell& pageRangeCell, int atIndex) {
/*----------------------------------------------------------------------------------
	Abstract:
		Setup to put the visual representation of the slug data for an ad on screen.		
		
	Parameters and modes:
		References to the cells for each of the 6 columns in our list box.		IN
		
	Returns:
		Nothing.
		
	Change Log:
		08-Jun-06	New.
----------------------------------------------------------------------------------*/
	if(!verifyState())
		return;

	do			// false loop
	{
		IControlView * listBox = this->FindCurrentListBox();
		if(listBox == nil) {
			break;
		}

		// Create an instance of a list element
		InterfacePtr<IListBoxAttributes> listAttr(listBox, UseDefaultIID());
		if(listAttr == nil) {
			break;	
		}

		RsrcID widgetRsrcID = listAttr->GetItemWidgetRsrcID();
		if (widgetRsrcID == 0)
				return;
		
		RsrcSpec elementResSpec (LocaleSetting::GetLocale(), 
									fOwningPluginID, kViewRsrcType, widgetRsrcID);

		// Create an instance of the list element type
		InterfacePtr<IControlView> newElView ( 
							(IControlView*) ::CreateObject(::GetDataBase(listBox), 
							elementResSpec, IID_ICONTROLVIEW));
		
		ASSERT_MSG (newElView != nil, 
							"MyListBoxHelper::AddRow_() Cannot create element");
		if(newElView == nil) {
			break;
		}

		AddListElementWidget (newElView, clientCell, fileNameCell, adSizeCell, 
								colorCell, sectionCell, pageRangeCell, atIndex);
		
		// TBD:  ENABLE THE BUTTONS HERE OR ELSE IN 
		// NCPaletteListBoxObserver::PutBlackBoxDataIntoListBox_() -- 
		// THERE'S AT LEAST 1 ITEM IN THE LIST.
	}
	while (false);			// false loop
}

void MyListBoxHelper::RemoveElementAt (int indexRemove)
{
	if(!verifyState())
		return;

	do			// false loop
	{
		IControlView * listBox = this->FindCurrentListBox();
		if(listBox == nil) {
			break;
		}
		InterfacePtr<IListControlData> listControlData(listBox, UseDefaultIID());
		ASSERT_MSG(listControlData != nil, "MyListBoxHelper::RemoveElementAt() Found listbox but not control data?");
		if(listControlData==nil) {
			break;
		}
		if(indexRemove < 0 || indexRemove >= listControlData->Length()) {
			// Don't remove outside of list data bounds
			break;
		}
		removeCellWidget(listBox, indexRemove);
		listControlData->Remove(indexRemove);
	}
	while (false);			// false loop
}



void MyListBoxHelper::RemoveLastElement()
{
	if(!verifyState())
		return;

	do
	{
		IControlView * listBox = this->FindCurrentListBox();
		if(listBox == nil) {
			break;
		}

		
		InterfacePtr<IListControlData> listControlData(listBox, UseDefaultIID());
		ASSERT_MSG(listControlData != nil, "MyListBoxHelper::RemoveLastElement() Found listbox but not control data?");
		if(listControlData==nil) {
			break;
		}
		int lastIndex = listControlData->Length()-1;
		if(lastIndex >= 0) {		
			listControlData->Remove(lastIndex);
			removeCellWidget(listBox, lastIndex);
		}
		
	}
	while (false);
}



int MyListBoxHelper::GetElementCount() 
{
	int retval=0;
	do {
	
		IControlView * listBox = this->FindCurrentListBox();
		if(listBox == nil) {
			break;
		}

		InterfacePtr<IListControlData> listControlData(listBox, UseDefaultIID());
		ASSERT_MSG(listControlData != nil, "MyListBoxHelper::GetElementCount() Found listbox but not control data?");
		if(listControlData==nil) {
			break;
		}
		retval = listControlData->Length();
	} while(0);
	
	return retval;
}

void MyListBoxHelper::removeCellWidget(IControlView * listBox, int removeIndex) {
	
	do {

		if(listBox==nil) break;
		// recall that when the element is added, it is added as a child of the cell-panel
		// widget. Therefore, navigate to the cell panel and remove the child at the specified
		// index. Simultaneously, remove the corresponding element from the list controldata.
		// +
		InterfacePtr<IPanelControlData> panelData(listBox, UseDefaultIID());
		ASSERT_MSG(panelData != nil, "MyListBoxHelper::removeCellWidget()  Cannot get panelData");
		if(panelData == nil) {
			break;
		}
		
		IControlView* cellControlView = panelData->FindWidget(kCellPanelWidgetID);
		ASSERT_MSG(cellControlView != nil, "MyListBoxHelper::removeCellWidget() cannot find cellControlView");
		if(cellControlView == nil) {
			break;
		}

		InterfacePtr<IPanelControlData> cellPanelData (cellControlView, UseDefaultIID());
		ASSERT_MSG(cellPanelData != nil,"MyListBoxHelper::removeCellWidget() cellPanelData nil"); 
		if(cellPanelData == nil) {
			break;
		}

		if(removeIndex < 0 || removeIndex >= cellPanelData->Length()) {
			break;
		}
		
		InterfacePtr<IControlView> view(cellPanelData->GetWidget(removeIndex), IID_ICONTROLVIEW);
		ASSERT_MSG(view != nil,"MyListBoxHelper::removeCellWidget() view nil");
		
		cellPanelData->RemoveWidget(removeIndex);
		
		if( view )
			Utils<IWidgetUtils>()->DeleteWidgetAndChildren(view);
		// -

	} while(0);

}


void MyListBoxHelper::AddListElementWidget (
						InterfacePtr<IControlView> & elView, 
						const PMString & displayName, WidgetID updateWidgetId, 
						const PMString & displayName2, WidgetID updateWidget2Id, 
						int atIndex)
{
	IControlView * listbox = this->FindCurrentListBox();
	if(elView == nil || listbox == nil ) {
		return;
	}

	do {
		// Find the child widgets
		InterfacePtr<IPanelControlData> newElPanelData (elView, UseDefaultIID());
		if (newElPanelData == nil) {
			break;
		}
		// Locate the child that displays the 'name' value
		IControlView* nameTextView = newElPanelData->FindWidget(updateWidgetId);
		if ( (nameTextView == nil)  ) {
			break;
		}
		// Set the  name in the static text widget of this element
		InterfacePtr<ITextControlData> newEltext (nameTextView,UseDefaultIID());
		if (newEltext == nil) {
			break;
		}	
		newEltext->SetString(displayName, kTrue, kTrue);

		// Locate the child that displays the 'name' value
		IControlView* nameText2View = newElPanelData->FindWidget(updateWidget2Id);
		if ( (nameText2View == nil)  ) {
			break;
		}
		// Set the  name in the static text widget of this element
		InterfacePtr<ITextControlData> newEltext2 (nameText2View,UseDefaultIID());
		if (newEltext2 == nil) {
			break;
		}	
		newEltext2->SetString(displayName2, kTrue, kTrue);

		// Find the Cell Panel widget and it's panel control data interface
		InterfacePtr<IPanelControlData> panelData(listbox,UseDefaultIID());
		ASSERT_MSG(panelData != nil, "MyListBoxHelper::AddListElementWidget() Cannot get panelData");
		if(panelData == nil) {
			break;
		}

		IControlView* cellControlView = panelData->FindWidget(kCellPanelWidgetID);
		ASSERT_MSG(cellControlView != nil, "MyListBoxHelper::AddListElementWidget() cannot find cellControlView");
		if(cellControlView == nil) {
			break;
		}

		InterfacePtr<IPanelControlData> cellPanelData (cellControlView, UseDefaultIID());
		ASSERT_MSG(cellPanelData != nil, "MyListBoxHelper::AddListElementWidget()  cellPanelData nil");
		if(cellPanelData == nil) {
			break;
		}

		// Add the element widget to the list
		if(atIndex<0 || atIndex >= cellPanelData->Length()) {
			// Caution: an index of (-1) signifies add at the end of the panel controldata, but
			// and index of (-2) signifies add at the end of the list controldata.
			cellPanelData->AddWidget(elView);
		// add at the end (default)
		}
		else {
			cellPanelData->AddWidget(elView,atIndex);	
		}
		InterfacePtr< IListControlDataOf<IControlView*> > listData(listbox, UseDefaultIID());
		ASSERT_MSG(listData != nil, "MyListBoxHelper::AddListElementWidget() listData nil");
		if(listData == nil) { 
			break;
		}
		listData->Add(elView, atIndex);
	} while(0);

}


/*================================================================================*/
void MyListBoxHelper::AddListElementWidget (InterfacePtr<IControlView> & elView, 
	NCListBoxCell& clientCell, NCListBoxCell& fileNameCell, 
	NCListBoxCell& adSizeCell, NCListBoxCell& colorCell, NCListBoxCell& sectionCell, 
	NCListBoxCell& pageRangeCell, int atIndex) {
/*----------------------------------------------------------------------------------
	Abstract:
		This function adds an element (1 row by 6 columns) to the list box.  It 
		is one of the kludgiest things I've written in years because all the code 
		is dumped inline.
		
	Parameters and modes:

	Returns:

	Change Log:
----------------------------------------------------------------------------------*/
	IControlView * listbox = this->FindCurrentListBox();
	if(elView == nil || listbox == nil ) {
		return;
	}

	do {
		// Find the child widgets
		InterfacePtr<IPanelControlData> newElPanelData (elView, UseDefaultIID());
		if (newElPanelData == nil) {
			break;
		}
		
		// Locate the child that displays the "Client" value
		IControlView* clientTextView = 
							newElPanelData->FindWidget (clientCell.WidgetID_());
		if ( (clientTextView == nil)  ) {
			break;
		}
		// Set the  name in the static text widget of this element
		InterfacePtr<ITextControlData> clientEltext (
												clientTextView, UseDefaultIID ());
		if (clientEltext == nil) {
			break;
		}
		PMString clientPMStr;
		clientCell.GetDataAsPMString_(clientPMStr);
		clientEltext->SetString (clientPMStr, kTrue, kTrue);

		// Locate the child that displays the "Filename" value
		IControlView* filnamTextView = 
							newElPanelData->FindWidget (fileNameCell.WidgetID_());
		if ( (filnamTextView == nil)  ) {
			break;
		}
		// Set the  name in the static text widget of this element
		InterfacePtr<ITextControlData> filnamEltext (filnamTextView, UseDefaultIID());
		if (filnamEltext == nil) {
			break;
		}	
		PMString filnamPMStr;
		fileNameCell.GetDataAsPMString_(filnamPMStr);
		filnamEltext->SetString (filnamPMStr, kTrue, kTrue);

		// Locate the child that displays the "Ad Size" value
		IControlView* adsizeTextView = 
							newElPanelData->FindWidget (adSizeCell.WidgetID_());
		if ( (adsizeTextView == nil)  ) {
			break;
		}
		// Set the  name in the static text widget of this element
		InterfacePtr<ITextControlData> adsizeEltext (adsizeTextView, UseDefaultIID());
		if (adsizeEltext == nil) {
			break;
		}	
		PMString adsizePMStr;
		adSizeCell.GetDataAsPMString_(adsizePMStr);
		adsizeEltext->SetString (adsizePMStr, kTrue, kTrue);

		// Locate the child that displays the "Colors" (4/C, 2/C, BW) value
		IControlView* colorTextView = 
							newElPanelData->FindWidget (colorCell.WidgetID_());
		if ( (colorTextView == nil)  ) {
			break;
		}
		// Set the  name in the static text widget of this element
		InterfacePtr<ITextControlData> colorEltext (colorTextView, UseDefaultIID());
		if (colorEltext == nil) {
			break;
		}	
		PMString colorPMStr;
		colorCell.GetDataAsPMString_(colorPMStr);
		colorEltext->SetString (colorPMStr, kTrue, kTrue);

		// Locate the child that displays the "Section" value
		IControlView* sectionTextView = 
							newElPanelData->FindWidget (sectionCell.WidgetID_());
		if ( (sectionTextView == nil)  ) {
			break;
		}
		// Set the  name in the static text widget of this element
		InterfacePtr<ITextControlData> sectionEltext (
											sectionTextView, UseDefaultIID () );
		if (sectionEltext == nil) {
			break;
		}	
		PMString sectionPMStr;
		sectionCell.GetDataAsPMString_(sectionPMStr);
		sectionEltext->SetString (sectionPMStr, kTrue, kTrue);

		// Locate the child that displays the "Page Range" value
		IControlView* pgrngTextView = 
							newElPanelData->FindWidget (pageRangeCell.WidgetID_() );
		if ( (pgrngTextView == nil)  ) {
			break;
		}
		// Set the  name in the static text widget of this element
		InterfacePtr<ITextControlData> pgrngEltext (
												pgrngTextView, UseDefaultIID () );
		if (pgrngEltext == nil) {
			break;
		}	
		PMString pgrngPMStr;
		pageRangeCell.GetDataAsPMString_(pgrngPMStr);
		pgrngEltext->SetString (pgrngPMStr, kTrue, kTrue);

		// Find the Cell Panel widget and its panel control data interface
		InterfacePtr<IPanelControlData> panelData ( listbox, UseDefaultIID () );
		ASSERT_MSG (panelData != nil, 
				"MyListBoxHelper::AddListElementWidget() Cannot get panelData");
		if(panelData == nil) {
			break;
		}

		IControlView* cellControlView = panelData->FindWidget (kCellPanelWidgetID);
		ASSERT_MSG (cellControlView != nil, 
			"MyListBoxHelper::AddListElementWidget() cannot find cellControlView");
		if(cellControlView == nil) {
			break;
		}

		InterfacePtr<IPanelControlData> cellPanelData (
												cellControlView, UseDefaultIID());
		ASSERT_MSG(cellPanelData != nil, 
					"MyListBoxHelper::AddListElementWidget()  cellPanelData nil");
		if(cellPanelData == nil) {
			break;
		}

		// Add the element widget to the list
		if ( atIndex < 0 || atIndex >= cellPanelData->Length() ) {
			// Caution: an index of (-1) signifies add at the end of the 
			// panel controldata, but an index of (-2) signifies add at 
			// the end of the list controldata.
			cellPanelData->AddWidget (elView);
			// add at the end (default)
		}
		else {
			cellPanelData->AddWidget (elView, atIndex);	
		}

		InterfacePtr< IListControlDataOf<IControlView*> > 
											listData (listbox, UseDefaultIID());
		ASSERT_MSG(listData != nil, 
						"MyListBoxHelper::AddListElementWidget() listData nil");
		if(listData == nil) { 
			break;
		}
		listData->Add (elView, atIndex);
	} while (0);

}

void MyListBoxHelper::EmptyCurrentListBox()
{
	do {
		IControlView* listBoxControlView = this->FindCurrentListBox();
		if(listBoxControlView == nil) {
			break;
		}
		InterfacePtr<IListControlData> listData (listBoxControlView, UseDefaultIID());
		if(listData == nil) {
			break;
		}
		InterfacePtr<IPanelControlData> iPanelControlData(listBoxControlView, UseDefaultIID());
		if(iPanelControlData == nil) {
			break;
		}
		IControlView* panelControlView = iPanelControlData->FindWidget(kCellPanelWidgetID);
		if(panelControlView == nil) {
			break;
		}
		InterfacePtr<IPanelControlData> panelData(panelControlView, UseDefaultIID());
		if(panelData == nil) {
			break;
		}
		listData->Clear(kFalse, kFalse);
		panelData->ReleaseAll();
		listBoxControlView->Invalidate();
	} while(0);
}

