#ifndef _HGR_DUMMY_H
#define _HGR_DUMMY_H


#include <hgr/Node.h>


namespace hgr
{


/**
 * Dummy object for storing non-rendering related properties.
 * 
 * @ingroup hgr
 */
class Dummy : 
	public Node
{
public:
	/** Creates default dummy object. */
	Dummy();

	/** Create a value copy of this object. */
	Dummy( const Dummy& other );

	///
	~Dummy();

	/** Sets box dimensions for this dummy object. Default is (0,0,0), (0,0,0). */
	void	setBox( const math::float3& boxmin, const math::float3& boxmax );

	/** Returns box minimum corner for this object. Default is (0,0,0). */
	const math::float3&	boxMin() const			{return m_boxMin;}

	/** Returns box minimum corner for this object. Default is (0,0,0). */
	const math::float3&	boxMax() const			{return m_boxMax;}

private:
	math::float3			m_boxMin;
	math::float3			m_boxMax;

	Dummy& operator=( const Dummy& other );
};


} // hgr


#endif // _HGR_DUMMY_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
