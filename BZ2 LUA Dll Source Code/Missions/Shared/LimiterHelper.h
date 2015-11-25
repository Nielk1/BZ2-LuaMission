#ifndef _LimiterHelper_
#define _LimiterHelper_

#include "..\..\source\fun3d\ScriptUtils.h"

class BZ1Helper;

class LimiterHelper
{

public:
	LimiterHelper();

	void AddLimiterObject(const Handle h, const int Team, const char *ODFName);

	void Execute(BZ1Helper &bz1Helper);
};

#endif
