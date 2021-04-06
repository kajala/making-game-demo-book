#ifndef _LANG_EXCEPTION_H
#define _LANG_EXCEPTION_H


#include <lang/Throwable.h>


namespace lang
{


/**
 * Base class for all exceptions that an application might want to handle.
 * 
 * @ingroup lang
 */
class Exception :
	public Throwable
{
public:
	/** Creates an exception with no error description. */
	Exception() {}

	/** Creates an exception with the specified error description. */
	Exception( const lang::Format& msg )											: Throwable(msg) {}
};


} // lang


#endif // _LANG_EXCEPTION_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
