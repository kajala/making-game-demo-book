#ifndef _IO_OUTPUTSTREAM_H
#define _IO_OUTPUTSTREAM_H


#include <lang/Object.h>
#include <lang/String.h>


namespace lang {
	class String;}


namespace io
{


/**
 * Base class of all classes writing stream of bytes.
 * 
 * @ingroup io
 */
class OutputStream :
	public lang::Object
{
public:
	/**
	 * Writes specified number of bytes to the stream.
	 * @exception IOException
	 */
	virtual void 			write( const void* data, int size ) = 0;

	/** Returns name of the stream. */
	virtual lang::String	toString() const = 0;
};


} // io


#endif // _IO_OUTPUTSTREAM_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
