#include <io/ByteArrayOutputStream.h>
#include <string.h>
#include <config.h>


namespace io
{


ByteArrayOutputStream::ByteArrayOutputStream( int size )
{
	m_buffer.resize( size );
	m_buffer.resize( 0 );
}

ByteArrayOutputStream::~ByteArrayOutputStream()
{
}

void ByteArrayOutputStream::reset()
{
	m_buffer.resize( 0 );
}

void ByteArrayOutputStream::write( const void* data, int size )
{
	int oldsize = m_buffer.size();
	m_buffer.resize( oldsize + size );
	for ( int i = 0 ; i < size ; ++i )
		m_buffer[oldsize+i] = reinterpret_cast<const char*>(data)[i];
}

int ByteArrayOutputStream::size() const
{
	return m_buffer.size();
}

lang::String ByteArrayOutputStream::toString() const
{
	return "ByteArrayInputStream";
}


} // io

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
