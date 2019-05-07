//========================================================================================
//  
//  $File: $
//  
//  Owner: Ross Edgar
//  
//  $Author: $
//  
//  $DateTime: $
//  
//  $Revision: $
//  
//  $Change: $
//  
//  Copyright 1997-2008 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================


#ifndef __NC_ID_h__
#define __NC_ID_h__

#include "SDKDef.h"

/* ------------------------------- C O M P A N Y -------------------------------- */

#define kNC_CompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kNC_CompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.


/* ---------------------------------- PLUG-IN ----------------------------------- */

#define kNC_PluginName	"NewsCAD"			// Name of this plug-in.
#define kNC_PrefixNumber	0xd9940 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
// Next 2 are for the About box.
#define kNC_Version		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
#define kNC_Author		"Ross Edgar"					// Author of this plug-in (for the About Box).

/* ------------------------ P L U G - I N   P R E F I X ------------------------- */

#define kNC_Prefix		RezLong(kNC_PrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kNC_StringPrefix	SDK_DEF_STRINGIZE(kNC_PrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

/* ----------------------- M I S S I N G   P L U G - I N ------------------------ */
// (see ExtraPluginInfo resource)
#define kNC_MissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kNC_MissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

/* ---------------------------------- PluginID ---------------------------------- */

DECLARE_PMID(kPlugInIDSpace, kNC_PluginID, kNC_Prefix + 0)


/* ---------------------------------- ClassIDs ---------------------------------- */

DECLARE_PMID(kClassIDSpace,	kNC_ActionComponentBoss,	kNC_Prefix + 0)
DECLARE_PMID(kClassIDSpace,	kCreateAdDialogBoss,		kNC_Prefix + 3)
DECLARE_PMID(kClassIDSpace, kPreferencesDialogBoss,		kNC_Prefix + 4)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 5)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 6)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 7)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 8)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 9)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 10)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 11)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 12)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 13)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 14)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 15)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 16)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 17)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 18)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 19)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 20)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 21)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 22)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 23)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 24)
//DECLARE_PMID(kClassIDSpace, kNC_Boss, kNC_Prefix + 25)


/* -------------------------------- InterfaceIDs -------------------------------- */

//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 0)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 1)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 2)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 3)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 4)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 5)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 6)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 7)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 8)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_INC_INTERFACE, kNC_Prefix + 25)



/* ----------------------------- ImplementationIDs ------------------------------ */

DECLARE_PMID(kImplementationIDSpace, kNC_ActionComponentImpl,	kNC_Prefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kCreateAdOnTheFly_DialogControllerImpl,	kNC_Prefix + 1)
DECLARE_PMID(kImplementationIDSpace, kCreateAdOnTheFly_DialogObserverImpl,		kNC_Prefix + 2)
DECLARE_PMID(kImplementationIDSpace, kPreferences_DialogControllerImpl, kNC_Prefix + 3)
DECLARE_PMID(kImplementationIDSpace, kPreferences_DialogObserverImpl, kNC_Prefix + 4)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 5)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 6)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 7)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 8)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 9)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 10)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 11)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 12)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 13)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 14)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 15)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 16)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kNC_Impl, kNC_Prefix + 25)


