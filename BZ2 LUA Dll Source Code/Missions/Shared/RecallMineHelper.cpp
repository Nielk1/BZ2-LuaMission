#include "BZ1Helper.h"
#include "RecallMineHelper.h"
#include "BZCScriptUtils.h"
#include <string.h>
//#include <list>
#include <set>

RecallMineHelper::RecallMineHelper()
{
//	char SayHiMsg[1024] = "Nielk1's Recall Mine 1.0.0\n";
//	PrintConsoleMessage(SayHiMsg);
}

bool RecallMineHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		// Make sure to clear your attachedList first, it might not be in a Load function...
		recallMines.clear(); // Or whatever the call was...

		static char tempStr[2048];

		recallMineData s;
		Read(&s.owner, 1);
		while(s.owner!=0)
		{
			Read(&s.recallMine1, 1);
			Read(&s.recallMine2, 1);
			Read(&s.multiplyVelocity.x, 1);
			Read(&s.multiplyVelocity.y, 1);
			Read(&s.multiplyVelocity.z, 1);
			Read(&s.addVelocity.x, 1);
			Read(&s.addVelocity.y, 1);
			Read(&s.addVelocity.z, 1);
			Read(&s.chanceOfExplosion, 1);
			Read(&s.chanceOfEnterExplosion, 1);
			Read(&s.chanceOfFullFail, 1);
			Read(&s.chanceOfBadDamage, 1);
			Read(&s.chanceOfRuinedShip, 1);
			
			Read(&s.keepHeight, 1);
			Read(&s.randomHeading, 1);
			Read(&s.teleInAtMine, 1);

			Read(&s.minRadius, 1);
			Read(&s.maxRadius, 1);

			Read(&s.heightVarianceLow, 1);
			Read(&s.heightVarianceHigh, 1);

			//int strSize;
			//Read(&strSize, 1);
			//if(strSize>2047) throw std::exception("String Size out of bounds");
			//Read((void*)tempStr, strSize);
			//s.loopName = std::string(tempStr);

			int strSize;

			memset(tempStr, 0, sizeof(tempStr));//do this to all char arrays for bz2
			Read(&strSize, 1);
			if(strSize>2047) throw std::exception("String Size out of bounds");
			Read((void*)tempStr, strSize);
			s.teleIn = std::string(tempStr);

			memset(tempStr, 0, sizeof(tempStr));//do this to all char arrays for bz2
			Read(&strSize, 1);
			if(strSize>2047) throw std::exception("String Size out of bounds");
			Read((void*)tempStr, strSize);
			s.teleOut = std::string(tempStr);

			memset(tempStr, 0, sizeof(tempStr));//do this to all char arrays for bz2
			Read(&strSize, 1);
			if(strSize>2047) throw std::exception("String Size out of bounds");
			Read((void*)tempStr, strSize);
			s.teleFail = std::string(tempStr);

			memset(tempStr, 0, sizeof(tempStr));//do this to all char arrays for bz2
			Read(&strSize, 1);
			if(strSize>2047) throw std::exception("String Size out of bounds");
			Read((void*)tempStr, strSize);
			s.enterExplosionSound = std::string(tempStr);

			memset(tempStr, 0, sizeof(tempStr));//do this to all char arrays for bz2
			Read(&strSize, 1);
			if(strSize>2047) throw std::exception("String Size out of bounds");
			Read((void*)tempStr, strSize);
			s.exitExplosionSound = std::string(tempStr);

			memset(tempStr, 0, sizeof(tempStr));//do this to all char arrays for bz2
			Read(&strSize, 1);
			if(strSize>2047) throw std::exception("String Size out of bounds");
			Read((void*)tempStr, strSize);
			s.failSound = std::string(tempStr);

			memset(tempStr, 0, sizeof(tempStr));//do this to all char arrays for bz2
			Read(&strSize, 1);
			if(strSize>2047) throw std::exception("String Size out of bounds");
			Read((void*)tempStr, strSize);
			s.badDamageSound = std::string(tempStr);

			memset(tempStr, 0, sizeof(tempStr));//do this to all char arrays for bz2
			Read(&strSize, 1);
			if(strSize>2047) throw std::exception("String Size out of bounds");
			Read((void*)tempStr, strSize);
			s.ruinedShipSound = std::string(tempStr);

			memset(tempStr, 0, sizeof(tempStr));//do this to all char arrays for bz2
			Read(&strSize, 1);
			if(strSize>2047) throw std::exception("String Size out of bounds");
			Read((void*)tempStr, strSize);
			s.tooCloseSound = std::string(tempStr);

			memset(tempStr, 0, sizeof(tempStr));//do this to all char arrays for bz2
			Read(&strSize, 1);
			if(strSize>2047) throw std::exception("String Size out of bounds");
			Read((void*)tempStr, strSize);
			s.tooFarSound = std::string(tempStr);

			//Read(&(s.loopTime), 1);
			//Read(&(s.loopMode), 1);
			
			
			//std::set<Handle> attached;
	/*		attachedObjects s2;
			Read(&(s2.attachedHandle), 1);
			while(s2.attachedHandle!=0)
			{
				Read(&(s2.type), 1);
				Read(&(s2.displacement), 1);
				Read(&(s2.relayAttack), 1);
				Read(&(s2.deathType), 1);
				Read(&(s2.doingSharedAttack), 1);

				s.attached.insert(s2);

				Read(&(s2.attachedHandle), 1);
			}*/

			//int strSize;
			//Read(&strSize, 1);
			//if(strSize>2047) throw std::exception("String Size out of bounds");
			//Read((void*)tempStr, strSize);
			//s.loopName = std::string(tempStr);

			recallMines.insert(s);
			Read(&s.owner, 1);
		}


		outOfRangeRemoveList.clear();
		Handle outOfRangeMine;
		Read(&outOfRangeMine, 1);
		while(outOfRangeMine!=0)
		{
			outOfRangeRemoveList.insert(outOfRangeMine);
			Read(&outOfRangeMine, 1);
		}



		//Read(&m_GameTPS,1);
	}
	return true;
}

