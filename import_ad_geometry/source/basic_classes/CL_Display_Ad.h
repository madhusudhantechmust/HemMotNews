/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	CL_Display_Ad.hpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	02-9-02 ca. 10:13 PM
	
	ab	Abstract:
	ab	Declarations for CL_Display_Ad.hpp class.
	
	cl	Change Log:
	cl	10-Sep-05	Began port to InDesign.
	cl	19-Feb-05	Began port to QXP 6.x.
	cl	02-Sep-02	New.
----------------------------------------------------------------------------------*/

#ifndef CL_DISPLAY_AD_H
#define	CL_DISPLAY_AD_H

class ITextModel;

//using namespace std;
#include <string>
#include "Basic_Display_Ad.h"

class CL_NewsCadRect;
class CL_Page;

class CL_Display_Ad : public Basic_Display_Ad {
public:
	enum ContentType { None, Picture, Text };
	
	/* -------- C O N S T R U C T I O N   A N D   D E S T R U C T I O N --------- */

	CL_Display_Ad ();									// Create
	CL_Display_Ad (std::string & inputRec);					// Create from STL string
	CL_Display_Ad (const CL_Display_Ad & inObj);		// Copy
	CL_Display_Ad & operator=
					(const CL_Display_Ad & inObj);		// Assign
	virtual	~CL_Display_Ad ();							// Currently, nothing

	// Helper/bottleneck for copy/assign.
    // Change by TechMust-MJ - 20190627
    using Basic_Display_Ad::CopyAllMembers_;
    void	CopyAllMembers_(const CL_Display_Ad & inObj);
	
	// Zero out/empty all instance variables.
	virtual void	Clear_();

	// Convert width in columns to width in points.
	virtual void	ComputeAdWidth_();
	
	void 			GetQuarkRect_
						(CL_NewsCadRect* inFRect);

	PMReal			GetAdLeft_()				{ return XPosition (); }
	PMReal			GetAdTop_()					{ return YPosition (); }
	PMReal			GetAdRight_()				{ return XPosition () + width_; }
	PMReal			GetAdBottom_()				{ return YPosition () + height_; }

///////////////////////////////////////////////////////////////////////////////
#if 0
#pragma mark ALL THIS IS IN THE BASE CLASS
#pragma mark •• GETTERS ••
	/* ----------------------------- G E T T E R S ------------------------------ */

	// Make an STL string copy of the input for this ad.
	virtual void	GetInputRecord_
						(string & outInputRec)	{ outInputRec = inputRecord_; }

	// Copy raw input into STL string instance variable.
	virtual void	SetInputRecord_
						(string & inInputRec)	{ inputRecord_ = inInputRec; }
	virtual void	SetInputRecord_
						(char* inCStr)			{ inputRecord_ = inCStr; }
						
	virtual int32	GetColumnCount_()			{ return cols_; }
	virtual PMReal	GetHeight_()				{ return height_; }
	virtual void	GetCustomer_
						(string & outCustomerName)
												{ outCustomerName = customer_; }
	virtual void	GetArtFileName_
						(string & outArtFileName)	
												{ outArtFileName = fileName_; }
	virtual const char*	GetArtFileNameCStr_()		{ return fileName_.c_str (); }
	virtual void	GetSalesRepName_
						(string & outSalesRepName)
												{ outSalesRepName = salesRepName_; }
	virtual int16	GetAbsolutePageNumber_() 	{ return absolutePage_;}
	int16			GetLowerPage_()				{ return lowerPage_; }
	int16			GetUpperPage_()				{ return upperPage_; }
	virtual bool	GetOddPageFlag_()			{ return oddPageFlag_; }
	virtual bool	GetCameraReadyFlag_()		{ return cameraReadyFlag_; }
	virtual void	GetColor_
						(string & outColor)
												{ outColor = color_; }
	virtual void	GetNotes_
						(string & outNotes)
												{ outNotes = notes_; }
	virtual void	GetSection_
						(string & outSection)
												{ outSection = section_; }
	virtual int32	GetCategory_()				{ return category_; }
	virtual PMReal	GetWidth_()					{ return width_; }
	PMReal			GetAdLeft_()				{ return boxx_; }
	PMReal			GetAdTop_()					{ return boxy_; }

