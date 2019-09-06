/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	CL_NewsCAD_Prefs.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	1/9/11 ca. 1:00 PM
	
	ab	Abstract:
	ab	Implementation of CL_NewsCAD_Prefs class.
	ab	
	
	cl	Change Log:
	cl	1/9/11	New.
----------------------------------------------------------------------------------*/
#include "newscad.h"
#include "CL_NewsCAD_Prefs.h"
#include "parser.h"
#include "CL_Log_File.h"
#include "global_data.h"
#include "FileUtils.h"

// This source was developed as a C++ tool so it could be debugged outside the 
// InDesign plugin framework.  Set compiler options here for handling error and 
// information messages.
#ifndef	INDESIGN_PLUGIN
#define	INDESIGN_PLUGIN					1
#endif
#ifndef	UNIX_COMMAND_LINE_PROGRAM
#define	UNIX_COMMAND_LINE_PROGRAM		2
// Use iostreams to display the messages on the console.
#endif

// Now set the product to one of INDESIGN_PLUGIN, UNIX_COMMAND_LINE_PROGRAM, or 0.
#ifndef HNM_PRODUCT
#define	HNM_PRODUCT			INDESIGN_PLUGIN
#endif

#if HNM_PRODUCT == INDESIGN_PLUGIN
// For plugins, use the Adobe alert to display messages.
#include "CAlert.h"
#endif

using namespace std;

#define		DEFAULT_RULE_WEIGHT_STR		"1 pt"
#define		DEFAULT_AD_COVERAGE_STR		"55%"

void XML_Service_Routine (const char* inXPath, 
		const char* inXMLElem, 
		const TElementValue & inVal)
{
	gpPrefsObj->SetPreference_(inXPath, inXMLElem, inVal);
}


//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and D E S T R U C T I O N
//----------------------------------------------------------------------------------
CL_NewsCAD_Prefs::CL_NewsCAD_Prefs ()					// Default CTOR
	:	prefsFileName_(NULL)
	,	pathToArtFiles_(NULL)
	,	ruleWeightCStr_(NULL)
	,	grayOutline_(true)
	,	adCoveragePctCStr_(NULL)
{

	ruleWeightCStr_ = new char [::strlen (DEFAULT_RULE_WEIGHT_STR) + 1];
	::strcpy (ruleWeightCStr_, DEFAULT_RULE_WEIGHT_STR);
	
	adCoveragePctCStr_ = new char [::strlen (DEFAULT_AD_COVERAGE_STR) + 1];
	::strcpy (adCoveragePctCStr_, DEFAULT_AD_COVERAGE_STR);
}

CL_NewsCAD_Prefs::~CL_NewsCAD_Prefs ()					// DTOR
{
	delete [] prefsFileName_;
	delete [] ruleWeightCStr_;
	delete [] adCoveragePctCStr_;
}

//------------------------------------------------------------------------------
//	LoadPreferencesFromFile_(char* fileName)
//
//	Set member variable for preferences file name, then call the load method 
//	to get the actual work done.
//------------------------------------------------------------------------------
void CL_NewsCAD_Prefs::LoadPreferencesFromFile_(char* inFileName)
{
	size_t len = ::strlen (inFileName) + 1;
	prefsFileName_ = new char [len];
	::strcpy (prefsFileName_, inFileName);
	LoadPreferences_();
}


