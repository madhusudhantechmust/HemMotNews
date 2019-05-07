/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	CL_NewsCadRect.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	9/9/05
	
	ab	Abstract:
	ab	Implementation of CL_NewsCadRect class for InDesign.
	
	cl	Change Log:
	cl	02-Dec-10	Brought forward to CS4; no changes needed!
	cl	09-Sep-05	New.
------------------------------------------------------------------------------*/

#include "CL_NewsCadRect_ID.h"
#include "CL_Log_File.h"
#include "global_data.h"

using namespace std;

//----------------------------------------------------------------------------------
//	FRectinFRect_
//
//	Checks to see if two rectangles intersect or touch and returns true if they do.
//----------------------------------------------------------------------------------
// bool CL_NewsCadRect::FRectinFRect_(PMRect& rectA, PMRect& rectB)
// {
// 	return IntersectOrAbut (rectA, rectB);
// }


/* -------------------------------- LOG + DEBUG --------------------------------- */

void CL_NewsCadRect::Log_()
{
#if VERSION == IN_HOUSE
	char logStr[256];
	::sprintf (logStr, 
				"<NewsCadRect x1 = \"%5.3f\"  y1 = \"%5.3f\"  w = \"%5.3f\""
				"h = \"%5.3f\"  x2 = \"%5.3f\"  y2 = \"%5.3f\"  area = \"%5.3f\"/>", 
				ToFloat (Left ()), ToFloat (Top ()), ToFloat (Width ()), 
				ToFloat (Height ()), ToFloat (Right ()), ToFloat (Bottom ()),
				ToFloat (AreaInt32_()) );
	LOG (logStr);
#endif
}

/* ---------------------------------- UTILITY ----------------------------------- */

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

// END OF FILE

