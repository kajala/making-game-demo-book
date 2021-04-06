#include <io/FileOutputStream.h>
#include <io/FileNotFoundException.h>
#include <stdio.h>
#include <string.h>
#include <config.h>


using namespace lang;


namespace io
{


FileOutputStream::FileOutputStream( const String& filename ) :
	m_filename( filename )
{
	char buf[1000];
	String::cpy( buf, sizeof(buf), filename );
	m_fh = fopen( buf, "wb" );
	if ( !m_fh )
		throwError( FileNotFoundException( Format("Failed to open {0}", filename) ) );
}

FileOutputStream::~FileOutputStream()
{
	FILE* fh = reinterpret_cast<FILE*>(m_fh);
	if ( fh )
		fclose( fh );
}

void FileOutputStream::write( const void* data, int size )
{
	FILE* fh = reinterpret_cast<FILE*>(m_fh);
	int bytes = fwrite( data, 1, size, fh );
	if ( bytes < size && ferror(fh) )
		throwError( IOException( Format("Failed to write {1} bytes to {0}", toString(), size) ) );
}

lang::String FileOutputStream::toString() const
{
	return m_filename;
}


} // io

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