/*================================================================================*/
void CL_NewsCAD_Prefs::LoadPreferences_() {
/*----------------------------------------------------------------------------------
	Abstract:
		
	Parameters and modes:

	Returns:

	Change Log:
		09-Jan-11	New.
----------------------------------------------------------------------------------*/
	OSStatus osStatus = ComputeFullPathToPreferencesFile_();
	if (osStatus == noErr)
	{
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
		cout << "Prefs file full path = " << prefsFileFullPath_ << endl;
#endif
		if (DoesPrefsFileExistOnDisk_())
		{
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
			cout << "Preferences file exists.  Read it in and store values " <<
				"in our member variables." << endl;
#endif
			TSourceBuffer* srcbuf = new TSourceBuffer (prefsFileFullPath_.c_str());
 			//TParser parser (srcbuf, this, &CL_NewsCAD_Prefs::SetPreference_);
 			TParser parser (srcbuf, XML_Service_Routine);
			
 			// Read in the file, convert values to internal representation, and store.
 			// This will repeatedly call this->SetPreference_() to send us the values.
			parser.Parse ();

			// Returning this object to free store closes the input file and 
			// cleans up associated memory.
			delete srcbuf;
			
// 			CFXMLTreeRef    cfXMLTree;
// 			CFDataRef       xmlData;
// 			CFURLRef		sourceURL;
// 			
// 			// Load the XML data using its URL.
// 			CFStringRef filePath = CFStringCreateWithCString (
// 				kCFAllocatorDefault,
// 				(const char *) prefsFileFullPath_,
// 				kCFStringEncodingMacRoman
// 			);
// 
// 			sourceURL = CFURLCreateWithFileSystemPath (kCFAllocatorDefault,
// 				filePath, kCFURLPOSIXPathStyle, false);
// 
// 			CFURLCreateDataAndPropertiesFromResource (kCFAllocatorDefault,
//             	sourceURL, &xmlData, NULL, NULL, NULL);
// 
// 			// Parse the XML and get the CFXMLTree.
// 			cfXMLTree = CFXMLTreeCreateFromData (kCFAllocatorDefault,
// 							xmlData, sourceURL, 
// 							kCFXMLParserSkipWhitespace, kCFXMLNodeCurrentVersion);
// 
// 			CFXMLTreeRef    xmlTreeNode;
// 			CFXMLNodeRef    xmlNode;
// 			int             childCount;
// 			int             index;
// 			
// 			// Get a count of the top level node’s children.
// 			childCount = CFTreeGetChildCount(cfXMLTree);
// 			
// 			// Print the data string for each top-level node.
// 			cout << "DUMP OF XML TREE:" << endl;
// 			for (index = 0; index < childCount; index++) {
// 				xmlTreeNode = CFTreeGetChildAtIndex(cfXMLTree, index);
// 				xmlNode = CFXMLTreeGetNode(xmlTreeNode);
// 				CFShow(CFXMLNodeGetString(xmlNode));
// 			} 			
		}
		else
		{
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
			cout << "Preferences file does not exist.  Create one with defaults."
				<< endl;
#endif
			// The CTOR initializes our members to default values.  All we need to 
			// do at this point is to write them out to the preferences file.
			StorePreferencesInFile_(prefsFileFullPath_.c_str());
		}
	}
}

//------------------------------------------------------------------------------
//	DoesPrefsFileExistOnDisk_
//
//	Tell caller whether the file at prefsFileFullPath_ exists on disk.
//------------------------------------------------------------------------------
bool CL_NewsCAD_Prefs::DoesPrefsFileExistOnDisk_()
{
	bool retval = false;
	prefsFile_.open (prefsFileFullPath_, fstream::in);
	if (prefsFile_.is_open())
	{
		prefsFile_.close ();
		retval = true;
	}
	
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
	cout << "End of DoesPrefsFileExistOnDisk_.  prefsFileFullPath_ = " 
		<< prefsFileFullPath_ << endl;
#endif

	return retval;
}


