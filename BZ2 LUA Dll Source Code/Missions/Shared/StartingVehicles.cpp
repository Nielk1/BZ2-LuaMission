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
#include "..\Shared\BZCScriptUtils.h"
#include "StartingVehicles.h"

#define VEHICLE_SPACING_DISTANCE (30.0f)

namespace StartingVehicleManager
{
	// Max # of vehicles that can be in the list. Do *NOT* put this over 32 without expanding the # of bits used to send this across the network. 
	// ivar4 is used to encode this.
	const int MAX_STARTING_VEHICLES = 32;

	char StartingVehicleList[MAX_STARTING_VEHICLES][MAX_ODF_LENGTH];
	// Size of the above array (i.e. the # of entries with non-blank data
	int NumStartingVehicleEntries = 0;

	static bool Initialized = false;

	// Read in and initialize the list of vehicles
	void Init(const char *Filename)
	{
		if(Initialized)
			return;

		// Pre-clear this
		memset(StartingVehicleList, 0, sizeof(StartingVehicleList));

		for(int i = 0; i < MAX_STARTING_VEHICLES; i++)
		{
			const char *Contents = GetNetworkListItem(NETLIST_StratStarting, i);
			if(Contents != NULL)
				strcpy_s(StartingVehicleList[i], Contents);
		}

		// Count entries, stopping when we run into a blank one
		NumStartingVehicleEntries = 0;
		for(int i = 0; i < MAX_STARTING_VEHICLES; i++)
		{
			if(StartingVehicleList[i][0])
				NumStartingVehicleEntries++;
			else
				break;
		}

		Initialized=true;
	}

	// Do any memory cleanups necessary
	void Cleanup(void)
	{
		// Do nothing for now; have a static array above
	}

	// Given a bitmask and team #, create all vehicles for them.
	void CreateVehicles(const int Team, const char TeamRace, const int Bitmask, const Vector Where)
	{
		char NewODF[MAX_ODF_LENGTH];
		Vector RandomizedPosition(0, 0, 0);
		Handle VehicleH = 0;

		if(!Initialized)
			return;

		for(int i = 0; i < NumStartingVehicleEntries; i++)
		{
			if(Bitmask & (1<<i))
			{
				// Need to build this.
				RandomizedPosition = GetPositionNear(Where, VEHICLE_SPACING_DISTANCE, 4.0f * VEHICLE_SPACING_DISTANCE);
				strcpy_s(NewODF, StartingVehicleList[i]);

				NewODF[0] = TeamRace;
				GetWorldVariantODF(NewODF);

				if((NewODF[0]) && (DoesODFExist(NewODF)))
				{
					VehicleH = BuildObject(NewODF, Team, RandomizedPosition);
					SetRandomHeadingAngle(VehicleH);
					SetBestGroup(VehicleH);
				}
			} // bit is on-- need to build.
		} // loop over all entries.
	}

	// Load/save the read-in list of vehicles out to disk
	void Save(bool MissionSave)
	{
		if (MissionSave)
			return;

		// Why do we do this? So the host can't cheat by tweaking svars after launch.  If the info read in by the server is saved
		// along with all the other data they send to clients on join, then clients should be guaranteed to get the same info.

		Write(&NumStartingVehicleEntries, 1);
		for(int i = 0; i < NumStartingVehicleEntries; i++)
			Write(StartingVehicleList[i], sizeof(StartingVehicleList[i]));
	}

	void Load(bool MissionSave)
	{
		if (MissionSave)
			return;

		Read(&NumStartingVehicleEntries, 1);
		for(int i = 0; i < NumStartingVehicleEntries; i++)
			Read(StartingVehicleList[i], sizeof(StartingVehicleList[i]));
	}
} // 	namespace StartingVehicleManager
