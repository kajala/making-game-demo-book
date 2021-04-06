#include <lang/OutOfMemoryException.h>
#include <config.h>


namespace lang
{


void throw_OutOfMemoryException()
{
	throwError( OutOfMemoryException() );
}
	
OutOfMemoryException::OutOfMemoryException()
{
}


} // lang

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
