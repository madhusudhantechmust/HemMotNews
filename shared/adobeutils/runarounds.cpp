/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	runarounds.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	11/9/05 ca. 12:23 PM
	
	ab	Abstract:
	ab	Implementation of runarounds class.
	ab	
	
	cl	Change Log:
	cl	11/9/05	New.
----------------------------------------------------------------------------------*/
// Interface includes:
#include "ITextWrapFacade.h"
#include "IStandOff.h"

// General includes:
// #include "TextRange.h"
// #include "CmdUtils.h"
#include "UIDList.h"


// A D O B E   S T U F F
/*
FROM ITextWrapFacade.h
virtual ErrorCode SetMargins (const UIDList &textWrapItems, 
						const PMReal &topMargin, 
						const PMReal &leftMargin, 
						const PMReal &bottomMargin, 
						const PMReal &rightMargin) const =0

virtual ErrorCode SetIgnoreWrap 	( 	const UIDList & 	textWrapItems,
		bool8 	bIgnore
	) 	const [pure virtual]
Description: Should the text wrap be ignored?
Parameters:
    	textWrapItems 	IN: The list of text wrap items to which we will apply the ignore setting.
    	bIgnore 	IN: kTrue to ignore the text wrap. 
Returns:
    kTrue if the text wrap is user modified, kFalse otherwise.

FROM IStandOff.h:
enum mode {
	kNone, // no effect on text
	kBand, // text leads past area of strip
	kNextFrame, // text continues in next frame
	kBothSides, // text on both sides of object (PM)
	kWithinHoles, // text within holes in object
	kAutoContour, // text follows the shape (implies kBothSides)
	kManualContour, // text follows the standoff shape (implies kBothSides)
	kLeftContour, // text wraps only to the left side of the standoff
	kRightContour, // text wraps only to the right side of the standoff
	kDefaultMode = kNone
}; 
*/


/* ---------------------------- P R O T O T Y P E S ----------------------------- */

ErrorCode SetRunaround_(const UIDRef& graphicFrameUIDRef, 
						const PMReal inAmount = 0, 
						IStandOff::mode mode = IStandOff::kBothSides);
ErrorCode SetRunaround_(const UIDRef& graphicFrameUIDRef, 
						const PMReal &topMargin, 
						const PMReal &leftMargin, 
						const PMReal &bottomMargin, 
						const PMReal &rightMargin,
						IStandOff::mode mode = IStandOff::kBothSides);
ErrorCode ChangeTextWrapMode_(
						const UIDRef& graphicFrameUIDRef, IStandOff::mode mode);
ErrorCode SetIgnoreRunarounds_(const UIDRef& inTextFrameUIDRef, bool inIgnore);

#pragma mark -

/*================================================================================*/
ErrorCode SetRunaround_(const UIDRef& graphicFrameUIDRef, 
								const PMReal inAmount, IStandOff::mode inMode) {
/*----------------------------------------------------------------------------------
	Abstract:
		
	Parameters and modes:

	Returns:

	Change Log:
----------------------------------------------------------------------------------*/
	ErrorCode result = kFailure;
	Utils<HighLevelAPI::ITextWrapFacade> textWrapFacade;
	if (textWrapFacade)
	{
		result = textWrapFacade->SetMargins ( UIDList (graphicFrameUIDRef), 
								inAmount, inAmount, inAmount, inAmount);
		if (result == kSuccess)
		{
			result = ChangeTextWrapMode_(graphicFrameUIDRef, inMode);
		}
	}

	return result;
}


/*================================================================================*/
ErrorCode SetRunaround_(const UIDRef& graphicFrameUIDRef, 
						const PMReal &topMargin, const PMReal &leftMargin, 
						const PMReal &bottomMargin, const PMReal &rightMargin,
						IStandOff::mode inMode) {
/*----------------------------------------------------------------------------------
	Abstract:
		
	Parameters and modes:

	Returns:

	Change Log:
		19-SEP-07	New.
----------------------------------------------------------------------------------*/
	ErrorCode result = kFailure;
	Utils<HighLevelAPI::ITextWrapFacade> textWrapFacade;
	if (textWrapFacade)
	{
		result = textWrapFacade->SetMargins ( UIDList (graphicFrameUIDRef), 
								topMargin, leftMargin, bottomMargin, rightMargin);
		if (result == kSuccess)
		{
			result = ChangeTextWrapMode_(graphicFrameUIDRef, inMode);
		}
	}

	return result;
}


/*================================================================================*/
ErrorCode ChangeTextWrapMode_(
						const UIDRef& graphicFrameUIDRef, IStandOff::mode mode) {
/*----------------------------------------------------------------------------------
	Abstract:
		Adjust the stand off mode of the given frame using ITextWrapFacade.

	Parameters and modes:
		graphicFrameUIDRef	Reference to the frame object (kSplineItemBoss).	IN
		mode				The kind of stand off to be applied.				IN

	Returns:
		kSuccess on success, other ErrorCode otherwise.

	Change Log:
		09-Nov-05	I lifted this code from /Users/Shared/
					Adobe InDesign CS2 Products SDK/source/sdksamples/
					codesnippets/SnpManipulateTextFrame.cpp
					and made some minor changes.
----------------------------------------------------------------------------------*/
	ErrorCode result = kFailure;
	do {
		Utils<HighLevelAPI::ITextWrapFacade> textWrapFacade;
		if (!textWrapFacade) {
			break;
		}
		if (textWrapFacade->GetMode(graphicFrameUIDRef) == mode) {
			break;
		}
		bool16 setUserModified = kFalse;
		result = textWrapFacade->SetMode (UIDList(graphicFrameUIDRef), 
											mode, setUserModified);
		/*ASSERT_MSG(result == kSuccess, "ITextWrapFacade::SetMode failed");*/
	} while(false);
	return result;
}


/*================================================================================*/
ErrorCode SetIgnoreRunarounds_(const UIDRef& inTextFrameUIDRef, bool inIgnore) {
/*----------------------------------------------------------------------------------
	Abstract:
		Set the given text frame to ignore or obey runarounds of colliding items.

	Parameters and modes:
		inTextFrameUIDRef	Reference to the frame object (kSplineItemBoss).	IN
		inIgnore			Pass true to ignore or false to obey.				IN
		
	Returns:
		kSuccess on success, other ErrorCode otherwise.

	Change Log:
		12-Dec-05	New.
----------------------------------------------------------------------------------*/
	ErrorCode retval = kFailure;
	Utils<HighLevelAPI::ITextWrapFacade> textWrapFacade;
	if (textWrapFacade)
	{
		retval = textWrapFacade->SetIgnoreWrap ( UIDList (inTextFrameUIDRef), 
										inIgnore ? kTrue : kFalse);
	}

	return retval;
}

/*
#include "runarounds.hpp"

//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and D E S T R U C T I O N
//----------------------------------------------------------------------------------
CL_runarounds::CL_runarounds ()
{

}

CL_runarounds::~CL_runarounds ()
{

}
*/