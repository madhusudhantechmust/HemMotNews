/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	newscad.h
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	4/25/10 ca. 4:54 PM
	
	ab	Abstract:
	ab	
	
	cl	Change Log:
	cl	4/25/10	New.
------------------------------------------------------------------------------*/

#ifndef newscad_H
#define newscad_H

// End products
#define	INDESIGN_PLUGIN		1
#define	QUARKXTENSION		2

// Client
#define	HEMMINGS		1
#define	CLIENT			HEMMINGS

#ifndef PRODUCT
#define	PRODUCT			INDESIGN_PLUGIN
#endif

// Plugin IDs
// NewsCAD plugin
// Unique prefix number for the "main" plugin that contains all the menus and dialogs.
// The plugin that makes the text and picture frames uses this ID when it adds the 
// black box data that identifies the page item as an item belonging to NewsCAD.
// These next 4 lines were copied in here from /id6sdk/source/frank/newscad_main/NC_ID.h
#ifndef kNC_PrefixNumber
#define kNC_PrefixNumber	0xd9940 			// Main 
#endif
#ifndef kNC_Prefix
#define kNC_Prefix		RezLong(kNC_PrefixNumber)				// The unique numeric prefix for all object model IDs for the "main" plug-in.
DECLARE_PMID(kPlugInIDSpace, kNC_PluginID, kNC_Prefix + 0)
#endif
#ifndef kNC_StringPrefix
#define kNC_StringPrefix	SDK_DEF_STRINGIZE(kNC_PrefixNumber)	// The string equivalent of the unique prefix number for  the "main" plug-in.
#endif


// Import Ad Geometry
#ifndef kImportGeomPrefixNumber
#define kImportGeomPrefixNumber	0xd9990 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#endif

// Import Art Files
#ifndef kImportArtPrefixNumber
#define kImportArtPrefixNumber	0xd99a0 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#endif

// Typeset Continued Lines
#ifndef kTypesetContinuedLinesPrefixNumber
#define kTypesetContinuedLinesPrefixNumber	0xd99b0 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#endif

// Create Ad Report
#ifndef kWriteAdReportPrefixNumber
#define kWriteAdReportPrefixNumber	0xd99c0 		// Unique prefix number for this plug-in(*Must* be obtained from Adobe Developer Support).
#endif

// Compilation version
#define IN_HOUSE 1
#define PRODUCTION 2
#define VERSION	IN_HOUSE

#endif

// END OF FILE