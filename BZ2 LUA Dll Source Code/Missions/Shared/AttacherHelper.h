#ifndef _AttacherHelper_
#define _AttacherHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class AttacherHelper
{
private:

	struct AttacherClass {
		Handle AttacherObject; // This Attacher Object Object.
		Handle TapObject[MAX_TAPS]; // This Attacher's Attached Objects.
		//unsigned int BZCClassHash[MAX_TAPS]; // Each Tap's BZCClasslabel Hash.
		bool TapIndependance[MAX_TAPS]; // Independance setting.
		bool TapIsAttacking[MAX_TAPS]; // Is it attacking currently?
		bool BuiltAttacherTap[MAX_TAPS]; // Has the object been built? (we build objects in execute, not addobject)
		bool TapShutOff[MAX_TAPS]; // Shutoff flag if the host object is empty.
		bool TapCoordinateFire[MAX_TAPS]; // Coordinate fire or try to find seperate Targets?
		int TapDeployBehavior[MAX_TAPS]; // 0 = Always on, 1 = Only active when Deployed, 2 = Only active when undeployed.
		bool IsHostDeployed[MAX_TAPS]; // Bool for playing animation.
		float TapEngageRange[MAX_TAPS]; // Engage Range :D
		Vector TapOffset[MAX_TAPS]; // XYZ Offset for each tap.
		Vector TapRotation[MAX_TAPS]; // XYZ Rotation for each tap.
		int NumberOfTaps; // Save the number of taps.
		bool PDSMode[MAX_TAPS]; // Is this Tap in PDS mode? (Only target Weapons and Persons)
		char TapWeaponSymmetryMask[MAX_TAPS][MAX_HARDPOINTS+1]; // Bitmask weaponmask flag for which weapons from host to symmetry copy over to tap.
		// Special Cerberus functions.
		bool IsCerberus; // Is this host a Cerberus class ship?
		bool IsCerberusTail[MAX_TAPS]; // Flag for if this tap is a Cerberus Tail.
		float CerbTailConeAngle[MAX_TAPS]; // Cone Angle for firing the Tail.
		bool IsCerberusTailAnimated[MAX_TAPS]; // Flag for if the Cerberus unit has the 3 Tail animations.
		int CerbTailSwinging[MAX_TAPS]; // Is it swinging? 0 = No, 1 = Left, 2 = Right.
		float CerbAnimLength[MAX_TAPS]; // Current Animation Length;
		Matrix TapPosition[MAX_TAPS]; // It's last known Position Matrix.

		AttacherClass() { memset(this, 0, sizeof(AttacherClass)); };
	};
	std::vector<AttacherClass> AttacherList;

public:
	AttacherHelper();

	void AddAttacherObject(const Handle h, const char* TempODF1, const char* TempODF2, const int BZCClassHash);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