//----------------------------------------------------------------------------------
//	ComputeFullPathToPreferencesFile_
//----------------------------------------------------------------------------------
OSStatus CL_NewsCAD_Prefs::ComputeFullPathToPreferencesFile_()
{
	FSRef		ref;
	OSStatus	err = kSuccess;
	
	// Get a MacOS X file system reference to the current user's Documents folder.

    IDFile oPrefFile;
    PMString strPrefFile;
    FileUtils::GetAppDocumentFolder(&oPrefFile, "");
    FileUtils::IDFileToPMString(oPrefFile, strPrefFile, kTrue);
    strPrefFile.Append("/");
    strPrefFile.Append(prefsFileName_);
    prefsFileFullPath_ = strPrefFile.GetPlatformString();

/*
	err = FSFindFolder ( kUserDomain, kDocumentsFolderType, 
							kDontCreateFolder, &ref );
	if (!err)
	{
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
		cout << "FSFindFolder SUCCEEDED for ~/User/Documents" << endl;
#endif
		UInt8 path[256];
		
		// Convert the file system ref to a POSIX path.
		err = FSRefMakePath ( &ref, (UInt8 *)path, 256 );
		if (!err)
		{
			//printf ("SUCCESS!  POSIX path = %s\n", (char*) path);
			// Add the preferences filename to the path.
			strcpy (prefsFileFullPath_, (const char *)path);
			strcat (prefsFileFullPath_, "/");
			strcat (prefsFileFullPath_, prefsFileName_);
		}
		else
		{
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
			cout << "FSRefMakePath FAILED err = " << err << endl;
#endif
		}
	}
	else
	{
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
		cout << "FSFindFolder FAILED err = " << err << endl;
#endif
	}
*/
	return err;
}


/*================================================================================*/
void CL_NewsCAD_Prefs::StorePreferencesDialogInput_(char* inFileName) {
/*----------------------------------------------------------------------------------
	Abstract:
		This function will cause the values in the preferences object (this) to be 
		written to the file passed in fileName.  The caller (in this case, the 
		dialog's OK button handler) is responsible for storing in the object the 
		values it wants written to the file before calling this function.
		
	Parameters and modes:
		fileName	Name of the XML file to store the values.			IN
		
	Returns:
		Nothing.
		
	Change Log:
		04-Sep-11	New.
----------------------------------------------------------------------------------*/
	size_t len = ::strlen (inFileName) + 1;
	prefsFileName_ = new char [len];
	::strcpy (prefsFileName_, inFileName);

	OSStatus osStatus = ComputeFullPathToPreferencesFile_();
	if (osStatus == noErr)
	{
		StorePreferencesInFile_(prefsFileFullPath_.c_str());
	}
}


//----------------------------------------------------------------------------------
//	StorePreferencesInFile_
//
//	Persist this object by writing its members out to the preferences file.
//----------------------------------------------------------------------------------
void CL_NewsCAD_Prefs::StorePreferencesInFile_(const char* inPrefsPath)
{
	LOG_BEGIN_FUNCTION;
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
	cout << "CALLED:  StorePreferencesInFile_(" << inPrefsPath << ")" << endl;
#endif
	prefsFile_.open (prefsFileFullPath_, fstream::out);
	if (prefsFile_.is_open ())
	{
		prefsFile_ << "<?xml version=\"1.0\" ?>" << endl;	// XML declaration
		prefsFile_ << "<NewsCADPreferences>" << endl;		// Root element
		prefsFile_ << "\t<GraphicsPath>";					// Path to art files
		if (pathToArtFiles_)
		{
			prefsFile_ << pathToArtFiles_;
		}
		prefsFile_ << "</GraphicsPath>" << endl;
		
		prefsFile_ << "\t<RuleWeight>" << ruleWeightCStr_ 	// Rule weight
			<< "</RuleWeight>" << endl;
															// Gray outline
		prefsFile_ << "\t<GrayOutline>" << grayOutline_ << 
			"</GrayOutline>" << endl;
															// Pct of page for ads
		prefsFile_ << "\t<AdCoverage>" << adCoveragePctCStr_ 
			<< "</AdCoverage>" << endl;
		prefsFile_ << "</NewsCADPreferences>" << endl;

		prefsFile_.close();
	}
	else
	{
		GiveAlert_("CL_NewsCAD_Prefs::StorePreferencesInFile_ FAILED:  Could not open prefs file for output.", true);
	}
	LOG_END_FUNCTION;
}


