/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	ImportAdGeometry.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	10/14/10 ca. 10:01 PM
	
	ab	Abstract:
	ab	
	ab	
	
	cl	Change Log:
	cl	10/14/10	New.
----------------------------------------------------------------------------------*/
#include "VCPlugInHeaders.h"
#include "IAbstractGeometryImporter.h"
#include "ImportGeomID.h"
#include "global_data.h"
#include "CL_Log_File.h"
#include "CAlert.h"
#include "newscad.h"
#include "HM_Geometry_Importer.h"
#include <string>
#include <iostream>
#include <fstream>

using namespace std;

class CL_GeometryImportWorkhorse : public CPMUnknown<IAbstractGeometryImporter> {
public:
	CL_GeometryImportWorkhorse (IPMUnknown* boss) 
		: CPMUnknown<IAbstractGeometryImporter>(boss)
	{}
	virtual void	ImportAdGeometry_(PMString& inStr);
};

// This is what enables our code to be called by InDesign:
CREATE_PMINTERFACE (CL_GeometryImportWorkhorse, kImportGeomImpl)


//------------------------------------------------------------------------------
//	ImportAdGeometry_() runs the geometry import.
//	This function is called when the user chooses NewsCAD->Import Ad Geometry
//	from the menu bar.
//------------------------------------------------------------------------------
void CL_GeometryImportWorkhorse::ImportAdGeometry_(PMString& inStr)
{
#if 0
	TRACE("CL_GeometryImportWorkhorse::ImportAdGeometry_(PMString& inStr)\n");
	CAlert::ModalAlert
	(
		inStr,
		kOKString, 					// OK button
		kNullString, 				// No second button
		kNullString, 				// No third button
		1,							// Set OK button to default
		CAlert::eInformationIcon	// Information icon.
	);
#endif

	OPEN_LOG_FILE;
	LOG_BEGIN_FUNCTION;
#if CLIENT == CAPE_GAZETTE || CLIENT == REMINDER
	CL_Geometry_Importer *pImporter = new CL_Geometry_Importer;
#elif CLIENT == THRIFTY_NICKEL
	TN_Geometry_Importer *pImporter = new TN_Geometry_Importer;
#elif CLIENT == HEMMINGS
	HM_Geometry_Importer *pImporter = new HM_Geometry_Importer;
#elif CLIENT == MOUNTAIN_ENTERPRISE
	ME_Geometry_Importer *pImporter = new ME_Geometry_Importer;
#endif

	pImporter->Run_();
	delete pImporter;

	LOG_END_FUNCTION;

	CLOSE_LOG_FILE;
}