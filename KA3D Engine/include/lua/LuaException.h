#ifndef _LUA_LUAEXCEPTION_H
#define _LUA_LUAEXCEPTION_H


#include <lang/Exception.h>


namespace lua
{


/**
 * Thrown if Lua operation fails.
 * 
 * @ingroup lua
 */
class LuaException :
	public lang::Exception
{
public:
	LuaException( const lang::Format& msg )				: Exception(msg) {}
};


} // lua


#endif // _LUA_LUAEXCEPTION_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
