/*----------------------------------------------------------------------------------
	sn	Source Name:
	sn	CL_NewsCAD_Prefs.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	1/9/11 ca. 12:49 PM
	
	ab	Abstract:
	ab	Declarations for CL_NewsCAD_Prefs class.
	
	cl	Change Log:
	cl	1/9/11	New.
----------------------------------------------------------------------------------*/
#ifndef _NewsCAD_Prefs_H
#define _NewsCAD_Prefs_H

#include <CoreServices/CoreServices.h>
#include <iostream>
#include <fstream>
#include "misc.h"
#include "CL_Abstract_Prefs.h"
//typedef unsigned char uchar;
//typedef int int16;

class CL_NewsCAD_Prefs : public CL_Abstract_Prefs {
public:
	CL_NewsCAD_Prefs ();											// Create
	virtual	~CL_NewsCAD_Prefs ();									// Destroy

private:
	// Prevent copy and assign (for now).
	CL_NewsCAD_Prefs (const CL_NewsCAD_Prefs & inObj);				// Copy
	CL_NewsCAD_Prefs & operator= (const CL_NewsCAD_Prefs & inObj)	// Assign
	{
		if (this != &inObj)									// Beware of "a = a"
		{
			// put some code here
		}
	
		return *this;
	}


public:
	virtual void	LoadPreferencesFromFile_(char* fileName);			// Load from file
	virtual void	LoadPreferences_();			
	virtual void	StorePreferencesInFile_(char* inPrefsFileFullPath);	// Store in file
	virtual void	StorePreferencesDialogInput_(char* inFileName);
	void			SetPreference_(const char* inXPath, 
						const char* inXMLElem, 
						const TElementValue & inVal);
	// Getters
	bool	GetGrayOutline_()					{return grayOutline_;}
	void 	GetAdCoveragePercent_(uchar* s);
	void 	GetAdCoveragePercent_(char* s);
	void	GetGraphicsPath_(uchar* s);
	char*	GraphicsPath_() const				{ return pathToArtFiles_; }
	void	GetGraphicsPath_(char* s);
	void	GetRuleWeightStr_(uchar *s);
	void	GetRuleWeightStr_(char* ioCStr);

	// Setters
	void	SetGraphicsPath_(char* inCString);
	void	SetRuleWeightCStr_(char* s);
	void	SetGrayOutline_(int16 state)		{grayOutline_ = state;}
	void	SetGrayOutline_(bool state)			{grayOutline_ = state;}
	void	SetAdCoveragePercentCStr_(char* s);

protected:
	OSStatus		ComputeFullPathToPreferencesFile_();
	bool			DoesPrefsFileExistOnDisk_();
	
	// Preferences file
	char*			prefsFileName_;
	std::fstream	prefsFile_;

	// Preferences values
	char*		pathToArtFiles_;			// Graphics folder full path
	char*		ruleWeightCStr_;			// Rule weight for rules around ads
	bool		grayOutline_;				// TRUE if they want gray border around 
											// ads--default is TRUE
	char*		adCoveragePctCStr_;			// Maximum real estate ads can consume
											// on any page

private:
	void		GiveAlert_(char* inMsg, bool inIsError = false);
};



#endif

// END OF FILE
