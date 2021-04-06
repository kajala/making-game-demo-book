#include <hgr/SceneOutputStream.h>
#include <gr/Primitive.h>
#include <gr/VertexFormat.h>
#include <io/IOException.h>
#include <hgr/Mesh.h>
#include <hgr/Lines.h>
#include <hgr/Light.h>
#include <hgr/Dummy.h>
#include <hgr/Camera.h>
#include <hgr/UserPropertySet.h>
#include <hgr/TransformAnimation.h>
#include <math/float.h>
#include <math/float3x4.h>
#include <math/quaternion.h>
#include <config.h>


using namespace gr;
using namespace io;
using namespace lang;
using namespace math;


namespace hgr
{


const int SceneOutputStream::VERSION = 170;


SceneOutputStream::SceneOutputStream( OutputStream* out, int dataflags ) :
	DataOutputStream( out ),
	m_dataFlags( dataflags )
{
	writeChars( "hgrf" );
	writeShort( VERSION );
	writeShort( dataflags );
}

SceneOutputStream::~SceneOutputStream()
{
}

void SceneOutputStream::writeFloat3( const float3& obj )
{
	writeFloat( obj.x );
	writeFloat( obj.y );
	writeFloat( obj.z );
}

void SceneOutputStream::writeFloat4( const float4& obj )
{
	writeFloat( obj.x );
	writeFloat( obj.y );
	writeFloat( obj.z );
	writeFloat( obj.w );
}

void SceneOutputStream::writeQuaternion( const quaternion& obj )
{
	writeFloat( obj.x );
	writeFloat( obj.y );
	writeFloat( obj.z );
	writeFloat( obj.w );
}

void SceneOutputStream::writeFloat3x4( const float3x4& obj )
{
	for ( int i = 0 ; i < float3x4::ROWS ; ++i )
		for ( int j = 0 ; j < float3x4::COLUMNS ; ++j )
			writeFloat( obj(i,j) );
}

void SceneOutputStream::writeVertexFormat( const VertexFormat& vf )
{
	int count = 0;
	for ( int i = 0 ; i < VertexFormat::DT_SIZE ; ++i )
	{
		VertexFormat::DataType dt = (VertexFormat::DataType)i;
		if ( vf.hasData(dt) )
			++count;
	}
	writeByte( count );

	for ( int i = 0 ; i < VertexFormat::DT_SIZE ; ++i )
	{
		VertexFormat::DataType dt = (VertexFormat::DataType)i;
		VertexFormat::DataFormat df = vf.getDataFormat(dt);
		if ( df != VertexFormat::DF_NONE )
		{
			writeUTF( VertexFormat::toString(dt) );
			writeUTF( VertexFormat::toString(df) );
		}
	}
}

void SceneOutputStream::writePrimitive( Primitive* obj, const Array<Shader*>& globshaderlist )
{
	Primitive::Lock lock( obj, Primitive::LOCK_READ );

	const int verts = obj->vertices();
	writeShort( verts );
	writeShort( obj->indices() );
	
	VertexFormat vf = obj->vertexFormat();
	writeVertexFormat( vf );

	int matindex = globshaderlist.indexOf( obj->shader() );
	if ( matindex < 0 )
		throwError( IOException( Format("Shader which was used in primitive is not listed in global shader list of scene file \"{0}\"", toString()) ) );
	writeShort( matindex );

	writeShort( obj->type() );

	for ( int i = 0 ; i < VertexFormat::DT_SIZE ; ++i )
	{
		VertexFormat::DataType dt = (VertexFormat::DataType)i;
		VertexFormat::DataFormat df = vf.getDataFormat( dt );

		if ( VertexFormat::DF_NONE != df )
		{
			writeFloat( 1.f );
			writeFloat4( float4(0,0,0,0) );

			uint8_t* data = 0;
			int pitch = 0;
			obj->getVertexDataPtr( dt, &data, &pitch );
			int vsize = VertexFormat::getDataSize( df );
	
			// WARNING: Endianess dependent
			for ( int n = 0 ; n < verts ; ++n )
			{
				write( data, vsize );
				data += pitch;
			}
		}
	}

	uint16_t* indexdata = 0;
	int indexsize = 0;
	obj->getIndexDataPtr( &indexdata, &indexsize );
	if ( indexsize != 2 )
		throwError( IOException( Format("Index size not 2 in scene file \"{0}\"", toString()) ) );
	// WARNING: Endianess dependent
	write( indexdata, indexsize*obj->indices() );
	for ( int i = 0 ; i < obj->indices() ; ++i )
	{
		if ( (int)indexdata[i] >= verts )
			throwError( IOException( Format("Wrote out-of-bounds index data in scene file \"{0}\"", toString()) ) );
	}

	writeByte( obj->usedBones() );
	write( obj->usedBoneArray(), obj->usedBones() );
}

void SceneOutputStream::writeNode( Node* obj, const Array<Node*>& globnodelist )
{
	writeUTF( obj->name() );

	writeFloat3x4( obj->transform() );

	writeInt( obj->flags() );

	int parentix = -1;
	if ( obj->parent() != 0 )
	{
		parentix = globnodelist.indexOf( obj->parent() );
		if ( parentix < 0 )
			throwError( IOException( Format("Parent node of {0} not in global node list in scene file \"{1}\"", obj->name(), toString()) ) );
	}
	writeInt( parentix );
}

void SceneOutputStream::writeMesh( Mesh* obj,
	const Array<Primitive*>& globprimitivelist,
	const Array<Node*>& globnodelist )
{
	writeNode( obj, globnodelist );

	writeInt( obj->primitives() );
	for ( int i = 0 ; i < obj->primitives() ; ++i )
	{
		int primix = globprimitivelist.indexOf( obj->getPrimitive(i) );
		if ( primix < 0 )
			throwError( IOException( Format("Primitive of {0} not in global primitive list in scene file \"{1}\"", obj->name(), toString()) ) );

		writeInt( primix );
	}

	writeInt( obj->bones() );
	for ( int i = 0 ; i < obj->bones() ; ++i )
	{
		int boneix = globnodelist.indexOf( obj->getBone(i) );
		if ( boneix < 0 )
			throwError( IOException( Format("Bone of {0} not in global node list in scene file \"{1}\"", obj->name(), toString()) ) );

		writeInt( boneix );
		writeFloat3x4( obj->getBoneInverseRestTransform(i) );
	}
}

void SceneOutputStream::writeCamera( Camera* obj, const Array<Node*>& globnodelist )
{
	writeNode( obj, globnodelist );

	writeFloat( obj->front() );
	writeFloat( obj->back() );
	writeFloat( obj->horizontalFov() );
}

void SceneOutputStream::writeLight( Light* obj, const Array<Node*>& globnodelist )
{
	writeNode( obj, globnodelist );

	writeFloat3( obj->color() );
	writeFloat( 0.f );
	writeFloat( 0.f );
	writeFloat( obj->farAttenStart() );
	writeFloat( obj->farAttenEnd() );
	writeFloat( obj->innerCone() );
	writeFloat( obj->outerCone() );
	writeByte( obj->type() );
}

void SceneOutputStream::writeDummy( Dummy* obj, const Array<Node*>& globnodelist )
{
	writeNode( obj, globnodelist );

	writeFloat3( obj->boxMin() );
	writeFloat3( obj->boxMax() );
}

void SceneOutputStream::writeLines( Lines* obj, const Array<Node*>& globnodelist )
{
	writeNode( obj, globnodelist );

	writeInt( obj->lines() );
	writeInt( obj->paths() );

	for ( int i = 0 ; i < obj->lines() ; ++i )
	{
		writeFloat3( obj->getLineStart(i) );
		writeFloat3( obj->getLineEnd(i) );
	}

	for ( int i = 0 ; i < obj->paths() ; ++i )
	{
		writeInt( obj->getPathBegin(i) );
		writeInt( obj->getPathEnd(i) );
	}
}

void SceneOutputStream::writeTransformAnimation( TransformAnimation* obj )
{
	writeByte( obj->positionKeyRate() );
	writeByte( obj->rotationKeyRate() );
	writeByte( obj->scaleKeyRate() );
	writeByte( obj->endBehaviour() );

	writeKeyframeSequence( obj->positionKeyframeSequence() );
	writeKeyframeSequence( obj->rotationKeyframeSequence() );
	writeKeyframeSequence( obj->scaleKeyframeSequence() );
}

void SceneOutputStream::writeKeyframeSequence( KeyframeSequence* obj )
{
	int keys = 0;
	VertexFormat::DataFormat format = VertexFormat::DataFormat();
	float scale = 1.f;
	float4 bias(0,0,0,0);
	
	if ( obj != 0 )
	{
		keys = obj->keys();
		format = obj->format();
		scale = obj->scale();
		bias = obj->bias();
	}

	writeInt( keys );
	writeUTF( VertexFormat::toString(format) );
	writeFloat( scale );
	writeFloat4( bias );

	if ( keys > 0 )
		write( obj->data(), VertexFormat::getDataSize(format,keys) );
}

void SceneOutputStream::writeUserPropertySet( UserPropertySet* obj )
{
	writeInt( obj->size() );
	for ( HashtableIterator<String,String> it = obj->begin() ; it != obj->end() ; ++it )
	{
		writeUTF( it.key() );
		writeUTF( it.value() );
	}
}

bool SceneOutputStream::hasData( int flags ) const
{
	return 0 != (m_dataFlags & flags);
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
