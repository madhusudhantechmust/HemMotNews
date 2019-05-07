/* 
	Basic_Display_Ad.cpp

	Author:			
	Description:	<describe the Basic_Display_Ad class here>
*/

//	Fields in input record for display ads::
//
//	Field						Macro in old			
//	Number	Contents			CL_Display_Ad		New Macro
//	------	----------------	-------------		---------
//	 1		Columns             xPos				COLCOUNT
//	 2		Depth in Points     yPos				DEPTH
//	 3		Customer Name       clientPos			CUSTOMER
//	 4		Ad File Name        filePos				FILENAME
//	 5		Sales Rep Name      initPos				SALESPERSON
//	 6		Absolute Page       bPos				ABSOLUTE_PAGE
//	 7		Begin Page          pagePos				LOWER_PAGE
//	 8		End Page            lowPos				UPPER_PAGE
//	 9		Odd Page Flag       UpPos				ODD_PAGE
//	10		Camera Ready Flag   OddPos				CAMERA_READY
//	11		Color               LocalPos			COLOR
//	12		Notes               colorPos			NOTES
//	13		Section             Not in old code		SECTION
//	14		Placement Category	Not in old code		DA_CATEGORY

#include "Basic_Display_Ad.h"


//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and D E S T R U C T I O N
//----------------------------------------------------------------------------------

/* --------------------- Default CTOR, not very interesting --------------------- */
Basic_Display_Ad::Basic_Display_Ad ()
	:	cols_(0)
	,	height_(0)
	,	absolutePage_(0)
	,	lowerPage_(0)
	,	upperPage_(0)
	,	oddPageFlag_(false)
	,	cameraReadyFlag_(false)
	,	category_(0)
	,	boxX_(0)
	,	boxY_(0)
{
	Initialize_();
}


/* ------------------------------ Copy Constructor ------------------------------ */
Basic_Display_Ad::Basic_Display_Ad (const Basic_Display_Ad & inObj)
{
	CopyAllMembers_(inObj);
}


/* ---------------------------- Assignment Operator ----------------------------- */
Basic_Display_Ad & Basic_Display_Ad::operator= (const Basic_Display_Ad & inObj)
{
	if (this != &inObj)									// Beware of "a = a"
	{
		CopyAllMembers_(inObj);
	}

	return *this;
}


/* --------------------------------- Destructor --------------------------------- */
Basic_Display_Ad::~Basic_Display_Ad ()
{
}


/* ------------------------- Construct from STL string -------------------------- */
Basic_Display_Ad::Basic_Display_Ad (string & inputRec)
	: cols_(0)
	, height_(0)
	, absolutePage_(0)
	, lowerPage_(0)
	, upperPage_(0)
	, oddPageFlag_(false)
	, cameraReadyFlag_(false)
	, category_(0)
	, boxX_(0)
	, boxY_(0)
{
	Initialize_();								// To finish construction

	// Save the input for storage in a QXP slug later on.
	inputRecord_ = inputRec;

	// Convert and store input into members.
	char ch;
	int fldnum = COLCOUNT;
	string curtok;
	string::iterator p = inputRecord_.begin ();
	
	while (p < inputRecord_.end () && fldnum <= SECTION)
	{
		ch = *p;
		switch (ch)
		{
			case '\r':								// End of field & record
			case '|':								// End of field
				ProcessField_(static_cast<FieldNumber>(fldnum), curtok);
				++fldnum;
				curtok.resize (0);
				break;
			default:								// Append character to token.
				curtok += ch;
				break;
		}		
		++p;
	}
}

/* ---------------------------------- Clear_() ---------------------------------- */
//	Make an instance "tabula rasa."
//----------------------------------------------------------------------------------
void Basic_Display_Ad::Clear_()
{
	inputRecord_.resize (0);
	cols_ = 0;
	height_ = 0;
	customer_.resize (0);
	fileName_.resize (0);
	salesRepName_.resize (0);
	absolutePage_ = 0;
	lowerPage_ = 0;
	upperPage_ = 0;
	oddPageFlag_ = false;
	cameraReadyFlag_ = false;
	color_.resize (0);
	notes_.resize (0);
	section_.resize (0);
	category_ = 0;
	boxX_ = 0;
	boxY_ = 0;
}


