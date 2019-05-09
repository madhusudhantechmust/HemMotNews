/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	IAbstractPrefsHandler.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	8/11/11 ca. 8:45 PM
	
	ab	Abstract:
	ab	Declarations for IAbstractPrefsHandler class.  This 
	ab	abstract base class connects the Preferences plugin 
	ab	to the main plugin.  The main plugin contains the  
	ab	menu items for all the SDS plugins.
	
	cl	Change Log:
	cl	8/11/11	New.
------------------------------------------------------------------------------*/
#ifndef __IAbstractPrefsHandler_h__
#define __IAbstractPrefsHandler_h__

// Interface includes:
#include "IPMUnknown.h"

//#include "PMString.h"
#include "NCPrefsID.h"

class IAbstractPrefsHandler : public IPMUnknown {
public:
	enum { kDefaultIID = IID_INCPREFSINTERFACE };
	
	virtual void	DoPreferences_() = 0;
};

#endif		// __IAbstractPrefsHandler_h__

// END OF FILE

