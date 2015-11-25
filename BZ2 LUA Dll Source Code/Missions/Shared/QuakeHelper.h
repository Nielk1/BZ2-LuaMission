#ifndef _QuakeHelper_
#define _QuakeHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class QuakeHelper
{
private:

	float f_first,
		m_QuakeDecay, // Quake Decay, this is global across world.
		m_QuakeMagnitude, // Magnitude. :)
		m_MinQuakeMagnitude, // Minimum Quake Magnitude. Used by DLLStartEarthQuake(), DLLUpdateEarthQuake(), and DLLStopEarthQuake().
		f_last;

	float *f_array;
	int f_count;

public:
	QuakeHelper();

	void AddQuakeObject(const Handle h, const char* TempODF1, const char* TempODF2);
	void DeleteQuakeObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team);

	bool Load(bool missionSave);
	//bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);

	void DLLStartEarthQuake(const float Magnitude);
	void DLLUpdateEarthQuake(const float Magnitude);
	void DLLStopEarthQuake(void);
};

#endif
