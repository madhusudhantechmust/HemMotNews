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


#ifndef __TypesetContinuedLinesID_h__
#define __TypesetContinuedLinesID_h__

#include "SDKDef.h"

// Company:
#define kTypesetContinuedLinesCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kTypesetContinuedLinesCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

// Plug-in:
#define kTypesetContinuedLinesPluginName	"HMTypesetContinuedLines"			// Name of this plug-in.
#define kTypesetContinuedLinesPrefixNumber	0xd99b0 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kTypesetContinuedLinesVersion		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
#define kTypesetContinuedLinesAuthor		"Ross Edgar"					// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kTypesetContinuedLinesPrefixNumber above to modify the prefix.)
#define kTypesetContinuedLinesPrefix		RezLong(kTypesetContinuedLinesPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kTypesetContinuedLinesStringPrefix	SDK_DEF_STRINGIZE(kTypesetContinuedLinesPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kTypesetContinuedLinesMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kTypesetContinuedLinesMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kTypesetContinuedLinesPluginID, kTypesetContinuedLinesPrefix + 0)

// ClassIDs:
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesActionComponentBoss, kTypesetContinuedLinesPrefix + 0)
DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 0)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 4)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 5)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 6)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 7)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 8)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 9)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 10)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 11)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 12)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 13)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 14)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 15)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 16)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 17)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 18)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 19)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 20)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 21)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 22)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 23)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 24)
//DECLARE_PMID(kClassIDSpace, kTypesetContinuedLinesBoss, kTypesetContinuedLinesPrefix + 25)


// InterfaceIDs:
DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 0)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 1)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 2)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 3)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 4)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 5)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 6)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 7)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 8)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_ITYPESETCONTINUEDLINESINTERFACE, kTypesetContinuedLinesPrefix + 25)


// ImplementationIDs:
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesActionComponentImpl, kTypesetContinuedLinesPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 1)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 2)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 3)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 4)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 5)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 6)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 7)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 8)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 9)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 10)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 11)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 12)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 13)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 14)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 15)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 16)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kTypesetContinuedLinesImpl, kTypesetContinuedLinesPrefix + 25)


// ActionIDs:
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesAboutActionID, kTypesetContinuedLinesPrefix + 0)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 5)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 6)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 7)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 8)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 9)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 10)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 11)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 12)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 13)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 14)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 15)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 16)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 17)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 18)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 19)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 20)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 21)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 22)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 23)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 24)
//DECLARE_PMID(kActionIDSpace, kTypesetContinuedLinesActionID, kTypesetContinuedLinesPrefix + 25)


// WidgetIDs:
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 2)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 3)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 4)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 5)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 6)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 7)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 8)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 9)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 10)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 11)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 12)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 13)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 14)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 15)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 16)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 17)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 18)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 19)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 20)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 21)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 22)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 23)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kTypesetContinuedLinesWidgetID, kTypesetContinuedLinesPrefix + 25)


// "About Plug-ins" sub-menu:
#define kTypesetContinuedLinesAboutMenuKey			kTypesetContinuedLinesStringPrefix "kTypesetContinuedLinesAboutMenuKey"
#define kTypesetContinuedLinesAboutMenuPath		kSDKDefStandardAboutMenuPath kTypesetContinuedLinesCompanyKey

// "Plug-ins" sub-menu:
#define kTypesetContinuedLinesPluginsMenuKey 		kTypesetContinuedLinesStringPrefix "kTypesetContinuedLinesPluginsMenuKey"
#define kTypesetContinuedLinesPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kTypesetContinuedLinesCompanyKey kSDKDefDelimitMenuPath kTypesetContinuedLinesPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kTypesetContinuedLinesAboutBoxStringKey	kTypesetContinuedLinesStringPrefix "kTypesetContinuedLinesAboutBoxStringKey"
#define kTypesetContinuedLinesTargetMenuPath kTypesetContinuedLinesPluginsMenuPath

// Menu item positions:


// Initial data format version numbers
#define kTypesetContinuedLinesFirstMajorFormatNumber  RezLong(1)
#define kTypesetContinuedLinesFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kTypesetContinuedLinesCurrentMajorFormatNumber kTypesetContinuedLinesFirstMajorFormatNumber
#define kTypesetContinuedLinesCurrentMinorFormatNumber kTypesetContinuedLinesFirstMinorFormatNumber

#endif // __TypesetContinuedLinesID_h__

//  Code generated by DollyXs code generator
