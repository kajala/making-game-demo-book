#include <framework/App.h>
#include <gr/all.h>
#include <hgr/all.h>


class HierarchyApp : 
	public framework::App
{
public:
	HierarchyApp( gr::Context* context );

	void		update( float dt, gr::Context* context );
	void		drawLine( const math::float3& wp0, const math::float3& wp1 );

	static HierarchyApp*	get()			{return static_cast<HierarchyApp*>( App::get() );}	

private:
	float				m_time;
	P(hgr::Scene)		m_scene;
	P(hgr::Camera)		m_camera;
	P(hgr::Node)		m_sun;
	P(hgr::Node)		m_earth;
	P(hgr::Node)		m_moon;
	P(hgr::PipeSetup)	m_pipeSetup;
	P(hgr::Pipe)		m_defaultPipe;
	P(hgr::Pipe)		m_glowPipe;
};

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
