#include <hgr/SceneInputStream.h>
#include <hgr/Scene.h>
#include <gr/Context.h>
#include <gr/impl/DIPrimitive.h>
#include <io/IOException.h>
#include <lang/Debug.h>
#include <math/float3x4.h>
#include <math/quaternion.h>
#include <string.h>
#include <config.h>


using namespace gr;
using namespace io;
using namespace lang;
using namespace math;


namespace hgr
{


const int SceneInputStream::MIN_VERSION = 170;
const int SceneInputStream::MAX_VERSION = 170;


SceneInputStream::SceneInputStream( InputStream* in ) :
	DataInputStream( in ),
	m_ver( 0 ),
	m_dataFlags( 0 )
{
	char magic[4];
	read( magic, sizeof(magic) );
	if ( memcmp(magic,"hgrf",4) )
		throwError( IOException( Format("Scene file \"{0}\" does not start with 'hgrf' tag", toString()) ) );
	
	m_ver = readShort();
	if ( m_ver < MIN_VERSION || m_ver > MAX_VERSION )
		throwError( IOException( Format("Scene file \"{0}\" is version {1}.{2}, should be {3}.{4}", toString(), m_ver/100, m_ver%100, MAX_VERSION/100, MAX_VERSION%100) ) );

	if ( m_ver >= 140 )
		m_dataFlags = readShort();
	else
		m_dataFlags = uint16_t(-1);
}

SceneInputStream::~SceneInputStream()
{
}

float3 SceneInputStream::readFloat3()
{
	float3 obj;
	obj.x = readFloat();
	obj.y = readFloat();
	obj.z = readFloat();
	return obj;
}

float4 SceneInputStream::readFloat4()
{
	float4 obj;
	obj.x = readFloat();
	obj.y = readFloat();
	obj.z = readFloat();
	obj.w = readFloat();
	return obj;
}

quaternion SceneInputStream::readQuaternion()
{
	quaternion obj;
	obj.x = readFloat();
	obj.y = readFloat();
	obj.z = readFloat();
	obj.w = readFloat();
	return obj;
}

float3x4 SceneInputStream::readFloat3x4()
{
	float3x4 obj;
	for ( int i = 0 ; i < float3x4::ROWS ; ++i )
		for ( int j = 0 ; j < float3x4::COLUMNS ; ++j )
			obj(i,j) = readFloat();
	return obj;
}

void SceneInputStream::readNode( Node* node, int* parentindex )
{
	node->setName( readUTF() );
	node->setTransform( readFloat3x4() );
	node->setFlags( readInt() );
	*parentindex = readInt();
}

P(TransformAnimation) SceneInputStream::readTransformAnimation()
{
	int poskeyrate = readByte();
	int rotkeyrate = readByte();
	int sclkeyrate = readByte();
	TransformAnimation::BehaviourType endbehaviour = (TransformAnimation::BehaviourType)readByte();

	if ( endbehaviour >= TransformAnimation::BEHAVIOUR_COUNT )
		throwError( IOException( Format("Failed to load scene \"{0}\". Invalid transform animation end behaviour ({1}).", toString(), int(endbehaviour)) ) );

	P(KeyframeSequence) pos = readKeyframeSequence();
	P(KeyframeSequence) rot = readKeyframeSequence();
	P(KeyframeSequence) scl = readKeyframeSequence();
	P(TransformAnimation) obj = new TransformAnimation( endbehaviour, pos, rot, scl, poskeyrate, rotkeyrate, sclkeyrate );
	return obj;
}

P(KeyframeSequence) SceneInputStream::readKeyframeSequence()
{
	int keys = readInt();
	
	char buf[256];
	readUTF( buf, sizeof(buf) );
	VertexFormat::DataFormat format = VertexFormat::toDataFormat( buf );

	float scale = readFloat();
	float4 bias = readFloat4();

	if ( 0 ==keys )
		return 0;

	P(KeyframeSequence) obj = new KeyframeSequence( keys, format );
	obj->setScale( scale );
	obj->setBias( bias );
	readFully( obj->data(), VertexFormat::getDataSize(format,keys) );
	return obj;
}

void SceneInputStream::readVertexFormat( VertexFormat* vf )
{
	char buf[256];
	int count = readByte();
	for ( int i = 0 ; i < count ; ++i )
	{
		readUTF( buf, sizeof(buf) );
		VertexFormat::DataType dt = VertexFormat::toDataType( buf );
		if ( dt == VertexFormat::DT_SIZE )
			throwError( IOException( Format("Invalid vertex data type in \"{0}\": {1}", toString(), buf) ) );

		readUTF( buf, sizeof(buf) );
		VertexFormat::DataFormat df = VertexFormat::toDataFormat( buf );
		if ( df == VertexFormat::DF_SIZE )
			throwError( IOException( Format("Invalid vertex data format in \"{0}\": {1}", toString(), buf) ) );

		vf->setDataFormat( dt, df );
	}
}

P(Primitive) SceneInputStream::readPrimitive( Context* context, int* materialindex )
{
	const int verts = readShort();
	if ( verts < 0 || verts > 0x100000 )
		throwError( IOException( Format("Failed to load scene \"{0}\". Invalid vertex count ({1}).", toString(), verts) ) );

	const int inds = readShort();
	if ( inds < 0 || inds > 0xFFFF )
		throwError( IOException( Format("Failed to load scene \"{0}\". Invalid face index count ({1}).", toString(), inds) ) );

	VertexFormat vf;
	readVertexFormat( &vf );

	*materialindex = readShort();

	Primitive::PrimType type = Primitive::PRIM_TRI;
	type = (Primitive::PrimType)readShort();
	if ( int(type) < 0 || int(type) >= Primitive::PRIM_INVALID )
		throwError( IOException( Format("Failed to load scene \"{0}\". Primitive type ({1}) invalid", toString(), int(type)) ) );

	P(Primitive) prim = context->createPrimitive( type, vf, verts, inds );
	if ( vf != prim->vertexFormat() )
		throwError( IOException( Format("Failed to load scene \"{0}\". Primitive vertex format ({1}) should be ({2})", toString(), vf.toString(), prim->vertexFormat().toString()) ) );

	Primitive::Lock lk( prim, Primitive::LOCK_READWRITE );

	Array<uint8_t> buf;
	for ( int k = 0 ; k < (int)VertexFormat::DT_SIZE ; ++k )
	{
		VertexFormat::DataType dt = (VertexFormat::DataType)k;
		VertexFormat::DataFormat df = vf.getDataFormat( dt );

		if ( VertexFormat::DF_NONE != df )
		{
			float scale = readFloat();
			float4 bias = readFloat4();

			buf.resize( verts*VertexFormat::getDataSize(df) );
			readFully( &buf[0], buf.size() );

			prim->setVertexData( dt, 0, &buf[0], df, verts, float4(scale,scale,scale,scale), bias );
		}
	}

	if ( inds > 0 )
	{
		uint16_t* indexdata = 0;
		int indexsize = 0;
		prim->getIndexDataPtr( &indexdata, &indexsize );
		if ( indexsize != 2 )
			throwError( IOException( Format("Failed to load scene \"{0}\". Invalid face index size ({1}).", toString(), indexsize) ) );

		// WARNING: Endianess dependent read
		readFully( indexdata, inds*indexsize );

		// check validity of indices
		for ( int k = 0 ; k < inds ; ++k )
		{
			if ( indexdata[k] >= verts )
				throwError( IOException( Format("Failed to load scene \"{0}\". Invalid face vertex index (face {1}).", toString(), k) ) );
		}
	}

	uint8_t usedbonearray[Primitive::MAX_BONES];
	int usedbones = readByte();
	if ( usedbones > Primitive::MAX_BONES )
		throwError( IOException( Format("Failed to load scene \"{0}\". Too many bones ({1}).", toString(), usedbones) ) );
	readFully( usedbonearray, usedbones );
	prim->setUsedBones( usedbonearray, usedbones );
	return prim;
}

P(UserPropertySet) SceneInputStream::readUserPropertySet( Scene* scene )
{
	// read number of user property strings
	int count = readInt();
	if ( count < 0 || count > 1000000 )
		throwError( IOException( Format("Failed to load scene \"{0}\". Invalid user property set size ({1}).", toString(), count) ) );
	if ( 0 == count )
		return 0;
		
	// get access to existing node name strings
	TransformAnimationSet* tmanims = 0;
	if ( scene != 0 )
		tmanims = scene->transformAnimations();

	P(UserPropertySet) ups = new UserPropertySet( count );
	for ( int i = 0 ; i < count ; ++i )
	{
		String key = readUTF();

		// replace key with shared key string to save memory
		// (=same value but shared object)
		if ( tmanims && tmanims->containsKey(key) )
		{
			key = tmanims->getIterator(key).key();
		}
		else if ( scene != 0 )
		{
			for ( Node* node = scene->root() ; node != 0 ; node = node->next() )
			{
				if ( node->name() == key )
				{
					key = node->name();
					break;
				}
			}
		}

		String value = readUTF();
		ups->put( key, value );
	}

	return ups;
}

bool SceneInputStream::hasData( int flags ) const
{
	return 0 != (m_dataFlags & flags);
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
