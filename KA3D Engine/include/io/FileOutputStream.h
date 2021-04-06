#ifndef _IO_FILEOUTPUTSTREAM_H
#define _IO_FILEOUTPUTSTREAM_H


#include <io/OutputStream.h>


namespace io
{


/**
 * FileOutputStream writes bytes to a file in a file system.
 * 
 * @ingroup io
 */
class FileOutputStream :
	public OutputStream
{
public:
	/** 
	 * Opens a file output stream. 
	 * @exception IOException
	 */
	explicit FileOutputStream( const lang::String& filename );

	///
	~FileOutputStream();

	/**
	 * Writes specified number of bytes to the stream.
	 * @exception IOException
	 */
	void			write( const void* data, int size );

	/** Returns name of the file. */
	lang::String	toString() const;

private:
#ifdef PLATFORM_PS2
	int				m_fh;
#else
	void*			m_fh;
#endif
	lang::String 	m_filename;

	FileOutputStream();
	FileOutputStream( const FileOutputStream& );
	FileOutputStream& operator=( const FileOutputStream& );
};


} // io


#endif // _IO_FILEOUTPUTSTREAM_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
