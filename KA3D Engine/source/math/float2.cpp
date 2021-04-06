#include <math/float2.h>
#include <float.h>
#include <config.h>


namespace math
{


bool float2::finite() const
{
	return 
		x >= -FLT_MAX && x <= FLT_MAX &&
		y >= -FLT_MAX && y <= FLT_MAX;
}

float2 normalize0( const float2& v )
{
	const float len = v.length();
	float invlen = 0.f; 
	if ( len >= FLT_MIN ) 
		invlen = 1.f / len; 
	return v * invlen;
}


} // math

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
