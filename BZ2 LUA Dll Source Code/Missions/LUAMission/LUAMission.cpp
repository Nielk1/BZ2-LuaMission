#include <iostream>
#include <stdlib.h>
#include <signal.h>

#include <sstream>

#include "lua.hpp"
//#include "lualib.h"
//#include "lauxlib.h"
#include "..\..\Source\fun3d\ScriptUtils.h" // This file does not follow the C/C++ standards, uncompilable in modern compiler

/*typedef char* Name;
typedef int Handle;
typedef int TeamNum;
typedef float Time;
typedef float Dist;
typedef int ScrapValue;
typedef unsigned long DWORD;
typedef DWORD DPID;
enum EjectKillRetCodes {
	DoEjectPilot, // Do 'standard' eject
	DoRespawnSafest, // Respawn a 'PLAYER' at safest spawnpoint
	DLLHandled, // DLL handled actions. Do nothing ingame
	DoGameOver, // Game over, man.
};*/

EjectKillRetCodes convertNumberToEjectKillRetCodes(lua_Number num)
{
	if(num <= 0.1) return DoEjectPilot;
	if(num <= 1.1) return DoRespawnSafest;
	if(num <= 2.1) return DLLHandled;
	return DoGameOver;
}

//#define DLLAPI __cdecl
//#define DLLAPI __attribute__ ((dllexport))
//#define DLLAPI







	int Lua_FailMission (lua_State *L) {
		Time t = static_cast<Time>(luaL_checknumber(L, 1)); // forces abort on fail
		if (lua_type(L, 2) != LUA_TNONE) // do we have a 2nd param?
		{
			size_t len;
			const char *tmpFileName = luaL_checklstring(L, 2, &len); // forces abort on fail
			char *fileName = new char[len+1];
			fileName[len] = 0;
			memcpy(fileName,tmpFileName,len);
			misnImport.FailMission(t, fileName);
			delete [] fileName;
			return 0;
		}
		misnImport.FailMission(t, NULL);
		return 0;
	}

	int Lua_SucceedMission (lua_State *L) {
		Time t = static_cast<Time>(luaL_checknumber(L, 1)); // forces abort on fail
		if (lua_type(L, 2) != LUA_TNONE) // do we have a 2nd param?
		{
			size_t len;
			const char *tmpFileName = luaL_checklstring(L, 2, &len); // forces abort on fail
			char *fileName = new char[len+1];
			fileName[len] = 0;
			memcpy(fileName,tmpFileName,len);
			misnImport.SucceedMission(t, fileName);
			delete [] fileName;
			return 0;
		}
		misnImport.SucceedMission(t, NULL);
		return 0;
	}

	int Lua_ChangeSide (lua_State *L) {
		(void)(L); /*unused*/
		misnImport.ChangeSide();
		return 0;
	}

	int Lua_AddScrap (lua_State *L) {
		TeamNum t = luaL_checkinteger(L, 1);
		ScrapValue v = luaL_checkinteger(L, 2);
		ScrapValue retVal = misnImport.AddScrap(t,v);
		lua_pushinteger(L, retVal);
		return 1;
	}

	int Lua_SetScrap (lua_State *L) {
		TeamNum t = luaL_checkinteger(L, 1);
		ScrapValue v = luaL_checkinteger(L, 2);
		ScrapValue retVal = misnImport.SetScrap(t,v);
		lua_pushinteger(L, retVal);
		return 1;
	}

	int Lua_GetScrap (lua_State *L) {
		TeamNum t = luaL_checkinteger(L, 1);
		ScrapValue retVal = misnImport.GetScrap(t);
		lua_pushinteger(L, retVal);
		return 1;
	}

	int Lua_GetMaxScrap (lua_State *L) {
		TeamNum t = luaL_checkinteger(L, 1);
		ScrapValue retVal = misnImport.GetMaxScrap(t);
		lua_pushinteger(L, retVal);
		return 1;
	}

	int Lua_GetTug(lua_State *L) {
			Handle h = luaL_checkinteger(L, 1);
			Handle tug = misnImport.GetTug(h);
			lua_pushinteger(L, tug);
			return 1;
		}

	int Lua_HasCargo(lua_State *L) {
		Handle h = luaL_checkinteger(L, 1);
		bool hasCargo = misnImport.HasCargo(h);
		lua_pushboolean(L, hasCargo);
		return 1;
	}

	int Lua_GetDistanceObject(lua_State *L) {
		Handle h = luaL_checkinteger(L, 1);
		Handle h2 = luaL_checkinteger(L, 2);
		Dist distance = misnImport.GetDistanceObject(h, h2);
		lua_pushnumber(L, distance);
		return 1;
	}

	int Lua_GetDistancePath(lua_State *L) {
		Handle h = luaL_checkinteger(L, 1);
		
		size_t len;
		const char *tmp_path = luaL_checklstring(L, 2, &len);
		char *path = new char[len+1];
		path[len] = 0;
		memcpy(path,tmp_path,len);

		int point = luaL_checkinteger(L, 2);
		Dist distance = misnImport.GetDistancePath(h, path, point);

		delete [] path;

		lua_pushnumber(L, distance);
		return 1;
	}

	int Lua_GetNearestObject(lua_State *L) {
		Handle h = luaL_checkinteger(L, 1);
		Handle h2 = misnImport.GetNearestObject(h);
		lua_pushinteger(L, h2);
		return 1;
	}

	int Lua_GetNearestVehicleObject(lua_State *L) {
		Handle h = luaL_checkinteger(L, 1);
		Handle h2 = misnImport.GetNearestVehicleObject(h);
		lua_pushinteger(L, h2);
		return 1;
	}

	int Lua_GetNearestVehiclePath(lua_State *L) {
		size_t len;
		const char *tmp_path = luaL_checklstring(L, 1, &len);
		char *path = new char[len+1];
		path[len] = 0;
		memcpy(path,tmp_path,len);

		int point = luaL_checkinteger(L, 2);
		Handle h2 = misnImport.GetNearestVehiclePath(path, point);

		delete [] path;

		lua_pushinteger(L, h2);
		return 1;
	}

	int Lua_GetNearestBuilding(lua_State *L) {
		Handle h = luaL_checkinteger(L, 1);
		Handle h2 = misnImport.GetNearestBuilding(h);
		lua_pushinteger(L, h2);
		return 1;
	}

	int Lua_GetNearestEnemy(lua_State *L) {
		Handle h = luaL_checkinteger(L, 1);
		Handle h2 = misnImport.GetNearestEnemy(h);
		lua_pushinteger(L, h2);
		return 1;
	}


	static const luaL_Reg MisnImportLib[] = {
		{"FailMission", Lua_FailMission},
		{"SucceedMission", Lua_SucceedMission},
		{"ChangeSide", Lua_ChangeSide},
		{"AddScrap", Lua_AddScrap},
		{"SetScrap", Lua_SetScrap},
		{"GetScrap", Lua_GetScrap},
		{"GetMaxScrap", Lua_GetMaxScrap},
		{"GetTug", Lua_GetTug},
		{"HasCargo", Lua_HasCargo},
		{"GetDistanceObject", Lua_GetDistanceObject},
		{"GetDistancePath", Lua_GetDistancePath},
		//{"GetDistancePathPtr", Lua_GetDistancePathPtr},
		{"GetNearestObject", Lua_GetNearestObject},
		{"GetNearestVehicleObject", Lua_GetNearestVehicleObject},
		{"GetNearestVehiclePath", Lua_GetNearestVehiclePath},
		{"GetNearestBuilding", Lua_GetNearestBuilding},
		{"GetNearestEnemy", Lua_GetNearestEnemy},
		{0,0}
	};