// ActionIDs:
DECLARE_PMID(kActionIDSpace, kNC_AboutActionID, kNC_Prefix + 0)
//DECLARE_PMID(kActionIDSpace, kNC_ActionID, kNC_Prefix + 5)
//DECLARE_PMID(kActionIDSpace, kNC_ActionID, kNC_Prefix + 6)
//DECLARE_PMID(kActionIDSpace, kNC_ActionID, kNC_Prefix + 7)
//DECLARE_PMID(kActionIDSpace, kNC_ActionID, kNC_Prefix + 8)
//DECLARE_PMID(kActionIDSpace, kNC_ActionID, kNC_Prefix + 9)
DECLARE_PMID(kActionIDSpace, kNC_ImportAdGeometry_ActionID, kNC_Prefix + 10)
DECLARE_PMID(kActionIDSpace, kNC_ImportArtFiles_ActionID, kNC_Prefix + 11)
DECLARE_PMID(kActionIDSpace, kNC_CreateAdReport_ActionID, kNC_Prefix + 12)
DECLARE_PMID(kActionIDSpace, kNC_ContinuedLines_ActionID, kNC_Prefix + 13)
DECLARE_PMID(kActionIDSpace, kNC_CreateAdOnTheFly_ActionID, kNC_Prefix + 14)
DECLARE_PMID(kActionIDSpace, kNC_Preferences_ActionID, kNC_Prefix + 15)
DECLARE_PMID(kActionIDSpace, kNC_Separator1ActionID, kNC_Prefix + 16)
DECLARE_PMID(kActionIDSpace, kNC_Separator2ActionID, kNC_Prefix + 17)
DECLARE_PMID(kActionIDSpace, kNC_Separator3ActionID, kNC_Prefix + 18)
DECLARE_PMID(kActionIDSpace, kNC_Separator4ActionID, kNC_Prefix + 19)
//DECLARE_PMID(kActionIDSpace, kNC_ActionID, kNC_Prefix + 20)
//DECLARE_PMID(kActionIDSpace, kNC_ActionID, kNC_Prefix + 21)
//DECLARE_PMID(kActionIDSpace, kNC_ActionID, kNC_Prefix + 22)
//DECLARE_PMID(kActionIDSpace, kNC_ActionID, kNC_Prefix + 23)
//DECLARE_PMID(kActionIDSpace, kNC_ActionID, kNC_Prefix + 24)
//DECLARE_PMID(kActionIDSpace, kNC_ActionID, kNC_Prefix + 25)


/* --------------------------------- WidgetIDs ---------------------------------- */

// The "Create Ad On The Fly ..." dialog:
DECLARE_PMID(kWidgetIDSpace, kCreateAdDialog_WidgetID,			kNC_Prefix + 2)
DECLARE_PMID(kWidgetIDSpace, kWidthLabel_WidgetID,				kNC_Prefix + 3)
DECLARE_PMID(kWidgetIDSpace, kColumnsLabel_WidgetID,			kNC_Prefix + 4)
DECLARE_PMID(kWidgetIDSpace, kColumnCountEditText_WidgetID,		kNC_Prefix + 5)
DECLARE_PMID(kWidgetIDSpace, kDepthLabel_WidgetID,				kNC_Prefix + 6)
DECLARE_PMID(kWidgetIDSpace, kDepthEditText_WidgetID,			kNC_Prefix + 7)
DECLARE_PMID(kWidgetIDSpace, kClientNameLabel_WidgetID,			kNC_Prefix + 8)
DECLARE_PMID(kWidgetIDSpace, kClientEditText_WidgetID,			kNC_Prefix + 9)
DECLARE_PMID(kWidgetIDSpace, kArtFileLabel_WidgetID,			kNC_Prefix + 10)
DECLARE_PMID(kWidgetIDSpace, kArtFileNameEditText_WidgetID,		kNC_Prefix + 11)
DECLARE_PMID(kWidgetIDSpace, kSalespersonNameEditText_WidgetID, kNC_Prefix + 12)
DECLARE_PMID(kWidgetIDSpace, kSalespersonNameLabel_WidgetID,	kNC_Prefix + 13)

// The Preferences... dialog
DECLARE_PMID(kWidgetIDSpace, kPreferencesDialog_WidgetID,		kNC_Prefix + 14)

// The drop-down list (popup menu on Mac) in the dialog
DECLARE_PMID(kWidgetIDSpace, kPreferencesDropDownList_WidgetID,	kNC_Prefix + 15)

// Text Edit Box
DECLARE_PMID(kWidgetIDSpace, kPreferencesTextEditBox_WidgetID,				kNC_Prefix + 16)