	PMReal			GetAdRight_()				{ return boxx_ + width_; }
	PMReal			GetAdBottom_()				{ return boxy_ + height_; }
	void			GetAdNumber_(string & stlStr);
	UIDRef			PicBoxUIDRef_()				{ return picBoxUidRef_; }
	CL_Page*		MyPage_()					{ return pMyPage_; }
	
#pragma mark •• SETTERS ••
	/* ----------------------------- S E T T E R S ------------------------------ */

	virtual void	SetWidth_(Fixed inColumnWidth, Fixed inGutterWidth);
	virtual void	SetCustomer_(string & inCustomer)
												{ customer_ = inCustomer; }
	virtual void	SetCustomer_(uchar* inCustomerPascalString);
	virtual void	SetArtFilename_(string & inFilename)
												{ fileName_ = inFilename; }
	virtual void	SetArtFilename_(uchar* inArtFilePascalString);
	virtual void	SetSalesperson_(string & inSalesperson)
												{ salesRepName_ = inSalesperson; }
	virtual void	SetSalesperson_(uchar* inSalespersonPascalString);
	virtual void	SetPicBoxUIDRef_(UIDRef inRef)
												{ picBoxUidRef_ = inRef; }
	virtual void	SetBoxx_(PMReal inBoxx)		{ boxx_ = inBoxx; }
	virtual void	SetBoxy_(PMReal inBoxy)		{ boxy_ = inBoxy; }
#endif
///////////////////////////////////////////////////////////////////////////////

/* ------------------- G E T T E R S   A N D   S E T T E R S -------------------- */

	virtual int16	GetPageNumber_() 			{ return pageNum_;}
	virtual int16	GetPosition_() 				{ return position_; }
	virtual bool	GetPlaced_()				{ return placed_; }

	virtual void	SetPageNumber_(int16 inNum)	{ pageNum_ = inNum; }
	virtual void	SetPosition_(int16 inPos)	{ position_ = inPos; }
	virtual void	SetPlaced_(bool inPlaced)	{ placed_ = inPlaced; }
	
	/* ----------------------- T R A C E   &   D E B U G ------------------------ */

	// Write an ad object to the log file, like a description method, 
	// or an overloaded operator<< (but it isn't, really).
	virtual void	Log_();

	// Write out information about ad's placement on the page, including geometry.
	virtual void	LogPasteup_();

	virtual void	LogNameAndPage_();			// For ads placed on spec'd page #.
	virtual void	LogNameAndPageLimits_();	// Pretty-print upper/lower page #s.
	virtual void	LogInputRec_();				// Pretty-print the input

	/* ------------------------- P A G E   M A K E U P -------------------------- */

	// Put an ad on the page, at last!  This is an overloaded version of the 
	// method in the Basic_Display_Ad class that simply plunks the ad down in 
	// the middle of the page.

    // Change by TechMust-MJ - 20190627
    using Basic_Display_Ad::PasteUp_;
    void PasteUp_(CL_Page* p) throw (OSStatus);
	
	/* --------------------------- A D   R E P O R T ---------------------------- */

	// Talk back to the database for billing purposes especially.
	virtual void	FormatForAdReport_(std::string & adRecord);
	
///////////////////////////////////////////////////////////////////////////////
#if 0
NOT READY TO IMPLEMENT YET
	/* ---------------------- E D I T   S L U G   I N F O ----------------------- */

	// Allow user to update information for Client, 
	// Filename, and Designer (initials_).
	virtual void	StoreDialogResultsInBoxSlug_();
#endif
///////////////////////////////////////////////////////////////////////////////

protected:
	// Box creation
	virtual UIDRef		CreateBox_ID_(ContentType inContentType);
	virtual UIDRef		CreatePictureAndTextBoxes_ID_();

///////////////////////////////////////////////////////////////////////////////
#if 0
THESE ARE TEMPORARILY HANDLED BY THE BASE CLASS; FOR THE MOMENT, DO NOT OVERRIDE
	// Object persistence
	virtual void	StoreInBoxSlug_(UIDRef& uidref);
	virtual void		InsertAdDescriptiveText_(UIDRef inStoryUidRef);
	virtual void		ComputeAdDescriptionText_(string & ioDescText);
	virtual ErrorCode	ApplyTextAttributeOverrides_(ITextModel* textModel, 
									const TextIndex position, const int32 length);
#endif
///////////////////////////////////////////////////////////////////////////////
///////////////////////////////////////////////////////////////////////////////
#if 0
THIS IS QUARK ONLY
	virtual void	ChangeAdInfo_();	// For updating with info from the 
									// "Edit Ad Information" WAP
	inline PMReal BoxX_() const
		{
			return boxx_;
		}
	inline PMReal BoxY_() const
		{
			return boxy_;
		}
#endif
///////////////////////////////////////////////////////////////////////////////

