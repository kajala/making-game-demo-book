#include <framework/App.h>
#include <gr/all.h>
#include <hgr/all.h>


class ParticleApp : 
	public framework::App
{
public:
	ParticleApp( gr::Context* context );
	
	void update( float dt, gr::Context* context );

private:
	P(gr::Primitive)	createSimplePrimitive( gr::Context* context );

	P(hgr::Scene)				m_scene;
	P(hgr::ParticleSystem)		m_particleSystem;
	P(hgr::Camera)				m_camera;
};

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
