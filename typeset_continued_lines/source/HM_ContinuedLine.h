/* 
	HM_ContinuedLine.h

	Author:			
	Description:	<describe the HM_ContinuedLine class here>
*/

#ifndef HM_ContinuedLine_H
#define HM_ContinuedLine_H

#include "UIDRef.h"
#include "PMString.h"

class HM_ContinuedLine {
public:
			HM_ContinuedLine();
	virtual	~HM_ContinuedLine();
	virtual bool Typeset_();
	virtual void SetBoxDepthToTextDepth_(UIDRef & textBoxUidRef);
	HM_ContinuedLine(UIDRef storyUidRef, PMString & text);
protected:
	UIDRef storyUidRef_;
	PMString text_;

};

#endif	// HM_ContinuedLine_H
