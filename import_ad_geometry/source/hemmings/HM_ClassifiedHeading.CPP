using namespace std;
#include <cstring>

#include "ITextModel.h"
#include "IAttributeStrand.h"
#include "ILayoutUIUtils.h"
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

#include "HM_ClassifiedHeading.h"
#include "CL_Log_File.h"
#include "global_data.h"

void HM_ClassifiedHeading::ComputeDataForContinuedLine_(
						UIDRef inStoryUIDRef, IDataBase* inDocDatabase)
{
	LOG_BEGIN_FUNCTION;
	DECLARE_logstr;
	
	InterfacePtr<ITextModel> pITextModel (inStoryUIDRef, UseDefaultIID());
	if ( ! pITextModel )
	{
		LOG_END_FUNCTION;
		throw ("ComputeDataForContinuedLine_(): could not get ITextModel.");
	}
	
	RangeData textRange (StartingOffset_(), EndingOffset_());
	int32 firstFrameIndex, lastFrameIndex;
	Utils<ITextUtils>()->GetTextFramesContaining (pITextModel, textRange, 
												&firstFrameIndex, &lastFrameIndex);
	
	// Get UID of frame that contains this classified heading.
	IFrameList* frameList = pITextModel->QueryFrameList ();
	if (!frameList) throw "IFrameList";
	UID textBoxUID = frameList->GetNthFrameUID ( firstFrameIndex );
	SetTextBoxUID_(textBoxUID);
	
	// Figure out which page and spread contains that frame.
	InterfacePtr<IPMUnknown> unknown (inDocDatabase, textBoxUID, IID_IUNKNOWN);
	InterfacePtr<IHierarchy> hierarchy (unknown, UseDefaultIID());
	if (hierarchy)
	{
		UID pageUID = Utils<ILayoutUtils>()->GetOwnerPageUID (hierarchy);
		SetPageUID_(pageUID);
		InterfacePtr<IPMUnknown> unknown2 (inDocDatabase, pageUID, 
												IID_IUNKNOWN);
		InterfacePtr<IMasterPage> masterPage (unknown2, IID_IMASTERPAGE);
		if (masterPage)
		{
			IDocument* frontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument ();
			// 
			if (frontDoc)
			{
				InterfacePtr<IPageList> pageList (
											frontDoc, UseDefaultIID());
				if (pageList)
				{
					SetPageIndex_(pageList->GetPageIndex (pageUID));
					SetPageType_(pageList->GetPageType (pageUID));
				}
			}
		}
		// Use for debugging
		LOG ("<ClassifiedHeadingData>");
		LOG_TAG_AND_VALUE ("StartChar", int (StartingOffset_()));
		LOG_TAG_AND_VALUE ("EndChar", int (EndingOffset_()));
		::sprintf (logstr, "<TextBoxUid>0x%x</TextBoxUid>", TextBoxUID_());
		LOG (logstr);
		::sprintf (logstr, "<PageUid>0x%x</PageUid>", PageUID_());
		LOG (logstr);
		LOG_TAG_AND_VALUE ("PageIndex", PageIndex_());
		switch (PageType_()) {
			case kLeftPage:		LOG_TAG_AND_VALUE ("PageType", "LEFT");		break;
			case kUnisexPage:	LOG_TAG_AND_VALUE ("PageType", "UNISEX");	break;
			case kRightPage:	LOG_TAG_AND_VALUE ("PageType", "RIGHT");	break;
		}
		LOG ("</ClassifiedHeadingData>");
	}
	else
	{
		// Not sure whether this is a serious error or not ...
		LOG ("Could not getIHierarchy");
	}
	
	LOG_END_FUNCTION;
}


/*
from:
void HM_Geometry_Importer::ProcessStory_(UIDRef inStoryUIDRef)	throw (char*) {

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
*/
