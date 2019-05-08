/* 
	CL_InDesignStyleSheetHelper.h

	Author:			
	Description:	<describe the CL_InDesignStyleSheetHelper class here>
*/

#ifndef CL_InDesignStyleSheetHelper_H
#define CL_InDesignStyleSheetHelper_H

//using namespace std;
//#include "LayoutUIUtils.h"
//#include "ILayoutUtils.h"
#include "ILayoutUIUtils.h"
#include "IDocument.h"
#include "PMString.h"
#include "IWorkspace.h"
#include "IStyleNameTable.h"
#include "IDocument.h"
#include "ICommand.h"
#include "IStyleInfo.h"
#include "ITextModel.h"



class CL_InDesignStyleSheetHelper {
public:
	//------------------------------------------------------------------------------
	// C O N S T R U C T I O N  and  D E S T R U C T I O N
	//------------------------------------------------------------------------------
	CL_InDesignStyleSheetHelper();										// Create
	virtual ~CL_InDesignStyleSheetHelper();								// Destroy

private:
	//------------------------------------------------------------------------------
	// P R E V E N T   C O P Y I N G   A N D   A S S I G N M E N T
	//------------------------------------------------------------------------------
	CL_InDesignStyleSheetHelper (										// Copy
				const CL_InDesignStyleSheetHelper & inObj)
	{
	
	}
	
	CL_InDesignStyleSheetHelper & operator= (							// Assign
				const CL_InDesignStyleSheetHelper & inObj)
	{
		if (this != &inObj)									// Beware of "a = a"
		{
			// put some code here
		}
	
		return *this;
	}
	
public:
	virtual UID FindByName_(PMString & inSSName) const;
	virtual UID	DuplicateParagraphStyle_(PMString & inSourceStyleName, 
											PMString & inNewStyleName) const;
	void ApplyStyleSheet_(ITextModel* inITextModel, UID inStyleUid,
						TextIndex inStartingOffset = 1, int32 inLength = 1) const;
	virtual UID CreateContinuedStyleSheet_();

};

#endif	// CL_InDesignStyleSheetHelper_H
