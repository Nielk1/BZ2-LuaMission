//
// BZ2 Network Manager (MPVehicles) source code
//
// Subsection: Code that reads in the MPVehicles.txt file 
//
// Copyright 1998 Pandemic Studios, LLC
//
// NM 8/14/98

#ifndef MPVehicles_H__
#define MPVehicles_H__

typedef unsigned long U32;

namespace MPVehicleManager 
{
	// How many MP Vehicles we've read in
	extern int NumMPVehicleEntries;

		// Read in and initialize the list of vehicles
	void Init(void);

	// Do any memory cleanups necessary
	void Cleanup(void);
	
	// Load/save the read-in list of vehicles out to disk
	void Save(bool MissionSave);
	void Load(bool MissionSave);

	// Given a bitfield (hopefully only 1 bit on) saying which vehicle
	// is present, return the odf for that slot
	char *GetVehicleODFFromBitfield(unsigned long Bits);

} // 	namespace MPVehicleManager

#endif

