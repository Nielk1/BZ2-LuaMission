#ifndef _ProductionClassHelper_
#define _ProductionClassHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class ProductionClassHelper
{
private:

	static ProductionClassHelper *m_ProductionClassHelper;

	struct CameraPodClass {
		Handle CameraPodObject; // This CameraPod Object.

		CameraPodClass() { memset(this, 0, sizeof(CameraPodClass)); };
	};
	std::vector<CameraPodClass> CameraPodList;
	static bool ShouldRemoveCameraPod(const CameraPodClass &entry) { return !IsAround(entry.CameraPodObject); }

	/*
	bool b_first,
		m_Distance1Check[MAX_TEAMS],
		m_Distance2Check[MAX_TEAMS],
		SlfPlayingAnim[MAX_TEAMS],
		b_last;

	float f_first,
		Distance1[MAX_TEAMS],
		Distance2[MAX_TEAMS],
		SLFAnimationFrames[MAX_TEAMS],
		f_last;

	Handle h_first,
		RecScrap[MAX_TEAMS], // OFF MAP!
		MufScrap[MAX_TEAMS], // OFF MAP!
		SlfScrap[MAX_TEAMS], // OFF MAP!
		CnstScrap[MAX_TEAMS], // OFF MAP!
		Recy[MAX_TEAMS],
		Muf[MAX_TEAMS],
		Slf[MAX_TEAMS],
		Cnst[MAX_TEAMS],
		SLFTargets[MAX_TEAMS],
		RecyRadar[MAX_TEAMS],
		MufRadar[MAX_TEAMS],
		SlfRadar[MAX_TEAMS],
		h_last;

	bool *b_array;
	int b_count;

	float *f_array;
	int f_count;

	int *h_array;
	int h_count;
	*/

public:
	ProductionClassHelper();
	~ProductionClassHelper();
	static ProductionClassHelper &Get() { return *m_ProductionClassHelper; }

	/*
	Handle &RefRecScrapHold(const int Team) { return RecScrap[Team]; }
	Handle &RefMufScrapHold(const int Team) { return MufScrap[Team]; }
	Handle &RefSlfScrapHold(const int Team) { return SlfScrap[Team]; }
	Handle &RefCnstScrapHold(const int Team) { return CnstScrap[Team]; }

	// Production Unit Handles. BZC Production units can undeploy, meaning their handle is lost. Use this in SP mission scripts to keep track of production units.
	Handle GetRecy(const int Team) {return Recy[Team];}
	Handle GetMuf(const int Team) {return Muf[Team];}
	Handle GetSlf(const int Team) {return Slf[Team];}
	Handle GetCnst(const int Team) {return Cnst[Team];}
	*/

	void AddProductionClassObject(const Handle h, const char* TempODF1, const char* TempODF2, const int Team, const int ObjClassHash, const int BZCClassHash);
	void AddCameraObject(const Handle h);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
