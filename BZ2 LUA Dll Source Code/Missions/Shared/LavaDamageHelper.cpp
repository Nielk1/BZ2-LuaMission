#include "BZ1Helper.h"
#include "LavaDamageHelper.h"
#include "BZCScriptUtils.h"
#include "math.h"
#include <malloc.h>

//LavaDamageHelper *LavaDamageHelper::m_LavaDamageHelper;

LavaDamageHelper::LavaDamageHelper()
{
//	m_LavaDamageHelper = this;
}

/*
LavaDamageHelper::~LavaDamageHelper()
{
//	 m_LavaDamageHelper = NULL;
}
*/

//bool LavaDamageHelper::Load(bool missionSave)
//{
	///*
	//if (!missionSave)
	//{
	//	char mapTrnFile[2*MAX_ODF_LENGTH] = {0};
	//	strcpy_s(mapTrnFile, GetMapTRNFilename());

//		LoadLavaField(GetMapTRNFilename());
	//}
	//*/

	/*
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		LavaList.resize(size);
		if(size)
			Read(&LavaList.front(), sizeof(LavaFieldClass)*size);
	}
	*/
//	return true;
//}

/*
bool LavaDamageHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(LavaList.size());
		Write(&size, 1);
		if(size)
			Write(&LavaList.front(), sizeof(LavaFieldClass)*size);
	}
	return true;
}
*/

