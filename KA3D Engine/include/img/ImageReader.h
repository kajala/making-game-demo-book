#ifndef _IMG_IMAGEREADER_H
#define _IMG_IMAGEREADER_H


#include <gr/SurfaceFormat.h>
#include <lang/Array.h>
#include <lang/Object.h>
#include <stdint.h>


namespace io {
	class InputStream;}

namespace lang {
	class String;}


namespace img
{


/**
 * Low level image file reader.
 * @ingroup img
 */
class ImageReader :
	public lang::Object
{	
public:
	/**
	 * Supported image file formats.
	 */
	enum FileFormat
	{
		/** Unknown image file format. */
		FILEFORMAT_UNKNOWN,
		/** BMP image file. */
		FILEFORMAT_BMP,
		/** TGA image file. */
		FILEFORMAT_TGA,
		/** JPG image file. */
		FILEFORMAT_JPG,
		/** DDS image file. */
		FILEFORMAT_DDS,
		/** PSD image file. */
		FILEFORMAT_PSD,
		/** PNG image file. */
		FILEFORMAT_PNG,
		/** PCX image file. */
		FILEFORMAT_PCX,
		/** PNM image file. */
		FILEFORMAT_PNM,
		/** GIF image file. */
		FILEFORMAT_GIF,
		/** TIFF image file. */
		FILEFORMAT_TIF,
	};
	
	/**
	 * Starts reading image file using specified format.
	 * Note that ImageReader does not add a reference to InputStream,
	 * so you must make sure InputStream exists as long as the image is read from it.
	 * @exception IOException
	 */
	ImageReader( io::InputStream* in, FileFormat filefmt );

	///
	~ImageReader();

	/**
	 * Reads data of current active surface and iterates reading to next surface.
	 *
	 * Iteration order for cubemaps:
	 * <ol>
	 * <li>All mipmaps for +X surface
	 * <li>All mipmaps for -X surface
	 * <li>All mipmaps for +Y surface
	 * <li>All mipmaps for -Y surface
	 * <li>All mipmaps for +Z surface
	 * <li>All mipmaps for -Z surface
	 * <li>...
	 * </ol>
	 *
	 * @exception IOException
	 */
	void 				readSurface( void* bits, int pitch, int w, int h, gr::SurfaceFormat fmt,
							const void* pal, gr::SurfaceFormat palfmt );
	
	/**
	 * Returns total number of surfaces to be read.
	 * This includes all mipmap levels too,
	 * i.e. for cubemap with three mipmap levels surfaces() will be 18.
	 */
	int					surfaces() const;

	/**
	 * Returns index of current surface being read.
	 */
	int					surfaceIndex() const;

	/**
	 * Returns true if the image is cube map.
	 */
	bool				cubeMap() const;

	/**
	 * Returns number of mipmap levels in image.
	 * Minimum returned value is 1 (this level).
	 */
	int					mipLevels() const;

	/**
	 * Returns pixel format of the image.
	 */
	gr::SurfaceFormat	format() const;

	/**
	 * Returns pointer to palette data if any.
	 * Palette data is file format dependent.
	 * @return 0 if no palette.
	 */
	const void*			paletteData() const;

	/**
	 * Returns pixel format of the palette if any.
	 * @return SURFACE_UNKNOWN if palette data format is non-standard.
	 */
	gr::SurfaceFormat	paletteFormat() const;
	
	/**
	 * Returns width of current active surface in pixels.
	 */
	int					surfaceWidth() const;
	
	/**
	 * Returns height of current active surface in pixels.
	 */
	int					surfaceHeight() const;

	/**
	 * Guesses image file type based on file name.
	 * @return FILEFORMAT_UNKNOWN if format cannot be guessed from file name suffix.
	 */
	static FileFormat	guessFileFormat( const lang::String& filename );

private:
	class Surface
	{
	public:
		lang::Array<uint8_t>	data;
		int						width;
		int						height;
	};

	io::InputStream*		m_in;
	int						m_width;
	int						m_height;
	int						m_bitsPerPixel;
	int						m_pitch;
	int						m_mipLevels;
	int						m_surfaces;
	int						m_surfaceIndex;
	uint8_t					m_colormap[256][4];
	lang::Array<uint8_t>	m_scanlinebuffer;
	gr::SurfaceFormat		m_fmt;
	gr::SurfaceFormat		m_palfmt;
	FileFormat				m_filefmt;
	bool					m_bottomUp;
	bool					m_cubeMap;
	uint64_t				m_ffbuffer[100]; // file format specific buffer
	lang::Array<Surface>	m_surfaceBuffer; // when loaded by IL

	static void			readFully( io::InputStream* in, void* buf, int bytes );
	static void			readColorMap( io::InputStream* in, int entrysize, int entries, uint8_t* colormap );
	static uint16_t		getUInt16LE( const void* data, int offset );
	static uint32_t		getUInt32LE( const void* data, int offset );

	void	readHeader_bmp();
	void	readHeader_tga();
	void	readHeader_jpg();
	void	readHeader_il();
	void 	readSurface_jpg( void* bits, int pitch, int w, int h, gr::SurfaceFormat fmt, const void* pal, gr::SurfaceFormat palfmt );
	void 	readSurface_il( void* bits, int pitch, int w, int h, gr::SurfaceFormat fmt, const void* pal, gr::SurfaceFormat palfmt );
	void 	readScanlines( void* bits, int pitch, int w, int h, gr::SurfaceFormat fmt, const void* pal, gr::SurfaceFormat palfmt );
	void	readScanline_jpg( void* buffer );
	void	finish_jpg();

	ImageReader( const ImageReader& );
	ImageReader& operator=( const ImageReader& );
};


} // img


#endif // _IMG_IMAGEREADER_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
