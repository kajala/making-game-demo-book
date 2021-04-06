#include <lang/internalError.h>
#include <lang/Exception.h>
#include <config.h>


namespace lang
{


void lang::internalError( const char* filename, int line, const char* expr )
{
	throwError( Exception( Format("Internal error at {0}({1}): {2}", filename, line, expr) ) );
}


} // lang

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
