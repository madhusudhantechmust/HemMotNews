/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	HM_TableOfContents.CPP
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	3/19/07 ca. 8:58 PM
	
	ab	Abstract:
	ab	Implementation of HM_TableOfContents class.
	ab	
	
	cl	Change Log:
	cl	3/19/07	New.
----------------------------------------------------------------------------------*/
using namespace std;
#include <cstring>

#include "ITextModel.h"
#include "IAttributeStrand.h"
#include "IDocument.h"
#include "IStoryList.h"
#include "IStyleInfo.h"
#include "IFrameList.h"
#include "IHierarchy.h"
#include "IMasterPage.h"		// For enhanced logging
#include "IPageList.h"			// ditto
#include "TextIterator.h"
#include "IPasteboardUtils.h"
#include "ISpread.h"
#include "IFrameListComposer.h"
#include "CAlert.h"
#include "WFPID.h"
#include "TextStoryAccessTypes.h"
#include "SDKFileHelper.h"
#include "ILayoutUIUtils.h"
#include "ImportGeomID.h"
#include "FileUtils.h"

#include "HM_ClassifiedHeading.h"
#include "HM_TableOfContents.h"
#include "CL_Log_File.h"
#include "global_data.h"

//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and D E S T R U C T I O N
//----------------------------------------------------------------------------------
HM_TableOfContents::HM_TableOfContents ()
	:	okToProcess_(false)
	,docPageCount_(-1)
{

}

HM_TableOfContents::~HM_TableOfContents ()
{

}


void HM_TableOfContents::Run_()
{
	MakeContinuedLines_();
return;
	OpenOutputFile_();
	if (okToProcess_)
	{
		WriteXPressTagsFileInitialization_();
		ScanDocumentText_();
		CloseOutputFile_();
	}
}


void HM_TableOfContents::WriteXPressTagsFileInitialization_()
{
	PMString xpressTagsInitString ("<v2.05><e0>");
	xpressTagsInitString += "\r";
	xpressTagsInitString += "@TOC:";
    m_oTOCFile << xpressTagsInitString.GetPlatformString();
/*
    const char* s = xpressTagsInitString.GetPlatformString ().c_str();
	long byteCount = ::strlen (s);
	::FSWrite ( tocFileRefNum_, &byteCount, s );
*/
}

