#ifndef _LANG_OUTOFMEMORYEXCEPTION_H
#define _LANG_OUTOFMEMORYEXCEPTION_H


#include <lang/Exception.h>


namespace lang
{


/** 
 * Thrown if application rans out of memory. 
 * @ingroup lang
 */
class OutOfMemoryException :
	public Exception
{
public:
	OutOfMemoryException();
};


/*
 * Throws OutOfMemoryException. (used to avoid dependencies to header OutOfMemoryException.h) 
 */
void throw_OutOfMemoryException();


} // lang


#endif // _LANG_OUTOFMEMORYEXCEPTION_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
