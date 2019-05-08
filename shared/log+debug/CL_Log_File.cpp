/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	CL_Log_File.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	01-Sep-2002
	
	ab	Abstract:
	ab	Handles the log file.
	
	cl	Change Log:
	cl	27-Oct-10	Began port to InDesign CS4 for Intel Mac.
	cl	23-Jan-04	Generalized some of the code for use by Techbooks (XMath).
	cl	01-Sep-02	Adapted from the Amway P5 XT's log file facility.
------------------------------------------------------------------------------*/
/*
#ifdef __MWERKS__
	#define TARGET_API_MAC_OSX 1
	#include <MSL MacHeadersMach-O.h>
#endif // ifdef __MWERKS__
*/
#include "CAlert.h"
#include "CL_Log_File.h"
#include <cstring>
#include <iostream>
#include <fstream>

using namespace std;

//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and D E S T R U C T I O N
//----------------------------------------------------------------------------------
CL_Log_File::CL_Log_File ()
	:	tabLevel_(0)
{
	OpenLogFile_();
	
	// Write XML declaration and root element
	Log_("<?xml version=\"1.0\"?>");
	Log_("<?xml-stylesheet type=\"text/xsl\" href=\"newscad_log.xsl\"?>");
	Log_("<!DOCTYPE newscad_log SYSTEM \"xmath_log.dtd\">");
	Log_("<!-- If you are reading this, we have managed to get the log file open! -->");
	Log_("\r<newscad_log>");
}


CL_Log_File::~CL_Log_File ()
{
	// Write root element closing tag
	Log_("\r</newscad_log>");
	
	CloseLogFile_();
}


/* ------------------------------ Pretty Print ------------------------------ */

void CL_Log_File::InsertTabChars_()
{
	if (tabLevel_)
	{
		tabLevel_ = tabLevel_ > 20 ? 20 : tabLevel_;
		char tabs[24];
		int i;
		for (i = 0; i < tabLevel_; i++)
		{
			tabs[i] = '\t';
		}
		tabs[i] ='\0';
		Log_(tabs);
	}
}


/*================================================================================*/
void CL_Log_File::Log_(const char *str) {
/*----------------------------------------------------------------------------------
	Abstract:
		Write a string to the log file.

	Parameters and modes:
		str		Ptr to string to write.			IN
		
	Returns:
		Nothing.
		
	Change Log:
		27-Oct-10	Trying standard C++ file I/O.
		23-Jan-04	Documented block that flushes the output.
		25-Oct-97	New.
----------------------------------------------------------------------------------*/
#if VERSION==IN_HOUSE
	if (logfile_.is_open ()) {
		logfile_ << str << endl;
		logfile_.flush ();
	}
#endif
}


/*================================================================================*/
void CL_Log_File::OpenLogFile_() {
/*----------------------------------------------------------------------------------
	Abstract:
		Opens and prepares the log file for writing.

	Parameters and modes:
		
	Returns:
		Nothing.
		
	Change Log:
		27-Oct-10	Trying standard C++ file I/O.
		25-Oct-97	New.
----------------------------------------------------------------------------------*/
#if VERSION==IN_HOUSE
	FSRef		ref;
	OSStatus	err;
	
	// Get a MacOS X file system reference to the current user's Documents folder.
	err = FSFindFolder ( kUserDomain, kDocumentsFolderType, 
							kDontCreateFolder, &ref );
	if (!err)
	{
		UInt8 path[256];
		
		// Convert the file system ref to a POSIX path.
		err = FSRefMakePath ( &ref, (UInt8 *)path, 256 );
		if (!err)
		{
			//printf ("SUCCESS!  POSIX path = %s\n", (char*) path);
			// Add the log filename to the path.
			char* fullPath = new char [512];
			strcpy (fullPath, (const char *)path);
			strcat (fullPath, "/newscad_log.xml");
			logfile_.open (fullPath, fstream::out); 
			delete [ ] fullPath;
			if ( ! logfile_.is_open () )
			{
				CAlert::ModalAlert
				(
					"NewsCAD: Unable to open log file.",// Alert string
					kOKString, 							// OK button
					kNullString, 						// No second button
					kNullString, 						// No third button
					1,									// Set OK button to default
					CAlert::eInformationIcon			// Information icon.
				);
			}
		}
		else
		{
			char errStr[256];
			sprintf (errStr, "FSRefMakePath FAILED err = %d\n", err);
			CAlert::ModalAlert
			(
				errStr,								// Alert string
				kOKString, 							// OK button
				kNullString, 						// No second button
				kNullString, 						// No third button
				1,									// Set OK button to default
				CAlert::eInformationIcon			// Information icon.
			);
		}
	}
	else
	{
		char errStr[256];
		sprintf (errStr, "FSFindFolder FAILED err = %d\n", err);
		CAlert::ModalAlert
		(
			errStr,								// Alert string
			kOKString, 							// OK button
			kNullString, 						// No second button
			kNullString, 						// No third button
			1,									// Set OK button to default
			CAlert::eInformationIcon			// Information icon.
		);
	}
/*
	char logFileName[256];
	::strcpy (logFileName, LOG_FILE_NAME);
	logfile_.open (logFileName, fstream::out); 
	if ( ! logfile_.is_open () ) {
		CAlert::ModalAlert
		(
			"NewsCAD: Unable to open log file.",// Alert string
			kOKString, 							// OK button
			kNullString, 						// No second button
			kNullString, 						// No third button
			1,									// Set OK button to default
			CAlert::eInformationIcon			// Information icon.
		);
*/
#endif
}

