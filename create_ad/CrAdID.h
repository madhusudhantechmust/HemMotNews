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


#ifndef __CrAdID_h__
#define __CrAdID_h__

#include "SDKDef.h"

// Company:
#define kCrAdCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kCrAdCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

// Plug-in:
#define kCrAdPluginName	"CreateAd"			// Name of this plug-in.
#define kCrAdPrefixNumber	0xd9950 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kCrAdVersion		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
#define kCrAdAuthor		"Ross Edgar"					// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kCrAdPrefixNumber above to modify the prefix.)
#define kCrAdPrefix		RezLong(kCrAdPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kCrAdStringPrefix	SDK_DEF_STRINGIZE(kCrAdPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kCrAdMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kCrAdMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kCrAdPluginID, kCrAdPrefix + 0)

// ClassIDs:
// -- DECLARE_PMID(kClassIDSpace, kCrAdActionComponentBoss, kCrAdPrefix + 0)
DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 3)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 4)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 5)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 6)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 7)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 8)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 9)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 10)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 11)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 12)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 13)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 14)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 15)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 16)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 17)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 18)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 19)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 20)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 21)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 22)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 23)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 24)
//DECLARE_PMID(kClassIDSpace, kCrAdBoss, kCrAdPrefix + 25)


// InterfaceIDs:
DECLARE_PMID(kInterfaceIDSpace, IID_ICREATEAD, kCrAdPrefix + 0)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 1)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 2)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 3)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 4)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 5)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 6)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 7)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 8)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_ICRADINTERFACE, kCrAdPrefix + 25)


// ImplementationIDs:
//DECLARE_PMID(kImplementationIDSpace, kCrAdActionComponentImpl, kCrAdPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 1)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 2)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 3)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 4)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 5)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 6)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 7)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 8)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 9)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 10)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 11)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 12)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 13)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 14)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 15)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 16)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kCrAdImpl, kCrAdPrefix + 25)


// ActionIDs:
//DECLARE_PMID(kActionIDSpace, kCrAdAboutActionID, kCrAdPrefix + 0)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 5)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 6)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 7)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 8)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 9)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 10)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 11)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 12)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 13)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 14)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 15)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 16)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 17)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 18)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 19)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 20)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 21)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 22)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 23)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 24)
//DECLARE_PMID(kActionIDSpace, kCrAdActionID, kCrAdPrefix + 25)


// WidgetIDs:
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 2)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 3)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 4)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 5)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 6)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 7)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 8)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 9)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 10)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 11)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 12)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 13)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 14)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 15)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 16)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 17)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 18)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 19)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 20)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 21)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 22)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 23)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kCrAdWidgetID, kCrAdPrefix + 25)


// "About Plug-ins" sub-menu:
#define kCrAdAboutMenuKey			kCrAdStringPrefix "kCrAdAboutMenuKey"
#define kCrAdAboutMenuPath		kSDKDefStandardAboutMenuPath kCrAdCompanyKey

// "Plug-ins" sub-menu:
#define kCrAdPluginsMenuKey 		kCrAdStringPrefix "kCrAdPluginsMenuKey"
#define kCrAdPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kCrAdCompanyKey kSDKDefDelimitMenuPath kCrAdPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kCrAdAboutBoxStringKey	kCrAdStringPrefix "kCrAdAboutBoxStringKey"
#define kCrAdTargetMenuPath kCrAdPluginsMenuPath

// Menu item positions:


// Initial data format version numbers
#define kCrAdFirstMajorFormatNumber  RezLong(1)
#define kCrAdFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kCrAdCurrentMajorFormatNumber kCrAdFirstMajorFormatNumber
#define kCrAdCurrentMinorFormatNumber kCrAdFirstMinorFormatNumber

#endif // __CrAdID_h__

//  Code generated by DollyXs code generator
