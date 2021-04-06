#ifndef _HGR_SCENE_H
#define _HGR_SCENE_H


#include <hgr/Node.h>
#include <hgr/ParticleSystem.h>
#include <hgr/UserPropertySet.h>
#include <hgr/TransformAnimationSet.h>
#include <lang/Array.h>


namespace gr {
	class Context;}


namespace hgr
{


class Mesh;
class Camera;
class ResourceManager;
class SceneInputStream;


/**
 * Root node of scene node hierarchy. 
 * Provides support for scene file input.
 * Contains data which apply to all nodes.
 * @ingroup hgr
 */
class Scene : 
	public Node
{
public:
	/**
	 * Fog mode. Exact usage of this depends on used rendering pipeline.
	 */
	enum FogType
	{
		/** No fog. */
		FOG_NONE,
		/** Linear fog controlled by fogStart and fogEnd. */
		FOG_LINEAR
	};

	/** 
	 * Creates empty scene.
	 */
	Scene();

	/**
	 * Loads a scene from hgr file.
	 * @param context Rendering context to be used while loading.
	 * @param filename Scene file name relative to current working directory.
	 * @param res Resource manager to load textures and particles from.
	 * @param texturepath Texture path relative to current working directory.
	 * @param shaderpath Shader path relative to current working directory.
	 * @param particlepath Particle system path relative to current working directory.
	 * @exception IOException
	 * @exception GraphicsException
	 */
	Scene( gr::Context* context, const lang::String& filename,
		ResourceManager* res=0,
		const lang::String& texturepath="",
		const lang::String& shaderpath="",
		const lang::String& particlepath="" );

	/** 
	 * Create a value copy of this scene. 
	 */
	Scene( const Scene& other );

	///
	~Scene();

	/** 
	 * Applies hierarchy transforms from transform animation set
	 * and updates particle systems.
	 * @param time Current absolute time in seconds.
	 * @param dt Time since last update in seconds.
	 */
	void	applyAnimations( float time, float dt );

	/**
	 * Returns the first camera in the scene.
	 * If camera is not found then default camera is created, parented and returned.
	 */
	Camera*	camera();

	/**
	 * Merges another scene to this one.
	 * Note that animations are indexed by name so some animations
	 * might be lost from this scene.
	 */
	void	merge( Scene* other );

	/**
	 * Removes lights and cameras from the scene.
	 * Objects are removed only if they don't have any children.
	 * Commonly used for example when merging character scene to the level.
	 */
	void	removeLightsAndCameras();

	/**
	 * Sets fog parameters.
	 */
	void	setFog( FogType type, float start, float end, const math::float3& color );

	/**
	 * Returns transform animations associated with this scene (if any).
	 */
	TransformAnimationSet*	transformAnimations() const;

	/**
	 * Returns user properties associated with this scene (if any).
	 */
	UserPropertySet*		userProperties() const;

	/**
	 * Returns fog type.
	 */
	FogType	fogType() const;

	/**
	 * Returns fog start distance.
	 */
	float	fogStart() const;

	/**
	 * Returns fog end distance.
	 */
	float	fogEnd() const;
	
	/**
	 * Returns fog color.
	 */
	const math::float3& fogColor() const;

	/**
	 * Prints scene hierarchy to debug output.
	 * Useful for debugging loaded scenes.
	 */
	void	printHierarchy() const;

	/**
	 * Returns combined scene bounding box.
	 */
	void	getBoundBox( math::float3* boxmin, math::float3* boxmax ) const;

private:
	struct MeshBoneCount {Mesh* mesh; int bonecount;};
	struct MeshBone {int boneindex; math::float3x4 invresttm;};

	P(TransformAnimationSet)		m_transformAnims;
	P(UserPropertySet)				m_userProperties;

	math::float3	m_fogColor;
	float			m_fogStart;
	float			m_fogEnd;
	FogType			m_fogType;

	void	readId( SceneInputStream& in, int& id );
	void	readNode( SceneInputStream& in, Node* node, lang::Array<int>& nodeparents, lang::Array<lang::String>& nodenames, lang::Array<P(Node)>& nodes );

	Scene& operator=( const Scene& other );
};


#include <hgr/Scene.inl>


} // hgr


#endif // _HGR_SCENE_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