/*================================================================================*/
void HM_TableOfContents::ScanDocumentText_() throw (char*) {
/*----------------------------------------------------------------------------------
	Abstract:
		Walk through all the text in the file.  Look for classification 
		headings (marked up by the "Head1" stylesheet).  Compute the corresponding 
		folio for the page on which each appears.
		
	Parameters and modes:

	Returns:

	Change Log:
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	try {
		// Iterate over all the stories in the document.  NewsCAD works only 
		// with the frontmost document -- the current one.
        IDocument* iDocument = Utils<ILayoutUIUtils>()->GetFrontDocument ();			// LayoutUIUtils.h
		if (!iDocument) throw ("Could not get IDocument*.");

		// Get the document's database
		IDataBase* database = ::GetDataBase (iDocument);
		if (!database)
		{
			LOG_END_FUNCTION;
			throw ("Could not get IDataBase*.");
		}
		
		// Get the list of stories in this document
		InterfacePtr<IStoryList> storyList (iDocument, UseDefaultIID());
		if (!storyList)
		{
			LOG_END_FUNCTION;
			throw ("Could not get IStoryList.");
		}

#if VERSION == IN_HOUSE
		char* s = new char [256];
		PMString documentName;
		iDocument->GetName(documentName);
		sprintf (s, "Stories in document %s", documentName.GetPlatformString().c_str());
		LOG (s);
		delete [] s;
#endif

		// Iterate over all the stories:
		for (int32 i = 0; i < storyList->GetAllTextModelCount (); i++ )
		{
			TextStory_StoryAccess storyAccess = storyList->GetNthTextModelAccess(i);
	
			// Get the text model of the story.
			UIDRef storyUIDRef = storyList->GetNthTextModelUID(i);
			if (storyAccess == kUserAccessible)
			{
				// Note that not all stories in a document are for display in text 
				// frames. Some have other uses.  If you are only interested in 
				// displayed stories you need to check what type of access
				// the story has.
				ProcessStory_(storyUIDRef, database);
			}
		} // for all the stories on the list
	}
	catch (char* cstr) {
		char exception_str[256];
		::strcpy (exception_str, "ScanDocumentText_() EXCEPTION:  ");
		::strcat (exception_str, cstr);
		::strcat (exception_str, " Could not create table of contents.");
		LOG (exception_str);
		GiveAlert_(exception_str);
	}
	LOG_END_FUNCTION;
}


/*================================================================================*/
void HM_TableOfContents::ProcessStory_(UIDRef inStoryUIDRef, 
		IDataBase* inDocDatabase) throw (char*) {
/*----------------------------------------------------------------------------------
	Abstract:
		Look at all the text runs in this story.  If the style sheet for classified 
		headings has been applied, TBD do something really cool.
		
	Parameters and modes:
		inStoryUIDRef					IN
		inDocDatabase					IN
		
	Returns:
		Nothing.
		
	Change Log:
		22-Mar-07	New.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;

#if VERSION == IN_HOUSE
	#define	LOG_CLASSIFIED_HEADING_TEXT											\
			::sprintf ( logstr, "<ClassifiedHeading>%s</ClassifiedHeading>",	\
								pmsClassification.GetPlatformString ().c_str() );				\
			LOG (logstr)
	#define	LOG_FRAME_INDICES													\
			::sprintf (logstr, "<FrameIndices first=\"%d\" last=\"%d\"/>", 		\
								firstFrameIndex, lastFrameIndex);				\
			LOG (logstr);
	#define	LOG_FRAME_DATA														\
			::sprintf (logstr, "<FrameData index=\"%d\" page_item_uid=\"0x%x\"/>", \
						firstFrameIndex, textBoxUID);							   \
			LOG (logstr);
	#define	LOG_CLASSIFICATION_START_PAGE_DATA(_X)	LogClassStartPageData_(_X)
	#define	LOG_TEXT_RUN_DATA(startPos, length, name)							\
			::sprintf (logstr, 													\
						"<TextRun position=\"%d\" length=\"%d\" "				\
						"stylename=\"%s\"/>", 									\
						startPos, length, name );								\
			LOG (logstr);		
#else
	#define	LOG_CLASSIFIED_HEADING_TEXT						NULL
	#define LOG_FRAME_INDICES								NULL
	#define	LOG_FRAME_DATA									NULL
	#define LOG_CLASSIFICATION_START_PAGE_DATA(_X)			NULL
	#define	LOG_TEXT_RUN_DATA(startPos, length, name)		NULL
#endif
	
	DECLARE_logstr;
	
	InterfacePtr<ITextModel> textModel (inStoryUIDRef, UseDefaultIID());
	if (!textModel)
	{
		LOG_END_FUNCTION;
		throw ("ProcessStory_(): could not get ITextModel.");
	}
	
	int32 storyLength = textModel->TotalLength ();

	// Navigate to the paragraph attribute strand for this story.
	InterfacePtr<IAttributeStrand> attributeStrand (
		(IAttributeStrand*)textModel->QueryStrand (
			kParaAttrStrandBoss, IAttributeStrand::kDefaultIID));		
	if (!attributeStrand)
	{
		LOG_END_FUNCTION;
		throw ("ProcessStory_(): could not get IAttributeStrand.");
	}
	
	// Initialize loop control variables.
	TextIndex position = 0;
	int32 length = 0;
	int32 numberOfRuns = 0;

	// Loop over the story reporting runs on the paragraph attribute strand
	// and count up the total number of runs.
	while (position < storyLength)
	{
		IFrameList*	currentStoryFrameList = NULL;

		// The length returned by GetStyleUID indicates the boundary
		// of the run to which the style applies.
		UID styleUID = attributeStrand->GetStyleUID (position, &length);
		
		// Get the name of the style.
		InterfacePtr<IStyleInfo> styleInfo ( inDocDatabase, 
												styleUID, UseDefaultIID () );
		if (styleInfo == nil)
		{
			LOG_END_FUNCTION;
			throw ("ProcessStory_(): could not get IStyleInfo.");
		}
		
		const PMString& styleName = styleInfo->GetName ();
		LOG_TEXT_RUN_DATA (position, length, styleName.GetPlatformString().c_str ());

		// Is that style the interesting style?
		const PMString head1StyleName ("Head1");
		if (styleName == head1StyleName)
		{
			// Get the text of the classification heading.
			PMString pmsClassification;
			TextIterator firstCh (textModel, position);
			TextIterator lastCh  (textModel, position + length);
			for (TextIterator iter = firstCh; iter != lastCh; iter++)
			{
				const UTF32TextChar characterCode = *iter;
				if (characterCode == kTextChar_CR)
				{
					continue;
				}
				pmsClassification.AppendW (characterCode);
			}
			LOG_CLASSIFIED_HEADING_TEXT;

			// Find the frame that contains the heading and map it to a page.
			RangeData textRange (position, position + length);
			int32 firstFrameIndex, lastFrameIndex;
			InterfacePtr<ITextModel> textModelPtr (inStoryUIDRef, UseDefaultIID());
			Utils<ITextUtils>()->GetTextFramesContaining (
				textModelPtr, textRange, &firstFrameIndex, &lastFrameIndex);
			LOG_FRAME_INDICES;
			
			// If the heading is not in any frame in the document, it must 
			// be in overset -- give a message and exit now.
			if (firstFrameIndex < 0)
			{
				CAlert::ModalAlert (
					kNewsCadOversetWarningKey,	// Alert string
					kOKString, 					// OK button
					kNullString, 				// No second button
					kNullString, 				// No third button
					1,							// Set OK button to default
					CAlert::eWarningIcon		// Information icon.
				);
				return;
			}
			// Get UID of frame that contains the text to which the 
			// interesting stylesheet has been applied.
			IFrameList* frameList = textModelPtr->QueryFrameList ();
			if (!frameList) throw "IFrameList";
			UID textBoxUID = frameList->GetNthFrameUID ( firstFrameIndex );
			LOG_FRAME_DATA;
			
			// Figure out which page and spread contains that text box.  We need 
			// only the page's UID to perform subsequent operations.  The rest of 
			// the code is to put the page number into the output file.
			InterfacePtr<IPMUnknown> unknown (
										inDocDatabase, textBoxUID, IID_IUNKNOWN);
			InterfacePtr<IHierarchy> hierarchy (unknown, UseDefaultIID());
			if (hierarchy)
			{
				UID pageUID = Utils<ILayoutUtils>()->GetOwnerPageUID (hierarchy);
				::sprintf (logstr, "<Page uid=\"0x%x\"", pageUID);
				InterfacePtr<IPMUnknown> unknown2 (inDocDatabase, pageUID, 
														IID_IUNKNOWN);
				InterfacePtr<IMasterPage> masterPage (unknown2, IID_IMASTERPAGE);
				if (masterPage)
				{
					IDocument* frontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument ();
					
					// Add a string describing the page.
					if (frontDoc)
					{
						InterfacePtr<IPageList> pageList (
													frontDoc, UseDefaultIID());
						if (pageList)
						{
							PMString pageStr;
							pageList->GetPageString (pageUID, &pageStr);
							
							// Write classification and page to output file now:
							WriteRecord_(pmsClassification, pageStr);

							::strcat (logstr, "  GetPageString=\"");
							::strcat (logstr, pageStr.GetPlatformString ().c_str ());
							::strcat (logstr, "\"");
						}
					}
				}
				::strcat (logstr, "/>");
				LOG (logstr);
			}
			else
			{
				// Not sure whether this is a serious error or not ...
				LOG ("Could not getIHierarchy");
			}
		}

		position += length;
		numberOfRuns++;
	}
#if VERSION == IN_HOUSE
	::sprintf (logstr, "<!-- Number of text runs=%d -->", numberOfRuns);
	LOG (logstr);
#endif

	LOG_END_FUNCTION;
}
#ifdef LOG_CLASSIFIED_HEADING_TEXT
	#undef	LOG_CLASSIFIED_HEADING_TEXT
#endif
#ifdef LOG_FRAME_INDICES
	#undef LOG_FRAME_INDICES
#endif
#ifdef LOG_FRAME_DATA
	#undef	LOG_FRAME_DATA
#endif
#ifdef LOG_CLASSIFICATION_START_PAGE_DATA
	#undef LOG_CLASSIFICATION_START_PAGE_DATA
#endif
#ifdef LOG_TEXT_RUN_DATA
	#undef LOG_TEXT_RUN_DATA
#endif


void HM_TableOfContents::OpenOutputFile_()
{
	LOG_BEGIN_FUNCTION;

	// Have the user specify the output file.  ChooseOutputFile_ puts up the 
	// dialog and if the user OKs, stores the file info in the reportFile_ 
	// member and returns true.  If he Cancels, it just returns false.
	okToProcess_ = false;						// CTOR already did this, but ...
	bool doesFileExist;
	if (ChooseOutputFile_(doesFileExist))
	{
        PMString strTOCFile;
        FileUtils::IDFileToPMString(tocFile_, strTOCFile, kTrue);
        m_oTOCFile.open(strTOCFile.GetPlatformString().c_str(), fstream::out);
/*
		// Try to create (or if it already exists, open) the file.
		OSErr err = noErr;
		if (!doesFileExist)										// Already on disk?
		{														// No, so create
			err = ::FSpCreate (&outputFileMacFSSpec_, 'R*ch', 	// BBEdit file
								'TEXT', smSystemScript);
			if (err)
			{
				char errorStr[256];
				::sprintf (errorStr, "OpenOutputFile_(): Could not create file.  "
										"Error = %d", err);
				GiveAlert_(errorStr);
				LOG_END_FUNCTION;
				return;
			}
		}
		
		if (!err)
		{
			// No problems -- open the file.
			err = ::FSpOpenDF (&outputFileMacFSSpec_, 
									fsRdWrPerm, &tocFileRefNum_);
			if (err)
			{
				char errorStr[256];
				::sprintf (errorStr, "OpenOutputFile_(): Could not open file.  "
										"Error = %d", err);
				GiveAlert_(errorStr);
				LOG_END_FUNCTION;
				return;
			}
			err = ::SetEOF (tocFileRefNum_, 0);
			if (err)
			{
				char errorStr[256];
				::sprintf (errorStr, "OpenOutputFile_(): Could not set file EOF "
										"pointer.  Error = %d", err);
				GiveAlert_(errorStr);
				LOG_END_FUNCTION;
				return;
			}
		}
*/
		// If we get this far, all is well.
		okToProcess_ = true;
	}
	
	LOG_END_FUNCTION;
}


