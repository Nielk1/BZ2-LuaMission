//
// BZ2 Network Manager (StartingVehicles) .cpp source code
//
// Subsection: Code that reads in the StartingVehicles.txt file 
//
// Copyright 1998 Pandemic Studios, LLC
//
// NM 8/14/98

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <assert.h>
#include "..\Shared\DLLBase.h"
#include "StartingVehicles.h"

#define VEHICLE_SPACING_DISTANCE (30.0f)

// Set this to 1 to compile against 1.3.6.2 public beta DLL source. Set to 0
// to compile against later, which is more secure.
#define COMPATIBLE_WITH_1_3_6_2 0

namespace StartingVehicleManager
{
	// Max # of vehicles that can be in the list. Do *NOT* put this over
	// 32 without expanding the # of bits used to send this across the
	// network. ivar4 is used to encode this
	const int MAX_STARTING_VEHICLES = 32;

	// Max name of an ODF filename
	const U32 MAX_ODFNAMELEN = 64;

	char s_StartingVehicleList[MAX_STARTING_VEHICLES][MAX_ODFNAMELEN];
	// Size of the above array (i.e. the # of entries with non-blank data
	int s_NumStartingVehicleEntries = 0;

	static bool s_bInitialized = false;

	// Read in and initialize the list of vehicles
	void Init(void)
	{
		if(s_bInitialized)
			return;

		// Pre-clear this
		memset(s_StartingVehicleList, 0, sizeof(s_StartingVehicleList));

		size_t i;
		for(i=0;i<MAX_STARTING_VEHICLES;i++)
		{
			const char* pContents;
#if COMPATIBLE_WITH_1_3_6_2
			char tempStr[128];
			sprintf_s(tempStr, "network.stratstarting.entry%d", i);
			pContents = GetVarItemStr(tempStr);
#else
			pContents = GetNetworkListItem(NETLIST_StratStarting, i);
#endif
			if(pContents != NULL)
			{
				strcpy_s(s_StartingVehicleList[i], pContents);
			}
		}

		// Count entries, stopping when we run into a blank one
		s_NumStartingVehicleEntries = 0;
		for(i=0;i<MAX_STARTING_VEHICLES;i++)
		{
			if(s_StartingVehicleList[i][0])
				s_NumStartingVehicleEntries++;
			else
				break;
		}

		s_bInitialized = true;
	}

	// Do any memory cleanups necessary
	void Cleanup(void)
	{
		// Do nothing for now; have a static array above
	}

	// Given a bitmask and team #, create all vehicles for them.
	void CreateVehicles(const int Team, const char TeamRace, const int Bitmask, const Vector Where)
	{
		char NewODF[MAX_ODFNAMELEN+4];
		Vector RandomizedPosition;
		int i;
		Handle VehicleH = 0;

		if(!s_bInitialized)
			return;

		for(i=0;i<s_NumStartingVehicleEntries;i++)
		{
			if(Bitmask & (1<<i))
			{
				// Need to build this.
				RandomizedPosition=GetPositionNear(Where, VEHICLE_SPACING_DISTANCE, 4*VEHICLE_SPACING_DISTANCE);
				strcpy_s(NewODF, s_StartingVehicleList[i]);
				NewODF[sizeof(NewODF)-1] = 0;

				NewODF[0]=TeamRace;

				VehicleH=BuildObject(NewODF, Team, RandomizedPosition);
				SetRandomHeadingAngle(VehicleH);
				SetBestGroup(VehicleH);
			} // bit is on-- need to build.
		} // loop over all entries.

	}

	// Load/save the read-in list of vehicles out to disk
	void Save(bool MissionSave)
	{
		if (MissionSave)
			return;

		// Why do we do this? So the host can't cheat by tweaking svars
		// after launch.  If the info read in by the server is saved
		// along with all the other data they send to clients on join, 
		// then clients should be guaranteed to get the same info.

		Write(&s_NumStartingVehicleEntries, 1);
		for(int i=0;i<s_NumStartingVehicleEntries;i++)
		{
			Write(s_StartingVehicleList[i], sizeof(s_StartingVehicleList[i]));
		}
	}

	void Load(bool MissionSave)
	{
		if (MissionSave)
			return;

		Read(&s_NumStartingVehicleEntries, 1);
		for(int i=0;i<s_NumStartingVehicleEntries;i++)
		{
			Read(s_StartingVehicleList[i], sizeof(s_StartingVehicleList[i]));
		}
	}
} // 	namespace StartingVehicleManager
