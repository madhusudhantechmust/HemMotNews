//------------------------------------------------------------------------------
//	Source name:
//		CL_Geometry_Importer.cpp
//
//	Abstract:
//		Implementation of NewsCAD geometry file importer class.  This object 
//		reads in an export file supplied by the database, then builds a 
//		QuarkXPress document from it.
//
//	History:
//		01-Sep-02	New.
//
//------------------------------------------------------------------------------

/* ---------------------------------- Includes ---------------------------------- */

#include <string>
#include "IPageList.h"
#include "IDocument.h"
#include "newscad.h"
#include "Basic_Display_Ad.h"
#include "CL_Geometry_Importer.h"
#include "CL_Log_File.h"
#include "CL_NewsCAD_Prefs.h"
#include "global_data.h"
#include "SDKFileHelper.h"
#include "CAlert.h"
#include "IBlackBoxCommands.h"
#include "IBlackBoxCmdData.h"
#include "ILayoutUIUtils.h"
#include "FileUtils.h"
//#include "ImportGeomID.h"

void dump_ad (CL_Display_Ad ad);

using namespace std;

#include "CL_Display_Ad.h"
#include "CL_Document.h"

/* --------------------------------- Debugging ---------------------------------- */

// Turn this on to get tons of alerts when adCoverageVector_ is allocated/accessed.
#define	DEBUG_AD_COVERAGE_VECTOR_READS_AND_WRITES	0

///////////////////////////////////////////////////////////////////////////////
#if 0
#if HNM_PRODUCT == PLUGIN
	#include "WFPID.h"
// 	#include "ITextModel.h"
// 	#include "IAttributeStrand.h"
// 	#include "LayoutUIUtils.h"
// 	#include "LayoutUtils.h"
// 	#include "IStoryList.h"
// 	#include "IStyleInfo.h"
// 	#include "IFrameList.h"
// 	#include "IHierarchy.h"
// 	#include "IMasterPage.h"		// For enhanced logging
// 	#include "TextIterator.h"
// 	#include "IPasteboardUtils.h"
// 	#include "ISpread.h"
#endif

/*
#include <string>
#include "CL_NewsCAD_XT.h"
//#include "NewsCAD.hpp"
#include "CL_Page.h"
#if CLIENT == REMINDER
	#include "RM_Page.h"
#endif
#if CLIENT == HEMMINGS
	#include "HM_Page.h"
#endif
#include "WarningMessage.h"
*/

void dump_ad (CL_Display_Ad ad);
void dump_ad_input_rec (CL_Display_Ad ad);
#endif
///////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and  D E S T R U C T I O N
//----------------------------------------------------------------------------------
CL_Geometry_Importer::CL_Geometry_Importer ()
	:	masterPagesRecFound_(false)
	,	xtErr_(noErr)
	,	inputFilePath_(NULL)
	,	adCoverageThreshhold_(0)
	,	currentDocument_(NULL)
{
	LOG_BEGIN_FUNCTION;

	// Possibly optimize memory management a bit by pre-allocating space in vectors
	// TBD: Handle exceptions/errors
	unplacedAdsVector_.reserve (512);
	currentDocument_ = NULL;
	adsToPlaceVector_.reserve (512);
	placedAdsVector_.reserve (512);
	colorPagesVector_.reserve (512);
	int16 pageCount;
//	IDocument* iDocument = ::GetFrontDocument ();	// LayoutUIUtils.h
	auto_ptr<CL_Document> apCurrentDocument (new CL_Document);
	currentDocument_ = apCurrentDocument.get ();
	IDocument* iDocument = currentDocument_->IDocument_();
	InterfacePtr<IPageList> pageList (iDocument, UseDefaultIID());
	pageCount = pageList->GetPageCount ();
	LOG_TAG_AND_VALUE ("InitialPageCount", pageCount);
	int vecsiz = pageCount + 1;						// Start value subject to change
	vecsiz <<= 4;									// Times 16
	adCoverageVector_.reserve (vecsiz);
	adCoverageVector_.assign (vecsiz, 0L);
char s[256];
sprintf (s, "Allocated adCoverageVector_: %d slots", vecsiz);
#if DEBUG_AD_COVERAGE_VECTOR_READS_AND_WRITES
	CAlert::InformationAlert (s);
#endif
LOG (s);
///////////////////////////////////////////////////////////////////////////////
#if 0
	inputFileSpec_.vRefNum = 0;						// short
	inputFileSpec_.parID = 0;						// long
	inputFileSpec_.name[0] = 0;						// a Str63 on MacOS
	inputFileRefNum_ = 0;
	
#if HNM_PRODUCT == QUARKXTENSION
	CL_Document* pCurDoc = gpNewsCAD_XT->GetCurrentDocumentObject_();
	ASSERT (pCurDoc);
	pCurDoc->GetPageCount_(&pageCount);
#else												// InDesign-specific
#endif
#endif
///////////////////////////////////////////////////////////////////////////////

	LOG_END_FUNCTION;
}

CL_Geometry_Importer::~CL_Geometry_Importer ()
{
	LOG_BEGIN_FUNCTION;
	LOG_END_FUNCTION;
}


//------------------------------------------------------------------------------
void dump_ad (CL_Display_Ad ad)
{
	ad.Log_();
}

//------------------------------------------------------------------------------
void CL_Geometry_Importer::Run_()
{
	LOG_BEGIN_FUNCTION;
	try {
		// Display dialog for user to choose file to import.
		ChooseFile_();
		
		// If user chose a file, process it.
		if (inputFilePath_)
		{
			CreateAdObjectsFromInputFile_();
			delete [ ] inputFilePath_;
		}
		
	}
	catch (string exceptionString) {
		// Log the error and put it on the screen for the user.
		LOG ("<exception>");
		LOG ( exceptionString.c_str () );
		LOG ("</exception>");
		PMString logstr ( exceptionString.c_str () );
		CAlert::ModalAlert
		(
			logstr,
			kOKString, 					// OK button
			kNullString, 				// No second button
			kNullString, 				// No third button
			1,							// Set OK button to default
			CAlert::eInformationIcon	// Information icon.
		);
		delete gpPrefsObj;
		gpPrefsObj = NULL;
	}
	catch (...) {
		// EMPTY
		// i don't know yet
	}
	
	// Place the ads in the current InDesign document.
	try {
		auto_ptr<CL_Document> apCurrentDocument (new CL_Document);
		currentDocument_ = apCurrentDocument.get ();
		gpPrefsObj = new CL_NewsCAD_Prefs;
		gpPrefsObj->LoadPreferencesFromFile_("NewsCAD_Prefs.XML");
		BuildInDesignDocument_();
		StoreUnplacedAdsInDocSlug_();
		delete gpPrefsObj;
		gpPrefsObj = NULL;
	}
	catch (OSStatus e) {
		// EMPTY
		// i don't know yet
		delete gpPrefsObj;
		gpPrefsObj = NULL;
	}
	LOG_END_FUNCTION;
}

