#include "BZ1Helper.h"
#include "AmmoDrainHelper.h"
#include "BZCScriptUtils.h"
#include "math.h"
#include <algorithm>

AmmoDrainHelper::AmmoDrainHelper()
{
}
AmmoDrainHelper::~AmmoDrainHelper()
{
}

bool AmmoDrainHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		AmmoDrainList.resize(size);
		if(size)
			Read(&AmmoDrainList.front(), sizeof(AmmoDrainClass)*size);
	}
	return true;
}

bool AmmoDrainHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<AmmoDrainClass>::iterator iter = AmmoDrainList.begin(); iter != AmmoDrainList.end(); ++iter)
		{
			AmmoDrainClass &sao = *iter;
			ConvertHandles(&sao.AmmoDrainObject, 1);
			ConvertHandles(&sao.AmmoDrainTarget, 1);
		}
	}
	return true;
}

bool AmmoDrainHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(AmmoDrainList.size());
		Write(&size, 1);
		if(size)
			Write(&AmmoDrainList.front(), sizeof(AmmoDrainClass)*size);
	}
	return true;
}

void AmmoDrainHelper::PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF, char *TempODF1, char *TempODF2)
{
	int TempTeamFilter = 0;
	int Team = GetTeamNum(victimHandle);
	int ShooterTeam = ordnanceTeam;
	if(!ShooterTeam)
		ShooterTeam = GetTeamNum(shooterHandle);

	GetODFInt(TempODF1, TempODF2, "VampireClass", "TeamFilter", &TempTeamFilter, 4);
	// Make 0 all teams. 
	if(TempTeamFilter == 0)
		TempTeamFilter = 7;

	if(((GetMaxAmmo(victimHandle) > 0) || 
		(GetMaxHealth(victimHandle) > 0) || 
		(IsSilo(victimHandle)) || 
		(IsScavenger(victimHandle)) || 
		(IsRecycler(victimHandle)) ) && 
		(TeamFilterCheck(TempTeamFilter, ShooterTeam, Team)) && (!IsGunTower(victimHandle)))
	{
		float TempAmmoDrainTime = 0.0f;

		AmmoDrainList.resize(AmmoDrainList.size()+1);
		AmmoDrainClass &sao = AmmoDrainList[AmmoDrainList.size()-1]; // No temporary being created, *much* faster
		memset(&sao, 0, sizeof(AmmoDrainClass)); // Zero things out at start.
		
		GetODFFloat(TempODF1, TempODF2, "LeaderRoundClass", "stickTime", &TempAmmoDrainTime, 0.0f);
		GetODFFloat(TempODF1, TempODF2, "VampireClass", "AmmoDrainAmount", &sao.AmmoDrainAmount, 0);
		GetODFFloat(TempODF1, TempODF2, "VampireClass", "HealthDrainAmount", &sao.HealthDrainAmount, 0);
		GetODFFloat(TempODF1, TempODF2, "VampireClass", "ScrapDrainAmount", &sao.ScrapDrainAmount, 0);

		sao.AmmoDrainObject = victimHandle;
		sao.AmmoDrainTarget = shooterHandle;
		sao.DrainTime = ConvertToTurns(TempAmmoDrainTime);
		sao.AmmoDrainAmount = ConvertToSeconds(sao.AmmoDrainAmount);
		sao.HealthDrainAmount = ConvertToSeconds(sao.HealthDrainAmount);
		sao.ScrapDrainAmount = ConvertToSeconds(sao.ScrapDrainAmount);
	}
}

void AmmoDrainHelper::Execute(BZ1Helper &bz1Helper)
{
	// pre-clean the vector
	AmmoDrainList.erase(std::remove_if(AmmoDrainList.begin(), AmmoDrainList.end(), ShouldRemoveAmmoDrain), AmmoDrainList.end());

	for (std::vector<AmmoDrainClass>::iterator iter = AmmoDrainList.begin(); iter != AmmoDrainList.end(); ++iter)
	{
		if(iter->DrainTime > 0)
		{
			if(iter->AmmoDrainAmount > 0)
			{
				long TargetCurAmmo = GetCurAmmo(iter->AmmoDrainObject);

				// If this object still has any ammo left.
				if(TargetCurAmmo > 0)
				{	
					iter->AmmoDrainRemainder += iter->AmmoDrainAmount;
					int TempDrainAmmo = int(iter->AmmoDrainRemainder);
					iter->AmmoDrainRemainder -= TempDrainAmmo;

					AddAmmo(iter->AmmoDrainTarget, TempDrainAmmo);
					AddAmmo(iter->AmmoDrainObject, -TempDrainAmmo);
				}
			}
			if(iter->HealthDrainAmount > 0)
			{
				long TargetCurHealth = GetCurHealth(iter->AmmoDrainObject);

				// If this object still has any health left.
				if(TargetCurHealth > 0)
				{	
					iter->HealthDrainRemainder += iter->HealthDrainAmount;
					int TempDrainHealth = int(iter->HealthDrainRemainder);
					iter->HealthDrainRemainder -= TempDrainHealth;

					AddHealth(iter->AmmoDrainTarget, TempDrainHealth);
					AddHealth(iter->AmmoDrainObject, -TempDrainHealth);
				}
			}
			if((iter->ScrapDrainAmount > 0) && ((IsSilo(iter->AmmoDrainObject)) || (IsScavenger(iter->AmmoDrainObject)) || (IsRecycler(iter->AmmoDrainObject))))
			{
				int TargetTeam = GetTeamNum(iter->AmmoDrainObject);

				int TargetCurScrap = 0;
				//int ShooterCurScrap = GetScavengerCurScrap(iter->AmmoDrainTarget);

				if(IsScavenger(iter->AmmoDrainObject))
					TargetCurScrap = GetScavengerCurScrap(iter->AmmoDrainObject);
				else
					TargetCurScrap = GetScrap(TargetTeam);

				// If this object still has any ammo left.
				if(TargetCurScrap > 0)
				{
					int ShooterTeam = GetTeamNum(iter->AmmoDrainTarget);

					iter->ScrapDrainRemainder += iter->ScrapDrainAmount;
					int TempDrainScrap = int(iter->ScrapDrainRemainder);
					iter->ScrapDrainRemainder -= TempDrainScrap;

					if((IsScavenger(iter->AmmoDrainTarget)) && ((GetScavengerCurScrap(iter->AmmoDrainTarget) + TempDrainScrap) < GetScavengerMaxScrap(iter->AmmoDrainTarget)))
						AddScavengerCurScrap(iter->AmmoDrainTarget, TempDrainScrap);
					else
						AddScrap(ShooterTeam, TempDrainScrap);

					if(IsScavenger(iter->AmmoDrainObject))
						AddScavengerCurScrap(iter->AmmoDrainObject, -TempDrainScrap);
					else
						AddScrap(TargetTeam, -TempDrainScrap);
				}
			}
			--iter->DrainTime;
		}
	}
}
