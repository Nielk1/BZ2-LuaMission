#ifndef _DeployHelper_
#define _DeployHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class DeployHelper
{
private:

	struct DeployClass {
		Handle DeployObject; // This Deploy Object.
		bool IsItDeployed; // Is it deployed?

		DeployClass() { memset(this, 0, sizeof(DeployClass)); };
	};
	std::vector<DeployClass> DeployList;
	static bool ShouldRemoveDeploy(const DeployClass &entry) { return !IsAround(entry.DeployObject); }

public:
	DeployHelper();

	void AddDeployObject(const Handle h);

	bool Load(bool missionSave);
	bool PostLoad(bool missionSave);
	bool Save(bool missionSave);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
