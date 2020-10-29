#include "lua_lib.h"
#include "lua_api.h"
#include "lua_utils.h"

#include "lua_loaders.h"

LUA_EXPORT int luaopen_YALP(lua_State *L)
{
	return luaopen_YALP_MTA(L);
}

LUA_EXPORT int luaopen_MTA(lua_State *L)
{
	return luaopen_YALP_MTA(L);
}

LUA_EXPORT int luaopen_YALP_MTA(lua_State *L)
{
	// find "package" table
	if(lua_getglobal(L, "require") != LUA_TFUNCTION || !lua_getupvalue(L, -1, 1))
	{
		lua::pushliteral(L, "'require' must be available and unmodified");
		return lua::error(L);
	}

#ifdef _WIN32
	lua::pushliteral(L, "plugins" LUA_DIRSEP "mta" LUA_DIRSEP "?.dll");
#else
	lua::pushliteral(L, "plugins" LUA_DIRSEP "mta" LUA_DIRSEP "?.so");
#endif
	lua_setfield(L, -2, "mpath");

	lua_getfield(L, -1, "searchers");
	lua_pushvalue(L, -2);
	lua_pushcclosure(L, searcher_MTA, 1);
	lua_pushvalue(L, -1);
	lua_rawseti(L, -3, lua_rawlen(L, -3) + 1);

	return 1;
}
