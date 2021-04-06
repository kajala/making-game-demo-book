#include <img/ImageReader.h>
#include <io/InputStream.h>
#include <io/IOException.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <config.h>


using namespace io;
using namespace gr;
using namespace lang;


namespace img
{


ImageReader::ImageReader( InputStream* in, FileFormat filefmt ) :
	m_in( in ),
	m_width( 0 ),
	m_height( 0 ),
	m_bitsPerPixel( 0 ),
	m_pitch( 0 ),
	m_mipLevels( 1 ),
	m_surfaces( 0 ),
	m_surfaceIndex( 0 ),
	m_scanlinebuffer(),
	m_fmt(),
	m_palfmt(),
	m_filefmt( filefmt ),
	m_bottomUp( false ),
	m_cubeMap( false )
{
	switch ( m_filefmt )
	{
	case FILEFORMAT_BMP:	readHeader_bmp(); break;
	case FILEFORMAT_TGA:	readHeader_tga(); break;
	case FILEFORMAT_JPG:	readHeader_jpg(); break;
	case FILEFORMAT_DDS:
	case FILEFORMAT_PSD:
	case FILEFORMAT_PNG:
	case FILEFORMAT_PCX:	
	case FILEFORMAT_PNM:	
	case FILEFORMAT_GIF:
	case FILEFORMAT_TIF:	readHeader_il(); break;
	default:				throwError( IOException( Format("Unsupported image file format while reading {0}", m_in->toString()) ) );
	}
}

ImageReader::~ImageReader()
{
}

void ImageReader::readSurface( void* bits, int pitch, int w, int h, SurfaceFormat fmt,
	const void* pal, SurfaceFormat palfmt )
{
	assert( w == surfaceWidth() );
	assert( h == surfaceHeight() );
	
	switch ( m_filefmt )
	{
	case FILEFORMAT_BMP:	readScanlines( bits, pitch, w, h, fmt, pal, palfmt ); break;
	case FILEFORMAT_TGA:	readScanlines( bits, pitch, w, h, fmt, pal, palfmt ); break;
	case FILEFORMAT_JPG:	readSurface_jpg( bits, pitch, w, h, fmt, pal, palfmt ); break;
	default:				readSurface_il( bits, pitch, w, h, fmt, pal, palfmt ); break;
	}
}

int ImageReader::surfaces() const
{
	return m_surfaces;
}

int	ImageReader::surfaceIndex() const
{
	return m_surfaceIndex;
}

bool ImageReader::cubeMap() const
{
	return m_cubeMap;
}

int ImageReader::mipLevels() const
{
	return m_mipLevels;
}

SurfaceFormat ImageReader::format() const
{
	return m_fmt;
}

const void* ImageReader::paletteData() const
{
	return &m_colormap[0][0];
}

SurfaceFormat ImageReader::paletteFormat() const
{
	return m_palfmt;
}

int ImageReader::surfaceWidth() const
{
	return m_width;
}

int ImageReader::surfaceHeight() const
{
	return m_height;
}

ImageReader::FileFormat ImageReader::guessFileFormat( const String& filename )
{
	ImageReader::FileFormat ff = ImageReader::FILEFORMAT_UNKNOWN;

	int len = filename.length();
	if ( len >= 5 )
	{
		char ext[4];
		filename.getChars( len-3, len, ext );
		ext[3] = 0;

		for ( int i = 0 ; i < 3 ; ++i )
			ext[i] = tolower( ext[i] );

		if ( !strcmp(ext,"bmp") )
			ff = ImageReader::FILEFORMAT_BMP;
		else if ( !strcmp(ext,"tga") )
			ff = ImageReader::FILEFORMAT_TGA;
		else if ( !strcmp(ext,"jpg") )
			ff = ImageReader::FILEFORMAT_JPG;
		else if ( !strcmp(ext,"dds") )
			ff = ImageReader::FILEFORMAT_DDS;
		else if ( !strcmp(ext,"psd") )
			ff = ImageReader::FILEFORMAT_PSD;
		else if ( !strcmp(ext,"png") )
			ff = ImageReader::FILEFORMAT_PNG;
		else if ( !strcmp(ext,"pcx") )
			ff = ImageReader::FILEFORMAT_PCX;
		else if ( !strcmp(ext,"pnm") )
			ff = ImageReader::FILEFORMAT_PNM;
		else if ( !strcmp(ext,"gif") )
			ff = ImageReader::FILEFORMAT_GIF;
		else if ( !strcmp(ext,"tif") )
			ff = ImageReader::FILEFORMAT_TIF;
	}

	return ff;
}

void ImageReader::readFully( io::InputStream* in, void* buf, int bytes )
{
	if ( in->read(buf,bytes) != bytes )
		throwError( IOException( Format("Failed to read {0} bytes from {1}", bytes, in->toString()) ) );
}

void ImageReader::readColorMap( InputStream* in, int entrysize, int entries, uint8_t* colormap )
{
	uint8_t colr[4];
	for ( int i = 0 ; i < entries ; ++i )
	{
		memset( colr, 0, sizeof(colr) );
		readFully( in, colr, entrysize );
		colormap[i*4+0] = colr[2];
		colormap[i*4+1] = colr[1];
		colormap[i*4+2] = colr[0];
		colormap[i*4+3] = colr[3];
	}
}

uint16_t ImageReader::getUInt16LE( const void* data, int offset )
{
	const uint8_t* bytes = reinterpret_cast<const uint8_t*>(data) + offset;
	return uint16_t( ( unsigned(bytes[1]) << 8 ) + unsigned(bytes[0]) );
}

uint32_t ImageReader::getUInt32LE( const void* data, int offset )
{
	const uint8_t* bytes = reinterpret_cast<const uint8_t*>(data) + offset;
	return ( uint32_t(bytes[3]) << 24 ) + ( uint32_t(bytes[2]) << 16 ) + ( uint32_t(bytes[1]) << 8 ) + uint32_t(bytes[0]);
}

void ImageReader::readScanlines( void* bits, int pitch, int w, int h, SurfaceFormat fmt,
	const void* pal, SurfaceFormat palfmt )
{
	assert( w == m_width );
	assert( h == m_height );

	// make sure we have big enough scanline buffer,
	// but use dynamic allocation only if absolutely necessary
	const int DEFAULT_SCANLINE_BUFFER = 1024*4;
	uint8_t defaultscanlinebuffer[DEFAULT_SCANLINE_BUFFER];
	uint8_t* scanlinebuffer = defaultscanlinebuffer;
	if ( m_pitch*2 > DEFAULT_SCANLINE_BUFFER )
	{
		m_scanlinebuffer.resize( m_pitch*2 );
		scanlinebuffer = m_scanlinebuffer.begin();
	}

	// read pixels
	for ( int j = 0 ; j < m_height ; ++j )
	{
		switch ( m_filefmt )
		{
		case FILEFORMAT_BMP:
		case FILEFORMAT_TGA:
			readFully( m_in, scanlinebuffer, m_pitch );
			break;
		case FILEFORMAT_JPG:
			readScanline_jpg( scanlinebuffer );
			break;
		default:
			throwError( IOException( Format("Unsupported image file format while reading {0}", m_in->toString()) ) );
		}

		int j1 = j;
		if ( m_bottomUp )
			j1 = m_height - j - 1;
		uint8_t* dst = reinterpret_cast<uint8_t*>(bits) + j1*pitch;

		fmt.copyPixels( dst, palfmt, pal, m_fmt, scanlinebuffer, m_palfmt, &m_colormap[0][0], m_width );
	}
}


} // img

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
