//
// BZ2 Network Manager (StartingVehicles) source code
//
// Subsection: Code that reads in the StartingVehicles.txt file 
//
// Copyright 1998 Pandemic Studios, LLC
//
// NM 8/14/98

#ifndef _StartingVehicles_H_
#define _StartingVehicles_H_

typedef unsigned long U32;

namespace StartingVehicleManager
{
	// Read in and initialize the list of vehicles, from the specified
	// filename.
	void Init(void);

	// Do any memory cleanups necessary
	void Cleanup(void);

	// Given all the specifications for the team, create things for that team
	void CreateVehicles(const int Team, const char TeamRace, const int Bitmask, const Vector Where);

	// Load/save the read-in list of vehicles out to disk
	void Save(bool MissionSave);
	void Load(bool MissionSave);

} // 	namespace StartingVehicleManager

#endif

