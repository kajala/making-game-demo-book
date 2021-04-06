#ifndef _DEMOAPP_H
#define _DEMOAPP_H


#include <framework/App.h>
#include <gr/Context.h>
#include <hgr/Console.h>
#include <math/float2.h>
#include "Map.h"
#include "Anim.h"


namespace lua {
	class LuaState;}


class DemoApp : 
	public framework::App
{
public:
	DemoApp( gr::Context* context );

	~DemoApp();

	void	keyDown( KeyType key );

	void	keyUp( KeyType key );

	void	mouseMove( int dx, int dy );

	void	activate( bool active );

	void	update( float dt, gr::Context* context );

	static DemoApp*		get()		{return static_cast<DemoApp*>( App::get() );}

private:
	float				m_dt;
	P(gr::Context)		m_context;
	P(hgr::Console)		m_console;
	P(lua::LuaState)	m_lua;
	P(Map)				m_map;
	P(Anim)				m_ship;
	int					m_shipFrame;

	void	simulate( float dt );
	void	render( gr::Context* context );

	DemoApp( const DemoApp& );
	DemoApp& operator=( const DemoApp& );
};


#endif // _DEMOAPP_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
