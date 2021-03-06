#ifndef _IO_FILENOTFOUNDEXCEPTION_H
#define _IO_FILENOTFOUNDEXCEPTION_H


#include <io/IOException.h>


namespace io
{


/**
 * Thrown if specified file or path is not found or 
 * if the file cannot be opened for some other reason.
 * (for example if the file tried to be opened is a directory)
 * 
 * @ingroup io
 */
class FileNotFoundException :
	public IOException
{
public:
	///
	FileNotFoundException( const lang::Format& msg )								: IOException(msg) {}
};


} // io


#endif // _IO_FILENOTFOUNDEXCEPTION_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
