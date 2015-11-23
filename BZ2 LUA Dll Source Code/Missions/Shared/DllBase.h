#ifndef _DLLBase_H__
#define _DLLBase_H__

#include "..\..\source\fun3d\ScriptUtils.h"

#include <crtdbg.h>
#include <stdlib.h>
#include <memory.h>

#pragma warning (disable : 4514) //unreferenced inline/local function has been removed
#pragma warning (disable : 4100) //unreferenced formal parameter
#pragma warning (disable : 4127)	// conditional expression is constant -- NEEDED FOR _ASSERTE


//
// Warnings we don't want
//
#pragma warning(disable: 4201 4505 4121)
#pragma warning(disable: 4324) // c:\src\bz2\trunk\source\fun3d\factory.h(167) : warning C4324: 'Factory' : structure was padded due to __declspec(align())

// Warnings we DO want
#pragma warning (error : 4013) //'function' undefined; assuming extern returning int
#pragma warning (error : 4020) // too many actual parameters (i.e. different from declaration)
#pragma warning (error : 4024) // formal parameter N different from declaration
#pragma warning (error : 4028) // formal parameter N different from declaration (not quite C4024)
#pragma warning (error : 4033) // function must return a value
#pragma warning (error : 4087) // function : declared with 'void' parameter list
#pragma warning (error : 4133) //'type' : incompatible types - from 'type1' to 'type2'
#pragma warning (error : 4307) // integral constant overflow
#pragma warning (error : 4700) // local variable 'foo' used without having been initialized
#pragma warning (error : 4701) // local variable 'foo' may used without having been initialized
#pragma warning (error : 4715) // function: not all control paths return a value
#pragma warning (error : 4265) // warning C4265: class has virtual functions, but destructor is not virtual
#pragma warning (error : 4905) // wide string literal cast to 'LPSTR'
#pragma warning (error : 4906) // string literal cast to 'LPWSTR'
#pragma warning (error : 4431) // 'missing type specifier - int assumed. Note: C no longer supports default-int
#pragma warning (error : 4806) // unsafe operation: no value of type 'bool' promoted to type 'int' can equal the given constant



#define DPID_UNKNOWN		0xFFFFFFFF

class DLLBase {
public:
	virtual ~DLLBase()
	{
	}

	// Set of virtual functions mirroring misnExport structure.
	virtual void InitialSetup(void);
	virtual bool Save(bool missionSave);
	virtual bool Load(bool missionSave);
	virtual bool PostLoad(bool missionSave);
	virtual void AddObject(Handle h);
	virtual void DeleteObject(Handle h);
	virtual void Execute(void);
	virtual void PostRun(void);
	virtual bool AddPlayer(DPID id, int Team, bool ShouldCreateThem);
	virtual void DeletePlayer(DPID id);
	virtual EjectKillRetCodes PlayerEjected(Handle DeadObjectHandle);
	virtual EjectKillRetCodes ObjectKilled(int DeadObjectHandle,int KillersHandle);
	virtual EjectKillRetCodes ObjectSniped(int DeadObjectHandle,int KillersHandle);
	virtual char *GetNextRandomVehicleODF(int Team);
	virtual void SetWorld(int nextWorld);
	virtual void ProcessCommand(unsigned long crc);
	virtual void SetRandomSeed(unsigned long seed);

	// Items added after 1.3.6.4 public beta

	// Notification to the DLL: a sniper shell has hit a piloted
	// craft. The exe passes the current world, shooters handle,
	// victim handle, and the ODF string of the ordnance involved in
	// the snipe. Returns a code detailing what to do.
	//
	// !! Note : If DLLs want to do any actions to the world based on this
	// PreSnipe callback, they should (1) Ensure curWorld == 0 (lockstep)
	// -- do NOTHING if curWorld is != 0, and (2) probably queue up an
	// action to do in the next Execute() call.
	virtual PreSnipeReturnCodes PreSnipe(const int curWorld, Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF);

	// Notification to the DLL: ordnance has hit something. This
	// passes the shooters handle, victim handle, ordnance's team, and
	// the ODF string of the ordnance involved. Note that the shooters
	// handle *might* be invalid -- consider a mortar (or other slow
	// ordnance) launched by a craft just before that craft
	// explodes. By the time that mortar lands, the shooter is dead.
	// Thus the ordnance's team is also passed as a possible fallback
	// for identification.
	//
	// This call may not quite catch all ordnance hit events yet; anything
	// derived from a Bullet will probably work well. Anything else may or
	// may not work.  For sniper shells hitting targets, they should
	// always generate a PreOrdnanceHitCallback (in the lockstep world),
	// followed by (an optional) PreSnipeCallback if the sniper shell hit
	// a snipable cockpit, etc. In visual worlds, sniper shells will only
	// generate the PreSnipeCallback.
	//
	virtual void PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF);

	// Notification to the DLL: called when a pilot tries to enter an
	// empty craft, and all other checks (i.e. craft is empty, masks
	// match, etc) have passed. DLLs can prevent that pilot from
	// entering the craft if desired.
	//
	// !! Note : If DLLs want to do any actions to the world based on this
	// PreSnipe callback, they should (1) Ensure curWorld == 0 (lockstep)
	// -- do NOTHING if curWorld is != 0, and (2) probably queue up an
	// action to do in the next Execute() call.
	virtual PreGetInReturnCodes PreGetIn(const int curWorld, Handle pilotHandle, Handle emptyCraftHandle);

	// Notification to the DLL: called when a pilot/craft is about to
	// pickup a power up, and all other checks have passed. DLLs can
	// prevent that pick up if desired.
	//
	// !! Note : If DLLs want to do any actions to the world based on this
	// PreSnipe callback, they should (1) Ensure curWorld == 0 (lockstep)
	// -- do NOTHING if curWorld is != 0, and (2) probably queue up an
	// action to do in the next Execute() call.
	virtual PrePickupPowerupReturnCodes PrePickupPowerup(const int curWorld, Handle me, Handle powerupHandle);

	// Notification to the DLL: called when a pilot/craft has changed targets
	virtual void PostTargetChangedCallback(Handle craft, Handle previousTarget, Handle currentTarget);
};

// Function to create a new instance of your mission class. Must be present in project
DLLBase *BuildMission(void);

#endif // _DLLBase_H__