void LavaDamageHelper::Init(BZ1Helper &bz1Helper)
{
	bool Logging = BZ1Helper::Logging;

	int TotalLavaObjects = 0;

	BZ1Helper::MapInfo const &mapInfo = BZ1Helper::Get().GetMapInfo();
	float MapMPG = mapInfo.MPG;
	float MapWidth = mapInfo.MapWidth;
	float MapDepth = mapInfo.MapDepth;
	float MinX = mapInfo.MinX;
	float MinZ = mapInfo.MinZ;

	/*
	{
		char mapTrnFile[2*MAX_ODF_LENGTH] = {0};
		strcpy_s(mapTrnFile, GetMapTRNFilename());
		if(OpenODF2(mapTrnFile))
		{
			// Read Lava Damage data.
			GetODFFloat(mapTrnFile, "Size", "MetersPerGrid", &MapMPG); // Grab the map's Meters Per Grid, used to base Count off of. 
			GetODFFloat(mapTrnFile, "Size", "Width", &MapWidth); // Grab the map's original width
			GetODFFloat(mapTrnFile, "Size", "Depth", &MapDepth); // Grab the map's original depth
			GetODFFloat(mapTrnFile, "Size", "MinX", &MinX);
			GetODFFloat(mapTrnFile, "Size", "MinZ", &MinZ);
		*/

	//LoadLavaField(GetMapTRNFilename());
	//	}
//	}

	// Lava Pathing code. Place objects around lava boarder to make units path around.
	// Code by General BlackDragon, with help from Nielk1, DeathDoom, and Ken Miller.
	const float ClustersPerMeter = 0.25f / MapMPG;
	const int ClustersPerRow = int(ceilf(MapWidth * ClustersPerMeter));
	const int ClustersPerColumn = int(ceilf(MapDepth * ClustersPerMeter));
	const int TotalClusters = (ClustersPerRow*ClustersPerColumn);
	const int TotalClusterBytes = (TotalClusters + 7) / 8;	// round up to the next byte

	// allocate an array of bits, one per total cluster
	unsigned char *PlacedLavaObject = static_cast<unsigned char *>(_alloca(TotalClusterBytes));
	memset(PlacedLavaObject, 0, TotalClusterBytes);

	for (std::vector<LavaFieldClass>::iterator iter = LavaList.begin(); iter != LavaList.end(); ++iter)
	{
		//int ResidualDistance = 0;
		const int numOfSides = iter->Field.size(); //LavaField[x].size();

		/* Iterate through each line */
		int i,j;
		for ( i = 0, j = 1; i < numOfSides; i++, j++)
		{
			if(j >= numOfSides)
				j = 0;

			// Path segment endpoints
			const VECTOR_2D &start = iter->Field[i]; //LavaField[x][i];
			const VECTOR_2D &end = iter->Field[j]; //LavaField[x][j];

			// Skip segments running along a world boundary
			if ((start.x < MinX + 1.0f) && (end.x < MinX + 1.0f))
				continue;
			if ((start.z < MinZ + 1.0f) && (end.z < MinZ + 1.0f))
				continue;
			const float MaxX = MinX + MapWidth;
			if ((start.x > MaxX - 1.0f) && (end.x > MaxX - 1.0f))
				continue;
			const float MaxZ = MinZ + MapDepth;
			if ((start.z > MaxZ - 1.0f) && (end.z > MaxZ - 1.0f))
				continue;

			//const float Distance = GetDistance2D(end, start);
			const float Distance = max(fabsf(end.x - start.x), fabsf(end.z - start.z));

			const int NumberOfBuildings = int(ceilf(Distance * ClustersPerMeter));

			// Get step distance per building
			const float dx = (end.x - start.x) / float(NumberOfBuildings);
			const float dz = (end.z - start.z) / float(NumberOfBuildings);

			if(Logging)
			{
				FormatLogMessage("Distance is: %f Original Coordinates: X1:%f Z1:%f X2:%f Z2:%f, DX Is: %f, DZ is: %f", Distance, start.x,start.z, end.x, end.z, dx, dz); 
				FormatLogMessage("Number of Buildings is: %d", NumberOfBuildings); 
			}

			// Start at first point.
			Vector Position(start.x, 0, start.z);

			for(int u = 0; u <= NumberOfBuildings; u++)
			{
				 // Get the cluster index for the current position
				const int ClusterIndex =
					clamp(int((Position.z - MinZ) * ClustersPerMeter), 0, ClustersPerColumn - 1) * ClustersPerRow +
					clamp(int((Position.x - MinX) * ClustersPerMeter), 0, ClustersPerRow - 1);

				// If the cell is not already occupied...
				const int ClusterByte = ClusterIndex>>3;
				const unsigned char ClusterBit = 1 << (ClusterIndex & 7);
				if(!(PlacedLavaObject[ClusterByte] & ClusterBit))
				{
					// Mark the cell as occupied
					PlacedLavaObject[ClusterByte] |= ClusterBit;

					// Build the lava field
					Position.y = TerrainFindFloor(Position.x,Position.z);
					BuildObject("BZClavafield",0,Position);
					++TotalLavaObjects;

					if(Logging)
						FormatLogMessage("Destination Position is: X: %f Y: %f", Position.x, Position.z); 
				}
				else
				{
					if(Logging)
						FormatLogMessage("Skipping some Lava");
				}

				// Advance to the next position
				Position.x += dx;
				Position.z += dz;
			}
		}
	}

	if(Logging)
		FormatLogMessage("Total Lava Objects Placed: %d", TotalLavaObjects);

//	m_DidInit = true;
}

