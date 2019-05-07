/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	CL_NewsCadRect.hpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	9 September 2005
	
	ab	Abstract:
	ab	Declarations for CL_NewsCadRect class.  Its reason for being is to 
	ab	encapsulate the code and data NewsCAD uses for placement of ads within 
	ab	rectangles.  
	ab
	ab	For QuarkXPress, it was sort of an amalgamation of a Mac Rect and a 
	ab	QuarkXPress fixedrect.
	ab
	ab	For InDesign, Adobe furnishes a class called PMRect, which uses doubles, 
	ab	and handles the conversions between that type and the system data types.
	
	cl	Change Log:
	cl	09-Sep-05	New.
----------------------------------------------------------------------------------*/

#ifndef CL_NEWSCAD_RECT_ID_H
#define	CL_NEWSCAD_RECT_ID_H
#include "PMRect.h"

class CL_NewsCadRect : public PMRect {
public:
			CL_NewsCadRect () : PMRect (0, 0, 0, 0)		{}
			CL_NewsCadRect (const PMReal& l, const PMReal& t, const PMReal& r, const PMReal& b)
				:	PMRect (l, t, r, b)
			{
			}
	virtual ~CL_NewsCadRect ()							{}
	
	inline PMReal	AreaPMReal_() const
	{
		return Width () * Height ();
	}
	
	inline int32	AreaInt32_()
	{
		return ToInt32 (AreaPMReal_() );
	}
	
	// Debugging
	virtual void	Log_();
	
	// Utility
	static bool FRectinFRect_(PMRect& rectA, PMRect& rectB)
	{
		return IntersectOrAbut (rectA, rectB);
	}
	
protected:


private:
};
	
// Plain C utility functions.
bool Compare_NewsCadRects_Area (const CL_NewsCadRect & a, const CL_NewsCadRect & b);
bool Compare_NewsCadRects_X (const CL_NewsCadRect & a, const CL_NewsCadRect & b);

bool Compare_NewsCadRects_X_LeftPage (
		const CL_NewsCadRect & a, const CL_NewsCadRect & b);

bool Compare_NewsCadRects_Y (const CL_NewsCadRect & a, const CL_NewsCadRect & b);
bool Compare_NewsCadRects_Y_Top (const CL_NewsCadRect & a, const CL_NewsCadRect & b);
bool are_rect_bounds_bad (CL_NewsCadRect & a);

#endif // CL_NEWSCAD_RECT_ID_H

// END OF FILE
