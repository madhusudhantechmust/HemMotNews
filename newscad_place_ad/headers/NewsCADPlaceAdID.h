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
//	DollyXs generated this file initially.  I added tons o' stuff to it, most of which 
//	was lifted from our old CS2 plugin code.
//========================================================================================


#ifndef __NewsCADPlaceAdID_h__
#define __NewsCADPlaceAdID_h__

#include "SDKDef.h"

// Company:
#define kNewsCADPlaceAdCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
#define kNewsCADPlaceAdCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

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
	// This Plug-in:
#define kNewsCADPlaceAdPluginName	"NewsCAD: Place Ad"			// Name of this plug-in.
#define kNewsCADPlaceAdPrefixNumber	0xd9980 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#define kNewsCADPlaceAdVersion		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
#define kNewsCADPlaceAdAuthor		"Ross Edgar"					// Author of this plug-in (for the About Box).

// Plug-in Prefix: (please change kNewsCADPlaceAdPrefixNumber above to modify the prefix.)
#define kNewsCADPlaceAdPrefix		RezLong(kNewsCADPlaceAdPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
#define kNewsCADPlaceAdStringPrefix	SDK_DEF_STRINGIZE(kNewsCADPlaceAdPrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
#define kNewsCADPlaceAdMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
#define kNewsCADPlaceAdMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
DECLARE_PMID(kPlugInIDSpace, kNewsCADPlaceAdPluginID, kNewsCADPlaceAdPrefix + 0)

// ClassIDs:
DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdActionComponentBoss,	kNewsCADPlaceAdPrefix + 0)
DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdPanelWidgetBoss,		kNewsCADPlaceAdPrefix + 1)
DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdTextWidgetBoss,		kNewsCADPlaceAdPrefix + 3)
DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdListBoxWidgetBoss,	kNewsCADPlaceAdPrefix + 4)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 5)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 6)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 7)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 8)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 9)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 10)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 11)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 12)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 13)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 14)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 15)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 16)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 17)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 18)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 19)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 20)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 21)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 22)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 23)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 24)
//DECLARE_PMID(kClassIDSpace, kNewsCADPlaceAdBoss, kNewsCADPlaceAdPrefix + 25)


// InterfaceIDs:
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 0)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 1)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 2)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 3)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 4)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 5)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 6)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 7)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 8)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 9)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 10)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 11)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 12)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 13)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 14)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 15)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 16)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 17)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 18)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 19)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 20)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 21)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 22)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 23)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 24)
//DECLARE_PMID(kInterfaceIDSpace, IID_INEWSCADPLACEADINTERFACE, kNewsCADPlaceAdPrefix + 25)


// ImplementationIDs:
DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdActionComponentImpl,	kNewsCADPlaceAdPrefix + 0 )
DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdListBoxObserverImpl,	kNewsCADPlaceAdPrefix + 1)
DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdPanelObserverImpl,		kNewsCADPlaceAdPrefix + 2)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 3)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 4)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 5)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 6)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 7)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 8)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 9)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 10)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 11)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 12)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 13)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 14)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 15)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 16)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 17)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 18)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 19)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 20)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 21)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 22)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 23)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 24)
//DECLARE_PMID(kImplementationIDSpace, kNewsCADPlaceAdImpl, kNewsCADPlaceAdPrefix + 25)


// ActionIDs:
DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdAboutActionID, 					kNewsCADPlaceAdPrefix + 0)
DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdPanelWidgetActionID, 			kNewsCADPlaceAdPrefix + 1)
DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdSeparator1ActionID, 			kNewsCADPlaceAdPrefix + 2)
DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdPopupAboutThisActionID, 		kNewsCADPlaceAdPrefix + 3)
DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdPopupPlaceAdActionID, 			kNewsCADPlaceAdPrefix + 5)
DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdPopupRemoveFromListActionID,	kNewsCADPlaceAdPrefix + 6)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 7)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 8)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 9)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 10)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 11)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 12)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 13)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 14)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 15)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 16)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 17)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 18)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 19)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 20)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 21)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 22)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 23)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 24)
//DECLARE_PMID(kActionIDSpace, kNewsCADPlaceAdActionID, kNewsCADPlaceAdPrefix + 25)


