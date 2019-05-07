/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	HM_Geometry_Importer.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	6/04/03 ca. 12:22 PM
	
	ab	Abstract:
	ab	Implementation of HM_Geometry_Importer class.
	
	cl	Change Log:
	cl		08-Dec-10	Began port to CS4.
	cl		6/04/03	New.
------------------------------------------------------------------------------*/
using namespace std;

#include "newscad.h"
#include <cstring>
#include <string>
#include <cctype>
#include "IStoryList.h"
#include "ITextModel.h"
#include "IAttributeStrand.h"
#include "IStyleInfo.h"
#include "TextIterator.h"
#include "CAlert.h"
#include "ImportGeomID.h"
#include "ILayoutUtils.h"
#include "IPasteboardUtils.h"
#include "IMasterPage.h"		// For enhanced logging
#include "IPageList.h"			// ditto
#include "IHierarchy.h"
#include "IFrameListComposer.h"

#include "HM_Page.h"

///////////////////////////////////////////////////////////////////////////////
#if 0
#if PRODUCT == INDESIGN_PLUGIN
	#include "LayoutUtils.h"
	#include "IDocument.h"
	#include "IFrameList.h"
	#include "ISpread.h"
	
#endif
#endif
///////////////////////////////////////////////////////////////////////////////
#include "CL_Geometry_Importer.h"
#include "HM_Geometry_Importer.h"
#include "CL_Log_File.h"
#include "global_data.h"
///////////////////////////////////////////////////////////////////////////////
#if 0
//#include "CL_Document.h"
#include "CL_Display_Ad.h"
#endif
///////////////////////////////////////////////////////////////////////////////

//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and D E S T R U C T I O N
//----------------------------------------------------------------------------------
HM_Geometry_Importer::HM_Geometry_Importer ()
	:	currentStoryFrameList_(NULL)
{
	LOG_BEGIN_FUNCTION;
	LOG ("Constructing the Hemmings geometry importer.");
	LOG_END_FUNCTION;
}

HM_Geometry_Importer::~HM_Geometry_Importer ()
{
	LOG_BEGIN_FUNCTION;
	LOG ("Destroying the Hemmings geometry importer.");
	LOG_END_FUNCTION;
}

/* -------------------- B U I L D I N G   D O C U M E N T S --------------------- */
#pragma mark -
//------------------------------------------------------------------------------
// Make up the pages by eventually creating QXP objects for the display ads that 
// go on the pages.  Overrides the basic method in CL_Geometry_Importer.
//
//	These functions place display ads on or near pages that contain their 
//	classification.  IOW, a display ad for an Avanti for sale should come out on 
//	(or be placed very close to) one of the pages that contains classified 
//	ads in the section "Avanti Cars For Sale".
//
//	The function in use searches the document looking for classified headings.  
//	When it finds one, it searches the list of ads to be placed for ads that  
//	belong under that heading.  It then goes to that page and pastes up the ads 
//	within their classification.
//------------------------------------------------------------------------------


