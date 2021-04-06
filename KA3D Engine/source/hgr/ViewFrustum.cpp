#include <hgr/ViewFrustum.h>
#include <lang/Math.h>
#include <lang/Debug.h>
#include <math/float3x4.h>
#include <config.h>


#define MIN_HORZ_FOV 1.f
#define MAX_HORZ_FOV 179.f
#define MIN_FRONT 1e-3f
#define MAX_BACK 1e6f


using namespace lang;
using namespace math;


namespace hgr
{


ViewFrustum::ViewFrustum() :
	m_aspect( 1.33f ),
	m_front( 0.1f ),
	m_back( 10000.f ),
	m_horzFov( 1.57f )
{
}

void ViewFrustum::setHorizontalFov( float horzfov )
{
	assert( horzfov >= Math::toRadians(MIN_HORZ_FOV) && horzfov <= Math::toRadians(MAX_HORZ_FOV) );

	m_horzFov = horzfov;
}

void ViewFrustum::setFront( float front )
{
	assert( front >= MIN_FRONT && front <= MAX_BACK );
	
	m_front = front;
}

void ViewFrustum::setBack( float back )
{
	assert( back >= MIN_FRONT && back <= MAX_BACK );

	m_back = back;
}

void ViewFrustum::setAspect( float aspect )
{
	assert( aspect > 0.1f && aspect < 10.f );
	m_aspect = aspect;
}

void ViewFrustum::getViewDimensions( float* x, float* y, float* z ) const
{
    float w = 1.f / Math::tan( m_horzFov * .5f );
	if ( w < 0.f )
		w = -w;

	float vw = 2.f * m_front / w;
	float vh = vw * (1.f/m_aspect);

	*x = vw;
	*y = vh;
	*z = m_front;
}

float ViewFrustum::verticalFov() const
{
	float vx, vy, front;
	getViewDimensions( &vx, &vy, &front );
	float tana = (vy*.5f) / front;
	return 2.f * Math::atan( tana );
}

void ViewFrustum::getPlanes( const float3x4& tm, float4* planes ) const
{
	float halffovhorz = m_horzFov * 0.5f;
	float halffovvert = verticalFov() * 0.5f;
	float3 rightout = tm.rotate( float3(1,0,0).rotate( float3(0,1,0), halffovhorz ) );
	float3 leftout = tm.rotate( float3(-1,0,0).rotate( float3(0,1,0), -halffovhorz ) );
	float3 topout = tm.rotate( float3(0,1,0).rotate( float3(1,0,0), -halffovvert ) );
	float3 bottomout = tm.rotate( float3(0,-1,0).rotate( float3(1,0,0), halffovvert ) );
	float3 orig = tm.translation();
	float3 z = tm.getColumn(2);

	assert( PLANE_COUNT == 6 );
	planes[PLANE_LEFT].setPlane( leftout, orig );
	planes[PLANE_RIGHT].setPlane( rightout, orig );
	planes[PLANE_NEAR].setPlane( -z, orig+z*m_front );
	planes[PLANE_FAR].setPlane( z, orig+z*m_back );
	planes[PLANE_TOP].setPlane( topout, orig );
	planes[PLANE_BOTTOM].setPlane( bottomout, orig );
}

bool ViewFrustum::testOBox( const float3x4& boxtm,
	const float3& boxmin, const float3& boxmax, const float4* planes )
{
	for ( int i = 0 ; i < PLANE_COUNT ; ++i )
	{
		float4 p; p.w = 1.f;
		int out = 0;

		// boxmin boxmin boxmin
		p.x = boxtm(0,3) + boxtm(0,0) * boxmin.x + boxtm(0,1) * boxmin.y + boxtm(0,2) * boxmin.z;
		p.y = boxtm(1,3) + boxtm(1,0) * boxmin.x + boxtm(1,1) * boxmin.y + boxtm(1,2) * boxmin.z;
		p.z = boxtm(2,3) + boxtm(2,0) * boxmin.x + boxtm(2,1) * boxmin.y + boxtm(2,2) * boxmin.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmin boxmin boxmax
		p.x = boxtm(0,3) + boxtm(0,0) * boxmin.x + boxtm(0,1) * boxmin.y + boxtm(0,2) * boxmax.z;
		p.y = boxtm(1,3) + boxtm(1,0) * boxmin.x + boxtm(1,1) * boxmin.y + boxtm(1,2) * boxmax.z;
		p.z = boxtm(2,3) + boxtm(2,0) * boxmin.x + boxtm(2,1) * boxmin.y + boxtm(2,2) * boxmax.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmin boxmax boxmin
		p.x = boxtm(0,3) + boxtm(0,0) * boxmin.x + boxtm(0,1) * boxmax.y + boxtm(0,2) * boxmin.z;
		p.y = boxtm(1,3) + boxtm(1,0) * boxmin.x + boxtm(1,1) * boxmax.y + boxtm(1,2) * boxmin.z;
		p.z = boxtm(2,3) + boxtm(2,0) * boxmin.x + boxtm(2,1) * boxmax.y + boxtm(2,2) * boxmin.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmin boxmax boxmax
		p.x = boxtm(0,3) + boxtm(0,0) * boxmin.x + boxtm(0,1) * boxmax.y + boxtm(0,2) * boxmax.z;
		p.y = boxtm(1,3) + boxtm(1,0) * boxmin.x + boxtm(1,1) * boxmax.y + boxtm(1,2) * boxmax.z;
		p.z = boxtm(2,3) + boxtm(2,0) * boxmin.x + boxtm(2,1) * boxmax.y + boxtm(2,2) * boxmax.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmax boxmin boxmin
		p.x = boxtm(0,3) + boxtm(0,0) * boxmax.x + boxtm(0,1) * boxmin.y + boxtm(0,2) * boxmin.z;
		p.y = boxtm(1,3) + boxtm(1,0) * boxmax.x + boxtm(1,1) * boxmin.y + boxtm(1,2) * boxmin.z;
		p.z = boxtm(2,3) + boxtm(2,0) * boxmax.x + boxtm(2,1) * boxmin.y + boxtm(2,2) * boxmin.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmax boxmin boxmax
		p.x = boxtm(0,3) + boxtm(0,0) * boxmax.x + boxtm(0,1) * boxmin.y + boxtm(0,2) * boxmax.z;
		p.y = boxtm(1,3) + boxtm(1,0) * boxmax.x + boxtm(1,1) * boxmin.y + boxtm(1,2) * boxmax.z;
		p.z = boxtm(2,3) + boxtm(2,0) * boxmax.x + boxtm(2,1) * boxmin.y + boxtm(2,2) * boxmax.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmax boxmax boxmin
		p.x = boxtm(0,3) + boxtm(0,0) * boxmax.x + boxtm(0,1) * boxmax.y + boxtm(0,2) * boxmin.z;
		p.y = boxtm(1,3) + boxtm(1,0) * boxmax.x + boxtm(1,1) * boxmax.y + boxtm(1,2) * boxmin.z;
		p.z = boxtm(2,3) + boxtm(2,0) * boxmax.x + boxtm(2,1) * boxmax.y + boxtm(2,2) * boxmin.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmax boxmax boxmax
		p.x = boxtm(0,3) + boxtm(0,0) * boxmax.x + boxtm(0,1) * boxmax.y + boxtm(0,2) * boxmax.z;
		p.y = boxtm(1,3) + boxtm(1,0) * boxmax.x + boxtm(1,1) * boxmax.y + boxtm(1,2) * boxmax.z;
		p.z = boxtm(2,3) + boxtm(2,0) * boxmax.x + boxtm(2,1) * boxmax.y + boxtm(2,2) * boxmax.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		if ( 8 == out )
			return false;
	}
	return true;
}

bool ViewFrustum::testAABox( const float3& boxmin, const float3& boxmax, 
	const float4* planes )
{
	// TODO: optimize
	for ( int i = 0 ; i < PLANE_COUNT ; ++i )
	{
		float4 p; p.w = 1.f;
		int out = 0;

		// boxmin boxmin boxmin
		p.x = boxmin.x;
		p.y = boxmin.y;
		p.z = boxmin.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmin boxmin boxmax
		p.x = boxmin.x;
		p.y = boxmin.y;
		p.z = boxmax.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmin boxmax boxmin
		p.x = boxmin.x;
		p.y = boxmax.y;
		p.z = boxmin.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmin boxmax boxmax
		p.x = boxmin.x;
		p.y = boxmax.y;
		p.z = boxmax.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmax boxmin boxmin
		p.x = boxmax.x;
		p.y = boxmin.y;
		p.z = boxmin.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmax boxmin boxmax
		p.x = boxmax.x;
		p.y = boxmin.y;
		p.z = boxmax.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmax boxmax boxmin
		p.x = boxmax.x;
		p.y = boxmax.y;
		p.z = boxmin.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		// boxmax boxmax boxmax
		p.x = boxmax.x;
		p.y = boxmax.y;
		p.z = boxmax.z;
		out += ( dot(p,planes[i]) > 0.f ? 1 : 0 );

		if ( 8 == out )
			return false;
	}
	return true;
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
