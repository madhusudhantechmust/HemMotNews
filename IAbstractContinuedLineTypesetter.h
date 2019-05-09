/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	IAbstractContinuedLineTypesetter.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	4/23/11 ca. 11:25 PM
	
	ab	Abstract:
	ab	Declarations for IAbstractContinuedLineTypesetter class.  This 
	ab	abstract base class connects the continued line typesetting plugin 
	ab	to the main plugin.  The main plugin contains the user interface
	ab	for all the SDS plugins.
	
	cl	Change Log:
	cl	4/23/11	New.
------------------------------------------------------------------------------*/
#ifndef __IAbstractContinuedLineTypesetter_h__
#define __IAbstractContinuedLineTypesetter_h__

// Interface includes:
#include "IPMUnknown.h"

//#include "PMString.h"
#include "TypesetContinuedLinesID.h"

class IAbstractContinuedLineTypesetter : public IPMUnknown {
public:
	enum { kDefaultIID = IID_ITYPESETCONTINUEDLINESINTERFACE };
	
	virtual void	TypesetContinuedLines_() = 0;
};

#endif		// __IAbstractContinuedLineTypesetter_h__

// END OF FILE