/*================================================================================*/
void HM_Geometry_Importer::BuildInDesignDocument_() {
/*----------------------------------------------------------------------------------
	Abstract:
		Search all the stories in the document and look for classified headings in 
		them.  When the program finds one, determine the page on which it is 
		showing.  Then paste up the display ads for that classification as close 
		as possible to the heading for that classification.
		
	Parameters and modes:
		None.
		
	Returns:
		Nothing.
		
	Change Log:
		11-Dec-10	Began port to CS4.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	DECLARE_logstr;							// Debugging only
	if (unplacedAdsVector_.empty ())		// Anything to do?
	{
		LOG ("<p>No ads to place.</p>");
		LOG_END_FUNCTION;
		return;
	}

	frontDoc_	= NULL;
	frontDocDB_	= NULL;
	
	try {
		// Iterate over all the stories in the document.  NewsCAD works only 
		// with the frontmost document -- the current one.
		CurrentDocument_()->Update_();
		IDocument* iDocument = frontDoc_ = CurrentDocument_()->IDocument_();
		if (!iDocument) throw ("IDocument");

		// Get the document's database
		IDataBase* database = frontDocDB_ = ::GetDataBase (iDocument);
		if (!database) throw ("IDataBase");
		
		// Get the list of stories in this document
		InterfacePtr<IStoryList> storyList (iDocument, UseDefaultIID());
		if (!storyList) throw ("IStoryList");
		
		LogDocumentStories_();
		
		// Big, ugly loop to iterate over all the stories:
		for (int32 i = 0; i < storyList->GetAllTextModelCount (); i++ )
		{
			TextStory_StoryAccess storyAccess = storyList->GetNthTextModelAccess(i);
	
			// Get the text model of the story.
			UIDRef storyUIDRef = storyList->GetNthTextModelUID(i);

			InterfacePtr<ITextModel> textModel (storyUIDRef, UseDefaultIID());
			if (!textModel) throw ("ITextModel");
			int32 totalLength = textModel->TotalLength ();
			LogCurrentStoryData_(i, textModel, storyUIDRef, totalLength, storyAccess);

			if (storyAccess == kUserAccessible)
			{
				// Note that not all stories in a document are for display in text 
				// frames. Some have other uses.  If you are only interested in 
				// displayed stories you need to check what type of access
				// the story has.
				ProcessStory_(storyUIDRef);
			}
		} // for all the stories on the list
	}
	catch (char* cstr) {
		char exception_str[256];
		::strcpy (exception_str, "EXCEPTION:  ");
		::strcat (exception_str, cstr);
		LOG (exception_str);
	}

	LOG_END_FUNCTION;
}


#if VERSION == IN_HOUSE
	#define	LOG_CLASSIFIED_HEADING_TEXT											\
			::sprintf ( logstr, "<ClassifiedHeading>%s</ClassifiedHeading>",	\
								pmsClassification.GrabCString () );				\
			LOG (logstr)
	#define	LOG_FRAME_INDICES													\
			sprintf (logstr, "<FrameIndices first=\"%d\" last=\"%d\"/>", 		\
								firstFrameIndex, lastFrameIndex);				\
			LOG (logstr);
	#define	LOG_FRAME_DATA														\
			::sprintf (logstr, "<FrameData index=\"%d\" page_item_uid=\"0x%x\"/>", \
						firstFrameIndex, textBoxUID.Get());							   \
			LOG (logstr);
	#define	LOG_CLASSIFICATION_START_PAGE_DATA(_X)	LogClassStartPageData_(_X)
#else
	#define	LOG_CLASSIFIED_HEADING_TEXT						NULL
	#define LOG_FRAME_INDICES								NULL
	#define	LOG_FRAME_DATA									NULL
	#define LOG_CLASSIFICATION_START_PAGE_DATA(_X)			NULL
#endif

/*================================================================================*/
void HM_Geometry_Importer::ProcessStory_(UIDRef inStoryUIDRef)	throw (char*) {
/*----------------------------------------------------------------------------------
	Abstract:
		Look at all the text runs in this story.  If the style sheet for classified 
		headings has been applied, paste up the display ads for that classification.
		
	Parameters and modes:
		inDatabasePtr		Database for the document we're pasting up.			IN
		inTextModelPtr		Text model for the story we're processing.			IN
		inStoryLength		Story char count.									IN
		
	Returns:
		Nothing.
		
	Change Log:
		19-Sep-05	New.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;

	bool wereAdsPastedUp = false;
	DECLARE_logstr;
	
	InterfacePtr<ITextModel> textModel (inStoryUIDRef, UseDefaultIID());
	if (!textModel) throw ("ITextModel");
	int32 storyLength = textModel->TotalLength ();

	// Navigate to the paragraph attribute strand for this story.
	InterfacePtr<IAttributeStrand> attributeStrand (
		(IAttributeStrand*)textModel->QueryStrand (
			kParaAttrStrandBoss, IAttributeStrand::kDefaultIID));		
	if (!attributeStrand) throw ("IAttributeStrand");
	
	// Initialize loop control variables.
	TextIndex position = 0;
	int32 length = 0;
	int32 numberOfRuns = 0;

	// Loop over the story reporting runs on the paragraph attribute strand
	// and count up the total number of runs.
//	LOG ("position, length, styleName");
	while (position < storyLength)
	{
		currentStoryFrameList_ = NULL;

		// The length returned by GetStyleUID indicates the boundary
		// of the run to which the style applies.
		UID styleUID = attributeStrand->GetStyleUID (position, &length);
		
		// Get the name of the style.
		InterfacePtr<IStyleInfo> styleInfo ( frontDocDB_, 
												styleUID, UseDefaultIID () );
		if (styleInfo == nil) throw ("IStyleInfo");
		const PMString& styleName = styleInfo->GetName ();
		LogTextRunData_(position, length, styleName.GrabCString());

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
			// the code is to put the page number into the log file.
			InterfacePtr<IPMUnknown> unknown (
										frontDocDB_, textBoxUID, IID_IUNKNOWN);
			InterfacePtr<IHierarchy> hierarchy (unknown, UseDefaultIID());
			if (hierarchy)
			{
				UID pageUID = Utils<ILayoutUtils>()->GetOwnerPageUID (hierarchy);
				ISpread* spread = 
								Utils<IPasteboardUtils>()->QuerySpread (hierarchy);
				LOG_CLASSIFICATION_START_PAGE_DATA (pageUID);
				
				// Paste up display ads with this classification
				currentStoryFrameList_ = frameList;
				wereAdsPastedUp = PasteUpDisplayAdsForClassification_(
									pmsClassification, spread, pageUID);
			}
			else
			{
				throw "IHierarchy";
			}
		}

		position += length;
		numberOfRuns++;
	}
#if VERSION == IN_HOUSE
	sprintf (logstr, "<!-- Number of text runs=%d -->", numberOfRuns);
	LOG (logstr);
#endif

/* soon, but not today:	LOG_AD_VECTORS_STATUS; */

	LOG_END_FUNCTION;
}

#undef	LOG_CLASSIFIED_HEADING_TEXT
#undef	LOG_FRAME_INDICES
#undef	LOG_FRAME_DATA
#undef	LOG_CLASSIFICATION_START_PAGE_DATA


/*================================================================================*/
bool HM_Geometry_Importer::PasteUpDisplayAdsForClassification_(
							PMString & inClassification, 
							ISpread* inSpreadPtr, UID inPageUID) {
/*----------------------------------------------------------------------------------
	Abstract:
		Go through the vector of ads to place and look for those whose 
		classification matches inClassification.  If any are found, go to the page 
		where that classified heading appears and paste up the ads we found.
		
	Parameters and modes:
		inClassification	InDesign string containing the classified heading.	IN
		inSpreadPtr			The ISpread that contains the page where the 		IN
							classification begins.
		inPageUID			UID of page where the heading appears.				IN
		
	Returns:
		true if any ads were pasted up, else false.
		
	Change Log:
		26-Sep-05	New.
----------------------------------------------------------------------------------*/
	LOG_BEGIN_FUNCTION;
	bool retval = false;
 	DECLARE_logstr;

#if VERSION == IN_HOUSE
	::sprintf (logstr, "<Classification>%s</Classification>", 
								inClassification.GrabCString () );
	LOG (logstr);
	::sprintf (logstr, "<PageUID>0x%x</PageUID>", inPageUID.Get());
	LOG (logstr);
#endif
	
	// Store ads with the same classification as inClassification in another vector.
	// Exit immediately if no display ads for the passed-in classification.
 	SelectAdsInCurrentClassification_( inClassification.GrabCString () );
	if (adsToPlaceVector_.size () <= 0)
	{
		LOG ("<AdCount>0</AdCount>");
		LOG_END_FUNCTION;
		return retval;
	}

#if VERSION == IN_HOUSE
	::sprintf (logstr, "<AdCount>%d</AdCount>", adsToPlaceVector_.size ());
	LOG (logstr);
#endif

 	// Compute percentage of page that's allowed to be covered by ads.
 	ComputeAdCoverageThreshhold_();
#if VERSION == IN_HOUSE
	::sprintf (logstr, "<AdCoverageThreshhold>%d</AdCoverageThreshhold>", 
							adCoverageThreshhold_);
	LOG (logstr);
#endif
	
	// Here's where we place the ads.
	// Start at the page where the current classification begins.  Loop over 
	// the remaining pages in the document and place the ads.  Exit the loop 
	// when we run out of either ads to place or pages to scan.

	// We need to discover the index of the page in the document.  Those page 
	// indices are zero-based (yup, page 1 is index 0).
	int32 pageIndex;
	int32 curFolio;
	int totalRects = 0;
	InterfacePtr<IPageList> pageList (frontDoc_, UseDefaultIID());
	if (!pageList) throw ("IPageList");

	pageIndex = pageList->GetPageIndex (inPageUID);

	// For each PAGE in the document and while there are ads to place:
	LOG ("<p>Start of page makeup loop</p>");

#if 0
THIS IS JUST TEST CODE -- DOES EVERYTHING EXCEPT PLACE THE ADS
	for (int32 i = 0; i <= 5; i++) {
		UID currPageUID = pageList->GetNthPageUID (i);
		PMString pageStr;
		pageList->GetPageString (currPageUID, &pageStr);
		curFolio = pageStr.GetAsNumber ();
		HM_Page *pageObj = new HM_Page (this, pageList, currPageUID, i, curFolio);
		pageObj->SetIsColorPage_(IsColorPage_(curFolio));
		pageObj->InitializeInDesignData_();
		pageObj->GoTo_();
		delete pageObj;
	}
#endif
	for (int32 i = pageIndex; 
			i < pageList->GetPageCount () && adsToPlaceVector_.size () > 0; i++)
	{
		UID currPageUID = pageList->GetNthPageUID (i);
		PMString pageStr;
		pageList->GetPageString (currPageUID, &pageStr);
		curFolio = pageStr.GetAsNumber ();
		HM_Page *pageObj = new HM_Page (this, pageList, currPageUID, i, curFolio);
		pageObj->SetIsColorPage_(IsColorPage_(curFolio));
		pageObj->InitializeInDesignData_();
		pageObj->GoTo_();
		pageObj->SetupForAdPlacement_();
		totalRects = pageObj->FindOpenRects_();
		if (totalRects > 0)
		{
			pageObj->PasteUp_(adsToPlaceVector_);
		}
		
		delete pageObj;
	}

// 
// /*
// 	// We need to discover which page this is in the spread.
// 	int32 pageIndex = inSpreadPtr->GetPageIndex (inPageUID);
// 	::sprintf (logstr, "<PageIndexInSpread>%d</PageIndexInSpread>", pageIndex);
// 	LOG (logstr);
// 	
// */
// 
	// Copy candidates that weren't placed back into the input vector.
	UpdateAdVectorsAfterPasteUp_();

	LOG ("<!-- End of HM_Geometry_Importer::PasteUpDisplayAdsForClassification_() -->");
	LOG_END_FUNCTION;
	return retval;
}


//----------------------------------------------------------------------------------
void HM_Geometry_Importer::SelectAdsInCurrentClassification_(
		const char* inClassification)
{
	LOG_BEGIN_FUNCTION;
	DECLARE_logstr;
#if 0
TBD:  HANDLE MULTI-BYTE CHARACTERS.
	// Convert "typesetter" quotes in the text stream to 
	// "typewriter" quotes to match the geometry file.
	char *pc = inClassification;
	while (*pc)
	{
		if (*pc == '‘' || *pc == '’')
		{
			*pc = 0x27;
		}
		if (*pc == '“' || *pc == '”')
		{
			*pc = 0x22;
		}
		++pc;
	}

	string classificationInProcess = inClassification;
#endif
	// Downcase the string -- new code 4/10/11
	int len = ::strlen (inClassification) + 2;
	char* sClassificationInProcess = new char [len];
	::strcpy (sClassificationInProcess, inClassification);
	char* pc = sClassificationInProcess;
	while (*pc)
	{
		*pc = tolower (*pc);
		++pc;
	}
	string classificationInProcess = sClassificationInProcess;
	::sprintf (logstr, 
			"<classificationInProcess>%s</classificationInProcess>", 
			classificationInProcess.c_str());
	LOG (logstr);
	
	string adClassification;
	const char* sAdClassification;
	char* sAdClassification2;
	vector<CL_Display_Ad>::iterator p = unplacedAdsVector_.begin ();
	while (p < unplacedAdsVector_.end ())				// For each AD in the file
	{
		p->GetSection_(adClassification);
		
		// Downcase the string -- new code 4/10/11
		sAdClassification = adClassification.c_str();
		len = ::strlen (sAdClassification) + 2;
		sAdClassification2 = new char [len];
		::strcpy (sAdClassification2, sAdClassification);
		pc = sAdClassification2;
		while (*pc)
		{
			*pc = tolower (*pc);
			++pc;
		}
		adClassification = sAdClassification2;
		delete [] sAdClassification2;
		::sprintf (logstr, 
				"<adClassification>%s</adClassification>", 
				adClassification.c_str());
		LOG (logstr);
		
		if (classificationInProcess == adClassification)
		{
#if VERSION == IN_HOUSE
			string artFile;
			p->GetArtFileName_(artFile);
			::sprintf (logstr, "<AdToPasteUp>%s</AdToPasteUp>", artFile.c_str());
			LOG (logstr);
#endif
			// Move to "ads to place" by copy/delete from input vector.
			// p will point to the element after this one after erasing.
			adsToPlaceVector_.push_back (*p);
			unplacedAdsVector_.erase (p);			// TBD:  Optimize this -- causes hundreds of calls to operator()=
		}
		else
		{
			++p;
		}
	} // End for each AD in the file

	delete [] sClassificationInProcess;

	LOG_END_FUNCTION;
}


//------------------------------------------------------------------------------
//	UpdateAdVectorsAfterPagePasteup_()
//	Called at the end of pasteup on each page, this method updates the "ads to 
//	place" and the "ads that have been placed" vectors.
//------------------------------------------------------------------------------
bool HM_Geometry_Importer::UpdateAdVectorsAfterPagePasteup_()
{
	LOG_BEGIN_FUNCTION;
	bool retval = false;
	vector<CL_Display_Ad>::iterator p;
	
	p = adsToPlaceVector_.begin ();
	while (p < adsToPlaceVector_.end ())
	{
		if (p->GetPlaced_())
		{
			retval = true;
			
			// Copy to placed ads vector.	
			placedAdsVector_.push_back (*p);
			adsToPlaceVector_.erase (p);
		}
		else
		{
			++p;
		}
	}
	LOG_END_FUNCTION;
	return retval;
}


#pragma mark -

/* -------------------------------- LOG + DEBUG --------------------------------- */

//------------------------------------------------------------------------------
//	LogDocumentStories_
//
//	DANGER:  This function assumes caller has set frontDoc_ (pointer to ID doc.)
//------------------------------------------------------------------------------
void HM_Geometry_Importer::LogDocumentStories_()
{
#if VERSION == IN_HOUSE
	DECLARE_logstr;	
	PMString documentName;
	frontDoc_->GetName(documentName);
	sprintf (logstr, "<h3>Stories in document %s</h3>", 
				documentName.GrabCString());
	LOG (logstr);
#endif
}

//------------------------------------------------------------------------------
//	LogCurrentStoryData_
//------------------------------------------------------------------------------
void HM_Geometry_Importer::LogCurrentStoryData_(
		int32 inStoryCounter, ITextModel* ipCurTextModel, UIDRef inStoryUIDRef, 
		int32 inStoryCharCount, TextStory_StoryAccess inStoryAccess)
{
#if VERSION == IN_HOUSE
	char* storyAccessName[] = { 
							"invalid", "userAccessible", "notUserAccessible"};	
	DECLARE_logstr;	
	int32 primaryStoryThreadSpan = ipCurTextModel->GetPrimaryStoryThreadSpan ();
	LOG ("<table><tr><td>Loop</td><td>UID</td><td>StoryAccess</td>"
			"<td>Length</td><td>PrimaryStoryThreadSpan</td></tr>");
	::sprintf (logstr, "<tr><td>%d</td><td>0x%x</td><td>%s</td>"
						"<td>%d</td><td>%d</td></tr></table>", 
						inStoryCounter, 
						inStoryUIDRef.GetUID().Get(),
						storyAccessName[inStoryAccess],
						inStoryCharCount,
						primaryStoryThreadSpan);
	LOG (logstr);
#endif
}

//------------------------------------------------------------------------------
//	LogTextRunData_
//------------------------------------------------------------------------------
void HM_Geometry_Importer::LogTextRunData_(
			const TextIndex startPos, const int32 length, const char* name)
{
#if VERSION == IN_HOUSE
	LOG ("<table><tr><td>Position</td><td>Length</td><td>StyleName</td></tr>");
	DECLARE_logstr;
	sprintf (logstr, 
				"<tr><td>%d</td><td>%d</td><td>%s</td></tr></table>", 
				startPos, length, name );
	LOG (logstr);		
#endif
}


//------------------------------------------------------------------------------
//	LogClassStartPageData_
//------------------------------------------------------------------------------
void HM_Geometry_Importer::LogClassStartPageData_(const UID pageUID)
{
#if VERSION == IN_HOUSE
	DECLARE_logstr;
	::sprintf (logstr, "<Page uid=\"0x%x\"", pageUID.Get());

	InterfacePtr<IPMUnknown> unknown2 (frontDocDB_, pageUID, 
														IID_IUNKNOWN);
	InterfacePtr<IMasterPage> masterPage (unknown2, IID_IMASTERPAGE);
	if (masterPage)
	{
		// Add a string describing the page.
		if (frontDoc_)
		{
			InterfacePtr<IPageList> pageList (frontDoc_, UseDefaultIID());
			if (pageList)
			{
				PMString pageStr;
				pageList->GetPageString (pageUID, &pageStr);
				::strcat (logstr, "  num=\"");
				::strcat (logstr, pageStr.GrabCString ());
				::strcat (logstr, "\"");
			}
		}
	}
	::strcat (logstr, "/>");
	LOG (logstr);
#endif
}


// Called from HM_Page::PasteUp_() as soon as it has finished pasting up an ad.
void HM_Geometry_Importer::HandleAdPastedUp_(int inIdx)
{
	LOG_BEGIN_FUNCTION;
	
	CL_Display_Ad & ad = adsToPlaceVector_[inIdx];
//	CL_Page* pAdPage = ad.ParentPage_();
	Basic_Page* pAdPage = ad.ParentPage_();

	// Compute area of ad, accumulate in ad area for current page
//	StoreArea_(ad.Width_(), ad.Height_(), pAdPage->GetFolio_());
	StoreArea_(ad.Width_(), ad.Height_(), pAdPage->PageIndex_());
/*
	THIS CODE USED TO BE IN HM_Page::PasteUp_()
	PMReal adWidth =  (adsToPlaceVector[theAd].GetWidth_());
	PMReal adHeight = (adsToPlaceVector[theAd].GetHeight_());
	pGeometryImporter->StoreArea_(adWidth, adHeight, folio_);
*/
	// The just-placed ad has a runaround so we have to re-H&J the story
	int32 startRetypesetFrameIndex = 
				currentStoryFrameList_->GetFirstDamagedFrameIndex ();
	if (startRetypesetFrameIndex != -1)
	{
		// Placing the ad has affected text flow, probably because of a 
		// runaround applied to the frame.  We have to re-H&J.
		LOG ("Re-HNJ");
		InterfacePtr<IFrameListComposer> frameListComposer (
									currentStoryFrameList_, UseDefaultIID());
		ASSERT (frameListComposer);
		frameListComposer->RecomposeThruNthFrame (-1);

		// If the story's overset, do something!
		if (Utils<ITextUtils>()->IsOverset (currentStoryFrameList_))
		{
			LOG ("OVERSET");
		}
	}

	LOG_END_FUNCTION;
}


// END OF FILE

