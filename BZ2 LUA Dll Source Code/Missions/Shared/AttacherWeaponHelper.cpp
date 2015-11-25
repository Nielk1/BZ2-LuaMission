#include "BZ1Helper.h"
#include "AttacherWeaponHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

AttacherWeaponHelper::AttacherWeaponHelper()
{
}

bool AttacherWeaponHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		AttacherWeaponList.resize(size);
		if(size)
			Read(&AttacherWeaponList.front(), sizeof(AttacherWeaponClass)*size);
	}
	return true;
}

bool AttacherWeaponHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<AttacherWeaponClass>::iterator iter = AttacherWeaponList.begin(); iter != AttacherWeaponList.end(); ++iter)
		{
			AttacherWeaponClass &sao = *iter;
			ConvertHandles(&sao.HostObject, 1);
			ConvertHandles(&sao.WeaponObject, 1);
		}
	}
	return true;
}

bool AttacherWeaponHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(AttacherWeaponList.size());
		Write(&size, 1);
		if(size)
			Write(&AttacherWeaponList.front(), sizeof(AttacherWeaponClass)*size);
	}
	return true;
}

void AttacherWeaponHelper::AddAttacherWeaponToggleObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team, const char *ODFName)
{
	bool Logging = BZ1Helper::Logging; //true;

	Handle TempHost = GetNearestVehicle(h); //0;
	float DistanceSquared = GetDistance3DSquared(GetPosition(h), GetPosition(TempHost));
	bool AddWeapon = true;

	/*
	float BestDistanceSquared = FLT_MAX;
	bool SecondBreak = false;
	for(int a = 0; a < BZ1Helper::Get().GetNumHandles(); a++)
	{
		BZ1Helper::ObjectClass objectInfo = BZ1Helper::Get().GetHandleInfo(a);
		Handle TempH = objectInfo.ObjectHandle;
		float DistanceSquared = GetDistance3DSquared(GetPosition(h), objectInfo.Position);

		if((BestDistanceSquared > DistanceSquared) && (h != TempH) && (objectInfo.CategoryID == Vehicle) && (GetMaxAmmo(TempH) > 0))
		{
			BestDistanceSquared = DistanceSquared;

			*/
			if((DistanceSquared < Squared(10.0f)) && (GetMaxAmmo(TempHost) > 0)) //if(BestDistanceSquared < Squared(10.0f))
			{
				//TempHost = TempH;
				for (std::vector<AttacherWeaponClass>::iterator iter = AttacherWeaponList.begin(); iter != AttacherWeaponList.end(); ++iter)
				{
					// This host is our close host possible object, if it's already on, switch it off.
					if(TempHost == iter->HostObject) //TempH == 
					{
						if(iter->IsActive) // This is already active, deactivate it.
						{
							if(Logging)
								FormatLogMessage("Found an Active one! Deactivating");

							iter->IsActive = false;

							SwapAttacherWeapon(iter->HostObject);

							if((iter->WeaponObject != h) && (IsAround(iter->WeaponObject)))
								RemoveObject(iter->WeaponObject);
						}
						AddWeapon = false;
						//SecondBreak = true;
						break;
					}
				}
			}
			/*
			if(SecondBreak)
				break;
		}
	}
	*/

	char TempODFCompare[MAX_ODF_LENGTH] = {0};
	char BuildODF[MAX_ODF_LENGTH] = {0};
	int TempLocalAmmo = 0;
	int TempMaxAmmo = 0;
	bool LocalAmmo = false;
	int TempWeaponSlot = -1;

	for(int w = 0; w < MAX_HARDPOINTS; w++)
	{
		char TempHostWeapon[MAX_ODF_LENGTH] = {0};
		if(GetObjInfo(TempHost, ObjectInfoType(Get_Weapon0ODF+w), TempHostWeapon))
		{
			if(OpenODF2(TempHostWeapon))
			{
				if(GetODFString(TempHostWeapon, "DispenserClass", "objectClass", MAX_ODF_LENGTH, TempODFCompare)) // Grab the value.
				{
					// Got it.
				}

				if(GetODFInt(TempHostWeapon, "WeaponClass", "localAmmo", &TempLocalAmmo)) // Grab the value.
					LocalAmmo = true;

				if((LocalAmmo) && (ODFName) && (_stricmp(TempODFCompare, ODFName) == 0))
				{
					TempWeaponSlot = w;
					break;
				}
			}
		}
	}

	// Get the variables we need from this toggle object.
	GetODFString(TempODF1, TempODF2, "AttacherWeaponClass", "DeployType", MAX_ODF_LENGTH, BuildODF);
	// If maxammo > 0, add to make up for the bz2 engine ammo cost. It's usually a dispensed object, so it will use MaxAmmo for it's in-game ammo cost.
	GetODFInt(TempODF1, TempODF2, "GameObjectClass", "maxAmmo", &TempMaxAmmo);

	if(TempMaxAmmo > 0)
	{
		if(LocalAmmo)
			AddLocalAmmo(TempHost, float(TempMaxAmmo), TempWeaponSlot);
		else
			AddAmmo(TempHost, TempMaxAmmo);
	}

	if(BuildODF[0])
	{
		if(DoesODFExist(BuildODF))
		{
			if((AddWeapon) && (IsAround(TempHost)))
			{
				AttacherWeaponList.resize(AttacherWeaponList.size()+1);
				AttacherWeaponClass &sao = AttacherWeaponList[AttacherWeaponList.size()-1]; // No temporary being created, *much* faster
				memset(&sao, 0, sizeof(AttacherWeaponClass)); // Zero things out at start.

				int TempAmmo = 0;
				float TempLifeSpan = 0.0f;
				GetODFInt(TempODF1, TempODF2, "AttacherWeaponClass", "ammoCost", &TempAmmo);
				GetODFFloat(TempODF1, TempODF2, "AttacherWeaponClass", "lifeSpan", &TempLifeSpan);

				SwapAttacherWeapon(TempHost);

				sao.HostObject = TempHost;
				//strcpy_s(sao.WeaponODF, sizeof(BuildODF), BuildODF);
				sao.WeaponObject = BuildObject(BuildODF, Team, TempHost);
				sao.IsActive = true;
				sao.AmmoCost = (TempAmmo / BZ1Helper::m_GameTPS);
				sao.Lifespan = ConvertToTurns(TempLifeSpan);
				//sao.UseLocalAmmo = LocalAmmo;
				sao.LocalWeaponIndex = TempWeaponSlot;

				if(Logging)
					FormatLogMessage("Weapon Not active, building! Host: %d, ODF: %s, AmmoCost: %f, WeaponIndex: %d", sao.HostObject, BuildODF, sao.AmmoCost, sao.LocalWeaponIndex);
			}
		}
		else
		{
			FormatLogMessage("ERROR: ODF %s Doesn't exist! Fix ODFs!", BuildODF);
		}
	}
	// Que this object for deletion.
	BZ1Helper::Get().DeleteAfterDelay(h);
}

