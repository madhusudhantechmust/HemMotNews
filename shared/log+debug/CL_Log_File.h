//------------------------------------------------------------------------------
//	Source name:
//		CL_Log_File.h
//
//	Abstract:
//		Declares log file class.
//
//	History:
//		27-Feb-10	Began port to InDesign CS4.  Was formerly hoping to use this 
//					X-platform; those hopes are dashed now.
//		02-Feb-05	Added the check for target name (Metrowerks only) to 
//					use in configuring the log file name.
//		01-Sep-02	New.
//
//------------------------------------------------------------------------------
#ifndef CL_Log_File_H
#define CL_Log_File_H

#include <cstring>
#include <iostream>
#include <fstream>
#include "newscad.h"
#include "XT_Assert.h"
#include "CL_NewsCadRect_ID.h"		// #includes "PMRect.h"


/* --------------------------- Macros for Logging --------------------------- */

// Name for log file.
#ifdef LOG_FILE_NAME
#undef LOG_FILE_NAME
#endif
#define LOG_FILE_NAME	"newscad_log.xml"

// Macro for logging information in debug file.

#if VERSION==IN_HOUSE

	// CTOR inits the instance and opens the actual disk file.
	#define OPEN_LOG_FILE	if (!gpLogFile)						\
							{									\
								gpLogFile = new CL_Log_File;	\
								ASSERT (gpLogFile);				\
							}

	// This writes a C string to the log file.
	#define	LOG(_x)			if (gpLogFile) gpLogFile->Log_(_x)
	
	// This writes a C string and some helpful info to the log file if error occurs.
	#define	LOG_ERROR(_x)	if (gpLogFile && (_x)) 				\
									gpLogFile->LogError_(_x, __FILE__, __LINE__)
	
	// Log information about a menu command.
	// InDesign-specific
	#define LOG_MENU_CMD(_str, _num)				\
									if (gpLogFile) 		\
										gpLogFile->LogMenuCommand_(_str, _num, 0)

	#define LOG_START_FUNCTION(_x)	{											\
										if (gpLogFile)							\
										{										\
											char S_[128];						\
											::strcpy (S_, "<function name=\"");	\
											::strcat (S_, _x);					\
											::strcat (S_, "\">");				\
											gpLogFile->Log_(S_);				\
											gpLogFile->IncrementTabLevel_();	\
										}										\
									}
	
	#define LOG_BEGIN_FUNCTION	{											\
									if (gpLogFile)							\
									{										\
										char S_[128];						\
										::strcpy (S_, "<function name=\"");	\
										::strcat (S_, __PRETTY_FUNCTION__);	\
										::strcat (S_, "\">");				\
										gpLogFile->Log_(S_);				\
										gpLogFile->IncrementTabLevel_();	\
									}										\
								}

	#define	LOG_END_FUNCTION	if (gpLogFile)								\
								{											\
									gpLogFile->DecrementTabLevel_();		\
									gpLogFile->Log_("</function>");			\
								}

	#define CLOSE_LOG_FILE			delete gpLogFile;	\
									gpLogFile = NULL;

	#define LOG_AD_VECTORS_STATUS	LogAdVectorStatus_()
	
	#define	DECLARE_logstr			char logstr[256]

	#define	LOG_TAG_AND_VALUE(_T,_V)	if (gpLogFile)							\
										{										\
											gpLogFile->LogTagAndValue_(_T,_V);	\
										}
#else

	// In production code, these debugging macros don't get compiled.
	#define QUARKXPRESS_APP_DIR
	#define XTENSION_RESOURCE_DIR
	#define LOG_FILE_DIR
	#define OPEN_LOG_FILE
	#define	LOG(_x)
	#define	LOG_ERROR(_x)
	#define LOG_MENU_CMD(_str, _num, _ptr)
	#define LOG_START_FUNCTION(_x)
	#define LOG_BEGIN_FUNCTION
	#define LOG_END_FUNCTION
	#define CLOSE_LOG_FILE
	#define LOG_AD_VECTORS_STATUS
	#define	DECLARE_logstr
	#define	LOG_TAG_AND_VALUE(_T,_V)

#endif


/* ----------------------------- CL_Log_File class ------------------------------ */

class CL_Log_File {
public:
			CL_Log_File ();
	virtual ~CL_Log_File ();

	virtual void Log_(const char *str);

	// InDesign-specific declaration
	virtual void LogMenuCommand_(
									const char* itemStr, int cmdNum,
									long pServiceRoutine);

	void	IncrementTabLevel_()		{ ++tabLevel_; }
	void	DecrementTabLevel_()		{ --tabLevel_; }

	void	LogError_(
						OSErr inErrorCode, 
						char* inSourceFileName, 
						unsigned long inSourceFileLineNum);

	void	LogTagAndValue_(const char* inTag, const int inVal);
	virtual void LogTagAndValue_(const char* inTag, const char* inVal);
	void	LogPMRect_(const char* msg, const PMRect & bBox);
	
protected:
	virtual void InsertTabChars_();

private:
	virtual void OpenLogFile_();		// Called only by CTOR
	virtual void CloseLogFile_();		// Called only by DTOR

	short			tabLevel_;
	std::fstream	logfile_;
};

#endif

// END OF FILE