/* -------------------------------- ChooseFile_ --------------------------------- */
void CL_Geometry_Importer::ChooseFile_()
{
	LOG_BEGIN_FUNCTION;

	SDKFileOpenChooser fileChooser;
	PMString pmsTitle ("Choose Geometry File");			// Setup dialog title
	pmsTitle.SetTranslatable (kFalse);
	PMString pmsFilterTxtFiles ("TEXT files (txt)");	// Setup file filter
	pmsFilterTxtFiles.SetTranslatable (kFalse);
	fileChooser.AddFilter ('TEXT', "txt", pmsFilterTxtFiles);
	PMString pmsFilterTextFiles ("TEXT files (text)");
	pmsFilterTextFiles.SetTranslatable (kFalse);
	fileChooser.AddFilter (kTEXTFileTypeInfoID, pmsFilterTextFiles);
	
	// Add filter that allows the user to turn off filtering and show all files.
	fileChooser.AddAllFiles ();

	fileChooser.SetTitle (pmsTitle);
	fileChooser.ShowDialog ();
	if ( fileChooser.IsChosen () )
	{
		// Set our members to specify the file.
		IDFile chosenFile = fileChooser.GetIDFile ();
/*
        // Deprecated use of methodology to convert HFS to POSIX
        PMString filePath = fileChooser.GetPath ();
		int i = 0;
		CharCounter k = filePath.IndexOfCharacter (':');
		while (k > 0)
		{
			filePath.Remove (k, 1);
			filePath.Insert ('/', k);
			k = filePath.IndexOfCharacter (':');
			++i;
			if (i > 512) break;
		}
		filePath.Insert ("/Volumes/");
		int32 pathlen = filePath.GetPlatformString().length();
		ConstCString path = filePath.GetPlatformString().c_str ();
 */
        PMString filePath;
        FileUtils::IDFileToPMString (chosenFile, filePath, kTrue);
		inputFilePath_ = new char [filePath.GetPlatformString().length() + 1];
		::strcpy (inputFilePath_, filePath.GetPlatformString().c_str ());
#if VERSION == IN_HOUSE
		string s;
		s = "<ExportsFile path=\"";
		s += inputFilePath_;
		s += "\"/>";
		const char *cstr = s.c_str ();
		LOG (cstr);
#endif
	}
	
	LOG_END_FUNCTION;
}


//----------------------------------------------------------------------------------
// CreateAdObjectsFromInputFile_()
//
// Stores input file contents into display ad objects.
// Replaces the old Parse_() method.
//----------------------------------------------------------------------------------
void CL_Geometry_Importer::CreateAdObjectsFromInputFile_() throw (string)
{
#define	DUMP_PARSED	1
	enum State {
		initializing, color_pages, master_pages, 
		ad_records, not_newscad_file, exit
	};

	LOG_BEGIN_FUNCTION;
	LOG ("<InputFilePath>");
	LOG (inputFilePath_);
	LOG ("</InputFilePath>");
	LOG ("<InputFileContents>");

	ifstream geometry_file (inputFilePath_);
	if (geometry_file.is_open()) {
		LOG ("SUCCESS -- Opened geometry file for input.");
		char* inputCstr = new char [8192];
		if (!inputCstr) {
			geometry_file.close ();
			LOG ("</InputFileContents>");
			throw "Could not allocate memory to read file -- aborting.";
		}
		// The geometry file is open and ready to read.
		State state = initializing;
		while (geometry_file) {
			geometry_file.getline (inputCstr, 512, '\r');
			if (geometry_file) {
				if ( ::strlen (inputCstr) == 0 ) {				// Ignore blank or empty records
					LOG ("<record />");
					continue;
				}
				string s (inputCstr);							// Copy/convert input to STL string
#if VERSION == IN_HOUSE
				string logstr ("<record>" + s + "</record>");
				LOG ( logstr.c_str () );
#endif
				switch (state) {
					case initializing:							// Make sure it's a NewsCAD file
						VerifyFile_(s) ? state = color_pages : state = not_newscad_file;
						break;
					case not_newscad_file:
						throw ("Selected file is not a NewsCAD file.");
						break;
					case color_pages:
						if (StoreColorPages_(s))
						{
							// After the "CP|nnn|nnn|..." record, we MAY get a master 
							// pages record that looks like "MP|...".  Note that the 
							// presence of a "MP|..." record is not required, so we just 
							// set it as the next state, and handle it there.
							state = master_pages;
						}
						else
						{
							state = not_newscad_file;
						}
						break;
						case master_pages:
							// Examine the 1st 2 chars of the input record, see if they're 
							// "MP".  If they are, process this record as a master page 
							// record, otherwise, the record is an ad record.
							if (s[0] == 'M' && s[1] == 'P')
							{
								masterPagesRecFound_ = true;
								masterPageCmd_ = s;
								state = ad_records;
								break;		// Read next record.
							}
						
							// If we get here, the record is not a "MP" record, it's an 
							// ad record.  Update state and fall thru and process it.
							state = ad_records;

case ad_records:
	{
		// Create ad object from raw (file) data
		// TBD:  Override the placement method in a new class, CL_Display_Ad
		CL_Display_Ad adObj (s);
		
		// Store ad object in vector
		unplacedAdsVector_.push_back (adObj);
	}
	break;
							case exit:
								break;
							default:
								break;
				}
			}
		}
		geometry_file.close ();
	}
	else {
		LOG ("</InputFileContents>");
		throw "FAILED -- Did not open geometry file.\n";
	}

	LOG ("</InputFileContents>");

#if VERSION == IN_HOUSE && DUMP_PARSED == 1
	LOG ("<DumpunplacedAdsVector_>");
	char logstr[256];
	sprintf (logstr, "<!-- Ad vector contains: %d elements. -->", 
				unplacedAdsVector_.size ());
	LOG (logstr);
	for_each (unplacedAdsVector_.begin (), 
				unplacedAdsVector_.end (), dump_ad );
	LOG ("</DumpunplacedAdsVector_>");
#endif

	LOG_END_FUNCTION;
}


