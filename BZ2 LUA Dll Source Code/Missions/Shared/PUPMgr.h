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
// Re-Written by General BlackDragon, 1/20/15.

class BZ1Helper;

struct myprup {
	Handle pupHandle;
	float time;
	float dt;
	int team;
	char str[MAX_ODF_LENGTH];
	char odf[MAX_ODF_LENGTH];
	char label[MAX_ODF_LENGTH];
	bool waiting;
};

class PUPMgr {
private:

	static std::vector<myprup> PUPList;

public:

	// Scan over lists of AIPaths in the world, generate tables for powerups
	static void Init(BZ1Helper &m_BZ1Helper);

	// Run occasionally
	static void Execute(BZ1Helper &bz1Helper);

	// Load & save functions
	static bool Load(bool missionSave);
	static bool Save(bool missionSave);
	static bool PostLoad(bool missionSave);
};

#endif
