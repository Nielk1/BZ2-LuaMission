#include "SubMission.h"

bool SubMission::Load(bool missionSave)
{
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
		Read(b_array, b_count);

	// floats
	if(f_count)
		Read(f_array, f_count);

	// Handles
	if(h_count)
		Read(h_array, h_count);

	// ints
	if(i_count)
		Read(i_array, i_count);

	return ret;
}

bool SubMission::PostLoad(bool missionSave)
{
	if (missionSave)
		return true;

	bool ret = true;

	ConvertHandles(h_array, h_count);

	return ret;
}

bool SubMission::Save(bool missionSave)
{
	if (missionSave)
		return true;

	bool ret = true;

	// bools
	if(b_count)
		Write(b_array, b_count);

	// floats
	if(f_count)
		Write(f_array, f_count);

	// Handles
	if(h_count)
		Write(h_array, h_count);

	// ints
	if(i_count)
		Write(i_array, i_count);

	return ret;
}
