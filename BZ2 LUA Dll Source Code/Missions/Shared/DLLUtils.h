#ifndef DLLUTILS_H__
#define DLLUTILS_H__

// A few handy functions that can be used from multiple DLLs

namespace DLLUtils
{
	// Returns true if h is a recycler or recycler vehicle, false
	// if h is invalid, or not a recycler
	bool IsRecycler(Handle h);

	// Given a team #, counts the number of allied human players
	// currently playing. (i.e. not the number of possible allies, but
	// the number of actual allies) Note: should return at least 1,
	// because team is an ally of itself. Does not count the neutral
	// team (0), which is an ally of all teams by default.
	int CountAlliedPlayers(int team);


	// Sanity wrapper for GetVarItemStr. Reads the specified svar, and
	// verifies it's present in the specified list. If not found in
	// there, returns NULL.
	const char* GetCheckedNetworkSvar(size_t svar, NETWORK_LIST_TYPE listType);

}

#endif
