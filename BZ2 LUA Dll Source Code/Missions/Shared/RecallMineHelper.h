#ifndef _RecallMineHelper_
#define _RecallMineHelper_

#include "..\..\source\fun3d\ScriptUtils.h"
//#include <vector>
//#include <list>
#include <set>

class BZ1Helper;

class RecallMineHelper
{

public:
	/*struct attachedObjects {
		//int numObjects;
		Handle attachedHandle;
		//std::string loopName;
		bool type;
		float displacement;
		bool relayAttack;
		int deathType;
		bool doingSharedAttack;
	};*/

	struct recallMineData {
		Handle owner;
		Handle recallMine1;
		Handle recallMine2;
		//bool removePair;
		Vector multiplyVelocity;
		Vector addVelocity;
		std::string teleIn;
		std::string teleOut;
		std::string teleFail;
		//Name recallWep;
		//int numObjects;
		//std::set<attachedObjects> attached;
		//std::string loopName;
		float chanceOfExplosion;
		float chanceOfEnterExplosion;
		float chanceOfFullFail;

		float chanceOfBadDamage;
		float chanceOfRuinedShip;

		std::string exitExplosionSound;
		std::string enterExplosionSound;
		std::string failSound;

		std::string badDamageSound;
		std::string ruinedShipSound;

		bool keepHeight;
		bool randomHeading;
		bool teleInAtMine;

		float minRadius;
		float maxRadius;

		float heightVarianceLow;
		float heightVarianceHigh;

		float minRange;
		float maxRange;

		std::string tooCloseSound;
		std::string tooFarSound;
	};

	std::set<recallMineData> recallMines;
	std::set<Handle> outOfRangeRemoveList;

public:
	RecallMineHelper();	
	bool Save(bool missionSave);
	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	void AddRecallMineObject(const Handle h, const char* TempODF1, const char* TempODF2);
	void Execute(BZ1Helper &bz1Helper);
};
#endif

