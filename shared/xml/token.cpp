/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	token.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	1/11/04 ca. 7:21 PM
	
	ab	Abstract:
	ab	Implementation of token classes:
	ab		TToken, TWordToken, TNumberToken, TStringToken, 
	ab		TSpecialToken, TEOFToken, TErrorToken
	
	cl	Change Log:
	cl	29-Aug-05	Port to InDesign.
	cl	3/2/04	Added TEntityToken to for representing XML entities.
	cl	1/11/04	New.
------------------------------------------------------------------------------*/

// #ifndef PLUGIN								// Compiling for QuarkXPress
// 	#ifdef __MWERKS__							// Mac specific includes
// 		#define TARGET_API_MAC_OSX 1
// 		#include <MSL MacHeadersMach-O.h>
// 	#endif // ifdef __MWERKS__
// 	
// 	#include "XP_XTIncE6.H"						// Quark includes
// #endif

using namespace std;

#include "token.h"

// Tag token

//------------------------------------------------------------------------------
//	TTagToken::Get					Extract a word from the input.
//------------------------------------------------------------------------------
void TTagToken::Get (TTextInBuffer& buffer)
{
	char prevChar = buffer.PutBackChar ();
	char ch = buffer.GetChar ();				// 1st char after '<' or "</"
	char* ps = string;
	
	// Extract the word
	do
	{
		*ps++ = ch;
		ch = buffer.GetChar ();
	} while (charCodeMap[ch] == ccLetter || charCodeMap[ch] == ccDigit);
	
	*ps = '\0';
	code = prevChar == '<' ? tcStartMarkup : tcEndMarkup;
}


//------------------------------------------------------------------------------
//	TTagToken::SetAttribute
//------------------------------------------------------------------------------
void TTagToken::SetAttribute (TToken* ptok)
{
	attribToken.Set (ptok->String());	
}


//------------------------------------------------------------------------------
//	TTagToken::Print
//------------------------------------------------------------------------------
/*
void TTagToken::Print () const
{
	cout << "\t\t>> tag:\t\t" << string << "\t\t";
	code == tcStartMarkup ? cout << "begin markup" : cout << "end markup";
	cout << endl;
	if (attribToken.GetName() && attribToken.GetValue())
	{
		cout << "\t\t\t>> attribute:\t\t" << attribToken.GetName() << endl;
		cout << "\t\t\t>> value    :\t\t" << attribToken.GetValue() << endl;
	}
}
*/

//------------------------------------------------------------------------------
//	TAttributeToken::Set
//------------------------------------------------------------------------------
void TAttributeToken::Set (char * instr)
{
	if (!name)
	{
		name = new char [strlen (instr) + 1];
		strcpy (name, instr);	
	}
	else
	{
		if (!value)
		{
			value = new char [strlen (instr) + 1];
			strcpy (value, instr);	
		}
	}
}



// Word Tokens

//------------------------------------------------------------------------------
//	TWordToken::Get					Extract a word from the input.
//------------------------------------------------------------------------------
void TWordToken::Get (TTextInBuffer &buffer)
{
	char ch = buffer.Char ();
	char* ps = string;
	
	// Extract the word
	do
	{
		*ps++ = ch;
		ch = buffer.GetChar ();
	} while (charCodeMap[ch] == ccLetter || charCodeMap[ch] == ccDigit);
	
	*ps = '\0';
//	strlwr (string);
	code = tcWord;
	type = tyString;
}


//------------------------------------------------------------------------------
//	TWordToken::Print
//------------------------------------------------------------------------------
/*
void TWordToken::Print () const
{
	cout << ">> word:  " << string << endl;
}
*/

// Number tokens

//------------------------------------------------------------------------------
//	TNumberToken::Get
//------------------------------------------------------------------------------
void TNumberToken::Get (TTextInBuffer &buffer)
{
	const int maxDigitCount = 4;
	
	char ch = buffer.Char ();
	char* ps = string;
	int digitCount = 0;
	bool countErrorFlag = false;
	
	// Accumulate value as long as count of digits doesn't exceed maxDigitCount.
	value.integer = 0;
	do
	{
		*ps++ = ch;
		
		// Shift left and add.
		if (++digitCount <= maxDigitCount)
		{
			value.integer = 10 * value.integer + (ch - '0');
		}
		else
		{
			countErrorFlag = true;
		}

		ch = buffer.GetChar ();
	} while (charCodeMap[ch] == ccDigit);

	*ps = '\0';
	code = countErrorFlag ? tcError : tcNumber;
	type = tyInteger;
}


//------------------------------------------------------------------------------
//	TNumberToken::Print
//------------------------------------------------------------------------------
/*
void TNumberToken::Print () const
{
	cout << ">> number:  " << value.integer << endl;
}
*/


// Special tokens

//------------------------------------------------------------------------------
//	TSpecialToken::Get
//
//	Handle start/end of tags and entities and assignment in attributes.
//------------------------------------------------------------------------------
void TSpecialToken::Get (TTextInBuffer &buffer)
{
	char ch = buffer.Char ();
	char* ps = string;

	*ps++ = ch;
	
	switch (ch) {
	case '<':							// 3 legal possibilities: < or </ or <?
		ch = buffer.GetChar ();
		if (ch == '/')
		{
			*ps++ = ch;
			code = tcStartCloseTag;
			buffer.GetChar ();
		}
		else if (ch == '?')
		{
			*ps++ = ch;
			code = tcStartProcInst;
			buffer.GetChar ();
		}
		else
		{
			code = tcStartOpenTag;
		}
		break;

	case '>':
		ch = buffer.GetChar ();
		code = tcEndTag;
		break;
	
	case '=':
		ch = buffer.GetChar ();
		code = tcEquals;
		break;
		
	case '&':							// Parameter or general entity
		ch = buffer.GetChar ();
		code = tcStartEntity;
		break;
		
	default:
		code = (ch == '.' ? tcPeriod : tcError);	
		ch = buffer.GetChar ();
	}
/*	
	if (ch == '\0')
	{
		buffer.GetChar ();
	}
*/	
	*ps = '\0';
}


