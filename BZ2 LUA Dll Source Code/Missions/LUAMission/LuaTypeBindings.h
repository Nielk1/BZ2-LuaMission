#ifndef _LuaTypeBindings_
#define _LuaTypeBindings_

#include "lua.hpp"
#include "..\..\source\fun3d\ScriptUtils.h"

#pragma region Handle Type Lua Bindings

// push a handle to the lua stack
static void PushHandle(lua_State *L, Handle h)
{
	if (h)
	{
		Handle *v = static_cast<Handle *>(lua_newuserdata(L, sizeof(Handle)));
		luaL_getmetatable(L, "Handle");
		lua_setmetatable(L, -2);
		*v = h;
	}
	else
	{
		lua_pushnil(L);
	}
}

// get a handle from the lua stack
static Handle GetHandle(lua_State *L, int n)
{
	return *static_cast<Handle *>(luaL_checkudata(L, n, "Handle"));
}

// Require a handle, or warn the player there's not one.
static Handle RequireHandle(lua_State *L, int n)
{
	if (lua_isnil(L, n))
		return NULL;

	return *static_cast<Handle *>(luaL_checkudata(L, n, "Handle"));
}

#pragma endregion Handle Type Lua Bindings

#pragma region Vector Type Lua Bindings

// get a vector from the lua stack
// returns NULL if the item is not a vector
static Vector *GetVector(lua_State *L, int n)
{
	return static_cast<Vector *>(luaL_testudata(L, n, "Vector"));
}

// get a required vector from the lua stack
static Vector *RequireVector(lua_State *L, int n)
{
	return static_cast<Vector *>(luaL_checkudata(L, n, "Vector"));
}

// create a vector on the lua stack
// note, this only allocates the vector, you need to insert the vector value into this pointer
static Vector *NewVector(lua_State *L)
{
	Vector *v = static_cast<Vector *>(lua_newuserdata(L, sizeof(Vector)));
	luaL_getmetatable(L, "Vector");
	lua_setmetatable(L, -2);
	return v;
}

#pragma endregion Vector Type Lua Bindings

#pragma region Matrix Type Lua Bindings

// get a matrix from the lua stack
// returns NULL if the item is not a matrix
static Matrix *GetMatrix(lua_State *L, int n)
{
	return static_cast<Matrix *>(luaL_testudata(L, n, "Matrix"));
}

// get a required Matrix from the lua stack
static Matrix *RequireMatrix(lua_State *L, int n)
{
	return static_cast<Matrix *>(luaL_checkudata(L, n, "Matrix"));
}

// create a matrix on the lua stack
// note, this only allocates the matrix, you need to insert the matrix value into this pointer
static Matrix *NewMatrix(lua_State *L)
{
	Matrix *m = static_cast<Matrix *>(lua_newuserdata(L, sizeof(Matrix)));
	luaL_getmetatable(L, "Matrix");
	lua_setmetatable(L, -2);
	return m;
}

#pragma endregion Matrix Type Lua Bindings

#endif