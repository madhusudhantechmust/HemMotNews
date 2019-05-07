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


#ifndef __NCPaletteID_h__
#define __NCPaletteID_h__

#include "SDKDef.h"

// Company:
#define kNCPaletteCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kNCPaletteCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

// Plug-in:
#define kNCPalettePluginName	"NewsCAD Palette"			// Name of this plug-in.
#define kNCPalettePrefixNumber	0xd9900 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kNCPaletteVersion		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
#define kNCPaletteAuthor		"Ross L. Edgar"					// Author of this plug-in (for the About Box).

// NewsCAD plugin
#define kWFPPrefixNumber	0x61000 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kWFPPrefix			RezLong(kWFPPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
DECLARE_PMID(kPlugInIDSpace, kWFPPluginID, kWFPPrefix + 0)

// Plug-in Prefix: (please change kNCPalettePrefixNumber above to modify the prefix.)
#define kNCPalettePrefix		RezLong(kNCPalettePrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kNCPaletteStringPrefix	SDK_DEF_STRINGIZE(kNCPalettePrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kNCPaletteMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kNCPaletteMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kNCPalettePluginID, kNCPalettePrefix + 0)

// ClassIDs:
DECLARE_PMID(kClassIDSpace, kNCPaletteActionComponentBoss, kNCPalettePrefix + 0)
DECLARE_PMID(kClassIDSpace, kNCPalettePanelWidgetBoss, kNCPalettePrefix + 1)
DECLARE_PMID(kClassIDSpace, kNCPaletteTextWidgetBoss, kNCPalettePrefix + 3)
DECLARE_PMID(kClassIDSpace, kNCPaletteListBoxWidgetBoss, kNCPalettePrefix + 4)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 5)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 6)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 7)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 8)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 9)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 10)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 11)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 12)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 13)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 14)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 15)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 16)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 17)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 18)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 19)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 20)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 21)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 22)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 23)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 24)
//DECLARE_PMID(kClassIDSpace, kNCPaletteBoss, kNCPalettePrefix + 25)


// InterfaceIDs:
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 0)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 1)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 2)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 3)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 4)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 5)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 6)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 7)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 8)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_INCPALETTEINTERFACE, kNCPalettePrefix + 25)


// ImplementationIDs:
DECLARE_PMID(kImplementationIDSpace, kNCPaletteActionComponentImpl, kNCPalettePrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kNCPaletteListBoxObserverImpl, kNCPalettePrefix + 1)
DECLARE_PMID(kImplementationIDSpace, kNCPalettePanelObserverImpl, kNCPalettePrefix + 2)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 3)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 4)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 5)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 6)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 7)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 8)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 9)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 10)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 11)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 12)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 13)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 14)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 15)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 16)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteImpl, kNCPalettePrefix + 25)


// ActionIDs:
DECLARE_PMID(kActionIDSpace, kNCPaletteAboutActionID, kNCPalettePrefix + 0)
DECLARE_PMID(kActionIDSpace, kNCPalettePanelWidgetActionID, kNCPalettePrefix + 1)
DECLARE_PMID(kActionIDSpace, kNCPaletteSeparator1ActionID, kNCPalettePrefix + 2)
DECLARE_PMID(kActionIDSpace, kNCPalettePopupAboutThisActionID, kNCPalettePrefix + 3)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 5)
DECLARE_PMID(kActionIDSpace, kNCPalettePopupPlaceAdActionID, kNCPalettePrefix + 6)
DECLARE_PMID(kActionIDSpace, kNCPalettePopupRemoveFromListActionID, kNCPalettePrefix + 7)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 8)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 9)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 10)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 11)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 12)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 13)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 14)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 15)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 16)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 17)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 18)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 19)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 20)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 21)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 22)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 23)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 24)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 25)