static int Lua_LoadFromBZ2(lua_State *L) {
	size_t len;
	const char *tmpFileName = luaL_checklstring(L, 1, &len); // forces abort on fail
	char *fileName = new char[len+1];
	fileName[len] = 0;
	memcpy(fileName,tmpFileName,len);

		void *pData;
		size_t bufSize = 0;
		LoadFile(fileName, NULL, bufSize);
		pData = malloc(bufSize+1);
		((char*)pData)[bufSize] = 0; // null terminator
		LoadFile(fileName, pData, bufSize);
		int status = luaL_dostring(L,(char*)pData);
		free(pData);
	
	
	delete [] fileName;
	return 0;
}



	static const luaL_Reg CoreLib[] = {
		{"LoadFromBZ2", Lua_LoadFromBZ2},
		{0,0}
	};

int report(lua_State *L, int status)
{
	if(status!=0)
	{
		std::cerr << "-- " << lua_tostring(L, -1) << std::endl;

		char *message = new char[1024];
		strcpy_s(message,1024,lua_tostring(L, -1));
		PrintConsoleMessage(message);

		lua_pop(L, 1); // remove error message
	}
	return status;
}



static lua_State *globalL = NULL;
static const char *progname = "luajitexamplifying";
static void lstop(lua_State *L, lua_Debug *ar)
{
	(void)ar;  /* unused arg. */
	lua_sethook(L, NULL, 0, 0);
	/* Avoid luaL_error -- a C hook doesn't add an extra frame. */
	luaL_where(L, 0);
	lua_pushfstring(L, "%sinterrupted!", lua_tostring(L, -1));
	lua_error(L);
}
static void __cdecl laction(int i)
{
	signal(i, SIG_DFL); /* if another SIGINT happens before lstop,
	terminate process (default action) */
	lua_sethook(globalL, lstop, LUA_MASKCALL | LUA_MASKRET | LUA_MASKCOUNT, 1);
}
static int traceback(lua_State *L)
{
	if (!lua_isstring(L, 1)) { /* Non-string error object? Try metamethod. */
		if (lua_isnoneornil(L, 1) ||
			!luaL_callmeta(L, 1, "__tostring") ||
			!lua_isstring(L, -1))
			return 1;  /* Return non-string error object. */
			lua_remove(L, 1);  /* Replace object by result of __tostring metamethod. */
	}
	luaL_traceback(L, L, lua_tostring(L, 1), 1);
	return 1;
}


