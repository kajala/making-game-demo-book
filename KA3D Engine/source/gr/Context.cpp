#include <gr/Context.h>
#include <gr/Texture.h>
#include <lang/String.h>
#include <string.h>
#include <config.h>


namespace gr
{


Context::RenderScene::RenderScene( Context* context ) :
	m_context( context )
{
	m_context->beginScene();
}
	
Context::RenderScene::~RenderScene()
{
	m_context->endScene();
}

void Context::Statistics::reset()
{
	memset( this, 0, sizeof(Statistics) );
}

Context::Context()
{
	statistics.reset();
}

Context::~Context()
{
}

float Context::aspect() const
{
	return (float)width() / (float)height();
}


} // gr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