void HM_TableOfContents::CloseOutputFile_()
{
	LOG_BEGIN_FUNCTION;
    if (m_oTOCFile.is_open ())
        m_oTOCFile.close ();
/*
	OSErr err = ::FSClose (tocFileRefNum_);
	if (err)
	{
		char errorStr[256];
		::sprintf (errorStr, "CloseOutputFile_(): File did not close normally. "
								" Error = %d", err);
		GiveAlert_(errorStr);
		LOG ("<!-- Error closing TOC file. -->");
	}
	else
	{
		LOG ("<!-- TOC file closed normally. -->");
	}
*/
	LOG_END_FUNCTION;
}


bool HM_TableOfContents::ChooseOutputFile_(bool & ioDoesFileExist)
		throw (char*)
{
	SDKFileSaveChooser outputFileChooser;
	PMString pmsTitle ("Save TOC File As:");				// Setup dialog title
	pmsTitle.SetTranslatable (kFalse);
	PMString pmsFilterTextFiles ("TEXT files (text)");
	pmsFilterTextFiles.SetTranslatable (kFalse);
	outputFileChooser.AddFilter (kTEXTFileTypeInfoID, pmsFilterTextFiles);
	outputFileChooser.SetTitle (pmsTitle);
	
	// Compute a default filename.
	IDocument* iDocument = Utils<ILayoutUIUtils>()->GetFrontDocument ();			// LayoutUIUtils.h
	if (!iDocument) throw ("Could not get IDocument*.");
	PMString pmsOutFileName;
	iDocument->GetName (pmsOutFileName);
	CharCounter dotIndex = pmsOutFileName.LastIndexOfCharacter ('.');
	PMString pms1 ("_TOC");
	if (dotIndex)
	{
		pmsOutFileName.Insert (pms1, dotIndex, 4);
	}
	else
	{
		pmsOutFileName.Append (pms1);
	}

	LOG (pmsOutFileName.GetPlatformString().c_str ());
	outputFileChooser.SetFilename (pmsOutFileName);
	
	// Now put the dialog on the screen.  Runs until user OKs or Cancels.
	outputFileChooser.ShowDialog ();
	
	if ( outputFileChooser.IsChosen () )
	{
		// Save the InDesign file object and fill in a Mac FSSpec for the file.
		tocFile_ = outputFileChooser.GetIDFile ();
        ioDoesFileExist = tocFile_.Exists();
		return true;
	}

	return false;
}


