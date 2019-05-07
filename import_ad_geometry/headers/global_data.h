/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	global_data.h

	au	Author:
	au	Ross L. Edgar

	da	Date
	da	Today is 2/2/05. It is now 5:18 PM

	ab	Abstract:
	ab	Declarations of global variables.

	cl	Change Log:
	cl	02-Feb-05	New.
----------------------------------------------------------------------------------*/
#ifndef	_GLOBAL_DATA_H_
#define	_GLOBAL_DATA_H_

//class CL_Log_File;
//class CL_NewsCAD_Prefs;
#include "CL_Log_File.h"
#include "CL_NewsCAD_Prefs.h"

extern CL_Log_File*			gpLogFile;
extern CL_NewsCAD_Prefs*	gpPrefsObj;

extern bool				gGeometryImportRunning;

const char cStartDelim = '<';
const char cEndDelim = '>';

#endif