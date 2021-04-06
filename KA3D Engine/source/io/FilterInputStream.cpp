#include <io/FilterInputStream.h>
#include <config.h>


namespace io
{


FilterInputStream::FilterInputStream( InputStream* source ) : 
	m_source( source ),
	m_bytesRead( 0 )
{
}

int FilterInputStream::read( void* data, int size )											
{
	int bytes = m_source->read( data, size );
	m_bytesRead += bytes;
	return bytes;
}

int FilterInputStream::skip( int n )															
{
	assert( n >= 0 );
	int bytes = m_source->skip(n);
	m_bytesRead += bytes;
	return bytes;
}

int FilterInputStream::available() const														
{
	return m_source->available();
}

lang::String FilterInputStream::toString() const
{
	return m_source->toString();
}

int FilterInputStream::bytesRead() const
{
	return m_bytesRead;
}


} // io

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
