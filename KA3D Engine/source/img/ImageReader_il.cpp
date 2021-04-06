#ifdef IMG_IL_SUPPORT

#include <img/ImageReader.h>
#include <io/InputStream.h>
#include <io/IOException.h>
#include <string.h>
#include <stdint.h>
#include "il/il.h"
#include <config.h>


using namespace io;
using namespace gr;
using namespace lang;


namespace img
{


void ImageReader::readHeader_il()
{
	// init
	ilInit();
    ilEnable( IL_ORIGIN_SET );
	ilSetInteger( IL_ORIGIN_MODE, IL_ORIGIN_UPPER_LEFT );
	ILuint img = 0;
	ilGenImages( 1, &img );
	ilBindImage( img );

	// read image file
	Array<char> lump;
	lump.resize( m_in->available() );
	if ( lump.size() != m_in->read( &lump[0], lump.size() ) )
		throwError( IOException(Format("Failed to load image \"{0}\"", m_in->toString())) );
	bool ok = ilLoadL( IL_TYPE_UNKNOWN, &lump[0], lump.size() ) != 0;
	if ( ok )
	{
		// convert format -> A8R8G8B8
		ilConvertImage( IL_BGRA, IL_UNSIGNED_BYTE );

		// read surfaces
		m_width = ilGetInteger(IL_IMAGE_WIDTH);
		m_height = ilGetInteger(IL_IMAGE_HEIGHT);
		m_bitsPerPixel = 4;
		m_pitch = m_width * 4;
		m_mipLevels = ilGetInteger(IL_NUM_MIPMAPS) + 1;
		m_surfaces = ilGetInteger(IL_NUM_IMAGES) + 1;
		m_surfaces *= m_mipLevels;
		m_fmt = SurfaceFormat::SURFACE_A8R8G8B8;
		m_palfmt = SurfaceFormat();
		m_cubeMap = (ilGetInteger(IL_IMAGE_CUBEFLAGS) != 0);

		int faces = m_surfaces/m_mipLevels;
		assert( !m_cubeMap || faces == 6 );
		m_surfaceBuffer.resize( m_surfaces );
		const int bytesperpixel = m_fmt.bitsPerPixel()/8;

		for ( int i = 0 ; i < faces ; ++i )
		{
			for ( int k = 0 ; k < m_mipLevels ; ++k )
			{
				ilBindImage( img );
				if ( faces > 1 )
					ilActiveImage( i );
				if ( m_mipLevels > 1 )
					ilActiveMipmap( k );

				Surface& s = m_surfaceBuffer[i*m_mipLevels+k];

				s.width = ilGetInteger(IL_IMAGE_WIDTH);
				s.height = ilGetInteger(IL_IMAGE_HEIGHT);
				s.data.resize( s.width*s.height*bytesperpixel );

				ILubyte* data = ilGetData();
				assert( data );
				memcpy( &s.data[0], data, s.width*s.height*bytesperpixel );
			}
		}
	}

	// cleanup
	ILenum err = ilGetError();
	ilDeleteImages( 1, &img );
	ilShutDown();

	// error occured?
	if ( err != 0 )
		throwError( IOException(Format("Failed to load image \"{0}\" (err={1})", m_in->toString(), err)) );
}

void ImageReader::readSurface_il( void* bits, int pitch, int w, int h, SurfaceFormat fmt,
	const void* pal, SurfaceFormat palfmt )
{
	assert( m_surfaceIndex >= 0 && m_surfaceIndex < m_surfaceBuffer.size() );
	
	Surface& s = m_surfaceBuffer[m_surfaceIndex];
	assert( s.width == w );
	assert( s.height == h );

	fmt.copyPixels( bits, pitch, palfmt, pal,
		m_fmt, &s.data[0], s.width*m_fmt.bitsPerPixel()/8, SurfaceFormat(), 0,
		w, h );

	// iterate to next surface
	m_surfaceIndex++;
	if ( m_surfaceIndex < m_surfaceBuffer.size() )
	{
		m_width = m_surfaceBuffer[m_surfaceIndex].width;
		m_height = m_surfaceBuffer[m_surfaceIndex].height;
	}
}


} // img

#else

#include <img/ImageReader.h>
#include <io/InputStream.h>
#include <io/IOException.h>

namespace img
{

void ImageReader::readHeader_il()
{
	throwError( io::IOException( lang::Format("Extended image file support is not enabled (file {0})", m_in->toString()) ) );
}

void ImageReader::readSurface_il( void* bits, int pitch, int w, int h, gr::SurfaceFormat fmt,
	const void* pal, gr::SurfaceFormat palfmt )
{
	throwError( io::IOException( lang::Format("Extended image file support is not enabled (file {0})", m_in->toString()) ) );
}

} // img

#endif // IMG_IL_SUPPORT

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
