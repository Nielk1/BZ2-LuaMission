//
// BZ2 Network Manager (MPVehicles) .cpp source code
//
// Subsection: Code that reads in the MPVehicles.txt file 
//
// Copyright 1998 Pandemic Studios, LLC
//
// NM 8/14/98

#include <stdio.h>
#include <memory.h>
#include <string.h>
#include <assert.h>
#include "..\Shared\DLLBase.h"
#include "MPVehicles.h"

namespace MPVehicleManager
{
	// Max # of vehicles that can be in the list. Do *NOT* put this over
	// 32 without expanding the # of bits used to send this across the
	// network. ivar4 is used to encode this
	const int MAX_MP_VEHICLES = 32;

	// Max name of an ODF filename
	const U32 MAX_ODFNAMELEN = 64;

	char MPVehicleList[MAX_MP_VEHICLES][MAX_ODFNAMELEN];
	// Size of the above array (i.e. the # of entries with non-blank data
	int NumMPVehicleEntries=0;

	static bool Initialized=false;
	// Read in and initialize the list of vehicles
	void Init(void)
	{
		if(Initialized)
			return;

		// Pre-clear this
		memset(MPVehicleList,0,sizeof(MPVehicleList));

		int i;
		char TempStr[128];
		for(i=0;i<MAX_MP_VEHICLES;i++)
		{
			sprintf_s(TempStr,"network.mpvehicles.entry%d",i);
			const char *Contents=GetVarItemStr(TempStr);
			if(Contents != NULL)
			{
				strncpy(MPVehicleList[i],Contents,MAX_ODFNAMELEN);
				MPVehicleList[i][MAX_ODFNAMELEN-1] = 0; // ensure this is cleared
			}
		}

		// Count entries, stopping when we run into a blank one
		NumMPVehicleEntries = 0;
		for(i=0;i<MAX_MP_VEHICLES;i++)
		{
			if(MPVehicleList[i][0])
				NumMPVehicleEntries++;
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

	// Given a bitfield (hopefully only 1 bit on) saying which vehicle
	// is present, return the odf for that slot
	char *GetVehicleODFFromBitfield(unsigned long Bits)
	{
		// Find first bit set on, and return slot for that
		for(int i=0;i<NumMPVehicleEntries;i++)
		{
			if(Bits & (1<<i))
			{
				return MPVehicleList[i];
			}
		}
		return "player"; // Default; this BETTER be present
	}

	// Load/save the read-in list of vehicles out to disk
	void Save(bool MissionSave)
	{
		if (MissionSave)
			return;

		Write(&NumMPVehicleEntries,1);
		for(int i=0;i<NumMPVehicleEntries;i++)
		{
			Write(MPVehicleList[i],sizeof(MPVehicleList[i]));
		}

	}

	void Load(bool MissionSave)
	{
		if (MissionSave)
			return;

		Read(&NumMPVehicleEntries,1);
		for(int i=0;i<NumMPVehicleEntries;i++)
		{
			Read(MPVehicleList[i],sizeof(MPVehicleList[i]));
		}
	}


} // 	namespace MPVehicleManager
