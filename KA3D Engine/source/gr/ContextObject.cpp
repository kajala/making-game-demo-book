#include <gr/ContextObject.h>
#include <config.h>


namespace gr
{

	
ContextObject::Lock::Lock( ContextObject* obj, LockType lock ) :
	m_obj( obj )
{
	assert( obj );
	obj->lock( lock );
}
	
ContextObject::Lock::~Lock()
{
	if ( m_obj )
		m_obj->unlock();
}


ContextObject::ContextObject( ClassId classid ) : 
	m_classId(classid) 
{
}

ContextObject::LockType ContextObject::locked() const
{
	return LOCK_READWRITE;
}

void ContextObject::lock( LockType /*lock*/ )
{
}

void ContextObject::unlock()
{
}


} // gr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
