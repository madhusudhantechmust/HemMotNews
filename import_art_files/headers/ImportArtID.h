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


#ifndef __ImportArtID_h__
#define __ImportArtID_h__

#include "SDKDef.h"

// Company:
#define kImportArtCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kImportArtCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

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

// Plug-in:
#define kImportArtPluginName	"ImportArtFiles"			// Name of this plug-in.
#define kImportArtPrefixNumber	0xd99a0 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kImportArtVersion		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
#define kImportArtAuthor		"Ross Edgar"					// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kImportArtPrefixNumber above to modify the prefix.)
#define kImportArtPrefix		RezLong(kImportArtPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kImportArtStringPrefix	SDK_DEF_STRINGIZE(kImportArtPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kImportArtMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kImportArtMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kImportArtPluginID, kImportArtPrefix + 0)

// ClassIDs:
DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 0)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 3)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 4)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 5)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 6)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 7)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 8)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 9)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 10)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 11)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 12)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 13)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 14)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 15)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 16)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 17)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 18)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 19)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 20)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 21)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 22)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 23)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 24)
//DECLARE_PMID(kClassIDSpace, kImportArtBoss, kImportArtPrefix + 25)


// InterfaceIDs:
DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 0)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 1)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 2)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 3)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 4)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 5)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 6)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 7)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 8)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_IIMPORTARTINTERFACE, kImportArtPrefix + 25)


// ImplementationIDs:
//DECLARE_PMID(kImplementationIDSpace, kImportArtActionComponentImpl, kImportArtPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 1)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 2)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 3)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 4)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 5)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 6)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 7)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 8)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 9)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 10)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 11)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 12)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 13)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 14)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 15)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 16)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kImportArtImpl, kImportArtPrefix + 25)


// ActionIDs:
DECLARE_PMID(kActionIDSpace, kImportArtAboutActionID, kImportArtPrefix + 0)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 5)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 6)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 7)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 8)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 9)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 10)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 11)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 12)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 13)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 14)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 15)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 16)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 17)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 18)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 19)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 20)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 21)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 22)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 23)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 24)
//DECLARE_PMID(kActionIDSpace, kImportArtActionID, kImportArtPrefix + 25)


// WidgetIDs:
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 2)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 3)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 4)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 5)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 6)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 7)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 8)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 9)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 10)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 11)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 12)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 13)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 14)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 15)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 16)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 17)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 18)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 19)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 20)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 21)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 22)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 23)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kImportArtWidgetID, kImportArtPrefix + 25)


// "About Plug-ins" sub-menu:
#define kImportArtAboutMenuKey			kImportArtStringPrefix "kImportArtAboutMenuKey"
#define kImportArtAboutMenuPath		kSDKDefStandardAboutMenuPath kImportArtCompanyKey

// "Plug-ins" sub-menu:
#define kImportArtPluginsMenuKey 		kImportArtStringPrefix "kImportArtPluginsMenuKey"
#define kImportArtPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kImportArtCompanyKey kSDKDefDelimitMenuPath kImportArtPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kImportArtAboutBoxStringKey	kImportArtStringPrefix "kImportArtAboutBoxStringKey"
#define kImportArtTargetMenuPath kImportArtPluginsMenuPath

// Menu item positions:


// Initial data format version numbers
#define kImportArtFirstMajorFormatNumber  RezLong(1)
#define kImportArtFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kImportArtCurrentMajorFormatNumber kImportArtFirstMajorFormatNumber
#define kImportArtCurrentMinorFormatNumber kImportArtFirstMinorFormatNumber

#endif // __ImportArtID_h__

//  Code generated by DollyXs code generator
