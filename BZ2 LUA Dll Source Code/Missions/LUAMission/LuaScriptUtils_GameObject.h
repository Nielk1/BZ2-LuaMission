#ifndef _LuaScriptUtils_GameObject_
#define _LuaScriptUtils_GameObject_

#include "lua.hpp"
#include "..\..\source\fun3d\ScriptUtils.h"
#include "LuaTypeBindings.h"

//DLLEXPORT void DLLAPI SetGroup(Handle h, int group);
static int SetGroup(lua_State *L)
{
	Handle h = RequireHandle(L, 1);
	int group = luaL_optinteger(L, 2, 0);
	SetGroup(h, group);
	return 0;
}

//void Attack(Handle me, Handle him, int priority = 1);
static int Attack(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	Attack(me, him, priority);
	return 0;
}

//DLLEXPORT void DLLAPI Service(Handle me, Handle him, int priority = 1);
static int Service(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	Service(me, him, priority);
	return 0;
}

//void Goto(Handle me, const Vector &pos, int priority = 1);
//void Goto(Handle me, Name path, int priority = 1);
//void Goto(Handle me, Handle him, int priority = 1);
//inline void Goto(Handle h, Matrix Position, int Priority) { Goto(h, Position.posit, Priority); }
static int Goto(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	int priority = luaL_optinteger(L, 3, 1);
	if (Matrix *mat = GetMatrix(L, 2))
	{
		Goto(me, *mat, priority);
	}
	else if (Vector *pos = GetVector(L, 2))
	{
		Goto(me, *pos, priority);
	}
	else if (lua_isstring(L, 2))
	{
		Name path = Name(lua_tostring(L, 2));

		if (lua_isnumber(L, 4))
		{
			int point = luaL_optinteger(L, 3, 0);
			priority = luaL_optinteger(L, 4, 1);
			Goto(me, path, point, priority);
		}
		else
		{
			Goto(me, path, priority);
		}
	}
	else
	{
		Handle him = RequireHandle(L, 2);
		Goto(me, him, priority);
	}
	return 0;
}

//void Mine(Handle me, Name path, int priority = 1);
static int Mine(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	int priority = luaL_optinteger(L, 3, 1);
	///* //!-- BZ2 doesn't support these functions. :( // Function Created, Vector/Matrix versions theoretically untested. :-/
	if (Matrix *mat = GetMatrix(L, 2))
	{
		Mine(me, mat->posit, priority);
	}
	else if (Vector *pos = GetVector(L, 2))
	{
		Mine(me, *pos, priority);
	}
	else if (lua_isstring(L, 2))
	{
		//	*/
		Name path = Name(lua_tostring(L, 2));

		if (lua_isnumber(L, 4))
		{
			int point = luaL_optinteger(L, 3, 0);
			priority = luaL_optinteger(L, 4, 1);
			Mine(me, path, point, priority);
		}
		else
		{
			Mine(me, path, priority);
		}
	}
	else
	{
		Handle him = RequireHandle(L, 2);
		Mine(me, him, priority);
	}
	return 0;
}

//void Follow(Handle me, Handle him, int priority = 1);
static int Follow(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	Follow(me, him, priority);
	return 0;
}

//void Defend(Handle me, int priority = 1);
static int Defend(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	int priority = luaL_optinteger(L, 2, 1);
	Defend(me, priority);
	return 0;
}

//void Defend2(Handle me, Handle him, int priority = 1);
static int Defend2(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	Defend2(me, him, priority);
	return 0;
}

//void Stop(Handle me, int priority = 1);
static int Stop(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	int priority = luaL_optinteger(L, 2, 1);
	Stop(me, priority);
	return 0;
}

//void Patrol(Handle me, Name path, int priority = 1);
static int Patrol(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Name path = Name(luaL_checkstring(L, 2));
	int priority = luaL_optinteger(L, 3, 1);
	Patrol(me, path, priority);
	return 0;
}

//void Retreat(Handle me, Name path, int priority = 1);
//void Retreat(Handle me, Handle him, int priority = 1);
// Vector + Matrix improvised with Goto/Independence.
static int Retreat(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	int priority = luaL_optinteger(L, 3, 1);
	if (Matrix *mat = GetMatrix(L, 2))
	{
		Retreat(me, *mat, priority);
	}
	else if (Vector *pos = GetVector(L, 2))
	{
		Retreat(me, *pos, priority);
	}
	else if (lua_isstring(L, 2))
	{
		Name path = Name(lua_tostring(L, 2));

		if (lua_isnumber(L, 4))
		{
			int point = luaL_optinteger(L, 3, 0);
			priority = luaL_optinteger(L, 4, 1);
			Retreat(me, path, point, priority);
		}
		else
		{
			Retreat(me, path, priority);
		}
	}
	else
	{
		Handle him = RequireHandle(L, 2);
		Retreat(me, him, priority);
	}
	return 0;
}

//void GetIn(Handle me, Handle him, int priority = 1);
static int GetIn(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	GetIn(me, him, priority);
	return 0;
}

//void Pickup(Handle me, Handle him, int priority = 1);
static int Pickup(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	Pickup(me, him, priority);
	return 0;
}

//void Dropoff(Handle me, const Matrix &mat, int priority = 1);
//void Dropoff(Handle me, const Vector &pos, int priority = 1);
//void Dropoff(Handle me, Name path, int priority = 1);
static int Dropoff(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	int priority = luaL_optinteger(L, 3, 1);

	///* //!-- BZ2 doesn't have this fucntion // Vector/Matrix versions created, but theoretically untested.
	if (Matrix *mat = GetMatrix(L, 2))
	{
		Dropoff(me, *mat, priority);
	}
	else if (Vector *pos = GetVector(L, 2))
	{
		Dropoff(me, *pos, priority);
	}
	else if (lua_isstring(L, 2))
	{
		//*/
		Name path = Name(luaL_checkstring(L, 2));

		if (lua_isnumber(L, 4))
		{
			int point = luaL_optinteger(L, 3, 0);
			priority = luaL_optinteger(L, 4, 1);
			Dropoff(me, path, point, priority);
		}
		else
		{
			Dropoff(me, path, priority);
		}
	}
	else
	{
		Handle him = RequireHandle(L, 2);
		Dropoff(me, him, priority);
	}
	return 0;
}

//void Build(Handle me, char *odf, int priority = 1);
static int Build(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	char *odf = const_cast<char *>(luaL_checkstring(L, 2));
	int priority = luaL_optinteger(L, 3, 1);
	Build(me, odf, priority);
	return 0;
}

//DLLEXPORT void DLLAPI LookAt(Handle me, Handle him, int priority = 1);
static int LookAt(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	LookAt(me, him, priority);
	return 0;
}

// Lua script utils functions
static const luaL_Reg sLuaScriptUtils_GameObject[] = {
	{ "SetGroup", SetGroup },
	{ "Attack", Attack },
	{ "Service", Service },
	{ "Goto", Goto },
	{ "Mine", Mine },
	{ "Follow", Follow },
	{ "Defend", Defend },
	{ "Defend2", Defend2 },
	{ "Stop", Stop },
	{ "Patrol", Patrol },
	{ "Retreat", Retreat },
	{ "GetIn", GetIn },
	{ "Pickup", Pickup },
	{ "Dropoff", Dropoff },
	{ "Build", Build },
	{ "LookAt", LookAt },
	{ NULL, NULL }
};

#endif