/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	parser.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	1/11/04 ca. 8:45 PM
	
	ab	Abstract:
	ab	Declarations for TParser.
	
	cl	Change Log:
	cl	29-Aug-05	Port to InDesign.
	cl	1/11/04	New.
------------------------------------------------------------------------------*/
#ifndef _XML_PARSER_H_
#define	_XML_PARSER_H_

#include "misc.h"
#include "buffer.h"
#include "token.h"
#include "scanner.h"
#include "CL_NewsCAD_Prefs.h"
typedef void (CL_NewsCAD_Prefs::*NewsCADPrefsMemFn)(
					const char* path, const char* elem, const TElementValue & v);
typedef void (*XMLCallback)(const char* path, const char* elem, const TElementValue & v);
class TParser {
public:
	//TParser (TTextInBuffer *pBuffer, CL_NewsCAD_Prefs* pInPrefs, NewsCADPrefsMemFn XMLProcFunc)
	TParser (TTextInBuffer *pBuffer, XMLCallback XMLProcFunc)
		: 	pScanner (new TTextScanner (pBuffer))
			, parserState (stInitial)
			, tagLevel_ (0)
			, pDataToken_(NULL)
//			, pPrefs_(pInPrefs)
			, XMLProcFunc_(XMLProcFunc)
	{
		for (int i = 0; i < 32; i++)		tags_[i] = NULL;
	}
	
	~TParser ()		{ delete pScanner; }
	
	void Parse ();

protected:
	void ProcessToken_();
	void ProcessTag_();
	void ProcessTagValue_();
	
private:
	TTextScanner* const		pScanner;		// Pointer to scanner
	TToken*					pToken;			// Pointer to current token
	TToken*					pDataToken_;	// Data enclosed inside tag
	TTokenCode				token;			// Code of current token
	ParserState				parserState;
	int						tagLevel_;		// Track nesting of tags
	char*					tags_[32];
//	CL_NewsCAD_Prefs*		pPrefs_;
//	NewsCADPrefsMemFn		XMLProcFunc_;
	XMLCallback				XMLProcFunc_;
		
	void GetToken ()
	{
		pToken = pScanner->Get ();
		token = pToken->Code ();
	}
};

#endif

// END OF FILE

