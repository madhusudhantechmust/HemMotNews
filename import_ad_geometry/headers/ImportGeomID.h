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


#ifndef __ImportGeomID_h__
#define __ImportGeomID_h__

#include "SDKDef.h"

// Company:
#define kImportGeomCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kImportGeomCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

// Plug-in:
#define kImportGeomPluginName	"ImportAdGeometry"			// Name of this plug-in.
#define kImportGeomPrefixNumber	0xd9990 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kImportGeomVersion		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
#define kImportGeomAuthor		"Ross Edgar"					// Author of this plug-in (for the About Box).

// Plug-ins:
	// NewsCAD plugin
	// Unique prefix number for the "main" plugin that contains all the menus and dialogs.
	// The plugin that makes the text and picture frames uses this ID when it adds the 
	// black box data that identifies the page item as an item belonging to NewsCAD.
	// These next 4 lines were copied in here from /id6sdk/source/frank/newscad_main/NC_ID.h
//#define kNC_PrefixNumber	0xd9940 		// Unique prefix number (*Must* be obtained from Adobe Developer Support).
//#define kNC_Prefix		RezLong(kNC_PrefixNumber)				// The unique numeric prefix for all object model IDs for the "main" plug-in.
//#define kNC_StringPrefix	SDK_DEF_STRINGIZE(kNC_PrefixNumber)	// The string equivalent of the unique prefix number for  the "main" plug-in.
//DECLARE_PMID(kPlugInIDSpace, kNC_PluginID, kNC_Prefix + 0)

// Plug-in Prefix: (please change kImportGeomPrefixNumber above to modify the prefix.)
#define kImportGeomPrefix		RezLong(kImportGeomPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kImportGeomStringPrefix	SDK_DEF_STRINGIZE(kImportGeomPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kImportGeomMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kImportGeomMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kImportGeomPluginID, kImportGeomPrefix + 0)

// ClassIDs:
DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 0)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 3)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 4)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 5)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 6)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 7)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 8)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 9)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 10)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 11)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 12)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 13)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 14)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 15)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 16)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 17)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 18)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 19)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 20)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 21)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 22)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 23)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 24)
//DECLARE_PMID(kClassIDSpace, kImportGeomBoss, kImportGeomPrefix + 25)


// InterfaceIDs:
DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 0)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 1)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 2)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 3)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 4)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 5)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 6)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 7)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 8)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTGEOMINTERFACE, kImportGeomPrefix + 25)


// ImplementationIDs:
//DECLARE_PMID(kImplementationIDSpace, kImportGeomActionComponentImpl, kImportGeomPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 1)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 2)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 3)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 4)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 5)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 6)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 7)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 8)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 9)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 10)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 11)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 12)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 13)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 14)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 15)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 16)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kImportGeomImpl, kImportGeomPrefix + 25)


// ActionIDs:
//DECLARE_PMID(kActionIDSpace, kImportGeomAboutActionID, kImportGeomPrefix + 0)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 5)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 6)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 7)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 8)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 9)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 10)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 11)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 12)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 13)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 14)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 15)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 16)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 17)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 18)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 19)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 20)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 21)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 22)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 23)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 24)
//DECLARE_PMID(kActionIDSpace, kImportGeomActionID, kImportGeomPrefix + 25)


// WidgetIDs:
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 2)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 3)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 4)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 5)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 6)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 7)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 8)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 9)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 10)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 11)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 12)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 13)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 14)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 15)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 16)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 17)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 18)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 19)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 20)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 21)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 22)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 23)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kImportGeomWidgetID, kImportGeomPrefix + 25)


// "About Plug-ins" sub-menu:
#define kImportGeomAboutMenuKey			kImportGeomStringPrefix "kImportGeomAboutMenuKey"
#define kImportGeomAboutMenuPath		kSDKDefStandardAboutMenuPath kImportGeomCompanyKey

// "Plug-ins" sub-menu:
#define kImportGeomPluginsMenuKey 		kImportGeomStringPrefix "kImportGeomPluginsMenuKey"
#define kImportGeomPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kImportGeomCompanyKey kSDKDefDelimitMenuPath kImportGeomPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kImportGeomAboutBoxStringKey	kImportGeomStringPrefix "kImportGeomAboutBoxStringKey"
#define kImportGeomTargetMenuPath		kImportGeomPluginsMenuPath
#define kNewsCadOversetWarningKey		kImportGeomStringPrefix "kNewsCadOversetWarningKey"

// Menu item positions:


// Initial data format version numbers
#define kImportGeomFirstMajorFormatNumber  RezLong(1)
#define kImportGeomFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kImportGeomCurrentMajorFormatNumber kImportGeomFirstMajorFormatNumber
#define kImportGeomCurrentMinorFormatNumber kImportGeomFirstMinorFormatNumber

#endif // __ImportGeomID_h__

//  Code generated by DollyXs code generator