void HM_TableOfContents::WriteRecord_(PMString & inClassifiedHeading, 
										PMString & inPageStr) {
    m_oTOCFile << inClassifiedHeading.GetPlatformString() << "\t" << inPageStr.GetPlatformString() << "\r";
/*
    PMString pmsOut = inClassifiedHeading + "\t" + inPageStr + "\r";
    m_oTOCFile << pmsOut;
	const char* outstr = pmsOut.GetPlatformString().c_str ();
	long byteCount = ::strlen (outstr);
	::FSWrite (tocFileRefNum_, &byteCount, outstr);
*/
}


#pragma mark -

void HM_TableOfContents::MakeContinuedLines_() throw (char*) {
	LOG_BEGIN_FUNCTION;
	try {
		// Iterate over all the stories in the document.  NewsCAD works only 
		// with the frontmost document -- the current one.
		IDocument* iDocument = Utils<ILayoutUIUtils>()->GetFrontDocument ();			// LayoutUIUtils.h
		if (!iDocument) throw ("Could not get IDocument*.");

		// Get the document's database
		IDataBase* database = ::GetDataBase (iDocument);
		if (!database)
		{
			LOG_END_FUNCTION;
			throw ("Could not get IDataBase*.");
		}
		
		// Get the list of stories in this document
		InterfacePtr<IStoryList> storyList (iDocument, UseDefaultIID());
		if (!storyList)
		{
			LOG_END_FUNCTION;
			throw ("Could not get IStoryList.");
		}

		// Iterate over all the stories:
		for (int32 i = 0; i < storyList->GetAllTextModelCount (); i++ )
		{
			TextStory_StoryAccess storyAccess = storyList->GetNthTextModelAccess(i);
	
			// Get the text model of the story.
			UIDRef storyUIDRef = storyList->GetNthTextModelUID(i);
			if (storyAccess == kUserAccessible)
			{
				// Note that not all stories in a document are for display in text 
				// frames. Some have other uses.  If you are only interested in 
				// displayed stories you need to check what type of access
				// the story has.
				FindAllClassifiedHeadings_(storyUIDRef, database);
				
				// If this story contains any classified headings, insert 
				// continued lines as needed.
				if ( ! classifiedHeadingsVector_.empty () )
				{
					CreateContinuedLinesForStory_(storyUIDRef, database);
				}
			}
		} // for all the stories on the list
	}
	catch (char* cstr) {
		char exception_str[256];
		::strcpy (exception_str, "MakeContinuedLines_() EXCEPTION:  ");
		::strcat (exception_str, cstr);
		::strcat (exception_str, " Could not create continued lines.");
		LOG (exception_str);
		GiveAlert_(exception_str);
	}
	LOG_END_FUNCTION;
}


