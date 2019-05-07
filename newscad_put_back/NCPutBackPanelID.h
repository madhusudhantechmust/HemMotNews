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


#ifndef __NCPutBackPanelID_h__
#define __NCPutBackPanelID_h__

#include "SDKDef.h"

/* ------------------------------- C O M P A N Y -------------------------------- */
#define kNCPutBackPanelCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kNCPutBackPanelCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

// Plug-ins:
	// NewsCAD plugin
	// Unique prefix number for the "main" plugin that contains all the menus and dialogs.
	// The plugin that makes the text and picture frames uses this ID when it adds the 
	// black box data that identifies the page item as an item belonging to NewsCAD.
	// These next 4 lines were copied in here from /id6sdk/source/frank/newscad_main/NC_ID.h
#define kNC_PrefixNumber	0xd9940 		// Unique prefix number (*Must* be obtained from Adobe Developer Support).
#define kNC_Prefix		RezLong(kNC_PrefixNumber)				// The unique numeric prefix for all object model IDs for the "main" plug-in.
#define kNC_StringPrefix	SDK_DEF_STRINGIZE(kNC_PrefixNumber)	// The string equivalent of the unique prefix number for  the "main" plug-in.
DECLARE_PMID(kPlugInIDSpace, kNC_PluginID, kNC_Prefix + 0)

	// Put Back plugin
#define kNCPutBackPanelPluginName	"NewsCAD: Put Back"			// Name of this plug-in.
#define kNCPutBackPanelPrefixNumber	0xd9960 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kNCPutBackPanelVersion		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
#define kNCPutBackPanelAuthor		"Ross L. Edgar"					// Author of this plug-in (for the About Box).


// Plug-in Prefix: (please change kNCPutBackPanelPrefixNumber above to modify the prefix.)
#define kNCPutBackPanelPrefix		RezLong(kNCPutBackPanelPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kNCPutBackPanelStringPrefix	SDK_DEF_STRINGIZE(kNCPutBackPanelPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kNCPutBackPanelMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kNCPutBackPanelMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kNCPutBackPanelPluginID, kNCPutBackPanelPrefix + 0)

// ClassIDs:
DECLARE_PMID(kClassIDSpace, kNCPutBackPanelActionComponentBoss, kNCPutBackPanelPrefix + 0)
DECLARE_PMID(kClassIDSpace, kNCPutBackPanelPanelWidgetBoss, kNCPutBackPanelPrefix + 1)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 3)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 4)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 5)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 6)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 7)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 8)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 9)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 10)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 11)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 12)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 13)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 14)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 15)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 16)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 17)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 18)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 19)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 20)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 21)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 22)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 23)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 24)
//DECLARE_PMID(kClassIDSpace, kNCPutBackPanelBoss, kNCPutBackPanelPrefix + 25)


// InterfaceIDs:
DECLARE_PMID (kInterfaceIDSpace, IID_NCSELSUITE, kNCPutBackPanelPrefix + 0)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 1)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 2)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 3)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 4)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 5)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 6)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 7)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 8)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPUTBACKPANELINTERFACE, kNCPutBackPanelPrefix + 25)


// ImplementationIDs:
DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelActionComponentImpl, kNCPutBackPanelPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kNCPutBackSelectionObserverImpl, kNCPutBackPanelPrefix + 1 )
DECLARE_PMID(kImplementationIDSpace, kNCSelectionSuiteCSBImpl, kNCPutBackPanelPrefix + 2)
DECLARE_PMID(kImplementationIDSpace, kNCSelectionSuiteASBImpl, kNCPutBackPanelPrefix + 3)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 4)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 5)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 6)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 7)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 8)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 9)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 10)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 11)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 12)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 13)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 14)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 15)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 16)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kNCPutBackPanelImpl, kNCPutBackPanelPrefix + 25)


