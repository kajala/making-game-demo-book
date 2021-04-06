// Note: In actual application you should
// include individual headers for optimal
// compilation performance. This example includes
// everything just for convenience.
#include <framework/App.h>
#include <io/all.h>
#include <gr/all.h>
#include <hgr/all.h>
#include <lang/all.h>
#include <math/all.h>
#include <ode/ODEWorld.h>
#include <ode/ODEObject.h>


class PhysicsApp : 
	public framework::App
{
public:
	PhysicsApp( gr::Context* context );

	~PhysicsApp();
	
	void	keyDown( KeyType key );

	void	update( float dt, gr::Context* context );

private:
	float				m_timeToUpdate;
	P(gr::Context)		m_context;
	P(hgr::Scene)		m_scene;
	P(hgr::Camera)		m_camera;
	P(hgr::Console)		m_console;

	P(ode::ODEWorld)				m_world;
	lang::Array<P(ode::ODEObject)>	m_objects;

	void		simulate( float dt );
	void		render( gr::Context* context, float fps );
	void		renderDebugInfo( gr::Context* context, float fps );
	void		swapBackBuffer( gr::Context* context );

	void		restart();
	void		initSim();
	static void collisionCheckCallback( void* data, dGeomID o1, dGeomID o2 );

	PhysicsApp( const PhysicsApp& );
	PhysicsApp& operator=( const PhysicsApp& );
};

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