void LavaDamageHelper::Execute(int HandleIndex)
{
//	if(!m_DidInit)
//		Init();

	bool Logging = BZ1Helper::Logging;

	BZ1Helper::ObjectClass &objectInfo = BZ1Helper::Get().RefHandleInfo(HandleIndex);
	Handle TempH = objectInfo.ObjectHandle;

	for (std::vector<LavaFieldClass>::iterator iter = LavaList.begin(); iter != LavaList.end(); ++iter)
	{
		if( (objectInfo.CategoryID == Powerup) || 
			(objectInfo.CategoryID == Other) || 
			(objectInfo.CategoryID == Weapon) || 
			(GetMaxHealth(TempH) <= 0)) // || (IsFury(TempH))) // Fury check lags too much. Furies shouldn't be low enough to be effected anyway.
			return; // Skip this object.

		if(Logging)
			if(IsPlayer(TempH))
				FormatLogMessage("GetWindingCount LavaFieldIndex: %d, Count: %d", iter - LavaList.begin(), GetWindCount(iter->Field, GetPosition(GetPlayerHandle())));

		if(iter->DamageOutside != PointInPoly(iter->Field, objectInfo.Position))
		{
			if((iter->DeleteScrap) && (objectInfo.CategoryID == Scrap))
			{
				RemoveObject(TempH);
				return; // Exit early.
			}

			float FloorHeight = TerrainFindFloor(objectInfo.Position.x, objectInfo.Position.z);
			float Altitude = (objectInfo.Position.y - FloorHeight);

			if(Altitude < iter->DamageHeight)
			{
				Altitude = Altitude - 1.0f;
				if(Altitude < 1.0f)
					Altitude = 1.0f; // Clamp this. 

				float Ratio = 0.0f;
				if(Altitude > 1.0f)
					Ratio = 1.25f - (0.25f * Altitude);
				else if(Altitude > 0.0f)
					Ratio = 2.0f - (1.0f * Altitude);
				else
					Ratio = 2.0f;

				if(Ratio < 0.0f)
					Ratio = 0.0f;

			//	float NewDamageValue = ((40.0f * 4.0f * Ratio) / BZ1Helper::m_GameTPS); //(LavaDamageAmount/m_GameTPS)* Ratio;
				float DamageValue = (ConvertToSeconds(iter->DamageAmount) / Altitude);
				DamageF(TempH, DamageValue);

				if(iter->SpeedMod > 0.0f)
				{
					// drag on object's velocity
					Vector v = GetVelocity(TempH);
					float speed = sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
					float drag = iter->SpeedMod * ConvertToSeconds(1.0f); //dt;
					if (speed > drag)
					{
						float scale = (1.0f - drag / speed);
						v = Vector_Scale(v, scale);
					}
					else
					{
						v = Vector(0.0f, 0.0f, 0.0f);
					}
					SetVelocity(TempH, v);
				}

				if(IsPlayer(TempH)) // && (GetTeamNum(TempH) == GetLocalPlayerTeamNumber()))
				{
					if(!IsNetworkOn())
					{
						SetColorFade((0.01f*DamageValue), 5.0f, iter->FadeColor);
					}
				//	/* //!-- Test this resync fix? -GBD
					else
					{
						Handle TempH = BuildObject(iter->DamageEmitter, 0, objectInfo.Position);
						SetLifespan(TempH, ConvertToSeconds(1.0f));
					}
				//	*/
				}

			//	if(Logging)
			//		FormatLogMessage("Altitude is %f, ColorFadeRate is: %f, OldCalcDamage is: %f NewCalcDamage Is: %f, Ratio is: %f", Altitude, (0.005f*DamageValue), DamageValue, NewDamageValue, Ratio); 
			}
		}
	}


	/*
	// Scrap Lava Cull code, Removes scrap from lava fields.
	if((ScrapHelper::Get().GetNumScrapObjects() > 0) && (iter->DeleteScrap))
	{
		for(int i = 0; i<ScrapHelper::Get().GetNumScrapObjects(); i++)
		{
			if(IsAround(ScrapHelper::Get().RefScrapHandle(i)))
			{
				Vector ObjectPosition =	GetPosition(ScrapHelper::Get().RefScrapHandle(i));
				for(int x = 0; x<m_NumLavaFields; x++)
				{
					if(PointInPoly(iter->Field, ObjectPosition))
					{
						RemoveObject(ScrapHelper::Get().RefScrapHandle(i));

						//ScrapHelper::Get().RemoveScrapHandle(i);
						--i; // back up, so we don't skip an object this pass
					}
				}
			}
		}
	}
	*/
}

