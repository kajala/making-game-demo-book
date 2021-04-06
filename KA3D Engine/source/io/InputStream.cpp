#include <io/InputStream.h>
#include <stdint.h>
#include <config.h>


namespace io
{


int InputStream::skip( int n )
{
	assert( n >= 0 );

	const int buffSize = 256;
	uint8_t buff[buffSize];

	int bytesSkipped = 0;
	while ( bytesSkipped < n )
	{
		int bytes = n - bytesSkipped;
		if ( bytes > buffSize )
			bytes = buffSize;

		bytes = read( buff, bytes );
		bytesSkipped += bytes;

		if ( 0 == bytes )
			break;
	}

	return bytesSkipped;
}


} // io

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
