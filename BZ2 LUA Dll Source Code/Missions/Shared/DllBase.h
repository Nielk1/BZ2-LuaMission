#ifndef _DLLBase_H__
#define _DLLBase_H__

#include "..\..\source\fun3d\ScriptUtils.h"

#include <crtdbg.h>
#include <stdlib.h>
#include <memory.h>

#pragma warning (disable : 4514) //unreferenced inline/local function has been removed
#pragma warning (disable : 4100) //unreferenced formal parameter
#pragma warning (disable : 4127)	// conditional expression is constant -- NEEDED FOR _ASSERTE

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

};

// Function to create a new instance of your mission class. Must be present in project
DLLBase *BuildMission(void);

#endif // _DLLBase_H__
