#include "lua_utils.h"

#include <unordered_map>
#include <assert.h>

int lua::error(lua_State *L)
{
	if(lua_type(L, -1) == LUA_TSTRING)
	{
		luaL_where(L, 1);
		lua_insert(L, -2);
		lua_concat(L, 2);
	}
	return lua_error(L);
}
