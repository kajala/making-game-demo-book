#include <lua/LuaStackRestore.h>
#include <lua/LuaState.h>
#include "lua-5.0.2/include/lua.h"
#include <config.h>


namespace lua
{


LuaStackRestore::LuaStackRestore( LuaState* luastate ) :
	m_impl( luastate->impl() ),
	m_top( lua_gettop(m_impl) )
{
}

LuaStackRestore::LuaStackRestore( lua_State* impl ) :
	m_impl( impl ),
	m_top( lua_gettop(m_impl) )
{
}

LuaStackRestore::~LuaStackRestore()
{
	lua_settop( m_impl, m_top );
}


} // lua

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