//------------------------------------------------------------------------------
// VerifyFile_()
// Ensure the file the user chose is a NewsCAD geometry file.
//------------------------------------------------------------------------------
bool CL_Geometry_Importer::VerifyFile_(string & s)
{
	LOG_BEGIN_FUNCTION;

	string cmpstr = "NewsCAD Geometry File";
	LOG_END_FUNCTION;
	
	return (s.compare (0, cmpstr.length (), cmpstr) == 0);
}

//----------------------------------------------------------------------------------
// StoreColorPages_()
// Ensure the record passed in is a color pages record.  If it is, store the 
// color page numbers in our member variables.  Inform caller of how things went.
//----------------------------------------------------------------------------------
bool CL_Geometry_Importer::StoreColorPages_(string & s)
{
	LOG_BEGIN_FUNCTION;

	enum State			{initializing, color_pages, finished};
	State				state = initializing;
	bool				retval = false;
	string				currentToken;
	string::iterator	p = s.begin ();
	char				c = *p;
	
	while (state != finished)
	{
		switch (state) {
		case initializing:
			if (c < ' ')					// No color pages-- s == "CP\r"
			{
				LOG ("No color pages.");
				state = finished;
				retval = true;
			}
			else if (c != '|')
			{
				currentToken += c;
			}
			else
			{
				// End of token.
				#if VERSION == IN_HOUSE
					string ds = "\tToken = ";
					ds += currentToken;
					const char *cstr = ds.c_str ();
					LOG (cstr);
				#endif
				if (currentToken == "CP")
				{
					state = color_pages;
				}
				else
				{
					state = finished;
					LOG ("Color pages record isn't formatted correctly.");
				}
				currentToken.resize (0);
			}
			break;
		
		case color_pages:
			if (c == '|')					// End of token
			{
				ProcessColorPageToken_(currentToken);
			}
			else if (c < ' ')				// End of token and record
			{
				ProcessColorPageToken_(currentToken);
				state = finished;
				retval = true;
			}
			else
			{
				if (isdigit (c))
				{
					currentToken += c;
				}
			} 			
			break;
		
		case finished:						// Never gets here.
			break;
		
		default:
			break;
		}
		
		++p;
		c = *p;
	}

	LOG_END_FUNCTION;
	
	return retval;
}


//----------------------------------------------------------------------------------
// ProcessColorPageToken_()
//----------------------------------------------------------------------------------
void CL_Geometry_Importer::ProcessColorPageToken_(string & tok)
{
	LOG_BEGIN_FUNCTION;

#if VERSION == IN_HOUSE
	string debugStr = "<Token>" + tok + "</Token>";
	const char* s = debugStr.c_str ();
	LOG (s);
#endif
	const char *colorPageStr = tok.c_str ();
	long colorPageNum = ::atol (colorPageStr);
	colorPagesVector_.push_back (colorPageNum);
	tok.resize (0);

	LOG_END_FUNCTION;
}


/*================================================================================*/
void CL_Geometry_Importer::BuildInDesignDocument_() {
/*----------------------------------------------------------------------------------
	Abstract:
		No code in here yet, because our only InDesign customers are Hemmings and 
		Mountain Enterprise.  As we sell the product to other clients, function will 
		have to be fleshed out. See HM_Geometry_Importer and ME_Geometry_Importer 
		for the client-specific stuff.
		
	Parameters and modes:

	Returns:

	Change Log:
		14-Dec-10	Began port to CS4.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	LOG ("ERROR: To build a document, derive a class and override this function.");
	LOG_END_FUNCTION;
}


//------------------------------------------------------------------------------
// Compute percentage of page that's allowed to be covered by ads
//------------------------------------------------------------------------------
void CL_Geometry_Importer::ComputeAdCoverageThreshhold_()
{
	ASSERT (gpPrefsObj);
	char s[256];
	gpPrefsObj->GetAdCoveragePercent_(s);
	
	// We're interested only in the string up to the first non-numeric character.
	char* p = s;
	while ( *p && isdigit (*p) )
	{
		++p;
	}
	*p = '\0';
	
	// Convert from integral percent to Fixed.
	double d = static_cast<double> ( ::atoi (s) );
	d /= 100.0;
	d *= 65536;
	adCoverageThreshhold_ = d;
}


//------------------------------------------------------------------------------
//	Returns true if pageNum was designated as printing in color by the 
//	NewsCAD geometry input file.  Folios of pages designated for color are 
//	stored in our own object's colorPagesVector_.
//------------------------------------------------------------------------------
bool CL_Geometry_Importer::IsColorPage_(int16 pageNum)
{
	vector<long>::iterator p = find (colorPagesVector_.begin (), 
									colorPagesVector_.end (),
									pageNum);
	return ( p != colorPagesVector_.end () );
}




//------------------------------------------------------------------------------
//	Check the number of elements in the adCoverageVector_ and if it is not at 
//	least inMinimumSize, resize it to that value plus one and initialize the 
//	new elements to zero.
//------------------------------------------------------------------------------
void CL_Geometry_Importer::ResizeAdCoverageVectorIfNeeded_(
									int16 inMinimumSize)
{
	LOG_BEGIN_FUNCTION;
	// Does the vector have enough slots, or must we resize?
	int vecsiz = adCoverageVector_.size ();
char s[256];
sprintf (s, "ResizeAdCoverageVectorIfNeeded_(%d):  adCoverageVector_ has %d slots", 
inMinimumSize, vecsiz);
#if DEBUG_AD_COVERAGE_VECTOR_READS_AND_WRITES
	CAlert::InformationAlert (s);
#endif
LOG (s);
	DECLARE_logstr;
	::sprintf (logstr, "Ad coverage vector size = %d, minimum required = %d<br/>",
				vecsiz, inMinimumSize);
	LOG (logstr);
	if (vecsiz <= inMinimumSize)
	{
		adCoverageVector_.resize (inMinimumSize + 1, 0);
		vecsiz = adCoverageVector_.size ();
		::sprintf (logstr, "After resizing ad coverage vector size = %d<br/>",
				vecsiz);
#if DEBUG_AD_COVERAGE_VECTOR_READS_AND_WRITES
	CAlert::InformationAlert (logstr);
#endif
		LOG (logstr);
	}
	LOG_END_FUNCTION;
}
//----------------------------------------------------------------------------------
//	StoreArea_
//
//	Keep track of area on each page consumed by ads.  Writes to adCoverageVector_.
//----------------------------------------------------------------------------------
void CL_Geometry_Importer::StoreArea_(PMReal adWidth, PMReal adHeight, int16 folio)
{
	// Does the vector have enough slots, or must we resize?
/*
	int vecsiz = adCoverageVector_.size ();
	if (vecsiz < folio)
	{
		adCoverageVector_.resize (folio + 1, 0);
	}
*/
	ResizeAdCoverageVectorIfNeeded_(folio);
	
	// Store the info
char s[256];
int vecsiz = adCoverageVector_.size();
sprintf (s, 
"StoreArea_():  Writing to adCoverageVector_; index = %d, slots = %d", 
folio, vecsiz);
#if DEBUG_AD_COVERAGE_VECTOR_READS_AND_WRITES
	CAlert::InformationAlert (s);
#endif
LOG (s);
	adCoverageVector_[folio] += adWidth * adHeight;
}



