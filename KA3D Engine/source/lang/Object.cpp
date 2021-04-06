#include <lang/Object.h>
#include <config.h>


namespace lang
{
	

Object::Object() :
	m_refs(0)
{
}

Object::Object( const Object& ) :
	m_refs(0)
{
}

Object::~Object()
{
	assert( 0 == m_refs ); // ref count of a stack/auto object incremented?
}

	
} // lang

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
