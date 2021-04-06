#ifndef _IO_BYTEARRAYOUTPUTSTREAM_H
#define _IO_BYTEARRAYOUTPUTSTREAM_H


#include <io/OutputStream.h>
#include <lang/Array.h>


namespace io
{


/**
 * ByteArrayOutputStream writes bytes to a memory buffer.
 * 
 * @ingroup io
 */
class ByteArrayOutputStream :
	public OutputStream
{
public:
	/** 
	 * Creates an output stream to a memory buffer of specified initial capacity.
	 */
	explicit ByteArrayOutputStream( int size=0 );

	///
	~ByteArrayOutputStream();

	/** Discards all written bytes and resets write pointer. */
	void			reset();

	/**
	 * Writes specified number of bytes to the stream.
	 */
	void			write( const void* data, int size );

	/**
	 * Returns pointer to the memory buffer used by the stream.
	 * Any write operation to the stream can invalidate the returned pointer.
	 */
	const void*		toByteArray() const												{return m_buffer.begin();}

	/**
	 * Returns number of bytes written to the stream.
	 */
	int				size() const;

	/** Returns byte array identifier. */
	lang::String	toString() const;

private:
	lang::Array<char>	m_buffer;

	ByteArrayOutputStream( const ByteArrayOutputStream& );
	ByteArrayOutputStream& operator=( const ByteArrayOutputStream& );
};


} // io


#endif // _IO_BYTEARRAYOUTPUTSTREAM_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
