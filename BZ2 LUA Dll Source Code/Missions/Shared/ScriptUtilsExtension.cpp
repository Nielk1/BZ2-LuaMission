#include "ScriptUtilsExtension.h"

// Get Vector from a Path point: By Nielk1.
Vector GetVectorFromPath(const char* path, const int point)
{
	// New code from Ken Miller.
	Vector retVal(0, 0, 0);
	size_t bufSize = 0;
	GetPathPoints(path, bufSize, NULL);

	// Bad Path, abort!
	if (!bufSize)
		return retVal;

	if (point < UnsignedToSigned(bufSize))
	{
		float *pData = static_cast<float *>(_alloca(sizeof(float) * 2 * bufSize));
		if (GetPathPoints(path, bufSize, pData))
			retVal = Vector(pData[2 * point + 0], TerrainFindFloor(pData[2 * point + 0], pData[2 * point + 1]), pData[2 * point + 1]);
	}

	return retVal;
}