/*================================================================================*/
void HM_TableOfContents::FindAllClassifiedHeadings_(
					UIDRef inStoryUIDRef, IDataBase* inDocDatabase)
					throw (char*) {
/*----------------------------------------------------------------------------------
	Abstract:
		Walk through the text stream.  Find and store the start & end offsets for 
		each QXP paragraph run that has the "Head1" stylesheet applied.
		
	Modifies:
		Fills in the classifiedHeadingsVector_.
	
	Parameters & modes:

	Returns:

	Change Log:
----------------------------------------------------------------------------------*/
#if VERSION == IN_HOUSE
	#define	LOG_TEXT_RUN_DATA(startPos, length, name)							\
			::sprintf (logstr, 													\
						"<TextRun position=\"%d\" length=\"%d\" "				\
						"stylename=\"%s\"/>", 									\
						startPos, length, name );								\
			LOG (logstr);		
#else
	#define	LOG_TEXT_RUN_DATA(startPos, length, name)		NULL
#endif

	LOG_BEGIN_FUNCTION;
	DECLARE_logstr;

	// Eliminate stale data if any
	classifiedHeadingsVector_.clear ();

	// Get story's text model, which contains all the info about text runs.
	InterfacePtr<ITextModel> textModel (inStoryUIDRef, UseDefaultIID());
	if (!textModel)
	{
		LOG_END_FUNCTION;
		throw ("FindAllClassifiedHeadings_(): could not get ITextModel.");
	}

	int32 storyLength = textModel->TotalLength ();

	// Navigate to the paragraph attribute strand for this story.
	InterfacePtr<IAttributeStrand> attributeStrand (
		(IAttributeStrand*)textModel->QueryStrand (
			kParaAttrStrandBoss, IAttributeStrand::kDefaultIID));		
	if (!attributeStrand)
	{
		LOG_END_FUNCTION;
		throw ("FindAllClassifiedHeadings_(): could not get IAttributeStrand.");
	}
	
	// Initialize loop control variables.
	TextIndex position = 0;
	int32 length = 0;
	int32 numberOfRuns = 0;

	// Loop over the story reporting runs on the paragraph attribute strand
	// and count up the total number of runs.
	while (position < storyLength)
	{
		IFrameList*	currentStoryFrameList = NULL;

		// The length returned by GetStyleUID indicates the boundary
		// of the run to which the style applies.
		UID styleUID = attributeStrand->GetStyleUID (position, &length);
		
		// Get the name of the style.
		InterfacePtr<IStyleInfo> styleInfo ( inDocDatabase, 
												styleUID, UseDefaultIID () );
		if (styleInfo == nil)
		{
			LOG_END_FUNCTION;
			throw ("ProcessStory_(): could not get IStyleInfo.");
		}
		
		const PMString& styleName = styleInfo->GetName ();
		LOG_TEXT_RUN_DATA (position, length, styleName.GetPlatformString().c_str());

		// Is that style the interesting style?
		const PMString head1StyleName ("Head1");
		if (styleName == head1StyleName)
		{
			// Compute and store start and ending offset of classified heading..
			HM_ClassifiedHeading classHd (position, position + length);
			classifiedHeadingsVector_.push_back (classHd);
		}

		position += length;
		numberOfRuns++;
	}
	
	LOG_END_FUNCTION;
}