bool RecallMineHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		std::set<recallMineData>::iterator s = recallMines.begin();
		while( s != recallMines.end() )
		{
			ConvertHandles(&(s->owner), 1);
			ConvertHandles(&(s->recallMine1), 1);
			ConvertHandles(&(s->recallMine2), 1);
			s++;
		}

		std::set<Handle>::iterator s2 = outOfRangeRemoveList.begin();
		while( s2 != outOfRangeRemoveList.end() )
		{
			Handle* tmpHandle = &*s2;

			ConvertHandles(tmpHandle, 1);
			s2++;
		}
	}
	return true;
}

bool RecallMineHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		std::set<recallMineData>::iterator s = recallMines.begin();
		while( s != recallMines.end() )
		{
			Write(&(s->owner), 1);
			Write(&(s->recallMine1), 1);
			Write(&(s->recallMine2), 1);
			Write(&(s->multiplyVelocity.x), 1);
			Write(&(s->multiplyVelocity.y), 1);
			Write(&(s->multiplyVelocity.z), 1);
			Write(&(s->addVelocity.x), 1);
			Write(&(s->addVelocity.y), 1);
			Write(&(s->addVelocity.z), 1);
			Write(&(s->chanceOfExplosion), 1);
			Write(&(s->chanceOfEnterExplosion), 1);
			Write(&(s->chanceOfFullFail), 1);
			Write(&(s->chanceOfBadDamage), 1);
			Write(&(s->chanceOfRuinedShip), 1);

			Write(&(s->keepHeight), 1);
			Write(&(s->randomHeading), 1);
			Write(&(s->teleInAtMine), 1);

			Write(&(s->minRadius), 1);
			Write(&(s->maxRadius), 1);

			Write(&(s->heightVarianceLow), 1);
			Write(&(s->heightVarianceHigh), 1);

			int teleInSize = s->teleIn.size()+1;
			Write(&teleInSize, 1);
			Write((void*)s->teleIn.c_str(), teleInSize);

			int teleOutSize = s->teleOut.size()+1;
			Write(&teleOutSize, 1);
			Write((void*)s->teleOut.c_str(), teleOutSize);

			int teleFailSize = s->teleFail.size()+1;
			Write(&teleFailSize, 1);
			Write((void*)s->teleFail.c_str(), teleFailSize);

			int enterExplosionSoundSize = s->enterExplosionSound.size()+1;
			Write(&enterExplosionSoundSize, 1);
			Write((void*)s->enterExplosionSound.c_str(), enterExplosionSoundSize);

			int exitExplosionSoundSize = s->exitExplosionSound.size()+1;
			Write(&exitExplosionSoundSize, 1);
			Write((void*)s->exitExplosionSound.c_str(), exitExplosionSoundSize);

			int failSoundSize = s->failSound.size()+1;
			Write(&failSoundSize, 1);
			Write((void*)s->failSound.c_str(), failSoundSize);

			int badDamageSoundSize = s->badDamageSound.size()+1;
			Write(&badDamageSoundSize, 1);
			Write((void*)s->badDamageSound.c_str(), badDamageSoundSize);

			int ruinedShipSoundSize = s->ruinedShipSound.size()+1;
			Write(&ruinedShipSoundSize, 1);
			Write((void*)s->ruinedShipSound.c_str(), ruinedShipSoundSize);

			int tooCloseSoundSize = s->tooCloseSound.size()+1;
			Write(&tooCloseSoundSize, 1);
			Write((void*)s->tooCloseSound.c_str(), tooCloseSoundSize);

			int tooFarSoundSize = s->tooFarSound.size()+1;
			Write(&tooFarSoundSize, 1);
			Write((void*)s->tooFarSound.c_str(), tooFarSoundSize);

			/*Write(&(s->attachObject), 1);

			std::set<attachedObjects>::iterator s2 = s->attached.begin();
			while( s2 != s->attached.end() )
			{
				Write(&(s2->attachedHandle), 1);
				Write(&(s2->type), 1);
				Write(&(s2->displacement), 1);
				Write(&(s2->relayAttack), 1);
				Write(&(s2->deathType), 1);
				Write(&(s2->doingSharedAttack), 1);

				s2++;
			}
			int temp=0;
			Write(&temp, 1);*/

			s++;
		}
		int temp=0;
		Write(&temp, 1); // terminating zero
		//Write(&m_GameTPS,1);
		

		std::set<Handle>::iterator s2 = outOfRangeRemoveList.begin();
		while( s2 != outOfRangeRemoveList.end() )
		{
			Write(&s2, 1);
			s2++;
		}
		int temp2=0;
		Write(&temp2, 1); // terminating zero
	}
	return true;
}

