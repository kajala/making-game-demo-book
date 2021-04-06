#ifndef _LANG_FLOAT_H
#define _LANG_FLOAT_H


namespace lang
{


/** 
 * Float type information.
 * 
 * @ingroup lang
 */
class Float
{
public:
	/** 
	 * Maximum float value.
	 */
	static const float	MAX_VALUE;

	/** 
	 * Minimum float value. 
	 * This is smallest (positive) value that returns valid results when inverted.
	 */
	static const float	MIN_VALUE;
};


} // lang


#endif // _LANG_FLOAT_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