// Static text widgets
DECLARE_PMID(kWidgetIDSpace, kPreferencesStaticText_WidgetID,				kNC_Prefix + 17)

// Label for rule weight combo box:  "Rule Weight:"
DECLARE_PMID(kWidgetIDSpace, kPreferencesRuleWeightComboBoxLabel_WidgetID,	kNC_Prefix + 18)

// Label for text field: "Current Graphics Folder:"
DECLARE_PMID(kWidgetIDSpace, kPreferencesGraphicsFolderLabel_WidgetID,		kNC_Prefix + 19)

// Browse for folder button
DECLARE_PMID(kWidgetIDSpace, kPreferencesChangeGraphicsFolderBtn_WidgetID,	kNC_Prefix + 20)

// TextEditBox -- path to graphics folder
DECLARE_PMID(kWidgetIDSpace, kPreferencesChangeFolderEditText_WidgetID,		kNC_Prefix + 21)

// Checkbox -- Gray Ad Outline
DECLARE_PMID(kWidgetIDSpace, kPreferencesGrayAdOutlineCheckBox_WidgetID,	kNC_Prefix + 22)

// Label for edit text field: "Ad Coverage:"
DECLARE_PMID(kWidgetIDSpace, kPreferencesAdCoverageStaticText_WidgetID,		kNC_Prefix + 23)

// Edit Text -- ad coverage percentage
DECLARE_PMID(kWidgetIDSpace, kPreferencesAdCoverageEditText_WidgetID,		kNC_Prefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kPreferences_WidgetID, kNC_Prefix + 25)

/* ------------------------- "About Plug-ins" sub-menu -------------------------- */

#define kNC_AboutMenuKey			kNC_StringPrefix "kNC_AboutMenuKey"
#define kNC_AboutMenuPath			kSDKDefStandardAboutMenuPath kNC_CompanyKey


/* ---------------------------- "Plug-ins" sub-menu: ---------------------------- */
/*
	I'm removing the next 2 lines generated by DollyXs
	We are going to try putting our own NewsCAD menu in the menu bar
	rather than putting our stuff on the Adobe Plugins menu
#define kNC_PluginsMenuKey 		kNC_StringPrefix "kNC_PluginsMenuKey"
#define kNC_PluginsMenuPath		kSDKDefPlugInsStandardMenuPath kNC_CompanyKey kSDKDefDelimitMenuPath kNC_PluginsMenuKey
*/

/* -------------------------------------------------------------------------------++
++	BEGIN CODE TO PUT OUR MENU IN THE MAIN MENU BAR (SEE COMMENTS JUST ABOVE)
++--------------------------------------------------------------------------------*/

/* ------------------------------- Menu item keys ------------------------------- */

// defines 'NewsCAD' item in menubar
#define kNewsCADMenuKey 		kNC_StringPrefix "kNewsCADMenuKey"
#define kNewsCADMenuPath		"Main:" kNewsCADMenuKey // the path
#define kNewsCADMenuPathKey		kNewsCADMenuPath ":"
// define the submenus under the NewsCAD menu
#define kNC_ImportAdGeometryMenuKey		kNC_StringPrefix "kNC_ImportAdGeometryMenuKey"
#define kNC_ImportArtFilesMenuKey		kNC_StringPrefix "kNC_ImportArtFilesMenuKey"
#define kNC_CreateAdReportMenuKey		kNC_StringPrefix "kNC_CreateAdReportMenuKey"
#define kNC_ContinuedLinesMenuKey		kNC_StringPrefix "kNC_ContinuedLinesMenuKey"
#define kNC_CreateAdOnTheFlyMenuKey		kNC_StringPrefix "kNC_CreateAdOnTheFlyMenuKey"
#define kNC_PreferencesMenuKey			kNC_StringPrefix "kNC_PreferencesMenuKey"

