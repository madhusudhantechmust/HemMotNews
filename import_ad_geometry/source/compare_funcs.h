#ifndef	_DECLARE_COMPARE_FUNCTIONS_
#define	_DECLARE_COMPARE_FUNCTIONS_
#include "CL_NewsCadRect_ID.h"
extern "C" {
	// Plain C utility functions.
	bool Compare_NewsCadRects_Area (const CL_NewsCadRect & a, const CL_NewsCadRect & b);
	bool Compare_NewsCadRects_X (const CL_NewsCadRect & a, const CL_NewsCadRect & b);
	
	bool Compare_NewsCadRects_X_LeftPage (
			const CL_NewsCadRect & a, const CL_NewsCadRect & b);
	
	bool Compare_NewsCadRects_Y (const CL_NewsCadRect & a, const CL_NewsCadRect & b);
	bool Compare_NewsCadRects_Y_Top (const CL_NewsCadRect & a, const CL_NewsCadRect & b);
	bool are_rect_bounds_bad (CL_NewsCadRect & a);
};
#endif
