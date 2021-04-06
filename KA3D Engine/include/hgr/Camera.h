#ifndef _HGR_CAMERA_H
#define _HGR_CAMERA_H


#include <hgr/Node.h>
#include <hgr/LightSorter.h>
#include <hgr/ViewFrustum.h>
#include <lang/Array.h>
#include <math/float4x4.h>


namespace gr {
	class Shader;
	class Context;}


namespace hgr
{


class Light;
class Visual;


/**
 * Camera node in scene graph.
 * Camera contains point-of-view, field-of-view, viewport and 
 * other attributes that desribe how the scene is rendered.
 * There can be multiple cameras in the scene.
 * 
 * @ingroup hgr
 */
class Camera : 
	public Node
{
public:
	/**
	 * Scene rendering statistics.
	 */
	struct Statistics
	{
		/** Number of visuals before cull. */
		int visualsBeforeCull;
		/** Number of visuals after cull. */
		int visualsAfterCull;

		/** Sets all statistics counters to 0. */
		void reset();
	} statistics;

	/*
	 * Temporary buffers used in rendering.
	 */
	class TempBuffers
	{
	public:
		lang::Array<math::float4x4>	bonematrices;
		lang::Array<math::float4x4*>	bonematrixp;
	};

	/** 
	 * Creates default camera. 
	 */
	Camera();

	/** 
	 * Copy by value. New camera is unlinked. 
	 */
	Camera( const Camera& other );

	///
	~Camera();

	/** 
	 * Sets horizontal field of view (in radians). 
	 */
	void	setHorizontalFov( float horzFov );

	/** 
	 * Sets front plane distance. 
	 */
	void	setFront( float front );
	
	/** 
	 * Sets back plane distance. 
	 */
	void	setBack( float back );

	/** 
	 * Renders the scene to the rendering context. 
	 * @param context Rendering context.
	 */
	void	render( gr::Context* context );

	/** 
	 * Renders the scene to the rendering context.
	 * @param context Rendering context.
	 * @param minpriority Minimum shader priority to render in this pass.
	 * @param maxpriority Maximum shader priority to render in this pass.
	 * @param visual Visuals to be rendered.
	 * @param priorities Priorities to be rendered.
	 * @param lightsorter Lights to be rendered.
	 */
	void	render( gr::Context* context, int minpriority, int maxpriority,
				const lang::Array<Visual*>& visuals, const lang::Array<int>& priorities,
				LightSorter* lightsorter );

	/** 
	 * Caches camera transforms and all world transforms from the nodes.
	 * Used in rendering. After this method, Camera::getCachedTransform() can be
	 * used to get valid world transform for any node in the array.
	 */
	void	cacheTransforms( gr::Context* context, const lang::Array<Node*>& nodes );

	/**
	 * Collects all visible visuals from 'nodes' array to 'visuals' array
	 * and sorts them by ascending distance to camera.
	 * Visuals are culled by bounding box before adding them to the array.
	 */
	void	cullVisuals( const lang::Array<Node*>& nodes, lang::Array<Visual*>& visuals );

	/** 
	 * Returns front plane distance. 
	 */
	float	front() const;
	
	/** 
	 * Returns back plane distance. 
	 */
	float	back() const;

	/** 
	 * Returns horizontal field of view in radians. 
	 */
	float	horizontalFov() const;

	/** 
	 * Returns vertical field of view in radians. 
	 */
	float	verticalFov() const;

	/** 
	 * Returns projection transform. 
	 */
	math::float4x4			projectionTransform() const;

	/** 
	 * Returns view (inverse world) transform. 
	 */
	math::float3x4			viewTransform() const;

	/** 
	 * Returns combined view-projection transform. 
	 */
	math::float4x4			viewProjectionTransform() const;

	/**
	 * Returns cached world transformation for the node.
	 * This function can be used only during rendering.
	 * (Requires that the node has been in the scene graph
	 * when Camera::cacheTransforms was called AND that
	 * the scene graph hasn't been modified after since.)
	 */
	const math::float3x4&	getCachedWorldTransform( Node* node ) const;

	/**
	 * Returns world (=camera to world) transformation.
	 * This function can be used only during rendering.
	 * Updated by calling cacheTransforms().
	 */
	const math::float3x4&	cachedWorldTransform() const;

	/**
	 * Returns view (=world to camera) transformation.
	 * This function can be used only during rendering.
	 * Updated by calling cacheTransforms().
	 */
	const math::float3x4&	cachedViewTransform() const;

	/**
	 * Returns projection transformation.
	 * This function can be used only during rendering.
	 * Updated by calling cacheTransforms().
	 */
	const math::float4x4&	cachedProjectionTransform() const;

	/**
	 * Returns view-projection transformation.
	 * This function can be used only during rendering.
	 * Updated by calling cacheTransforms().
	 */
	const math::float4x4&	cachedViewProjectionTransform() const;

	/**
	 * Returns screen-view-projection transformation.
	 * This function can be used only during rendering.
	 * Updated by calling cacheTransforms().
	 */
	const math::float4x4&	cachedScreenViewProjectionTransform() const;

	/**
	 * Gets cached lights sorted by distance to world position.
	 * Number of lights returned is limited to Shader::MAX_LIGHTS.
	 * This function can be used only during rendering.
	 */
	lang::Array<Light*>&	getLightsSortedByDistance( const math::float3& worldpos ) const;

	/*
	 * Returns camera temporary buffers used in rendering.
	 */
	TempBuffers&			tempBuffers() const		{return m_temp;}

private:
	/* Helper class used to sort visuals. */
	class VisualSorter
	{
	public:
		Visual*	obj;
		float	depth;

		bool	operator<( const VisualSorter& other ) const	{return depth < other.depth;}
	};

	ViewFrustum				m_frustum;
	math::float3x4			m_worldtm;
	math::float3x4			m_viewtm;
	math::float4x4			m_projtm;
	math::float4x4			m_viewprojtm;
	math::float4x4			m_screenviewprojtm;
	mutable TempBuffers		m_temp;

	lang::Array<math::float3x4>	m_worldTransformCache; // used by cacheTransforms
	lang::Array<Node*>				m_nodes; // used by simple render
	lang::Array<Visual*>			m_visuals; // used by simple render
	lang::Array<gr::Shader*>		m_shaders; // used by simple render
	lang::Array<int>				m_priorities; // used by simple render
	LightSorter						m_lightSorter; // used by simple render
	LightSorter*					m_lightSorterPtr; // used by getLightsSortedByDistance
	lang::Array<VisualSorter>		m_visualSorter; // used by cullVisuals

	Camera& operator=( const Camera& other );
};


#include <hgr/Camera.inl>


} // hgr


#endif // _HGR_CAMERA_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
