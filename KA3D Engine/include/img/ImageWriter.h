#ifndef _IMG_IMAGEWRITER_H
#define _IMG_IMAGEWRITER_H


#include <gr/SurfaceFormat.h>
#include <lang/Array.h>
#include <lang/Object.h>
#include <stdint.h>


namespace lang {
	class String;}


namespace img
{


/**
 * Image file writer.
 * @ingroup img
 */
class ImageWriter :
	public lang::Object
{	
public:
	/**
	 * Saves image as PNG file.
	 * @exception IOException
	 */
	static void	writePNG( const lang::String& filename,
					const void* bits, int width, int height, int pitch,
					gr::SurfaceFormat srcformat, const void* srcpal, gr::SurfaceFormat srcpalformat );

	/**
	 * Saves image file and tries to guess format by filename.
	 * @exception IOException
	 */
	static void	write( const lang::String& filename, gr::SurfaceFormat dstformat,
					const void* bits, int width, int height, int pitch,
					gr::SurfaceFormat srcformat, const void* srcpal, gr::SurfaceFormat srcpalformat );
};


} // img


#endif // _IMG_IMAGEWRITER_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