//------------------------------------------------------------------------------
//	GetAreaCoveredByAds_
//
//	Tell caller the percentage of the page represented by pgidx is covered 
//	by display ads.  Reads from adCoverageVector_.
//------------------------------------------------------------------------------
PMReal CL_Geometry_Importer::GetAreaCoveredByAds_(int16 pgidx)
{
		ResizeAdCoverageVectorIfNeeded_(pgidx);
		ASSERT (pgidx < adCoverageVector_.size ());
char s[256];
sprintf (s, "GetAreaCoveredByAds_(%d):  reading slot %d  adCoverageVector_ has %d slots", 
pgidx, pgidx, adCoverageVector_.size ());
#if DEBUG_AD_COVERAGE_VECTOR_READS_AND_WRITES
	CAlert::InformationAlert (s);
#endif
LOG (s);

	return adCoverageVector_[pgidx];
}

//------------------------------------------------------------------------------
// Copy candidate ads that were pasted up into the vector of "have been placed" 
// ads.  Copy any ads that weren't pasted up back into the "ads to place" 
// vector so the program will continue to try to paste them up.
//------------------------------------------------------------------------------
void CL_Geometry_Importer::UpdateAdVectorsAfterPasteUp_()
{
	LOG_BEGIN_FUNCTION;

	vector<CL_Display_Ad>::iterator p = adsToPlaceVector_.begin ();
	while (p < adsToPlaceVector_.end ())
	{
#if VERSION == IN_HOUSE
	char logStr[256];
	string str;
	p->GetArtFileName_(str);
	::sprintf (logStr, "<adsToPlaceVector_ ad=\"%s\" ", str.c_str());
#endif

		if (p->GetPlaced_())
		{
			// Copy to placed ads vector.
#if VERSION == IN_HOUSE
			::strcat (logStr, "action=\"Copy to placed ads vector\"/>");
#endif
			placedAdsVector_.push_back (*p);
		}
		else
		{
			// Copy back into input vector.
#if VERSION == IN_HOUSE
			::strcat (logStr, "action=\"Copy back into input vector\"/>");
#endif
			unplacedAdsVector_.push_back (*p);
		}
		++p;
#if VERSION == IN_HOUSE
		LOG (logStr);
#endif
	}
	adsToPlaceVector_.clear ();				// Clean up for next loop

	LOG_END_FUNCTION;
}


/*================================================================================*/
void CL_Geometry_Importer::StoreUnplacedAdsInDocSlug_() {
/*----------------------------------------------------------------------------------
	Abstract:
		If there are unplaced ads, store them in the InDesign document 
		as persistent data by writing their records out to a string and 
		saving that as an IBlackBoxData.  This is our reinvented document 
		slug.  If all ads were placed, write out a doc slug that contains 
		just a null byte.
		TBD:  Suppose they've already imported a geometry file into the 
		current document.  The existing slug will be overwritten.

	Change Log:
		08-Mar-06	New.

	Notes:
		Choose your level of logging:  
		0 for none, 1 for function entry and exit, 2 for value returned by 
		InDesign's ProcessCommand(), 3 for really detailed.
----------------------------------------------------------------------------------*/
#define		StoreUnplacedAdsInDocSlug__LogLevel		3
	LOG_BEGIN_FUNCTION;
#if VERSION == IN_HOUSE && StoreUnplacedAdsInDocSlug__LogLevel == 3
	LOG ("<DumpunplacedAdsVector_>");
	char logstr[256];
	sprintf (logstr, "<!-- Ad vector contains: %d elements. -->", 
				unplacedAdsVector_.size ());
	LOG (logstr);
	for_each (unplacedAdsVector_.begin (), 
				unplacedAdsVector_.end (), dump_ad );
	LOG ("</DumpunplacedAdsVector_>");
#endif
	size_t adCount = unplacedAdsVector_.size ();
	string stlDocSlugStr;
	
	if (adCount > 0)
	{
		// Save CL_Display_Ad objects' input in a string.
		string stlAdInput;
		vector<CL_Display_Ad>::iterator p = unplacedAdsVector_.begin ();
		while (p < unplacedAdsVector_.end ())
		{
			stlDocSlugStr += cStartDelim;
			p->GetInputRecord_(stlAdInput);
			stlDocSlugStr += stlAdInput;
			stlDocSlugStr += cEndDelim;
			++p;
		}

		// Add the string to the document as IBlackBoxData.
		// Create the command.
		InterfacePtr<ICommand> blackboxSetDataCmd (
			CmdUtils::CreateCommand (kBlackBoxAddDataCmdBoss));
		ASSERT (blackboxSetDataCmd);

		if (blackboxSetDataCmd)
		{
			#if StoreUnplacedAdsInDocSlug__LogLevel > 2
				LOG ("CREATED COMMAND");
			#endif
			// Pass the attribute value to be applied.
			InterfacePtr<IBlackBoxCmdData> blackboxData (blackboxSetDataCmd, 
															UseDefaultIID());
			ASSERT (blackboxData);
			if (blackboxData)
			{
				// Need a UIDRef for the document.
// CS2 CODE:	UIDRef docUidRef = ::GetUIDRef(::GetFrontDocument ());
				UIDRef docUidRef = ::GetUIDRef (
									Utils<ILayoutUIUtils>()->GetFrontDocument() );
				if (docUidRef)
				{
					
					// Set the value of the data.
					blackboxData->Set (docUidRef, kNC_PluginID, 
												(void*)stlDocSlugStr.c_str (), 
												stlDocSlugStr.length ());

					#if StoreUnplacedAdsInDocSlug__LogLevel > 2
						LOG ("VALUE FOR DATA SET SUCCESSFULLY");
					#endif

					// Process the command to set the attribute for these objects.
					blackboxSetDataCmd->SetItemList ( UIDList (docUidRef) );
					ErrorCode e = CmdUtils::ProcessCommand (blackboxSetDataCmd);
					#if StoreUnplacedAdsInDocSlug__LogLevel > 1
						DECLARE_logstr;
						::sprintf (logstr, "ProcessCommand() returned %d", e);
									LOG (logstr);
					#endif
					#if StoreUnplacedAdsInDocSlug__LogLevel > 2
						// This is just "double check" code to see if the black 
						// box data really worked.
						InterfacePtr<IBlackBoxData> bbd (
												docUidRef, UseDefaultIID());
						if (bbd)
						{
							::sprintf (logstr, "Data Length = %d", 
												bbd->GetDataLength (kNC_PluginID));
							LOG (logstr);
							char* dataStr = new char 
											[bbd->GetDataLength (kNC_PluginID) + 1];
							bbd->ReadData (kNC_PluginID, (void*)dataStr, 
											bbd->GetDataLength (kNC_PluginID));
							dataStr[bbd->GetDataLength (kNC_PluginID)] = '\0';
							LOG (dataStr);
							delete [] dataStr;
						}
						else
						{
							LOG ("LOOKS LIKE BLACK BOX DATA WASN'T REALLY ADDED.  NUTS.");
						}
					#endif
			 	}
			 	else
			 	{
					#if StoreUnplacedAdsInDocSlug__LogLevel > 2
						LOG ("COULD NOT GET DOCUMENT UIDRef");
					#endif
			 	}
			}
			else
			{
				#if StoreUnplacedAdsInDocSlug__LogLevel > 2
					LOG ("DID NOT GET COMMAND DATA InterfacePtr");
				#endif
			}
		}
		else
		{
			#if StoreUnplacedAdsInDocSlug__LogLevel > 2
				LOG ("COULD NOT CREATE COMMAND");
			#endif
		}
	} // if (adCount > 0)
	else
	{
		// No ads were left over; they've all been placed.
		LOG ("<p>All ads were placed.  None left over.</p>");
	}

	LOG_END_FUNCTION;
}

