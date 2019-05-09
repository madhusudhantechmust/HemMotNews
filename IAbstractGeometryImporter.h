/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	IAbstractGeometryImporter.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	10/14/10 ca. 6:15 PM
	
	ab	Abstract:
	ab	This abstract base class connects the geometry importer plugin to 
	ab	the main plugin that contains the user interface for all the SDS 
	ab	plugins.
	
	cl	Change Log:
	cl	10/14/10	New.
------------------------------------------------------------------------------*/
#ifndef __IAbstractImportGeom_h__
#define __IAbstractImportGeom_h__

// Interface includes:
#include "IPMUnknown.h"
#include "PMString.h"
#include "./import_ad_geometry/headers/ImportGeomID.h"

class IAbstractGeometryImporter : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_IIMPORTGEOMINTERFACE };
	
	virtual void	ImportAdGeometry_(PMString& inStr) = 0;

};

#endif