#include "..\Shared\DLLBase.h"
#include "DLLUtils.h"
#include <string.h>


// Returns true if h is a recycler or recycler vehicle, false
// if h is invalid, or not a recycler
bool DLLUtils::IsRecycler(Handle h)
{
	char ObjClass[64];
	if(!GetObjInfo(h, Get_GOClass, ObjClass))
	{
		return false;
	}

	return (_stricmp(ObjClass, "CLASS_RECYCLERVEHICLE") == 0) || (_stricmp(ObjClass, "CLASS_RECYCLER") == 0);
}


// Given a team #, counts the number of allied human players currently
// playing. (i.e. not the number of possible allies, but the number of
// actual allies) Note: should return at least 1, because each team is
// an ally of itself. Does not count the neutral team (0), which is an
// ally of all teams by default.
int DLLUtils::CountAlliedPlayers(int team)
{
	int count = 0;
	int i;
	
	for(i=1; i<MAX_TEAMS; ++i)
	{
		if(IsTeamAllied(i, team))
		{
			Handle h = GetPlayerHandle(i);
			if(h != 0)
			{
				++count;
			}
		} // team is allied
	}

	return count;
}

// Sanity wrapper for GetVarItemStr. Reads the specified svar, and
// verifies it's present in the specified list. If not found in
// there, returns NULL.
const char* DLLUtils::GetCheckedNetworkSvar(size_t svar, NETWORK_LIST_TYPE listType)
{
	char svarStr[128];
	sprintf_s(svarStr, "network.session.svar%d", svar);

	const char* pContents = GetVarItemStr(svarStr);
	// Error finding? Bail, asap
	if(pContents == NULL)
	{
		return NULL;
	}

	size_t count = GetNetworkListCount(listType);
	size_t i;
	for(i=0; i<count; ++i)
	{
		const char* pItem = GetNetworkListItem(listType, i);
		if(pItem == NULL)
			continue;

		if(_stricmp(pContents, pItem) == 0)
		{
			// Got a good match. Return it
			return pContents;
		}
	}

	// No match found. Report error
	return NULL;
}