// WidgetIDs:
DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdPanelWidgetID,			kNewsCADPlaceAdPrefix + 0)
DECLARE_PMID(kWidgetIDSpace, kUnplacedAdsListBoxWidgetID,			kNewsCADPlaceAdPrefix + 2)
DECLARE_PMID(kWidgetIDSpace, kUnplacedAdsListElementRsrcID,			kNewsCADPlaceAdPrefix + 3)
DECLARE_PMID(kWidgetIDSpace, kColumn1StaticTextWidgetID,			kNewsCADPlaceAdPrefix + 4)
DECLARE_PMID(kWidgetIDSpace, kColumn2StaticTextWidgetID,			kNewsCADPlaceAdPrefix + 5)
DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdListParentWidgetID,		kNewsCADPlaceAdPrefix + 6)
DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdText1WidgetID,			kNewsCADPlaceAdPrefix + 7)
DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdText2WidgetID,			kNewsCADPlaceAdPrefix + 8)
DECLARE_PMID(kWidgetIDSpace, kColumn3StaticTextWidgetID,			kNewsCADPlaceAdPrefix + 9)
DECLARE_PMID(kWidgetIDSpace, kColumn4StaticTextWidgetID,			kNewsCADPlaceAdPrefix + 10)
DECLARE_PMID(kWidgetIDSpace, kColumn5StaticTextWidgetID,			kNewsCADPlaceAdPrefix + 11)
DECLARE_PMID(kWidgetIDSpace, kColumn6StaticTextWidgetID,			kNewsCADPlaceAdPrefix + 12)
DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdPlaceAdButtonWidgetID,	kNewsCADPlaceAdPrefix + 13)
DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdRemoveButtonWidgetID,	kNewsCADPlaceAdPrefix + 14)
DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdText3WidgetID,			kNewsCADPlaceAdPrefix + 15)
DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdText4WidgetID,			kNewsCADPlaceAdPrefix + 16)
DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdText5WidgetID,			kNewsCADPlaceAdPrefix + 17)
DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdText6WidgetID,			kNewsCADPlaceAdPrefix + 18)
//DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdWidgetID, kNewsCADPlaceAdPrefix + 19)
//DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdWidgetID, kNewsCADPlaceAdPrefix + 20)
//DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdWidgetID, kNewsCADPlaceAdPrefix + 21)
//DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdWidgetID, kNewsCADPlaceAdPrefix + 22)
//DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdWidgetID, kNewsCADPlaceAdPrefix + 23)
//DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdWidgetID, kNewsCADPlaceAdPrefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kNewsCADPlaceAdWidgetID, kNewsCADPlaceAdPrefix + 25)


// "About Plug-ins" sub-menu:
#define kNewsCADPlaceAdAboutMenuKey			kNewsCADPlaceAdStringPrefix "kNewsCADPlaceAdAboutMenuKey"
#define kNewsCADPlaceAdAboutMenuPath		kSDKDefStandardAboutMenuPath kNewsCADPlaceAdCompanyKey

// "Plug-ins" sub-menu:
#define kNewsCADPlaceAdPluginsMenuKey 		kNewsCADPlaceAdStringPrefix "kNewsCADPlaceAdPluginsMenuKey"
#define kNewsCADPlaceAdPluginsMenuPath		kSDKDefPlugInsStandardMenuPath kNewsCADPlaceAdCompanyKey kSDKDefDelimitMenuPath kNewsCADPlaceAdPluginsMenuKey

// Menu item keys:

// Other StringKeys:
#define kNewsCADPlaceAdAboutBoxStringKey		kNewsCADPlaceAdStringPrefix "kNewsCADPlaceAdAboutBoxStringKey"
#define kNewsCADPlaceAdPanelTitleKey					kNewsCADPlaceAdStringPrefix	"kNewsCADPlaceAdPanelTitleKey"
#define kNewsCADPlaceAdStaticTextKey			kNewsCADPlaceAdStringPrefix	"kNewsCADPlaceAdStaticTextKey"
#define kNewsCADPlaceAdInternalPopupMenuNameKey	kNewsCADPlaceAdStringPrefix	"kNewsCADPlaceAdInternalPopupMenuNameKey"
#define kNewsCADPlaceAdTargetMenuPath			kNewsCADPlaceAdInternalPopupMenuNameKey

// Column heads for the scrolling list
#define kColumn1StringKey	kNewsCADPlaceAdStringPrefix "kColumn1StringKey"
#define kColumn2StringKey	kNewsCADPlaceAdStringPrefix "kColumn2StringKey"
#define kColumn3StringKey	kNewsCADPlaceAdStringPrefix "kColumn3StringKey"
#define kColumn4StringKey	kNewsCADPlaceAdStringPrefix "kColumn4StringKey"
#define kColumn5StringKey	kNewsCADPlaceAdStringPrefix "kColumn5StringKey"
#define kColumn6StringKey	kNewsCADPlaceAdStringPrefix "kColumn6StringKey"

// Button labels
#define kPlaceAdButtonKey				"Place Ad"
#define kRemoveFromListButtonKey		"Remove From List"

