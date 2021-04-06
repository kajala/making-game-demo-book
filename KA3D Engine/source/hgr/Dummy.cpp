#include <hgr/Dummy.h>
#include <config.h>


using namespace math;


namespace hgr
{


Dummy::Dummy() :
	m_boxMin(0,0,0),
	m_boxMax(0,0,0)
{
}

Dummy::~Dummy()
{
}

Dummy::Dummy( const Dummy& other ) :
	Node(other),
	m_boxMin( other.m_boxMin ),
	m_boxMax( other.m_boxMax )
{
}

void Dummy::setBox( const float3& boxmin, const float3& boxmax )
{
	m_boxMin = boxmin;
	m_boxMax = boxmax;
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
