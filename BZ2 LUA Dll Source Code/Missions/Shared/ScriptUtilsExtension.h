#ifndef _ScriptUtilsExtension_
#define _ScriptUtilsExtension_

#include "lua.hpp"
#include "..\..\source\fun3d\ScriptUtils.h"
#include "MathUtils.h"
#include <math.h>
#include <vector>
#include <hash_map>
#include <algorithm>
#include <limits.h>
#include <float.h>
#include <ctype.h>

// String for who modified this DLL, for printing in console of misisons. If you make changes, simply add your name here.
static const char *DLLAuthors = "Avatar, General BlackDragon, Nielk1, & Ken Miller";







// Get a Vector from a Path point. Code from Nielk1. Height is the Terrain height at the position.
extern Vector GetVectorFromPath(const char* path, const int point = 0);






// Overload functions that take const parameters.
inline Handle BuildObject(const char* odf, const int Team, const Handle him) { return BuildObject(const_cast<char *>(odf), Team, him); }
inline Handle BuildObject(const char* odf, const int Team, const char *APath) { return BuildObject(const_cast<char *>(odf), Team, const_cast<Name>(APath)); }
inline Handle BuildObject(const char* odf, const int Team, Name APath) { return BuildObject(const_cast<char *>(odf), Team, APath); }
inline Handle BuildObject(const char* odf, const int Team, const AiPath *APath) { return BuildObject(const_cast<char *>(odf), Team, const_cast<AiPath *>(APath)); }
inline Handle BuildObject(const char* odf, const int Team, const Vector pos) { return BuildObject(const_cast<char *>(odf), Team, const_cast<Vector &>(pos)); }
inline Handle BuildObject(const char* odf, const int Team, const Matrix mat) { return BuildObject(const_cast<char *>(odf), Team, const_cast<Matrix &>(mat)); }
//**SetPilotClass..GetNearestVehicle
inline void Goto(const Handle me, const char* path, const int priority = 1) { Goto(me, const_cast<Name>(path), priority); }
inline void Mine(const Handle me, const char* path, const int priority = 1) { Mine(me, const_cast<Name>(path), priority); }
inline void Patrol(const Handle me, const char* path, const int priority = 1) { Patrol(me, const_cast<Name>(path), priority); }
inline void Retreat(const Handle me, const char* path, const int priority = 1) { Retreat(me, const_cast<Name>(path), priority); }
inline void Dropoff(const Handle me, const char* path, const int priority = 1) { Dropoff(me, const_cast<Name>(path), priority); }
//**SetPosition...Network_SetInteger
inline bool GetPathPoints(const char* path, size_t& bufSize, float* pData) { return GetPathPoints(const_cast<Name>(path), bufSize, pData); }
//inline void GetAiPaths(const int &pathCount, const char* *&pathNames) { GetAiPaths(
inline void SetPosition(const Handle h, const Matrix &m) { SetPosition(h, m); }


// Function Overloads, these are various name overloads for stock functions that take in different types of arguments by default. Does back end fiddly work for you.
//**SetPosition..GetDistance
// Goto that takes a Matrix.
inline void Goto(const Handle h, const Matrix Position, const int Priority = 1) { Goto(h, Position.posit, Priority); }
inline void Goto(const Handle h, const char* Path, const int Point, const int Priority) { Goto(h, GetVectorFromPath(Path, Point), Priority); }
// Command for Mine that take a Handle/Vector/Matrix/Path Point.
//inline void Mine(const Handle h, const Vector Where, const int Priority = 1) { SetCommand(h, CMD_LAY_MINES, Priority, 0, Where); }
inline void Mine(const Handle h, const Matrix Where, const int Priority = 1) { Mine(h, Where.posit, Priority); }
inline void Mine(const Handle me, const Handle him, int Priority = 1) { Mine(me, GetPosition(him), Priority); }
inline void Mine(const Handle h, const char* Path, const int Point, const int Priority) { Mine(h, GetVectorFromPath(Path, Point), Priority); }
// Command for Dropoff that take a Handle/Vector/Matrix/Path Point.
//inline void Dropoff(const Handle h, const Vector Where, const int Priority = 1) { SetCommand(h, CMD_DROPOFF, Priority, 0, Where); }
inline void Dropoff(const Handle h, const Matrix Where, const int Priority = 1) { Dropoff(h, Where.posit, Priority); }
inline void Dropoff(const Handle me, const Handle him, const int Priority = 1) { Dropoff(me, GetPosition(him), Priority); }
inline void Dropoff(const Handle h, const char* Path, const int Point, const int Priority) { Dropoff(h, GetVectorFromPath(Path, Point), Priority); }
// Command for Retreat that take a Vector/Matrix/Path Point.
inline void Retreat(const Handle h, const Vector Where, const int Priority = 1) { SetIndependence(h, 0); Goto(h, Where, Priority); }
inline void Retreat(const Handle h, const Matrix Where, const int Priority = 1) { Retreat(h, Where.posit, Priority); }
inline void Retreat(const Handle h, const char* Path, const int Point, const int Priority) { Retreat(h, GetVectorFromPath(Path, Point), Priority); }
//**GiveWeapon..CameraPath





#endif