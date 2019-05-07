/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	global_data.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	8/2/05 ca. 5:12 PM
	
	ab	Abstract:
	ab	Declarations of global data.
	ab	
	
	cl	Change Log:
	cl	27-Aug-05	New.
------------------------------------------------------------------------------*/

using namespace std;

#include "CL_Log_File.h"
#include "CL_NewsCAD_Prefs.h"

CL_Log_File*			gpLogFile = NULL;
CL_NewsCAD_Prefs*		gpPrefsObj = NULL;
bool				gGeometryImportRunning = false;

#if 0
	#if CLIENT == HEMMINGS
		int32			xtCreateAdWapCBcode;		// QUARK ONLY!
	#endif
#endif