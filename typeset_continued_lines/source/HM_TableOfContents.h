/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	HM_TableOfContents.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	3/19/07 ca. 8:29 PM
	
	ab	Abstract:
	ab	Declarations for HM_TableOfContents class.
	ab	An object of this class scans the document text, looks for classified 
	ab	ad headings, and writes out a text file showing each heading and the 
	ab	containing page.
	
	cl	Change Log:
	cl	3/19/07	New.
----------------------------------------------------------------------------------*/

#pragma once

#include <vector>
#include <fstream>
#include "IDFile.h"
#include "TextIterator.h"
#include "IPageList.h"
#include "HM_ClassifiedHeading.h"

class HM_TableOfContents {
public:
		HM_TableOfContents ();											// Create
		virtual	~HM_TableOfContents ();									// Destroy

private:
		// Prevent copying and assignment
		HM_TableOfContents (const HM_TableOfContents & inObj)
			:	docPageCount_(-1)									// Copy
		{
			// NULL
		}
																		// Assign
		HM_TableOfContents & operator= (
							const HM_TableOfContents & inObj)
		{
			if (this != &inObj)									// Beware of "a = a"
			{
				// put some code here
			}
		
			return *this;
		}

public:
	virtual void 	Run_();

protected:
	virtual void	ScanDocumentText_() throw (char*) ;
	virtual void	ProcessStory_(UIDRef inStoryUIDRef, 
							IDataBase* inDocDatabase) throw (char*);
	virtual void	OpenOutputFile_();
	virtual void	WriteRecord_(PMString & inClassifiedHeading, 
									PMString & inPageStr);
	virtual void	CloseOutputFile_();
	virtual bool	ChooseOutputFile_(bool & ioDoesFileExist) throw (char*);
	virtual void	WriteXPressTagsFileInitialization_();
	
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
	
	virtual void	MakeContinuedLines_() throw (char*) ;
	virtual void	FindAllClassifiedHeadings_(UIDRef inStoryUIDRef, 
							IDataBase* inDocDatabase) throw (char*) ;
	virtual void	CreateContinuedLinesForStory_(UIDRef inStoryUIDRef, 
							IDataBase* inDocDatabase);
	virtual int		GetClassificationStartPageNum_(UIDRef inStoryUIDRef, 
							IDataBase* inDocDatabase, 
							vector<HM_ClassifiedHeading>::iterator iterHeading );
	virtual void	TypesetAndPasteUpContinuedLine_(int32 inPageNum, 
							int32 inNextClassFirstPageNum, 
							HM_ClassifiedHeading& inClassHeading, 
							UIDRef inStoryUIDRef);
	IDFile	tocFile_;
    std::fstream    m_oTOCFile;
	bool	okToProcess_;
	vector<HM_ClassifiedHeading>	classifiedHeadingsVector_;
	int32 docPageCount_;

	inline int32 DocPageCount_()					{ return docPageCount_; }
	inline void SetPageCount_(int32 inPageCount)	{ docPageCount_ = inPageCount; }

private:
};


// END OF FILE
