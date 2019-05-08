/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	buffer.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	1/11/04 ca. 9:25 PM
	
	ab	Abstract:
	ab	Implementation of buffer classes.
	ab		TTextInBuffer, TSourceBuffer, TTextOutBuffer, TListBuffer
	
	cl	Change Log:
	cl	11-Jan-11	Port to CS4.
	cl	29-Aug-05	Port to InDesign.
	cl	1/11/04	New.
------------------------------------------------------------------------------*/
// I'd like to divorce the XML from the rest of the program 
// & make it completely standalone.
//	#include "CL_Log_File.h"
//	#include "global_data.h"
#include <iostream>			// TESTING
#include <cstring>
//#include "common.h"
#include "buffer.h"
using namespace std;

char	eofChar = 0x7f;				// Special end-of-file character
int		inputPosition;				// Virtual position of the current char in 
									// the input buffer, with tabs expanded

//------------------------------------------------------------------------------
//	TTextInBuffer:TTextInBuffer									CTOR
//------------------------------------------------------------------------------
//TTextInBuffer::TTextInBuffer (const char* pInputFileFullPath, TAbortCode ac)
TTextInBuffer::TTextInBuffer (const char* pInputFileFullPath)
	: pInputFileFullPath_( new char[strlen (pInputFileFullPath) + 1])
{
	// Use a copy of the input filename.
	::strcpy (pInputFileFullPath_, pInputFileFullPath);
}


// ------------------------------------------------------------------------------
// 	TTextInBuffer:TTextInBuffer									CTOR
// ------------------------------------------------------------------------------
// TTextInBuffer::TTextInBuffer (const FSSpec* pInputFileSpec, TAbortCode ac)
// 	:	pFileName_(NULL)
// 	,	fsSpec_(*pInputFileSpec)
// {
// }


//------------------------------------------------------------------------------
//	TTextInBuffer::GetChar
//
//	Fetch and return the next character from the text buffer.  If at the end of 
//	the buffer, read the next source line.  If at the end of the file, return 
//	the end-of-file character.
//------------------------------------------------------------------------------
char TTextInBuffer::GetChar ()
{
	const int tabSize = 8;
	char chr;
	
	if (*pChar == eofChar)
	{
		return eofChar;
	}
	else if (*pChar == '\0')
	{
		chr = GetLine ();
	}
	else
	{
		++pChar;
		++inputPosition;
		chr = *pChar;
	}
	
	// If tab char, bump inputPosition to the next multiple of tabSize.
	if (chr == '\t')
	{
		inputPosition += tabSize - inputPosition % tabSize;
	}

	return chr;
}


//------------------------------------------------------------------------------
//	TTextInBuffer::PutBackChar
//------------------------------------------------------------------------------
char TTextInBuffer::PutBackChar ()
{
	--pChar;
	--inputPosition;
	return *pChar;
}


//------------------------------------------------------------------------------
//	TTextInBuffer::OpenFile_
//------------------------------------------------------------------------------
void TTextInBuffer::OpenFile_()
{
// OLD MAC CLASSIC CODE
// #ifndef PLUGIN
// 	uchar filnam[256];
// 	C2PSTRCPY ( (uchar *) filnam, (uchar *) pFileName_ );
// 
// 	// Try to open the file.
// 	osErr_ = xtPrefsOpen (filnam, kDataFork, fsRdWrPerm, &prefsFileID_);
// 	if (!osErr_)
// 	{
// 		osErr_ = xtPrefsGetEOF ( prefsFileID_, &logEOF_ );
// 	}
// #else
// 	osErr_ = ::FSpOpenDF (&fsSpec_, fsRdWrPerm, &prefsFileID_);
// 	LOG_ERROR (osErr_);
// 	if (!osErr_)
// 	{
// 		osErr_ = ::GetEOF ( prefsFileID_, &logEOF_ );
// 		LOG_ERROR (osErr_);
// 		if (!osErr_)
// 		{
// 			osErr_ = ::SetFPos ( prefsFileID_, fsFromStart, 0L );
// 			LOG_ERROR (osErr_);
// 		}
// 	}
// #endif
	inputFile_.open (pInputFileFullPath_, fstream::in);
}

#pragma mark -

