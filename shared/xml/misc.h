/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	misc.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	1/11/04 ca. 6:10 PM
	
	ab	Abstract:
	ab	Declarations for TBD.
	
	cl	Change Log:
	cl	1/11/11	Port to CS4.
	cl	1/11/04	New.
------------------------------------------------------------------------------*/
#ifndef	XML_PROC_MISC_H
#define	XML_PROC_MISC_H

enum TCharCode {							// Character codes.
	ccTagStart, 
	ccTagEnd, 
	ccLetter, 
	ccDigit, 
	ccSpecial, 
	ccQuote,
	ccWhiteSpace, 
	ccQuestionMark,
	ccEndOfFile, 
	ccError
};


enum TTokenCode {							// Token codes.
	tcDummy, 
	tcWord, 
	tcNumber, 
	tcString, 
	tcPeriod, 
	tcEquals,
	tcStartOpenTag, 
	tcStartCloseTag, 
	tcStartProcInst,
	tcEndTag,
	tcStartMarkup, 
	tcEndMarkup,
	tcCharData,
	tcStartEntity,
	tcEntity,
	tcEndOfFile, 
	tcError
};


enum TDataType {							// Data Types.
	tyDummy,
	tyInteger,
	tyReal,
	tyCharacter,
	tyString
};


union TDataValue {							// Data values.
	int		integer;
	float	real;
	char	character;
	char*	pString;
};

enum ParserState {
	stInitial,
	stOpenTag,
	stAttribute,
	stAttributeValue,
	stCloseTag,
	stCData,
	stEntity,
	stFinished
};


struct TElementValue {
	TDataType	dataType_;
	TDataValue	value_;
};

#endif

// END OF FILE
