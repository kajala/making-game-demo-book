#include <hgr/Camera.h>
#include <gr/Shader.h>
#include <gr/Context.h>
#include <hgr/Light.h>
#include <hgr/Visual.h>
#include <hgr/PipeSetup.h>
#include <lang/Debug.h>
#include <lang/Profile.h>
#include <string.h>
#include <algorithm>
#include <config.h>


using namespace gr;
using namespace math;
using namespace lang;


namespace hgr
{


void Camera::Statistics::reset()
{
	memset( this, 0, sizeof(Statistics) );
}


Camera::Camera() :
	m_frustum(),
	m_worldtm( 1.f ),
	m_viewtm( 1.f ),
	m_projtm( 1.f ),
	m_viewprojtm( 1.f ),
	m_screenviewprojtm( 1.f ),
	m_lightSorterPtr( 0 )
{
	setClassId( NODE_CAMERA );
}

Camera::Camera( const Camera& other ) : 
	Node( other ),
	m_frustum( other.m_frustum ),
	m_worldtm( other.m_worldtm ),
	m_viewtm( other.m_viewtm ),
	m_projtm( other.m_projtm ),
	m_viewprojtm( other.m_viewprojtm ),
	m_screenviewprojtm( other.m_screenviewprojtm ),
	m_lightSorterPtr( other.m_lightSorterPtr )
{
	setClassId( NODE_CAMERA );
}

Camera::~Camera()
{
}

void Camera::setHorizontalFov( float horzFov )
{
	m_frustum.setHorizontalFov( horzFov );
}

void Camera::setFront( float front )
{
	m_frustum.setFront( front );
}

void Camera::setBack( float back )													
{
	m_frustum.setBack( back );
}

float4x4 Camera::projectionTransform() const
{
	float4x4 proj;
	proj.setPerspectiveProjection( horizontalFov(), front(), back(), m_frustum.aspect() );
	return proj;
}

float3x4 Camera::viewTransform() const
{
	return worldTransform().inverse();
}

float4x4 Camera::viewProjectionTransform() const
{
	return projectionTransform() * viewTransform();
}

void Camera::render( Context* context )
{
	PipeSetup::getNodes( this, m_nodes );
	PipeSetup::getLights( m_nodes, m_lightSorter );
	cacheTransforms( context, m_nodes );
	cullVisuals( m_nodes, m_visuals );
	PipeSetup::getShaders( m_visuals, m_shaders );
	PipeSetup::getPriorities( m_shaders, m_priorities );

	render( context, -100, 100, m_visuals, m_priorities, &m_lightSorter );
}

void Camera::render( Context* context, int minpriority, int maxpriority,
	const Array<Visual*>& visuals, const Array<int>& priorities,
	LightSorter* lightsorter )
{
	const int visualcount = visuals.size();
	const int prioritycount = priorities.size();

	m_lightSorterPtr = lightsorter;

	for ( int k = 0 ; k < prioritycount ; ++k )
	{
		int priority = priorities[k];
		if ( priority >= minpriority && priority <= maxpriority )
		{
			if ( (priority&1) == 0 )
			{
				// even priority materials are rendered front-to-back
				for ( int i = 0 ; i < visualcount ; ++i )
					visuals[i]->render( context, this, priority );
			}
			else
			{
				// odd priority materials are rendered back-to-front
				for ( int i = visualcount-1 ; i >= 0 ; --i )
					visuals[i]->render( context, this, priority );
			}
		}
	}

	m_lightSorterPtr = 0;
}

float Camera::front() const															
{
	return m_frustum.front();
}

float Camera::back() const															
{
	return m_frustum.back();
}

float Camera::horizontalFov() const
{
	return m_frustum.horizontalFov();
}

float Camera::verticalFov() const
{
	return m_frustum.verticalFov();
}

Array<Light*>& Camera::getLightsSortedByDistance( const float3& worldpos ) const
{
	assert( m_lightSorterPtr ); // if 0 then not in rendering
	return m_lightSorterPtr->getLightsByDistance( worldpos );
}

void Camera::cullVisuals( const Array<Node*>& nodes, Array<Visual*>& visuals )
{
	assert( m_viewtm == viewTransform() ); // cached transforms not up-to-date

	// view frustum in world space
	float4 frustumworld[ViewFrustum::PLANE_COUNT];
	m_frustum.getPlanes( cachedWorldTransform(), frustumworld );

	// cull visuals by bounding boxes
	m_visualSorter.clear();
	for ( int i = 0 ; i < nodes.size() ; ++i )
	{
		Node* node = nodes[i];

		if ( NODE_VISUAL == node->classId() )
		{
			Visual* vis = static_cast<Visual*>( node );
			if ( vis->renderable() )
			{
				const float3x4& worldtm = getCachedWorldTransform( vis );
				bool visible = vis->isBoundInfinity();

				if ( !visible )
				{
					if ( vis->isBoundWorld() )
						visible = ViewFrustum::testAABox( vis->boundBoxMinWorld(), vis->boundBoxMaxWorld(), frustumworld );
					else
						visible = ViewFrustum::testOBox( worldtm, vis->boundBoxMin(), vis->boundBoxMax(), frustumworld );
				}

				if ( visible )
				{
					VisualSorter objdata;
					objdata.depth = m_viewtm(2,0)*worldtm(0,3) + m_viewtm(2,1)*worldtm(1,3) + m_viewtm(2,2)*worldtm(2,3) + m_viewtm(2,3);
					objdata.obj = vis;
					m_visualSorter.add( objdata );
				}
				else
				{
					statistics.visualsBeforeCull++;
				}
			}
		}
	}

	// sort visuals by Z-distance to camera (smaller camera space Z first)
	std::sort( m_visualSorter.begin(), m_visualSorter.end() );
	
	// return visuals in the user array
	visuals.resize( m_visualSorter.size() );
	for ( int i = 0 ; i < visuals.size() ; ++i )
		visuals[i] = m_visualSorter[i].obj;

	// update stats
	statistics.visualsBeforeCull += m_visualSorter.size();
	statistics.visualsAfterCull += m_visualSorter.size();
}

void Camera::cacheTransforms( Context* context, const Array<Node*>& nodes )
{
	Array<float3x4>& tmcache = m_worldTransformCache;
	tmcache.clear();

	for ( int i = 0 ; i < nodes.size() ; ++i )
	{
		assert( tmcache.size() < 0x10000 );

		Node* node = nodes[i];
		node->m_tmindex = (short)tmcache.size();

		Node* parent = node->parent();
		if ( 0 == parent )
			tmcache.add( node->transform() );
		else
			tmcache.add( tmcache[parent->m_tmindex] * node->transform() );
	}

	m_frustum.setAspect( context->aspect() );
	m_worldtm = tmcache[m_tmindex];
	m_viewtm = m_worldtm.inverse();
	m_projtm = projectionTransform();
	m_viewprojtm = viewProjectionTransform();
	m_screenviewprojtm = context->screenTransform() * m_viewprojtm;
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
