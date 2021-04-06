#ifndef _HGR_VIEWFRUSTUM_H
#define _HGR_VIEWFRUSTUM_H


#include <math/float4.h>


namespace math {
	class float3x4;}


namespace hgr
{


/*
 * Camera view frustum. Plane normals point away from the frustum.
 * 
 * @ingroup hgr
 */
class ViewFrustum
{
public:
	enum Plane
	{ 
		/** Left plane index. */
		PLANE_LEFT,
		/** Right plane index. */
		PLANE_RIGHT,
		/** Near plane index. */
		PLANE_NEAR,
		/** Far plane index. */
		PLANE_FAR,
		/** Top plane index. */
		PLANE_TOP,
		/** Bottom plane index. */
		PLANE_BOTTOM,
		/** Number of planes (6) defining the view frustum volume. */
		PLANE_COUNT
	};

	/** Creates default view frustum. */
	ViewFrustum();

	/** Sets field of view (in radians). */
	void	setHorizontalFov( float horzFov );

	/** Sets front plane distance. */
	void	setFront( float front );
	
	/** Sets back plane distance. */
	void	setBack( float back );

	/** Sets aspect ratio (screen w/h). */
	void	setAspect( float aspect );

	/** Returns front plane distance. */
	float	front() const;
	
	/** Returns back plane distance. */
	float	back() const;

	/** Returns horizontal field of view in radians. */
	float	horizontalFov() const;

	/** Returns vertical field of view in radians. */
	float	verticalFov() const;

	/** Returns aspect ratio (screen w/h). */
	float	aspect() const;

	/** 
	 * Return front-plane view rectangle dimensions. 
	 * @param x Dimension is the camera space width/2 of the view on front plane.
	 * @param y Dimension is the camera space height/2 of the view on front plane.
	 * @param z Dimension is the camera space distance of the front plane.
	 */
	void	getViewDimensions( float* x, float* y, float* z ) const;

	/** 
	 * Returns view frustum planes (6) in user defined space.
	 * Plane normals point away from the view frustum volume.
	 * @see Plane
	 */
	void	getPlanes( const math::float3x4& tm, math::float4* planes ) const;

	/**
	 * Tests if oriented box potentially intersects or is inside frustum.
	 * Plane xyz is the plane normal and w is distance from origin to the plane
	 * along that normal. Number of planes is PLANE_COUNT.
	 * Note that this method is approximation, i.e. the object
	 * might not intersect even tho intersection is reported.
	 * @param boxtm Box transform.
	 * @param boxmin Box volume minimum coordinates.
	 * @param boxmax Box volume maximum coordinates.
	 * @param planes Volume planes. See float4::setPlane.
	 * @param count Number of planes that define volume.
	 */
	static bool testOBox( const math::float3x4& boxtm,
		const math::float3& boxmin, const math::float3& boxmax,
		const math::float4* planes );

	/**
	 * Tests if axis-aligned box potentially intersects or is inside frustum.
	 * Plane xyz is the plane normal and w is distance from origin to the plane
	 * along that normal. Number of planes is PLANE_COUNT.
	 * Note that this method is approximation, i.e. the object
	 * might not intersect even tho intersection is reported.
	 * @param boxmin Box volume minimum coordinates.
	 * @param boxmax Box volume maximum coordinates.
	 * @param planes Volume planes. See float4::setPlane.
	 * @param count Number of planes that define volume.
	 */
	static bool testAABox( const math::float3& boxmin, const math::float3& boxmax,
		const math::float4* planes );

private:
	float	m_aspect;
	float	m_front;
	float	m_back;
	float	m_horzFov;
};


#include <hgr/ViewFrustum.inl>


} // hgr


#endif // _HGR_VIEWFRUSTUM_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
