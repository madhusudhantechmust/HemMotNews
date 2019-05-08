/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	XT_Assert.h

	au	Author:
	au	Ross L. Edgar

	da	Date
	da	Friday, January 23, 2004.

	ab	Abstract:
	ab	

	cl	Change Log:
	cl	25-Aug-05	Work around Adobe's #define for ASSERT.
------------------------------------------------------------------------------*/
#ifndef XT_Assert_H
#define XT_Assert_H

// I don't want to use Adobe's version of ASSERT.
#ifdef ASSERT
#undef ASSERT
#endif

/* ----------------------------- Assert Macro ------------------------------- */

// Macro for logging information in debug file.
#if VERSION==IN_HOUSE
	// If _x is false, this will put up an alert with the file name and line 
	// number where the assertion failed and quit QuarkXPress when user clicks OK.
	#define	ASSERT(_x)		if (!(_x))	\
								XT_Assert(__FILE__, __LINE__)
#else
	// In production code, this debugging macro doesn't get compiled.
	#define	ASSERT(_x)
#endif

/* ------------------------------- Prototypes ------------------------------- */

void XT_Assert (char *file, int32 line);

#endif

// END OF FILE
