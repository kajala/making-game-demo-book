#ifndef _MAP_H
#define _MAP_H


#include <gr/Context.h>
#include <gr/Texture.h>
#include <hgr/Console.h>
#include <lua/LuaObject.h>
#include <lang/Array.h>


class Map :
	public lua::LuaObject
{
public:
	Map( lua::LuaState* lua, gr::Context* context, hgr::Console* console, const lang::String& filename );

	void	render();

private:
	P(gr::Context)			m_context;
	P(hgr::Console)			m_console;
	P(gr::Texture)			m_blockTex;
	lang::Array<gr::Rect>	m_blockRects;
	lang::Array<int>		m_map;

	int		m_blocks;
	int		m_blockw;
	int		m_blockh;
	int		m_blockdepth;
	int		m_mapw;
	int		m_maph;

	void	initMapData();
	void	initBlockBitmaps();
};


#endif // _MAP_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
