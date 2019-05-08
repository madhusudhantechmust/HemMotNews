/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	parser.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	1/11/04 ca. 8:51 PM
	
	ab	Abstract:
	ab	Implementation of TParser class.
	
	cl	Change Log:
	cl	13-Jan-10	Port to CS4.
	cl	29-Aug-05	Port to InDesign.
	cl	1/11/04	New.
------------------------------------------------------------------------------*/

//#include <iostream>
//using namespace std;
//#include "common.h"
#include "buffer.h"
//#include "error.h"
#include "parser.h"
//#include "XT_Assert.h"
//#include "CL_Log_File.h"
//#include "global_data.h"

//------------------------------------------------------------------------------
//	TParser::Parse
//------------------------------------------------------------------------------
void TParser::Parse ()
{
	// Loop to extract and put out tokens until end of input.
	do
	{
		if (parserState == stInitial || parserState == stOpenTag || 
			parserState == stAttribute || parserState == stAttributeValue || 
			parserState == stCData || parserState == stCloseTag || 
			parserState == stEntity)
		{
			GetToken ();
		}
		if (token != tcError)
		{
			ProcessToken_();
		}
		else
		{

			// Error handler for InDesign.
			char* warning = new char[256];
			::sprintf (warning, 
				"*** XML PARSER ERROR *** :  file: %s  line: %d", 
				__FILE__, __LINE__);
//			LOG (warning);
			cout << warning << endl;
			delete [] warning;
		}
	} while (token != tcEndOfFile);
	
	if (token == tcEndOfFile)
		cout << "TParser::Parse ()          :  EOF reached.  Stop." << endl;

	// Compiling for simple ANSI C++ app
	// Put out summary report.
//	cout << "Report" << endl;
//	cout << "Lines in the input = " << currentLineNumber << endl;
//	cout << "Syntax errors = " << errorCount << endl;
}


//------------------------------------------------------------------------------
//	TParser::ProcessToken_
//------------------------------------------------------------------------------
void TParser::ProcessToken_()
{
	if (token == tcStartOpenTag)
	{
		parserState = stOpenTag;
	}
	else if (token == tcCharData)
	{
		pDataToken_ = pToken;
	}
	else if (token == tcWord)
	{
		if (parserState == stOpenTag)
		{
			ProcessTag_();
			++tagLevel_;
			parserState = stAttribute;
		}
		else if (parserState == stAttribute)
		{
			parserState = stAttributeValue;
		}
		else if (parserState == stCloseTag)
		{
			--tagLevel_;
			ProcessTagValue_();
			pDataToken_ = NULL;
		}
	}
	else if (token == tcEquals)
	{
	}
	else if (token == tcString)
	{
		if (parserState == stAttributeValue)
		{
			parserState = stAttribute;
		}
	}
	else if (token == tcEndTag)
	{
		parserState = stCData;
	}
	else if (token == tcStartCloseTag)
	{
		parserState = stCloseTag;
	}
	else if (token == tcStartEntity)
	{
		parserState = stEntity;
	}
	else if (token == tcEntity)
	{
		pDataToken_ = pToken;
	}
	else
	{
	}
}


//------------------------------------------------------------------------------
//	TParser::ProcessTag_
//------------------------------------------------------------------------------
void TParser::ProcessTag_()
{
	char *p = pToken->String ();
	char *q = new char [strlen (p) + 1];
	strcpy (q, p);
	delete [] tags_[tagLevel_];
	tags_[tagLevel_] = q;
	cout << "TParser::ProcessTag_()  :  " << q << endl;
}

extern void NewsCAD_Prefs_XML_Callback (
		CL_NewsCAD_Prefs& newsCADPrefsObj, 
		NewsCADPrefsMemFn SetPreferenceValue,
		const char* inXPath, 
		const char* inXMLElem, 
		const TElementValue & inVal)
;
//------------------------------------------------------------------------------
//	TParser::ProcessTagValue_
//------------------------------------------------------------------------------
void TParser::ProcessTagValue_()
{
	if ( pDataToken_ )
	{
		char dataStructName[256];
		strcpy (dataStructName, tags_[1]);
		int i = 2;
		while (tags_[i] && i < tagLevel_)
		{
			strcat (dataStructName, "/");
			strcat (dataStructName, tags_[i]);
			++i;
		}

		TElementValue v;
		v.dataType_ = pDataToken_->Type ();
		v.value_ = pDataToken_->Value ();
		if (v.dataType_ == tyCharacter)
		{
			char *s = pDataToken_->String ();
			if (::strlen (s) > 1)
			{
				v.dataType_ = tyString;
				v.value_.pString = s;					// Pass ptr to string
			}
			else
			{
				v.dataType_ = tyCharacter;
				v.value_.character = *s;					// Pass a char value
			}
		}

		// Send the parsed data to the preferences object.
//		ASSERT (v.value_.pString);
//		pPrefs_->SetPreference_( dataStructName, tags_[tagLevel_], v );
//		NewsCAD_Prefs_XML_Callback (*pPrefs_, XMLProcFunc_, dataStructName, tags_[tagLevel_], v );
		XMLProcFunc_(dataStructName, tags_[tagLevel_], v );
/*		cout << "TParser::ProcessTagValue_() data for callback :" << endl;
		cout << "     dataStructName = " << dataStructName << endl;
		cout << "     Data type      = " << v.dataType_ << endl;
		cout <<	"     TAG            = " << tags_[tagLevel_] << endl;
		cout << "     TElementValue  = ";
		switch (v.dataType_) {
			case tyDummy:		cout << "tyDummy" << endl;				break;
			case tyInteger:		cout << v.value_.integer << endl;		break;
			case tyReal:		cout << v.value_.real << endl;			break;
			case tyCharacter:	cout << v.value_.character << endl;		break;
			case tyString:		cout << v.value_.pString << endl;		break;
		} */
	}
	else
	{
//		cout << "TParser::ProcessTagValue_() :" << endl;
//		cout << "     Empty tag (no PCDATA) found.  Tag = " << pToken->String () << endl;
//		char S_[256];
//		::sprintf (S_, "     %s -->", 
//						
//		cout << S_ << endl;
//		LOG (S_);
//		cout << "\tTag = " << pToken->String() << endl;
	}
}

// END OF FILE
