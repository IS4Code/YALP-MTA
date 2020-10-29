#include "lua_api.h"

#ifdef _WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#ifdef _WIN32
lib_module::lib_module(const char *name)
{
	if(!GetModuleHandleEx(0, name, &reinterpret_cast<HMODULE&>(handle)))
	{
		handle = LoadLibrary(name);
		first = true;
	}
}

void *lib_module::get_proc(const char *name) const
{
	if(!handle) return nullptr;
	return GetProcAddress(static_cast<HMODULE>(handle), name);
}

lib_module::~lib_module()
{
	if(handle)
	{
		FreeLibrary(static_cast<HMODULE>(handle));
		handle = nullptr;
	}
}

const lib_module &get_lua_module()
{
	static lib_module module("YALP.dll");
	return module;
}
#else
struct lib_module
{
	void *handle;

	lib_module(const char *name)
	{
		handle = dlopen(name, RTLD_NOW | RTLD_GLOBAL | RTLD_NOLOAD);
		if(!handle)
		{
			handle = dlopen(name, RTLD_NOW | RTLD_GLOBAL);
			first = true;
		}
	}

	lib_module(const lib_module&) = delete;

	lib_module &operator=(const lib_module&) = delete;

	void *get_proc(const char *name) const
	{
		if(!handle) return nullptr;
		return GetProcAddress(handle, name);
	}

	~lib_module()
	{
		if(handle)
		{
			dlclose(handle);
			handle = nullptr;
		}
	}
};

static const lib_module &get_lua_module()
{
	static lib_module module("YALP.so");
	return module;
}
#endif
