/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	scanner.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	1/11/04 ca. 6:38 PM
	
	ab	Abstract:
	ab	Implementation of TTextScanner.
	
	cl	Change Log:
	cl	13-Jan-10	Port to CS4.
	cl	29-Aug-05	Port to InDesign.
	cl	1/11/04	New.
------------------------------------------------------------------------------*/

#include "scanner.h"
using namespace std;

TCharCode	charCodeMap[256];

//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and  D E S T R U C T I O N
//----------------------------------------------------------------------------------

TTextScanner::TTextScanner ( TTextInBuffer* pBuffer)					// Create
	: pTextInBuffer (pBuffer), prevTokenCode (tcDummy)
{
	int i;
	
	// Init char code map.

	for (i = 0; i <= 255; i++)				// Everything's an error.
		charCodeMap[i] = ccError;

	charCodeMap['\0'] = ccWhiteSpace;		// NULL
	charCodeMap['\t'] = ccWhiteSpace;		// 9
	charCodeMap[10]   = ccWhiteSpace;		// 10
	charCodeMap['\r'] = ccWhiteSpace;		// 13
	charCodeMap['\n'] = ccWhiteSpace;		// 13+10
	charCodeMap[' ']  = ccWhiteSpace;		// 32	

	for (i = '!'; i <= '.'; i++)			// 33 - 46
		charCodeMap[i] = ccLetter;
	
//	charCodeMap['&'] = ccSpecial;			// 38
	charCodeMap['/'] = ccSpecial;			// 47
	
	for (i = '0'; i <= ';'; i++)			// 48 - 59
		charCodeMap[i] = ccLetter;			// '0'-'9', ':', and ';'
		
	charCodeMap['<'] = ccSpecial;			// 60
	charCodeMap['='] = ccSpecial;			// 61
	charCodeMap['>'] = ccSpecial;			// 62
	charCodeMap['?'] = ccQuestionMark;		// 63
//	charCodeMap['@'] = ccSpecial;			// 64

	for (i = '@'; i <= 'Z'; i++)			// 64 - 90
		charCodeMap[i] = ccLetter;
											// 91 - 93 are error [,\,]
	charCodeMap['^'] = ccLetter;			// 94
	charCodeMap['_'] = ccLetter;			// 95
											// 96 '`' is error

	for (i = 'a'; i <= '{'; i++)			// 97 - 123 are a thru z and {
		charCodeMap[i] = ccLetter;			// '{' is fraction bar on MathPi 1
											// 124 ("pipe") is error |
	charCodeMap['}'] = ccLetter;			// 125 is legal (fraction bar char)
											// 126 is an error ~

	charCodeMap[eofChar] = ccEndOfFile;		// 127 EOF
}


//------------------------------------------------------------------------------
//	SkipWhiteSpcae
//
//	<TBD> Watch out for EOF in the middle of a comment.
//------------------------------------------------------------------------------
void
TTextScanner::SkipWhiteSpace ()
{
	char ch = pTextInBuffer->Char ();

	while (charCodeMap[ch] == ccWhiteSpace)
	{
		ch = pTextInBuffer->GetChar ();
	}

	if (ch == '<')
	{
		char s[4];
		s[0] = pTextInBuffer->GetChar ();
		s[1] = pTextInBuffer->GetChar ();
		s[2] = pTextInBuffer->GetChar ();
		s[3] = '\0';
		if ( strcmp (s, "!--") == 0 )
		{
			// Found start of comment.
			do {
				ch = pTextInBuffer->GetChar ();
				if (ch == '-')
				{
					s[0] = pTextInBuffer->GetChar ();
					s[1] = pTextInBuffer->GetChar ();
					s[2] = '\0';
					if ( strcmp (s, "->") == 0 )
					{
						// Found end of comment.
						ch = pTextInBuffer->GetChar ();
						if (ch == '\0' || charCodeMap[ch] == ccWhiteSpace)
						{
							SkipWhiteSpace ();
						}
						break;
					}
					else
					{
						// Restore input stream.
						pTextInBuffer->PutBackChar ();
						pTextInBuffer->PutBackChar ();
						ch = pTextInBuffer->Char ();
					}
				}
			} while (ch != eofChar);
		}
		else
		{
			// Restore input stream.
			pTextInBuffer->PutBackChar ();
			pTextInBuffer->PutBackChar ();
			pTextInBuffer->PutBackChar ();
			ch = pTextInBuffer->Char ();
		}
	}

/*
	while (charCodeMap[ch] == ccWhiteSpace)
	{
		ch = pTextInBuffer->GetChar ();
	}
*/
}


//------------------------------------------------------------------------------
//	Get
//
//	Extract the next token from the input based on the current character 
//	and return a pointer to that extracted token.
//------------------------------------------------------------------------------
TToken*
TTextScanner::Get ()
{
	SkipWhiteSpace ();
	TToken*	pToken;
	
	// Determine token class based on current character.
	switch (charCodeMap[pTextInBuffer->Char ()]) {
//		case ccTagEnd:		pToken = &charDataToken;	break;
		case ccQuestionMark:
		case ccLetter:		pToken = &wordToken;		break;
		case ccDigit:		pToken = &numberToken;		break;
		case ccQuote:		pToken = &stringToken;		break;
		case ccSpecial:		pToken = &specialToken;		break;
		case ccEndOfFile:	pToken = &eofToken;			break;
		default:			pToken = &errorToken;		break;
	}
	
	// If upcoming token is a word token and the previous char was the 
	// end of tag delimiter, get character data rather than a word.
	if (pToken == &wordToken)
	{
		if (prevTokenCode == tcEndTag)
		{
			pToken = &charDataToken;
		}
		else if (prevTokenCode == tcStartEntity)
		{
			pToken = &entityToken;
		}
		else
		{
			// i don't know, but this is an error i think
		}
	}
	
	pToken->Get (*pTextInBuffer);
	
	prevTokenCode = pToken->Code ();
	
	return pToken;
}


TToken*
TTextScanner::Get (ParserState inParserState)
{
	TToken*	pToken;

	if (charCodeMap[pTextInBuffer->Char ()] != ccEndOfFile)
	{
		switch (inParserState) {
			case stInitial:
			case stOpenTag:
			case stAttribute:
			case stAttributeValue:
				pToken = Get ();
				break;
			
			case stCData:
			case stCloseTag:
				pToken = GetCharacterData ();
				break;
			
			case stFinished:
				break;
			
			default:
// #ifndef PLUGIN									// Compiling for QuarkXTension
// 	#if TARGET_API_MAC_CARBON					// An XTension on the Mac
// 				char* warning = new char[256];
// 				::sprintf (warning, 
// 					"Unrecognized token type:  file: %s  line: %d", 
// 					__FILE__, __LINE__);
// 				C2PSTR (warning);
// 				WarningMessage_( (const uchar*) warning );
// 				delete [] warning;
// 	#endif
// #elif PLUGIN									// Compiling for InDesign plugin
// 				char* warning = new char[256];
// 				::sprintf (warning, 
// 					"Unrecognized token type:  file: %s  line: %d", 
// 					__FILE__, __LINE__);
// 				LOG (warning);
// 				delete [] warning;
// #else											// Compiling for ANSI console app
/*				cout << "Unrecognized case:  file = " 
							<< __FILE__ << " line =  " << __LINE__;*/
//#endif
				break;
		}
	}
	else
	{
		pToken = &eofToken;
	}
	return pToken;
}


TToken*
TTextScanner::GetCharacterData ()
{
	TToken*	pToken = &charDataToken;
	pToken->Get (*pTextInBuffer);
	return pToken;
}


// END OF FILE