void RecallMineHelper::AddRecallMineObject(const Handle h, const char* TempODF1, const char* TempODF2)
{
	recallMineData tmpHolder;
	Handle tmpOwner = GetNearestVehicle(h);
	if(tmpOwner) // We have a nearest Vehicle.
	{
		if(recallMines.size() > 0)
		{
			std::set<recallMineData>::iterator iter1 = recallMines.begin();
			while( iter1 != recallMines.end() )
			{
				if(tmpOwner == iter1->owner)
				{
					iter1->recallMine2 = h;
					Vector minePosition1 = GetPosition(iter1->recallMine1);
					Vector minePosition2 = GetPosition(iter1->recallMine2);

					if(iter1->maxRange > 0)
					{
						float currentDistance = GetDistance(iter1->recallMine1,iter1->owner);

						if(currentDistance > iter1->maxRange)
						{
							if((iter1->owner == GetPlayerHandle())&&(!iter1->tooFarSound.empty()))
							{
								StartAudio2D(iter1->tooFarSound.c_str(), 1.0f, 0.0f, -1.0f, false, false);
							}
						}
						if(currentDistance < iter1->minRange)
						{
							if((iter1->owner == GetPlayerHandle())&&(!iter1->tooCloseSound.empty()))
							{
								StartAudio2D(iter1->tooCloseSound.c_str(), 1.0f, 0.0f, -1.0f, false, false);
							}
						}
						if((currentDistance > iter1->maxRange) || (currentDistance < iter1->minRange))
						{
							outOfRangeRemoveList.insert(h);
							return;
						}
					}

					/*if((iter1->chanceOfFullFail > 0) || (iter1->chanceOfEnterExplosion > 0) || (iter1->chanceOfExplosion > 0))
					{
						const float totalArea = 3.0f;
						float randomFloat = GetRandomFloat(totalArea);
					}*/
					if(iter1->chanceOfFullFail > 0)
					{
						if(iter1->chanceOfFullFail < GetRandomFloat(1.0))
						{
							if((!iter1->teleFail.empty()))
							{

								if(iter1->teleInAtMine)
								{
									BuildObject((char*)(iter1->teleFail.c_str()), 0, minePosition2);
								}
								else
								{
									Vector ownerVector = GetPosition(iter1->owner);
									BuildObject((char*)(iter1->teleFail.c_str()), 0, ownerVector);
								}
							}
							if((iter1->owner == GetPlayerHandle())&&(!iter1->failSound.empty())){StartAudio2D(iter1->failSound.c_str(), 1.0f, 0.0f, -1.0f, false, false);}
							return;
						}
					}

					if(iter1->chanceOfEnterExplosion > 0)
					{
						if(iter1->chanceOfEnterExplosion < GetRandomFloat(1.0))
						{
							if((iter1->owner == GetPlayerHandle())&&(!iter1->enterExplosionSound.empty())){StartAudio2D(iter1->enterExplosionSound.c_str(), 1.0f, 0.0f, -1.0f, false, false);}
							SelfDamage(iter1->owner, GetMaxHealth(iter1->owner) + 1.0f);
							return;
						}
					}

					//get velocity and edit it
					Vector currentVelocity = GetVelocity(iter1->owner);

//		char TestMsg[1024];
//		sprintf_s(TestMsg, "Velocity Vector: %d, %d, %d\n", currentVelocity.x, currentVelocity.y, currentVelocity.z);
//		PrintConsoleMessage(TestMsg);

					currentVelocity.x *= iter1->multiplyVelocity.x;
					currentVelocity.y *= iter1->multiplyVelocity.y;
					currentVelocity.z *= iter1->multiplyVelocity.z;
					currentVelocity.x += iter1->addVelocity.x;
					currentVelocity.y += iter1->addVelocity.y;
					currentVelocity.z += iter1->addVelocity.z;

//		sprintf_s(TestMsg, "Edited Velocity Vector: %d, %d, %d\n", currentVelocity.x, currentVelocity.y, currentVelocity.z);
//		PrintConsoleMessage(TestMsg);


					Vector tmpVector;
					Vector newVector = minePosition1;
					GetPosition(iter1->owner,tmpVector);
					if(iter1->maxRadius > 0)
					{
						newVector = GetPositionNear(newVector, iter1->minRadius, iter1->maxRadius);
					}
					if(iter1->keepHeight)
					{
						float tmpFloor = TerrainFindFloor(tmpVector.x,tmpVector.z);
						newVector.y += (tmpVector.y - tmpFloor);
					}
					newVector.y += 1;

					if((iter1->heightVarianceHigh != 0) || (iter1->heightVarianceLow != 0)) // something is not default
					{
						newVector.y += iter1->heightVarianceLow + GetRandomFloat(iter1->heightVarianceHigh - iter1->heightVarianceLow);
					}

					SetVectorPosition(iter1->owner, newVector);
					
					if((!iter1->teleOut.empty()))
					{
						if(iter1->maxRadius > 0)
						{
							Vector tmpVector2 = newVector;
							tmpVector2.y -= 1;
							BuildObject((char*)(iter1->teleOut.c_str()), 0, tmpVector2);
						}else{
							BuildObject((char*)(iter1->teleOut.c_str()), 0, minePosition1);
						}
					}
					if((!iter1->teleIn.empty()))
					{
						//BuildObject((char*)(iter1->teleIn.c_str()), 0, minePosition2);
						if(iter1->teleInAtMine)
						{
							BuildObject((char*)(iter1->teleIn.c_str()), 0, minePosition2);
						}else{
							Vector ownerVector;
							GetPosition(iter1->owner,ownerVector);
							BuildObject((char*)(iter1->teleIn.c_str()), 0, ownerVector);
						}
					}

					if(iter1->chanceOfExplosion > 0)
					{
						if(iter1->chanceOfExplosion < GetRandomFloat(1.0))
						{
							if((iter1->owner == GetPlayerHandle())&&(!iter1->exitExplosionSound.empty())){StartAudio2D(iter1->exitExplosionSound.c_str(), 1.0f, 0.0f, -1.0f, false, false);}
							SelfDamage(iter1->owner, GetMaxHealth(iter1->owner) + 1.0f);
							return;
						}
					}

					if(iter1->chanceOfBadDamage > 0)
					{
						if(iter1->chanceOfBadDamage < GetRandomFloat(1.0))
						{
							if((iter1->owner == GetPlayerHandle())&&(!iter1->badDamageSound.empty())){StartAudio2D(iter1->badDamageSound.c_str(), 1.0f, 0.0f, -1.0f, false, false);}
							SetCurAmmo(iter1->owner, 1);
							SetCurHealth(iter1->owner, 1);
							//return;
						}
					}

					if(iter1->chanceOfRuinedShip > 0)
					{
						if(iter1->chanceOfRuinedShip < GetRandomFloat(1.0))
						{
							if((iter1->owner == GetPlayerHandle())&&(!iter1->ruinedShipSound.empty())){StartAudio2D(iter1->ruinedShipSound.c_str(), 1.0f, 0.0f, -1.0f, false, false);}
							SetMaxAmmo(iter1->owner, 1);
							SetMaxHealth(iter1->owner, 1);
							//return;
						}
					}

					/*Vector currentVelocity = GetVelocity(iter1->owner);
					currentVelocity.x *= iter1->multiplyVelocity.x;
					currentVelocity.y *= iter1->multiplyVelocity.y;
					currentVelocity.z *= iter1->multiplyVelocity.z;
					currentVelocity.x += iter1->addVelocity.x;
					currentVelocity.y += iter1->addVelocity.y;
					currentVelocity.z += iter1->addVelocity.z;
					SetVelocity(iter1->owner, iter1->currentVelocity);*/

					//apply new velocity from before
					SetVelocity(iter1->owner, currentVelocity);

					if(iter1->randomHeading){SetRandomHeadingAngle(iter1->owner);}


//		sprintf_s(TestMsg, "Add Velocity Vector: %d, %d, %d\n", iter1->addVelocity.x, iter1->addVelocity.y, iter1->addVelocity.z);
//		PrintConsoleMessage(TestMsg);

//						/*currentVelocity*/zeroVector.x += iter1->addVelocity.x;
//						/*currentVelocity*/zeroVector.y += iter1->addVelocity.y;
//						/*currentVelocity*/zeroVector.z += iter1->addVelocity.z;
						
//						SetVelocity(iter1->owner, zeroVector);
						//SetVelocity(iter1->owner, iter1->zeroVelocity);
					//}

					return; //nothing left
				}
				iter1++;
			}
		}
		tmpHolder.owner = tmpOwner;
		
		//char loopAnimationName[65]; //one more in length
		//memset(loopAnimationName, 0, sizeof(loopAnimationName));//do this to all char arrays for bz2
		//GetODFString(ODFName, "GameObjectClass", "loopAnimationName", 64, &loopAnimationName[0]);//causes the code to dump into the adjacent memory, which happens to be part of the array. This consuzed me for so long
		//tmpHolder.loopName = loopAnimationName;
		//GetODFFloat(ODFName, "GameObjectClass", "loopAnimationDelay", &tmpHolder.loopTime);
		//GetODFBool(ODFName, "GameObjectClass", "loopAnimationType", &tmpHolder.loopMode);
		//CloseODF(ODFName);
		//tmpHolder.loopTime *= m_GameTPS;
		//char attachName[65]; //one more in length
		//memset(attachName, 0, sizeof(attachName));//do this to all char arrays for bz2
		//GetODFString(ODFName, "RecallMineClass", "triggerMineWep", 64, &attachName[0]);

		//tmpHolder.owner = GetWhoShotMe(h);
		tmpHolder.recallMine1 = h;
		//GetODFBool(ODFName, "RecallMineClass", "zeroVelocity", &tmpHolder.zeroVelocity);
		GetODFVector(TempODF1, TempODF2, "RecallMineClass", "multiplyVelocity", &tmpHolder.multiplyVelocity,Vector(1.0f, 1.0f, 1.0f)); // I know, a waist of the mem for 3 float for a mear 3 bool
		GetODFVector(TempODF1, TempODF2, "RecallMineClass", "addVelocity", &tmpHolder.addVelocity);

		GetODFFloat(TempODF1, TempODF2, "RecallMineClass", "chanceOfExplosion", &tmpHolder.chanceOfExplosion);
		GetODFFloat(TempODF1, TempODF2, "RecallMineClass", "chanceOfEnterExplosion", &tmpHolder.chanceOfEnterExplosion);
		GetODFFloat(TempODF1, TempODF2, "RecallMineClass", "chanceOfFullFail", &tmpHolder.chanceOfFullFail);

		GetODFFloat(TempODF1, TempODF2, "RecallMineClass", "chanceOfBadDamage", &tmpHolder.chanceOfBadDamage);
		GetODFFloat(TempODF1, TempODF2, "RecallMineClass", "chanceOfRuinedShip", &tmpHolder.chanceOfRuinedShip);

		GetODFBool(TempODF1, TempODF2, "RecallMineClass", "keepHeight", &tmpHolder.keepHeight, false);
		GetODFBool(TempODF1, TempODF2, "RecallMineClass", "randomHeading", &tmpHolder.randomHeading, false);
		GetODFBool(TempODF1, TempODF2, "RecallMineClass", "teleInAtMine", &tmpHolder.teleInAtMine, false);

		GetODFFloat(TempODF1, TempODF2, "RecallMineClass", "minRadius", &tmpHolder.minRadius);
		GetODFFloat(TempODF1, TempODF2, "RecallMineClass", "maxRadius", &tmpHolder.maxRadius);

		GetODFFloat(TempODF1, TempODF2, "RecallMineClass", "heightVarianceLow",  &tmpHolder.heightVarianceLow);
		GetODFFloat(TempODF1, TempODF2, "RecallMineClass", "heightVarianceHigh",  &tmpHolder.heightVarianceHigh);

		GetODFFloat(TempODF1, TempODF2, "RecallMineClass", "minRange", &tmpHolder.minRange);
		GetODFFloat(TempODF1, TempODF2, "RecallMineClass", "maxRange", &tmpHolder.maxRange);
		if(&tmpHolder.minRadius < 0){tmpHolder.minRadius *= (-1);}
		if(&tmpHolder.maxRadius < 0){tmpHolder.maxRadius *= (-1);}

		char teleIn[MAX_ODF_LENGTH] = {0};
		GetODFString(TempODF1, TempODF2, "RecallMineClass", "teleIn", MAX_ODF_LENGTH, teleIn);//causes the code to dump into the adjacent memory, which happens to be part of the array. This consuzed me for so long
		tmpHolder.teleIn = teleIn;

		char teleOut[MAX_ODF_LENGTH] = {0};
		GetODFString(TempODF1, TempODF2, "RecallMineClass", "teleOut", MAX_ODF_LENGTH, teleOut);//causes the code to dump into the adjacent memory, which happens to be part of the array. This consuzed me for so long
		tmpHolder.teleOut = teleOut;

		char teleFail[MAX_ODF_LENGTH] = {0};
		GetODFString(TempODF1, TempODF2, "RecallMineClass", "teleFail", MAX_ODF_LENGTH, teleFail);//causes the code to dump into the adjacent memory, which happens to be part of the array. This consuzed me for so long
		tmpHolder.teleFail = teleFail;

		char exitExplosionSound[MAX_ODF_LENGTH] = {0};
		GetODFString(TempODF1, TempODF2, "RecallMineClass", "exitExplosionSound", MAX_ODF_LENGTH, exitExplosionSound);//causes the code to dump into the adjacent memory, which happens to be part of the array. This consuzed me for so long
		tmpHolder.exitExplosionSound = exitExplosionSound;

		char enterExplosionSound[MAX_ODF_LENGTH] = {0};
		GetODFString(TempODF1, TempODF2, "RecallMineClass", "enterExplosionSound", MAX_ODF_LENGTH, enterExplosionSound);//causes the code to dump into the adjacent memory, which happens to be part of the array. This consuzed me for so long
		tmpHolder.enterExplosionSound = enterExplosionSound;

		char failSound[MAX_ODF_LENGTH] = {0};
		GetODFString(TempODF1, TempODF2, "RecallMineClass", "failSound", MAX_ODF_LENGTH, failSound);//causes the code to dump into the adjacent memory, which happens to be part of the array. This consuzed me for so long
		tmpHolder.failSound = failSound;

		char badDamageSound[MAX_ODF_LENGTH] = {0};
		GetODFString(TempODF1, TempODF2, "RecallMineClass", "badDamageSound", MAX_ODF_LENGTH, badDamageSound);//causes the code to dump into the adjacent memory, which happens to be part of the array. This consuzed me for so long
		tmpHolder.badDamageSound = badDamageSound;

		char ruinedShipSound[MAX_ODF_LENGTH] = {0};
		GetODFString(TempODF1, TempODF2, "RecallMineClass", "ruinedShipSound", MAX_ODF_LENGTH, ruinedShipSound);//causes the code to dump into the adjacent memory, which happens to be part of the array. This consuzed me for so long
		tmpHolder.ruinedShipSound = ruinedShipSound;

		char tooCloseSound[MAX_ODF_LENGTH] = {0};
		GetODFString(TempODF1, TempODF2, "RecallMineClass", "tooCloseSound", MAX_ODF_LENGTH, tooCloseSound);//causes the code to dump into the adjacent memory, which happens to be part of the array. This consuzed me for so long
		tmpHolder.tooCloseSound = tooCloseSound;

		char tooFarSound[MAX_ODF_LENGTH] = {0};
		GetODFString(TempODF1, TempODF2, "RecallMineClass", "tooFarSound", MAX_ODF_LENGTH, tooFarSound);//causes the code to dump into the adjacent memory, which happens to be part of the array. This consuzed me for so long
		tmpHolder.tooFarSound = tooFarSound;

		//char TestMsg[1024];
		//sprintf_s(TestMsg, "who shot mine: %d player is: %d\n", tmpHolder.owner, GetPlayerHandle(1));
		//PrintConsoleMessage(TestMsg);

		recallMines.insert(tmpHolder);
	}
	// End Queue Loop Animation
}

