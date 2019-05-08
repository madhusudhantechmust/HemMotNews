/* 	InDesignTextFrameOptionsManipulator.h	Author:				Description:	<describe the InDesignTextFrameOptionsManipulator class here>*/#ifndef InDesignTextFrameOptionsManipulator_H#define InDesignTextFrameOptionsManipulator_H#include "ITextFrame.h"#include "ITextInset.h"#include "UIDRef.h"#include "PMReal.h"#include "PMRect.h"class InDesignTextFrameOptionsManipulator {public:	InDesignTextFrameOptionsManipulator();	virtual ~InDesignTextFrameOptionsManipulator();	virtual ErrorCode SetAndApply_(UIDRef& inGraphicFrameUidRef, 							PMReal inLeft, PMReal inTop, 							PMReal inRight, PMReal inBottom);protected:	UIDRef graphicFrameUIDRef_;	PMReal left_, top_, right_, bottom_;};#endif	// InDesignTextFrameOptionsManipulator_H