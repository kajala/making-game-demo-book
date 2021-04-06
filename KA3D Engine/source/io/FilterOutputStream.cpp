#include <io/FilterOutputStream.h>
#include <config.h>


namespace io
{


FilterOutputStream::FilterOutputStream( OutputStream* target ) :
	m_target( target ),
	m_bytesWritten( 0 )
{
}

void FilterOutputStream::write( const void* data, int size )									
{
	m_target->write( data, size );
	m_bytesWritten += size;
}

lang::String FilterOutputStream::toString() const
{
	return m_target->toString();
}

int FilterOutputStream::bytesWritten() const
{
	return m_bytesWritten;
}


} // io

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
