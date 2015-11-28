#ifndef _LuaScriptUtils_GameObject_
#define _LuaScriptUtils_GameObject_

#include "lua.hpp"
#include "..\..\source\fun3d\ScriptUtils.h"
#include "LuaTypeBindings.h"


//Handle BuildObject(char *odf, int team, Handle h);
//Handle BuildObject(char *odf, int team, Name path, int point = 0)
//Handle BuildObject(char *odf, int team, Vector v);
//Handle BuildObject(char *odf, int team, Matrix m);
static int BuildObject(lua_State *L)
{
	char *odf = const_cast<char *>(luaL_checkstring(L, 1));
	int team = luaL_checkinteger(L, 2);
	Handle o;
	if (Matrix *mat = GetMatrix(L, 3))
	{
		o = BuildObject(odf, team, *mat); // ScriptUtils
	}
	else if (Vector *pos = GetVector(L, 3))
	{
		o = BuildObject(odf, team, *pos); // ScriptUtils
	}
	else if (lua_isstring(L, 3))
	{
		Name path = Name(lua_tostring(L, 3));
		int point = luaL_optinteger(L, 4, 0);
		if (point)
		{
			Vector pos = GetVectorFromPath(path, point); // ScriptUtilsExtension
			o = BuildObject(odf, team, pos); // ScriptUtils
		}
		else
		{
			o = BuildObject(odf, team, path); // ScriptUtils
		}
	}
	else
	{
		Handle h = RequireHandle(L, 3);
		o = BuildObject(odf, team, h);
	}
	PushHandle(L, o);
	return 1;
}

// void RemoveObject(Handle h)
static int RemoveObject(lua_State *L)
{
	Handle h = RequireHandle(L, 1);
	RemoveObject(h); // ScriptUtils
	return 0;
}

//DLLEXPORT void DLLAPI SetGroup(Handle h, int group);
static int SetGroup(lua_State *L)
{
	Handle h = RequireHandle(L, 1);
	int group = luaL_optinteger(L, 2, 0);
	SetGroup(h, group); // ScriptUtils
	return 0;
}

//void Attack(Handle me, Handle him, int priority = 1);
static int Attack(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	Attack(me, him, priority); // ScriptUtils
	return 0;
}

//DLLEXPORT void DLLAPI Service(Handle me, Handle him, int priority = 1);
static int Service(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	Service(me, him, priority); // ScriptUtils
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
		Goto(me, *mat, priority); // ScriptUtilsExtensions
	}
	else if (Vector *pos = GetVector(L, 2))
	{
		Goto(me, *pos, priority); // ScriptUtils
	}
	else if (lua_isstring(L, 2))
	{
		Name path = Name(lua_tostring(L, 2));

		if (lua_isnumber(L, 4))
		{
			int point = luaL_optinteger(L, 3, 0);
			priority = luaL_optinteger(L, 4, 1);
			Goto(me, path, point, priority); // ScriptUtilsExtensions
		}
		else
		{
			Goto(me, path, priority); // ScriptUtils
		}
	}
	else
	{
		Handle him = RequireHandle(L, 2);
		Goto(me, him, priority); // ScriptUtils
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
		Mine(me, mat->posit, priority); // ScriptUtils
	}
	else if (Vector *pos = GetVector(L, 2))
	{
		Mine(me, *pos, priority); // ScriptUtils
	}
	else if (lua_isstring(L, 2))
	{
		//	*/
		Name path = Name(lua_tostring(L, 2));

		if (lua_isnumber(L, 4))
		{
			int point = luaL_optinteger(L, 3, 0);
			priority = luaL_optinteger(L, 4, 1);
			Mine(me, path, point, priority); // ScriptUtilsExtensions
		}
		else
		{
			Mine(me, path, priority); // ScriptUtils
		}
	}
	else
	{
		Handle him = RequireHandle(L, 2);
		Mine(me, him, priority); // ScriptUtilsExtensions
	}
	return 0;
}

//void Follow(Handle me, Handle him, int priority = 1);
static int Follow(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	Follow(me, him, priority); // ScriptUtils
	return 0;
}

