#ifndef _ShieldHelper_
#define _ShieldHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class ShieldHelper
{
private:

	// Information about a shield wall
	struct ShieldWallClass {
		int Count;// number of active shield segments.
		//int TotalCount; // How many walls have been placed on this shield.
		Handle Fields[MAX_SHIELDS];// field object handle for each shield segment.
		Handle Effects[MAX_SHIELDS];// visual object handle for each shield segment.
		bool IsActive; // Is it Active?
		float Interval;// distance between shield segments.
		float Width;// width of each shield segment.
		Handle Target;// target shield tower for this shield wall.
		//int TargetTapIndex;// target shield tower tap number.
		float TargetWidth; // Width of Target's Shield emitter.
		char ShieldEmitterODF[MAX_ODF_LENGTH]; // ODF for this shieldwall's emitter.
		char ShieldEffectODF[MAX_ODF_LENGTH]; // ODF for this shiedlwall's effect. (a / b are added to end for beginning/end)
		Vector Rotation; // This emitter's Front vector.
		Vector TargetFront; // This shield emitter's Front facing vector.

		ShieldWallClass() { memset(this, 0, sizeof(ShieldWallClass)); };
	};

	// Information about a shield tower
	struct ShieldTowerClass {
		Handle ShieldTowerObject; // This ShieldTower Object.
		float MaxRange; // Max range to search.
		float MatchConeAngle; // Cone angle for line up tolerance.
		int TeamFilter; // Team Filter.
		int EmitterCount; // How many emitter's this shield has.
		ShieldWallClass Walls[MAX_TAPS];// info for each potential wall.

		ShieldTowerClass() { memset(this, 0, sizeof(ShieldTowerClass)); };
	};
	std::vector<ShieldTowerClass> ShieldList;

public:
	ShieldHelper();

	void AddShieldTowerObject(const Handle h, const char* TempODF1, const char* TempODF2);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
