/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	TypesetContinuedLines.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	4/23/11 ca. 11:45 PM
	
	ab	Abstract:
	ab	Implementation of SOME class.
	ab	This file declares, implements, and makes callable by InDesign a class 
	ab	whose purpose is to construct a XXXXXXXX object and call a method 
	ab	in that class that will start the continued line typesetting process.
	
	cl	Change Log:
	cl	4/23/11	New.
----------------------------------------------------------------------------------*/

#include "CAlert.h"
#include "IStoryList.h"
#include "ILayoutUtils.h"
#include "ITextModel.h"
#include "IAttributeStrand.h"
#include "IStyleInfo.h"
#include "IMasterPage.h"
#include "IPageList.h"
#include "TextIterator.h"
#include "IFrameListComposer.h"
#include "IHierarchy.h"
#include "IDocument.h"

#include "newscad.h"
#include "TypesetContinuedLinesID.h"
#include "IAbstractContinuedLineTypesetter.h"
#include "CL_Document.h"
#include "HM_ClassifiedHeading.h"
#include "HM_Page.h"
#include "CL_Log_File.h"
#include "global_data.h"

#include <vector>

using namespace std;

class CL_TypesetContinuedLinesWorkhorse : 
			public CPMUnknown<IAbstractContinuedLineTypesetter> {
	public:
		CL_TypesetContinuedLinesWorkhorse (IPMUnknown* boss) 
			: CPMUnknown<IAbstractContinuedLineTypesetter>(boss)
		{}
		virtual void	TypesetContinuedLines_();

	protected:
		// Use this for putting string exceptions on the screen, or just for errors.
		inline void GiveAlert_(char* except)
		{
			PMString alertString (except);
			CAlert::ModalAlert
			(
				alertString,		// Text to display
				kOKString,			// OK button
				kNullString, 		// No second button
				kNullString, 		// No third button
				1,					// Set OK button to default
				CAlert::eErrorIcon
			);
		}

		virtual void	FindAllClassifiedHeadings_(UIDRef inStoryUIDRef, 
							IDataBase* inDocDatabase) throw (char*) ;
		virtual void	CreateContinuedLinesForStory_(UIDRef inStoryUIDRef, 
							IDataBase* inDocDatabase);
		virtual int		GetClassificationStartPageNum_(UIDRef inStoryUIDRef, 
							IDataBase* inDocDatabase, 
							vector<HM_ClassifiedHeading>::iterator iterHeading );
		virtual void	TypesetAndPasteUpContinuedLine_(int32 inPageNum, 
							int32 inNextClassFirstPageNum, 
							TextIndex nStart,
                            TextIndex nEnd,
							UIDRef inStoryUIDRef);

	private:
		vector<HM_ClassifiedHeading>	classifiedHeadingsVector_;
		int32 docPageCount_;
		
		inline int32 DocPageCount_()					{ return docPageCount_; }
		inline void SetPageCount_(int32 inPageCount)	{ docPageCount_ = inPageCount; }
};

// This is what enables our code to be called by InDesign:
CREATE_PMINTERFACE (CL_TypesetContinuedLinesWorkhorse, kTypesetContinuedLinesImpl)

#pragma mark -

void CL_TypesetContinuedLinesWorkhorse::TypesetContinuedLines_()
{
/*
	CAlert::ModalAlert
	(
		"BEGIN typesetting continued lines ...",
		kOKString, 					// OK button
		kNullString, 				// No second button
		kNullString, 				// No third button
		1,							// Set OK button to default
		CAlert::eInformationIcon	// Information icon.
	);
*/	
	// And this is where the rubber finally meets the road.
	OPEN_LOG_FILE;
	LOG_BEGIN_FUNCTION;
	try {
		// Iterate over all the stories in the document.  NewsCAD works only 
		// with the frontmost document -- the current one.
		auto_ptr<CL_Document> apCurrentDocument (new CL_Document);
		CL_Document* currentDocument = apCurrentDocument.get ();
		IDocument* iDocument = currentDocument->IDocument_();
		if (!iDocument) throw ("Could not get IDocument*.");

		// Get the document's database
		IDataBase* database = ::GetDataBase (iDocument);
		if (!database) throw ("Could not get IDataBase*.");

		// Get the list of stories in this document
		InterfacePtr<IStoryList> storyList (iDocument, UseDefaultIID());
		if (!storyList) throw ("Could not get IStoryList");

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
		::strcpy (exception_str, "TypesetContinuedLines_() EXCEPTION:  ");
		::strcat (exception_str, cstr);
		::strcat (exception_str, " Could not create continued lines.");
		LOG (exception_str);
		CAlert::ModalAlert
		(
			exception_str,
			kOKString, 				// OK button
			kNullString, 			// No second button
			kNullString, 			// No third button
			1,						// Set OK button to default
			CAlert::eErrorIcon		// Information icon.
		);
	}

	LOG_END_FUNCTION;
	CLOSE_LOG_FILE;
}


/*================================================================================*/
void CL_TypesetContinuedLinesWorkhorse::FindAllClassifiedHeadings_(
				UIDRef inStoryUIDRef, IDataBase* inDocDatabase) throw (char*) {
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


/*================================================================================*/
void CL_TypesetContinuedLinesWorkhorse::CreateContinuedLinesForStory_(
									UIDRef inStoryUIDRef, IDataBase* inDocDB) {
/*----------------------------------------------------------------------------------
	Abstract:
		Loop over the classifiedHeadingsVector_ and set continued lines as needed.
		
	Parameters and modes:
		
	Returns:

	Change Log:
		10-May-11	Begin port to CS4.
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
                HM_ClassifiedHeading oClassifiedHeading = *p;
                TextIndex nStartOffset = oClassifiedHeading.StartingOffset_();
                TextIndex nEndOffset = oClassifiedHeading.EndingOffset_();
				TypesetAndPasteUpContinuedLine_(nextLeftPageNum, endPageNum, nStartOffset, nEndOffset, inStoryUIDRef
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
					// EMPTY
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
int CL_TypesetContinuedLinesWorkhorse::GetClassificationStartPageNum_(
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
			auto_ptr<CL_Document> apCurrentDocument (new CL_Document);
			CL_Document* curDoc = apCurrentDocument.get ();
			IDocument* frontDoc = curDoc->IDocument_();
//			if (!frontDoc) throw ("Could not get IDocument*.");
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


/*================================================================================*/
void CL_TypesetContinuedLinesWorkhorse::TypesetAndPasteUpContinuedLine_(
                   int32 inPageNum,
                   int32 inNextClassFirstPageNum,
                   TextIndex nStart,
                   TextIndex nEnd,
                   UIDRef inStoryUIDRef/*, Rect *inNextHeadRect,
			boxid inBoxID, long inTextStart, long inTextEnd*/)
{
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
	auto_ptr<CL_Document> apCurrentDocument (new CL_Document);
	CL_Document* curDoc = apCurrentDocument.get ();
	IDocument* frontDocument = curDoc->IDocument_();
	InterfacePtr<IPageList> pageList (frontDocument, UseDefaultIID());
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
        TextIterator firstCh (textModel, nStart);// inClassHeading.StartingOffset_());
        TextIterator lastCh  (textModel, nEnd);//inClassHeading.EndingOffset_());
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
