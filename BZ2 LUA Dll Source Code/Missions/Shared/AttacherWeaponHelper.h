#ifndef _AttacherWeaponHelper_
#define _AttacherWeaponHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class AttacherWeaponHelper
{
private:

	struct AttacherWeaponClass {
		Handle HostObject; // This Attacher Host Object.
		Handle WeaponObject; // This is the object attached to the Host.
		bool IsActive; // Is it active?
	//	bool UseLocalAmmo; // LocalAmmo or Normal Ammo?
		int AmmoCost; // Ammo Cost.
		int LocalWeaponIndex; // Index for if LocalAmmo is used.
	//	char WeaponODF[MAX_ODF_LENGTH]; // Weapon ODF.
	//	bool BuiltWeapon; // Is it first time built?
		int Lifespan; // Attachee lifespan, in Turns.
		Matrix HostPosition; // Save it's position.

		AttacherWeaponClass() { memset(this, 0, sizeof(AttacherWeaponClass)); };
	};
	std::vector<AttacherWeaponClass> AttacherWeaponList;

public:
	AttacherWeaponHelper();

	void AddAttacherWeaponToggleObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team, const char *ODFName);

	void SwapAttacherWeapon(const Handle h);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
