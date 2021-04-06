#include <hgr/Mesh.h>
#include <hgr/Camera.h>
#include <gr/Context.h>
#include <gr/GraphicsException.h>
#include <hgr/Light.h>
#include <lang/Math.h>
#include <lang/Debug.h>
#include <lang/String.h>
#include <lang/Profile.h>
#include <config.h>


using namespace gr;
using namespace lang;
using namespace math;


namespace hgr
{


Mesh::Mesh()
{
}

Mesh::Mesh( const Mesh& other ) :
	Visual( other ),
	m_primitives( other.m_primitives ),
	m_lights( other.m_lights ),
	m_bones( other.m_bones )
{
}

Mesh::~Mesh()
{
}

void Mesh::render( Context* /*context*/, Camera* camera, int priority )
{
	bool					init = true;
	Camera::TempBuffers&	tmp = camera->tempBuffers();
	const float3x4&			worldtm = camera->getCachedWorldTransform( this );
	const float4x4&			viewtm = camera->cachedViewTransform();
	const float4x4&			viewtminv = camera->cachedWorldTransform();
	const float4x4&			viewprojtm = camera->cachedViewProjectionTransform();
	const float4x4&			screenviewprojtm = camera->cachedScreenViewProjectionTransform();
	Array<Light*>*			lights = 0;
	float4x4				totaltm;

	for ( int i = 0 ; i < m_primitives.size() ; ++i )
	{
		Primitive* prim = m_primitives[i];
		Shader* fx = prim->shader();

		if ( fx->priority() == priority )
		{
			if ( init )
			{
				init = false;
				totaltm = screenviewprojtm * worldtm;;
				lights = m_lights.size() > 0 ? &m_lights : &camera->getLightsSortedByDistance( worldtm.translation() );

				// get transforms for all bones
				if ( m_bones.size() > 0 )
				{
					tmp.bonematrices.resize( m_bones.size() );
					getBoneMatrix4x3Array( camera, tmp.bonematrices.begin(), tmp.bonematrices.size() );
				}
			}

			Shader::Begin use( fx );

			int passes = use.passes();
			if ( passes > 0 )
			{
				fx->setVector( Shader::PARAM_CAMERAP, float4(camera->cachedWorldTransform().translation(),1.f) );
				fx->setMatrix( Shader::PARAM_TOTALTM, totaltm );
				fx->setMatrix( Shader::PARAM_WORLDTM, worldtm );
				fx->setMatrix( Shader::PARAM_VIEWTM, viewtm );
				fx->setMatrix( Shader::PARAM_VIEWTMINV, viewtminv );
				fx->setMatrix( Shader::PARAM_VIEWPROJTM, viewprojtm );

				for ( int k = 0 ; k < lights->size() ; ++k )
				{
					Light* lt = lights->begin()[k];
					fx->setVector( Shader::ParamType(Shader::PARAM_LIGHTP0+k), float4(camera->getCachedWorldTransform(lt).translation(),1.f) );
					fx->setVector( Shader::ParamType(Shader::PARAM_LIGHTC0+k), float4(lt->color(),1.f) );
				}

				if ( m_bones.size() > 0 )
				{
					int usedbones = prim->usedBones();
					tmp.bonematrixp.resize( usedbones );
					const uint8_t* usedbonearray = prim->usedBoneArray();
					for ( int k = 0 ; k < usedbones ; ++k )
						tmp.bonematrixp[k] = &tmp.bonematrices[ usedbonearray[k] ];
					fx->setMatrixArray( Shader::PARAM_BONEWORLDTM, tmp.bonematrixp.begin(), tmp.bonematrixp.size() );
				}

				for ( int k = 0 ; k < passes ; ++k )
				{
					Shader::Pass pass( fx, k );
					prim->render();
				}
			}
		}
	}
}

void Mesh::addPrimitive( Primitive* primitive )
{
	assert( primitive );
	m_primitives.add( primitive );
}

void Mesh::removePrimitive( int index )
{
	assert( index >= 0 && index < primitives() );
	m_primitives.remove( index );
}

void Mesh::removePrimitives()
{
	m_primitives.clear();
}

void Mesh::addBone( Node* bone, const float3x4& invresttm )
{
	assert( !hasParent(bone) );

	Bone bonedata;
	bonedata.invresttm = invresttm;
	bonedata.node = bone;
	m_bones.add( bonedata );
}

void Mesh::removeBone( int index )
{
	assert( index >= 0 && index < bones() );
	m_bones.remove( index );
}

Node* Mesh::getBone( int index ) const
{
	assert( index >= 0 && index < bones() );
	return m_bones[index].node;
}

int Mesh::bones() const
{
	return m_bones.size();
}

void Mesh::getBoneMatrix4x3Array( Camera* camera, float4x4* tm, int count ) const
{
	assert( count == bones() );
	assert( count > 0 );

	for ( int i = 0 ; i < count ; ++i )
	{
		const Bone& bone = m_bones[i];
		float4x4& m = tm[i];
		m = camera->getCachedWorldTransform( bone.node ) * bone.invresttm;
		m.setRow( 3, m.getColumn(3) );
		m.setColumn( 3, float4(0,0,0,0) );
	}
}

const float3x4& Mesh::getBoneInverseRestTransform( int index ) const
{
	assert( index >= 0 && index < bones() );
	return m_bones[index].invresttm;
}

void Mesh::getShaders( Array<Shader*>& shaders )
{
	for ( int k = 0 ; k < m_primitives.size() ; ++k )
		shaders.add( m_primitives[k]->shader() );
}

void Mesh::addLight( Light* lt )
{
	assert( m_lights.size() < Shader::MAX_LIGHTS );

	m_lights.add( lt );
}

void Mesh::removeLights()
{
	m_lights.clear();
}

void Mesh::removeLight( int i )
{
	assert( i >= 0 && i < m_lights.size() );
	m_lights.remove( i );
}

void Mesh::computeBound()
{
	float3 minv( MAX_BOUND, MAX_BOUND, MAX_BOUND );
	float3 maxv( -MAX_BOUND, -MAX_BOUND, -MAX_BOUND );
	const int VERTEX_BATCH = 16;
	float4 verts[VERTEX_BATCH];
	
	for ( int i = 0 ; i < m_primitives.size() ; ++i )
	{
		Primitive* prim = m_primitives[i];
		if ( prim->vertexFormat().hasData(VertexFormat::DT_BONEWEIGHTS) )
		{
			setBoundInfinity();
			return;
		}

		Primitive::Lock lk( prim, Primitive::LOCK_READ );
		const int vertices = prim->vertices();

		for ( int k = 0 ; k < vertices ; )
		{
			int c = vertices - k;
			if ( c > VERTEX_BATCH )
				c = VERTEX_BATCH;
			prim->getVertexPositions( k, verts, c );

			for ( int n = 0 ; n < c ; ++n )
			{
				const float4& v = verts[n];
				minv.x = Math::min( v.x, minv.x );
				minv.y = Math::min( v.y, minv.y );
				minv.z = Math::min( v.z, minv.z );
				maxv.x = Math::max( v.x, maxv.x );
				maxv.y = Math::max( v.y, maxv.y );
				maxv.z = Math::max( v.z, maxv.z );
			}

			k += c;
		}
	}

	if ( minv.x > maxv.x )
		minv = maxv = float3(0,0,0);
	setBoundBox( minv, maxv );
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
