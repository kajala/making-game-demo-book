#include "Map.h"
#include <io/FileInputStream.h>
#include <lang/Math.h>
#include <lang/Exception.h>
#include <config.h>


using namespace gr;
using namespace io;
using namespace hgr;
using namespace lua;
using namespace lang;


Map::Map( LuaState* lua, Context* context, Console* console, const String& filename ) :
	LuaObject( lua ),
	m_context( context ),
	m_console( console )
{
	// compile exporter map Lua script
	FileInputStream in( filename );
	lua->compile( &in, in.available(), filename, this );

	initMapData();
	initBlockBitmaps();
}

void Map::initMapData()
{
	// get some frequently used params from Lua table
	m_mapw = (int)getNumber("MAPWIDTH");
	m_maph = (int)getNumber("MAPHEIGHT");
	int numblocks = (int)getNumber("NUMBLOCKGFX");

	LuaTable maptab = getTable("MAP");
	m_map.resize( m_mapw * m_maph );

	for ( int j = 0 ; j < m_maph ; ++j )
	{
		for ( int i = 0 ; i < m_mapw ; ++i )
		{
			int ix = int( maptab.getNumber(1+j*m_mapw+i) );
			if ( ix < 0 || ix >= numblocks )
				throwError( Exception( Format("Map index out of bounds") ) );
			
			m_map[j*m_mapw+i] = ix;
		}
	}
}

void Map::initBlockBitmaps()
{
	// get some frequently used params from Lua table
	m_blocks = (int)getNumber("NUMBLOCKGFX");
	m_blockw = (int)getNumber("BLOCKWIDTH");
	m_blockh = (int)getNumber("BLOCKHEIGHT");
	m_blockdepth = (int)getNumber("BLOCKHEIGHT");

	// check creation params
	if ( m_blockdepth != 32 )
		throwError( Exception( Format("This test supports only 32-bit block bitmaps") ) );
	if ( m_blocks < 1 )
		throwError( Exception( Format("Not enough m_blocks in the map") ) );
	if ( m_blockw != m_blockh )
		throwError( Exception( Format("Block width and height must be equal") ) );

	// calculate needed bitmap size
	int blocksqrt = int(Math::sqrt( getNumber("NUMBLOCKGFX") ));
	int w0 = (blocksqrt+1) * m_blockw;
	int h0 = (blocksqrt+1) * m_blockh;
	int w = 1;
	while ( w < w0 || w < h0 )
		w <<= 1;

	// create block bitmap
	int blocksinrow = w/m_blockw;
	m_blockTex = m_context->createTexture( w, w, SurfaceFormat::SURFACE_A8R8G8B8, 0, Context::USAGE_STATIC );
	LuaTable blockbitmaps = getTable("BLOCKBITMAPS");
	Array<int> blockdata;

	for ( int i = 0 ; i < m_blocks ; ++i )
	{
		int blockx = i % blocksinrow;
		int blocky = i / blocksinrow;

		blockdata.resize( m_blockw*m_blockh );
		for ( int y = 0 ; y < m_blockh ; ++y )
		{
			int ix = 1 + (y+i*m_blockh)*m_blockw*4;
			for ( int x = 0 ; x < m_blockw ; ++x )
			{
				float a = blockbitmaps.getNumber( ix+x*4 );
				float r = blockbitmaps.getNumber( ix+x*4+1 );
				float g = blockbitmaps.getNumber( ix+x*4+2 );
				float b = blockbitmaps.getNumber( ix+x*4+3 );
				int c = 0xFF000000 + (int(r)<<16) + (int(g)<<8) + (int(b));
				blockdata[x+y*m_blockw] = c;
			}
		}

		int x0 = blockx*m_blockw;
		int y0 = blocky*m_blockh;
		m_blockTex->blt( x0, y0, &blockdata[0], m_blockw*4, m_blockw, m_blockh, SurfaceFormat::SURFACE_A8R8G8B8, 0, SurfaceFormat() );

		// store rectangle (could be re-calculated later but this is just simpler)
		m_blockRects.add( Rect(x0,y0,x0+m_blockw,y0+m_blockh) );
	}
}

void Map::render()
{
	int screenw = m_context->width();
	int screenh = m_context->height();

	int j = 0;
	for ( int y = 0 ; y < screenh ; y += m_blockh )
	{
		int i = 0;
		for ( int x = 0 ; x < screenw ; x += m_blockw )
		{
			int ix = m_map[j*m_mapw+i];
			
			Rect rc = m_blockRects[ix];
			m_console->drawImage( x, y, rc, m_blockTex );

			++i;
		}
		
		++j;
	}
}

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
