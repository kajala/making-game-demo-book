#ifndef _GR_BASETEXTURE_H 
#define _GR_BASETEXTURE_H


#include <gr/ContextObject.h>
#include <gr/SurfaceFormat.h>


namespace gr
{

	
/**
 * Base class for platform dependent textures.
 * @ingroup gr
 */
class BaseTexture :
	public ContextObject
{
public:
	explicit BaseTexture( ClassId classid );

	/**
	 * Returns texture top level surface width in pixels.
	 */
	virtual int				width() const = 0;

	/**
	 * Returns texture top level surface height in pixels.
	 */
	virtual int				height() const = 0;

	/**
	 * Returns surface pixel storage format of this texture.
	 */
	virtual SurfaceFormat 	format() const = 0;
};


} // gr


#endif // _GR_BASETEXTURE_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