#include "CL_Document.h"

/*================================================================================*/
void HM_TableOfContents::CreateContinuedLinesForStory_(UIDRef inStoryUIDRef, 
												IDataBase* inDocDB) {
/*----------------------------------------------------------------------------------
	Abstract:
		Loop over the classifiedHeadingsVector_ and set continued lines as needed.
		
	Parameters and modes:
		
	Returns:

	Change Log:
		10-Apr-07	Began port to InDesign.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;

	// For each classified head, see if its classified ads' text (that is, text 
	// that has the "ClassBody" stylesheet applied to it) goes beyond 
	// the current spread.  If it does, place continued lines on left-hand 
	// pages until the classification changes.
	vector<HM_ClassifiedHeading>::iterator p = classifiedHeadingsVector_.begin ();
	vector<HM_ClassifiedHeading>::iterator q = p + 1;
	while (p < classifiedHeadingsVector_.end ())
	{
		auto_ptr<CL_Document> myFrontDoc (new CL_Document); 
		int16 numPages;
		myFrontDoc->GetPageCount_(&numPages);
		
		// Get starting page number and other relevant information applicable to 
		// making continued lines for current classifications.
		p->ComputeDataForContinuedLine_(inStoryUIDRef, inDocDB);
		int32 startPageNum = p->PageIndex_();

		// Figure next left-hand page following current classification start page.
		// As an example, assuming pages are numbered with even numbers on the 
		// left, if the current classified head falls on page 3, the next left 
		// page is 4.  If the current classified head falls on page 6, the next 
		// verso is 8.
		int32 nextLeftPageNum = 
				p->PageType_() == kLeftPage ? startPageNum + 2 : startPageNum + 1;

		// Get starting page number of next classification -- IOW, the page on 
		// which the next classified head currently falls.  Be careful here!!--
		// handle the special case where this entire document contains only one 
		// classification, or the current classification is the last one in the 
		// document.  In both cases, this also means there is no next classification.
		int32 endPageNum;
		if (q < classifiedHeadingsVector_.end())
		{
			q->ComputeDataForContinuedLine_(inStoryUIDRef, inDocDB);
			endPageNum = q->PageIndex_();
		}
		else
		{
			endPageNum = numPages - 1;
		}
		
// HM_Document::CreateContinuedLinesForStory_(boxid inBoxID) in old Quark code:
LOG_TAG_AND_VALUE ("StartPageNum", startPageNum);
LOG_TAG_AND_VALUE ("NextLeftPageNum", nextLeftPageNum);
LOG_TAG_AND_VALUE ("EndPageNum", endPageNum);

		if ( nextLeftPageNum <= numPages )
		{
			while (nextLeftPageNum <= endPageNum)
			{
				TypesetAndPasteUpContinuedLine_(nextLeftPageNum, endPageNum, *p,
								inStoryUIDRef
								/*, &macRect, inBoxID, 
								p->GetStartingOffset_(), p->GetEndingOffset_()*/);

				nextLeftPageNum += 2;									// Bump
				
				// Update folio where current classification ends.