#pragma mark --- REST OF THE FILE IS COMMENTED OUT ---
// THE REST OF THE FILE IS COMMENTED OUT:
///////////////////////////////////////////////////////////////////////////////
#if 0
//------------------------------------------------------------------------------
void CL_Geometry_Importer::StoreArea_(long adWidth, long adHeight, int16 folio)
//------------------------------------------------------------------------------
{
	// Need to resize?
	int vecsiz = adCoverageVector_.size ();
	if (vecsiz < folio)
	{
		adCoverageVector_.resize (vecsiz * 2, 0L);
	}

	// Store the info
	adCoverageVector_[folio] += adWidth * adHeight;
}


//----------------------------------------------------------------------------------
// Parse_()
// Stores input file contents into display ad objects.
//----------------------------------------------------------------------------------
void CL_Geometry_Importer::Parse_()
{
#define DUMP_PARSED		1

	LOG_BEGIN_FUNCTION;

	enum State {
		initializing, color_pages, master_pages, 
		ad_records, not_newscad_file, exit
	};
	xtErr_ = noErr;
	State state = initializing;

	// Count of chars to process, count of chars processed.
	const int32 dataSize = ::GetHandleSize ((Handle) inputFileContents_);
	int32* pDataSize = new int32;
	*pDataSize = ::GetHandleSize ((Handle) inputFileContents_);
	int32 cntCharsProcessed = 0L;
	string s;														// Buffer

	// Start of big, ugly loop to process the input.
	// Try reading a "record" from memory
	while ((Read_1_Record_(cntCharsProcessed, *pDataSize, s)) && !xtErr_)
	{
		if (s.length () == 0)				// Ignore blank or empty records
			continue;
			
		switch (state) {
			case initializing:
				VerifyFile_(s) ? state = color_pages : state = not_newscad_file;
				break;
			
			case not_newscad_file:
				LOG ("Not a NewsCAD file.");
				xtErr_ = 1;
				break;
				
			case color_pages:
				if (StoreColorPages_(s))
				{
					// After the "CP|nnn|nnn|..." record, we MAY get a master 
					// pages record that looks like "MP|...".  Note that the 
					// presence of a "MP|..." record is not required, so we just 
					// set it as the next state, and handle it there.
					state = master_pages;
				}
				else
				{
					state = not_newscad_file;
				}
				break;
			
			case master_pages:
				// Examine the 1st 2 chars of the input record, see if they're 
				// "MP".  If they are, process this record as a master page 
				// record, otherwise, the record is an ad record.
				if (s[0] == 'M' && s[1] == 'P')
				{
					masterPagesRecFound_ = true;
					masterPageCmd_ = s;
					state = ad_records;
					break;		// Read next record.
				}

				// If we get here, the record is not a "MP" record, it's an 
				// ad record.  Update state and fall thru and process it.
				state = ad_records;
				
			case ad_records:
				{
					// Create ad object from raw (file) data
					CL_Display_Ad adObj (s);
					
					// Store ad object in vector
					unplacedAdsVector_.push_back (adObj);
				}
				break;
			
			case exit:
				break;
			
			default:
				break;
			}

		cntCharsProcessed += s.length ();
		s.resize (0);
	}
	// End of big, ugly loop to process the input.

	delete pDataSize;

#if VERSION == IN_HOUSE && DUMP_PARSED
	LOG ("<DumpunplacedAdsVector_>");
	char logstr[256];
	sprintf (logstr, "<!-- Ad vector contains: %d elements. -->", 
				unplacedAdsVector_.size ());
	LOG (logstr);
	for_each (unplacedAdsVector_.begin (), 
				unplacedAdsVector_.end (), dump_ad );
	LOG ("</DumpunplacedAdsVector_>");
#endif

	LOG_END_FUNCTION;
}