//void Defend(Handle me, int priority = 1);
static int Defend(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	int priority = luaL_optinteger(L, 2, 1);
	Defend(me, priority); // ScriptUtils
	return 0;
}

//void Defend2(Handle me, Handle him, int priority = 1);
static int Defend2(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	Defend2(me, him, priority); // ScriptUtils
	return 0;
}

//void Stop(Handle me, int priority = 1);
static int Stop(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	int priority = luaL_optinteger(L, 2, 1);
	Stop(me, priority); // ScriptUtils
	return 0;
}

//void Patrol(Handle me, Name path, int priority = 1);
static int Patrol(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Name path = Name(luaL_checkstring(L, 2));
	int priority = luaL_optinteger(L, 3, 1);
	Patrol(me, path, priority); // ScriptUtils
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
		Retreat(me, *mat, priority); // ScriptUtilsExtensions
	}
	else if (Vector *pos = GetVector(L, 2))
	{
		Retreat(me, *pos, priority); // ScriptUtilsExtensions
	}
	else if (lua_isstring(L, 2))
	{
		Name path = Name(lua_tostring(L, 2));

		if (lua_isnumber(L, 4))
		{
			int point = luaL_optinteger(L, 3, 0);
			priority = luaL_optinteger(L, 4, 1);
			Retreat(me, path, point, priority); // ScriptUtilsExtensions
		}
		else
		{
			Retreat(me, path, priority); // ScriptUtils
		}
	}
	else
	{
		Handle him = RequireHandle(L, 2);
		Retreat(me, him, priority); // ScriptUtils
	}
	return 0;
}

//void GetIn(Handle me, Handle him, int priority = 1);
static int GetIn(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	GetIn(me, him, priority); // ScriptUtils
	return 0;
}

//void Pickup(Handle me, Handle him, int priority = 1);
static int Pickup(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	Pickup(me, him, priority); // ScriptUtils
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
		Dropoff(me, *mat, priority); // ScriptUtilsExtension
	}
	else if (Vector *pos = GetVector(L, 2))
	{
		Dropoff(me, *pos, priority); // ScriptUtils
	}
	else if (lua_isstring(L, 2))
	{
		//*/
		Name path = Name(luaL_checkstring(L, 2));

		if (lua_isnumber(L, 4))
		{
			int point = luaL_optinteger(L, 3, 0);
			priority = luaL_optinteger(L, 4, 1);
			Dropoff(me, path, point, priority); // ScriptUtilsExtension
		}
		else
		{
			Dropoff(me, path, priority); // ScriptUtils
		}
	}
	else
	{
		Handle him = RequireHandle(L, 2);
		Dropoff(me, him, priority); // ScriptUtilsExtension
	}
	return 0;
}

//void Build(Handle me, char *odf, int priority = 1);
static int Build(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	char *odf = const_cast<char *>(luaL_checkstring(L, 2));
	int priority = luaL_optinteger(L, 3, 1);
	Build(me, odf, priority); // ScriptUtils
	return 0;
}

//DLLEXPORT void DLLAPI LookAt(Handle me, Handle him, int priority = 1);
static int LookAt(lua_State *L)
{
	Handle me = RequireHandle(L, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	LookAt(me, him, priority); // ScriptUtils
	return 0;
}

//DLLEXPORT void DLLAPI AllLookAt(int team, Handle him, int priority = 1);
static int AllLookAt(lua_State *L)
{
	int team = luaL_optinteger(L, 1, 1);
	Handle him = RequireHandle(L, 2);
	int priority = luaL_optinteger(L, 3, 1);
	AllLookAt(team, him, priority); // ScriptUtils
	return 0;
}

// Lua script utils functions
static const luaL_Reg sLuaScriptUtils_GameObject[] = {
	// BZ2 Script Utils Functions.
	{ "BuildObject", BuildObject },
	{ "RemoveObject", RemoveObject },
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
	{ "AllLookAt", LookAt },
	{ NULL, NULL }
};

#endif