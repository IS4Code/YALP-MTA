#ifndef LUA_LOADERS_H_INCLUDED
#define LUA_LOADERS_H_INCLUDED

#include "lua_api.h"
#include "ILuaModuleManager.h"

int searcher_MTA(lua_State *L);

class mta_module : private lib_module
{
	bool(*const InitModule)(ILuaModuleManager &manager, char (&name)[MAX_INFO_LENGTH], char(&author)[MAX_INFO_LENGTH], float &version);
	bool(*const ShutdownModule)();

	char moduleName[MAX_INFO_LENGTH];
	char authorName[MAX_INFO_LENGTH];
	float ver;

public:
	mta_module(const char *name, ILuaModuleManager &manager);

	bool(*const DoPulse)();
	void(*const RegisterFunctions)(lua_State *L);
	void(*const ResourceStopping)(lua_State *L);
	void(*const ResourceStopped)(lua_State *L);

	const char *name() const { return moduleName; }
	const char *author() const { return authorName; }
	float version() const { return ver; }

	~mta_module();
};

#endif