//------------------------------------------------------------------------------
// Read_1_Record_()
// Stores input file contents into display ad objects.
//------------------------------------------------------------------------------
bool CL_Geometry_Importer::Read_1_Record_
					(int32 cntCharsProcessed, const int32 dataSize, string & s)
{
	LOG_BEGIN_FUNCTION;

	// Anything to do?
	if (cntCharsProcessed >= dataSize)
	{
		LOG ("***** END OF FILE *****");
		LOG_END_FUNCTION;
		return false;
	}
	
	char ch =  *(*inputFileContents_ + cntCharsProcessed);
	while ((unsigned char) ch >= (unsigned char) ' ')
	{
		s += ch;
		++cntCharsProcessed;
		ch =  *(*inputFileContents_ + cntCharsProcessed);
	}

	// Append line ending and any junk at the end of the record to the record.
	while ((unsigned char) ch < (unsigned char) ' ' && cntCharsProcessed < dataSize)
	{
		// Convert DOS and UNIX line endings to Macintosh.
		if (ch == 0x0a)
			ch = 0x0d;
		s += ch;
		++cntCharsProcessed;
		ch =  *(*inputFileContents_ + cntCharsProcessed);
	}

	// Append line ending to string that holds the record.  This will 
	// ensure that the the count of chars processed is accumulated correctly.
	// Be sure to handle all possible line endings:
	// Mac = 0x0d, DOS = 0x0d + 0x0a, UNIX = 0x0a.
/*
	if (ch == 0x0d || ch == 0x0a)
	{
		s += ch;
		if (ch == 0x0d)
		{
			ch = *(*inputFileContents_ + cntCharsProcessed + 1);
			if (ch == 0x0a)
			{
				s += ch;
			}
		}
	}
*/
#if VERSION == IN_HOUSE
	string ds = "\tRecord = [";
	ds += s;
	ds += "]";
	const char *cstr = ds.c_str ();
	LOG (cstr);
#endif

	LOG_END_FUNCTION;
	return true;
}





//------------------------------------------------------------------------------
//	ReadAndCheckFirstRecord
//	Tell caller whether this file's a NewsCAD geometry file.  Also tell him how 
//	many chars are in that first record if this is a geometry file.
//------------------------------------------------------------------------------
bool CL_Geometry_Importer::ReadAndCheckFirstRecord_(int32 *countCharsProcessed)
{
	LOG_BEGIN_FUNCTION;
	
	bool	retval = false;
	int32	reclen = 0;
	string	record;
	char 	c = *(*inputFileContents_ + reclen);

	while (c >= ' ')
	{
		record += c;
		++reclen;
		c = *(*inputFileContents_ + reclen);
	}
	
	// We don't count the end-of-record character.  Caller does that in his loop.
	
	string cmpstr = "NewsCAD Geometry File for";
	
	if (record.compare (0, cmpstr.length (), cmpstr) == 0)
	{
		*countCharsProcessed += reclen;
		retval = true;
	}
	
#if VERSION == IN_HOUSE
	const char *cstr = record.c_str ();
	LOG (cstr);
#endif

	LOG_END_FUNCTION;
	
	return retval;
}


//------------------------------------------------------------------------------
//	ReadAndStoreColorPages
//	Tell caller whether this record's a NewsCAD color pages record.  Also tell 
//	him how many chars are in that record if it is indeed a color pages record.
//------------------------------------------------------------------------------
bool CL_Geometry_Importer::ReadAndStoreColorPages_(int32 *countCharsProcessed)
{
	LOG_BEGIN_FUNCTION;

	enum State {initializing, color_pages, finished};
	State	state = initializing;
	bool	retval = false;
	int32	cnt = *countCharsProcessed;
	char 	c = *(*inputFileContents_ + cnt);
	string	currentToken;

	while (state != finished)
	{
		switch (state) {
		case initializing:
			if (c != '|')
			{
				currentToken += c;
			}
			else
			{
				// End of token.
				#if VERSION == IN_HOUSE
					string s = "\tToken = ";
					s += currentToken;
					const char *cstr = s.c_str ();
					LOG (cstr);
				#endif
				if (currentToken == "CP")
				{
					state = color_pages;
				}
				else
				{
					state = finished;
					LOG ("Color pages token isn't formatted correctly.");
				}
				currentToken.resize (0);
			}
			break;
		
		case color_pages:
			if (c == '|')					// End of token
			{
				ProcessColorPageToken_(currentToken);
			}
			else if (c < ' ')				// End of token and record
			{
				ProcessColorPageToken_(currentToken);
				state = finished;
				retval = true;
			}
			else
			{
				if (isdigit (c))
				{
					currentToken += c;
				}
			} 			
			break;
		
		case finished:						// Never gets here.
			break;
		
		default:
			break;
		}
		
		++cnt;
		c = *(*inputFileContents_ + cnt);
	}

	// We don't count the end-of-record character.  Caller does that in his loop.
	*countCharsProcessed = cnt - 1;

	LOG_END_FUNCTION;
	
	return retval;
}


