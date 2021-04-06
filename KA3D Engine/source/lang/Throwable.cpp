#include <lang/Throwable.h>
#include <lang/Debug.h>
#include <string.h>
#include <config.h>


namespace lang
{


Throwable::Throwable()
{
}

Throwable::Throwable( const lang::Format& msg ) :
	m_msg( msg )
{
	char buf[256];
	msg.format( buf, sizeof(buf) );
	Debug::printf( "Created Throwable( \"%s\" )\n", buf );
}

const lang::Format& Throwable::getMessage() const
{
	return m_msg;
}


} // lang

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
