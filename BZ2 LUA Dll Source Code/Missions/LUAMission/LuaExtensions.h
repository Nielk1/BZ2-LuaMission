#ifndef _LuaExtensions_
#define _LuaExtensions_

#include "lua.hpp"

#if !defined LUA_VERSION_NUM || LUA_VERSION_NUM==501
/*
** Adapted from Lua 5.2.0
*/
#define lua_pushglobaltable(L) lua_pushvalue(L, LUA_GLOBALSINDEX)

extern "C" static void luaL_setfuncs(lua_State *L, const luaL_Reg *l, int nup) {
	luaL_checkstack(L, nup, "too many upvalues");
	for (; l->name != NULL; l++) {  /* fill the table with given functions */
		int i;
		for (i = 0; i < nup; i++)  /* copy upvalues to the top */
			lua_pushvalue(L, -nup);
		lua_pushstring(L, l->name);
		lua_pushcclosure(L, l->func, nup);  /* closure with those upvalues */
		lua_settable(L, -(nup + 3));
	}
	lua_pop(L, nup);  /* remove upvalues */
}

extern "C" void *luaL_testudata(lua_State *L, int ud, const char *tname) {
	void *p = lua_touserdata(L, ud);
	if (p != NULL) {  /* value is a userdata? */
		if (lua_getmetatable(L, ud)) {  /* does it have a metatable? */
			luaL_getmetatable(L, tname);  /* get correct metatable */
			if (!lua_rawequal(L, -1, -2))  /* not the same? */
				p = NULL;  /* value is a userdata with wrong metatable */
			lua_pop(L, 2);  /* remove both metatables */
			return p;
		}
	}
	return NULL;  /* value is not a userdata with a metatable */
}
#endif

// Optional Boolean.
static bool luaL_optboolean(lua_State *L, int n, int defval)
{
	return luaL_opt(L, lua_toboolean, n, defval);
}

// copy of load_aux from lbaselib.c so we can use it
static int load_aux(lua_State *L, int status) {
	if (status == 0)  /* OK? */
		return 1;
	else {
		lua_pushnil(L);
		lua_insert(L, -2);  /* put before error message */
		return 2;  /* return nil plus error message */
	}
}


static const int sentinel_ = 0;
#define sentinel        ((void *)&sentinel_)
static int Lua_LoadFromBZ2(lua_State *L) {
	//stackDump(L);
	size_t len;
	const char *name = luaL_checklstring(L, 1, &len);

	//const char *name = luaL_checkstring(L, 1);
	int i;
	lua_settop(L, 1);  /* _LOADED table will be at index 2 */
	lua_getfield(L, LUA_REGISTRYINDEX, "_LOADED");
	lua_getfield(L, 2, name);
	if (lua_toboolean(L, -1)) {  /* is it there? */
		if (lua_touserdata(L, -1) == sentinel)  /* check loops */
			luaL_error(L, "loop or previous error loading module " LUA_QS, name);
		return 1;  /* package is already loaded */
	}
	//	/* else must load it; iterate over available loaders */
	//	lua_getfield(L, LUA_ENVIRONINDEX, "loaders");
	//	if (!lua_istable(L, -1))
	//		luaL_error(L, LUA_QL("package.loaders") " must be a table");
	//	lua_pushliteral(L, "");  /* error message accumulator */
	//	for (i = 1;; i++) {
	//		lua_rawgeti(L, -2, i);  /* get a loader */
	//		if (lua_isnil(L, -1))
	//			luaL_error(L, "module " LUA_QS " not found:%s",
	//			name, lua_tostring(L, -2));
	//		lua_pushstring(L, name);
	//		lua_call(L, 1, 1);  /* call it */
	//		if (lua_isfunction(L, -1))  /* did it find module? */
	//			break;  /* module loaded successfully */
	//		else if (lua_isstring(L, -1))  /* loader returned error message? */
	//			lua_concat(L, 2);  /* accumulate it */
	//		else
	//			lua_pop(L, 1);
	//	}

	{
		char *fileName = new char[len + 1];
		fileName[len] = 0;
		memcpy(fileName, name, len);

		char* FileBuffer;
		size_t bufSize = 0;
		bool loadedFile = LoadFile(fileName, NULL, bufSize);
		if (loadedFile || bufSize > 0) {
			FileBuffer = static_cast<char *>(malloc(bufSize + 1));
			loadedFile = LoadFile(fileName, FileBuffer, bufSize);
			FileBuffer[bufSize] = '\0';
			if (loadedFile) {
				//if(LuaCheckStatus(luaL_loadbuffer(L, FileBuffer, bufSize, fileName), L, "Lua script load error: %s"))
				//	LuaCheckStatus(lua_pcall(L, 0, LUA_MULTRET, 0), L, "Lua script run error: %s");
				load_aux(L, luaL_loadbuffer(L, FileBuffer, bufSize, fileName));
			}
			free(FileBuffer);
		}

		delete[] fileName;
	}

	lua_pushlightuserdata(L, sentinel);
	lua_setfield(L, 2, name);  /* _LOADED[name] = sentinel */
	lua_pushstring(L, name);  /* pass name as argument to module */
	lua_call(L, 1, 1);  /* run loaded module */
	if (!lua_isnil(L, -1))  /* non-nil return? */
		lua_setfield(L, 2, name);  /* _LOADED[name] = returned value */
	lua_getfield(L, 2, name);
	if (lua_touserdata(L, -1) == sentinel) {   /* module did not set a value? */
		lua_pushboolean(L, 1);  /* use true as result */
		lua_pushvalue(L, -1);  /* extra copy to be returned */
		lua_setfield(L, 2, name);  /* _LOADED[name] = true */
	}

	//stackDump(L);
	return 1;
}

#endif
