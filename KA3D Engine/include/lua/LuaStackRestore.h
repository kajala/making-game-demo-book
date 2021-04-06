#ifndef _LUA_LUASTACKRESTORE_H
#define _LUA_LUASTACKRESTORE_H


struct lua_State;


namespace lua
{


class LuaState;


/** 
 * Helper class for storing and restoring Lua stack stack top position.
 * Stores the top at the construction and restores it at the end of scope.
 * 
 * @ingroup lua
 */
class LuaStackRestore
{
public:
	/** Saves Lua stack top position. */
	LuaStackRestore( LuaState* luastate );

	/** Saves Lua stack top position. */
	LuaStackRestore( lua_State* impl );

	/** Restores Lua stack top position. */
	~LuaStackRestore();

private:
	lua_State*	m_impl;
	int			m_top;

	LuaStackRestore( const LuaStackRestore& );
	LuaStackRestore& operator=( const LuaStackRestore& );
};


} // lua


#endif // _LUA_LUASTACKRESTORE_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