//----------------------------------------------------------------------------------
// ProcessMasterPagesRecord_()
// 
//----------------------------------------------------------------------------------
bool CL_Geometry_Importer::ProcessMasterPagesRecord_()
{
	LOG_BEGIN_FUNCTION;

	bool		isInputValid = true;
	int			separatorIndex, nextSeparatorIndex;			// Index of '|' char
	int			commaIndex;									// Index of ','
	unsigned	pieceCount;				// Track the count of substrings found 
										// between pipe symbols, e.g., |x| or |x,y|
	string		piece;					// Data between the 2 pipes
	string		token;					// Data split into substrings between pipes
	int			inputLen = masterPageCmd_.length ();

	pieceCount = 0;
	separatorIndex = masterPageCmd_.find ('|');						// Priming read
	while (1)
	{
		nextSeparatorIndex = masterPageCmd_.find ('|', separatorIndex + 1);
		if (nextSeparatorIndex != -1)
		{
			++pieceCount;
			piece.assign (masterPageCmd_, separatorIndex + 1, 
							nextSeparatorIndex - separatorIndex - 1);
			if (pieceCount == 1)
			{
				// This is the basic master page's name -- e.g., |A-Master A|
				basicMasterPageName_ = piece;
			}
			else
			{
				// Try to split the token into a master page name 
				// and count to insert, e.g., |A-Master A,3|
				commaIndex = piece.find (',');
				if (commaIndex == -1)
				{
					// Required char not found, input illegal
					isInputValid = false;
					break;
				}
				
				// Get the master page name
				token.assign (piece, 0, commaIndex);
				masterPageCmdTokens_.push_back (token);
				
				// Get the count to create in the document
				token.assign (piece, commaIndex + 1, 
								piece.length () - commaIndex - 1);
				masterPageCmdTokens_.push_back (token);
			}
		}
		else
		{
			break;
		}
		
		// Move index to end of this piece (start of next piece)
		separatorIndex = nextSeparatorIndex;
	}
	
	// If input valid, create pages in document.  Generally, we do this by 
	// appending all the pages specified by the MP command, then removing the 
	// original document page(s).  But if this document has a NewsCAD slug, 
	// which means this is not the first time a NewsCAD file has been imported, 
	// we DO NOT delete the document's original pages.  This prevents accidental 
	// deletion of a user's hard work by a subsequent "Import Ad Geometry" op.
	if (isInputValid)
	{
		int16 origPageCount;						// Store original doc state
		CL_Document* doc = new CL_Document;
		doc->GetPageCount_(&origPageCount);
		LOG ("Basic master page name:");
		LOG (basicMasterPageName_.c_str ());
		LOG ("Master Page Tokens vector:");
		vector<string>::iterator ptok = masterPageCmdTokens_.begin ();
		pieceCount = 0;
		char* masterName;
		char* countStr;
		while (ptok < masterPageCmdTokens_.end ())
		{
			++pieceCount;
			if (pieceCount & 1)
			{
				// Odd means master page name
				masterName = (char*) ptok->c_str();
			}
			else
			{
				// Even means count of pages to create
				countStr = (char*) ptok->c_str();
				doc->AppendPages_((const char*) masterName, (const char*) countStr);
			}
			++ptok;
		}
		
		if (!doc->HasNewsCADDocSlug_())
		{
			// This is the first time we've imported a NewsCAD geometry file.  
			// Our master pages were appended, so we don't need the original pages.
			doc->DeletePages_(1, origPageCount);
		}

		delete doc;
	}

	LOG_END_FUNCTION;

	return isInputValid;
}


void dump_ad (CL_Display_Ad ad)
{
	ad.Log_();
}


void dump_ad_input_rec (CL_Display_Ad ad)
{
	ad.LogInputRec_();
}


//------------------------------------------------------------------------------
// Go thru all the ads in the unplacedAdsVector_ and adjust their widths 
// based on the width of a text column in the designated master page.  
// Their current widths were computed during by their CTOR, which doesn't 
// know anything about master pages.
//------------------------------------------------------------------------------
void CL_Geometry_Importer::RecomputeDisplayAdWidths_()
{
	LOG_BEGIN_FUNCTION;

	Fixed pageColumnWidth;
	Fixed pageGutterWidth;
	int pageColumnCount;
	
	CL_Document* pdoc = new CL_Document;

	// Find the index of the "basic" master page.
	int16 basicMasterIndex = 
				pdoc->GetMasterPageIndexFromName_(basicMasterPageName_.c_str() );
	
	if (basicMasterIndex)
	{
		// Go to that master page.  Caveats: XTGotoMaster considers the first user 
		// master "A-Master A" to be the first master spread rather than the third.  
		// You cannot specify the two default master spreads.
		basicMasterIndex -= 2;
		if (basicMasterIndex >= 1)		// Should be in range
		{
			APIERR apiErr = XTGotoMaster (pdoc->DocRef_(), basicMasterIndex);
			if (!apiErr)
			{
				CL_Page* pg = new CL_Page;
				pg->StorePageInfo_();
				pageColumnWidth = pg->GetColumnWidth_();
				pageColumnCount = pg->GetColumnCount_();
				pageGutterWidth = pg->GetGutterWidth_();
				delete pg;
				
				// Return to document.
				XTGotoMaster (pdoc->DocRef_(), 0);

				// Loop over the ads and notify each one to reset its width.
				vector<CL_Display_Ad>::iterator p = unplacedAdsVector_.begin ();
				while (p < unplacedAdsVector_.end ())
				{
					p->SetWidth_(pageColumnWidth, pageGutterWidth);
					++p;
				}
			}
		}
	}
/*	

	while (p < unplacedAdsVector_.end ())
	{
		long cols;

		// Get number of columns spanned by this ad, store in member var
		cols_ = cols = ::atol (pInStr);
		
		// Compute total column width spanned
		fTemp = FixMul (FIX (cols), pageColumnWidth);
		
		// Compute number of gutters spanned; accumulate gutter width
		--cols;
		if (cols)
			fTemp += FixMul (FIX (cols), pageGutterWidth);
		width_ = fTemp;
		++p;
	}
*/			
	delete pdoc;

	LOG_END_FUNCTION;
}


void CL_Geometry_Importer::LogAdVectorStatus_()
{
#if VERSION == IN_HOUSE
	char s[256];
	sprintf (s, "--------------------");
	LOG (s);

//	char logstr[256];
	sprintf (s, "Placed ads vector contains: %d elements.", 
				placedAdsVector_.size ());
	LOG (s);
	sprintf (s, "Unplaced ads vector contains: %d elements.", 
				unplacedAdsVector_.size ());
	LOG (s);
#endif	
}