// Menu item positions:
#define kNewsCADPlaceAdPlaceAdMenuItemPosition			1.0
#define kNewsCADPlaceAdRemoveFromListMenuItemPosition	2.0
#define	kNewsCADPlaceAdSeparator1MenuItemPosition		10.0
#define kNewsCADPlaceAdAboutThisMenuItemPosition		11.0


// Initial data format version numbers
#define kNewsCADPlaceAdFirstMajorFormatNumber  RezLong(1)
#define kNewsCADPlaceAdFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
#define kNewsCADPlaceAdCurrentMajorFormatNumber kNewsCADPlaceAdFirstMajorFormatNumber
#define kNewsCADPlaceAdCurrentMinorFormatNumber kNewsCADPlaceAdFirstMinorFormatNumber

#endif // __NewsCADPlaceAdID_h__

//  Code generated by DollyXs code generator

// Company:
//#define kNCPaletteCompanyKey	kSDKDefPlugInCompanyKey		// Company name used internally for menu paths and the like. Must be globally unique, only A-Z, 0-9, space and "_".
//#define kNCPaletteCompanyValue	kSDKDefPlugInCompanyValue	// Company name displayed externally.

// Plug-in:
//#define kNCPalettePluginName	"NewsCAD Palette"			// Name of this plug-in.
//#define kNCPalettePrefixNumber	0xd9900 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
//#define kNCPaletteVersion		kSDKDefPluginVersionString						// Version of this plug-in (for the About Box).
//#define kNCPaletteAuthor		"Ross L. Edgar"					// Author of this plug-in (for the About Box).

// NewsCAD plugin
//#define kWFPPrefixNumber	0xd9940 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
//#define kWFPPrefix			RezLong(kWFPPrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
//DECLARE_PMID(kPlugInIDSpace, kWFPPluginID, kWFPPrefix + 0)

// Plug-in Prefix: (please change kNCPalettePrefixNumber above to modify the prefix.)
//#define kNCPalettePrefix		RezLong(kNCPalettePrefixNumber)				// The unique numeric prefix for all object model IDs for this plug-in.
//#define kNCPaletteStringPrefix	SDK_DEF_STRINGIZE(kNCPalettePrefixNumber)	// The string equivalent of the unique prefix number for  this plug-in.

// Missing plug-in: (see ExtraPluginInfo resource)
//#define kNCPaletteMissingPluginURLValue		kSDKDefPartnersStandardValue_enUS // URL displayed in Missing Plug-in dialog
//#define kNCPaletteMissingPluginAlertValue	kSDKDefMissingPluginAlertValue // Message displayed in Missing Plug-in dialog - provide a string that instructs user how to solve their missing plug-in problem

// PluginID:
//DECLARE_PMID(kPlugInIDSpace, kNCPalettePluginID, kNCPalettePrefix + 0)

// ClassIDs:
//DECLARE_PMID(kClassIDSpace, kNCPaletteActionComponentBoss, kNCPalettePrefix + 0)
//DECLARE_PMID(kClassIDSpace, kNCPalettePanelWidgetBoss, kNCPalettePrefix + 1)
//DECLARE_PMID(kClassIDSpace, kNCPaletteTextWidgetBoss, kNCPalettePrefix + 3)
//DECLARE_PMID(kClassIDSpace, kNCPaletteListBoxWidgetBoss, kNCPalettePrefix + 4)
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
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteActionComponentImpl, kNCPalettePrefix + 0 )
//DECLARE_PMID(kImplementationIDSpace, kNCPaletteListBoxObserverImpl, kNCPalettePrefix + 1)
//DECLARE_PMID(kImplementationIDSpace, kNCPalettePanelObserverImpl, kNCPalettePrefix + 2)
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
//DECLARE_PMID(kActionIDSpace, kNCPaletteAboutActionID, kNCPalettePrefix + 0)
//DECLARE_PMID(kActionIDSpace, kNCPalettePanelWidgetActionID, kNCPalettePrefix + 1)
//DECLARE_PMID(kActionIDSpace, kNCPaletteSeparator1ActionID, kNCPalettePrefix + 2)
//DECLARE_PMID(kActionIDSpace, kNCPalettePopupAboutThisActionID, kNCPalettePrefix + 3)
//DECLARE_PMID(kActionIDSpace, kNCPaletteActionID, kNCPalettePrefix + 5)
//DECLARE_PMID(kActionIDSpace, kNCPalettePopupPlaceAdActionID, kNCPalettePrefix + 6)
//DECLARE_PMID(kActionIDSpace, kNCPalettePopupRemoveFromListActionID, kNCPalettePrefix + 7)
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
//DECLARE_PMID(kWidgetIDSpace, kNCPalettePanelWidgetID,	kNCPalettePrefix + 0)
//DECLARE_PMID(kWidgetIDSpace, kUnplacedAdsListBoxWidgetID,	kNCPalettePrefix + 2) 
//DECLARE_PMID(kWidgetIDSpace, kUnplacedAdsListElementRsrcID, kNCPalettePrefix + 3)
//DECLARE_PMID(kWidgetIDSpace, kColumn1StaticTextWidgetID, kNCPalettePrefix + 4)
//DECLARE_PMID(kWidgetIDSpace, kColumn2StaticTextWidgetID, kNCPalettePrefix + 5)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteListParentWidgetId, kNCPalettePrefix + 6)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteText1WidgetID, kNCPalettePrefix + 7)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteText2WidgetID, kNCPalettePrefix + 8)
//DECLARE_PMID(kWidgetIDSpace, kColumn3StaticTextWidgetID, kNCPalettePrefix + 9)
//DECLARE_PMID(kWidgetIDSpace, kColumn4StaticTextWidgetID, kNCPalettePrefix + 10)
//DECLARE_PMID(kWidgetIDSpace, kColumn5StaticTextWidgetID, kNCPalettePrefix + 11)
//DECLARE_PMID(kWidgetIDSpace, kColumn6StaticTextWidgetID, kNCPalettePrefix + 12)
//DECLARE_PMID(kWidgetIDSpace, kNCPalettePlaceAdButtonWidgetID, kNCPalettePrefix + 13)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteRemoveButtonWidgetID, kNCPalettePrefix + 14)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteText3WidgetID, kNCPalettePrefix + 15)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteText4WidgetID, kNCPalettePrefix + 16)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteText5WidgetID, kNCPalettePrefix + 17)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteText6WidgetID, kNCPalettePrefix + 18)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 19)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 20)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 21)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 22)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 23)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 24)
//DECLARE_PMID(kWidgetIDSpace, kNCPaletteWidgetID, kNCPalettePrefix + 25)


