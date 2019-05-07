
#include "VCPlugInHeaders.h"

#include "CPMUnknown.h"

// ----- Interfaces files
#include "NCSelectionSuite.h"
#include "IIntegratorTarget.h"

// ----- ID files
#include "NCPutBackPanelID.h"

#include "SelectionASBTemplates.tpp"	// for make_functor


class NCSelSuiteASB : public CPMUnknown<INCPutBackSelSuite>
{
	public:

		NCSelSuiteASB(IPMUnknown* boss);
		
		virtual ~NCSelSuiteASB();

		virtual ErrorCode GetTheSelection( UIDList** selection );
};

CREATE_PMINTERFACE (NCSelSuiteASB, kNCSelectionSuiteASBImpl)


NCSelSuiteASB::NCSelSuiteASB(IPMUnknown* boss) :
		CPMUnknown<INCPutBackSelSuite>(boss)
{
}

NCSelSuiteASB::~NCSelSuiteASB()
{
}

ErrorCode NCSelSuiteASB::GetTheSelection (UIDList** selection)	
{
	return (Process (
				make_functor (&INCPutBackSelSuite::GetTheSelection, selection), 
				this, IID_NCSELSUITE));
}