/* ------------------------------- Initialize_() -------------------------------- */
// Call this function only from a CTOR -- it clears only the members that 
// aren't dealt with by the initializer list in the CTOR.
//----------------------------------------------------------------------------------
void Basic_Display_Ad::Initialize_()
{
	inputRecord_.resize (0);
	customer_.resize (0);
	fileName_.resize (0);
	salesRepName_.resize (0);
	color_.resize (0);
	notes_.resize (0);
	section_.resize (0);
}


/* ------------------------------ CopyAllMembers_ ------------------------------- */
//	This method is called by the copy ctor and the assignment operator to set all 
//	data members.  Note my assumption that the STL takes care of deallocating the 
//	data in this's string objects when they get assigned.
//----------------------------------------------------------------------------------
void Basic_Display_Ad::CopyAllMembers_(const Basic_Display_Ad & inObj)
{
	inputRecord_ = inObj.inputRecord_;
	cols_ = inObj.cols_;
	height_ = inObj.height_;
	customer_ = inObj.customer_;
	fileName_ = inObj.fileName_;
	salesRepName_ = inObj.salesRepName_;
	absolutePage_ = inObj.absolutePage_;
	lowerPage_ = inObj.lowerPage_;
	upperPage_ = inObj.upperPage_;
	oddPageFlag_ = inObj.oddPageFlag_;
	cameraReadyFlag_ = inObj.cameraReadyFlag_;
	color_ = inObj.color_;
	notes_ = inObj.notes_;
	section_ = inObj.section_;
	category_ = inObj.category_;
	boxX_ = inObj.boxX_;
	boxY_ = inObj.boxY_;
}


//----------------------------------------------------------------------------------
// ProcessField_()
//
// Store a field value in the ad record into a member variable.  Convert text 
// to binary representation as needed.
//----------------------------------------------------------------------------------
void Basic_Display_Ad::ProcessField_(FieldNumber inFieldNumber, string& inString)
{
	// Strip leading spaces from the input
	string::iterator p;
	p = inString.begin ();
	while (*p == ' ')
	{
		inString.erase (p);
	}
	
	// Strip trailing spaces from the input
	size_t len = inString.length ();
	string::reverse_iterator q;
	q = inString.rbegin ();
	while (q != inString.rend ())
	{
		if (*q != ' ')
			break;

		--len;
		++q;
	}
	inString.resize (len);

#if PROCDUCT == QUARKXTENSION										// Quark only
	Fixed fTemp;
#else
 	PMReal fTemp;
#endif

 	const char *pInStr = inString.c_str ();
	PMString pmstr (pInStr);
	PMString::ConversionError convErr;
	CharCounter lenNotConverted;

	switch (inFieldNumber) {
	case COLCOUNT:							// Ad width in columns
 		// Get number of columns spanned by this ad, store in member var
 		long cols;
 		cols_ = cols = ::atol (pInStr);
		break;

 	case DEPTH:												// Ad height in points
//#if PRODUCT == QUARKXTENSION										// Quark only
 //		C2PSTR ( (char*) val );
// 		str2valbuf (NUMBER, 0, &fTemp, val);
// 		height_ = fTemp;
//#else																// InDesign only
		height_ = pmstr.GetAsDouble ( &convErr, &lenNotConverted);
//#endif
 		break;
 
	case CUSTOMER:
		customer_ = inString;
		break;

	case FILENAME:
		fileName_ = inString;
		break;

	case SALESPERSON:
		salesRepName_ = inString;
		break;
		
	case ABSOLUTE_PAGE:
		absolutePage_ = ::atol (pInStr);
		break;

	case LOWER_PAGE:
		lowerPage_ = ::atol (pInStr);
		break;

	case UPPER_PAGE:
		upperPage_ = ::atol (pInStr);
		break;

	case ODD_PAGE:
		if (::atol (pInStr) != 0)
		{
			oddPageFlag_ = true;
		}
		break;

	case CAMERA_READY:
		if (::atol (pInStr) != 0)
		{
			cameraReadyFlag_ = true;
		}
		break;

	case COLOR:
		color_ = inString;
		break;

	case NOTES:
		notes_ = inString;
		break;

	case SECTION:
		section_ = inString;
		break;
	
	// The DA Placement Category is an integer value assigned to each ad record.
	// It's used to group ads that are in related categories but different headings.
	// E.g., we might have "Garbage for Sale" and "Real Junk for Sale" in cat 7.
	case DA_CATEGORY:
		category_ = ::atol (pInStr);
		break;

	default:
		// i don't know yet
		break;
	}	
}
