/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	IAbstractArtImporter.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	2/28/11 ca. 9:27 PM
	
	ab	Abstract:
	ab	This abstract base class connects the art file importer plugin to 
	ab	the main plugin.  This in turn contains the user interface for 
	ab	all the SDS plugins.
	
	cl	Change Log:
	cl	2/28/11	New.
------------------------------------------------------------------------------*/
#ifndef	__IAbstractArtFilesImporter_h__
#define	__IAbstractArtFilesImporter_h__

// Interface includes:
#include "IPMUnknown.h"
//#include "PMString.h"
#include "ImportArtID.h"

class IAbstractArtImporter : public IPMUnknown {
public:
	enum { kDefaultIID = IID_IIMPORTARTINTERFACE };
	
	virtual void	ImportArtFiles_() = 0;
};

#endif