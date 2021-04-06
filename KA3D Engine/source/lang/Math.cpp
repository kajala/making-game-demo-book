#include <lang/Math.h>
#include <stdlib.h>
#include <config.h>


namespace lang
{


const float		Math::E		= 2.718281828459045f;
const float		Math::PI	= 3.141592653589793f;

	
float Math::random()
{
	const float RAND_SCALE = 1.f / ( float(RAND_MAX) + 1.f );
	return RAND_SCALE * float(rand());
}


} // lang

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
