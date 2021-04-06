#include <framework/App.h>
#include <gr/all.h>
#include <hgr/all.h>


class SceneAnimApp : 
	public framework::App
{
public:
	SceneAnimApp( gr::Context* context );

	void		update( float dt, gr::Context* context );
	void		drawLine( const math::float3& wp0, const math::float3& wp1 );

	static SceneAnimApp*	get()			{return static_cast<SceneAnimApp*>( App::get() );}	

private:
	P(hgr::Scene)		m_scene;
	P(hgr::Camera)		m_camera;
	float				m_time;
};

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
