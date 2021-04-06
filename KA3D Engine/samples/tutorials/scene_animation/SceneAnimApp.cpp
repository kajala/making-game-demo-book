//
// Tutorial: Scene Graph Animation Playback
//
// Tested on:
// - WinXP/DX9/Athlon XP 2700+/ATI Radeon 9600
//
#include "SceneAnimApp.h"
#include <io/PathName.h>
#include <lang/all.h>
#include <math/all.h>
#include <config.h>


using namespace gr;
using namespace io;
using namespace hgr;
using namespace lang;
using namespace math;
using namespace framework;


SceneAnimApp::SceneAnimApp( Context* context ) :
	m_time( 0 )
{
	// load scene file and textures from the same directory
	m_scene = new Scene( context, "data/zax.hgr" );
	m_camera = m_scene->camera();

	// print object names in hierarchy to debug output
	m_scene->printHierarchy();
}

void SceneAnimApp::update( float dt, Context* context )
{
	// update animations
	m_time += dt;
	m_scene->applyAnimations( m_time, dt );

	// render frame
	{
		Context::RenderScene rs( context );
		m_camera->render( context );
	}

	// flip back buffer
	context->present();
}

void framework::configure( App::Configuration& config )
{
	config.name = "Tutorial: Scene Graph Animation Playback";
}

App* framework::init( Context* context )
{
	return new SceneAnimApp( context );
}

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
