#ifndef _DX_SURFACEFORMATUTIL_H
#define _DX_SURFACEFORMATUTIL_H


#include "DX_common.h"
#include <gr/SurfaceFormat.h>


namespace gr
{


/**
 * Converts D3D pixel format to SurfaceFormat.
 * 
 */
SurfaceFormat
	toSurfaceFormat( D3DFORMAT d3dfmt );

/**
 * Converts SurfaceFormat to D3DFORMAT.
 * 
 */
D3DFORMAT
	toD3D( const SurfaceFormat& fmt );

	
} // gr


#endif // _DX_SURFACEFORMATUTIL_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
