#include <lang/Debug.h>
#include <lang/pp.h>
#include <string.h>
#include <stdio.h>
#include <stdarg.h>

#ifdef PLATFORM_WIN32
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#endif

#include <config.h>


// Maximum length of a single debug message.
#define MAX_MSG_LEN 1000


namespace lang
{


void Debug::printf( const char* fmt, ... )
{
	// format variable arguments
	const unsigned MAX_MSG = 2000;
	char msg[MAX_MSG+4];
	va_list marker;
	va_start( marker, fmt );
	vsprintf( msg, fmt, marker );
	va_end( marker );
	assert( strlen(msg) < MAX_MSG ); // too long debug message

#if defined(PLATFORM_SYMBIAN)
	FILE* fh = fopen( "C:\\log.txt", "at" );
	if ( fh != 0 )
	{
		fprintf( fh, "%s", msg );
		fclose( fh );
	}
#elif defined(PLATFORM_WIN32)
	OutputDebugString( msg );
#else
	::printf( msg );
#endif
}

const char*	Debug::stripSourceFilename( const char* fname )
{
	const char* sz = strrchr( fname, '/' );
	if ( sz != 0 )
	{
		while ( sz > fname )
		{
			--sz;
			if ( *sz == '/' )
			{
				++sz;
				break;
			}
		}
		return sz;
	}

	return fname;
}


} // lang

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
