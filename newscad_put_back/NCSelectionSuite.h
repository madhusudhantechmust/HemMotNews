//========================================================================================
//  
//  $File: IMySelectionSuite.h $
//  
//  Owner: Adobe Developer Technologies
//  
//  $DateTime: 2004/02/20 15:41:37 $
//  
//  $Revision: #1 $
//  
//  $Change: 223184 $
//  
//  Copyright 1997-2004 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================

#ifndef __INCPutBackSelSuite_h__
#define __INCPutBackSelSuite_h__

// Interface includes:
#include "IPMUnknown.h"
#include "NCPutBackPanelID.h"

class INCPutBackSelSuite : public IPMUnknown
{
public:
	enum { kDefaultIID = IID_NCSELSUITE };
	
	virtual ErrorCode GetTheSelection ( UIDList** selection ) = 0;
};

#endif	// __INCPutBackSelSuite_h__