	int16		pageNum_;			// Page number where it was placed
	int16 		position_;			// Coordinate position (set during pasteup)
	bool		placed_;			// Has ad been put on page yet?

/*
THESE ARE IN THE BASE CLASS NOW:
									// Raw input from the geometry file
	string	inputRecord_;			// Data from geometry file for this ad

									// Parsed into fields
	int32	cols_;					// columns wide				Field 1
	PMReal	height_;				// Height of ad (points)	Field 2
	string	customer_;				// Customer name			Field 3
	string	fileName_;				// Art file name			Field 4
	string	salesRepName_;			// Salesperson				Field 5
	int16	absolutePage_;			// Page number ad 			Field 6
									// must appear on
	int16	lowerPage_;				// Bottom end of page		Field 7
									// page range to place ad	
	int16 	upperPage_;				// Top end of allowed 		Field 8
									// placement range
	bool	oddPageFlag_;			// True if ad must be		Field 9
									// placed on odd foliio
	bool	cameraReadyFlag_;		// No art file--paste up	Field 10
	string	color_;					// "BW", "SC", or "4C"		Field 11
	string	notes_;					// User data				Field 12
	string	section_;				// Section in mag where		Field 13
									// ad must appear
	int32	category_;				// DA Placement Category	Field 14
									// For grouping ads in 
									// similar classifications
	// Dynamic stuff set during pasteup/placement
	PMReal		width_;				// Width of ad (points--computed from field 1)
	PMReal		boxx_;				// x1-pos where placed
	PMReal		boxy_;				// y1-pos where placed
	CL_Page*	pMyPage_;			// Page that contains this ad
	// UIDRef is a class, not a typedef.  It is for InDesign to create 
	// unique signatures for this ad's page items.
	// They're a combination of the UIDs (record numbers) and database identifier. 
	UIDRef		picBoxUidRef_;
	UIDRef		textBoxUidRef_;
*/	
};

#endif	// CL_DISPLAY_AD_H

// END OF FILE


/* Previous class private data members:
private:
	uchar	name_[24]; 		// art file name						Field 4
	uchar	client_[24]; 	// the advertiser name					Field 3
	uchar	initials_[16]; 	// designer's and up to 3 				Field 5
							// sales reps' initials
	Fixed	height_;		// Height of ad							Field 2
	Fixed	width_;			// Width of ad
	int32	priority_;		// Premium, house, legal
	Fixed	border_;		// border width							Field 6
	int16	pageNum_;		// page number it appears on			Field 7
	int16 	position_;		// coordinate position
	Fixed	boxx_;			// final x position (after placement)
	Fixed	boxy_;			// final y position (after placement)
	int16	lowerPage_;		// bottom end of allowed page 			Field 8
							// range to place
	int16 	upperPage_;		// top end of allowed placement range	Field 9
	int16	oddPage_;		// true if ad must appear on an 		Field 10
							// odd-number folio
	int16	local_;			// not using right now					Field 11
	uchar	color_[7];		//										Field 12
	int32	cols_;			// columns wide							Field 1

	uchar	skip_[2];		// not using right now -- always "no"
	bool	placed_;		// Has ad been put on page yet?
*/
// Old 3.32 68K program declaration:
//typedef struct {
//	unsigned char	name[24]; 		// art file name
//	unsigned char	client[24]; 	// the advertiser name
//	unsigned char	initials[16]; 	// designer's and up to 3 sales reps' initials
//	Fixed	        Height;			// Height of ad
//	Fixed			Width;			// Width of ad
//	long			priority;		// Premium, house, legal
//	Fixed			border;			// border width
//	char		 	PageNo;			// page number it appears on
//	char 			Position;		// coordinate position
//	Fixed			boxx;			// final x position (after placement)
//	Fixed			boxy;			// final y position (after placement)
//	char			LowerPage; 
//	char 			UpperPage;
//	char			OddPage;
//	char			Local;
//	unsigned char	Color[7];
//	int16			cols;
//	unsigned char	Skip[2];
//} Ads;
