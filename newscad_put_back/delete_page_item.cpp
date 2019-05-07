/*------------------------------------------------------------------------------
	sn	Source Name:
	sn	delete_page_item.cpp
	
	au	Author:
	au	Ross L. Edgar
	
	da	Date
	da	4/10/06 ca. 10:10 PM
	
	ab	Abstract:
	ab	Utility code to delete a page item.
	ab	
	
	cl	Change Log:
	cl	4/10/06	New.
------------------------------------------------------------------------------*/

#include "PageItemScrapID.h"			// for 'kDeleteCmdBoss'
#include "ICommand.h"
#include "CmdUtils.h"
#include "UIDList.h"

/* ---------------------------- P R O T O T Y P E S ----------------------------- */

void DeletePageItem ( UIDRef pageItemUID );

void DeletePageItem ( UIDRef pageItemUID )
{
	InterfacePtr<ICommand> deleteCmd (CmdUtils::CreateCommand (kDeleteCmdBoss));
	if ( deleteCmd )
	{
		deleteCmd->SetItemList (UIDList (pageItemUID) );
		CmdUtils::ProcessCommand (deleteCmd);
	}
}
