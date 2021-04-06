#ifndef _HGR_SCENEOUTPUTSTREAM_H
#define _HGR_SCENEOUTPUTSTREAM_H


#include <io/DataOutputStream.h>
#include <gr/Shader.h>
#include <lang/Array.h>


namespace gr {
	class VertexFormat;
	class Primitive;}

namespace math {
	class float3;
	class float3x4;
	class quaternion;}


namespace hgr
{


class Node;
class Mesh;
class Dummy;
class Lines;
class Light;
class Camera;
class UserPropertySet;
class KeyframeSequence;
class TransformAnimation;


/*
 * Helper class for writing hgr scene files. 
 * See maxexport and Scene::Scene for usage examples
 * and hgr-fileformat.txt for documentation.
 * @ingroup hgr
 */
class SceneOutputStream :
	public io::DataOutputStream
{
public:
	/** Version number (major*100+minor). */
	static const int VERSION;

	/**
	 * Prepares to write scene to the output stream.
	 * @param out Output stream
	 * @param dataflags Data content descriptor. See SceneInputStream::DataFlags.
	 * @exception IOException
	 */
	explicit SceneOutputStream( io::OutputStream* out, int dataflags );

	///
	~SceneOutputStream();

	/**
	 * Writes float3 data to the output stream.
	 * @param obj Object to write to the stream.
	 * @exception IOException
	 */
	void	writeFloat3( const math::float3& obj );

	/**
	 * Writes float4 data to the output stream.
	 * @param obj Object to write to the stream.
	 * @exception IOException
	 */
	void	writeFloat4( const math::float4& obj );

	/**
	 * Writes quaternion data to the output stream as 4 floats.
	 * @param obj Object to write to the stream.
	 * @exception IOException
	 */
	void	writeQuaternion( const math::quaternion& obj );

	/**
	 * Writes float3x4 data to the output stream.
	 * @param obj Object to write to the stream.
	 * @exception IOException
	 */
	void	writeFloat3x4( const math::float3x4& obj );

	/**
	 * Writes vertex format to the output stream.
	 * @param vf Vertex format to write to the stream.
	 * @exception IOException
	 */
	void	writeVertexFormat( const gr::VertexFormat& vf );

	/**
	 * Writes primitive data to the output stream.
	 * @param obj Primitive to write to the stream.
	 * @param globshaderlist Global shader/material list.
	 * @exception IOException
	 */
	void	writePrimitive( gr::Primitive* prim, const lang::Array<gr::Shader*>& globshaderlist );

	/**
	 * Writes node data to the output stream.
	 * @param globnodelist Used for finding out node parent index.
	 * @exception IOException
	 */
	void	writeNode( Node* obj, const lang::Array<Node*>& globnodelist );

	/**
	 * Writes mesh data to the output stream.
	 * @param globprimitivelist Global primitive/material list. Note: Not necessarily all are used by this mesh.
	 * @param globnodelist Global node list for finding out indices to bone nodes.
	 * @exception IOException
	 */
	void	writeMesh( Mesh* obj, const lang::Array<gr::Primitive*>& globprimitivelist,
				const lang::Array<Node*>& globnodelist );

	/**
	 * Writes camera data to the output stream.
	 * @param globnodelist Used for finding out node parent index.
	 * @exception IOException
	 */
	void	writeCamera( Camera* obj, const lang::Array<Node*>& globnodelist );

	/**
	 * Writes light data to the output stream.
	 * @param globnodelist Used for finding out node parent index.
	 * @exception IOException
	 */
	void	writeLight( Light* obj, const lang::Array<Node*>& globnodelist );

	/**
	 * Writes dummy data to the output stream.
	 * @param globnodelist Used for finding out node parent index.
	 * @exception IOException
	 */
	void	writeDummy( Dummy* obj, const lang::Array<Node*>& globnodelist );

	/**
	 * Writes line data to the output stream.
	 * @param globnodelist Used for finding out node parent index.
	 * @exception IOException
	 */
	void	writeLines( Lines* obj, const lang::Array<Node*>& globnodelist );

	/**
	 * Writes transform animation data to the output stream.
	 * @exception IOException
	 */
	void	writeTransformAnimation( TransformAnimation* obj );

	/**
	 * Writes keyframe data to the output stream.
	 * @exception IOException
	 */
	void	writeKeyframeSequence( KeyframeSequence* obj );

	/**
	 * Writes set of node user property strings to the output stream.
	 * @exception IOException
	 */
	void	writeUserPropertySet( UserPropertySet* obj );

	/**
	 * Returns true if the scene file has specified data.
	 * @see DataFlags
	 */
	bool	hasData( int flags ) const;

private:
	int		m_dataFlags;

	SceneOutputStream( const SceneOutputStream& );
	SceneOutputStream& operator=( const SceneOutputStream& );
};


} // hgr


#endif // _HGR_SCENEOUTPUTSTREAM_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
