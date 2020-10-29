#ifndef LUA_LIB_H_INCLUDED
#define LUA_LIB_H_INCLUDED

#include "lua_api.h"

#include <type_traits>

#if defined(_WIN32) || defined(__CYGWIN__)
#ifdef __GNUC__
#define LUA_EXPORT extern "C" __attribute__ ((dllexport))
#else
#define LUA_EXPORT extern "C" __declspec(dllexport)
#endif
#else
#if __GNUC__ >= 4
#define LUA_EXPORT extern "C"  __attribute__ ((visibility ("default")))
#else
#define LUA_EXPORT extern "C" 
#endif
#endif

LUA_EXPORT typename std::remove_pointer<lua_CFunction>::type luaopen_YALP;
LUA_EXPORT typename std::remove_pointer<lua_CFunction>::type luaopen_MTA;
LUA_EXPORT typename std::remove_pointer<lua_CFunction>::type luaopen_YALP_MTA;

#endif