/* THE OLD QUARK CODE ... REMOVE WHEN APPROPRIATE.
				endPageNum = GetClassificationStartPageNumData_(inBoxID, 
								q->GetStartingOffset_(), q->GetEndingOffset_(), 
								&pg, &macRect);
*/
				if (q < classifiedHeadingsVector_.end())
				{
					q->ComputeDataForContinuedLine_(inStoryUIDRef, inDocDB);
					endPageNum = q->PageIndex_();
				}
				else
				{
					NULL;	// EMPTY
				}
			}
		} // end for each page that contains the current classification

		// Next classifications
		++p;
		if (q < classifiedHeadingsVector_.end ()/* - 1*/)
		{
			++q;
		}
	} // end for each heading in the classifiedHeadingsVector_

	LOG_END_FUNCTION;
}

// Find the frame that contains the the text in the story referenced by 
// inStoryUIDRef at the offsets from inStartCharIdx to inEndCharIdx. 
// Then map that frame to a page.
int HM_TableOfContents::GetClassificationStartPageNum_(
								UIDRef inStoryUIDRef, 
								IDataBase* inDocDatabase, 
								vector<HM_ClassifiedHeading>::iterator iterHeading)
{
	LOG_BEGIN_FUNCTION;
	int x = 0;
	TextIndex inStartCharIdx = iterHeading->StartingOffset_();
	TextIndex inEndCharIdx = iterHeading->EndingOffset_();

	InterfacePtr<ITextModel> pITextModel (inStoryUIDRef, UseDefaultIID());
	if ( ! pITextModel )
	{
		LOG_END_FUNCTION;
		throw ("GetClassificationStartPageNum_(): could not get ITextModel.");
	}

	RangeData textRange (inStartCharIdx, inEndCharIdx);
	int32 firstFrameIndex, lastFrameIndex;
	Utils<ITextUtils>()->GetTextFramesContaining (pITextModel, textRange, 
												&firstFrameIndex, &lastFrameIndex);

	// Get UID of frame that contains this classified heading.
	IFrameList* frameList = pITextModel->QueryFrameList ();
	if (!frameList) throw "IFrameList";
	UID textBoxUID = frameList->GetNthFrameUID ( firstFrameIndex );

	// Figure out which page and spread contains that frame.
	InterfacePtr<IPMUnknown> unknown (inDocDatabase, textBoxUID, IID_IUNKNOWN);
	InterfacePtr<IHierarchy> hierarchy (unknown, UseDefaultIID());
	if (hierarchy)
	{
		UID pageUID = Utils<ILayoutUtils>()->GetOwnerPageUID (hierarchy);
		InterfacePtr<IPMUnknown> unknown2 (inDocDatabase, pageUID, 
												IID_IUNKNOWN);
		InterfacePtr<IMasterPage> masterPage (unknown2, IID_IMASTERPAGE);
		if (masterPage)
		{
			IDocument* frontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument ();
//#if 0			
			// 
			if (frontDoc)
			{
				InterfacePtr<IPageList> pageList (
											frontDoc, UseDefaultIID());
				if (pageList)
				{
					SetPageCount_( pageList->GetPageCount () );
/*					PMString pageStr;
					pageList->GetPageString (pageUID, &pageStr);
					
					// Write classification and page to output file now:
					WriteRecord_(pmsClassification, pageStr);

					::strcat (logstr, "  GetPageString=\"");
					::strcat (logstr, pageStr.GrabCString ());
					::strcat (logstr, "\"");
*/				}
			}
//#endif
		}
//		::strcat (logstr, "/>");
//		LOG (logstr);

	}
	else
	{
		// Not sure whether this is a serious error or not ...
		LOG ("Could not getIHierarchy");
	}



	LOG_END_FUNCTION;
	return x;
}
#ifdef LOG_TEXT_RUN_DATA
	#undef LOG_TEXT_RUN_DATA
