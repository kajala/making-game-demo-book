#include <hgr/Console.h>
#include <gr/Shader.h>
#include <gr/Context.h>
#include <gr/Primitive.h>
#include <gr/VertexFormat.h>
#include <lang/Debug.h>
#include <lang/Character.h>
#include <lang/UTFConverter.h>
#include <math/float2.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <config.h>


using namespace gr;
using namespace lang;
using namespace math;


namespace hgr
{


Console::Console( Context* context, 
	Texture* isolatin1, int fontwidth, int fontheight, int textoffset,
	const String& shadername ) :
	m_shader( context->createShader(shadername) ),
	m_fontWidth( fontwidth ),
	m_fontHeight( fontheight ),
	m_textOffset( textoffset ),
	m_cursorX( 0.f ),
	m_cursorY( 0.f )
{
	clear();
	addPage( isolatin1, 0 );
}

Console::~Console()
{
}

void Console::drawImage( int x, int y, Texture* tex )
{
	drawImage( x, y, Rect(0,0,tex->width(),tex->height()), tex );
}

void Console::drawImage( int x, int y, const Rect& subimage, Texture* tex )
{
	stretchImage( x, y, subimage.width(), subimage.height(), subimage, tex );
}

void Console::stretchImage( int x, int y, int w, int h, Texture* tex )
{
	stretchImage( x, y, w, h, Rect(0,0,tex->width(),tex->height()), tex );
}

void Console::stretchImage( int x, int y, int w, int h, const Rect& subimage, Texture* tex )
{
	Image img;
	img.pos = float3( float(x), float(y), 0.f );
	img.tex = tex;
	img.x0 = (float)subimage.left();
	img.y0 = (float)subimage.top();
	img.x1 = (float)subimage.right();
	img.y1 = (float)subimage.bottom();
	img.width = (float)w;
	img.height = (float)h;
	m_images.add( img );
}

void Console::printf( int x, int y, const char* fmt, ... )
{
	// format variable arguments
	const unsigned MAX_MSG = 2000;
	char msg[MAX_MSG+4];
	va_list marker;
	va_start( marker, fmt );
	vsprintf( msg, fmt, marker );
	va_end( marker );
	assert( strlen(msg) < MAX_MSG );

	put( (float)x, (float)y, msg );
}

void Console::printf( const char* fmt, ... )
{
	// format variable arguments
	const unsigned MAX_MSG = 2000;
	char msg[MAX_MSG+4];
	va_list marker;
	va_start( marker, fmt );
	vsprintf( msg, fmt, marker );
	va_end( marker );
	assert( strlen(msg) < MAX_MSG );

	put( m_cursorX, m_cursorY, msg );
}

void Console::put( float x, float y, const char* msg )
{
	UTFConverter conv( UTFConverter::ENCODING_UTF8 );
	m_cursorX = x;
	m_cursorY = y;

	for ( int i = 0 ; msg[i] ; )
	{
		TextChar ch;
		
		ch.x0 = m_cursorX;
		ch.y0 = m_cursorY;
		ch.code = 0;

		int bytes = 1;
		if ( conv.decode(&msg[i], m_text.end(), &bytes, &ch.code) )
		{
			if ( Character::isWhitespace(ch.code) )
			{
				if ( ch.code == '\n' )
				{
					m_cursorX = x;
					m_cursorY += m_fontHeight;
				}
				else
				{
					m_cursorX += (float)( m_fontWidth + m_textOffset );
				}

				ch.code = 0; // don't bother rendering whitespace
			}
			else
			{
				m_cursorX += (float)( m_fontWidth + m_textOffset );
			}
		}
		i += bytes;

		m_text.add( ch );
	}
}

void Console::clear()
{
	m_text.clear();
	m_images.clear();
	m_cursorX = 0;
	m_cursorY = 0;
}

void Console::render( Context* context )
{
	// draw text
	if ( m_text.size() > 0 )
	{
		assert( m_pages.size() > 0 );

		// font params
		const float fontw = (float)m_fontWidth;
		const float fonth = (float)m_fontHeight;

		// find out which texture pages are used
		int texpagelist[1024];
		int texpages = 0;
		for ( int i = 0 ; i < m_text.size() ; ++i )
		{
			int ch = m_text[i].code;
			int page = ch >> 8;

			for ( int k = 0 ; k < texpages ; ++k )
			{
				if ( texpagelist[k] == page )
				{
					page = -1;
					break;
				}
			}
			if ( -1 != page )
			{
				assert( texpages < 256 );
				texpagelist[texpages++] = page;
			}
		}

		// get temporary dynamic primitive from rendering context
		VertexFormat vf;
		vf.addTransformedPosition().addDiffuse().addTextureCoordinate( VertexFormat::DF_V2_32 );
		Primitive* prim = context->getDynamicPrimitive( Primitive::PRIM_TRI, vf, m_text.size()*6, 0 );

		// render / font texpage
		for ( int k = 0 ; k < texpages ; ++k )
		{
			const int pageindex = texpagelist[k];

			// set vertex data
			{
				Primitive::Lock lk( prim, Primitive::LOCK_WRITE );
				int vi = 0;

				// get data pointers
				assert( VertexFormat::DF_V4_32 == prim->vertexFormat().getDataFormat(VertexFormat::DT_POSITIONT) );
				assert( VertexFormat::DF_V4_8 == prim->vertexFormat().getDataFormat(VertexFormat::DT_DIFFUSE) );
				assert( VertexFormat::DF_V2_32 == prim->vertexFormat().getDataFormat(VertexFormat::DT_TEX0) );
				
				float* vpos;
				int vpospitch;
				prim->getVertexDataPtr( VertexFormat::DT_POSITIONT, reinterpret_cast<uint8_t**>(&vpos), &vpospitch );
				assert( (vpospitch&3) == 0 );
				vpospitch >>= 2;

				uint32_t* vcolor;
				int vcolorpitch;
				prim->getVertexDataPtr( VertexFormat::DT_DIFFUSE, reinterpret_cast<uint8_t**>(&vcolor), &vcolorpitch );
				assert( (vcolorpitch&3) == 0 );
				vcolorpitch >>= 2;

				float* vtexcoord;
				int vtexcoordpitch;
				prim->getVertexDataPtr( VertexFormat::DT_TEX0, reinterpret_cast<uint8_t**>(&vtexcoord), &vtexcoordpitch );
				assert( (vtexcoordpitch&3) == 0 );
				vtexcoordpitch >>= 2;

				// prepare sprite quads
				for ( int i = 0 ; i < m_text.size() ; ++i )
				{
					int ch = m_text[i].code;
					if ( ch > 0 )
					{
						int page = ch >> 8;
						ch &= 0xFF;

						if ( page >= 0 && page < m_pages.size() && m_pages[page] != 0 )
						{
							if ( page == pageindex )
							{
								Texture* tex = m_pages[pageindex];

								// prepare font character data
								float x = float(ch&0xF) * fontw;
								float y = float((ch>>4)&0xF) * fonth;
								float du = 1.f/float(tex->width());
								float dv = 1.f/float(tex->height());
								float u0 = du * float(x);
								float v0 = dv * float(y);
								float u1 = u0 + du * fontw;
								float v1 = v0 + dv * fonth;
								const float smallz = 0.0001f;
								float3 projpoint(0,0,smallz);
								float rhw = 1.f;
								float2 cursor( m_text[i].x0+.5f, m_text[i].y0+.5f );
								float2 p0( cursor );
								float2 p1( cursor + float2(fontw,0) );
								float2 p2( cursor + float2(fontw,fonth) );
								float2 p3( cursor + float2(0,fonth) );

								// set vertex positions (as two-triangle quad)
								vpos[0] = p0.x;
								vpos[1] = p0.y;
								vpos[2] = projpoint.z;
								vpos[3] = rhw;
								vpos += vpospitch;
								vpos[0] = p1.x;
								vpos[1] = p1.y;
								vpos[2] = projpoint.z;
								vpos[3] = rhw;
								vpos += vpospitch;
								vpos[0] = p2.x;
								vpos[1] = p2.y;
								vpos[2] = projpoint.z;
								vpos[3] = rhw;
								vpos += vpospitch;
								// tri 0,2,3
								vpos[0] = p0.x;
								vpos[1] = p0.y;
								vpos[2] = projpoint.z;
								vpos[3] = rhw;
								vpos += vpospitch;
								vpos[0] = p2.x;
								vpos[1] = p2.y;
								vpos[2] = projpoint.z;
								vpos[3] = rhw;
								vpos += vpospitch;
								vpos[0] = p3.x;
								vpos[1] = p3.y;
								vpos[2] = projpoint.z;
								vpos[3] = rhw;
								vpos += vpospitch;

								// set color0
								uint32_t color = 0xFFFFFFFF;
								*vcolor = color;
								vcolor += vcolorpitch;
								*vcolor = color;
								vcolor += vcolorpitch;
								*vcolor = color;
								vcolor += vcolorpitch;
								*vcolor = color;
								vcolor += vcolorpitch;
								*vcolor = color;
								vcolor += vcolorpitch;
								*vcolor = color;
								vcolor += vcolorpitch;

								// set texcoords
								vtexcoord[0] = u0;
								vtexcoord[1] = v0;
								vtexcoord += vtexcoordpitch;
								vtexcoord[0] = u1;
								vtexcoord[1] = v0;
								vtexcoord += vtexcoordpitch;
								vtexcoord[0] = u1;
								vtexcoord[1] = v1;
								vtexcoord += vtexcoordpitch;
								vtexcoord[0] = u0;
								vtexcoord[1] = v0;
								vtexcoord += vtexcoordpitch;
								vtexcoord[0] = u1;
								vtexcoord[1] = v1;
								vtexcoord += vtexcoordpitch;
								vtexcoord[0] = u0;
								vtexcoord[1] = v1;
								vtexcoord += vtexcoordpitch;

								vi += 6;
							}
						}
					}
				}

				prim->setVertexRangeEnd( vi );
			}

			// render prepared primitive
			if ( prim->vertexRangeEnd() > 0 && m_pages[pageindex] != 0 )
				draw( prim, m_pages[pageindex] );
		}
	}

	// draw images
	if ( m_images.size() > 0 )
	{
		// get temporary dynamic primitive from rendering context
		VertexFormat vf;
		vf.addTransformedPosition().addDiffuse().addTextureCoordinate( VertexFormat::DF_V2_32 );
		Primitive* prim = context->getDynamicPrimitive( Primitive::PRIM_TRI, vf, m_images.size()*6, 0 );

		for ( int k = 0 ; k < m_images.size() ; ++k )
		{
			// image props
			const Image& img = m_images[k];
			const float imgw = m_images[k].width;
			const float imgh = m_images[k].height;
			const float2 cursor( img.pos.x, img.pos.y );

			// set vertex data
			{
				Primitive::Lock lk( prim, Primitive::LOCK_WRITE );
				int vi = 0;

				// get data pointers
				assert( VertexFormat::DF_V4_32 == prim->vertexFormat().getDataFormat(VertexFormat::DT_POSITIONT) );
				assert( VertexFormat::DF_V4_8 == prim->vertexFormat().getDataFormat(VertexFormat::DT_DIFFUSE) );
				assert( VertexFormat::DF_V2_32 == prim->vertexFormat().getDataFormat(VertexFormat::DT_TEX0) );
				
				float* vpos;
				int vpospitch;
				prim->getVertexDataPtr( VertexFormat::DT_POSITIONT, reinterpret_cast<uint8_t**>(&vpos), &vpospitch );
				assert( (vpospitch&3) == 0 );
				vpospitch >>= 2;

				uint32_t* vcolor;
				int vcolorpitch;
				prim->getVertexDataPtr( VertexFormat::DT_DIFFUSE, reinterpret_cast<uint8_t**>(&vcolor), &vcolorpitch );
				assert( (vcolorpitch&3) == 0 );
				vcolorpitch >>= 2;

				float* vtexcoord;
				int vtexcoordpitch;
				prim->getVertexDataPtr( VertexFormat::DT_TEX0, reinterpret_cast<uint8_t**>(&vtexcoord), &vtexcoordpitch );
				assert( (vtexcoordpitch&3) == 0 );
				vtexcoordpitch >>= 2;

				// prepare image data
				float u0 = img.x0 / (float)img.tex->width();
				float v0 = img.y0 / (float)img.tex->height();
				float u1 = img.x1 / (float)img.tex->width();
				float v1 = img.y1 / (float)img.tex->height();
				const float smallz = 0.0001f;
				float3 projpoint(0,0,smallz);
				float rhw = 1.f;
				float2 p0( cursor );
				float2 p1( cursor + float2(imgw,0) );
				float2 p2( cursor + float2(imgw,imgh) );
				float2 p3( cursor + float2(0,imgh) );

				// set vertex positions (as two-triangle quad)
				vpos[0] = p0.x;
				vpos[1] = p0.y;
				vpos[2] = projpoint.z;
				vpos[3] = rhw;
				vpos += vpospitch;
				vpos[0] = p1.x;
				vpos[1] = p1.y;
				vpos[2] = projpoint.z;
				vpos[3] = rhw;
				vpos += vpospitch;
				vpos[0] = p2.x;
				vpos[1] = p2.y;
				vpos[2] = projpoint.z;
				vpos[3] = rhw;
				vpos += vpospitch;
				// tri 0,2,3
				vpos[0] = p0.x;
				vpos[1] = p0.y;
				vpos[2] = projpoint.z;
				vpos[3] = rhw;
				vpos += vpospitch;
				vpos[0] = p2.x;
				vpos[1] = p2.y;
				vpos[2] = projpoint.z;
				vpos[3] = rhw;
				vpos += vpospitch;
				vpos[0] = p3.x;
				vpos[1] = p3.y;
				vpos[2] = projpoint.z;
				vpos[3] = rhw;
				vpos += vpospitch;

				// set color0
				uint32_t color = 0xFFFFFFFF;
				*vcolor = color;
				vcolor += vcolorpitch;
				*vcolor = color;
				vcolor += vcolorpitch;
				*vcolor = color;
				vcolor += vcolorpitch;
				*vcolor = color;
				vcolor += vcolorpitch;
				*vcolor = color;
				vcolor += vcolorpitch;
				*vcolor = color;
				vcolor += vcolorpitch;

				// set texcoords
				vtexcoord[0] = u0;
				vtexcoord[1] = v0;
				vtexcoord += vtexcoordpitch;
				vtexcoord[0] = u1;
				vtexcoord[1] = v0;
				vtexcoord += vtexcoordpitch;
				vtexcoord[0] = u1;
				vtexcoord[1] = v1;
				vtexcoord += vtexcoordpitch;
				vtexcoord[0] = u0;
				vtexcoord[1] = v0;
				vtexcoord += vtexcoordpitch;
				vtexcoord[0] = u1;
				vtexcoord[1] = v1;
				vtexcoord += vtexcoordpitch;
				vtexcoord[0] = u0;
				vtexcoord[1] = v1;
				vtexcoord += vtexcoordpitch;

				vi += 6;
				prim->setVertexRangeEnd( vi );
			}

			// render prepared primitive
			if ( prim->vertexRangeEnd() > 0 )
				draw( prim, img.tex );
		}
	}
}

void Console::render( Context* context, Camera* /*camera*/, int priority )
{
	if ( priority == m_shader->priority() )
		render( context );
}

void Console::draw( Primitive* prim, Texture* tex )
{
	if ( tex != 0 )
	{
		m_shader->setTexture( "BASEMAP", tex );

		Shader::Begin use( m_shader );
		for ( int k = 0 ; k < use.passes() ; ++k )
		{
			Shader::Pass pass( m_shader, k );
			prim->render();
		}
	}
}

void Console::getShaders( Array<Shader*>& shaders )
{
	shaders.add( m_shader );
}

void Console::computeBound()
{
	setBoundInfinity();
}

void Console::addPage( Texture* tex, int page )
{
	assert( page >= 0 && page < 1024 );
	if ( page >= m_pages.size() )
		m_pages.resize( page+1 );
	m_pages[page] = tex;
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
