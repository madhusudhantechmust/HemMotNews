/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	PlaceAdPanelDisplayAd.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	7/19/06 ca. 10:56 PM
	
	ab	Abstract:
	ab	Declarations for PlaceAdPanelDisplayAd class.  This class represents 
	ab	an ad created as a result of the user choosing an item in our "Place Ad" 
	ab	palette and clicking "OK" or choosing "Place Ad" from the hierarchical menu.
	
	cl	Change Log:
	cl	7/19/06	New.
----------------------------------------------------------------------------------*/

#pragma once

#include "Basic_Display_Ad.h"

class PlaceAdPanelDisplayAd : public Basic_Display_Ad {
public:
	PlaceAdPanelDisplayAd ();								// Create
	PlaceAdPanelDisplayAd (
			int32 paletteSelectedItemIndex);				// Create from integer
	PlaceAdPanelDisplayAd (string & inputRec);				// Create from string
	PlaceAdPanelDisplayAd (
				const PlaceAdPanelDisplayAd & inObj);		// Copy
	PlaceAdPanelDisplayAd & operator= (
			const PlaceAdPanelDisplayAd & inObj);			// Assign
	virtual	~PlaceAdPanelDisplayAd ();						// Destroy


/* ------------------- G E T T E R S   A N D   S E T T E R S -------------------- */

	int32 Index_()					{ return paletteSelectedItemIndex_; }
	void SetIndex_(int32 inIdx)		{ paletteSelectedItemIndex_ = inIdx; }
	int32 PaletteSelectedItemIndex_()	{ return paletteSelectedItemIndex_; }

	// Where the rubber meets the road: Put ad object in the middle of current page.
	bool Realize_();

protected:
	void FindRecord_(string& inBlackBoxData, 
						int32 inRecordIndex, string& outStr);
	void SetMemberVariablesFromString_(string& inRecord);
	void UpdateDataForDocumentSlug_(string& inBlackBoxData, 
								int32 inRecordIndex, string & outBlackBoxData);
	
private:
	int32 paletteSelectedItemIndex_;
};

