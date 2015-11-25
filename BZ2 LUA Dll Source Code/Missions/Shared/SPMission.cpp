#include "BZ1Helper.h"
#include "SPMission.h"
#include "BZCScriptUtils.h"
#include <math.h>
#include <time.h>

SPMission *mission;

SPMission::SPMission(void)
{
	b_array = NULL;
	b_count = 0;

	f_array = NULL;
	f_count = 0;

	h_array = NULL;
	h_count = 0;

	i_array = NULL;
	i_count = 0;

	// Zero things out first off.
	if(i_array)
		memset(i_array, 0, i_count*sizeof(int));
	if(f_array)
		memset(f_array, 0, f_count*sizeof(float));
	if(h_array)
		memset(h_array, 0, h_count*sizeof(Handle));
	if(b_array)
		memset(b_array, 0, b_count*sizeof(bool));

	/*
	// Preload things here.
	for(int i=97; i<123; ++i)
	{
		PetWatchdogThread();
		PreloadRace(char(i));
		PetWatchdogThread();
	}
	*/
}

void SPMission::AddObject(Handle h)
{
	//and this - GBD
	BZ1Helper::AddObject(h);
}

void SPMission::DeleteObject(Handle h)
{
	//and this -GBD
	BZ1Helper::DeleteObject(h);
}

void SPMission::Execute(void)
{
	//and this -GBD
	BZ1Helper::Execute();
}

bool SPMission::Load(bool missionSave)
{
	//and this -GBD
	BZ1Helper::Load(missionSave);

	if (missionSave) {
		int i;

		// init bools
		for (i = 0; i < b_count; i++)
			b_array[i] = false;

		// init floats
		for (i = 0; i < f_count; i++)
			f_array[i] = 99999.0f;

		// init handles
		for (i = 0; i < h_count; i++)
			h_array[i] = 0;

		// init ints
		for (i = 0; i < i_count; i++)
			i_array[i] = 0;

		Setup();
		return true;
	}

	bool ret = true;

	// bools
	if(b_count)
		ret = ret && Read(b_array, b_count);

	// floats
	if(f_count)
		ret = ret && Read(f_array, f_count);

	// Handles
	if(h_count)
		ret = ret && Read(h_array, h_count);

	// ints
	if(i_count)
		ret = ret && Read(i_array, i_count);

	return ret;
}

bool SPMission::PostLoad(bool missionSave)
{
	//and this -GBD
	BZ1Helper::PostLoad(missionSave);

	if (missionSave)
		return true;

	bool ret = true;

	ConvertHandles(h_array, h_count);

	return ret;
}

bool SPMission::Save(bool missionSave)
{
	//and this -GBD
	BZ1Helper::Save(missionSave);

	if (missionSave)
		return true;

	bool ret = true;

	// bools
	if(b_count)
		ret = ret && Write(b_array, b_count);

	// floats
	if(f_count)
		ret = ret && Write(f_array, f_count);

	// Handles
	if(h_count)
		ret = ret && Write(h_array, h_count);

	// ints
	if(i_count)
		ret = ret && Write(i_array, i_count);

	return ret;
}

void SPMission::PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF)
{
	//and this -GBD
	BZ1Helper::PreOrdnanceHit(shooterHandle, victimHandle, ordnanceTeam, pOrdnanceODF);
}