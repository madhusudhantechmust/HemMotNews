/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	scanner.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	1/11/04 ca. 6:26 PM
	
	ab	Abstract:
	ab	Declarations for TScanner and TTextScanner classes.
	
	cl	Change Log:
	cl	13-Jan-11	Port to CS4.
	cl	29-Aug-05	Port to InDesign.
	cl	1/11/04	New.
------------------------------------------------------------------------------*/
#ifndef	_XML_SCANNER_H_
#define	_XML_SCANNER_H_

#include "misc.h"
#include "buffer.h"
#include "token.h"

class TScanner {									// Abstract scanner class
public:
	virtual	~TScanner () {};						// Destroy
	virtual TToken *Get () = 0;

protected:
	// Tokens extracted and returned by the scanner.
	TTagToken		tagToken;
	TWordToken		wordToken;
	TNumberToken	numberToken;
	TStringToken	stringToken;
	TCharDataToken	charDataToken;
	TSpecialToken	specialToken;
	TEntityToken	entityToken;
	TEOFToken		eofToken;
	TErrorToken		errorToken;

private:
};


class TTextScanner : public TScanner {
public:
	TTextScanner (TTextInBuffer* pBuffer);
	virtual ~TTextScanner () { delete pTextInBuffer; }
	virtual TToken* Get ();
	virtual TToken* Get (ParserState inParserState);
	virtual TToken* GetCharacterData ();
	
protected:

private:
	void SkipWhiteSpace ();

	TTextInBuffer* const	pTextInBuffer;			// Pointer to input to scan
	TTokenCode				prevTokenCode;
};	
	
#endif

// END OF FILE
