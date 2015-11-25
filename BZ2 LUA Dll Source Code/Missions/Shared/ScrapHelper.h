#ifndef _ScrapHelper_
#define _ScrapHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class ScrapHelper
{
private:

	static ScrapHelper *m_ScrapHelper;

	struct ScrapClass {
		Handle ScrapObject; // This Scrap Object.

		ScrapClass() { memset(this, 0, sizeof(ScrapClass)); };
	};
	std::vector<ScrapClass> ScrapList;
	static bool ShouldRemoveScrap(const ScrapClass &entry) { return !IsAround(entry.ScrapObject); }

public:
	ScrapHelper();
	~ScrapHelper();
	static ScrapHelper &Get() { return *m_ScrapHelper; }

	int GetNumScrapObjects() { return ScrapList.size(); }
	Handle &RefScrapHandle(const int index) { return ScrapList[index].ScrapObject; }

//	void RemoveScrapHandle(const int index);
	void GetRidOfSomeScrap(const int ScrapLimit = 1000);
	void ClearScrapAround(const Dist d, const Vector &pos, const float Percent = 1.0f);

	void AddScrapObject(const Handle h, const char* TempODF1, const char* TempODF2);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
