#include "BZ1Helper.h"
#include "DeployHelper.h"
#include "BZCScriptUtils.h"
#include <algorithm>

DeployHelper::DeployHelper()
{
}

bool DeployHelper::Load(bool missionSave)
{
	if (!missionSave)
	{
		int size;
		Read(&size, 1);
		DeployList.resize(size);
		if(size)
			Read(&DeployList.front(), sizeof(DeployClass)*size);
	}
	return true;
}

bool DeployHelper::PostLoad(bool missionSave)
{
	if (!missionSave)
	{
		for (std::vector<DeployClass>::iterator iter = DeployList.begin(); iter != DeployList.end(); ++iter)
		{
			DeployClass &sao = *iter;
			ConvertHandles(&sao.DeployObject, 1);
		}
	}
	return true;
}

bool DeployHelper::Save(bool missionSave)
{
	if (!missionSave)
	{
		int size = int(DeployList.size());
		Write(&size, 1);
		if(size)
			Write(&DeployList.front(), sizeof(DeployClass)*size);
	}
	return true;
}

void DeployHelper::AddDeployObject(const Handle h)
{
	for (std::vector<DeployClass>::iterator iter = DeployList.begin(); iter != DeployList.end(); ++iter)
	{
		if(iter->DeployObject == h)
			return;
	}

	DeployList.resize(DeployList.size()+1);
	DeployClass &sao = DeployList[DeployList.size()-1]; // No temporary being created, *much* faster
	memset(&sao, 0, sizeof(DeployClass)); // Zero things out at start.

	sao.DeployObject = h;
}

void DeployHelper::Execute(BZ1Helper &bz1Helper)
{
//	bool Logging = BZ1Helper::Logging;

	// pre-clean the vector
	DeployList.erase(std::remove_if(DeployList.begin(), DeployList.end(), ShouldRemoveDeploy), DeployList.end());

	for (std::vector<DeployClass>::iterator iter = DeployList.begin(); iter != DeployList.end(); ++iter)
	{
		if((!iter->IsItDeployed) && (IsDeployed(iter->DeployObject))) // Only proceed if the object is still around and working.
		{
			iter->IsItDeployed = true;
			SetAnimation(iter->DeployObject, "deploy", 1); // Restart the animation. 
			StartAnimation(iter->DeployObject);
		}
		else if((iter->IsItDeployed) && (!IsDeployed(iter->DeployObject)))
		{
			iter->IsItDeployed = false;
			SetAnimation(iter->DeployObject, "undeploy", 1); // Restart the animation. 
			StartAnimation(iter->DeployObject);
		}
	}
}
