/* 
	Basic_Display_Ad.h

	Author:			
	Description:	<describe the Basic_Display_Ad class here>
	cl	9/12/10	Re-examined as part of porting to CS4; no porting changes 
	cl			were required to compile this file successfully.
*/

#ifndef Basic_Display_Ad_H
#define Basic_Display_Ad_H

//using namespace std;

#include <string>
#include <cstring>

class Basic_Page;
class ITextModel;

class Basic_Display_Ad {
public:
	enum FieldNumber {
		COLCOUNT = 1, DEPTH, CUSTOMER, FILENAME, SALESPERSON, 
		ABSOLUTE_PAGE, LOWER_PAGE, UPPER_PAGE, ODD_PAGE, CAMERA_READY, 
		COLOR, NOTES, SECTION, DA_CATEGORY
	};

public:
	/* -------- C O N S T R U C T I O N   A N D   D E S T R U C T I O N --------- */
	Basic_Display_Ad ();
	virtual ~Basic_Display_Ad ();							// Does nothing (yet)
	Basic_Display_Ad (std::string & inputRec);					// Create from string
	Basic_Display_Ad (const Basic_Display_Ad & inObj);		// Copy
	Basic_Display_Ad & operator=
					(const Basic_Display_Ad & inObj);		// Assign
	virtual void Log_();

protected:
	virtual void Clear_();			// "Zero out" a live object
	virtual void Initialize_();		// Finish construction--call during CTOR only
	virtual void SetMemberVariablesFromSTLString_(std::string& inAdRecord);

	// Used for copying and assignment
	virtual void CopyAllMembers_(const Basic_Display_Ad & inObj);

	// Cook raw input into data for storage in member variables.
	virtual void ProcessField_(FieldNumber inFieldNumber, std::string& inString);

	// Object persistence
	virtual void	StoreInBoxSlug_(UIDRef& uidref) throw (PMString);
	
	// Debugging
	void DebugAlert_(const char* inStr);


/* ------------------- G E T T E R S   A N D   S E T T E R S -------------------- */

public:

	// inputRecord_
	virtual void	GetInputRecord_
						(std::string & outInputRec)	{ outInputRec = inputRecord_; }
	virtual void	SetInputRecord_
						(std::string & inInputRec)	{ inputRecord_ = inInputRec; }
	virtual void	SetInputRecord_
						(char* inCStr)			{ inputRecord_ = inCStr; }

	// Columns wide
	virtual int32	GetColumnCount_()			{ return cols_; }

	// Depth
	virtual PMReal	GetHeight_()				{ return height_; }
	virtual PMReal	Height_()					{ return height_; }

	// Customer
	virtual void	GetCustomer_
						(std::string & outCustomerName)
												{ outCustomerName = customer_; }
	// Picture file name
	virtual void	GetArtFileName_
						(std::string & outArtFileName)	
													{ outArtFileName = fileName_; }
	virtual const char*	GetArtFileNameCStr_()		{ return fileName_.c_str (); }

	// Salesman
	virtual void	GetSalesRepName_
						(std::string & outSalesRepName)
												{ outSalesRepName = salesRepName_; }
	// Black & white, spot color, or 4-color
	virtual void	GetColor_
						(std::string & outColor)
												{ outColor = color_; }
	// Section
	virtual void	GetSection_
						(std::string & outSection)
												{ outSection = section_; }
	// Ad width in points
	virtual void	SetWidth_(PMReal inWidth)	{ width_ = inWidth; }
	virtual PMReal	Width_()					{ return width_; }
	// Width
	virtual void	ComputeWidth_(PMReal inColumnWidth, PMReal inGutterWidth);
	
	// X-position
	virtual void	SetBoxX_(PMReal inX)		{ boxX_ = inX; }
	virtual PMReal	XPosition ()				{ return boxX_; }
	
	// Y-position
	virtual void	SetBoxY_(PMReal inY)		{ boxY_ = inY; }
	virtual PMReal	YPosition ()				{ return boxY_; }

	virtual void	SetPointerToParentPage_(
					Basic_Page* inParentPage)	{ pMyPage_ = inParentPage; }
	virtual 
	Basic_Page*		ParentPage_()			{ return pMyPage_; }
	
	// Put something representing an ad on the page:
	virtual void		PasteUp_(UIDRef destinationSpreadUIDRef, 
							PMRect & pageRect) throw (PMString);
	virtual void		InsertAdDescriptiveText_(UIDRef inStoryUidRef);
	virtual void		ComputeAdDescriptionText_(std::string & ioDescText);
	virtual ErrorCode	ApplyTextAttributeOverrides_(ITextModel* textModel, 
									const TextIndex position, const int32 length) 
									throw (PMString);

/* -------------------------- D A T A   M E M B E R S --------------------------- */

protected:
	// The data from which to create the ad.  During placement, it comes
	// from the geometry file.  For the "Put Back" panel, it's the 
	// box slug, or the black box data, depending on the target platform.
	std::string inputRecord_;

	// Storage locations for all the stuff we find in the record.
	int32 cols_;				// columns wide								Field 1
	PMReal height_;				// Height of ad (points)					Field 2
	std::string customer_;			// Customer name							Field 3
	std::string fileName_;			// Art file name							Field 4
	std::string salesRepName_;		// Salesperson								Field 5
	int16 absolutePage_;		// Designated page number for ad 			Field 6
	int16 lowerPage_;			// Bottom end of page						Field 7
								// page range to place ad	
	int16 upperPage_;			// Top end of allowed 						Field 8
								// placement range
	bool oddPageFlag_;			// True if ad must be placed on odd folio	Field 9
	bool cameraReadyFlag_;		// No art file--paste up					Field 10
	std::string color_;				// "BW", "SC", or "4C"						Field 11
	std::string notes_;				// User data								Field 12
	std::string section_;			// Section in mag where ad must appear		Field 13
	int32 category_;			// DA Placement Category					Field 14
								// For grouping ads in 
								// similar classifications
	PMReal width_;				// Accounting for columns and gutters spanned
	Basic_Page* pMyPage_;		// NewsCAD parent object representing page on which 
								// to paste nyself up.
	// UIDRef is a class, not a typedef.  It is for InDesign to create 
	// unique signatures for this ad's page items.
	// They're a combination of the UIDs (record numbers) and database identifier. 
	UIDRef	picBoxUidRef_;
	UIDRef	textBoxUidRef_;

	// Final coordinates of ad when placed
	PMReal boxX_;				// X-position
	PMReal boxY_;				// Y-position
};


#endif	// Basic_Display_Ad_H
