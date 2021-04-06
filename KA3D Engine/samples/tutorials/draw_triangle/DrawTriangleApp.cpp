//
// Tutorial: Drawing two textured triangles
//
// Tested on:
// - WinXP/DX9/Athlon XP 2700+/ATI Radeon 9600
//
#include "DrawTriangleApp.h"
#include <io/PathName.h>
#include <math/all.h>
#include <config.h>


using namespace gr;
using namespace io;
using namespace lang;
using namespace math;
using namespace framework;


DrawTriangleApp::DrawTriangleApp( Context* context )
{
	// prepare vertex format
	VertexFormat vf;
	vf.addTransformedPosition();
	vf.addDiffuse();
	vf.addTextureCoordinate( VertexFormat::DF_V2_32 );
	
	// create primitive list object
	const int VERTS = 4;
	const int INDICES = 6;
	m_prim = context->createPrimitive( Primitive::PRIM_TRI, vf, VERTS, INDICES );
	
	// set primitive list data
	{
		Primitive::Lock lock( m_prim, Primitive::LOCK_WRITE );
		
		float4 vertpos[VERTS] = {
			float4(10,10,0,1),
			float4(300,10,0,1),
			float4(300,300,0,1),
			float4(10,300,0,1) };
		m_prim->setVertexTransformedPositions( 0, vertpos, VERTS );
		
		for ( int i = 0 ; i < VERTS ; ++i )
		{
			vertpos[i].x += 2048.f;
			vertpos[i].y += 2048.f;
			vertpos[i].z = 5000.f;
		}

		float4 vertuv[VERTS] = {
			float4(0,0,0,0),
			float4(1,0,0,0),
			float4(1,1,0,0),
			float4(0,1,0,0) };
		m_prim->setVertexTextureCoordinates( 0, 0, vertuv, VERTS );

		float4 vertcolor[VERTS] = {
			float4(255,255,255,255),
			float4(255,255,255,255),
			float4(255,255,255,255),
			float4(255,255,255,255) };
		m_prim->setVertexDiffuseColors( 0, vertcolor, VERTS );
			
		int indices[INDICES] = {
			0,1,2,
			0,2,3 };
		m_prim->setIndices( 0, indices, INDICES );
	}
	
	// texture setup
	String datapath = "data";
	P(Texture) tex = context->createTexture( PathName(datapath,"rgb-4b.bmp").toString() );

	// shader setup
	P(Shader) fx = context->createShader( "sprite-copy" );
	fx->setTexture( "BASEMAP", tex );
	m_prim->setShader( fx );
}

void DrawTriangleApp::update( float /*dt*/, Context* context )
{
	// render frame
	{
		Context::RenderScene rs( context );
		Shader* shader = m_prim->shader();
		int passes = shader->begin();
		shader->setMatrix( Shader::PARAM_TOTALTM, context->screenTransform() );

		for ( int i = 0 ; i < passes ; ++i )
		{
			shader->beginPass( i );
			m_prim->render();
			shader->endPass();
		}

		shader->end();
	}

	// flip back buffer
	context->present();
}

void framework::configure( App::Configuration& config )
{
	config.name = "Tutorial: Drawing two textured triangles";
}

App* framework::init( gr::Context* context )
{
	return new DrawTriangleApp( context );
}

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