/*================================================================================*/
void CL_Log_File::LogMenuCommand_(
							const char* itemStr, 
							int quarkCmdNum, 
							long pServiceRoutine) {
/*----------------------------------------------------------------------------------
	Abstract:
		Logs information about a QuarkXPress menu command.
		
	Parameters and modes:
		itemStr				Text of the menu item.
		quarkCmdNum			Command number assigned by QuarkXPress at 
							"add menu items" time.
		pServiceRoutine		Pointer to a function that can handle the command.

	Returns:
		Nothing.

	Change Log:
		29-Jan-04	New.
----------------------------------------------------------------------------------*/
#if VERSION==IN_HOUSE
	char str[256];
#if MACOS
	char s[256];
	P2CSTRCPY ((uchar*)s, itemStr);
	::sprintf (str, "<menucmd text=\"%s\" number=\"%d\" routine=\"%p\" />",
									s, quarkCmdNum, pServiceRoutine);
#else
	::sprintf (str, "<menucmd text=\"%s\" number=\"%d\" routine=\"%p\" />",
									itemStr, quarkCmdNum, pServiceRoutine);
#endif
	Log_(str);
#endif
}


//----------------------------------------------------------------------------------
//	LogPMRect_
//----------------------------------------------------------------------------------
void CL_Log_File::LogPMRect_(const char* msg, const PMRect & bBox)
{
	char str[256];
	if (msg)
	{
		::sprintf (str, "<!-- %s -->", msg);
		Log_(str);
	}
	
	::sprintf (str, "<p>PMRect: Left = %f Top = %f Right = %f Bottom = %f</p>",
		::ToFloat (bBox.Left()),
		::ToFloat (bBox.Top()),
		::ToFloat (bBox.Right()),
		::ToFloat (bBox.Bottom()));
	Log_(str);
}


/*================================================================================*/
void CL_Log_File::CloseLogFile_() {
/*----------------------------------------------------------------------------------
	Abstract:
		Safely closes the log file.

	Parameters and modes:
		
	Returns:
		Nothing.
		
	Change Log:
		27-Oct-10	Trying standard C++ file I/O.
		25-Oct-97	New.
----------------------------------------------------------------------------------*/
#if VERSION==IN_HOUSE
	if (logfile_.is_open ()) {
		logfile_.close ();
	}
#endif
}


//------------------------------------------------------------------------------
//	LogTagAndValue_
//------------------------------------------------------------------------------
void CL_Log_File::LogTagAndValue_(const char* inTag, const char* inVal)
{
	char str[256];
	::sprintf (str, "<%s>%s</%s>", inTag, inVal, inTag);
	Log_(str);
}


//------------------------------------------------------------------------------
//	LogTagAndValue_
//------------------------------------------------------------------------------
void CL_Log_File::LogTagAndValue_(const char* inTag, const int inVal)
{
	char str[256];
	::sprintf (str, "<%s>%d</%s>", inTag, inVal, inTag);
	Log_(str);
}



/*** END OF FILE ***/
