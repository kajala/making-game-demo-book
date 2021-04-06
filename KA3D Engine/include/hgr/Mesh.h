#ifndef _HGR_MESH_H
#define _HGR_MESH_H


#include <gr/Primitive.h>
#include <hgr/Light.h>
#include <hgr/Visual.h>
#include <lang/Array.h>


namespace math {
	class float4x4;}


namespace hgr
{


class Light;
class Camera;


/**
 * Container for visual primitives.
 * 
 * @ingroup hgr
 */
class Mesh : 
	public Visual
{
public:
	///
	Mesh();

	/** 
	 * Creates a copy of this object. 
	 */
	Mesh( const Mesh& other );

	~Mesh();

	/** 
	 * Renders this mesh to the active device.
	 * @param context Rendering context.
	 * @param camera Active camera.
	 * @param priority Shader priority level which is to be rendered.
	 */
	void			render( gr::Context* context, Camera* camera, int priority );

	/**
	 * Returns array of shaders used by this visual.
	 * NOTE: Implementation should not clear the list of returned shaders
	 * before adding new ones, since shadesr from multiple Visuals might be 
	 * collected at the same time.
	 */
	void			getShaders( lang::Array<gr::Shader*>& shaders );

	/**
	 * Sets rendering technique of the shaders used by this mesh.
	 */
	void			setTechnique( const char* name );

	/**
	 * Computes bounding volume of the visual in model space.
	 */
	void			computeBound();

	/** 
	 * Adds a visual primitive to the mesh. 
	 */
	void			addPrimitive( gr::Primitive* primitive );

	/** 
	 * Removes a visual primitive from the mesh. 
	 */
	void			removePrimitive( int index );

	/** 
	 * Removes all visual primitives from the mesh. 
	 */
	void			removePrimitives();

	/** 
	 * Adds a bone to the mesh.
	 * @param bone Bone node.
	 * @param invresttm Transform from skin to bone space in non-deforming pose.
	 */
	void			addBone( Node* bone, const math::float3x4& invresttm );

	/** 
	 * Removes a bone from the mesh. 
	 */
	void			removeBone( int index );

	/**
	 * Adds a local light to this mesh.
	 * Local light affects only this mesh.
	 * If the mesh has no local lights, then closest
	 * lights are retrieved from the scene in run-time.
	 * As this causes some performance penalty and is
	 * less predictable in terms of visual output, it
	 * is usually best always to use local lights if possible.
	 */
	void			addLight( Light* lt );

	/**
	 * Removes ith local light from this mesh.
	 * @see addLight
	 */
	void			removeLight( int i );

	/**
	 * Removes all local lights from this mesh.
	 * @see addLight
	 */
	void			removeLights();

	/**
	 * Returns ith local light from this mesh.
	 * @see addLight
	 */
	Light*			getLight( int i ) const;

	/** 
	 * Returns specified visual primitive of the mesh. 
	 */
	gr::Primitive*	getPrimitive( int index ) const;

	/**
	 * Returns number of local lights in this mesh.
	 * @see addLight
	 */
	int				lights() const;

	/** 
	 * Returns number of visual primitives in the mesh. 
	 */
	int				primitives() const;

	/** 
	 * Returns a bone from the mesh. 
	 */
	Node*			getBone( int index ) const;

	/** 
	 * Returns transform from skin space to bone space. 
	 */
	const math::float3x4&	getBoneInverseRestTransform( int index ) const;

	/** 
	 * Returns model->world transforms for n bones.
	 * Matrices are returned in modified 4x3 format, i.e.
	 * translation at row 3, X-axis at column 0.
	 * Camera transform cache must contain valid world transforms of the node hierarchy.
	 * This can be ensured by calling Camera::cacheTransforms()
	 * before this method.
	 */
	void			getBoneMatrix4x3Array( Camera* camera, math::float4x4* tm, int count ) const;

	/** 
	 * Returns number of bones in the mesh. 
	 */
	int				bones() const;

private:
	class Bone
	{
	public:
		math::float3x4	invresttm; // skin->bone tm in non-deforming pose
		Node*			node;
	};

	lang::Array<P(gr::Primitive)>	m_primitives;
	lang::Array<hgr::Light*>		m_lights;
	lang::Array<Bone>				m_bones;

	Mesh& operator=( const Mesh& other );
};


#include <hgr/Mesh.inl>


} // hgr


#endif // _HGR_MESH_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