#endif


#include "HM_Page.h"

/*================================================================================*/
void HM_TableOfContents::TypesetAndPasteUpContinuedLine_(int32 inPageNum, 
			int32 inNextClassFirstPageNum, HM_ClassifiedHeading & inClassHeading, 
			UIDRef inStoryUIDRef/*, Rect *inNextHeadRect, 
			boxid inBoxID, long inTextStart, long inTextEnd*/) {
/*----------------------------------------------------------------------------------
	Abstract:
		Called by CreateContinuedLinesForStory_(), this method sets up everything 
		for typesetting the text of the classification heading and the word 
		"continued".  Then it creates a page object that places it at the top and 
		leftmost point of the ad live area that doesn't contain a display ad.
		
	Parameters and modes:
		inPageNum		Zero-based index of page on which to paste 				IN
						up continued line.
		inNextClassFirstPageNum
						Last page containing ads that belong to the 			IN
						current classification.
		inNextHeadRect	A Mac Rect representing the geometry of the next 		IN
						classification's heading.
		inBoxID			First box in text chain currently in process.			IN
		inTextStart		Starting offset of text of classified head in stream.	IN
		inTextEnd		Ending offset of text of classified head in stream.		IN
		
	Returns:

	Change Log:
		30-May-07	Began port from QuarkXPress.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	
	LOG_TAG_AND_VALUE ("PageForContinuedLine", inPageNum);
	LOG_TAG_AND_VALUE ("LastPageOfCurrentClassification", inNextClassFirstPageNum);
	
	// Setup a page object for the page where the continued line will go.
	InterfacePtr<IPageList> pageList (Utils<ILayoutUIUtils>()->GetFrontDocument (), UseDefaultIID());
	UID currPageUID = pageList->GetNthPageUID (inPageNum);
	PMString pageStr;
	pageList->GetPageString (currPageUID, &pageStr);
	int32 curFolio = pageStr.GetAsNumber ();
	HM_Page *pageObj = new HM_Page (NULL, pageList, currPageUID, 
												inPageNum, curFolio);
/*
	HM_Page* pageObj = new HM_Page (pageList, inClassHeading.PageUID_());
	pageObj->SetFolio_(inPageNum);
*/
	pageObj->InitializeInDesignData_();
	pageObj->GoTo_();
	pageObj->SetupForClassifiedContinuedLinePlacement_();
	
	if (pageObj->FindOpenRects_())			// Ensure there's a place for it
	{
		// Fetch text of head
		InterfacePtr<ITextModel> textModel (inStoryUIDRef, UseDefaultIID());
		if (!textModel) throw ("ITextModel");
		PMString pmsClassification;
		TextIterator firstCh (textModel, inClassHeading.StartingOffset_());
		TextIterator lastCh  (textModel, inClassHeading.EndingOffset_());
		for (TextIterator iter = firstCh; iter != lastCh; iter++)
		{
			const UTF32TextChar characterCode = *iter;
			if (characterCode == kTextChar_CR)
			{
				continue;
			}
			pmsClassification.AppendW (characterCode);
		}
		
		// Ask the page object to do the actual typesetting with computed data.
		pageObj->TypesetAndPasteupContinuedLine_(pmsClassification);
		
		// Re-HNJ the story
		IFrameList* frameList = textModel->QueryFrameList ();
		if (!frameList) throw "IFrameList";
		ASSERT (frameList);
		int32 startRetypesetFrameIndex = frameList->GetFirstDamagedFrameIndex ();
		if (startRetypesetFrameIndex != -1)
		{
			LOG ("Re-HNJ");
			InterfacePtr<IFrameListComposer> frameListComposer (
										frameList, UseDefaultIID());
			ASSERT (frameListComposer);
			frameListComposer->RecomposeThruTextIndex(kInvalidTextIndex);

			// If the story's overset, do something!
			if (Utils<ITextUtils>()->IsOverset (frameList))
			{
				LOG ("OVERSET");
			}
		}
	}

	delete pageObj;
	
	LOG_END_FUNCTION;
#undef LOG_CLASSIFIED_HEADING_TEXT
}
			
			
// END OF FILE