//------------------------------------------------------------------------------
//	TSourceBuffer::TSourceBuffer
//------------------------------------------------------------------------------
TSourceBuffer::TSourceBuffer (const char* pSourceFileFullPath)
//	: TTextInBuffer ( pSourceFileFullPath, abortSourceFileOpenFailed)
	: TTextInBuffer ( pSourceFileFullPath)
{
	OpenFile_();
	GetLine ();
}


//------------------------------------------------------------------------------
//	TSourceBuffer::TSourceBuffer
//------------------------------------------------------------------------------
// TSourceBuffer::TSourceBuffer (const FSSpec* pInputFileSpec)
// 	: TTextInBuffer ( pInputFileSpec, abortSourceFileOpenFailed )
// {
// 	OpenFile_();
// 	GetLine ();
// }


//------------------------------------------------------------------------------
//	TSourceBuffer::GetLine
//
//	Read the next line from the input file, and print it to the list preceded 
//	by the line number and the current nesting level.
//
//	Returns:
//		First char of the source line, or the eof char if at end of file.
//------------------------------------------------------------------------------
char TSourceBuffer::GetLine ()
{
	if (inputFile_.is_open () )
	{
		if (!inputFile_.eof())
		{
			inputFile_.getline (text_, sizeof (text_) - 1);
//			++currentLineNumber;
//			cout << "TSourceBuffer::GetLine ()  " << currentLineNumber << ":  " << text_ << endl;
		}
		else
	 	{
	 		text_[0] = eofChar;
	 		text_[1] = '\0';
//			cout << "TSourceBuffer::GetLine ()  :  EOF." << endl;
	 	}
	}
	else
	{
//		cout << "TSourceBuffer::GetLine () ERROR INPUT FILE NOT OPEN." << endl;
		text_[0] = '\0';
	}
	
	pChar = text_;
 	inputPosition = 0;				// At start of line
	return *pChar;					// First char on line

// 	if (!osErr_)
// 	{
// 		int32 startFilePos;
// #ifdef PLUGIN
// 		osErr_ = ::GetFPos ( prefsFileID_, &startFilePos );			// InDesign
// #else
// 		osErr_ = xtPrefsGetFPos ( prefsFileID_, &startFilePos);		// QuarkXPress
// #endif
// 		int32 count = 255;									// Max bytes to read
// #ifdef PLUGIN
// 		osErr_ = ::FSRead ( prefsFileID_, &count, (void*) text_ );	// InDesign
// #else
// 		osErr_ = xtPrefsRead ( prefsFileID_, &count, (void*) text_);// QuarkXPress
// #endif
// 		if (count == 255)
// 		{
// 			char c = text_[count];
// 			while (c != '\r' && c != '\n')
// 			{
// 				--count;
// 				c = text_[count];
// 			}
// 			int32 newFilePos = startFilePos + count;
// #ifdef PLUGIN
// 			::SetFPos ( prefsFileID_, fsFromStart, newFilePos ); 
// #else
// 			xtPrefsSetFPos ( prefsFileID_, fsFromStart, newFilePos); 
// #endif
// 			text_[count] = '\0';								// End of line
// 		}
// 		else
// 		{
// 			text_[count] = eofChar;								// At EOF
// 			text_[count+1] = '\0';
// 		}
// 		pChar = text_;
// 	}
// 	else
// 	{
// 		text_[0] = eofChar;
// 		text_[1] = '\0';
// 	}
// /* DEBUGGING
// 	uchar ts[256];
// 	C2PSTRCPY (ts, (const uchar*) text_);
// 	DebugStr (ts);
// */
// 	inputPosition = 0;
// 
// 	return *pChar;
}



/*
char TSourceBuffer::GetLine ()
{
	if (!osErr_)
	{
		int32 count = 255;									// Max bytes to read
		osErr_ = xtPrefsRead ( prefsFileID_, &count, (void*) text_);
		text_[count] = '\0';
		if (count < 255)
		{
			text_[count] = eofChar;
			text_[count+1] = '\0';
		}
		pChar = text_;
	}
	else
	{
		text_[0] = eofChar;
		text_[1] = '\0';
	}

	uchar ts[256];
	C2PSTRCPY (ts, (const uchar*) text_);
	DebugStr (ts);

	inputPosition = 0;

	return *pChar;
}
*/