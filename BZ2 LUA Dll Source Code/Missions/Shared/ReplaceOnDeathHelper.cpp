#include "BZ1Helper.h"
#include "ReplaceOnDeathHelper.h"
#include "BuildingHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

ReplaceOnDeathHelper::ReplaceOnDeathHelper()
{
}
ReplaceOnDeathHelper::~ReplaceOnDeathHelper()
{
	 //m_BZ1Helper = NULL;
}

bool ReplaceOnDeathHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		ReplaceOnDeathList.resize(size);
		if(size)
			Read(&ReplaceOnDeathList.front(), sizeof(ReplaceOnDeathClass)*size);
	}
	return true;
}

bool ReplaceOnDeathHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<ReplaceOnDeathClass>::iterator iter = ReplaceOnDeathList.begin(); iter != ReplaceOnDeathList.end(); ++iter)
		{
			ReplaceOnDeathClass &sao = *iter;
			ConvertHandles(&sao.ReplaceObject, 1);
			ConvertHandles(&sao.ReplaceHostObject, 1);
		}
	}
	return true;
}

bool ReplaceOnDeathHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(ReplaceOnDeathList.size());
		Write(&size, 1);
		if(size)
			Write(&ReplaceOnDeathList.front(), sizeof(ReplaceOnDeathClass)*size);
	}
	return true;
}

void ReplaceOnDeathHelper::AddReplaceOnDeathObject(const Handle h, const char *ReplaceODF, const int Team, const float HeightOffset, const bool DoExplode, const bool StaysTap, const float MinReplaceTime, const float MaxReplaceTime, const bool RandomizeTime)
{
	for (std::vector<ReplaceOnDeathClass>::iterator iter = ReplaceOnDeathList.begin(); iter != ReplaceOnDeathList.end(); ++iter)
	{
		if(iter->ReplaceObject == h)
			return;
	}

	char TempODF1[MAX_ODF_LENGTH] = {0};
	char TempODF2[MAX_ODF_LENGTH] = {0};
	GetAndOpenODFNames(h, TempODF1, TempODF2);

	char TestODF[MAX_ODF_LENGTH] = {0};
	if(!DoesODFExist(ReplaceODF))
		GetODFString(TempODF1, TempODF2, "GameObjectClass", "ReplaceOnDeath", MAX_ODF_LENGTH, TestODF, ReplaceODF);
	else
		strcpy_s(TestODF, ReplaceODF);

	GetWorldVariantODF(TestODF);

	// Add it if so. 
	if((TestODF[0]) && (DoesODFExist(TestODF)))
	{
		bool OnlyIfShot = false;
		GetODFBool(TempODF1, TempODF2, "GameObjectClass", "ReplaceOnlyIfShot", &OnlyIfShot, false);
		if((!OnlyIfShot) && (_stricmp(TestODF, GetODFName(h)) == 0))
		{
			FormatConsoleMessage("ERROR: ODF %s, ReplaceOnDeath %s attempted to replace itself, preventing infinite loop.", GetODFName(h), TestODF);
			return;
		}

		ReplaceOnDeathList.resize(ReplaceOnDeathList.size()+1);
		ReplaceOnDeathClass &sao = ReplaceOnDeathList[ReplaceOnDeathList.size()-1]; // No temporary being created, *much* faster
		memset(&sao, 0, sizeof(ReplaceOnDeathClass)); // Zero things out at start.

		strcpy_s(sao.ReplaceODF, TestODF);

		if(Team >= 0)
			sao.ReplaceTeam = Team;
		else
			GetODFInt(TempODF1, TempODF2, "GameObjectClass", "ReplaceTeam", &sao.ReplaceTeam, GetTeamNum(h));

		sao.ReplaceTeam = clamp(sao.ReplaceTeam, 0, 15);
		GetODFFloat(TempODF1, TempODF2, "GameObjectClass", "ReplaceHeight", &sao.ReplaceHeight, HeightOffset);
		GetODFBool(TempODF1, TempODF2, "GameObjectClass", "ReplaceExplode", &sao.DoExplode, DoExplode);
		GetODFBool(TempODF1, TempODF2, "GameObjectClass", "ReplaceStaysTap", &sao.StayTap, StaysTap);
		//GetODFBool(TempODF1, TempODF2, "GameObjectClass", "ReplaceOnlyIfShot", &sao.ReplaceOnlyIfShot, false);
		sao.ReplaceOnlyIfShot = OnlyIfShot;
		GetODFInt(TempODF1, TempODF2, "GameObjectClass", "ReplaceCount", &sao.ReplaceCount, 1);
		sao.ReplaceCount = clamp(sao.ReplaceCount, 0, 100);
		GetODFBool(TempODF1, TempODF2, "GameObjectClass", "ReplaceRandomHeading", &sao.RandomHeading, false);

		if((sao.StayTap) && (!IsAround(sao.ReplaceHostObject)))
		{
			bool Foundit = false;
			for(int i = 0; i < BuildingHelper::Get().GetNumBuildingObjects(); i++)
			{
				Handle Building = BuildingHelper::Get().GetBuildingInfo(i).BuildingObject;
				for(int x = 0; x < MAX_TAPS; x++)
				{
					if(h == GetTap(Building, x))
					{
						// We found one. 
						sao.ReplaceHostObject = Building;
						sao.TapIndex = x;
						Foundit = true;
						break;
					}
				}
				if(Foundit)
					break;
			}
			if(!Foundit)
				sao.StayTap = false;
		}

		float TempMinReplaceTime = 0.0f;
		float TempMaxReplaceTime = 0.0f;
		bool TempRandomizeTime = false;
		GetODFFloat(TempODF1, TempODF2, "GameObjectClass", "ReplaceMinTime", &TempMinReplaceTime, MinReplaceTime);
		GetODFFloat(TempODF1, TempODF2, "GameObjectClass", "ReplaceMaxTime", &TempMaxReplaceTime, MaxReplaceTime);
		if(TempMaxReplaceTime < TempMinReplaceTime)
			TempMaxReplaceTime = TempMinReplaceTime;
		GetODFBool(TempODF1, TempODF2, "GameObjectClass", "ReplaceRandomizeTime", &TempRandomizeTime, RandomizeTime);

		if((TempRandomizeTime) && (TempMaxReplaceTime > 0))
			sao.ReplaceTime = ConvertToTurns(GetRandomFloat(TempMinReplaceTime, TempMaxReplaceTime));
		else
			sao.ReplaceTime = ConvertToTurns(TempMinReplaceTime);

		sao.ReplaceObject = h; // Save it.
		sao.CurTeam = Team;
		GetPosition(h, sao.Position); // Save it's initial position.
	}
}