//------------------------------------------------------------------------------
// Loop over the ads and place those that must go on specified folios
//------------------------------------------------------------------------------
void CL_Geometry_Importer::PlaceAdsOnSpecifiedPages_()
{
	LOG_BEGIN_FUNCTION;
	
	CL_Document* pCurDoc = gpNewsCAD_XT->GetCurrentDocumentObject_();
	
	// Fake out the logic that might prevent absolute placement by 
	// setting the threshhold to 100%
	adCoverageThreshhold_ = 65536;				// 100% in Fixed notation

	vector<CL_Display_Ad>::iterator p = unplacedAdsVector_.begin ();
	int16 adPageNumber;
	int totalRects = 0, noToPlace = 0;
	while (p < unplacedAdsVector_.end ())
	{
		adPageNumber = p->GetAbsolutePageNumber_();
		if (adPageNumber)
		{
			noToPlace = 1;
#if CLIENT == CAPE_GAZETTE || CLIENT == THRIFTY_NICKEL
				CL_Page *pageObj = new CL_Page (this);
#elif CLIENT == REMINDER
				RM_Page *pageObj = new RM_Page (this);
#elif CLIENT == HEMMINGS
			HM_Page *pageObj = new HM_Page (this);
#endif
			pageObj->SetFolio_(adPageNumber);
//			if (pageObj->GoToFolio_())
			if ( pCurDoc->GoToPage_(adPageNumber) )
			{
				pageObj->SetIsColorPage_(IsColorPage_(adPageNumber));
				// TBD:  Don't think these next 2 lines are necessary, but need to test.
				xecalc (TRUE);
//OBSOLETE		pageObj->RedrawPage_();
				pCurDoc->RedrawPage_(adPageNumber);
				pageObj->SetupForAdPlacement_();
				totalRects = pageObj->FindOpenRects_();
				if (totalRects > 0)
				{
	#if VERSION == IN_HOUSE
					p->LogNameAndPage_();
	#endif
					// Copy ad into vector of ads to place.  We're placing 
					// only 1 ad, but PasteUp_ requires a reference to a vector.
					adsToPlaceVector_.push_back (*p);
					
					// Makeup the page.
					pageObj->PasteUp_(adsToPlaceVector_);

					// Set status of ad
					p->SetPlaced_(adsToPlaceVector_[0].GetPlaced_());
					adsToPlaceVector_.clear ();
				}
			}

			// If the ad was successfully placed (and why wouldn't 
			// it be?), move (cut/paste) it to the placed ads vector.
			if (p->GetPlaced_())
			{
				// Copy to placed ads vector.	
				placedAdsVector_.push_back (*p);
				// Delete it from our input vector.  p will point to the 
				// element after this one after erasing.
				unplacedAdsVector_.erase (p);
			}
			else
			{	// Advance to next ad
				++p;
			}

			delete pageObj;
		} // end if ad has absolute page
		else
		{
			++p;								// Look at the next element
		}
	}
	// end of absolute folio ad placement (big, ugly while loop)
	LOG_END_FUNCTION;
}


//------------------------------------------------------------------------------
// Loop over the pages and check those ads with a lower and upper limit and 
// range of LESS THAN 6 PAGES.  If the page number of the page the program 
// is currently operating on is inside this range, copy the ad into the 
// array of ads to place.
//------------------------------------------------------------------------------
void CL_Geometry_Importer::SelectAdsWithLessThan6PageRange (int16 inCurFolio)
{
	LOG_BEGIN_FUNCTION;

	vector<CL_Display_Ad>::iterator p = unplacedAdsVector_.begin ();
	while (p < unplacedAdsVector_.end ())
	{
		if ( p->GetAbsolutePageNumber_() == 0				&& 
		   ((p->GetUpperPage_() - p->GetLowerPage_()) < 6)	&& 
			 p->GetLowerPage_() <= inCurFolio				&& 
			 p->GetUpperPage_() >= inCurFolio				&&
			(p->GetOddPageFlag_() == 0 || inCurFolio % 2 != 0) )
		{
			// Found an ad that qualifies for placement on the current page
	#if VERSION == IN_HOUSE
			p->LogNameAndPageLimits_();
	#endif
			// Move to "ads to place" by copy/delete from input vector.
			// p will point to the element after this one after erasing.
			adsToPlaceVector_.push_back (*p);
			unplacedAdsVector_.erase (p);
		}
		++p;							// And look at the next ad
	} // End for each AD in the file

	LOG_END_FUNCTION;
}


//------------------------------------------------------------------------------
// Loop over the pages and check those ads with a lower and upper limit.  
// The range is not up for consideration.  This will select any ad that 
// does not have to go on a specified page, and copies the ad into the 
// array of ads to place.
//------------------------------------------------------------------------------
void CL_Geometry_Importer::SelectAdsWithAnyPageRange (int16 inCurFolio)
{
	LOG_BEGIN_FUNCTION;

#if VERSION == IN_HOUSE
	char logStr[256];
	sprintf (logStr, "unplacedAdsVector_ contains: %d elements.", unplacedAdsVector_.size ());
	LOG (logStr);
	string str;
#endif

	vector<CL_Display_Ad>::iterator p = unplacedAdsVector_.begin ();
	while (p < unplacedAdsVector_.end ())
	{
#if VERSION == IN_HOUSE
/* old stuff:
	uchar tmp[256];
	ptocstrcpy (tmp, p->GetName_());
*/
	p->GetArtFileName_(str);
	::sprintf ( logStr, "adsToPlaceVector_ ad: [%s]", str.c_str() );
	LOG (logStr);
#endif

		if ( p->GetAbsolutePageNumber_() == 0				&& 
			 p->GetLowerPage_() <= inCurFolio				&& 
			 p->GetUpperPage_() >= inCurFolio				&&
			(p->GetOddPageFlag_() == 0 || inCurFolio % 2 != 0) )
		{
			// Found an ad that qualifies for placement on the current page
#if VERSION == IN_HOUSE
			p->LogNameAndPageLimits_();
#endif
			// Move to "ads to place" by copy/delete from input vector.
			// p will point to the element after this one after erasing, 
			// so don't increment the index.
			adsToPlaceVector_.push_back (*p);
			unplacedAdsVector_.erase (p);
		}
		else
		{
			++p;							// And look at the next ad
		}
	} // End for each AD in the file

	LOG_END_FUNCTION;
}


#pragma mark •• INDESIGN-SPECIFIC

/* ----------------------------- InDesign-Specific ------------------------------ */


//#include "DocumentSlug.h"
#endif
///////////////////////////////////////////////////////////////////////////////

// END OF FILE


/*
void CL_Geometry_Importer::StoreUnplacedAdsInDocSlug_()
{
	size_t adCount = unplacedAdsVector_.size ();
	if (adCount > 0)
	{
		// Save vector of CL_Display_Ad objects.
		size_t slugByteCount = sizeof (DocumentSlug) + (adCount * sizeof (CL_Display_Ad));
		DocumentSlug** h = (DocumentSlug**)(::NewHandle (slugByteCount));
		if (h)
		{
			HLock ( Handle (h) );
			(*h)->countAdsToPlace = adCount;
			vector<CL_Display_Ad>::iterator q = unplacedAdsVector_.begin ();
			int i = 0;
			CL_Display_Ad *p = (*h)->adsToPlace;
			while (q < unplacedAdsVector_.end ())
			{
				*p = *q;
				++p;
				++q;
			}
			HUnlock ( Handle (h) );
			setslug (NULL, DOCSLUG, 'ANZ1', Handle (h) );
			verydirty ();
			::DisposeHandle (Handle (h) );
		}
	}
}
*/