static int call_void_void(lua_State *L, const char *fname)
{
	lua_getfield(L, LUA_GLOBALSINDEX, fname);
	return lua_pcall(L, 0, 0, 0);
}

static int call_void_number(lua_State *L, const char *fname, lua_Number arg0)
{
	lua_getfield(L, LUA_GLOBALSINDEX, fname);
	lua_pushnumber(L, arg0);
	return lua_pcall(L, 1, 0, 0);
}

static int call_number_number(lua_State *L, const char *fname, lua_Number *ret, lua_Number arg0)
{
	lua_getfield(L, LUA_GLOBALSINDEX, fname);
	lua_pushnumber(L, arg0);
	int status = lua_pcall(L, 1, 1, 0);
	if(status==0)
	{
		*ret = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	return status;
}

static int call_number_number_number(lua_State *L, const char *fname, lua_Number *ret, lua_Number arg0, lua_Number arg1)
{
	lua_getfield(L, LUA_GLOBALSINDEX, fname);
	lua_pushnumber(L, arg0);
	lua_pushnumber(L, arg1);
	int status = lua_pcall(L, 2, 1, 0);
	if(status==0)
	{
		*ret = lua_tonumber(L, -1);
		lua_pop(L, 1);
	}
	return status;
}

static std::string call_string_number_cache("");
static int call_string_number(lua_State *L, const char *fname, char **ret, lua_Number arg0)
{
	lua_getfield(L, LUA_GLOBALSINDEX, fname);
	lua_pushnumber(L, arg0);
	int status = lua_pcall(L, 1, 1, 0);
	if(status==0)
	{
		call_string_number_cache = lua_tostring(L, -1);
		*ret = const_cast<char*>(call_string_number_cache.c_str());
		lua_pop(L, 1);
	}
	else call_string_number_cache = "";
	return status;
}

static int call_bool_bool(lua_State *L, const char *fname, bool *ret, bool arg0)
{
	lua_getfield(L, LUA_GLOBALSINDEX, fname);
	lua_pushboolean(L, arg0);
	int status = lua_pcall(L, 1, 1, 0);
	if(status==0)
	{
		*ret = lua_toboolean(L, -1);
		lua_pop(L, 1);
	}
	return status;
}

static int call_bool_number_number_bool(lua_State *L, const char *fname, bool *ret, lua_Number arg0, lua_Number arg1, bool arg2)
{
	lua_getfield(L, LUA_GLOBALSINDEX, fname);
	lua_pushnumber(L, arg0);
	lua_pushnumber(L, arg1);
	lua_pushboolean(L, arg2);
	int status = lua_pcall(L, 3, 1, 0);
	if(status==0)
	{
		*ret = lua_toboolean(L, -1);
		lua_pop(L, 1);
	}
	return status;
}

static int docall(lua_State *L, int narg, int clear)
{
	int status;
	int base = lua_gettop(L) - narg;  /* function index */
	lua_pushcfunction(L, traceback);  /* push traceback function */
	lua_insert(L, base);  /* put it under chunk and args */
	#if !LJ_TARGET_CONSOLE
	signal(SIGINT, laction);
	#endif
	status = lua_pcall(L, narg, (clear ? 0 : LUA_MULTRET), base);
	#if !LJ_TARGET_CONSOLE
	signal(SIGINT, SIG_DFL);
	#endif
	lua_remove(L, base);  /* remove traceback function */
	/* force a complete garbage collection in case of errors */
	if (status != 0) lua_gc(L, LUA_GCCOLLECT, 0);
	return status;
}

#if 0
static void stackDump (lua_State *L) {
	std::ostringstream stringStream;
	
	char buffer [50];

	int i;
	int top = lua_gettop(L);
	for (i = 1; i <= top; i++) {  /* repeat for each level */
		int t = lua_type(L, i);
		switch (t) {
	
		case LUA_TSTRING:  /* strings */
			memset(buffer,0,50);
			sprintf_s(buffer,"'%s'", lua_tostring(L, i));
			stringStream << buffer;
			break;
	
		case LUA_TBOOLEAN:  /* booleans */
			memset(buffer,0,50);
			sprintf_s(buffer,"%s", lua_toboolean(L, i) ? "true" : "false");
			stringStream << buffer;
			break;
	
		case LUA_TNUMBER:  /* numbers */
			memset(buffer,0,50);
			sprintf_s(buffer,"%g", lua_tonumber(L, i));
			stringStream << buffer;
			break;
	
		default:  /* other values */
			memset(buffer,0,50);
			sprintf_s(buffer,"%s", lua_typename(L, t));
			stringStream << buffer;
			break;
	
		}
		memset(buffer,0,50);
		sprintf_s(buffer,"  ");  /* put a separator */
		stringStream << buffer;
	}
	memset(buffer,0,50);
	sprintf_s(buffer,"\n");  /* end the listing */
	stringStream << buffer;

	std::string copyOfStr = stringStream.str();

		char *message = new char[1024];
		strcpy_s(message,1024,copyOfStr.c_str());
		PrintConsoleMessage(message);
}
#endif

static int pSetup(lua_State *L)
{

	lua_gc(L, LUA_GCSTOP, 0);  /* stop collector during initialization */
	luaL_openlibs(L);  /* open libraries */
	lua_gc(L, LUA_GCRESTART, -1);
	
	int status = luaL_loadfile(L, "_api.lua");

	/*void *pData;
	size_t bufSize = 0;
	LoadFile("_api.lua", NULL, bufSize);
	pData = malloc(bufSize+1);
	((char*)pData)[bufSize] = 0; // null terminator
	LoadFile("_api.lua", pData, bufSize);
	int status = luaL_dostring(L,(char*)pData);
	free(pData);*/

	if(report(L, status)) return status;

	return docall(L, 0, 1);
}

lua_State *L = NULL;

// This makes some assumptions, if any of these are false then correct this:
// * InitialSetup will only be called once in a mission
// * It assumes that it is a possible that this DLL is not unloaded between missions
void DLLAPI InitialSetup(void)
{
	IFace_ConsoleCmd("console.log 1");

	LUAJIT_VERSION_SYM();
	
	int status;

	if(L != NULL)
	{
		lua_close(L);
		L = NULL;
	}
	
	L = lua_open();
	if(L == NULL) return;
	
	//LuaScriptUtils::luaopen_ScriptUtils(L);
	luaL_register(L, "MisnImport", MisnImportLib);

	luaL_register(L, "Core", CoreLib);

	status = lua_cpcall(L, pSetup, NULL);

	//stackDump(L);
	//char *message = new char[1024];
	//sprintf_s(message,1024,"File Run Status: %d",status);
	//PrintConsoleMessage(message);

	if(report(L, status)) return; // TODO: So need to do proper error checking...

	report(L, call_void_void(L, "InitialSetup"));
}

bool DLLAPI Save(bool misnSave)
{
	bool ret = false;
	report(L, call_bool_bool(L, "Save", &ret, misnSave));
	return ret;
}

bool DLLAPI Load(bool misnSave)
{
	bool ret = false;
	report(L, call_bool_bool(L, "Load", &ret, misnSave));
	return ret;
}

bool DLLAPI PostLoad(bool misnSave)
{
	bool ret = false;
	report(L, call_bool_bool(L, "PostLoad", &ret, misnSave));
	return ret;
}

void DLLAPI AddObject(Handle h)
{
	report(L, call_void_number(L, "AddObject", h));
}

void DLLAPI DeleteObject(Handle h)
{
	report(L, call_void_number(L, "DeleteObject", h));
}

void DLLAPI Update(void)
{
	report(L, call_void_void(L, "Update"));
	/*int status = call_void_void(L, "Update");
	stackDump(L);

		char *message = new char[1024];
		//strcpy_s(message,1024,copyOfStr.c_str());
		sprintf_s(message,1024,"Update Status: %d",status);
		PrintConsoleMessage(message);

	report(L, status);*/
}

void DLLAPI PostRun(void)
{
	report(L, call_void_void(L, "PostRun"));
	lua_close(L);
	L = NULL;
}

bool DLLAPI AddPlayer(DPID id, int Team, bool ShouldCreateThem)
{
	bool ret = false;
	report(L, call_bool_number_number_bool(L, "AddPlayer", &ret, id, Team, ShouldCreateThem));
	return ret;
}

void DLLAPI DeletePlayer(DPID id)
{
	report(L, call_void_number(L, "DeletePlayer", id));
}

EjectKillRetCodes DLLAPI PlayerEjected(Handle DeadObjectHandle)
{
	lua_Number ret = 256;
	report(L, call_number_number(L, "PlayerEjected", &ret, DeadObjectHandle));
	return convertNumberToEjectKillRetCodes(ret);
}

EjectKillRetCodes DLLAPI ObjectKilled(int DeadObjectHandle, int KillersHandle)
{
	lua_Number ret = 256;
	report(L, call_number_number_number(L, "ObjectKilled", &ret, DeadObjectHandle, KillersHandle));
	return convertNumberToEjectKillRetCodes(ret);
}

EjectKillRetCodes DLLAPI ObjectSniped(int DeadObjectHandle, int KillersHandle)
{
	lua_Number ret = 256;
	report(L, call_number_number_number(L, "ObjectSniped", &ret, DeadObjectHandle, KillersHandle));
	return convertNumberToEjectKillRetCodes(ret);
}

char * DLLAPI GetNextRandomVehicleODF(int Team)
{
	char *ret = const_cast<char*>("");
	report(L, call_string_number(L, "GetNextRandomVehicleODF", &ret, Team));
	return ret;
}

void DLLAPI SetWorld(int nextWorld)
{
	report(L, call_void_number(L, "SetWorld", nextWorld));
}

void DLLAPI ProcessCommand(unsigned long crc)
{
	report(L, call_void_number(L, "ProcessCommand", crc));
}

void DLLAPI SetRandomSeed(unsigned long seed)
{
	report(L, call_void_number(L, "SetRandomSeed", seed));
}


 static MisnExport misnExport;
 MisnImport misnImport;
 
 MisnExport * __cdecl GetMisnAPI(MisnImport *import)
 {
 	misnImport = *import;
 	memset(&misnExport, 0, sizeof(misnExport));
 	misnExport.version = LATEST_DLL_VERSION;
 	misnExport.VersionModifier = LATEST_DLL_VERSION_MODIFIER;
 	misnExport.misnImport = &misnImport;
 	misnExport.InitialSetup= InitialSetup;
 	misnExport.Save = Save;
 	misnExport.Load = Load;
 	misnExport.PostLoad = PostLoad;
 	misnExport.AddObject = AddObject;
 	misnExport.DeleteObject = DeleteObject;
 	misnExport.Update = Update;
 	misnExport.PostRun = PostRun;
 	misnExport.AddPlayer = AddPlayer;
 	misnExport.DeletePlayer = DeletePlayer;
 	misnExport.PlayerEjected = PlayerEjected;
 	misnExport.ObjectKilled = ObjectKilled;
 	misnExport.ObjectSniped = ObjectSniped;
 	misnExport.GetNextRandomVehicleODF = GetNextRandomVehicleODF;
 	misnExport.SetWorld = SetWorld;
 	misnExport.ProcessCommand = ProcessCommand;
 	misnExport.SetRandomSeed = SetRandomSeed;
 	//CurrentWorld = 0;
 	return &misnExport;
 }
