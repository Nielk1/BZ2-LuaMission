#ifndef _PUPMgr_h_
#define _PUPMgr_h_

//
// PUPMgr.cpp
// 
// Utility functions for DLLs-- 'PowerUP ManaGeR'-- handles the list
// of powerups given in the mission, respawns them as appropriate,
// etc.  Pulled out to a separate set of functions from a deathmatch
// cpp file to be more generic
//
// Converted to new DLL interface spec by Nathan Mates 2/23/99
//

// Max # of powerups we can have in the world at once.
#define MAX_POWERUPS 100

struct myprup {
	float time;
	float dt;
	char str[64];
	char odf[64];
	bool waiting;
};

class PUPMgr {
private:
	static struct myprup pup[MAX_POWERUPS];
	static int pupHandle[MAX_POWERUPS];
	static int PUPCount; // How many are used in the above list.

public:
	// Scan over lists of AIPaths in the world, generate tables for powerups
	static void Init(void);

	// Run occasionally
	static void Execute(void);

	// Load & save functions
	static bool Load(bool missionSave);
	static bool Save(bool missionSave);
	static bool PostLoad(bool missionSave);
};

#endif
