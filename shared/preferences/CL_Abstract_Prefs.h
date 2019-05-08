/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	CL_Basic_XT_Prefs.h

	au	Author:
	au	Ross L. Edgar

	da	Date
	da	April 23, 2005.

	ab	Abstract:
	ab	Declares basic preferences class for QuarkXTensions.  This is an abstract 
	ab	base class and thus cannot be instantiated.  An XTensions module must 
	ab	inherit this class and extend the class for the XT's particular use.

	cl	Change Log:
	cl	06-Jan-10	Began rewrite for CS4.
	cl	23-Apr-05	New.
----------------------------------------------------------------------------------*/

#ifndef CL_Abstract_Prefs_H
#define CL_Abstract_Prefs_H

#include <cstring>												// For char* & NULL

class CL_Abstract_Prefs {
public:
	CL_Abstract_Prefs ()										// Create
		: prefsFileFullPath_(NULL)
	{
		prefsFileFullPath_ = new char [1024];
	}
	virtual	~CL_Abstract_Prefs ()								// Destroy
	{
		delete [] prefsFileFullPath_;
	}

private:
	// For now, disable copying and assignment.
	CL_Abstract_Prefs (const CL_Abstract_Prefs & inObj)			// Copy
	{}
	CL_Abstract_Prefs & operator=(
							const CL_Abstract_Prefs & inObj)	// Assign
	{
		if (this != &inObj)									// Beware of "a = a"
		{
			// put some code here
		}
	
		return *this;
	}

public:
	virtual void	LoadPreferences_() = 0;						// Load (from file?)
	virtual void	StorePreferencesDialogInput_(
						char* fileName) = 0;					// Store (in file?)
	
protected:
	// Prefs file data
	char* 	prefsFileFullPath_;									// Prefs file
};

#endif

// END OF FILE