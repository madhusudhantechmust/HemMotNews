//========================================================================================
//  
//  $File: MyListBoxHelper.h $
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

#ifndef __MyListBoxHelper_h__
#define __MyListBoxHelper_h__

class IPMUnknown;
class IControlView;

class NCListBoxCell {
	public:
		NCListBoxCell ()												// Create
			:	cstr_(NULL), widgetid_(-1)
		{}

		NCListBoxCell (const char* inCStr, const WidgetID inWidgetID)
			:	cstr_(NULL), widgetid_(inWidgetID)
		{
			if (inCStr)
			{
				cstr_ = new char [ ::strlen (inCStr) + 1 ];
				::strcpy (cstr_, inCStr);
			}
		}

		NCListBoxCell (const NCListBoxCell & inObj)						// Copy
		{
			char* inCStr = inObj.StringData_();
			if (inCStr)
			{
				delete [] cstr_;
				cstr_ = new char [ ::strlen (inCStr) + 1 ];
				::strcpy (cstr_, inCStr);
			}
			widgetid_ = inObj.WidgetID_();
		}
		
		NCListBoxCell & operator= (const NCListBoxCell & inObj)			// Assign
		{
			if (this != &inObj)								// Beware of "a = a"
			{
				char* inCStr = inObj.StringData_();
				if (inCStr)
				{
					delete [] cstr_;
					cstr_ = new char [ ::strlen (inCStr) + 1 ];
					::strcpy (cstr_, inCStr);
				}
				widgetid_ = inObj.WidgetID_();
			}
		
			return *this;
		}
		
		virtual	~NCListBoxCell ()										// Destroy
		{
			delete [] cstr_;
		}
		
		/* --------------------------- S E T T E R S ---------------------------- */
		void SetStringData_(const char* inCStr)
		{
			delete [] cstr_;
			cstr_ = new char [ ::strlen (inCStr) + 1 ];
			::strcpy (cstr_, inCStr);
		}

		void SetWidgetID_(const WidgetID inWidgetID)
		{
			widgetid_ = inWidgetID;
		}

		/* --------------------------- G E T T E R S ---------------------------- */
		char* StringData_() const					{ return cstr_;				}
		void GetDataAsPMString_(
						PMString& outPMStr) const	{ outPMStr = cstr_;			}
		WidgetID WidgetID_() const					{ return widgetid_;			}

	protected:
	private:
		char*		cstr_;
		WidgetID	widgetid_;
};

/**
	Sample code that hides some of the detail of working with the list-box API.
	The list box could be on a panel or a dialog.

	
*/
class MyListBoxHelper
{
public:
	/**
		Constructor.
		@param fOwner reference to boss object using this helper.
		@param pluginID plug-in where resources can be found
		@param listBoxID the list box widget ID
		@param owningWidgetID could be a panel or a dialog this listbox is on
	*/
	MyListBoxHelper(IPMUnknown* fOwner, const PluginID& pluginID, const WidgetID& listBoxID, const WidgetID& owningWidgetID);
	virtual ~MyListBoxHelper();

	/**
		Add an element with the specified name at a given location in the listbox.
		@param displayName string value
		@param updateWidgetId the text widget ID within the cell
		@param atIndex specify the location, by default, the end of the list-box.
	*/
	void AddElement ( const PMString & displayName, WidgetID updateWidgetId, 
						const PMString & displayName2, WidgetID updateWidget2Id, 
						int atIndex = -2 /* kEnd */);
	void AddRow_(NCListBoxCell& clientCell, NCListBoxCell& fileNameCell, 
						NCListBoxCell& adSizeCell, NCListBoxCell& colorCell, 
						NCListBoxCell& sectionCell, NCListBoxCell& pageRangeCell, 
						int atIndex = -2);

	/**
		Method to remove a list-box element by position.
		@param indexRemove position of element to remove.
	*/
	void RemoveElementAt(int indexRemove);
	
	/**
		Method to remove the last element in the list-box.
	*/
	void RemoveLastElement();

	/**
		Query for the list-box on the panel or a dialog that is currently visible, assumes one visible at a time.
		@return reference to the current listbox, not add-ref'd.
	*/
	IControlView * FindCurrentListBox();

	/**
		Method to delete all the elements in the list-box.
	*/
	void EmptyCurrentListBox();

	/**
		Accessor for the number of elements in encapsulated list-box.
	*/
	int GetElementCount();
	
private:
	// impl methods
	bool16 verifyState() { return (fOwner!=nil) ? kTrue : kFalse; }
	/**
		helper method to a new list element widget.
	*/
	void AddListElementWidget (InterfacePtr<IControlView> & elView, 
				const PMString & displayName, WidgetID updateWidgetId, 
				const PMString & displayName2, WidgetID updateWidget2Id, int atIndex);
	void AddListElementWidget (InterfacePtr<IControlView> & elView, 
				NCListBoxCell& clientCell, NCListBoxCell& fileNameCell, 
				NCListBoxCell& adSizeCell, NCListBoxCell& colorCell, 
				NCListBoxCell& sectionCell, NCListBoxCell& pageRangeCell, 
				int atIndex);

	/**
		Helper method to remove the specified index.
	*/
	void removeCellWidget(IControlView * listBox, int removeIndex);

	IPMUnknown * fOwner;
	PluginID fOwningPluginID;
	WidgetID fListboxID;
	WidgetID fOwningWidgetID;
};


#endif // __MyListBoxHelper_h__




