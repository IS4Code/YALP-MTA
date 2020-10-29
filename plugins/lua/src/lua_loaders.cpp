#include "lua_loaders.h"

#include "main.h"
#include "lua_api.h"
#include "lua_utils.h"

#include "fixes/linux.h"

#include <stdio.h>
#include <cstring>
#include <cmath>

// Copied from loadlib
static int readable(const char *filename) {
	FILE *f = fopen(filename, "r");  /* try to open file */
	if(f == NULL) return 0;  /* open failed */
	fclose(f);
	return 1;
}


static const char *pushnexttemplate(lua_State *L, const char *path) {
	const char *l;
	while(*path == *LUA_PATH_SEP) path++;  /* skip separators */
	if(*path == '\0') return NULL;  /* no more templates */
	l = strchr(path, *LUA_PATH_SEP);  /* find next separator */
	if(l == NULL) l = path + strlen(path);
	lua_pushlstring(L, path, l - path);  /* template */
	return l;
}

static const char *searchpath(lua_State *L, const char *name,
	const char *path,
	const char *sep,
	const char *dirsep) {
	luaL_Buffer msg;  /* to build error message */
	luaL_buffinit(L, &msg);
	if(*sep != '\0')  /* non-empty separator? */
		name = luaL_gsub(L, name, sep, dirsep);  /* replace it by 'dirsep' */
	while((path = pushnexttemplate(L, path)) != NULL) {
		const char *filename = luaL_gsub(L, lua_tostring(L, -1),
			LUA_PATH_MARK, name);
		lua_remove(L, -2);  /* remove path template */
		if(readable(filename))  /* does file exist and is readable? */
			return filename;  /* return that file name */
		lua_pushfstring(L, "\n\tno file '%s'", filename);
		lua_remove(L, -2);  /* remove file name */
		luaL_addvalue(&msg);  /* concatenate error msg. entry */
	}
	luaL_pushresult(&msg);  /* create error message */
	return NULL;  /* not found */
}

void vlogprintf(const char *fmt, va_list args)
{
	va_list args2;
	va_copy(args2, args);
	std::string msg(vsnprintf(nullptr, 0, fmt, args), '\0');
	vsprintf(&msg[0], fmt, args2);
	va_end(args2);
	logprintf("%s", msg.c_str());
}

class LogModuleManager : public ILuaModuleManager10
{
public:
	virtual void ErrorPrintf(const char* szFormat, ...) override
	{
		va_list args;
		va_start(args, szFormat);
		vlogprintf(szFormat, args);
		va_end(args);
	}

	virtual void DebugPrintf(lua_State* luaVM, const char* szFormat, ...) override
	{

	}

	virtual void Printf(const char* szFormat, ...) override
	{
		va_list args;
		va_start(args, szFormat);
		vlogprintf(szFormat, args);
		va_end(args);
	}
};

class ResourceModuleManager : public LogModuleManager
{
public:
	virtual bool GetResourceName(lua_State* luaVM, std::string& strName) override
	{
		return false;
	}

	virtual CChecksum GetResourceMetaChecksum(lua_State* luaVM) override
	{
		return {};
	}

	virtual CChecksum GetResourceFileChecksum(lua_State* luaVM, const char* szFile) override
	{
		return {};
	}

	virtual unsigned long GetVersion() override
	{
		return 0;
	}

	virtual const char* GetVersionString() override
	{
		return "";
	}

	virtual const char* GetVersionName() override
	{
		return "";
	}

	virtual unsigned long GetNetcodeVersion() override
	{
		return 0;
	}

	virtual const char* GetOperatingSystemName() override
	{
		return "";
	}

	virtual lua_State* GetResourceFromName(const char* szResourceName) override
	{
		return nullptr;
	}

	virtual bool GetResourceName(lua_State* luaVM, char* szName, size_t length) override
	{
		return false;
	}

	virtual bool GetResourceFilePath(lua_State* luaVM, const char* fileName, char* path, size_t length) override
	{
		return false;
	}
};

class LuaModuleManager : public ResourceModuleManager
{
public:
	LuaModuleManager()
	{

	}

	virtual bool RegisterFunction(lua_State* luaVM, const char* szFunctionName, lua_CFunction Func) override
	{
		lua_pushvalue(luaVM, -1);
		lua_pushcclosure(luaVM, Func, 1);
		lua_setglobal(luaVM, szFunctionName);
		return true;
	}
};

int loader_MTA(lua_State *L)
{
	if(!logprintf)
	{
		lua::pushliteral(L, "YALP-MTA must be loaded as a plugin before it is imported from Lua!");
		return lua::error(L);
	}

	const char *path = lua_tostring(L, lua_upvalueindex(1));
	if(!path) return 0;

	// Replace the original registry table
	lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
	lua_newtable(L);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);

	LuaModuleManager manager{};
	auto& mod = lua::newuserdata<mta_module>(L, path, manager);

	mod.RegisterFunctions(L);

	lua_setglobal(L, "MODULE_DATA");

	lua_pushstring(L, mod.name());
	lua_setglobal(L, "MODULE_NAME");

	lua_pushstring(L, mod.author());
	lua_setglobal(L, "MODULE_AUTHOR");

	lua_pushnumber(L, std::round(mod.version() * 100000) / 100000.0);
	lua_setglobal(L, "MODULE_VERSION");

	// Restore the registry table
	lua_rawgeti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);
	lua_pushvalue(L, -2);
	lua_rawseti(L, LUA_REGISTRYINDEX, LUA_RIDX_GLOBALS);

	return 1;
}

int searcher_MTA(lua_State *L)
{
	const char *name = luaL_checkstring(L, 1);
	lua_getfield(L, lua_upvalueindex(1), "mpath");
	const char *path = lua_tostring(L, -1);
	if(path == NULL) luaL_error(L, "'package.mpath' must be a string");
	const char *f = searchpath(L, name, path, ".", LUA_DIRSEP);
	if(f == NULL)
	{
		lua_pushnil(L);
		lua_insert(L, -2);
		return 2;
	}

	lua_pushstring(L, f);
	lua_pushcclosure(L, &loader_MTA, 1);
	return 1;
}

mta_module::mta_module(const char *name, ILuaModuleManager &manager) :
	lib_module(name),
	InitModule(reinterpret_cast<decltype(InitModule)>(get_proc("InitModule"))),
	ShutdownModule(reinterpret_cast<decltype(ShutdownModule)>(get_proc("ShutdownModule"))),
	DoPulse(reinterpret_cast<decltype(DoPulse)>(get_proc("DoPulse"))),
	RegisterFunctions(reinterpret_cast<decltype(RegisterFunctions)>(get_proc("RegisterFunctions"))),
	ResourceStopping(reinterpret_cast<decltype(ResourceStopping)>(get_proc("ResourceStopping"))),
	ResourceStopped(reinterpret_cast<decltype(ResourceStopped)>(get_proc("ResourceStopped")))
{
	if(loaded_first())
	{
		InitModule(manager, moduleName, authorName, ver);
	}
}

mta_module::~mta_module()
{
	if(loaded_first())
	{
		ShutdownModule();
	}
}
