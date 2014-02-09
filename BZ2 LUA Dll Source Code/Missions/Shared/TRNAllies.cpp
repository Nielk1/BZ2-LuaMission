#include "..\Shared\DLLBase.h"
#include "TRNAllies.h"
#include <string.h>

// Pass in a trn file name, e.g. "bridges2i.trn", and it'll set up
// default team allies specified in the trn file. See changelog
// for exact specs of what needs to be in the TRN.
void TRNAllies::SetupTRNAllies(const char* pTRNName)
{
	// Sanity checks
	if((pTRNName == NULL) || (pTRNName[0] == 0))
	{
		return;
	}
	
	OpenODF(const_cast<char*>(pTRNName));

	bool moreEntries = true;
	int entryNum = 1;
	do
	{
		char tempStr[256];
		char keyName[256];
		sprintf_s(keyName, "Entry%d", entryNum);
		if(GetODFString(pTRNName, "DefaultAllies", keyName, sizeof(tempStr), tempStr, ""))
		{
			// Parse entry. Should look like "###_###".
			int teamA;
			int teamB = 0;
			teamA = atoi(tempStr);
			char* pUnderscore = strchr(tempStr, '_');
			if(pUnderscore != NULL)
			{
				teamB = atoi(pUnderscore + 1);
			}

			if(((teamA > 0) && (teamA < 16)) &&
			   ((teamB > 0) && (teamB < 16)))
			{
				Ally(teamA, teamB);
			}

			++entryNum;
		}
		else
		{
			// GetODFString couldn't find the entry. Exit the loop
			moreEntries = false;
		}
	} while (moreEntries);

	CloseODF(const_cast<char*>(pTRNName));
}
