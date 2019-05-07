/* 	Basic_Display_Ad.h	Author:				Description:	<describe the Basic_Display_Ad class here>*/#ifndef Basic_Display_Ad_H#define Basic_Display_Ad_Husing namespace std;#include <string>#include <cstring>class Basic_Display_Ad {public:	enum FieldNumber {		COLCOUNT = 1, DEPTH, CUSTOMER, FILENAME, SALESPERSON, 		ABSOLUTE_PAGE, LOWER_PAGE, UPPER_PAGE, ODD_PAGE, CAMERA_READY, 		COLOR, NOTES, SECTION, DA_CATEGORY	};public:	/* -------- C O N S T R U C T I O N   A N D   D E S T R U C T I O N --------- */	Basic_Display_Ad ();	virtual ~Basic_Display_Ad ();							// Does nothing (yet)	Basic_Display_Ad (string & inputRec);					// Create from string	Basic_Display_Ad (const Basic_Display_Ad & inObj);		// Copy	Basic_Display_Ad & operator=					(const Basic_Display_Ad & inObj);		// Assignprotected:	virtual void Clear_();			// "Zero out" a live object	virtual void Initialize_();		// Finish construction--call during CTOR only	// Used for copying and assignment	virtual void CopyAllMembers_(const Basic_Display_Ad & inObj);	// Cook raw input into data for storage in member variables.	virtual void ProcessField_(FieldNumber inFieldNumber, string& inString);/* ------------------- G E T T E R S   A N D   S E T T E R S -------------------- */public:	// inputRecord_	virtual void	GetInputRecord_						(string & outInputRec)	{ outInputRec = inputRecord_; }	virtual void	SetInputRecord_						(string & inInputRec)	{ inputRecord_ = inInputRec; }	virtual void	SetInputRecord_						(char* inCStr)			{ inputRecord_ = inCStr; }	// Columns wide	virtual int32	GetColumnCount_()			{ return cols_; }	// Depth	virtual PMReal	GetHeight_()				{ return height_; }	// Customer	virtual void	GetCustomer_						(string & outCustomerName)												{ outCustomerName = customer_; }	// Picture file name	virtual void	GetArtFileName_						(string & outArtFileName)														{ outArtFileName = fileName_; }	virtual const char*	GetArtFileNameCStr_()		{ return fileName_.c_str (); }	// Salesman	virtual void	GetSalesRepName_						(string & outSalesRepName)												{ outSalesRepName = salesRepName_; }	// X-position	// Y-position/* -------------------------- D A T A   M E M B E R S --------------------------- */protected:	// The data from which to create the ad.  During placement, it comes	// from the geometry file.  For the "Put Back" panel, it's the 	// box slug, or the black box data, depending on the target platform.	string inputRecord_;	// Storage locations for all the stuff we find in the record.	int32 cols_;				// columns wide								Field 1	PMReal height_;				// Height of ad (points)					Field 2	string customer_;			// Customer name							Field 3	string fileName_;			// Art file name							Field 4	string salesRepName_;		// Salesperson								Field 5	int16 absolutePage_;		// Designated page number for ad 			Field 6	int16 lowerPage_;			// Bottom end of page						Field 7								// page range to place ad		int16 upperPage_;			// Top end of allowed 						Field 8								// placement range	bool oddPageFlag_;			// True if ad must be placed on odd folio	Field 9	bool cameraReadyFlag_;		// No art file--paste up					Field 10	string color_;				// "BW", "SC", or "4C"						Field 11	string notes_;				// User data								Field 12	string section_;			// Section in mag where ad must appear		Field 13	int32 category_;			// DA Placement Category					Field 14								// For grouping ads in 								// similar classifications	PMReal boxX_;				// X-position	PMReal boxY_;				// Y-position};#endif	// Basic_Display_Ad_H