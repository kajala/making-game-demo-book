#ifndef _IO_FILEINPUTSTREAM_H
#define _IO_FILEINPUTSTREAM_H


#include <io/InputStream.h>


namespace io
{


/**
 * FileInputStream reads bytes from a standard file.
 * 
 * @ingroup io
 */
class FileInputStream :
	public InputStream
{
public:
	/** 
	 * Opens a file input stream. 
	 */
	explicit FileInputStream( const lang::String& filename );

	///
	~FileInputStream();

	/**
	 * Tries to read specified number of bytes from the stream.
	 * Doesn't block the caller if specified number of bytes isn't available.
	 *
	 * @return Number of bytes actually read.
	 */
	int				read( void* data, int size );

	/** 
	 * Returns the number of bytes that can be read from the stream without blocking.
	 */
	int				available() const;

	/**
	 * Returns name and seek position of the file.
	 */
	lang::String	toString() const;

private:
#ifdef PLATFORM_PS2
	int				m_fh;
#else
	void*			m_fh;
#endif
	lang::String	m_filename;

	FileInputStream();
	FileInputStream( const FileInputStream& );
	FileInputStream& operator=( const FileInputStream& );
};


} // io


#endif // _IO_FILEINPUTSTREAM_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
