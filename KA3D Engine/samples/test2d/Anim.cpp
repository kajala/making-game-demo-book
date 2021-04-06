#include "Anim.h"
#include <io/FileInputStream.h>
#include <img/ImageReader.h>
#include <lang/Math.h>
#include <lang/Exception.h>
#include <stdio.h>
#include <config.h>


using namespace gr;
using namespace io;
using namespace img;
using namespace hgr;
using namespace lang;


Anim::Anim( Context* context, Console* console, 
   const String& filenamefmt, int firstframe, int framecount ) :
	m_context( context ),
	m_console( console ),
	m_w( 0 ),
	m_h( 0 ),
	m_frames( framecount )
{
	// load first frame to get dimensions
	char filenamebuf[1024];
	sprintf( filenamebuf, filenamefmt.c_str(), firstframe );
	{FileInputStream in( filenamebuf );
	ImageReader reader( &in, ImageReader::guessFileFormat(filenamebuf) );
	m_w = reader.surfaceWidth();
	m_h = reader.surfaceHeight();}

	// calculate needed texture size
	int blocksqrt = int(Math::sqrt(float(m_frames)));
	int w0 = (blocksqrt+1) * m_w;
	int h0 = (blocksqrt+1) * m_h;
	int w = 1;
	while ( w < w0 || w < h0 )
		w <<= 1;

	// create anim texture
	int blocksinrow = w/m_w;
	m_tex = m_context->createTexture( w, w, SurfaceFormat::SURFACE_A8R8G8B8, 0, Context::USAGE_STATIC );

	Array<int> data;
	for ( int i = 0 ; i < m_frames ; ++i )
	{
		int blockx = i % blocksinrow;
		int blocky = i / blocksinrow;

		sprintf( filenamebuf, filenamefmt.c_str(), firstframe+i );
		FileInputStream in( filenamebuf );
		ImageReader reader( &in, ImageReader::guessFileFormat(filenamebuf) );
		if ( reader.surfaceWidth() != m_w || reader.surfaceHeight() != m_h )
			throwError( Exception( Format("Invalid anim sequence {0}, frame {1} not same size", filenamefmt, filenamebuf) ) );

		data.resize( reader.surfaceWidth()*reader.surfaceHeight() );
		reader.readSurface( &data[0], reader.surfaceWidth()*4, reader.surfaceWidth(), reader.surfaceHeight(), SurfaceFormat::SURFACE_A8R8G8B8, 0, SurfaceFormat() );

		int x0 = blockx*m_w;
		int y0 = blocky*m_h;
		m_tex->blt( x0, y0, &data[0], m_w*4, m_w, m_h, SurfaceFormat::SURFACE_A8R8G8B8, 0, SurfaceFormat() );

		// store rectangle (could be re-calculated later but this is just simpler)
		m_rects.add( Rect(x0,y0,x0+m_w,y0+m_h) );
	}
}

void Anim::render( int x, int y, int frameindex )
{
	assert( frameindex >= 0 && frameindex < m_frames );

	Rect rc = m_rects[frameindex];
	m_console->drawImage( x, y, rc, m_tex );
}

int	Anim::width() const
{
	return m_w;
}

int	Anim::height() const
{
	return m_h;
}

int	Anim::frames() const
{
	return m_frames;
}

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