/*================================================================================*/
void CL_NewsCAD_Prefs::SetPreference_(
				const char* inPath, const char* inElem, const TElementValue & inVal) {
/*----------------------------------------------------------------------------------
	Abstract:
		This method is called by TParser::ProcessTagValue_ (during parsing, obviously) 
		when it detects the end of a tag and it has computed the element's value.  
		Next step is to store a value for a preference in this NewsCAD_Prefs object.  
		This method is really just a traffic director -- it identifies in which struct 
		to store the value, then calls a function that can deal with that object.
	
	Parameters and modes:
		inPath	The XPath to the element, minus the root element 				IN
				and the element name (so it's a partial path).  So, if 
				we're processing "~/xmath_preferences/characters/cap_greek", 
				this value would be "characters".
		inElem	The element name.  In the example above, "cap_greek".			IN
		inVal	A reference to a struct that represents the value. 				IN
				In XML parlance, the value of the element.

	Returns:
		i don't know yet
		
	Change Log:
		17-Jan-11	Port to CS4.
		30-Aug-05	Porting to InDesign.  Improved logging.
		22-Jan-04	New.
----------------------------------------------------------------------------------*/
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
	cout << "REACHED CL_NewsCAD_Prefs::SetPreference_() !!" << endl;
#endif
	char* pval = NULL;
	char ch = 0;
	
	switch (inVal.dataType_) {
		case tyString:
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
			cout << "Data type = tyString" << endl;
#endif
			pval = inVal.value_.pString;
			break;
		case tyCharacter:
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
			cout << "Data type = tyCharacter" << endl;
#endif
			ch = inVal.value_.character;
			break;
		default:
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
			cout << "BAD NEWS:  UNKNOWN DATA TYPE." << endl;
#endif
			break;
	}

	// New value is either a string or a character.
	unsigned newValueLen = pval ? ::strlen (pval) : 1;
	unsigned oldValueLen;
	if (::strcmp (inElem, "GraphicsPath") == 0)
	{
		if (!pathToArtFiles_)
		{
			pathToArtFiles_ = new char [newValueLen + 1];
			oldValueLen = newValueLen;
		}
		else
		{
			oldValueLen = ::strlen (pathToArtFiles_);
		}
		if (pval)
		{
			// Room enough to store the new value at the same address as the old?
			if (newValueLen > oldValueLen)
			{
				// Release the old block and allocate a new one.
				delete [] pathToArtFiles_;
				pathToArtFiles_ = new char [newValueLen + 1];
			}
			::strcpy (pathToArtFiles_, pval);
		}
		else if (ch)
		{
			// Certainly not expecting this, but possible I guess.  If the program 
			// gets here, it's certain that memory has already been allocated and 
			// that the block's at least 2 bytes long.
			*pathToArtFiles_ = ch;
			*(pathToArtFiles_ + 1) = '\0';
		}
		else
		{
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
			cout << "THIS SHOULD NOT HAPPEN" << endl;			// Can't happen.
#endif
		}
	}
	else if (::strcmp (inElem, "RuleWeight") == 0)
	{
		SetRuleWeightCStr_(pval);
	}
	else if (::strcmp (inElem, "GrayOutline") == 0)
	{
		SetGrayOutline_((ch == '1') ? true : false);
	}
	else if (::strcmp (inElem, "AdCoverage") == 0)
	{
		SetAdCoveragePercentCStr_(pval);
	}
	else
	{
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
		cout << "UNRECOGNIZED ELEMENT NAME" << endl;	// Unrecognized element name -- 
														// just ignore for now.
#endif
	}
}


#pragma mark SETTERS
// ------------------------------  S E T T E R S  ------------------------------

