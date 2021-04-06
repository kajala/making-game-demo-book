#ifndef _IO_IOEXCEPTION_H
#define _IO_IOEXCEPTION_H


#include <lang/Exception.h>


namespace io
{


/**
 * Thrown if input/output operation fails.
 * 
 * @ingroup io
 */
class IOException :
	public lang::Exception
{
public:
	IOException( const lang::Format& msg )											: Exception(msg) {}
};


} // io


#endif // _IO_IOEXCEPTION_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