void AttacherWeaponHelper::Execute(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging; //true;

	// Sweep through the list and remove any of them that are no longer around
	std::vector<AttacherWeaponClass>::iterator validEnd = AttacherWeaponList.begin();
	std::vector<AttacherWeaponClass>::iterator lastValid = AttacherWeaponList.begin();
	for (std::vector<AttacherWeaponClass>::iterator iter = AttacherWeaponList.begin(); iter != AttacherWeaponList.end(); ++iter)
	{
		// If the host or weapon is around...
		if ((IsAround(iter->HostObject)) && (IsAround(iter->WeaponObject))) //((!iter->BuiltWeapon) || (IsAround(iter->WeaponObject))))
		{
			// Copy the valid entry into the valid portion
			if (lastValid != iter)
				*validEnd = *iter;
			++validEnd;
		}
		else
		{
			if(IsAround(iter->WeaponObject)) // Cleanup weapon objects if their host is no longer around.
				RemoveObject(iter->WeaponObject);
		}
	}
	// Now erase any invalid entries left behind
	AttacherWeaponList.erase(validEnd, AttacherWeaponList.end());

	for (std::vector<AttacherWeaponClass>::iterator iter = AttacherWeaponList.begin(); iter != AttacherWeaponList.end(); ++iter)
	{
		if(iter->IsActive)
		{
			/*
			if(!IsAround(iter->WeaponObject))
			{
				iter->WeaponObject = BuildObject(iter->WeaponODF, GetTeamNum(iter->HostObject), iter->HostObject);
				iter->BuiltWeapon = true;
			}
			*/

			Vector HostVelocity = GetVelocity(iter->HostObject);

			Matrix CurrPosition = GetMatrixPosition(iter->HostObject);

			if((BZ1Helper::m_GameVersion >= v13PB7) && ((!IsVehicle(iter->WeaponObject)) && (!IsPerson(iter->WeaponObject))))
				SetInterpolablePosition(iter->WeaponObject, &iter->HostPosition, &CurrPosition);
			else
				SetLastCurrentPosition(iter->WeaponObject, iter->HostPosition, CurrPosition);

		//	SetPosition(iter->WeaponObject, CurrPosition);
		//	SetVectorPosition(iter->WeaponObject, CurrPosition.posit);
			SetVelocity(iter->WeaponObject, HostVelocity);

			iter->HostPosition = CurrPosition;

			if(iter->LocalWeaponIndex >= 0)
			{
				AddLocalAmmo(iter->HostObject, -float(iter->AmmoCost), iter->LocalWeaponIndex);
				if(GetCurLocalAmmo(iter->HostObject, iter->LocalWeaponIndex) <= 0)
				{
					iter->IsActive = false;
				}
			}
			else
			{
				AddAmmo(iter->HostObject, -iter->AmmoCost);
				if(GetCurAmmo(iter->HostObject) <= 0)
					iter->IsActive = false;
			}

			if(iter->Lifespan > 0)
			{
				--iter->Lifespan;

				if(iter->Lifespan <= 0)
					iter->IsActive = false;
			}

			// Switch Weapon to altname on Deactivation.
			if(!iter->IsActive)
			{
				if(Logging)
					FormatLogMessage("Weapon Deactivated by AmmoLoss, Switching Weapons Back");

				SwapAttacherWeapon(iter->HostObject);

				if(IsAround(iter->WeaponObject))
					RemoveObject(iter->WeaponObject);
			}
		}
	//	else if(IsAround(iter->WeaponObject))
	//	{
	//		RemoveObject(iter->WeaponObject);
	//	}
	}
}

