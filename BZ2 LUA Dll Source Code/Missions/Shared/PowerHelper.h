#ifndef _PowerHelper_
#define _PowerHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class PowerHelper
{
public:

	struct PowerClass {
		Handle PowerObject; // This Power Object.
		bool WasDemolished; // Was it demolished?
		bool DoBettyLost; // Voice over for Lost?
		bool IsPowered; // Is this currently powered? (if it needs power. Power is power, so does power power?
		int PowerTeam; // What team was this on?
		float PowerRange; // How far it supplies power.
		int PowerMask; // Bitmask for what bits to look at.
		int PowerMatch; // Bitmask for what the bits need to be.
		int PowerProvide; // Bitmask for what kind of power this provides.
		Vector Position; // It's current position.

		PowerClass() { memset(this, 0, sizeof(PowerClass)); };
	};

private:

	static PowerHelper *m_PowerHelper;
	std::vector<PowerClass> PowerList;
	//static bool ShouldRemovePower(const PowerClass &entry) { return !IsAround(entry.PowerObject); }

public:
	PowerHelper();
	~PowerHelper();
	static PowerHelper &Get() { return *m_PowerHelper; }

	void AddPowerObject(const Handle h, const char* TempODF1, const char* TempODF2);
	void DeletePowerObject(const Handle h);

	int GetNumPowerObjects() { return PowerList.size(); }
	const PowerClass &GetPowerInfo(int index) const { return PowerList[index]; } // Read only version.
	/*
	Handle &RefPowerHandle(const int index) { return PowerList[index].PowerObject; }
	float GetPowerRadius(const int index) { return PowerList[index].PowerRange; }
	int GetPowerMask(const int index) { return PowerList[index].PowerMask; }
	int GetPowerMatch(const int index) { return PowerList[index].PowerMatch; }
	int GetPowerProvide(const int index) { return PowerList[index].PowerProvide; }
	*/

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif

