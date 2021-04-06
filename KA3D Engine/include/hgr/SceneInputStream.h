#ifndef _HGR_SCENEINPUTSTREAM_H
#define _HGR_SCENEINPUTSTREAM_H


#include <gr/Primitive.h>
#include <io/DataInputStream.h>
#include <hgr/UserPropertySet.h>
#include <hgr/TransformAnimation.h>
#include <lang/Array.h>
#include <math/float4.h>


namespace gr {
	class Context;}


namespace hgr
{


class Node;
class Mesh;
class Scene;
class Dummy;
class Light;
class Camera;
class TransformAnimation;


/* 
 * Helper class for reading hgr scene files. 
 * See maxexport and Scene::Scene for usage examples
 * and hgr-fileformat.txt for documentation.
 * @ingroup hgr
 */
class SceneInputStream :
	public io::DataInputStream
{
public:
	/** Data content flags. */
	enum DataFlags
	{
		/** Scene stream contains materials. */
		DATA_MATERIALS		= 1,
		/** Scene stream contains primitives. */
		DATA_PRIMITIVES		= 2,
		/** Scene stream contains nodes and hierarchy. */
		DATA_NODES			= 4,
		/** Scene stream contains animations. */
		DATA_ANIMATIONS		= 8,
		/** Scene stream contains user properties. */
		DATA_USERPROPERTIES	= 16,
	};

	/** Minimum version number (major*100+minor) the scene loader supports. */
	static const int MIN_VERSION;

	/** Maximum version number (major*100+minor) the scene loader supports. */
	static const int MAX_VERSION;

	/**
	 * Prepares to read scene from the input stream.
	 * @exception IOException
	 */
	explicit SceneInputStream( io::InputStream* in );

	///
	~SceneInputStream();

	/**
	 * Reads float3 data from the input stream.
	 * @exception IOException
	 */
	math::float3			readFloat3();

	/**
	 * Reads float4 data from the input stream.
	 * @exception IOException
	 */
	math::float4			readFloat4();

	/**
	 * Reads quaternion data from the input stream as 4 floats.
	 * @exception IOException
	 */
	math::quaternion		readQuaternion();

	/**
	 * Reads float3x4 data from the input stream.
	 * @exception IOException
	 */
	math::float3x4			readFloat3x4();

	/**
	 * Reads transform animation data from the input stream.
	 * @exception IOException
	 */
	P(TransformAnimation)	readTransformAnimation();

	/**
	 * Reads keyframe sequence data from the input stream.
	 * @exception IOException
	 */
	P(KeyframeSequence)		readKeyframeSequence();

	/**
	 * Reads set of node user property strings.
	 * @param anims Optional parameter, which can be used to enable sharing of node name strings.
	 * @exception IOException
	 */
	P(UserPropertySet)		readUserPropertySet( Scene* scene=0 );
		
	/**
	 * Reads vertex format from the input stream.
	 * @param vf [out] Receives read vertex format.
	 * @exception IOException
	 */
	void					readVertexFormat( gr::VertexFormat* vf );

	/**
	 * Reads geometry primitive data from the input stream.
	 * @param materialindex [out] Receives material index of the primitive.
	 * @exception IOException
	 */
	P(gr::Primitive)		readPrimitive( gr::Context* context, int* materialindex );

	/**
	 * Reads Node data from the input stream.
	 * @param node [out] Receives read node data.
	 * @param parentindex [out] Receives read node parent index.
	 * @exception IOException
	 */
	void					readNode( Node* node, int* parentindex );

	/**
	 * Returns version number (major*100+minor) of this scene file.
	 */
	int						version() const				{return m_ver;}

	/**
	 * Returns true if the scene file has specified data.
	 * @see DataFlags
	 */
	bool					hasData( int flags ) const;

private:
	int		m_ver;
	int		m_dataFlags;

	SceneInputStream( const SceneInputStream& );
	SceneInputStream& operator=( const SceneInputStream& );
};


} // hgr


#endif // _HGR_SCENEINPUTSTREAM_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
