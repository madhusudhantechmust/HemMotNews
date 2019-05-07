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


#ifndef __NCPrefsID_h__
#define __NCPrefsID_h__

#include "SDKDef.h"

// Company:
#define kNCPrefsCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kNCPrefsCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

// Plug-in:
#define kNCPrefsPluginName	"NewsCADPreferences"			// Name of this plug-in.
#define kNCPrefsPrefixNumber	0xd99d0 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kNCPrefsVersion		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
#define kNCPrefsAuthor		"Ross Edgar"					// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kNCPrefsPrefixNumber above to modify the prefix.)
#define kNCPrefsPrefix		RezLong(kNCPrefsPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kNCPrefsStringPrefix	SDK_DEF_STRINGIZE(kNCPrefsPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kNCPrefsMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kNCPrefsMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kNCPrefsPluginID, kNCPrefsPrefix + 0)

// ClassIDs:
//DECLARE_PMID(kClassIDSpace, kNCPrefsActionComponentBoss, kNCPrefsPrefix + 0)
DECLARE_PMID(kClassIDSpace, kNCPrefsDialogBoss, kNCPrefsPrefix + 2)
DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 3)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 4)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 5)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 6)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 7)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 8)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 9)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 10)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 11)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 12)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 13)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 14)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 15)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 16)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 17)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 18)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 19)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 20)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 21)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 22)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 23)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 24)
//DECLARE_PMID(kClassIDSpace, kNCPrefsBoss, kNCPrefsPrefix + 25)


// InterfaceIDs:
DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 0)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 1)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 2)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 3)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 4)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 5)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 6)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 7)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 8)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPREFSINTERFACE, kNCPrefsPrefix + 25)


// ImplementationIDs:
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsActionComponentImpl, kNCPrefsPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kNCPrefsDialogControllerImpl, kNCPrefsPrefix + 1 )
DECLARE_PMID(kImplementationIDSpace, kNCPrefsDialogObserverImpl, kNCPrefsPrefix + 2 )
DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 3)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 4)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 5)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 6)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 7)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 8)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 9)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 10)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 11)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 12)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 13)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 14)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 15)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 16)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kNCPrefsImpl, kNCPrefsPrefix + 25)


// ActionIDs:
DECLARE_PMID(kActionIDSpace, kNCPrefsAboutActionID, kNCPrefsPrefix + 0)
DECLARE_PMID(kActionIDSpace, kNCPrefsDialogActionID, kNCPrefsPrefix + 4)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 5)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 6)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 7)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 8)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 9)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 10)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 11)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 12)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 13)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 14)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 15)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 16)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 17)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 18)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 19)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 20)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 21)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 22)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 23)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 24)
//DECLARE_PMID(kActionIDSpace, kNCPrefsActionID, kNCPrefsPrefix + 25)


// WidgetIDs:
// The Preferences... dialog itself
DECLARE_PMID(kWidgetIDSpace, kNCPrefsDialogWidgetID, kNCPrefsPrefix + 1)

// Label for rule weight combo box:  "Rule Weight:"
DECLARE_PMID(kWidgetIDSpace,	kNCPrefsDropDownLabelWidgetID,	kNCPrefsPrefix + 2)
// The drop-down list (popup menu on Mac) in the dialog
DECLARE_PMID(kWidgetIDSpace,	kNCPrefsDropDownWidgetID,	kNCPrefsPrefix + 3)

// Checkbox -- Gray Ad Outline
DECLARE_PMID(kWidgetIDSpace,	kNCPrefsGrayAdOutlineCheckBoxWidgetID,	kNCPrefsPrefix + 4)

// Current Graphics Folder:
// Label for edit text field
DECLARE_PMID(kWidgetIDSpace,	kNCPrefsGraphicsFolderLabelWidgetID,	kNCPrefsPrefix + 5)
// Edit text field
DECLARE_PMID(kWidgetIDSpace,	kNCPrefsChangeFolderEditTextWidgetID,	kNCPrefsPrefix + 6)
// Browse for folder button
DECLARE_PMID(kWidgetIDSpace,	kNCPrefsChangeGraphicsFolderBtnWidgetID,kNCPrefsPrefix + 7)

// UI to specify maximum area of page that can be covered by ads:
// Label for edit text field: "Ad Coverage:"
DECLARE_PMID(kWidgetIDSpace,	kNCPrefsAdCoverageLabelWidgetID,		kNCPrefsPrefix + 8)
// Edit Text -- ad coverage percentage
DECLARE_PMID(kWidgetIDSpace,	kNCPrefsAdCoverageEditTextWidgetID,		kNCPrefsPrefix + 9)
// Static text -- percent sign at right hand end of field
DECLARE_PMID(kWidgetIDSpace,	kNCPrefsAdCoveragePercentWidgetID,		kNCPrefsPrefix + 10)
// Helpful prompt static text -- new for CS4
DECLARE_PMID(kWidgetIDSpace,	kNCPrefsAdCoveragePrompt1TextWidgetID,	kNCPrefsPrefix + 11)
DECLARE_PMID(kWidgetIDSpace,	kNCPrefsAdCoveragePrompt2TextWidgetID,	kNCPrefsPrefix + 12)
//DECLARE_PMID(kWidgetIDSpace, kNCPrefsWidgetID, kNCPrefsPrefix + 13)
//DECLARE_PMID(kWidgetIDSpace, kNCPrefsWidgetID, kNCPrefsPrefix + 14)
//DECLARE_PMID(kWidgetIDSpace, kNCPrefsWidgetID, kNCPrefsPrefix + 15)
//DECLARE_PMID(kWidgetIDSpace, kNCPrefsWidgetID, kNCPrefsPrefix + 16)
//DECLARE_PMID(kWidgetIDSpace, kNCPrefsWidgetID, kNCPrefsPrefix + 17)
//DECLARE_PMID(kWidgetIDSpace, kNCPrefsWidgetID, kNCPrefsPrefix + 18)
//DECLARE_PMID(kWidgetIDSpace, kNCPrefsWidgetID, kNCPrefsPrefix + 19)
//DECLARE_PMID(kWidgetIDSpace, kNCPrefsWidgetID, kNCPrefsPrefix + 20)
//DECLARE_PMID(kWidgetIDSpace, kNCPrefsWidgetID, kNCPrefsPrefix + 21)
//DECLARE_PMID(kWidgetIDSpace, kNCPrefsWidgetID, kNCPrefsPrefix + 22)
//DECLARE_PMID(kWidgetIDSpace, kNCPrefsWidgetID, kNCPrefsPrefix + 23)
//DECLARE_PMID(kWidgetIDSpace, kNCPrefsWidgetID, kNCPrefsPrefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kNCPrefsWidgetID, kNCPrefsPrefix + 25)