void AttacherWeaponHelper::SwapAttacherWeapon(const Handle h)
{
	if(!IsAround(h))
		return;

	char NewWeapon[MAX_HARDPOINTS][MAX_ODF_LENGTH] = {0};
	float AltWeaponLocalAmmo[MAX_HARDPOINTS] = {0};

	for(int x = 0; x < MAX_HARDPOINTS; x++)
	{
		GetObjInfo(h, ObjectInfoType(Get_Weapon0Config+x), NewWeapon[x]);

		AltWeaponLocalAmmo[x] = GetCurLocalAmmo(h, x);
		char WeaponODF[MAX_ODF_LENGTH] = {0};
		char MineODF[MAX_ODF_LENGTH] = {0};
		GetObjInfo(h, ObjectInfoType(Get_Weapon0ODF+x), WeaponODF);

		if((OpenODF2(WeaponODF)) && (GetODFString(WeaponODF, "DispenserClass", "objectClass", MAX_ODF_LENGTH, MineODF))) // Grab the value.
		{
			char TempBZCClass[MAX_ODF_LENGTH] = {0};
			strcat_s(MineODF, ".odf"); // Add .odf to the end.
			if( (OpenODF2(MineODF)) && (GetODFString(MineODF, "GameObjectClass", "BZCClasslabel", MAX_ODF_LENGTH, TempBZCClass)) && // Grab the value.
				(Hash(TempBZCClass) == BZCCLASS_ATTACHERWEAPON) && // It's the right kind of thing.
				(GetODFString(WeaponODF, "WeaponClass", "altName", MAX_ODF_LENGTH, NewWeapon[x]))) // Grab the value.
				break; // Got it.
		}
	}

	ReplaceWeapons(h, NewWeapon, AltWeaponLocalAmmo);
}