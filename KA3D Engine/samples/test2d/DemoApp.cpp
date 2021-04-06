/*
 * Simple Mappy 2D editor rendering example
 *
 * Features:
 * - Mappy 2D Exporter (see source_data/Export My Map.lua)
 * - Exported 2D map rendering
 * - 2D animation rendering (press LEFT/RIGHT arrows to rotate spaceship)
 *
 * Known issues:
 * - Block properties (like collisions) are not parsed
 *   (they are exported to Lua script tho so would be simple to add,
 *   check BLOCKINFO table in the exported testmap.mym Lua script)
 * - Space ship animation is badly done (it's not rotating
 *   with constant speed so it 'slows down' at the end)
 */
#include "DemoApp.h"
#include "version.h"
#include <io/all.h>
#include <gr/Rect.h>
#include <lua/LuaState.h>
#include <hgr/DefaultResourceManager.h>
#include <lang/Math.h>
#include <lang/Profile.h>
#include <config.h>


using namespace gr;
using namespace io;
using namespace lua;
using namespace hgr;
using namespace lang;
using namespace math;
using namespace framework;


DemoApp::DemoApp( Context* context ) :
	m_dt( 0.f ),
	m_context( context )
{
	P(ResourceManager) res = DefaultResourceManager::get( context );
	
	m_lua = new LuaState;

	m_console = new Console( context, res->getTexture("data/fonts/comic_sans_ms_20x23.dds"), 20, 23, -2, 
		"sprite-alpha-pointfilter.fx" );

	m_map = new Map( m_lua, context, m_console, "data/testmap.mym" );

	m_ship = new Anim( context, m_console, "data/ship/ship%04d.bmp", 0, 180 );
	m_shipFrame = 0;
}

DemoApp::~DemoApp()
{
}

void DemoApp::update( float dt, Context* context )
{
	const float FIXED_UPDATE_DT = 1.f/50.f;
	m_dt += dt;
	while ( m_dt >= FIXED_UPDATE_DT )
	{
		simulate( FIXED_UPDATE_DT );
		m_dt -= FIXED_UPDATE_DT;
	}

	render( context );
	context->present();
}

void DemoApp::simulate( float dt )
{
	if ( isKeyDown(KEY_LEFT) )
		m_shipFrame -= 1;
	if ( isKeyDown(KEY_RIGHT) )
		m_shipFrame += 1;

	// ensure [0,180) frame range
	m_shipFrame = (m_shipFrame+180)%180;
}

void DemoApp::keyDown( KeyType key )
{
}

void DemoApp::keyUp( KeyType key )
{
}

void DemoApp::activate( bool /*active*/ )
{
}

void DemoApp::mouseMove( int dx, int dy )
{
}

void DemoApp::render( Context* context )
{
	PROFILE(render);

	Context::RenderScene rs( context );

	m_map->render();
	m_ship->render( 470, 375, m_shipFrame );

	m_console->render( context );
	m_console->clear();
}

void framework::configure( App::Configuration& config )
{
	config.name = "Test2D Demo " TEST2D_VERSION_NUMBER;
	config.captureMouse = false;

	// load config
	LuaState lua;
	LuaTable tab( &lua );
	FileInputStream in( "data/config.lua" );
	tab.read( &in );

	config.fullscreen = tab.getBoolean( "fullscreen" );
	config.width = (int)tab.getNumber( "width" );
	config.height = (int)tab.getNumber( "height" );
	config.bits = (int)tab.getNumber( "bits" );
}

App* framework::init( gr::Context* context )
{
	return new DemoApp( context );
}

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