// "About Plug-ins" sub-menu:
#define kNCPrefsAboutMenuKey			kNCPrefsStringPrefix "kNCPrefsAboutMenuKey"
#define kNCPrefsAboutMenuPath		kSDKDefStandardAboutMenuPath kNCPrefsCompanyKey

// "Plug-ins" sub-menu:
#define kNCPrefsPluginsMenuKey 		kNCPrefsStringPrefix "kNCPrefsPluginsMenuKey"
#define kNCPrefsPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kNCPrefsCompanyKey kSDKDefDelimitMenuPath kNCPrefsPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kNCPrefsAboutBoxStringKey	kNCPrefsStringPrefix "kNCPrefsAboutBoxStringKey"
#define kNCPrefsTargetMenuPath kNCPrefsPluginsMenuPath

// Menu item positions:

/* -------------------------- Static Text String Keys --------------------------- */
// Preferences ...
// Preferences Dialog Drop Down List
#define kPrefsDropDownItem_1_Key	kNCPrefsStringPrefix "kPrefsDropDownItem_1_Key"
#define kPrefsDropDownItem_2_Key	kNCPrefsStringPrefix "kPrefsDropDownItem_2_Key"
#define kPrefsDropDownItem_3_Key	kNCPrefsStringPrefix "kPrefsDropDownItem_3_Key"
#define kPrefsDropDownItem_4_Key	kNCPrefsStringPrefix "kPrefsDropDownItem_4_Key"
#define kPrefsDropDownItem_5_Key	kNCPrefsStringPrefix "kPrefsDropDownItem_5_Key"
#define kPrefsDropDownItem_6_Key	kNCPrefsStringPrefix "kPrefsDropDownItem_6_Key"
#define kPrefsDropDownItem_7_Key	kNCPrefsStringPrefix "kPrefsDropDownItem_7_Key"

// Dialog title: "NewsCAD Preferences"
#define kNCPrefsDialogTitleKey 			kNCPrefsStringPrefix	"kNCPrefsDialogTitleKey"
// Rule weight combo box label
#define	kPrefsRuleWtDropDownLabelStrKey	kNCPrefsStringPrefix	"kPrefsRuleWtDropDownLabelStrKey"
// Current Graphics Folder text field label
#define kPrefsGraphicsFolderLabelStrKey	kNCPrefsStringPrefix	"kPrefsGraphicsFolderLabelStrKey"
// Checkbox label "Gray Ad Outline"
#define kPrefsGrayAdOutlineStrKey		kNCPrefsStringPrefix	"kPrefsGrayAdOutlineStrKey"
// Friendly prompt text for ad coverage percentage specification
#define	kPrefsAdCoveragePrompt1StrKey	kNCPrefsStringPrefix	"kPrefsAdCoveragePrompt1StrKey"
#define	kPrefsAdCoveragePrompt2StrKey	kNCPrefsStringPrefix	"kPrefsAdCoveragePrompt2StrKey"
// Label for percentage of coverage "Ad Coverage"
#define kPrefsAdCoverageLabelStrKey		kNCPrefsStringPrefix	"kPrefsAdCoverageLabelStrKey"
// Percent sign to right-hand end of field
#define	kPrefsAdCoveragePercentStrKey	kNCPrefsStringPrefix	"kPrefsAdCoveragePercentStrKey"
// Browse for Folder button Label String Key
#define kChangeGraphicsFolderBtnStrKey	"kChangeGraphicsFolderBtnStrKey"

// "Plug-ins" sub-menu item key for dialog:
#define kNCPrefsDialogMenuItemKey kNCPrefsStringPrefix "kNCPrefsDialogMenuItemKey"

// "Plug-ins" sub-menu item position for dialog:
#define kNCPrefsDialogMenuItemPosition	12.0


// Initial data format version numbers
#define kNCPrefsFirstMajorFormatNumber  RezLong(1)
#define kNCPrefsFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kNCPrefsCurrentMajorFormatNumber kNCPrefsFirstMajorFormatNumber
#define kNCPrefsCurrentMinorFormatNumber kNCPrefsFirstMinorFormatNumber

#endif // __NCPrefsID_h__

//  Code generated by DollyXs code generator