void LavaDamageHelper::LoadLavaField(const char* mapTrnFile)
{
//	char mapTrnFile[2*MAX_ODF_LENGTH] = {0};
//	strcpy_s(mapTrnFile, GetMapTRNFilename());
	if(OpenODF2(mapTrnFile))
	{
		// Pull lavafield damage things. Code from Nielk1. -GBD
		for(int x = 0; x<MAX_LAVA_FIELDS; x++)
		{
	//		LavaField[x].clear();
			size_t bufSize = 0;
			char DesiredValue[MAX_ODF_LENGTH] = {0};
			sprintf_s(DesiredValue, "lavafield%d", x+1);
			GetPathPoints(DesiredValue, bufSize, NULL);
			float *pData = static_cast<float *>(_alloca(sizeof(float) * 2 * bufSize));
			if(GetPathPoints(DesiredValue, bufSize, pData))
			{
				LavaList.resize(LavaList.size()+1);
				LavaFieldClass &sao = LavaList[LavaList.size()-1]; // No temporary being created, *much* faster
				memset(&sao, 0, sizeof(LavaFieldClass)); // Zero things out at start.

				char DesiredValue[MAX_ODF_LENGTH] = {0};
				sprintf_s(DesiredValue, "Lava%d", x+1);

				if((GetODFInt(mapTrnFile, DesiredValue, "LavaDamage", &sao.DamageAmount, 250)) || 
					(GetODFInt(mapTrnFile, "Lava", "LavaDamage", &sao.DamageAmount, 250)))
				{
				}

				if((GetODFFloat(mapTrnFile, DesiredValue, "LavaHeight", &sao.DamageHeight, 15.0f)) || 
					(GetODFFloat(mapTrnFile, "Lava", "LavaHeight", &sao.DamageHeight, 15.0f)))
				{
				}

				if((GetODFColor(mapTrnFile, DesiredValue, "LavaFadeColor", &sao.FadeColor, LAVACOLOR)) || 
					(GetODFColor(mapTrnFile, "Lava", "LavaFadeColor", &sao.FadeColor, LAVACOLOR)))
				{
				}

				if((GetODFBool(mapTrnFile, DesiredValue, "LavaRemovesScrap", &sao.DeleteScrap, false)) || 
					(GetODFBool(mapTrnFile, "Lava", "LavaRemovesScrap", &sao.DeleteScrap, false)))
				{
				}

				if((GetODFBool(mapTrnFile, DesiredValue, "LavaDamageOutside", &sao.DamageOutside, false)) || 
					(GetODFBool(mapTrnFile, "Lava", "LavaDamageOutside", &sao.DamageOutside, false)))
				{
				}

				if((GetODFFloat(mapTrnFile, DesiredValue, "LavaSpeedModifier", &sao.SpeedMod, 0.0f)) || 
					(GetODFFloat(mapTrnFile, "Lava", "LavaSpeedModifier", &sao.SpeedMod, 0.0f)))
				{
				}

				if((GetODFString(mapTrnFile, DesiredValue, "LavaDamageEmitter", MAX_ODF_LENGTH, sao.DamageEmitter, "BZClavadmg")) || 
					(GetODFString(mapTrnFile, "Lava", "LavaDamageEmitter", MAX_ODF_LENGTH, sao.DamageEmitter, "BZClavadmg")))
				{
				}

				//++m_NumLavaFields;
				//size_t i;
				for(size_t i = 0; i < bufSize; ++i)
				{
					VECTOR_2D newPoint; // does this need to be a pointer?
					newPoint.x = pData[2*i+0];
					newPoint.z = pData[2*i+1];

					sao.Field.resize(sao.Field.size()+1);
					VECTOR_2D &fld = sao.Field[sao.Field.size()-1]; // No temporary being created, *much* faster
					memset(&fld, 0, sizeof(VECTOR_2D)); // Zero things out at start.

					fld = newPoint;
					sao.PointCount++; // Increase this.
				}
			}
		}
	//	CloseODF(mapTrnFile);
	}
}