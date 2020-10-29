#ifndef LUA_API_H_INCLUDED
#define LUA_API_H_INCLUDED

#include "lua/lua.h"
#include "lua/lualib.h"
#include "lua/lauxlib.h"

class lib_module
{
	void *handle;
	bool first;

public:
	lib_module(const char *name);

	lib_module(const lib_module&) = delete;

	lib_module &operator=(const lib_module&) = delete;

	void *get_proc(const char *name) const;

	bool loaded() const { return handle; }

	bool loaded_first() const { return first; }

	~lib_module();
};

const lib_module &get_lua_module();

inline void *lua_func(const char *name)
{
	return get_lua_module().get_proc(name);
}

inline bool lua_loaded()
{
	return get_lua_module().loaded();
}

template <class FPtr, FPtr Func>
static FPtr _lua_func(const char *name)
{
	static FPtr ptr = reinterpret_cast<FPtr>(lua_func(name));
	return ptr;
}

#define lua(func) _lua_func<decltype(&func), &func>(#func)

#define lua_newstate(...) lua(lua_newstate)(__VA_ARGS__)
#define lua_close(...) lua(lua_close)(__VA_ARGS__)
#define lua_newthread(...) lua(lua_newthread)(__VA_ARGS__)
#define lua_atpanic(...) lua(lua_atpanic)(__VA_ARGS__)
#define lua_version(...) lua(lua_version)(__VA_ARGS__)
#define lua_absindex(...) lua(lua_absindex)(__VA_ARGS__)
#define lua_gettop(...) lua(lua_gettop)(__VA_ARGS__)
#define lua_settop(...) lua(lua_settop)(__VA_ARGS__)
#define lua_pushvalue(...) lua(lua_pushvalue)(__VA_ARGS__)
#define lua_rotate(...) lua(lua_rotate)(__VA_ARGS__)
#define lua_copy(...) lua(lua_copy)(__VA_ARGS__)
#define lua_checkstack(...) lua(lua_checkstack)(__VA_ARGS__)
#define lua_xmove(...) lua(lua_xmove)(__VA_ARGS__)
#define lua_isnumber(...) lua(lua_isnumber)(__VA_ARGS__)
#define lua_isstring(...) lua(lua_isstring)(__VA_ARGS__)
#define lua_iscfunction(...) lua(lua_iscfunction)(__VA_ARGS__)
#define lua_isinteger(...) lua(lua_isinteger)(__VA_ARGS__)
#define lua_isuserdata(...) lua(lua_isuserdata)(__VA_ARGS__)
#define lua_type(...) lua(lua_type)(__VA_ARGS__)
#define lua_typename(...) lua(lua_typename)(__VA_ARGS__)
#define lua_tonumberx(...) lua(lua_tonumberx)(__VA_ARGS__)
#define lua_tointegerx(...) lua(lua_tointegerx)(__VA_ARGS__)
#define lua_toboolean(...) lua(lua_toboolean)(__VA_ARGS__)
#define lua_tolstring(...) lua(lua_tolstring)(__VA_ARGS__)
#define lua_rawlen(...) lua(lua_rawlen)(__VA_ARGS__)
#define lua_tocfunction(...) lua(lua_tocfunction)(__VA_ARGS__)
#define lua_touserdata(...) lua(lua_touserdata)(__VA_ARGS__)
#define lua_tothread(...) lua(lua_tothread)(__VA_ARGS__)
#define lua_topointer(...) lua(lua_topointer)(__VA_ARGS__)
#define lua_arith(...) lua(lua_arith)(__VA_ARGS__)
#define lua_rawequal(...) lua(lua_rawequal)(__VA_ARGS__)
#define lua_compare(...) lua(lua_compare)(__VA_ARGS__)
#define lua_pushnil(...) lua(lua_pushnil)(__VA_ARGS__)
#define lua_pushnumber(...) lua(lua_pushnumber)(__VA_ARGS__)
#define lua_pushinteger(...) lua(lua_pushinteger)(__VA_ARGS__)
#define lua_pushlstring(...) lua(lua_pushlstring)(__VA_ARGS__)
#define lua_pushstring(...) lua(lua_pushstring)(__VA_ARGS__)
#define lua_pushvfstring(...) lua(lua_pushvfstring)(__VA_ARGS__)
#define lua_pushfstring(...) lua(lua_pushfstring)(__VA_ARGS__)
#define lua_pushcclosure(...) lua(lua_pushcclosure)(__VA_ARGS__)
#define lua_pushboolean(...) lua(lua_pushboolean)(__VA_ARGS__)
#define lua_pushlightuserdata(...) lua(lua_pushlightuserdata)(__VA_ARGS__)
#define lua_pushthread(...) lua(lua_pushthread)(__VA_ARGS__)
#define lua_getglobal(...) lua(lua_getglobal)(__VA_ARGS__)
#define lua_gettable(...) lua(lua_gettable)(__VA_ARGS__)
#define lua_getfield(...) lua(lua_getfield)(__VA_ARGS__)
#define lua_geti(...) lua(lua_geti)(__VA_ARGS__)
#define lua_rawget(...) lua(lua_rawget)(__VA_ARGS__)
#define lua_rawgeti(...) lua(lua_rawgeti)(__VA_ARGS__)
#define lua_rawgetp(...) lua(lua_rawgetp)(__VA_ARGS__)
#define lua_createtable(...) lua(lua_createtable)(__VA_ARGS__)
#define lua_newuserdata(...) lua(lua_newuserdata)(__VA_ARGS__)
#define lua_getmetatable(...) lua(lua_getmetatable)(__VA_ARGS__)
#define lua_getuservalue(...) lua(lua_getuservalue)(__VA_ARGS__)
#define lua_setglobal(...) lua(lua_setglobal)(__VA_ARGS__)
#define lua_settable(...) lua(lua_settable)(__VA_ARGS__)
#define lua_setfield(...) lua(lua_setfield)(__VA_ARGS__)
#define lua_seti(...) lua(lua_seti)(__VA_ARGS__)
#define lua_rawset(...) lua(lua_rawset)(__VA_ARGS__)
#define lua_rawseti(...) lua(lua_rawseti)(__VA_ARGS__)
#define lua_rawsetp(...) lua(lua_rawsetp)(__VA_ARGS__)
#define lua_setmetatable(...) lua(lua_setmetatable)(__VA_ARGS__)
#define lua_setuservalue(...) lua(lua_setuservalue)(__VA_ARGS__)
#define lua_callk(...) lua(lua_callk)(__VA_ARGS__)
#define lua_pcallk(...) lua(lua_pcallk)(__VA_ARGS__)
#define lua_load(...) lua(lua_load)(__VA_ARGS__)
#define lua_dump(...) lua(lua_dump)(__VA_ARGS__)
#define lua_yieldk(...) lua(lua_yieldk)(__VA_ARGS__)
#define lua_resume(...) lua(lua_resume)(__VA_ARGS__)
#define lua_status(...) lua(lua_status)(__VA_ARGS__)
#define lua_isyieldable(...) lua(lua_isyieldable)(__VA_ARGS__)
#define lua_gc(...) lua(lua_gc)(__VA_ARGS__)
#define lua_error(...) lua(lua_error)(__VA_ARGS__)
#define lua_next(...) lua(lua_next)(__VA_ARGS__)
#define lua_concat(...) lua(lua_concat)(__VA_ARGS__)
#define lua_len(...) lua(lua_len)(__VA_ARGS__)
#define lua_stringtonumber(...) lua(lua_stringtonumber)(__VA_ARGS__)
#define lua_getallocf(...) lua(lua_getallocf)(__VA_ARGS__)
#define lua_setallocf(...) lua(lua_setallocf)(__VA_ARGS__)
#define lua_getstack(...) lua(lua_getstack)(__VA_ARGS__)
#define lua_getinfo(...) lua(lua_getinfo)(__VA_ARGS__)
#define lua_getlocal(...) lua(lua_getlocal)(__VA_ARGS__)
#define lua_setlocal(...) lua(lua_setlocal)(__VA_ARGS__)
#define lua_getupvalue(...) lua(lua_getupvalue)(__VA_ARGS__)
#define lua_setupvalue(...) lua(lua_setupvalue)(__VA_ARGS__)
#define lua_upvalueid(...) lua(lua_upvalueid)(__VA_ARGS__)
#define lua_upvaluejoin(...) lua(lua_upvaluejoin)(__VA_ARGS__)
#define lua_sethook(...) lua(lua_sethook)(__VA_ARGS__)
#define lua_gethook(...) lua(lua_gethook)(__VA_ARGS__)
#define lua_gethookmask(...) lua(lua_gethookmask)(__VA_ARGS__)
#define lua_gethookcount(...) lua(lua_gethookcount)(__VA_ARGS__)
#define luaopen_base(...) lua(luaopen_base)(__VA_ARGS__)
#define luaopen_coroutine(...) lua(luaopen_coroutine)(__VA_ARGS__)
#define luaopen_table(...) lua(luaopen_table)(__VA_ARGS__)
#define luaopen_io(...) lua(luaopen_io)(__VA_ARGS__)
#define luaopen_os(...) lua(luaopen_os)(__VA_ARGS__)
#define luaopen_string(...) lua(luaopen_string)(__VA_ARGS__)
#define luaopen_utf8(...) lua(luaopen_utf8)(__VA_ARGS__)
#define luaopen_bit32(...) lua(luaopen_bit32)(__VA_ARGS__)
#define luaopen_math(...) lua(luaopen_math)(__VA_ARGS__)
#define luaopen_debug(...) lua(luaopen_debug)(__VA_ARGS__)
#define luaopen_package(...) lua(luaopen_package)(__VA_ARGS__)
#define luaL_openlibs(...) lua(luaL_openlibs)(__VA_ARGS__)
#define luaL_checkversion_(...) lua(luaL_checkversion_)(__VA_ARGS__)
#define luaL_getmetafield(...) lua(luaL_getmetafield)(__VA_ARGS__)
#define luaL_callmeta(...) lua(luaL_callmeta)(__VA_ARGS__)
#define luaL_tolstring(...) lua(luaL_tolstring)(__VA_ARGS__)
#define luaL_argerror(...) lua(luaL_argerror)(__VA_ARGS__)
#define luaL_checklstring(...) lua(luaL_checklstring)(__VA_ARGS__)
#define luaL_optlstring(...) lua(luaL_optlstring)(__VA_ARGS__)
#define luaL_checknumber(...) lua(luaL_checknumber)(__VA_ARGS__)
#define luaL_optnumber(...) lua(luaL_optnumber)(__VA_ARGS__)
#define luaL_checkinteger(...) lua(luaL_checkinteger)(__VA_ARGS__)
#define luaL_optinteger(...) lua(luaL_optinteger)(__VA_ARGS__)
#define luaL_checkstack(...) lua(luaL_checkstack)(__VA_ARGS__)
#define luaL_checktype(...) lua(luaL_checktype)(__VA_ARGS__)
#define luaL_checkany(...) lua(luaL_checkany)(__VA_ARGS__)
#define luaL_newmetatable(...) lua(luaL_newmetatable)(__VA_ARGS__)
#define luaL_setmetatable(...) lua(luaL_setmetatable)(__VA_ARGS__)
#define luaL_testudata(...) lua(luaL_testudata)(__VA_ARGS__)
#define luaL_checkudata(...) lua(luaL_checkudata)(__VA_ARGS__)
#define luaL_where(...) lua(luaL_where)(__VA_ARGS__)
#define luaL_error(...) lua(luaL_error)(__VA_ARGS__)
#define luaL_checkoption(...) lua(luaL_checkoption)(__VA_ARGS__)
#define luaL_fileresult(...) lua(luaL_fileresult)(__VA_ARGS__)
#define luaL_execresult(...) lua(luaL_execresult)(__VA_ARGS__)
#define luaL_ref(...) lua(luaL_ref)(__VA_ARGS__)
#define luaL_unref(...) lua(luaL_unref)(__VA_ARGS__)
#define luaL_loadfilex(...) lua(luaL_loadfilex)(__VA_ARGS__)
#define luaL_loadbufferx(...) lua(luaL_loadbufferx)(__VA_ARGS__)
#define luaL_loadstring(...) lua(luaL_loadstring)(__VA_ARGS__)
#define luaL_newstate(...) lua(luaL_newstate)(__VA_ARGS__)
#define luaL_len(...) lua(luaL_len)(__VA_ARGS__)
#define luaL_gsub(...) lua(luaL_gsub)(__VA_ARGS__)
#define luaL_setfuncs(...) lua(luaL_setfuncs)(__VA_ARGS__)
#define luaL_getsubtable(...) lua(luaL_getsubtable)(__VA_ARGS__)
#define luaL_traceback(...) lua(luaL_traceback)(__VA_ARGS__)
#define luaL_requiref(...) lua(luaL_requiref)(__VA_ARGS__)
#define luaL_buffinit(...) lua(luaL_buffinit)(__VA_ARGS__)
#define luaL_prepbuffsize(...) lua(luaL_prepbuffsize)(__VA_ARGS__)
#define luaL_addlstring(...) lua(luaL_addlstring)(__VA_ARGS__)
#define luaL_addstring(...) lua(luaL_addstring)(__VA_ARGS__)
#define luaL_addvalue(...) lua(luaL_addvalue)(__VA_ARGS__)
#define luaL_pushresult(...) lua(luaL_pushresult)(__VA_ARGS__)
#define luaL_pushresultsize(...) lua(luaL_pushresultsize)(__VA_ARGS__)
#define luaL_buffinitsize(...) lua(luaL_buffinitsize)(__VA_ARGS__)
#define luaL_pushmodule(...) lua(luaL_pushmodule)(__VA_ARGS__)
#define luaL_openlib(...) lua(luaL_openlib)(__VA_ARGS__)

#endif