void ReplaceOnDeathHelper::DeleteReplaceOnDeathObject(const Handle h)
{
	for (std::vector<ReplaceOnDeathClass>::iterator iter = ReplaceOnDeathList.begin(); iter != ReplaceOnDeathList.end(); ++iter)
	{
		if(iter->ReplaceObject == h)
		{
			if(IsAround(iter->ReplaceObject))
				iter->DidDemolish = true;

			// Update the position portion of the matrix to be as accurate as possible.
			//iter->Position.posit = GetPosition2(h);
			break;
		}
	}
}

void ReplaceOnDeathHelper::Execute(BZ1Helper &bz1Helper)
{
//	bool Logging = BZ1Helper::Logging;

	// Create a Temporary for building the replaced objects, so we don't invalidate our iterator by building a ReplaceObject while looping over our ReplaceObjects.
	struct TempBuildClass {
		Matrix Position; // Position of demise.
		int ReplaceTeam; // Team to put the replaced object on?
		char ReplaceODF[MAX_ODF_LENGTH]; // What to replace this object with.
		bool StayTap;
		int TapIndex;
		Handle HostObject;
		bool WasPlayer;
		int CurTeam;
		int ReplaceCount;
		bool RandomHeading;

		TempBuildClass() { memset(this, 0, sizeof(TempBuildClass)); };

		/*
		TempBuildClass(const Matrix &position = Identity_Matrix, const int replaceTeam = 0, const char *replaceODF = NULL, const bool stayTap = false, const int tapIndex = -1, const Handle hostObject = 0)
		: Position(position)
		, ReplaceTeam(replaceTeam)
		, StayTap(stayTap)
		, TapIndex(tapIndex)
		, HostObject(hostObject)
		{
		   strcpy_s(ReplaceODF, replaceODF);
		}
		[12:39:07 PM] Kenneth Miller: instead of doing it with initializers, you could set them all with assignments
		[12:40:04 PM] Kenneth Miller: TempBuildClass( all that stuff )
		{
		   Position = position;
		   ReplaceTeam = replaceTeam;
		   strcpy_s(ReplaceODF, replaceODF);
		   StayTap = stayTap;
		   TapIndex = tapIndex;
		   HostObject = hostObject;
		}
		*/
	};
	std::vector<TempBuildClass> TempBuildList;

	// Sweep through the ReplaceOnDeath Objects and remove any of them that are no longer around
	std::vector<ReplaceOnDeathClass>::iterator validEnd = ReplaceOnDeathList.begin();
	std::vector<ReplaceOnDeathClass>::iterator lastValid = ReplaceOnDeathList.begin();
	for (std::vector<ReplaceOnDeathClass>::iterator iter = ReplaceOnDeathList.begin(); iter != ReplaceOnDeathList.end(); ++iter)
	{
		// If the tower is around...
		if ((IsAround(iter->ReplaceObject)) || 
			((iter->DoExplode) && (IsNotDeadAndPilot2(iter->ReplaceObject))))
		{
			// Copy the valid entry into the valid portion
			if (lastValid != iter)
				*validEnd = *iter;
			++validEnd;
		}
		else
		{
			if( //((!iter->DoExplode) || (!IsNotDeadAndPilot2(iter->ReplaceObject))) && 
				((!iter->ReplaceOnlyIfShot) || (iter->ShotTurn >= BZ1Helper::m_TurnCounter - 2)) && 
				(!iter->DidDemolish) && (DoesODFExist(iter->ReplaceODF)))
			{
				// Align To Terrain if it says so.
				bool AlignToTerrain = false;
				char TempODF[MAX_ODF_LENGTH] = {0};
				char TempODF2[MAX_ODF_LENGTH] = {0};
				sprintf_s(TempODF, "%s.odf", iter->ReplaceODF); // Add odf to the end.

				if(OpenODF2(TempODF))
				{
					if(GetODFString(TempODF, "GameObjectClass", "classlabel", MAX_ODF_LENGTH, TempODF2))
					{
						strcat_s(TempODF2, ".odf"); // Add .odf to the end.
						OpenODF2(TempODF2);
					}

					if((GetODFBool(TempODF, "GameObjectClass", "AlignToTerrain", &AlignToTerrain, false)) || 
						(GetODFBool(TempODF2, "GameObjectClass", "AlignToTerrain", &AlignToTerrain, false)))
					{
						if(AlignToTerrain)
						{
							Vector ANormal(0, 0, 0);
							float Height = 0.0f;
							TerrainGetHeightAndNormal(iter->Position.posit, Height, ANormal, false);
							iter->Position.posit.y = Height; //TerrainFindFloor(iter->Position.posit.x, iter->Position.posit.z); // Ground it.
							iter->Position = Build_Orthogonal_Matrix(ANormal, iter->Position.front, iter->Position.posit);
						}
					}
				}

				iter->Position.posit.y += iter->ReplaceHeight; // Add Height here.

				TempBuildList.resize(TempBuildList.size()+1);
				TempBuildClass &sao = TempBuildList[TempBuildList.size()-1]; // No temporary being created, *much* faster
				memset(&sao, 0, sizeof(TempBuildClass)); // Zero things out at start.

				// Push_back way ken suggested.
				//TempBuildList.reserve(TempBuildList.size());
				//TempBuildList.push_back(TempBuildClass());
				//TempBuildClass &sao = TempBuildList.back();
				//memset(&sao, 0, sizeof(TempBuildClass));

				strcpy_s(sao.ReplaceODF, iter->ReplaceODF);
				sao.ReplaceTeam = iter->ReplaceTeam;
				sao.Position = iter->Position;
				sao.StayTap = iter->StayTap;
				sao.TapIndex = iter->TapIndex;
				sao.HostObject = iter->ReplaceHostObject;
				sao.WasPlayer = iter->WasPlayer;
				sao.CurTeam = iter->CurTeam;
				sao.ReplaceCount = iter->ReplaceCount;
				sao.RandomHeading = iter->RandomHeading;

				/*
				Handle TempH = BuildObject(iter->ReplaceODF, iter->ReplaceTeam, iter->Position);
				//SetVectorPosition(TempH, iter->Position.posit);

				if((iter->StayTap) && (IsAround(iter->ReplaceHostObject)))
					SetTap(iter->ReplaceHostObject, iter->TapIndex, TempH);
				*/
			}
		}
	}
	// Now erase any invalid entries left behind
	ReplaceOnDeathList.erase(validEnd, ReplaceOnDeathList.end());

	// Now build the replacement objects.
	for (std::vector<TempBuildClass>::iterator iter = TempBuildList.begin(); iter != TempBuildList.end(); ++iter)
	{
		for(int i = 0; i < iter->ReplaceCount; i++)
		{
			if(iter->RandomHeading)
			{
				Vector Front = HRotateFront(Vector(0.0f, 0.0f, 1.0f), GetRandomFloat(360.0f) * DEG_2_RAD);
				iter->Position = Build_Directinal_Matrix(iter->Position.posit, Front); // GetPosition2?
			}

			Handle TempH = BuildObject(iter->ReplaceODF, iter->ReplaceTeam, iter->Position);

			//SetVectorPosition(TempH, iter->Position.posit);
			if(iter->WasPlayer)
				SetAsUser(TempH, iter->CurTeam);

			if((iter->StayTap) && (IsAround(iter->HostObject)))
				SetTap(iter->HostObject, iter->TapIndex, TempH);
		}
	}

	for (std::vector<ReplaceOnDeathClass>::iterator iter = ReplaceOnDeathList.begin(); iter != ReplaceOnDeathList.end(); ++iter)
	{
		if(IsAround(iter->ReplaceObject))
		{
			if(IsPlayer(iter->ReplaceObject))
				iter->WasPlayer = true;
			else
				iter->WasPlayer = false;

			iter->CurTeam = GetTeamNum(iter->ReplaceObject);

			if(iter->ReplaceTime > 0)
			{
				--iter->ReplaceTime;

				if(iter->ReplaceTime <= 0) // Times up. Replace us now.
				{
					RemoveObject(iter->ReplaceObject);
					continue; // Skip to next item.
				}
			}

			// Update it's position.
			GetPosition(iter->ReplaceObject, iter->Position);

			if((iter->StayTap) && (!IsAround(iter->ReplaceHostObject)))
			{
				bool Foundit = false;
				for(int i = 0; i < BuildingHelper::Get().GetNumBuildingObjects(); i++)
				{
					BuildingHelper::BuildingClass buildingInfo = BuildingHelper::Get().GetBuildingInfo(i);
					Handle Building = buildingInfo.BuildingObject;
					for(int x = 0; x < buildingInfo.TapNumber; x++)
					{
						if(iter->ReplaceObject == GetTap(Building, x))
						{
							// We found one. 
							iter->ReplaceHostObject = Building;
							iter->TapIndex = x;
							Foundit = true;
							break;
						}
					}
					if(Foundit)
						break;
				}
				if(!Foundit)
					iter->StayTap = false;
			}
		}
	}
}

void ReplaceOnDeathHelper::PreOrdnanceHit(Handle shooterHandle, Handle victimHandle, int ordnanceTeam, char* pOrdnanceODF)
{
	// If this is not a Human controlled team.
	for (std::vector<ReplaceOnDeathClass>::iterator iter = ReplaceOnDeathList.begin(); iter != ReplaceOnDeathList.end(); ++iter)
	{
		if(victimHandle == iter->ReplaceObject)
		{
			iter->ShotTurn = BZ1Helper::m_TurnCounter;
			return; // Were done, exit early. 
		}
	}
}