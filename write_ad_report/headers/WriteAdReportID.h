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


#ifndef __WriteAdReportID_h__
#define __WriteAdReportID_h__

#include "SDKDef.h"

// Company:
#define kWriteAdReportCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kWriteAdReportCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

// Plug-in:
#define kWriteAdReportPluginName	"WriteAdReport"			// Name of this plug-in.
#define kWriteAdReportPrefixNumber	0xd99c0 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kWriteAdReportVersion		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
#define kWriteAdReportAuthor		"Ross Edgar"					// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kWriteAdReportPrefixNumber above to modify the prefix.)
#define kWriteAdReportPrefix		RezLong(kWriteAdReportPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kWriteAdReportStringPrefix	SDK_DEF_STRINGIZE(kWriteAdReportPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kWriteAdReportMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kWriteAdReportMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kWriteAdReportPluginID, kWriteAdReportPrefix + 0)

// ClassIDs:
//DECLARE_PMID(kClassIDSpace, kWriteAdReportActionComponentBoss, kWriteAdReportPrefix + 0)
DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 0)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 4)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 5)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 6)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 7)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 8)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 9)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 10)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 11)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 12)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 13)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 14)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 15)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 16)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 17)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 18)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 19)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 20)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 21)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 22)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 23)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 24)
//DECLARE_PMID(kClassIDSpace, kWriteAdReportBoss, kWriteAdReportPrefix + 25)


// InterfaceIDs:
DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 0)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 1)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 2)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 3)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 4)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 5)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 6)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 7)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 8)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_IWRITEADREPORTINTERFACE, kWriteAdReportPrefix + 25)


// ImplementationIDs:
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportActionComponentImpl, kWriteAdReportPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 1)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 2)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 3)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 4)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 5)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 6)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 7)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 8)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 9)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 10)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 11)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 12)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 13)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 14)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 15)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 16)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kWriteAdReportImpl, kWriteAdReportPrefix + 25)


// ActionIDs:
//DECLARE_PMID(kActionIDSpace, kWriteAdReportAboutActionID, kWriteAdReportPrefix + 0)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 5)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 6)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 7)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 8)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 9)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 10)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 11)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 12)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 13)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 14)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 15)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 16)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 17)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 18)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 19)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 20)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 21)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 22)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 23)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 24)
//DECLARE_PMID(kActionIDSpace, kWriteAdReportActionID, kWriteAdReportPrefix + 25)


// WidgetIDs:
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 2)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 3)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 4)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 5)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 6)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 7)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 8)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 9)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 10)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 11)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 12)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 13)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 14)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 15)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 16)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 17)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 18)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 19)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 20)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 21)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 22)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 23)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kWriteAdReportWidgetID, kWriteAdReportPrefix + 25)


// "About Plug-ins" sub-menu:
#define kWriteAdReportAboutMenuKey			kWriteAdReportStringPrefix "kWriteAdReportAboutMenuKey"
#define kWriteAdReportAboutMenuPath		kSDKDefStandardAboutMenuPath kWriteAdReportCompanyKey

// "Plug-ins" sub-menu:
#define kWriteAdReportPluginsMenuKey 		kWriteAdReportStringPrefix "kWriteAdReportPluginsMenuKey"
#define kWriteAdReportPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kWriteAdReportCompanyKey kSDKDefDelimitMenuPath kWriteAdReportPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kWriteAdReportAboutBoxStringKey	kWriteAdReportStringPrefix "kWriteAdReportAboutBoxStringKey"
#define kWriteAdReportTargetMenuPath kWriteAdReportPluginsMenuPath

// Menu item positions:


// Initial data format version numbers
#define kWriteAdReportFirstMajorFormatNumber  RezLong(1)
#define kWriteAdReportFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kWriteAdReportCurrentMajorFormatNumber kWriteAdReportFirstMajorFormatNumber
#define kWriteAdReportCurrentMinorFormatNumber kWriteAdReportFirstMinorFormatNumber

#endif // __WriteAdReportID_h__

//  Code generated by DollyXs code generator
