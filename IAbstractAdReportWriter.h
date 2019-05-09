/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	IAbstractAdReportWriter.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	5/25/11 ca. 10:00 PM
	
	ab	Abstract:
	ab	Declarations for IAbstractAdReportWriter class.  This 
	ab	abstract base class connects the Write Ad Report plugin 
	ab	to the main plugin.  The main plugin contains the user 
	ab	interface for all the SDS plugins.
	
	cl	Change Log:
	cl	5/25/11	New.
------------------------------------------------------------------------------*/
#ifndef __IAbstractAdReportWriter_h__
#define __IAbstractAdReportWriter_h__

// Interface includes:
#include "IPMUnknown.h"

//#include "PMString.h"
#include "WriteAdReportID.h"

class IAbstractAdReportWriter : public IPMUnknown {
public:
	enum { kDefaultIID = IID_IWRITEADREPORTINTERFACE };
	
	virtual void	WriteAdReport_() = 0;
};

#endif		// __IAbstractAdReportWriter_h__

// END OF FILE