void RecallMineHelper::Execute(BZ1Helper &bz1Helper)
{
//		char TestMsg[1024] = "In Execute\n";
//		PrintConsoleMessage(TestMsg);



	std::set<Handle>::iterator iter2 = outOfRangeRemoveList.begin();
	while( iter2 != outOfRangeRemoveList.end() )
	{
		//char TestMsg[1024];
		//sprintf_s(TestMsg, "test %d %d %d = %d\n", !IsAround(iter1->owner), !IsAround(iter1->recallMine1), !!iter1->removePair, ((!IsAround(iter1->owner)) || (!IsAround(iter1->recallMine1)) || (iter1->removePair)));
		//PrintConsoleMessage(TestMsg);

		Handle* tmpHandle = &*iter2;

		if(IsAround(*tmpHandle))
		{
			RemoveObject(*tmpHandle);
			iter2 = outOfRangeRemoveList.erase(iter2);
		}else{
			iter2++;
		}
	}




	std::set<recallMineData>::iterator iter1 = recallMines.begin();
	while( iter1 != recallMines.end() )
	{
		//char TestMsg[1024];
		//sprintf_s(TestMsg, "test %d %d %d = %d\n", !IsAround(iter1->owner), !IsAround(iter1->recallMine1), !!iter1->removePair, ((!IsAround(iter1->owner)) || (!IsAround(iter1->recallMine1)) || (iter1->removePair)));
		//PrintConsoleMessage(TestMsg);

		if((!IsAround(iter1->owner)) || (!IsAround(iter1->recallMine1)) || (IsAround(iter1->recallMine1) && IsAround(iter1->recallMine2)))// || (iter1->removePair))
		{
			if(IsAround(iter1->recallMine1)){RemoveObject(iter1->recallMine1);}
			if(IsAround(iter1->recallMine2)){RemoveObject(iter1->recallMine2);}
			//iter1->removePair = false;
			iter1 = recallMines.erase(iter1);
		}else{
			iter1++;
		}
	}
}

bool operator<(const RecallMineHelper::recallMineData &self, const RecallMineHelper::recallMineData &other) 
{  
	return self.recallMine1 < other.recallMine1;
}