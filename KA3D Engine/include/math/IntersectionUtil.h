#ifndef _MATH_INTERSECTIONUTIL_H
#define _MATH_INTERSECTIONUTIL_H


namespace math
{


class float2;
class float3;


/** 
 * Intersection testing utility functions.
 * @ingroup math
 */
class IntersectionUtil
{
public:
	/**
	 * Tests if two 2D-triangles overlap.
	 * @param a Vertices (3) of the first triangle.
	 * @param b Vertices (3) of the first triangle.
	 */
	static bool	testTriangleTriangleOverlap( 
		const float2* a, const float2* b );

	/**
	 * Tests if two 3D-triangles overlap.
	 * @param a Vertices (3) of the first triangle.
	 * @param b Vertices (3) of the first triangle.
	 */
	static bool	testTriangleTriangleOverlap( 
		const float3* a, const float3* b );
};


} // math


#endif // _MATH_INTERSECTIONUTIL_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