void CL_NewsCAD_Prefs::SetGraphicsPath_(char* inCString)
{
	LOG_BEGIN_FUNCTION;

	// If nothing passed in, clean up memory used by our object and return.
	if (!inCString)
	{
		delete [] pathToArtFiles_;
		pathToArtFiles_ = NULL;
/*		LOG ("<p>Suspicious:  NULL pointer for graphics path.</p>"); */
#if HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
		cout << "NULL pointer for graphics path." << endl;
#endif
		LOG_END_FUNCTION;
		return;
	}
	
	// If we get this far, inCString contains something.
	unsigned newValueLen = ::strlen (inCString);
	unsigned oldValueLen;
	if (!pathToArtFiles_)
	{
		pathToArtFiles_ = new char [newValueLen + 1];
		oldValueLen = newValueLen;
	}
	else
	{
		oldValueLen = ::strlen (pathToArtFiles_);
	}

	// Room enough to store the new value at the same address as the old?
	if (newValueLen > oldValueLen)
	{
		// Release the old block and allocate a new one.
		delete [] pathToArtFiles_;
		pathToArtFiles_ = new char [newValueLen + 1];
	}
	::strcpy (pathToArtFiles_, inCString);

 	LOG_END_FUNCTION;
}


void CL_NewsCAD_Prefs::SetRuleWeightCStr_(char* s)
{
	LOG_BEGIN_FUNCTION;
	// A block for rule weight was allocated by the CTOR.
	unsigned oldValueLen = ::strlen (ruleWeightCStr_);
	unsigned newValueLen = ::strlen (s);
	if (newValueLen > oldValueLen)
	{
		// Release the old block and allocate a new one.
		delete [] ruleWeightCStr_;
		ruleWeightCStr_ = new char [newValueLen + 1];
	}
	::strcpy (ruleWeightCStr_, s);
 	LOG_END_FUNCTION;
}


void CL_NewsCAD_Prefs::SetAdCoveragePercentCStr_(char* s)
{
	LOG_BEGIN_FUNCTION;
	// A block for ad coverage was allocated by the CTOR.
	unsigned oldValueLen = ::strlen (adCoveragePctCStr_);
	unsigned newValueLen = ::strlen (s);
	if (newValueLen > oldValueLen)
	{
		// Release the old block and allocate a new one.
		delete [] adCoveragePctCStr_;
		adCoveragePctCStr_ = new char [newValueLen + 1];
	}
	::strcpy (adCoveragePctCStr_, s);
 	LOG_END_FUNCTION;
}

#pragma mark GETTERS
// ------------------------------  G E T T E R S  ------------------------------

void CL_NewsCAD_Prefs::GetAdCoveragePercent_(char* str)
{
	LOG_BEGIN_FUNCTION;
	::strcpy (str, adCoveragePctCStr_);
 	LOG_END_FUNCTION;
}

void CL_NewsCAD_Prefs::GetRuleWeightStr_(char *str)
{
	LOG_BEGIN_FUNCTION;
	::strcpy (str, ruleWeightCStr_);
 	LOG_END_FUNCTION;
}

void CL_NewsCAD_Prefs::GetGraphicsPath_(char *str)
{
	LOG_BEGIN_FUNCTION;
	if (pathToArtFiles_)
	{
		::strcpy (str, pathToArtFiles_);
	}
	else
	{
		*str = 0;						// Send back an empty Pascal string
	}
 	LOG_END_FUNCTION;
}

#pragma mark -

void CL_NewsCAD_Prefs::GiveAlert_(char* inMsg, bool inIsError /*= false*/) {
#if HNM_PRODUCT == INDESIGN_PLUGIN
	CAlert::ModalAlert
	(
		inMsg,
		kOKString, 					// OK button
		kNullString, 				// No second button
		kNullString, 				// No third button
		1,							// Set OK button to default
		inIsError ? CAlert::eErrorIcon : CAlert::eInformationIcon
	);
#elif HNM_PRODUCT == UNIX_COMMAND_LINE_PROGRAM
	if (inIsError)
		cout << "Error:  ";
	else
		cout << "Info:  ";
	cout << inMsg << endl;
#else
	// This function compiles to nothing.
#endif
}

// END OF FILE
