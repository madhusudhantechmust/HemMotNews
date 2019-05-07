/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	NCSelectionSuiteCSB.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	3/9/06 ca. 11:13 PM
	
	ab	Abstract:
	ab	Implementation of NCSelectionSuiteCSB class.
	ab	Most of this stuff came from Adobe.  Their tech support guy tried to 
	ab	explain it, but it's a hopeless mess of spaghetti.
	
	cl	Change Log:
	cl	3/9/06	New.
------------------------------------------------------------------------------*/
/*
#include "NCSelectionSuiteCSB.hpp"

//----------------------------------------------------------------------------------
// C O N S T R U C T I O N  and D E S T R U C T I O N
//----------------------------------------------------------------------------------
CL_NCSelectionSuiteCSB::CL_NCSelectionSuiteCSB ()
{

}

CL_NCSelectionSuiteCSB::~CL_NCSelectionSuiteCSB ()
{

}
*/

#include "VCPlugInHeaders.h"

// ----- Interfaces files
#include "NCSelectionSuite.h"
#include "ILayoutTarget.h"

#include "ISelectionManager.h"

#include "NCPutBackPanelID.h"


class NCSelSuiteCSB : public CPMUnknown<INCPutBackSelSuite>
{
	public:
		/** Constructor
		@param boss
		*/
		NCSelSuiteCSB(IPMUnknown* boss);

		 /** Destructor */
		virtual ~NCSelSuiteCSB(void);
		
		virtual ErrorCode GetTheSelection( UIDList** selection );

};

CREATE_PMINTERFACE (NCSelSuiteCSB, kNCSelectionSuiteCSBImpl)

NCSelSuiteCSB::NCSelSuiteCSB(IPMUnknown* boss) :
		CPMUnknown<INCPutBackSelSuite>(boss)
{
}

NCSelSuiteCSB::~NCSelSuiteCSB(void)
{
}

//
// return a list of currently-selected page items

ErrorCode NCSelSuiteCSB::GetTheSelection (UIDList** selection)	
{
	ErrorCode error = kFailure;
	
	do
	{
		if ( selection != NULL ) *selection = NULL;
		else return kFailure;
		
		UIDList*		selectUIDList=NULL;
		
		InterfacePtr<ILayoutTarget>	iLayoutTarget(this, UseDefaultIID ());
		if ( iLayoutTarget != NULL )
		{	
			// we have a layout selection
			selectUIDList = new UIDList(iLayoutTarget->GetUIDList(kStripStandoffs));
			error = kSuccess;
		}
	
		if ( selectUIDList )
		{
			*selection = selectUIDList;
		}
	}
	while( false );
	
	return error;
}




