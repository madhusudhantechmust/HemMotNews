
#include "compare_funcs.h"

bool Compare_NewsCadRects_Area (
			const CL_NewsCadRect & a, const CL_NewsCadRect & b)
{
	return ( a.AreaPMReal_() > b.AreaPMReal_() );
}


bool Compare_NewsCadRects_X (const CL_NewsCadRect & a, const CL_NewsCadRect & b)
{
	return ( a.Left () > b.Left () );
}

bool Compare_NewsCadRects_X_LeftPage (
		const CL_NewsCadRect & a, const CL_NewsCadRect & b)
{
	return ( a.Left () < b.Left () );
}

bool Compare_NewsCadRects_Y (const CL_NewsCadRect & a, const CL_NewsCadRect & b)
{
	return ( a.Bottom () > b.Bottom () );
}

bool Compare_NewsCadRects_Y_Top (const CL_NewsCadRect & a, const CL_NewsCadRect & b)
{
	return ( a.Top () < b.Top () );
}

bool are_rect_bounds_bad (CL_NewsCadRect & a)
{
	if (a.AreaPMReal_() < 0)
 		return true;
 
	return false;
/*
	return (a.GetQuarkRectHeight_() < FIX (2)	|| 
			a.GetQuarkRectWidth_() < FIX (2)	||
			a.GetMacHeight_() < 2 || a.GetMacWidth_() < 2);
*/
}
