#include <hgr/PipeSetup.h>
#include <gr/Shader.h>
#include <gr/Context.h>
#include <hgr/Visual.h>
#include <hgr/Camera.h>
#include <functional>
#include <algorithm>
#include <config.h>


using namespace gr;
using namespace math;
using namespace lang;


namespace hgr
{


PipeSetup::PipeSetup( Context* context ) :
	m_frameCounter( -1 ),
	m_context( context )
{
}

PipeSetup::~PipeSetup()
{
}

void PipeSetup::setup( Camera* camera )
{
	getNodes( camera, nodes );
	getLights( nodes, lights );
	camera->cacheTransforms( m_context, nodes );
	camera->cullVisuals( nodes, visuals );
	getShaders( visuals, shaders );
	getPriorities( shaders, priorities );

	m_frameCounter = m_context->statistics.renderedFrames;
}

void PipeSetup::setTechnique( const char* name )
{
	assert( valid() ); // setup() not called in the same frame

	for ( int i = 0 ; i < shaders.size() ; ++i )
		shaders[i]->setTechnique( name );
}

Context* PipeSetup::context() const
{
	return m_context;
}

bool PipeSetup::valid() const
{
	return m_frameCounter == m_context->statistics.renderedFrames;
}

void PipeSetup::getNodes( Node* scene, Array<Node*>& nodes )
{
	nodes.clear();
	for ( Node* node = scene->root() ; node != 0 ; node = node->next() )
		nodes.add( node );
}

void PipeSetup::getLights( const Array<Node*>& nodes, LightSorter& lights )
{
	lights.removeLights();
	for ( int i = 0 ; i < nodes.size() ; ++i )
	{
		Node* node = nodes[i];
		if ( Node::NODE_LIGHT == node->classId() )
		{
			Light* lt = static_cast<Light*>(node);
			if ( lt->enabled() )
				lights.addLight( lt );
		}
	}
}

void PipeSetup::getShaders( const Array<Visual*>& visuals, Array<Shader*>& shaders )
{
	shaders.clear();
	for ( int i = 0 ; i < visuals.size() ; ++i )
	{
		Visual* vis = visuals[i];
		vis->getShaders( shaders );
	}

	std::sort( shaders.begin(), shaders.end() );
	shaders.resize( std::unique( shaders.begin(), shaders.end() ) - shaders.begin() );
}

void PipeSetup::getPriorities( const Array<Shader*>& shaders, Array<int>& priorities )
{
	priorities.clear();
	int lastpriority = 0x7FFFFFFF;
	for ( int i = 0 ; i < shaders.size() ; ++i )
	{
		int priority = shaders[i]->priority();
		if ( lastpriority != priority )
		{
			priorities.add( priority );
			lastpriority = priority;
		}
	}
	
	std::sort( priorities.begin(), priorities.end(), std::greater<int>() );
	priorities.resize( std::unique( priorities.begin(), priorities.end() ) - priorities.begin() );
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