// WidgetIDs:
DECLARE_PMID(kWidgetIDSpace, kNCPalettePanelWidgetID,	kNCPalettePrefix + 0)
DECLARE_PMID(kWidgetIDSpace, kUnplacedAdsListBoxWidgetID,	kNCPalettePrefix + 2) 
DECLARE_PMID(kWidgetIDSpace, kUnplacedAdsListElementRsrcID, kNCPalettePrefix + 3)
DECLARE_PMID(kWidgetIDSpace, kColumn1StaticTextWidgetID, kNCPalettePrefix + 4)
DECLARE_PMID(kWidgetIDSpace, kColumn2StaticTextWidgetID, kNCPalettePrefix + 5)
DECLARE_PMID(kWidgetIDSpace, kNCPaletteListParentWidgetId, kNCPalettePrefix + 6)
DECLARE_PMID(kWidgetIDSpace, kNCPaletteText1WidgetID, kNCPalettePrefix + 7)
DECLARE_PMID(kWidgetIDSpace, kNCPaletteText2WidgetID, kNCPalettePrefix + 8)
DECLARE_PMID(kWidgetIDSpace, kColumn3StaticTextWidgetID, kNCPalettePrefix + 9)
DECLARE_PMID(kWidgetIDSpace, kColumn4StaticTextWidgetID, kNCPalettePrefix + 10)
DECLARE_PMID(kWidgetIDSpace, kColumn5StaticTextWidgetID, kNCPalettePrefix + 11)
DECLARE_PMID(kWidgetIDSpace, kColumn6StaticTextWidgetID, kNCPalettePrefix + 12)
DECLARE_PMID(kWidgetIDSpace, kNCPalettePlaceAdButtonWidgetID, kNCPalettePrefix + 13)
DECLARE_PMID(kWidgetIDSpace, kNCPaletteRemoveButtonWidgetID, kNCPalettePrefix + 14)
DECLARE_PMID(kWidgetIDSpace, kNCPaletteText3WidgetID, kNCPalettePrefix + 15)
DECLARE_PMID(kWidgetIDSpace, kNCPaletteText4WidgetID, kNCPalettePrefix + 16)
DECLARE_PMID(kWidgetIDSpace, kNCPaletteText5WidgetID, kNCPalettePrefix + 17)
DECLARE_PMID(kWidgetIDSpace, kNCPaletteText6WidgetID, kNCPalettePrefix + 18)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 19)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 20)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 21)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 22)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 23)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 25)


// "About Plug-ins" sub-menu:
#define kNCPaletteAboutMenuKey			kNCPaletteStringPrefix "kNCPaletteAboutMenuKey"
#define kNCPaletteAboutMenuPath		kSDKDefStandardAboutMenuPath kNCPaletteCompanyKey

// "Plug-ins" sub-menu:
#define kNCPalettePluginsMenuKey 		kNCPaletteStringPrefix "kNCPalettePluginsMenuKey"
#define kNCPalettePluginsMenuPath		kSDKDefPlugInsStandardMenuPath kNCPaletteCompanyKey kSDKDefDelimitMenuPath kNCPalettePluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kNCPaletteAboutBoxStringKey			kNCPaletteStringPrefix	"kNCPaletteAboutBoxStringKey"
#define kNCPalettePanelTitleKey				kNCPaletteStringPrefix	"kNCPalettePanelTitleKey"
#define kNCPaletteStaticTextKey				kNCPaletteStringPrefix	"kNCPaletteStaticTextKey"
#define kNCPaletteInternalPopupMenuNameKey	kNCPaletteStringPrefix	"kNCPaletteInternalPopupMenuNameKey"
#define kNCPaletteTargetMenuPath			kNCPaletteInternalPopupMenuNameKey

// Column heads for the scrolling list
#define kColumn1StringKey	kNCPaletteStringPrefix "kColumn1StringKey"
#define kColumn2StringKey	kNCPaletteStringPrefix "kColumn2StringKey"
#define kColumn3StringKey	kNCPaletteStringPrefix "kColumn3StringKey"
#define kColumn4StringKey	kNCPaletteStringPrefix "kColumn4StringKey"
#define kColumn5StringKey	kNCPaletteStringPrefix "kColumn5StringKey"
#define kColumn6StringKey	kNCPaletteStringPrefix "kColumn6StringKey"

// Button labels
#define kPlaceAdButtonKey				"Place Ad"
#define kRemoveFromListButtonKey		"Remove From List"


// Menu item positions:

#define kNCPalettePlaceAdMenuItemPosition			1.0
#define kNCPaletteRemoveFromListMenuItemPosition	2.0
#define	kNCPaletteSeparator1MenuItemPosition		10.0
#define kNCPaletteAboutThisMenuItemPosition			11.0


// Initial data format version numbers
#define kNCPaletteFirstMajorFormatNumber  RezLong(1)
#define kNCPaletteFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kNCPaletteCurrentMajorFormatNumber kNCPaletteFirstMajorFormatNumber
#define kNCPaletteCurrentMinorFormatNumber kNCPaletteFirstMinorFormatNumber

#endif // __NCPaletteID_h__

//  Code generated by DollyXs code generator
