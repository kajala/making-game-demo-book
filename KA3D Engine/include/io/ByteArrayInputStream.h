#ifndef _IO_BYTEARRAYINPUTSTREAM_H
#define _IO_BYTEARRAYINPUTSTREAM_H


#include <io/InputStream.h>
#include <lang/Array.h>


namespace io
{


/**
 * ByteArrayInputStream reads bytes from a memory buffer.
 * 
 * @ingroup io
 */
class ByteArrayInputStream :
	public InputStream
{
public:
	/** 
	 * Creates an input stream from specified memory buffer.
	 * Note that the contents of the buffer is duplicated so the buffer
	 * can be freed immediately after ByteArrayInputStream constructor returns.
	 */
	ByteArrayInputStream( const void* data, int size );

	///
	~ByteArrayInputStream();

	/**
	 * Resets with new input buffer.
	 * Note that the contents of the buffer is duplicated so the buffer
	 * can be freed immediately after ByteArrayInputStream constructor returns.
	 */
	void	reset( const void* data, int size );

	/**
	 * Tries to read specified number of bytes from the stream.
	 * Doesn't block the caller if specified number of bytes isn't available.
	 * @return Number of bytes actually read.
	 */
	int		read( void* data, int size );

	/** 
	 * Returns the number of bytes that can be read from the stream without blocking.
	 */
	int		available() const;

	/** Returns byte array identifier. */
	lang::String	toString() const;

private:
	lang::Array<char>	m_data;
	int					m_index;

	ByteArrayInputStream();
	ByteArrayInputStream( const ByteArrayInputStream& );
	ByteArrayInputStream& operator=( const ByteArrayInputStream& );
};


} // io


#endif // _IO_BYTEARRAYINPUTSTREAM_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
