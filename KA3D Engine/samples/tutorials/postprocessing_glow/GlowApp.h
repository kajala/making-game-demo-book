#include <framework/App.h>
#include <gr/all.h>
#include <hgr/all.h>


class GlowApp : 
	public framework::App
{
public:
	GlowApp( gr::Context* context );
	
	void update( float dt, gr::Context* context );

	void keyDown( KeyType key );

private:
	P(hgr::PipeSetup)	m_pipeSetup;
	P(hgr::Pipe)		m_glowPipe;
	P(hgr::Pipe)		m_defaultPipe;
	float				m_time;
	P(hgr::Scene)		m_scene;
	P(hgr::Camera)		m_camera;
	bool				m_capture;
};

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
