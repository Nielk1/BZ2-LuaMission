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

#include "..\shared\PUPMgr.h"
#include "..\Shared\DLLBase.h"

#include <string.h>

struct myprup PUPMgr::pup[MAX_POWERUPS];
int PUPMgr::pupHandle[MAX_POWERUPS];
int PUPMgr::PUPCount;

void PUPMgr::Init(void)
{
	memset(pup, 0, sizeof(pup));
	PUPCount = 0;
	memset(pupHandle, 0, sizeof(pupHandle));

	int pathCount;
	char **pathNames;
	GetAiPaths(pathCount, pathNames);
	int i;
	for (i = 0; i < pathCount; ++i)
	{
		if(PUPCount <MAX_POWERUPS)
		{
			char *label = pathNames[i];
			if (strncmp(label, "king", 4) == 0)
				continue;	// king of the hill not implemented yet
			if (strncmp(label, "edge_path", 9) == 0)
				continue;	// this is not a powerup
			if (strncmp(label,"spawntank8",9)==0)
				continue;
			if (strncmp(label,"spawnrocket8",11)==0)
				continue;
			if (strncmp(label,"base1",5)==0) //Inserted for CTF
				continue;
			if (strncmp(label,"base2",5)==0) //Inserted for CTF
				continue;
			if (strncmp(label,"path",4)==0) //Inserted to debug
				continue;
			_ASSERTE(PUPCount < SIZEOF(pup));
			if(PUPCount < SIZEOF(pup))
			{
				strncpy_s(pup[PUPCount].str, label, sizeof(pup[PUPCount].str)-1);
				strncpy_s(pup[PUPCount].odf, label, sizeof(pup[PUPCount].odf)-1);
				char *ptr = strchr(pup[PUPCount].odf, '_');
				if(!ptr)
					continue; // Bad format on name. Don't continue
				if (ptr) *ptr = 0;
				ptr++;
				char tmp[64];
				strcpy_s(tmp, ptr);
				ptr = strchr(tmp, '_');
				if (ptr) *ptr = 0;
				int v = atoi(tmp);
				if (v > 0)
					pup[PUPCount].dt = (float) v;
				else
					pup[PUPCount].dt = 10.0f;
				PUPCount++;
			}
		}
	}

	for ( i = 0; i < PUPCount; i++)
	{
		pupHandle[i] = BuildObject(pup[i].odf, 0, pup[i].str);
		pup[i].waiting = false;
	}
}

void PUPMgr::Execute(void)
{
	int i;
	Time CurTime=GetTime();
	LogFloat(CurTime);
	for(i = 0; i < PUPCount; i++)
	{
		LogFloat(IsAlive(pupHandle[i]));
		LogFloat(pup[i].waiting);
		LogFloat(pup[i].dt);
		if (pup[i].waiting)
			LogFloat(pup[i].time);
		if (!IsAlive(pupHandle[i]) && !pup[i].waiting)
		{
			LogFloat(1.1f);
			pup[i].waiting = true;
			pup[i].time = CurTime + pup[i].dt;
		}
		if (pup[i].waiting && (CurTime > pup[i].time))
		{
			LogFloat(2.2f);
			pupHandle[i] = BuildObject(pup[i].odf, 0, pup[i].str);
			pup[i].waiting = false;
		}
	} 
}

bool PUPMgr::Load(bool missionSave)
{
	if(missionSave)
		return true;

	Read(&PUPCount,1);
	_ASSERTE(PUPCount<MAX_POWERUPS);
	if(PUPCount)
	{
		Read((void*)(&(pup[0])),PUPCount*sizeof(myprup));
		Read(pupHandle, PUPCount);
	}
	return true;
}

bool PUPMgr::Save(bool missionSave)
{
	if(missionSave)
		return true;

	_ASSERTE(PUPCount<MAX_POWERUPS);
	Write(&PUPCount,1);
	if(PUPCount)
	{
		Write((void*)(&(pup[0])),PUPCount*sizeof(myprup));
		Write(pupHandle, PUPCount);
	}
	return true;
}

bool PUPMgr::PostLoad(bool missionSave)
{
	if (missionSave)
		return true;
	ConvertHandles(pupHandle, PUPCount);
	return true;
}
