#ifndef _HGR_VISUAL_H
#define _HGR_VISUAL_H


#include <hgr/Node.h>
#include <lang/Array.h>


namespace gr {
	class Shader;
	class Primitive;}

namespace math {
	class float4x4;}


namespace hgr
{


/**
 * Base class for objects to be rendered on screen.
 *
 * @ingroup hgr
 */
class Visual :
	public Node
{
public:
	/** 
	 * Maximum bounding volume size.
	 */
	static const float	MAX_BOUND;

	///
	Visual();

	/** 
	 * Copy by value. 
	 */
	Visual( const Visual& other );

	///
	virtual ~Visual();

	/** 
	 * Renders this visual to the rendering context.
	 * @param context Rendering context.
	 * @param camera Active camera.
	 * @param priority Shader priority level which is to be rendered.
	 */
	virtual void	render( gr::Context* context, Camera* camera, int priority ) = 0;

	/**
	 * Returns array of shaders used by this visual.
	 * NOTE: Implementation should not clear the list of returned shaders
	 * before adding new ones, since shadesr from multiple Visuals might be 
	 * collected at the same time.
	 */
	virtual void	getShaders( lang::Array<gr::Shader*>& shaders ) = 0;

	/**
	 * Computes bounding volume of the visual in model space.
	 */
	virtual void	computeBound() = 0;

	/**
	 * Sets bounding box minimum and maximum of the visual in model space.
	 * Value ranges are between [-MAX_BOUND,MAX_BOUND].
	 * World space bounding box and model space bounding box are mutually exclusive.
	 */
	void			setBoundBox( const math::float3& boxmin, const math::float3& boxmax );

	/**
	 * Sets bounding box minimum and maximum of the visual in world space.
	 * Value ranges are between [-MAX_BOUND,MAX_BOUND].
	 * World space bounding box and model space bounding box are mutually exclusive.
	 */
	void			setBoundBoxWorld( const math::float3& boxmin, const math::float3& boxmax );

	/**
	 * Sets 'infinite' bounding box.
	 * 'Infinite' bounding box is equal to [-MAX_BOUND,MAX_BOUND] size.
	 * Infinite box is considered to be defined in world space.
	 */
	void			setBoundInfinity();

	/**
	 * Toggles rendering for this visual. Default is enabled.
	 */
	void			setRenderable( bool enabled );

	/**
	 * Gets bounding box minimum of the visual in model space.
	 * Requires that bounding box has been set with setBoundBox().
	 * Value ranges are between [-MAX_BOUND,MAX_BOUND].
	 */
	const math::float3&	boundBoxMin() const;

	/**
	 * Gets bounding box maximum of the visual in model space.
	 * Requires that bounding box has been set with setBoundBox().
	 * Value ranges are between [-MAX_BOUND,MAX_BOUND].
	 */
	const math::float3&	boundBoxMax() const;

	/**
	 * Gets bounding box minimum of the visual in world space.
	 * Requires that bounding box has been set with setBoundBoxWorld().
	 * Value ranges are between [-MAX_BOUND,MAX_BOUND].
	 */
	const math::float3&	boundBoxMinWorld() const;

	/**
	 * Gets bounding box maximum of the visual in world space.
	 * Requires that bounding box has been set with setBoundBoxWorld().
	 * Value ranges are between [-MAX_BOUND,MAX_BOUND].
	 */
	const math::float3&	boundBoxMaxWorld() const;

	/**
	 * Returns true if this Visual's bounding volumes is infinite,
	 * i.e. visual is always visible.
	 */
	bool		isBoundInfinity() const;

	/**
	 * Returns true if this Visual's bounding volumes are
	 * defined in world space (instead of default, model space).
	 */
	bool		isBoundWorld() const;

	/**
	 * Returns transformations which remove world space scaling
	 * and move pivot to geometric center.
	 * Useful for example to setup rigid body simulations
	 * and other applications which do not tolerate scaling and
	 * require pivot to be in geometric center.
	 *
	 * @param bodyworldtm [out] Receives geometric pivot transformation in uniformly scaled world space (ptr can be 0 if value not needed).
	 * @param bodytm [out] Visual world transform multiplied with this results uniform world transform (ptr can be 0 if value not needed).
	 * @param bodyvertextm [out] Visual vertex transformed with this results a vertex compatible with uniform world transform (ptr can be 0 if value not needed).
	 */
	void		getBodyTransform( math::float3x4* bodyworldtm, math::float3x4* bodytm=0, math::float3x4* bodyvertextm=0 ) const;

	/**
	 * Returns true if rendering is enabled for this visual. Default is enabled.
	 */
	bool		renderable() const;

private:
	math::float3	m_boundBoxMin;
	math::float3	m_boundBoxMax;

	void	setBoundModel();
	void	setBoundWorld();

	Visual& operator=( const Visual& other );
};


#include <hgr/Visual.inl>


} // hgr


#endif // _HGR_VISUAL_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
