#ifndef _IO_INPUTSTREAM_H
#define _IO_INPUTSTREAM_H


#include <lang/Object.h>
#include <lang/String.h>


namespace io
{


/**
 * Base class of all input stream classes.
 *
 * 
 * @ingroup io
 */
class InputStream :
	public lang::Object
{
public:
	/**
	 * Tries to read specified number of bytes from the stream.
	 * Doesn't block the caller if specified number of bytes isn't available.
	 * @return Number of bytes actually read.
	 * @exception IOException
	 */
	virtual int				read( void* data, int size ) = 0;

	/**
	 * Tries to skip over n bytes from the stream.
	 * @return Number of bytes actually skipped.
	 * @exception IOException
	 */
	virtual int				skip( int n );

	/** 
	 * Returns the number of bytes that can be read from the stream without blocking.
	 * @exception IOException
	 */
	virtual int				available() const = 0;

	/**
	 * Returns string description of the stream.
	 */
	virtual lang::String	toString() const = 0;
};


} // io


#endif // _IO_INPUTSTREAM_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
