#ifndef _LANG_FORMATEXCEPTION_H
#define _LANG_FORMATEXCEPTION_H


#include <lang/Exception.h>


namespace lang
{


/**
 * Exception thrown when string formatting error occurs.
 * 
 * @ingroup lang
 */
class FormatException :
	public Exception
{
public:
	/** Creates an exception with the specified error description. */
	FormatException( const lang::Format& msg )						: Exception(msg) {}
};


} // lang


#endif // _LANG_FORMATEXCEPTION_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
