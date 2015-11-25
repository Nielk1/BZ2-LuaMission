#ifndef LUAMISSION_H__
#define LUAMISSION_H__

#include "..\Shared\DLLBase.h"
#include "..\Shared\BZ1Helper.h" //Add in BZ1 Stuff. -GBD
//#include "include\lua.hpp"
#include "lua.hpp"
#include <vector>
#include <hash_map>
#include <algorithm>

#define lua_pushglobaltable(L) lua_pushvalue(L, LUA_GLOBALSINDEX);

struct lua_State;

class LuaMission : public BZ1Helper
{
private:

	int m_GameTPS; // How many turns per second there are (10, 15, 20, or 30)

	lua_State *L;

	// bools
	bool
		b_first,
		DidSetup, // This is the flag that turns off our "Setup" once it's been run. 
		StopScript, // This is a flag to spawn all pre-placed objects on the map and nothing else.

		b_last;

	// floats
	float
		f_first,

		f_last;

	// handles
	Handle
		h_first,

		h_last;

	// integers
	int
		i_first,
		m_ElapsedGameTime,
		Difficulty,

		i_last;

protected:

	bool *b_array;
	int b_count;

	float *f_array;
	int f_count;

	int *h_array;
	int h_count;

	int *i_array;
	int i_count;


public:
	// Calls made from the outside

	// Constructor
	LuaMission(void);
	~LuaMission();

	void InitialSetup(void); // For initialization for LUA code.
	void Setup(void); // This is an optional function, called within Execute. It could be called anything, but since it's run once at start, we use it to setup things for the mission.
	void AddObject(Handle h);
	void DeleteObject(Handle h);
	void Execute(void);
	void PostRun(void);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	bool AddPlayer(DPID id, int Team, bool IsNewPlayer);
	void DeletePlayer(DPID id);
	EjectKillRetCodes PlayerEjected(Handle DeadObjectHandle);
	EjectKillRetCodes ObjectKilled(int DeadObjectHandle, int KillersHandle);
	EjectKillRetCodes ObjectSniped(int DeadObjectHandle, int KillersHandle);

	virtual void PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF);
	virtual PreSnipeReturnCodes PreSnipe(const int curWorld, Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF);
	virtual PreGetInReturnCodes PreGetIn(const int curWorld, Handle pilotHandle, Handle emptyCraftHandle);
	virtual PrePickupPowerupReturnCodes PrePickupPowerup(const int curWorld, Handle me, Handle powerupHandle);
	virtual void PostTargetChangedCallback(Handle craft, Handle previousTarget, Handle currentTarget);

private:
	// Rest of calls are internal functions, thus made private.


}; // end of 'class InstantMission'

#endif // LUAMISSION_H__
