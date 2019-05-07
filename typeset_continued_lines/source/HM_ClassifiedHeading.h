/*------------------------------------------------------------------------------	sn	Source Name:	sn	HM_ClassifiedHeading.h — SDS_Plugins.bbprojectd		au	Author:	au	Ross L. Edgar		da	Date	da	5/3/11 ca. 9:21 PM		ab	Abstract:	ab	Declarations for HM_ClassifiedHeading.h — SDS_Plugins.bbprojectd class.		cl	Change Log:	cl	5/3/11	Began port to CS4.------------------------------------------------------------------------------*/#ifndef	__HM_CLASSIFIED_HEADING_H__#define	__HM_CLASSIFIED_HEADING_H__#pragma onceclass HM_ClassifiedHeading {	public:		// Construction		HM_ClassifiedHeading ()											// Create			:	start_(0)			,	end_(0)			,	textBoxUID_(kInvalidUID)			,	pageUID_(kInvalidUID)			,	pageType_(kUnisexPage)			,	pageIndex_(0)		{			// EMPTY		}		HM_ClassifiedHeading (TextIndex inStart, TextIndex inEnd)		// Create			:	textBoxUID_(kInvalidUID)			,	pageUID_(kInvalidUID)			,	pageType_(kUnisexPage)			, pageIndex_(0)		{			start_ = inStart;			end_ = inEnd;		}				// Destruction		virtual	~HM_ClassifiedHeading ()								// Destroy		{			// Nothing yet.		}		// We don't need to write copy/assign for now -- 		// compiler-generated default bitwise copy/assign good enough.		/*		HM_ClassifiedHeading (const HM_ClassifiedHeading & inObj);		// Copy		HM_ClassifiedHeading & operator= (								const HM_ClassifiedHeading & inObj)		// Assign		{			return *this;		}		*/		// Setters		void	SetStartingOffset_(TextIndex inStart)	{ start_ = inStart; }		void	SetEndingOffset_(TextIndex inEnd)		{ end_   = inEnd;   }		void	SetPageUID_(UID inPageUID)				{ pageUID_ = inPageUID; }		void	SetTextBoxUID_(UID inTextBoxUID)							{ textBoxUID_ = inTextBoxUID; }		void	SetPageType_(PageType inPageType)		{ pageType_ = inPageType; }		void	SetPageIndex_(int32 inPageIndex)		{ pageIndex_ = inPageIndex; }				// Getters		TextIndex	StartingOffset_() const				{ return start_; }		TextIndex	EndingOffset_()  const				{ return end_;   }		inline UID	TextBoxUID_() const					{ return textBoxUID_; }		inline UID	PageUID_() const					{ return pageUID_; }		inline PageType PageType_() const				{ return pageType_; }		inline int32 PageIndex_() const					{ return pageIndex_; }				virtual void ComputeDataForContinuedLine_(							UIDRef inStoryUIDRef, IDataBase* inDocDatabase);	protected:		private:		TextIndex	start_, end_;			// Offset in text stream of heading		UID			textBoxUID_;			// InDesign identifier of container		UID			pageUID_;				// ... and of its page		int32		pageIndex_;				// Location of page within the pub		PageType	pageType_;				// Left, right, or Unisex};#endif// END OF FILE