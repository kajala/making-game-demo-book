#include <gr/SurfaceFormat.h>
#include <config.h>


namespace gr
{


/** 
 * DXT1-5 surface color data struct. 
 */
struct DXTColBlock
{
	uint16_t	col0;
	uint16_t	col1;
	uint8_t		rows[4];
};

/** 
 * DXT2-3 surface alpha data struct. 
 */
struct DXTAlphaBlockExplicit
{
	uint16_t	rows[4];
};

/** 
 * DXT4-5 surface alpha data struct. 
 */
struct DXTAlphaBlock3BitLinear
{
	uint8_t		alpha0;
	uint8_t		alpha1;
	uint8_t		values[6];
};


const char* const FORMAT_NAMES[] =
{
	/** The surface format is unknown. */
	"UNKNOWN",
	/** 24-bit RGB pixel format. */
	"R8G8B8",
	/** 24-bit RGB pixel format. */
	"B8G8R8",
	/** 32-bit RGB pixel format with alpha. */
	"A8R8G8B8",
	/** 32-bit RGB pixel format where 8 bits are reserved for each color. */
	"X8R8G8B8",
	/** 32-bit RGB pixel format where 8 bits are reserved for each color.  */
	"X8B8G8R8",
	/** 32-bit RGB pixel format with alpha.  */
	"A8B8G8R8",
	/** 16-bit RGB pixel format. (PS2) */
	"R5G6B5",
	/** 16-bit RGB pixel format. */
	"R5G5B5",
	/** 4-bit palettized pixel format. (PC/PS2) */
	"P4",
	/** 8-bit palettized pixel format. (PC/PS2)	*/
	"P8",
	/** 8-bit luminosity format. (PC/PS2) */
	"L8",
	/** 16-bit pixel format where 5 bits are reserved for color and 1 bit is reserved for transparency. */
	"A1R5G5B5",
	/** 16-bit RGB pixel format where 4 bits are reserved for each color. */
	"X4R4G4B4",
	/** 16-bit RGB pixel format. */
	"A4R4G4B4",
	/** 16-bit pixel format where 5 bits are reserved for color and 1 bit is reserved for transparency. */
	"A1B5G5R5",
	/** 8-bit RGB texture format. (PS2) */
	"R3G3B2",
	/** 8-bit RGB texture format. */
	"R3G2B3",
	/** 8-bit alpha-only. */
	"A8",
	/** 16-bit RGB pixel format with alpha.	*/
	"A8R3G3B2",
	/** 16-bit RGB pixel format with alpha.	*/
	"A8R3G2B3",
	/** DirectX compressed texture */
	"DXT1",
	/** DirectX compressed texture */
	"DXT3",
	/** DirectX compressed texture */
	"DXT5",
	/** 32-bit depth buffer format */
	"D32",
	/** 24-bit depth buffer format */
	"D24",
	/** 16-bit depth buffer format */
	"D16",
	/** "32", depth using 24 bits and stencil 8 bits */
	"D24S8",
};

/** 
 * Descriptions of surface formats: 
 * {format, bitcount, red mask, green mask, blue mask, alpha mask}. 
 */
static const uint32_t FORMATDESC[][6] = 
{
	{SurfaceFormat::SURFACE_UNKNOWN   ,  0, 0x00000000, 0x00000000, 0x00000000, 0x00000000},
	{SurfaceFormat::SURFACE_R8G8B8    , 24, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000},
	{SurfaceFormat::SURFACE_B8G8R8    , 24, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000},
	{SurfaceFormat::SURFACE_A8R8G8B8  , 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0xff000000},
	{SurfaceFormat::SURFACE_X8R8G8B8  , 32, 0x00ff0000, 0x0000ff00, 0x000000ff, 0x00000000},
	{SurfaceFormat::SURFACE_X8B8G8R8  , 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0x00000000},
	{SurfaceFormat::SURFACE_A8B8G8R8  , 32, 0x000000ff, 0x0000ff00, 0x00ff0000, 0xff000000},
	{SurfaceFormat::SURFACE_R5G6B5    , 16, 0x0000f800, 0x000007e0, 0x0000001f, 0x00000000},
	{SurfaceFormat::SURFACE_R5G5B5    , 16, 0x00007c00, 0x000003e0, 0x0000001f, 0x00000000},
	{SurfaceFormat::SURFACE_P4		 ,  4,  0x00000000, 0x00000000, 0x00000000, 0x00000000},
	{SurfaceFormat::SURFACE_P8		 ,  8,  0x00000000, 0x00000000, 0x00000000, 0x00000000},
	{SurfaceFormat::SURFACE_L8		 ,  8,  0x000000FF, 0x000000FF, 0x000000FF, 0x00000000},
	{SurfaceFormat::SURFACE_A1R5G5B5  , 16, 0x00007c00, 0x000003e0, 0x0000001f, 0x00008000},
	{SurfaceFormat::SURFACE_X4R4G4B4  , 16, 0x00000f00, 0x000000f0, 0x0000000f, 0x00000000},
	{SurfaceFormat::SURFACE_A4R4G4B4  , 16, 0x00000f00, 0x000000f0, 0x0000000f, 0x0000f000},
	{SurfaceFormat::SURFACE_A1B5G5R5  , 16, 0x0000001f, 0x000003e0, 0x00007c00, 0x00008000},
	{SurfaceFormat::SURFACE_R3G3B2    ,  8, 0x000000e0, 0x0000001c, 0x00000003, 0x00000000},
	{SurfaceFormat::SURFACE_R3G2B3    ,  8, 0x000000e0, 0x00000018, 0x00000007, 0x00000000},
	{SurfaceFormat::SURFACE_A8        ,  8, 0x00000000, 0x00000000, 0x00000000, 0x000000ff},
	{SurfaceFormat::SURFACE_A8R3G3B2  , 16, 0x000000e0, 0x0000001c, 0x00000003, 0x0000ff00},
	{SurfaceFormat::SURFACE_A8R3G2B3  , 16, 0x000000e0, 0x00000018, 0x00000007, 0x0000ff00},
	{SurfaceFormat::SURFACE_DXT1	  , 0,  0x00000000, 0x00000000, 0x00000000, 0x00000000},
	{SurfaceFormat::SURFACE_DXT3	  , 0,  0x00000000, 0x00000000, 0x00000000, 0x00000000},
	{SurfaceFormat::SURFACE_DXT5	  , 0,  0x00000000, 0x00000000, 0x00000000, 0x00000000},
	{SurfaceFormat::SURFACE_D32		  , 32, 0x00000000, 0x00000000, 0x00000000, 0x00000000},
	{SurfaceFormat::SURFACE_D24		  , 24, 0x00000000, 0x00000000, 0x00000000, 0x00000000},
	{SurfaceFormat::SURFACE_D16		  , 16, 0x00000000, 0x00000000, 0x00000000, 0x00000000},
	{SurfaceFormat::SURFACE_D24S8	  , 32, 0x00000000, 0x00000000, 0x00000000, 0x00000000},
};

/** Number of supported surface formats. */
static const int FORMATS = sizeof(FORMATDESC)/sizeof(FORMATDESC[0]);


/** 
 * Returns left zero bit count before first non-zero bit.
 */
static inline int maskToShift( uint32_t mask )
{
	int count = 0;
	uint32_t testBit = 1;
	if ( 0 == mask )
		return 32;

	while ( 0 == (mask & testBit) )
	{
		++count;
		testBit += testBit;
	}
	return count;
}

/** 
 * Returns number of non-zero bits in the dword.
 */
static inline int countBits( uint32_t mask )
{
	int count = 0;
	uint32_t testBit = 1;
	while ( 0 != testBit )
	{
		if ( 0 != (mask&testBit) )
			++count;
		testBit += testBit;
	}
	return count;
}

/** 
 * Returns red component of R5G6B5 in range [0,255]. 
 */
static inline uint32_t getRedR5G6B5( uint32_t c )
{
	return (c>>11) * 255 / 31;
}

/** 
 * Returns green component of R5G6B5 in range [0,255]. 
 */
static inline uint32_t getGreenR5G6B5( uint32_t c )
{
	return ((c>>5)&0x3F) * 255 / 63;
}

/** 
 * Returns blue component of R5G6B5 in range [0,255]. 
 */
static inline uint32_t getBlueR5G6B5( uint32_t c )
{
	return (c&0x1F) * 255 / 31;
}

/** 
 * Returns ARGB8888 from values in range [0,255]. 
 */
static inline uint32_t makeA8R8G8B8( uint32_t red, uint32_t green, uint32_t blue, uint32_t alpha )
{
	return (alpha<<24) + (red<<16) + (green<<8) + blue;
}

/** 
 * Returns R5G6B5 as ARGB8888. 
 */
static inline uint32_t convertR5G6B5toARGB8888( uint32_t c )
{
	return makeA8R8G8B8( getRedR5G6B5(c), getGreenR5G6B5(c), getBlueR5G6B5(c), 0xFF );
}

/** 
 * Gets DXT1-3 block colors. 
 */
static inline void getBlockColorsA8R8G8B8( const DXTColBlock* colorBlock, uint32_t c[4] )
{
	if ( colorBlock->col0 > colorBlock->col1 ) 
	{
		c[0] = convertR5G6B5toARGB8888( colorBlock->col0 );
		c[1] = convertR5G6B5toARGB8888( colorBlock->col1 );

		// Four-color block: derive the other two colors.
		// 00 = c[0], 01 = c[1], 10 = c[2], 11 = c[3]
		// These 2-bit codes correspond to the 2-bit fields 
		// stored in the 64-bit block.
		c[2] = makeA8R8G8B8( 
			(2*getRedR5G6B5(colorBlock->col0)+getRedR5G6B5(colorBlock->col1)+1)/3, 
			(2*getGreenR5G6B5(colorBlock->col0)+getGreenR5G6B5(colorBlock->col1)+1)/3,
			(2*getBlueR5G6B5(colorBlock->col0)+getBlueR5G6B5(colorBlock->col1)+1)/3,
			255 );

		c[3] = makeA8R8G8B8( 
			(getRedR5G6B5(colorBlock->col0)+2*getRedR5G6B5(colorBlock->col1)+1)/3, 
			(getGreenR5G6B5(colorBlock->col0)+2*getGreenR5G6B5(colorBlock->col1)+1)/3,
			(getBlueR5G6B5(colorBlock->col0)+2*getBlueR5G6B5(colorBlock->col1)+1)/3,
			255 );
	}	 
	else
	{ 
		c[0] = convertR5G6B5toARGB8888( colorBlock->col0 );
		c[1] = convertR5G6B5toARGB8888( colorBlock->col1 );

		// Three-color block: derive the other color.
		// 00 = c[0],  01 = c[1],  10 = c[2],  
		// 11 = transparent.
		// These 2-bit codes correspond to the 2-bit fields 
		// stored in the 64-bit block. 
		c[2] = makeA8R8G8B8( 
			(getRedR5G6B5(colorBlock->col0)+getRedR5G6B5(colorBlock->col1))/2,
			(getGreenR5G6B5(colorBlock->col0)+getGreenR5G6B5(colorBlock->col1))/2,
			(getBlueR5G6B5(colorBlock->col0)+getBlueR5G6B5(colorBlock->col1))/2,
			255 );

		c[3] = makeA8R8G8B8(0,0,0,0);
	}
}

/** 
 * Gets DXT4-5 block alpha values. 
 */
static inline void getBlockAlphas( const DXTAlphaBlock3BitLinear* alphaBlock, uint32_t a[8] )
{
	// 8-alpha or 6-alpha block?   
	a[0] = alphaBlock->alpha0;
	a[1] = alphaBlock->alpha1;

	if ( alphaBlock->alpha0 > alphaBlock->alpha1 )
	{    
		// 8-alpha block:  derive the other six alphas.
		// Bit code 000 = a[0], 001 = a[1], others are interpolated.
		a[2] = (6 * alphaBlock->alpha0 + 1 * alphaBlock->alpha1 + 3) / 7;		// Bit code 010
		a[3] = (5 * alphaBlock->alpha0 + 2 * alphaBlock->alpha1 + 3) / 7;		// Bit code 011
		a[4] = (4 * alphaBlock->alpha0 + 3 * alphaBlock->alpha1 + 3) / 7;		// Bit code 100
		a[5] = (3 * alphaBlock->alpha0 + 4 * alphaBlock->alpha1 + 3) / 7;		// Bit code 101
		a[6] = (2 * alphaBlock->alpha0 + 5 * alphaBlock->alpha1 + 3) / 7;		// Bit code 110
		a[7] = (1 * alphaBlock->alpha0 + 6 * alphaBlock->alpha1 + 3) / 7;		// Bit code 111  
	}    
	else
	{  
		// 6-alpha block.
		// Bit code 000 = a[0], 001 = a[1], others are interpolated.
		a[2] = (4 * alphaBlock->alpha0 + 1 * alphaBlock->alpha1 + 2) / 5;		// Bit code 010
		a[3] = (3 * alphaBlock->alpha0 + 2 * alphaBlock->alpha1 + 2) / 5;		// Bit code 011
		a[4] = (2 * alphaBlock->alpha0 + 3 * alphaBlock->alpha1 + 2) / 5;		// Bit code 100
		a[5] = (1 * alphaBlock->alpha0 + 4 * alphaBlock->alpha1 + 2) / 5;		// Bit code 101
		a[6] = 0;																// Bit code 110
		a[7] = 255;																// Bit code 111
	}
}

/** 
 * Reads ARGB8888 pixel value from DXT1-compressed texture. 
 */
static inline uint32_t getPixelDXT1( int x, int y, const void* data, int pitch )
{
	int xblock = x >> 2;
	int yblock = y >> 2;

	const uint8_t* blockData = reinterpret_cast<const uint8_t*>(data) + xblock*8 + yblock*pitch;
	const DXTColBlock* colorBlock = reinterpret_cast<const DXTColBlock*>( blockData );

	uint32_t c[4];
	getBlockColorsA8R8G8B8( colorBlock, c );
	
	uint8_t b = colorBlock->rows[ y & 3 ];
	return c[ ((b>>(x&3)*2)&3) ];
}

/** 
 * Reads ARGB8888 pixel value from DXT2/3-compressed texture. 
 */
static inline uint32_t getPixelDXT3( int x, int y, const void* data, int pitch )
{
	int xblock = x >> 2;
	int yblock = y >> 2;

	const uint8_t* blockData = reinterpret_cast<const uint8_t*>(data) + xblock*16 + yblock*pitch;
	const DXTAlphaBlockExplicit* alphaBlock = reinterpret_cast<const DXTAlphaBlockExplicit*>( blockData );
	const DXTColBlock* colorBlock = reinterpret_cast<const DXTColBlock*>( blockData+8 );

	uint32_t c[4];
	getBlockColorsA8R8G8B8( colorBlock, c );
	uint8_t b = colorBlock->rows[ y & 3 ];
	uint32_t color = c[ ((b>>(x&3)*2)&3) ];

	uint32_t alpha = ( alphaBlock->rows[y&3] >> (4*(x&3)) ) & 0xF;
	alpha = (alpha * 255) / 15;

	return (color&0xFFFFFF) + (alpha<<24);
}

/** 
 * Reads ARGB8888 pixel value from DXT4/5-compressed texture. 
 */
static inline uint32_t getPixelDXT5( int x, int y, const void* data, int pitch )
{
	int xblock = x >> 2;
	int yblock = y >> 2;

	const uint8_t* blockData = reinterpret_cast<const uint8_t*>(data) + xblock*16 + yblock*pitch;
	const DXTAlphaBlock3BitLinear* alphaBlock = reinterpret_cast<const DXTAlphaBlock3BitLinear*>( blockData );
	const DXTColBlock* colorBlock = reinterpret_cast<const DXTColBlock*>( blockData+8 );

	uint32_t c[4];
	getBlockColorsA8R8G8B8( colorBlock, c );
	uint8_t b = colorBlock->rows[ y & 3 ];
	uint32_t color = c[ ((b>>(x&3)*2)&3) ];

	uint32_t a[8];
	getBlockAlphas( alphaBlock, a );
	
	// first two rows of 4 pixels each:
	const uint32_t mask = 0x00000007;		// bits = 00 00 01 11
	uint32_t bits = *reinterpret_cast<const uint32_t*>( alphaBlock->values );
	uint8_t alphaBits[4][4];
	alphaBits[0][0] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[0][1] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[0][2] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[0][3] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[1][0] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[1][1] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[1][2] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[1][3] = (uint8_t)( bits & mask );

	// now for last two rows:
	bits = *reinterpret_cast<const uint32_t*>( alphaBlock->values+3 );
	alphaBits[2][0] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[2][1] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[2][2] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[2][3] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[3][0] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[3][1] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[3][2] = (uint8_t)( bits & mask );
	bits >>= 3;
	alphaBits[3][3] = (uint8_t)( bits & mask );

	// get alpha value
	uint32_t alpha = a[ alphaBits[y&3][x&3] & 7 ];
	
	return (color&0xFFFFFF) + (alpha<<24);
}


SurfaceFormat::SurfaceFormat() :
	m_type( SurfaceFormat::SURFACE_UNKNOWN )
{
	assert( SURFACE_LAST == FORMATS );
}

SurfaceFormat::SurfaceFormat( SurfaceFormatType type ) :
	m_type( type )
{
	assert( (SurfaceFormatType)FORMATDESC[type][0] == type );
	assert( SURFACE_LAST == FORMATS );
}

SurfaceFormat::SurfaceFormat( int bitCount, uint32_t redMask, uint32_t greenMask, uint32_t blueMask, uint32_t alphaMask )
{
	m_type = SurfaceFormat::SURFACE_UNKNOWN;

	for ( int i = 0 ; i < FORMATS ; ++i )
	{
		if ( FORMATDESC[i][1] == (uint32_t)bitCount &&
			FORMATDESC[i][2] == (uint32_t)redMask &&
			FORMATDESC[i][3] == (uint32_t)greenMask &&
			FORMATDESC[i][4] == (uint32_t)blueMask &&
			FORMATDESC[i][5] == (uint32_t)alphaMask )
		{
			m_type = (SurfaceFormat::SurfaceFormatType)FORMATDESC[i][0];
		}
	}
}

SurfaceFormat::SurfaceFormatType SurfaceFormat::type() const
{
	return m_type;
}

int SurfaceFormat::bitsPerPixel() const
{
	return (int)FORMATDESC[m_type][1];
}

bool SurfaceFormat::hasAlpha() const
{
	return FORMATDESC[m_type][5] > 0 || compressed();
}

uint32_t SurfaceFormat::getChannelMask( int i ) const
{
	assert( i >= 0 && i < 4 );
	assert( !compressed() );

	if ( compressed() )
		return 0;
	else
		return FORMATDESC[m_type][2+i];
}

int SurfaceFormat::getChannelBitCount( int i ) const
{
	assert( i >= 0 && i < 4 );
	assert( !compressed() );

	if ( compressed() )
		return 0;
	else
		return countBits( FORMATDESC[m_type][2+i] );
}

int SurfaceFormat::paletteEntries() const
{
	if ( SURFACE_P8 == m_type )
		return 256;
	else if ( SURFACE_P4 == m_type )
		return 16;
	else
		return 0;
}

const char* SurfaceFormat::toString() const
{
	assert( m_type < SURFACE_LAST );
	return FORMAT_NAMES[m_type];
}

void SurfaceFormat::copyPixels( void* dst, const SurfaceFormat& dstpalfmt, const void* dstpal, 
	const SurfaceFormat& srcfmt, const void* src, const SurfaceFormat& srcpalfmt, const void* srcpal,
	int pixels ) const
{
	assert( !compressed() );
	assert( !srcfmt.compressed() );
	assert( srcfmt.m_type != SURFACE_UNKNOWN );
	assert( srcpalfmt.m_type != SURFACE_P4 && srcpalfmt.m_type != SURFACE_P8 );
	assert( dstpalfmt.m_type != SURFACE_P4 && dstpalfmt.m_type != SURFACE_P8 );
	assert( !srcpalfmt.palettized() );
	assert( !srcpalfmt.palettized() );
	assert( srcpalfmt.m_type == SURFACE_UNKNOWN || srcpal );
	assert( dstpalfmt.m_type == SURFACE_UNKNOWN || dstpal );

	if ( !compressed() && !srcfmt.compressed() )
	{
		if ( !srcfmt.palettized() )
			srcpal = 0;
		if ( !palettized() )
			dstpal = 0;

		const uint8_t*		srcbytes		= reinterpret_cast<const uint8_t*>( src );
		const uint8_t*		srcpalbytes		= reinterpret_cast<const uint8_t*>( srcpal );
		const int			srcpalentries	= srcfmt.paletteEntries();
		const int			srcbitcount		= srcfmt.bitsPerPixel();
		int					srcsubpix		= 0;
		const int			srcbitoffs		= (8-srcbitcount) & 7;
		const uint32_t*		srcfmtdesc		= &FORMATDESC[ (srcpal ? srcpalfmt.m_type : srcfmt.m_type) ][1];
		const uint32_t*		srcmask			= &srcfmtdesc[1];
		int					srcbits[4];
		int					srcshift[4];
		uint32_t 			srcval[4];
		const int 			srcpixbitcount	= srcfmtdesc[0];
		const int			srcpixsize		= srcpixbitcount >> 3;
		const uint8_t* 		srcpixbytes;

		uint8_t*			dstbytes		= reinterpret_cast<uint8_t*>( dst );
		const uint8_t*		dstpalbytes		= reinterpret_cast<const uint8_t*>( dstpal );
		const int			dstpalentries	= paletteEntries();
		const int			dstbitcount		= bitsPerPixel();
		int					dstsubpix		= 0;
		const uint32_t*		dstfmtdesc		= &FORMATDESC[ (dstpal ? dstpalfmt.m_type : m_type) ][1];
		const uint32_t*		dstmask			= &dstfmtdesc[1];
		int					dstbits[4];
		int					dstshift[4];
		uint32_t 			dstval[4];
		const int 			dstpixbitcount	= dstfmtdesc[0];
		const int			dstpixsize		= dstpixbitcount >> 3;
		const uint8_t* 		dstpixbytes;

		// compute aux channel variables
		for ( int i = 0 ; i < 4 ; ++i )
		{
			srcbits[i] = countBits( srcmask[i] );
			srcshift[i] = maskToShift( srcmask[i] );
			dstbits[i] = countBits( dstmask[i] );
			dstshift[i] = maskToShift( dstmask[i] );
		}

		for ( int i = 0 ; i < pixels ; ++i )
		{
			// apply palette if any
			if ( srcpal )
			{
				int ix = (*srcbytes >> (srcbitoffs-srcsubpix&7)) & (srcpalentries-1);
				assert( (unsigned)ix < (unsigned)srcpalentries ); // color index out of source palette range
				srcpixbytes = srcpalbytes + ix*srcpixsize;
			}
			else
			{
				srcpixbytes = srcbytes;
			}
			
			// read pixel data
			uint32_t srcpix = *srcpixbytes;
			switch ( srcpixbitcount )
			{
			case 32:	srcpix |= (uint32_t)srcpixbytes[3] << 24; // fall-through -> 24
			case 24:	srcpix |= (uint32_t)srcpixbytes[2] << 16; // fall-through -> 16
			case 16:	srcpix |= (uint32_t)srcpixbytes[1] << 8; break;
			}
			
			// convert pixel to R8G8B8A8
			for ( int k = 0 ; k < 4 ; ++k )
				srcval[k] = ( (srcpix & srcmask[k]) >> srcshift[k] ) << (8-srcbits[k]);
			
			// convert R8G8B8A8 to destination format
			uint32_t dstpix = uint32_t(-1);
			if ( dstpal )
			{
				// find best match in dst palette
				int mindist = 1<<30;
				dstpixbytes = dstpalbytes;
				for ( int n = 0 ; n < dstpalentries ; ++n )
				{
					// read dst palette data
					uint32_t pix = *dstpixbytes;
					switch ( dstpixbitcount )
					{
					case 32:	pix |= (uint32_t)dstpixbytes[3] << 24; // fall-through -> 24
					case 24:	pix |= (uint32_t)dstpixbytes[2] << 16; // fall-through -> 16
					case 16:	pix |= (uint32_t)dstpixbytes[1] << 8; break;
					}

					// convert dst palette data and compute distance to target color
					int dist = 0;
					for ( int k = 0 ; k < 4 ; ++k )
					{
						dstval[k] = ( (pix & dstmask[k]) >> dstshift[k] ) << (8-dstbits[k]);
						dist += (dstval[k]-srcval[k]) * (dstval[k]-srcval[k]);
					}
					
					// find closest color
					if ( dist < mindist )
					{
						mindist = dist;
						dstpix = n;
						if ( 0 == mindist )
							break;
					}
					
					dstpixbytes += dstpixsize;
				}
			}
			else
			{
				// convert channels to destination format
				for ( int k = 0 ; k < 4 ; ++k )
				{
					uint32_t dstv = ( srcval[k] >> (8-dstbits[k]) ) << dstshift[k];
					dstpix = (dstpix & ~dstmask[k]) | dstv;
					if ( 0 == srcbits[k] )
						dstpix |= dstmask[k];
				}
			}
			
			// write data
			switch ( dstbitcount )
			{
			case 32:	dstbytes[3] = (uint8_t)( dstpix >> 24 ); // fall-through -> 24
			case 24:	dstbytes[2] = (uint8_t)( dstpix >> 16 ); // fall-through -> 16
			case 16:	dstbytes[1] = (uint8_t)( dstpix >> 8 ); // fall-through -> 8
			case 8:		dstbytes[0] = (uint8_t)( dstpix ); break;
			case 4:		dstbytes[0] = (uint8_t)( dstbytes[0] & (~(0xF << dstsubpix)) );
						dstbytes[0] = (uint8_t)( dstbytes[0] + (dstpix << dstsubpix) ); break;
			}

			srcsubpix += srcbitcount;
			srcbytes += srcsubpix>>3;
			srcsubpix &= 7;
			
			dstsubpix += dstbitcount;
			dstbytes += dstsubpix>>3;
			dstsubpix &= 7;
		}
	}
}

void SurfaceFormat::copyPixels( void* dst, int dstpitch, const SurfaceFormat& dstpalfmt, const void* dstpal,
	const SurfaceFormat& srcfmt, const void* src, int srcpitch, const SurfaceFormat& srcpalfmt, const void* srcpal,
	int width, int height ) const
{
	assert( !compressed() );
	assert( bitsPerPixel() >= 8 || !srcfmt.compressed() );

	const int bytesperpixel = bitsPerPixel()/8;

	for ( int y = 0 ; y < height ; ++y )
	{
		uint8_t* d = reinterpret_cast<uint8_t*>(dst) + y*dstpitch;

		switch ( srcfmt.m_type )
		{
		case SURFACE_DXT1:
			for ( int x = 0 ; x < width ; ++x )
			{
				uint32_t pix = getPixelDXT1( x, y, src, srcpitch );
				copyPixels( d, dstpalfmt, dstpal, SurfaceFormat::SURFACE_A8R8G8B8, &pix, SurfaceFormat(), 0, 1 );
				d += bytesperpixel;
			}
			break;

		case SURFACE_DXT3:
			for ( int x = 0 ; x < width ; ++x )
			{
				uint32_t pix = getPixelDXT3( x, y, src, srcpitch );
				copyPixels( d, dstpalfmt, dstpal, SurfaceFormat::SURFACE_A8R8G8B8, &pix, SurfaceFormat(), 0, 1 );
				d += bytesperpixel;
			}
			break;

		case SURFACE_DXT5:
			for ( int x = 0 ; x < width ; ++x )
			{
				uint32_t pix = getPixelDXT5( x, y, src, srcpitch );
				copyPixels( d, dstpalfmt, dstpal, SurfaceFormat::SURFACE_A8R8G8B8, &pix, SurfaceFormat(), 0, 1 );
				d += bytesperpixel;
			}
			break;

		default:{
			const uint8_t* s = reinterpret_cast<const uint8_t*>(src) + y*srcpitch;
			copyPixels( d, dstpalfmt, dstpal, srcfmt, s, srcpalfmt, srcpal, width );
			break;}
		}
	}

}

void SurfaceFormat::getPixel( int x, int y,
	const void* data, int pitch, 
	const SurfaceFormat& palfmt, const void* pal,
	float* rgba ) const
{
	uint32_t pix = 0;

	switch ( m_type )
	{
	case SURFACE_DXT1:
		pix = getPixelDXT1( x, y, data, pitch );
		break;

	case SURFACE_DXT3:	
		pix = getPixelDXT3( x, y, data, pitch );
		break;

	case SURFACE_DXT5:
		pix = getPixelDXT5( x, y, data, pitch );
		break;

	default:{
		int pixelbytes = (int)FORMATDESC[m_type][1] >> 3;
		const uint8_t* src = reinterpret_cast<const uint8_t*>(data) + y*pitch + x*pixelbytes;
		SurfaceFormat(SURFACE_A8R8G8B8).copyPixels( &pix, SurfaceFormat(), 0, *this, src, palfmt, pal, 1 );
		}
	}

	uint8_t* buf = (uint8_t*)&pix;
	rgba[2] = (float)buf[0] * (1.f/255.f);
	rgba[1] = (float)buf[1] * (1.f/255.f);
	rgba[0] = (float)buf[2] * (1.f/255.f);
	rgba[3] = (float)buf[3] * (1.f/255.f);
}


} // gr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
