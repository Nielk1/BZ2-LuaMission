#ifndef _LavaDamageHelper_
#define _LavaDamageHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

//#include "ScrapHelper.h"

class BZ1Helper;

class LavaDamageHelper
{
private:

	//static LavaDamageHelper *m_LavaDamageHelper;

	struct LavaFieldClass {
		std::vector<VECTOR_2D> Field; // Path name. Must account 2x bufsize.
		int PointCount; // The size of the std:vector above of path points.
		bool DeleteScrap; // Does it delete Scrap in the area?
		bool DamageOutside; // Delas damage Outside?
		int DamageAmount; // How much Damage.
		float DamageHeight; // How high to deal damage.
		float SpeedMod; // Speed modifier inside.
		DWORD FadeColor; // Damage Fade Color. (SP only)
		char DamageEmitter[MAX_ODF_LENGTH]; // Damage FX ODF for MP.

		LavaFieldClass() { memset(this, 0, sizeof(LavaFieldClass)); };
	};
	std::vector<LavaFieldClass> LavaList;

	/*
		char LavaDamageEmitter[MAX_ODF_LENGTH];
		std::vector<VECTOR_2D> LavaField[MAX_LAVA_FIELDS];
		unsigned long LavaFadeColor;
	*/

public:
	LavaDamageHelper();
//	virtual ~LavaDamageHelper();
//	static LavaDamageHelper &Get() { return *m_LavaDamageHelper; }

	void LoadLavaField(const char* mapTrnFile);

	//bool Load(bool missionSave);
	//bool Save(bool missionSave);

	void Execute(int HandleIndex);
	void Init(BZ1Helper &bz1Helper);
};

#endif
