//========================================================================================
//  
//  $File: $
//  
//  Owner: Ross L. Edgar
//  
//  $Author: $
//  
//  $DateTime: $
//  
//  $Revision: $
//  
//  $Change: $
//  
//  Copyright 1997-2005 Adobe Systems Incorporated. All rights reserved.
//  
//  NOTICE:  Adobe permits you to use, modify, and distribute this file in accordance 
//  with the terms of the Adobe license agreement accompanying it.  If you have received
//  this file from a source other than Adobe, then your use, modification, or 
//  distribution of it requires the prior written permission of Adobe.
//  
//========================================================================================


#ifndef __TstNoMnuID_h__
#define __TstNoMnuID_h__

#include "SDKDef.h"

// Company:
#define kTstNoMnuCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kTstNoMnuCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

// Plug-in:
#define kTstNoMnuPluginName	"Test_No_Menus"			// Name of this plug-in.
#define kTstNoMnuPrefixNumber	0xd9950 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kTstNoMnuVersion		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
#define kTstNoMnuAuthor		"Ross L. Edgar"					// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kTstNoMnuPrefixNumber above to modify the prefix.)
#define kTstNoMnuPrefix		RezLong(kTstNoMnuPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kTstNoMnuStringPrefix	SDK_DEF_STRINGIZE(kTstNoMnuPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kTstNoMnuMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kTstNoMnuMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kTstNoMnuPluginID, kTstNoMnuPrefix + 0)

// ClassIDs:
//DECLARE_PMID(kClassIDSpace, kTstNoMnuActionComponentBoss, kTstNoMnuPrefix + 0)
DECLARE_PMID(kClassIDSpace, kTstNoMenuDoSomethingBoss, kTstNoMnuPrefix + 3)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 4)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 5)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 6)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 7)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 8)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 9)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 10)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 11)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 12)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 13)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 14)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 15)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 16)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 17)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 18)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 19)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 20)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 21)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 22)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 23)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 24)
//DECLARE_PMID(kClassIDSpace, kTstNoMnuBoss, kTstNoMnuPrefix + 25)


// InterfaceIDs:
DECLARE_PMID(kInterfaceIDSpace, IID_IDOSOMETHING, kTstNoMnuPrefix + 0)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 1)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 2)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 3)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 4)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 5)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 6)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 7)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 8)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITSTNOMNUINTERFACE, kTstNoMnuPrefix + 25)


// ImplementationIDs:
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuActionComponentImpl, kTstNoMnuPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kDoSomethingImpl, kTstNoMnuPrefix + 1)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 2)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 3)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 4)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 5)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 6)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 7)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 8)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 9)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 10)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 11)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 12)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 13)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 14)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 15)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 16)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kTstNoMnuImpl, kTstNoMnuPrefix + 25)


// ActionIDs:
//DECLARE_PMID(kActionIDSpace, kTstNoMnuAboutActionID, kTstNoMnuPrefix + 0)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 5)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 6)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 7)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 8)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 9)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 10)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 11)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 12)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 13)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 14)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 15)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 16)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 17)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 18)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 19)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 20)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 21)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 22)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 23)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 24)
//DECLARE_PMID(kActionIDSpace, kTstNoMnuActionID, kTstNoMnuPrefix + 25)


// WidgetIDs:
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 2)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 3)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 4)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 5)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 6)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 7)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 8)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 9)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 10)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 11)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 12)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 13)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 14)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 15)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 16)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 17)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 18)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 19)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 20)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 21)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 22)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 23)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kTstNoMnuWidgetID, kTstNoMnuPrefix + 25)


// "About Plug-ins" sub-menu:
#define kTstNoMnuAboutMenuKey			kTstNoMnuStringPrefix "kTstNoMnuAboutMenuKey"
#define kTstNoMnuAboutMenuPath		kSDKDefStandardAboutMenuPath kTstNoMnuCompanyKey

// "Plug-ins" sub-menu:
#define kTstNoMnuPluginsMenuKey 		kTstNoMnuStringPrefix "kTstNoMnuPluginsMenuKey"
#define kTstNoMnuPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kTstNoMnuCompanyKey kSDKDefDelimitMenuPath kTstNoMnuPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kTstNoMnuAboutBoxStringKey	kTstNoMnuStringPrefix "kTstNoMnuAboutBoxStringKey"
#define kTstNoMnuTargetMenuPath kTstNoMnuPluginsMenuPath

// Menu item positions:


// Initial data format version numbers
#define kTstNoMnuFirstMajorFormatNumber  RezLong(1)
#define kTstNoMnuFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kTstNoMnuCurrentMajorFormatNumber kTstNoMnuFirstMajorFormatNumber
#define kTstNoMnuCurrentMinorFormatNumber kTstNoMnuFirstMinorFormatNumber

#endif // __TstNoMnuID_h__

//  Code generated by DollyXs code generator
