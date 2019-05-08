/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	token.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	1/11/04 ca. 7:03 PM
	
	ab	Abstract:
	ab	Declarations for token classes:
	ab		TToken, TWordToken, TNumberToken, TStringToken, 
	ab		TSpecialToken, TEOFToken, TErrorToken
	
	cl	Change Log:
	cl	13-Jan-10	Port to CS4.
	cl	29-Aug-05	Port to InDesign.
	cl	3/2/04	Added TEntityToken to for representing XML entities.
	cl	1/11/04	New.
------------------------------------------------------------------------------*/
#ifndef	_XML_TOKEN_H_
#define	_XML_TOKEN_H_

#include "misc.h"
//#include "error.h"
#include "buffer.h"

extern TCharCode charCodeMap[];


class TToken {										// Abstract base clase
public:
	TToken ()
	{
		code			= tcDummy;
		type			= tyDummy;
		value.integer	= 0;
		string[0]		= 0;
	}
	
	TTokenCode	Code () const		{ return code; }
	TDataType	Type () const		{ return type; }
	TDataValue	Value () const		{ return value; }
	char*		String ()			{ return string; }
	
	virtual void	Get (TTextInBuffer &buffer)	= 0;
//	virtual void	Print () const				= 0;

protected:
	TTokenCode	code;
	TDataType	type;
	TDataValue	value;
	char		string[maxInputBufferSize];

private:

};


class TWordToken : public TToken {
public:
	virtual void	Get (TTextInBuffer &buffer)	;
//	virtual void	Print () const				;
};


class TNumberToken : public TToken {
public:
	TNumberToken ()	{ code = tcNumber; }
	virtual void	Get (TTextInBuffer &buffer)	;
//	virtual void	Print () const				;
};


class TStringToken : public TToken {
public:
	TStringToken ()	{ code = tcString; }
	virtual void	Get (TTextInBuffer &buffer);
//	virtual void	Print () const;
};


class TSpecialToken : public TToken {
public:
	virtual void	Get (TTextInBuffer &buffer)	;
//	virtual void	Print () const				;
};


class TCharDataToken : public TToken {
	virtual void	Get (TTextInBuffer &buffer)	;
//	virtual void	Print () const				;
};


class TEntityToken : public TToken {
	virtual void	Get (TTextInBuffer &buffer)	;
	virtual void	Print () const				;
};


class TAttributeToken {
public:
	TAttributeToken ()
		: name (NULL), value (NULL)
	{
	}
	virtual ~TAttributeToken ()
	{
		delete [] name;
		delete [] value;
	}
//	virtual void	Get (TTextInBuffer &buffer)	;
//	virtual void	Print () const	{}			;
	virtual void	Set (char* instr)			;
	char*			GetName () const		{ return name; }
	char*			GetValue () const		{ return value; }

private:
	char*			name;
	char*			value;
};


class TTagToken : public TToken {
public:
	TTagToken ()	{ code = tcStartMarkup; }
	virtual void	Get (TTextInBuffer &buffer)	;
//	virtual void	Print () const				;
	virtual void	SetAttribute (TToken *t);
	TAttributeToken	attribToken;

private:
};


class TEOFToken : public TToken {
public:
	TEOFToken ()	{ code = tcEndOfFile; }
	virtual void	Get (TTextInBuffer &buffer) { }
//	virtual void	Print () const				{cout << "ALL FINISHED" << endl;}
};


class TErrorToken : public TToken {
public:
	TErrorToken ()	{ code = tcError; }
	virtual void	Get (TTextInBuffer &buffer)	;
//	virtual void	Print () const
//	{
//TBD		cerr << "No implementation for TErrorToken::Print()" << endl;
//	}
};

#endif

// END OF FILE