// ActionIDs:
DECLARE_PMID(kActionIDSpace, kNCPutBackPanelAboutActionID, kNCPutBackPanelPrefix + 0)
DECLARE_PMID(kActionIDSpace, kNCPutBackPanelPanelWidgetActionID, kNCPutBackPanelPrefix + 1)
DECLARE_PMID(kActionIDSpace, kNCPutBackPanelSeparator1ActionID, kNCPutBackPanelPrefix + 2)
DECLARE_PMID(kActionIDSpace, kNCPutBackPanelPopupAboutThisActionID, kNCPutBackPanelPrefix + 3)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 5)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 6)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 7)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 8)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 9)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 10)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 11)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 12)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 13)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 14)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 15)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 16)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 17)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 18)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 19)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 20)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 21)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 22)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 23)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 24)
//DECLARE_PMID(kActionIDSpace, kNCPutBackPanelActionID, kNCPutBackPanelPrefix + 25)


// WidgetIDs:
DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelPanelWidgetID,	kNCPutBackPanelPrefix + 0)
DECLARE_PMID(kWidgetIDSpace, kStaticTextWidgetID_Client,	kNCPutBackPanelPrefix + 1)
DECLARE_PMID(kWidgetIDSpace, kStaticTextWidgetID_FileName,	kNCPutBackPanelPrefix + 2)
DECLARE_PMID(kWidgetIDSpace, kStaticTextWidgetID_SalesRep,	kNCPutBackPanelPrefix + 3)
DECLARE_PMID(kWidgetIDSpace, kStaticTextWidgetID_AdSize,	kNCPutBackPanelPrefix + 4)
DECLARE_PMID(kWidgetIDSpace, kStaticTextWidgetID_StartX,	kNCPutBackPanelPrefix + 5)
DECLARE_PMID(kWidgetIDSpace, kStaticTextWidgetID_StartY,	kNCPutBackPanelPrefix + 6)
//DECLARE_PMID(kWidgetIDSpace, kStaticTextWidgetID_Client,	kNCPutBackPanelPrefix + 7)
//DECLARE_PMID(kWidgetIDSpace, kStaticTextWidgetID_Client,	kNCPutBackPanelPrefix + 8)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 10)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 11)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 12)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 13)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 14)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 15)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 16)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 17)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 18)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 19)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 20)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 21)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 22)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 23)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kNCPutBackPanelWidgetID, kNCPutBackPanelPrefix + 25)


// "About Plug-ins" sub-menu:
#define kNCPutBackPanelAboutMenuKey			kNCPutBackPanelStringPrefix "kNCPutBackPanelAboutMenuKey"
#define kNCPutBackPanelAboutMenuPath		kSDKDefStandardAboutMenuPath kNCPutBackPanelCompanyKey

// "Plug-ins" sub-menu:
#define kNCPutBackPanelPluginsMenuKey 		kNCPutBackPanelStringPrefix "kNCPutBackPanelPluginsMenuKey"
#define kNCPutBackPanelPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kNCPutBackPanelCompanyKey kSDKDefDelimitMenuPath kNCPutBackPanelPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kNCPutBackPanelAboutBoxStringKey	kNCPutBackPanelStringPrefix "kNCPutBackPanelAboutBoxStringKey"
#define kNCPutBackPanelPanelTitleKey					kNCPutBackPanelStringPrefix	"kNCPutBackPanelPanelTitleKey"
#define kNCPutBackPanelStaticTextKey kNCPutBackPanelStringPrefix	"kNCPutBackPanelStaticTextKey"
#define kNCPutBackPanelInternalPopupMenuNameKey kNCPutBackPanelStringPrefix	"kNCPutBackPanelInternalPopupMenuNameKey"
#define kNCPutBackPanelTargetMenuPath kNCPutBackPanelInternalPopupMenuNameKey

// Button labels
#define kPutBackButtonKey				"Put Back"

// Menu item positions:

#define	kNCPutBackPanelSeparator1MenuItemPosition		10.0
#define kNCPutBackPanelAboutThisMenuItemPosition		11.0


// Initial data format version numbers
#define kNCPutBackPanelFirstMajorFormatNumber  RezLong(1)
#define kNCPutBackPanelFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kNCPutBackPanelCurrentMajorFormatNumber kNCPutBackPanelFirstMajorFormatNumber
#define kNCPutBackPanelCurrentMinorFormatNumber kNCPutBackPanelFirstMinorFormatNumber

#endif // __NCPutBackPanelID_h__

//  Code generated by DollyXs code generator
