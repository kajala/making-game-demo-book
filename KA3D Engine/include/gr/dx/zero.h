#include <memory.h>


template <class T> static void zero( T& v )
{
	memset( &v, 0, sizeof(v) );
}

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
