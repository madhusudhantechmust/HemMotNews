/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	CL_Document.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	12/08/02 ca. 4:39 PM
	
	ab	Abstract:
	ab	Implementation of CL_Document.cpp class.
	
	cl	Change Log:
	cl	12/08/02	New.
------------------------------------------------------------------------------*/
#include <vector>
#include "ILayoutUIUtils.h"
#include "IPageList.h"
#include "IDocument.h"
#include "newscad.h"
#include "CL_Document.h"
#include "CL_Display_Ad.h"
#include "global_data.h"
#include "CL_Log_File.h"

using std::vector;

//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and  D E S T R U C T I O N
//----------------------------------------------------------------------------------
CL_Document::CL_Document ()
	:	docRef_(NULL)
{
	placedAdsVector_.reserve (512);
	Update_();
}

CL_Document::~CL_Document ()
{

}

void CL_Document::Update_()
{
	docRef_ = Utils<ILayoutUIUtils>()->GetFrontDocument();
}

int16 CL_Document::GetMasterPageIndexFromName_(const char* inName) const
{
	return 23;
}

void CL_Document::GetPageCount_(int16* ioNumPages) const
{
	*ioNumPages = 0;
	IDocument* frontDoc = Utils<ILayoutUIUtils>()->GetFrontDocument();
	if (docRef_)
	{
		InterfacePtr<IPageList> pageList (
									frontDoc, UseDefaultIID());
		if (pageList)
		{
			*ioNumPages = pageList->GetPageCount ();
		}
	}
}


// END OF FILE