// "About Plug-ins" sub-menu:
//#define kNCPaletteAboutMenuKey			kNCPaletteStringPrefix "kNCPaletteAboutMenuKey"
//#define kNCPaletteAboutMenuPath		kSDKDefStandardAboutMenuPath kNCPaletteCompanyKey

// "Plug-ins" sub-menu:
//#define kNCPalettePluginsMenuKey 		kNCPaletteStringPrefix "kNCPalettePluginsMenuKey"
//#define kNCPalettePluginsMenuPath		kSDKDefPlugInsStandardMenuPath kNCPaletteCompanyKey kSDKDefDelimitMenuPath kNCPalettePluginsMenuKey

// Menu item keys:

// Other StringKeys:
//#define kNCPaletteAboutBoxStringKey			kNCPaletteStringPrefix	"kNCPaletteAboutBoxStringKey"
//#define kNCPalettePanelTitleKey				kNCPaletteStringPrefix	"kNCPalettePanelTitleKey"
//#define kNCPaletteStaticTextKey				kNCPaletteStringPrefix	"kNCPaletteStaticTextKey"
//#define kNCPaletteInternalPopupMenuNameKey	kNCPaletteStringPrefix	"kNCPaletteInternalPopupMenuNameKey"
//#define kNCPaletteTargetMenuPath			kNCPaletteInternalPopupMenuNameKey

// Column heads for the scrolling list
//#define kColumn1StringKey	kNCPaletteStringPrefix "kColumn1StringKey"
//#define kColumn2StringKey	kNCPaletteStringPrefix "kColumn2StringKey"
//#define kColumn3StringKey	kNCPaletteStringPrefix "kColumn3StringKey"
//#define kColumn4StringKey	kNCPaletteStringPrefix "kColumn4StringKey"
//#define kColumn5StringKey	kNCPaletteStringPrefix "kColumn5StringKey"
//#define kColumn6StringKey	kNCPaletteStringPrefix "kColumn6StringKey"

// Button labels
//#define kPlaceAdButtonKey				"Place Ad"
//#define kRemoveFromListButtonKey		"Remove From List"


// Menu item positions:

//#define kNCPalettePlaceAdMenuItemPosition			1.0
//#define kNCPaletteRemoveFromListMenuItemPosition	2.0
//#define	kNCPaletteSeparator1MenuItemPosition		10.0
//#define kNCPaletteAboutThisMenuItemPosition			11.0


// Initial data format version numbers
//#define kNCPaletteFirstMajorFormatNumber  RezLong(1)
//#define kNCPaletteFirstMinorFormatNumber  RezLong(0)

// Data format version numbers for the PluginVersion resource 
//#define kNCPaletteCurrentMajorFormatNumber kNCPaletteFirstMajorFormatNumber
//#define kNCPaletteCurrentMinorFormatNumber kNCPaletteFirstMinorFormatNumber

//#endif // __NCPaletteID_h__

//  Code generated by DollyXs code generator
