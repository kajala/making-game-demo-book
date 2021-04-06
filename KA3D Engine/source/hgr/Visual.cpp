#include <hgr/Visual.h>
#include <lang/Math.h>
#include <config.h>


using namespace lang;
using namespace math;


namespace hgr
{


const float Visual::MAX_BOUND = 1e10f;


Visual::Visual() :
	m_boundBoxMin( -MAX_BOUND,-MAX_BOUND,-MAX_BOUND ),
	m_boundBoxMax( MAX_BOUND,MAX_BOUND,MAX_BOUND )
{
	setClassId( NODE_VISUAL );
	setRenderable( true );
}

Visual::Visual( const Visual& other ) :
	Node( other ),
	m_boundBoxMin( other.m_boundBoxMin ),
	m_boundBoxMax( other.m_boundBoxMax )
{
}

Visual::~Visual()
{
}

void Visual::setBoundBox( const float3& boxmin, const float3& boxmax )
{
	assert( boxmin.x >= -MAX_BOUND && boxmin.x <= MAX_BOUND );
	assert( boxmin.y >= -MAX_BOUND && boxmin.y <= MAX_BOUND );
	assert( boxmin.z >= -MAX_BOUND && boxmin.z <= MAX_BOUND );
	assert( boxmax.x >= -MAX_BOUND && boxmax.x <= MAX_BOUND );
	assert( boxmax.y >= -MAX_BOUND && boxmax.y <= MAX_BOUND );
	assert( boxmax.z >= -MAX_BOUND && boxmax.z <= MAX_BOUND );

	m_boundBoxMin = boxmin;
	m_boundBoxMax = boxmax;
	setBoundModel();
}

void Visual::setBoundBoxWorld( const math::float3& boxmin, const math::float3& boxmax )
{
	assert( boxmin.x >= -MAX_BOUND && boxmin.x <= MAX_BOUND );
	assert( boxmin.y >= -MAX_BOUND && boxmin.y <= MAX_BOUND );
	assert( boxmin.z >= -MAX_BOUND && boxmin.z <= MAX_BOUND );
	assert( boxmax.x >= -MAX_BOUND && boxmax.x <= MAX_BOUND );
	assert( boxmax.y >= -MAX_BOUND && boxmax.y <= MAX_BOUND );
	assert( boxmax.z >= -MAX_BOUND && boxmax.z <= MAX_BOUND );

	m_boundBoxMin = boxmin;
	m_boundBoxMax = boxmax;
	setBoundWorld();
}

void Visual::setBoundInfinity()
{
	m_boundBoxMin = float3( -MAX_BOUND, -MAX_BOUND, -MAX_BOUND );
	m_boundBoxMax = float3( MAX_BOUND, MAX_BOUND, MAX_BOUND );
}

void Visual::getBodyTransform( float3x4* bodyworldtm,
	float3x4* bodytm, float3x4* bodyvertextm ) const
{
	assert( !isBoundWorld() ); // requires model space and
	assert( !isBoundInfinity() ); // finite bounding box

	float3 boxc = (m_boundBoxMax + m_boundBoxMin) * .5f;
	float3x4 world = worldTransform();
	float3x4 bodyworld( world.rotation().orthonormalize(), world.transform(boxc) );
	if ( bodyworldtm )
		*bodyworldtm = bodyworld;
	if ( bodyvertextm )
		*bodyvertextm = bodyworld.inverse() * world;
	if ( bodytm )
		*bodytm = world.inverse() * bodyworld;
}

inline void	Visual::setBoundModel()	
{
	setFlags( flags() & ~NODE_BOUNDWORLDSPACE );
}

inline void	Visual::setBoundWorld()	
{
	setFlags( flags() | NODE_BOUNDWORLDSPACE );
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
