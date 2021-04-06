#include <io/FileInputStream.h>
#include <io/IOException.h>
#include <stdio.h>
#include <config.h>


using namespace lang;


namespace io
{

	
FileInputStream::FileInputStream( const String& filename ) :
	m_filename( filename )
{
	char buf[1000];
	String::cpy( buf, sizeof(buf), filename );
	m_fh = fopen( buf, "rb" );
	if ( !m_fh )
		throwError( IOException( Format("Failed to open {0}", filename) ) );
}

FileInputStream::~FileInputStream()
{
	FILE* fh = reinterpret_cast<FILE*>(m_fh);
	if ( fh )
		fclose( fh );
}

int FileInputStream::read( void* data, int size )
{
	FILE* fh = reinterpret_cast<FILE*>(m_fh);
	int bytes = fread( data, 1, size, fh );
	if ( bytes < size && ferror(fh) )
		throwError( IOException( Format("Failed to read {1} bytes from {0}", toString(), size) ) );
	return bytes;
}

int FileInputStream::available() const
{
	FILE* fh = reinterpret_cast<FILE*>(m_fh);
	if ( !fh )
		return 0;
	int cur = ftell( fh );
	fseek( fh, 0, SEEK_END );
	int end = ftell( fh );
	fseek( fh, cur, SEEK_SET );
	if ( ferror(fh) )
		throwError( IOException( Format("Failed to seek {0}", toString()) ) );
	return end-cur;
}

lang::String FileInputStream::toString() const
{
	return m_filename;
}


} // io

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
