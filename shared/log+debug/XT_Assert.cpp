/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	XT_Assert.cpp

	au	Author:
	au	Ross L. Edgar

	da	Date
	da	Friday, January 23, 2004.

	ab	Abstract:
	ab	

	cl	Change Log:
	cl	27-Oct-10	Began port to InDesign CS4 on Intel Macs.
------------------------------------------------------------------------------*/
#include "CAlert.h"
#include "XT_Assert.h"

// DEVELOPER INCLUDES GO HERE
#include <cstdio>

/*================================================================================*/
void XT_Assert (char *file, int32 line) {
/*----------------------------------------------------------------------------------
	Abstract:
		A customized version of the ansi assert.  Creates and displays a Mac 
		alert using facilities in QXP.

	Parameters and modes:
		File name and source line number where the assertion failed.
		
	Returns:

	Change Log:
		06-Dec-97	New.
----------------------------------------------------------------------------------*/
	char s[256];

	/* Create strings to display */
	sprintf (s, "Assertion failed:  File %s, Line %ld", file, line);
	PMString pmstr (s);
	CAlert::ModalAlert
	(
		pmstr,								// Alert string
		kOKString, 							// OK button
		kNullString, 						// No second button
		kNullString, 						// No third button
		1,									// Set OK button to default
		CAlert::eInformationIcon			// Information icon.
	);
}