//------------------------------------------------------------------------------
//	TSpecialToken::Print
//------------------------------------------------------------------------------
/*
void TSpecialToken::Print () const
{
	cout << ">> special:  " << string << endl;
}
*/

//--------------------------------------------------------------
//  Get     Get a string token from the source.
//
//      pBuffer : ptr to text input buffer
//--------------------------------------------------------------

void TStringToken::Get (TTextInBuffer &buffer)
{
    char  ch;           // current character
    char *ps = string;  // ptr to char in string

    *ps++ = '\'';		// opening quote

    //--Get the string.
    ch = buffer.GetChar();  // first char after opening quote
    while (ch != eofChar)
    {
		//--Append current char to string, then get the next char.
		*ps++ = ch;
		ch = buffer.GetChar();

		// Check for end of string.		
		if (ch == '\'')
			break;
    }

// TBD:    if (ch == eofChar) Error (errUnexpectedEndOfFile);

    *ps++ = '\'';  // closing quote
    *ps   = '\0';
	buffer.GetChar ();
	code = tcString;
	type = tyString;
}

//--------------------------------------------------------------
//  Print       Print the token to the list file.
//--------------------------------------------------------------
/*
void TStringToken::Print (void) const
{
//    sprintf(list.text, "\t%-18s %-s", ">> string:", string);
//    list.PutLine();
	cout << ">> string:  " << string << endl;
}
*/

// Character Data token

//------------------------------------------------------------------------------
// TCharDataToken::Get		Get XML parsed character data.
//------------------------------------------------------------------------------
void TCharDataToken::Get (TTextInBuffer &buffer)
{
    char  ch;					// current character
    char *ps = string;			// ptr to char in string
    *ps = '\0';
    
    ch = buffer.Char();			// first char after '>'
    bool endOfCDATA = false;
    while (ch != eofChar && !endOfCDATA)
    {
		// Check for end of char data.
		switch (ch) {
			case '\n':
			case '\t':
			case '\0':
				ch = buffer.GetChar();
				break;

			case '&':
			{
				ch = buffer.GetChar();				// Get past '&'
				TToken* pTok = new TEntityToken;
				pTok->Get (buffer);
				int intch = pTok->Value().integer;
				*ps++ = char (intch);
				delete pTok;
				ch = buffer.Char();					// Update current char to 
													// next char after ';'
			}
			break;

			case '<':
				endOfCDATA = true;
				break;
			
			default:
				//--Append current char to data, then get the next char.
				*ps++ = ch;
				ch = buffer.GetChar();
		}
    }

// TBD:    if (ch == eofChar) Error (errUnexpectedEndOfFile);

    *ps   = '\0';
//	buffer.GetChar ();
	code = tcCharData;
	type = tyCharacter;
}

//--------------------------------------------------------------
//  TCharDataToken::Print       Print XML data to the list file.
//--------------------------------------------------------------
/*
void TCharDataToken::Print (void) const
{
    sprintf(list.text, "\t%-18s %-s", ">> #CDATA:", string);
    list.PutLine();
	cout << ">> data:  " << string << endl;
}
*/

// Entity token (general or parameter or both?)

//------------------------------------------------------------------------------
//	TEntityToken::Get
//------------------------------------------------------------------------------
void TEntityToken::Get (TTextInBuffer &buffer)
{
    char *ps = string;			// ptr to char in string
    *ps = '\0';
    
    char ch = buffer.Char();			// first char after '&'
    bool numericVal = false;
    
    while (ch != eofChar && ch != ';')		// Look for end of token or eof
    {
    	if (ch == '#' && ps == string)		// Look for '#' on 1st char after '&'
    	{
    		numericVal = true;
			value.integer = 0;				// Init our accumulator
    	}
    	else
    	{
			// Append current char to data, then get the next char.
    		*ps++ = ch;
    		
    		if (numericVal)
    		{
	    		if (ch >= '0' && ch <= '9')
	    		{
	    			value.integer = 10 * value.integer + (ch - '0');
	    		}
	    		else
	    		{
	    			code = tcError;
	    		}
    		}
		}

		ch = buffer.GetChar();
    }

    *ps   = '\0';
    
    if (ch != eofChar)
    {
   		buffer.GetChar ();			// Throw away ';' at end of token
   	}

	code = tcEntity;
	type = tyInteger;
}


void TEntityToken::Print () const
{
/*
	cout << ">> entity:  ";
	if (value.integer)
	{
		cout << value.integer << "  (integer)";
	}
	else
	{
		cout << string << "(string)";
	}
	cout << endl;
*/
}


// Error token

//------------------------------------------------------------------------------
//	TErrorToken::Get		Extract an invalid character from the source.
//------------------------------------------------------------------------------
void TErrorToken::Get (TTextInBuffer &buffer)
{
	string[0] = buffer.Char ();
	string[1] = '\0';
	buffer.GetChar ();
}

