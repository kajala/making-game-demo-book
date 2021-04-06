//
// Tutorial: Particle System Rendering
//
// Tested on:
// - WinXP/DX9/Athlon XP 2700+/ATI Radeon 9600
//
#include "ParticleApp.h"
#include <lang/all.h>
#include <math/all.h>
#include <config.h>


using namespace gr;
using namespace io;
using namespace hgr;
using namespace lang;
using namespace math;
using namespace framework;


ParticleApp::ParticleApp( Context* context )
{
	// scene setup
	m_scene = new Scene;

	// particle system setup
	m_particleSystem = new ParticleSystem( context, "data/fire.prt" );
	m_particleSystem->linkTo( m_scene );

	// camera setup
	m_camera = new Camera;
	m_camera->linkTo( m_scene );
	m_camera->setPosition( float3(0,3,-20) );
	m_camera->lookAt( m_particleSystem );
}

void ParticleApp::update( float dt, Context* context )
{
	// update particles
	m_particleSystem->update( dt );

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
	config.name = "Tutorial: Particle System Rendering";
}

framework::App* framework::init( Context* context )
{
	return new ParticleApp( context );
}

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
