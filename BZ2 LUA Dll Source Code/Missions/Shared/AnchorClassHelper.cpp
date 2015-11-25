#include "BZ1Helper.h"
#include "AnchorClassHelper.h"
#include "BZCScriptUtils.h"
#include "math.h"
#include <algorithm>

AnchorClassHelper::AnchorClassHelper()
{
}
AnchorClassHelper::~AnchorClassHelper()
{
}

bool AnchorClassHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		AnchorList.resize(size);
		if(size)
			Read(&AnchorList.front(), sizeof(AnchorClass)*size);
	}
	return true;
}

bool AnchorClassHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<AnchorClass>::iterator iter = AnchorList.begin(); iter != AnchorList.end(); ++iter)
		{
			AnchorClass &sao = *iter;
			ConvertHandles(&sao.AnchorObject, 1);
		}
	}
	return true;
}

bool AnchorClassHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(AnchorList.size());
		Write(&size, 1);
		if(size)
			Write(&AnchorList.front(), sizeof(AnchorClass)*size);
	}
	return true;
}

void AnchorClassHelper::PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF, char *TempODF1, char *TempODF2)
{
	char ObjClass[MAX_ODF_LENGTH] = {0};
	if(!GetObjInfo(victimHandle, Get_GOClass, ObjClass))
		return; // Invalid handle, bail.

	unsigned int ObjClassHash = Hash(ObjClass);

	int TempTeamFilter = 0;
	int Team = GetTeamNum(victimHandle);
	int ShooterTeam = ordnanceTeam;
	if(!ShooterTeam)
		ShooterTeam = GetTeamNum(shooterHandle);

	GetODFInt(TempODF1, TempODF2, "AnchorRocketClass", "TeamFilter", &TempTeamFilter, 4);
	// Make 0 all teams. 
	if(TempTeamFilter == 0)
		TempTeamFilter = 7;

	if( ( // Everything that Moves.
		IsVehicle(victimHandle) || IsPowerup(victimHandle) || IsPerson(victimHandle) || 
		(ObjClassHash == CLASS_TORPEDO) || (ObjClassHash == CLASS_SEEKER) || 
		(ObjClassHash == CLASS_ARTIFACT) || (ObjClassHash == CLASS_BOID) || (ObjClassHash == CLASS_FLAG) ) && 
		(TeamFilterCheck(TempTeamFilter, ShooterTeam, Team))) // Does Teamfilter pass?
	{
		float TempAnchorTime = 0;

		AnchorList.resize(AnchorList.size()+1);
		AnchorClass &sao = AnchorList[AnchorList.size()-1]; // No temporary being created, *much* faster
		memset(&sao, 0, sizeof(AnchorClass)); // Zero things out at start.
		
		GetODFFloat(TempODF1, TempODF2, "LeaderRoundClass", "stickTime", &TempAnchorTime, 0.0f);
		GetODFFloat(TempODF1, TempODF2, "AnchorRocketClass", "accelDrag", &sao.DragAmount, 1.0f);

		sao.AnchorObject = victimHandle;
		sao.DragTime = ConvertToTurns(TempAnchorTime);
	}
}

void AnchorClassHelper::Execute(BZ1Helper &bz1Helper)
{
	// pre-clean the vector
	AnchorList.erase(std::remove_if(AnchorList.begin(), AnchorList.end(), ShouldRemoveAnchorObject), AnchorList.end());

	for (std::vector<AnchorClass>::iterator iter = AnchorList.begin(); iter != AnchorList.end(); ++iter)
	{
		if(iter->DragTime > 0)
		{
			//* Fake Anchor Class
			if(iter->DragAmount > 0.0f)
			{
				// drag on object's velocity
				Vector v = GetVelocity(iter->AnchorObject);
				float speed = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
				float drag = iter->DragAmount * ConvertToSeconds(1.0f); //dt;
				if(speed > drag)
				{
					float scale = (1.0f - drag / speed);
					v = Vector_Scale(v, scale);
				}
				else
				{
					v = Vector(0.0f, 0.0f, 0.0f);
				}
				SetVelocity(iter->AnchorObject, v);
			}
			--iter->DragTime;
		}
	}
}