/* -------------------------------------------------------------------------------++
++	END CODE TO PUT OUR MENU IN THE MAIN MENU BAR
++--------------------------------------------------------------------------------*/


/* ------------------------------ Other StringKeys ------------------------------ */
// Create Ad On The Fly ...
#define kNC_AboutBoxStringKey		kNC_StringPrefix	"kNC_AboutBoxStringKey"
#define kNC_TargetMenuPath			kNC_PluginsMenuPath
#define kNC_CreateAdDialogTitleKey	kNC_StringPrefix	"kNC_CreateAdDialogTitleKey"
#define	kWidthLabelKey				kNC_StringPrefix	"kWidthLabelKey"
#define	kColumnsWideLabelKey		kNC_StringPrefix	"kColumnsWideLabelKey"
#define	kDepthLabelKey				kNC_StringPrefix	"kDepthLabelKey"
#define	kClientNameLabelKey			kNC_StringPrefix	"kClientNameLabelKey"
#define	kArtFileNameLabelKey		kNC_StringPrefix	"kArtFileNameLabelKey"
#define	kSalespersonNameLabelKey	kNC_StringPrefix	"kSalespersonNameLabelKey"

#if 0
/* -------------------------- Static Text String Keys --------------------------- */
// Preferences ...
// Preferences Dialog Drop Down List
#define kPrefsDropDownItem_1_Key	kNC_StringPrefix "kPrefsDropDownItem_1_Key"
#define kPrefsDropDownItem_2_Key	kNC_StringPrefix "kPrefsDropDownItem_2_Key"
#define kPrefsDropDownItem_3_Key	kNC_StringPrefix "kPrefsDropDownItem_3_Key"
#define kPrefsDropDownItem_4_Key	kNC_StringPrefix "kPrefsDropDownItem_4_Key"
#define kPrefsDropDownItem_5_Key	kNC_StringPrefix "kPrefsDropDownItem_5_Key"
#define kPrefsDropDownItem_6_Key	kNC_StringPrefix "kPrefsDropDownItem_6_Key"
#define kPrefsDropDownItem_7_Key	kNC_StringPrefix "kPrefsDropDownItem_7_Key"

// Dialog title: "NewsCAD Preferences"
#define	kNC_PreferencesDialogTitleKey					kNC_StringPrefix	"kNC_PreferencesDialogTitleKey"
// Rule weight combo box label
#define	kPreferencesRuleWeightComboBoxLabelStringKey	kNC_StringPrefix	"kPreferencesRuleWeightComboBoxLabelStringKey"
// Current Graphics Folder text field label
#define kPreferencesGraphicsFolderLabelStringKey		kNC_StringPrefix	"kPreferencesGraphicsFolderLabelStringKey"
// Checkbox label "Gray Ad Outline"
#define kGrayAdOutlineStringKey							kNC_StringPrefix	"kGrayAdOutlineStringKey"
// Label for percentage of coverage "Ad Coverage"
#define kPreferencesAdCoverageStringKey					kNC_StringPrefix	"kPreferencesAdCoverageStringKey"
#endif

/* -------------------------- Button Label String Key --------------------------- */

#define kChangeGraphicsFolderBtnStrKey		"kChangeGraphicsFolderBtnStrKey"

/* ---------------------------- Menu item positions ----------------------------- */
// i don't have any yet


/* -------------------- Initial data format version numbers --------------------- */

#define kNC_FirstMajorFormatNumber  RezLong(1)
#define kNC_FirstMinorFormatNumber  RezLong(0)


/* --------- Data format version numbers for the PluginVersion resource --------- */

#define kNC_CurrentMajorFormatNumber kNC_FirstMajorFormatNumber
#define kNC_CurrentMinorFormatNumber kNC_FirstMinorFormatNumber

#endif // __NC_ID_h__

//  Code generated by DollyXs code generator
//  then polished, embellished, and 
//	extended by the Ross code generator.

// END OF FILE
