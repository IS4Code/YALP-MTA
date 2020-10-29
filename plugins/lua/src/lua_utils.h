#ifndef LUA_UTILS_H_INCLUDED
#define LUA_UTILS_H_INCLUDED

#include "lua_api.h"

#include <type_traits>
#include <functional>

#if __GNUG__ && __GNUC__ < 5
#define is_trivially_constructible(T) __has_trivial_constructor(T)
#define is_trivially_destructible(T) __has_trivial_destructor(T)
#else
#define is_trivially_constructible(T) std::is_trivially_constructible<T>::value
#define is_trivially_destructible(T) std::is_trivially_destructible<T>::value
#endif

namespace lua
{
	template <class Type>
	struct mt_ctor
	{
		bool operator()(lua_State *L)
		{
			return false;
		}
	};

	template <class Type>
	struct _udata
	{
		typedef Type &type;

		static Type &to(lua_State *L, int idx)
		{
			return *reinterpret_cast<Type*>(lua_touserdata(L, idx));
		}

		static Type &check(lua_State *L, int idx, const char *tname)
		{
			return *reinterpret_cast<Type*>(luaL_checkudata(L, idx, tname));
		}
	};

	template <class Type>
	struct _udata<Type[]>
	{
		typedef Type *type;

		static Type *to(lua_State *L, int idx)
		{
			return reinterpret_cast<Type*>(lua_touserdata(L, idx));
		}

		static Type *check(lua_State *L, int idx, const char *tname)
		{
			return reinterpret_cast<Type*>(luaL_checkudata(L, idx, tname));
		}
	};

	template <class Type>
	typename _udata<Type>::type touserdata(lua_State *L, int idx)
	{
		return _udata<Type>::to(L, idx);
	}

	template <class Type>
	typename _udata<Type>::type checkudata(lua_State *L, int idx, const char *tname)
	{
		return _udata<Type>::check(L, idx, tname);
	}

	template <class Type>
	void _initmt(lua_State *L)
	{
		bool hasmt = mt_ctor<Type>()(L);
		if(!is_trivially_destructible(Type))
		{
			if(!hasmt)
			{
				lua_createtable(L, 0, 2);
				hasmt = true;
			}
			lua_pushboolean(L, false);
			lua_setfield(L, -2, "__metatable");
			lua_pushcfunction(L, [](lua_State *L) {
				lua::touserdata<Type>(L, -1).~Type();
				return 0;
			});
			lua_setfield(L, -2, "__gc");
		}
		if(hasmt)
		{
			lua_setmetatable(L, -2);
		}
	}

	template <class Type>
	Type &newuserdata(lua_State *L)
	{
		auto data = reinterpret_cast<Type*>(lua_newuserdata(L, sizeof(Type)));
		if(!is_trivially_constructible(Type))
		{
			new (data) Type();
		}
		_initmt<Type>(L);
		return *data;
	}

	template <class Type, class... Args>
	Type &newuserdata(lua_State *L, Args&&... args)
	{
		auto data = reinterpret_cast<Type*>(lua_newuserdata(L, sizeof(Type)));
		new (data) Type(std::forward<Args>(args)...);
		_initmt<Type>(L);
		return *data;
	}

	template <class Type>
	void pushuserdata(lua_State *L, const Type &val)
	{
		newuserdata<Type>(L) = val;
	}

	template <class Type, class=typename std::enable_if<!std::is_lvalue_reference<Type>::value>::type>
	void pushuserdata(lua_State *L, Type &&val)
	{
		newuserdata<typename std::remove_reference<Type>::type>(L) = std::move(val);
	}

	int error(lua_State *L);

	template <size_t Size>
	const char *pushliteral(lua_State *L, const char(&s)[Size])
	{
		return lua_pushlstring(L, s, Size - 1);
	}
}

#endif
