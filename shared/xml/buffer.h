/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	buffer.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	1/11/04 ca. 9:01 PM
	
	ab	Abstract:
	ab	Declarations for buffer classes:
	ab		TTextInBuffer, TSourceBuffer, TTextOutBuffer, TListBuffer
	
	cl	Change Log:
	cl	11-Jan-11	Port to CS4.
	cl	29-Aug-05	Port to InDesign.
	cl	1/11/04	New.
------------------------------------------------------------------------------*/
#ifndef	XML_PROC_BUFFER_H
#define	XML_PROC_BUFFER_H

// #include "CAlert.h"

#include <cstring>
#include <iostream>
#include <fstream>
using namespace std;
#include "misc.h"
//#include "error.h"

extern char	eofChar;
extern int	inputPosition;
extern bool	listFlag;
extern int	level;

const int	maxInputBufferSize = 256;


class TTextInBuffer {						// Abstract base class.
public:
//	TTextInBuffer (const char* pInputFileFullPath_, TAbortCode ac);
	TTextInBuffer (const char* pInputFileFullPath_);
	// Someday we need another variant of the CTOR that allows a 
	// pointer to a buffer of XML in memory to be passed and processed.
//	TTextInBuffer (const FSSpec* pInputFileSpec, TAbortCode ac);

	virtual ~TTextInBuffer ()
	{
// THIS IS THE OLD MAC CLASSIC CODE
// #ifdef PLUGIN
// 		osErr_ = ::FSClose (prefsFileID_);
// #else
// 		osErr_ = xtPrefsClose (prefsFileID_, kDataFork);
// #endif
		// Close the input file and clean up memory:
		if (inputFile_.is_open () ) {
/*
CAlert::ModalAlert
(
	"Closing preferences input file.",
	kOKString, 					// OK button
	kNullString, 				// No second button
	kNullString, 				// No third button
	1,							// Set OK button to default
	CAlert::eInformationIcon	// Information icon.
);
*/
			inputFile_.close ();
		}
		delete [] pInputFileFullPath_;
	}
	
	void	OpenFile_();
	char	Char () const		{ return *pChar; }
	char	GetChar ();
	char	PutBackChar ();

protected:
// 	char*		pFileName_;					// Ptr to file name
// 	FSSpec		fsSpec_;					// Setup during CTOR
	char*			pInputFileFullPath_;
	std::fstream	inputFile_;
	char			text_[256];					// Input buffer
	char*			pChar;						// Points to current char in input
	int				osErr_;
// #ifndef PLUGIN
// 	PrefsFileID prefsFileID_;
// #else
// 	short		prefsFileID_;				// File reference number
// #endif
	int				logEOF_;
	
	virtual char	GetLine () = 0;

private:

};


class TSourceBuffer : public TTextInBuffer {
public:
	TSourceBuffer (const char* pSourceFileFullPath);
//	TSourceBuffer (const FSSpec* pInputFileSpec);
	
protected:

private:
	virtual char GetLine ();
};

#endif