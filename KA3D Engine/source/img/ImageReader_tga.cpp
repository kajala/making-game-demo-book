#include <img/ImageReader.h>
#include <io/InputStream.h>
#include <io/IOException.h>
#include <string.h>
#include <stdint.h>
#include <config.h>


using namespace io;
using namespace gr;
using namespace lang;


namespace img
{


void ImageReader::readHeader_tga()
{
	uint8_t targaheader[18];
	readFully( m_in, targaheader, sizeof(targaheader) );

	int		biIdLen = targaheader[0];
	int		biCmapType = targaheader[1];
	int		biSubType = targaheader[2];
	int		biMapLen = getUInt16LE( targaheader, 5 );
	int		biMapEntrySize = targaheader[7]/8;
	long	biWidth = getUInt16LE( targaheader, 12 );
	long	biHeight = getUInt16LE( targaheader, 14 );
	int		biBitsPerPixel = targaheader[16];
	int		biImageDescr = targaheader[17];
	bool	biBottomUp = (0 == (0x20 & biImageDescr));
	int		biInterlaceType = (int)( unsigned(biImageDescr) >> 6 );
	bool	biRLE = (biSubType > 8);
	long	biPitch = biWidth * ((biBitsPerPixel+7)/8);

	if ( biCmapType > 1 || 0 != (biBitsPerPixel%8) || 0 != biInterlaceType )
		throwError( IOException( Format("Invalid bits per pixel in {0}",m_in->toString()) ) );
	if ( biRLE )
		throwError( IOException( Format("RLE Targas ({0}) not supported",m_in->toString()) ) );

	m_in->skip( biIdLen );

	// read colormap
	if ( biMapLen > 0 )
	{
		if ( biMapLen > 256 || 0 != getUInt16LE(targaheader,3) )
			throwError( IOException( Format("Invalid color map length in {0}",m_in->toString()) ) );

		readColorMap( m_in, biMapEntrySize, biMapLen, &m_colormap[0][0] );
	}
	else if ( 0 != biCmapType )
		throwError( IOException( Format("Invalid color map type in {0}",m_in->toString()) ) );

	// setup header info
	m_width = biWidth;
	m_height = biHeight;
	m_bitsPerPixel = biBitsPerPixel;
	m_pitch = biPitch;
	m_surfaces = 1;
	m_bottomUp = biBottomUp != 0;
	
	switch ( m_bitsPerPixel )
	{
	case 4:
		m_fmt = SurfaceFormat::SURFACE_P4;
		m_palfmt = SurfaceFormat::SURFACE_X8B8G8R8;
		break;

	case 8:
		m_fmt = SurfaceFormat::SURFACE_P8;
		m_palfmt = SurfaceFormat::SURFACE_X8B8G8R8;
		break;
	
	case 16:
		m_fmt = SurfaceFormat::SURFACE_R5G5B5;
		m_palfmt = SurfaceFormat::SURFACE_UNKNOWN;
		break;

	case 24:
		m_fmt = SurfaceFormat::SURFACE_R8G8B8;
		m_palfmt = SurfaceFormat::SURFACE_UNKNOWN;
		break;
	
	case 32:
		m_fmt = SurfaceFormat::SURFACE_A8R8G8B8;
		m_palfmt = SurfaceFormat::SURFACE_UNKNOWN;
		break;
	}
}


} // img

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
