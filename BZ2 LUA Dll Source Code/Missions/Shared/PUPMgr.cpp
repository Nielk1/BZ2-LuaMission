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
// Re-written by General BlackDragon. 1/20/15

#include "BZ1Helper.h"

#include "DLLBase.h"
#include "PUPMgr.h"

#include <string.h>

std::vector<myprup> PUPMgr::PUPList;

void PUPMgr::Init(BZ1Helper &m_BZ1Helper)
{
	bool Logging = BZ1Helper::Logging;

	for (int i = 0; i < m_BZ1Helper.AIpathCount; ++i)
	{
		const char *label = m_BZ1Helper.AIpathNames[i];

		if ((label == NULL) || // Uh Oh...
			(!strchr(label, ':'))) // No colin, incorrect format.
		//	(!strchr(label, '_')) || // No underscore, incorrect format.
		//	(strncmp(label, "edge_path", 9) == 0) ||  // Don't count Edge_Path. 
		//	(strncmp(label, "spawn_", 6) == 0) ||  // Don't count spawn_ names, that usually means a DLL SP spawn. If you want to use spawn.odf, place it in editor.
		//	(strncmp(label, "path_", 5) == 0)) // Don't count path_ names.
			continue; // Skip this path.

		char ODFName[MAX_ODF_LENGTH] = {0};
		char Label[MAX_ODF_LENGTH] = {0};
		int Team = 0; // Deafult.
		float Timer = 10.0f; // Default.
		//sscanf_s(label, "%[^_]_%f_team%d", ODFName, sizeof(ODFName), &Timer, &Team); // Special note about sscanf_s, a bufsize must be passed in for %s or %c vars, even if it's a fixed-sized buffer.
		sscanf_s(label, "%s:%f:%d:%s", ODFName, sizeof(ODFName), &Timer, &Team, Label, sizeof(Label)); // Special note about sscanf_s, a bufsize must be passed in for %s or %c vars, even if it's a fixed-sized buffer.
		GetWorldVariantODF(ODFName); // Apply world letter, if it applies. -GBD

		if(!DoesODFExist(ODFName))
			continue;

		PUPList.resize(PUPList.size()+1);
		myprup &sao = PUPList[PUPList.size()-1]; // No temporary being created, *much* faster
		memset(&sao, 0, sizeof(myprup)); // Zero things out at start.

		if(Logging)
			FormatLogMessage("PupMgr Path found: %s, odf: %s, Timer: %f, Team: %d, Label: %s", label, ODFName, Timer, Team, Label);

		// Save the info.
		strcpy_s(sao.odf, sizeof(sao.odf), ODFName);
		strcpy_s(sao.str, sizeof(sao.str), label);
		strcpy_s(sao.label, sizeof(sao.label), Label);
		sao.dt = Timer;
		sao.team = Team;
		sao.pupHandle = BuildObject(sao.odf, sao.team, sao.str);
		sao.waiting = false;
		if(sao.label[0])
			SetLabel(sao.pupHandle, sao.label);

		// Remove pilots from vehilces on Team 0, like in BZ1. -GBD
		if((sao.team == 0) && (IsVehicle(sao.pupHandle)))
			RemovePilotAI(sao.pupHandle);

		/*
		char *dst = sao.odf;
		const char *src = label;
		while (*src != '_' && *src != '\0' && dst < sao.odf + sizeof(sao.odf) - 1)
			*dst++ = *src++;
		*dst++ = '\0';

		if(!DoesODFExist(sao.odf))
			continue;

		// save the path label
		strcpy_s(sao.str, sizeof(sao.str), label);

		// if the label contained a '_'...
		if(*src == '_')
		{
			++src;
			// get the respawn period in seconds
			int t = atoi(src);
			sao.dt = t > 0 ? (float)t : 10.0f;
		}
		*/
	}
}

void PUPMgr::Execute(BZ1Helper &bz1Helper)
{
	Time CurTime = GetTime();
	//LogFloat(CurTime);
	for (std::vector<myprup>::iterator iter = PUPList.begin(); iter != PUPList.end(); ++iter)
	{
		/*
		LogFloat(IsAlive(iter->pupHandle));
		LogFloat(iter->waiting);
		LogFloat(iter->dt);
		if (iter->.waiting)
			LogFloat(iter->time);
		*/

		if (!IsAlive(iter->pupHandle) && !iter->waiting)
		{
		//	LogFloat(1.1f);
			iter->waiting = true;
			iter->time = CurTime + iter->dt;
		}
		if (iter->waiting && (CurTime > iter->time))
		{
		//	LogFloat(2.2f);
			iter->pupHandle = BuildObject(iter->odf, iter->team, iter->str);
			iter->waiting = false;

			if(iter->label[0])
				SetLabel(iter->pupHandle, iter->label);

			if((iter->team == 0) && (IsVehicle(iter->pupHandle)))
				RemovePilotAI(iter->pupHandle);
		}
	} 
}

bool PUPMgr::Load(bool missionSave)
{
	if(missionSave)
		return true;

	int size;
	Read(&size, 1);
	PUPList.resize(size);
	if(size)
		Read(&PUPList.front(), sizeof(myprup)*size);

	return true;
}

bool PUPMgr::PostLoad(bool missionSave)
{
	if (missionSave)
		return true;

	for (std::vector<myprup>::iterator iter = PUPList.begin(); iter != PUPList.end(); ++iter)
	{
		myprup &sao = *iter;
		ConvertHandles(&sao.pupHandle, 1);
	}
	return true;
}

bool PUPMgr::Save(bool missionSave)
{
	if(missionSave)
		return true;

	int size = int(PUPList.size());
	Write(&size, 1);
	if(size)
		Write(&PUPList.front(), sizeof(myprup)*size);

	return true;
}