#include <hgr/TransformAnimation.h>
#include <gr/GraphicsException.h>
#include <lang/Math.h>
#include <lang/Float.h>
#include <lang/Array.h>
#include <lang/Debug.h>
#include <lang/String.h>
#include <lang/OutOfMemoryException.h>
#include <math/float3.h>
#include <math/float3x3.h>
#include <math/float3x4.h>
#include <math/quaternion.h>
#include <math/InterpolationUtil.h>
#include <string.h>
#include <config.h>


using namespace gr;
using namespace lang;
using namespace math;


namespace hgr
{


TransformAnimation::TransformAnimation( BehaviourType endbehaviour, 
	int poskeys, int rotkeys, int sclkeys,
	int poskeyrate, int rotkeyrate, int sclkeyrate ) :
	m_pos( 0 ),
	m_rot( 0 ),
	m_scl( 0 ),
	m_poskeyrate( (uint8_t)poskeyrate ),
	m_rotkeyrate( (uint8_t)rotkeyrate ),
	m_sclkeyrate( (uint8_t)sclkeyrate ),
	m_endbehaviour( (uint8_t)endbehaviour )
{
	assert( rotkeys > 0 );
	assert( poskeys > 0 );

	m_pos = new KeyframeSequence( poskeys, VertexFormat::DF_V3_32 );
	m_rot = new KeyframeSequence( rotkeys, VertexFormat::DF_V4_32 );
	if ( sclkeys > 0 )
		m_scl = new KeyframeSequence( sclkeys, VertexFormat::DF_V3_32 );

	for ( int i = 0 ; i < poskeys ; ++i )
		setPositionKey( i, float3(0,0,0) );
	for ( int i = 0 ; i < rotkeys ; ++i )
		setRotationKey( i, quaternion(0,0,0,1) );
	for ( int i = 0 ; i < sclkeys ; ++i )
		setScaleKey( i, float3(1,1,1) );
}

TransformAnimation::TransformAnimation( BehaviourType endbehaviour,
	KeyframeSequence* poskeys, KeyframeSequence* rotkeys, KeyframeSequence* sclkeys,
	int poskeyrate, int rotkeyrate, int sclkeyrate ) :
	m_pos( poskeys ),
	m_rot( rotkeys ),
	m_scl( sclkeys ),
	m_poskeyrate( (uint8_t)poskeyrate ),
	m_rotkeyrate( (uint8_t)rotkeyrate ),
	m_sclkeyrate( (uint8_t)sclkeyrate ),
	m_endbehaviour( (uint8_t)endbehaviour )
{
	assert( rotkeys != 0 );
	assert( poskeys != 0 );
}

TransformAnimation::~TransformAnimation()
{
}

void TransformAnimation::eval( float time, float3* pos, quaternion* rot, float3* scl )
{
	assert( positionKeys() > 0 );
	assert( rotationKeys() > 0 );

	int frames[4];
	float u;

	// position (Catmull-Rom)
	InterpolationUtil::getFrame( time, (InterpolationUtil::BehaviourType)endBehaviour(),
		positionKeys(), m_poskeyrate, frames, &u );

	float3 posk0 = getPositionKey(frames[0]);
	float3 posk1 = getPositionKey(frames[1]);
	float3 posk2 = getPositionKey(frames[2]);
	float3 posk3 = getPositionKey(frames[3]);
	InterpolationUtil::interpolateVectorCatmullRom( 3, u,
		&posk0.x, &posk1.x, &posk2.x, &posk3.x,
		&pos->x );

	// rotation (spherical linear quaternion interpolation)
	InterpolationUtil::getFrame( time, 
		(InterpolationUtil::BehaviourType)endBehaviour(),
		rotationKeys(), m_rotkeyrate, frames, &u );

	quaternion rotq1 = getRotationKey( frames[1] );
	quaternion rotq2 = getRotationKey( frames[2] );
	if ( rotq1.dot(rotq2) < 0.f )
		rotq2 = -rotq2;
	*rot = rotq1.slerp( u, rotq2 ).normalize();

	// scale (Catmull-Rom)
	if ( scaleKeys() > 0 )
	{
		InterpolationUtil::getFrame( time, (InterpolationUtil::BehaviourType)endBehaviour(),
			scaleKeys(), m_sclkeyrate, frames, &u );

		float3 scalek0 = getScaleKey(frames[0]);
		float3 scalek1 = getScaleKey(frames[1]);
		float3 scalek2 = getScaleKey(frames[2]);
		float3 scalek3 = getScaleKey(frames[3]);
		InterpolationUtil::interpolateVectorCatmullRom( 3, u,
			&scalek0.x, &scalek1.x, &scalek2.x, &scalek3.x,
			&scl->x );
	}
	else
	{
		scl->x = 1.f;
		scl->y = 1.f;
		scl->z = 1.f;
	}
}

void TransformAnimation::eval( float time, float3x4* tm )
{
	float3 pos, scl;
	quaternion rotq;
	eval( time, &pos, &rotq, &scl );
	*tm = float3x4( rotq, pos, scl );
}

void TransformAnimation::setEndBehaviour( BehaviourType endbehaviour )
{
	m_endbehaviour = (uint8_t)endbehaviour;
}

void TransformAnimation::setPositionKey( int i, const float3& v )
{
	float iscale = 1.f / m_pos->scale();
	m_pos->setKeyframe( i, &v.x, sizeof(v), VertexFormat::DF_V4_32, iscale, m_pos->bias()*-iscale, 1 );
}

void TransformAnimation::setRotationKey( int i, const quaternion& v )
{
	float iscale = 1.f / m_rot->scale();
	m_rot->setKeyframe( i, &v.x, sizeof(v), VertexFormat::DF_V4_32, iscale, m_rot->bias()*-iscale, 1 );
}

void TransformAnimation::setScaleKey( int i, const float3& v )
{
	float iscale = 1.f / m_scl->scale();
	m_scl->setKeyframe( i, &v.x, sizeof(v), VertexFormat::DF_V3_32, iscale, m_scl->bias()*-iscale, 1 );
}

void TransformAnimation::optimize()
{
	if ( positionKeys() > 0 )
	{
		m_pos->removeRedundantKeys();
		//if ( m_pos->keys() > 1 && VertexFormat::DF_V3_16 != m_pos->format() )
		//	m_pos = m_pos->getPacked( VertexFormat::DF_V3_16, 32767.f );
	}

	if ( rotationKeys() > 0 )
	{
		m_rot->removeRedundantKeys();
		//if ( m_rot->keys() > 1 && VertexFormat::DF_V4_16 != m_rot->format() )
		//	m_rot = m_rot->getPacked( VertexFormat::DF_V4_16, 32767.f );
	}

	if ( scaleKeys() > 0 )
	{
		m_scl->removeRedundantKeys();
		//if ( m_scl->keys() > 1 && VertexFormat::DF_V3_16 != m_scl->format() )
		//	m_scl = m_scl->getPacked( VertexFormat::DF_V3_16, 32767.f );
	}
}

void TransformAnimation::rotate( const float3x3& rot )
{
	for ( int i = 0 ; i < rotationKeys() ; ++i )
	{
		quaternion q0 = getRotationKey( i );
		quaternion q1( rot * float3x3(q0) );
		setRotationKey( i, q1 );
	}

	for ( int i = 0 ; i < positionKeys() ; ++i )
	{
		float3 v = getPositionKey( i );
		v = rot.rotate(v);
		setPositionKey( i, v );
	}
}

void TransformAnimation::scale( float s )
{
	m_pos->setScale( m_pos->scale()*s );
	m_pos->setBias( m_pos->bias()*s );

	if ( m_scl )
	{
		m_scl->setScale( m_scl->scale()*s );
		m_scl->setBias( m_scl->bias()*s );
	}
}

float3 TransformAnimation::getLinearVelocity( float time ) const
{
	assert( positionKeys() > 0 );

	if ( positionKeys() < 2 )
		return float3(0,0,0);
	
	int frame0 = int( time * m_poskeyrate ) % positionKeys();
	int frame1 = frame0 + 1;
	if ( frame1 >= positionKeys() )
	{
		--frame0;
		--frame1;
	}
	assert( frame0 >= 0 && frame0 < positionKeys() );
	assert( frame1 >= 0 && frame1 < positionKeys() );

	float3 p0 = getPositionKey( frame0 );
	float3 p1 = getPositionKey( frame1 );

	p1 -= p0;
	p1 *= m_poskeyrate;
	return p1;
}

TransformAnimation::BehaviourType TransformAnimation::toBehaviour( const String& str )
{
	const char* sz[] = 
	{
		"REPEAT",
		"OSCILLATE",
		"RESET",
		"STOP" 
	};
	assert( sizeof(sz)/sizeof(sz[0]) == BEHAVIOUR_COUNT );

	String upper = str.toUpperCase();
	for ( int i = 0 ; i < BEHAVIOUR_COUNT ; ++i )
		if ( upper == sz[i] )
			return (BehaviourType)i;

	throwError( GraphicsException( Format("Invalid animation end behavior: {0}", str) ) );
	return BEHAVIOUR_COUNT;
}

float3 TransformAnimation::getPositionKey( int i ) const
{
	float3 v;
	m_pos->getKeyframe( i, &v.x, sizeof(v), VertexFormat::DF_V3_32, 1 );
	return v;
}

quaternion TransformAnimation::getRotationKey( int i ) const
{
	quaternion v;
	m_rot->getKeyframe( i, &v.x, sizeof(v), VertexFormat::DF_V4_32, 1 );
	return v;
}

float3 TransformAnimation::getScaleKey( int i ) const
{
	float3 v;
	m_scl->getKeyframe( i, &v.x, sizeof(v), VertexFormat::DF_V3_32, 1 );
	return v;
}

int	TransformAnimation::positionKeys() const
{
	return m_pos->keys();
}

int TransformAnimation::rotationKeys() const
{
	return m_rot->keys();
}

int TransformAnimation::scaleKeys() const
{
	return m_scl != 0 ? m_scl->keys() : 0;
}

int TransformAnimation::positionKeyRate() const
{
	return m_poskeyrate;
}

int TransformAnimation::rotationKeyRate() const
{
	return m_rotkeyrate;
}

int TransformAnimation::scaleKeyRate() const
{
	return m_sclkeyrate;
}

TransformAnimation::BehaviourType TransformAnimation::endBehaviour() const	
{
	return (BehaviourType)m_endbehaviour;
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
