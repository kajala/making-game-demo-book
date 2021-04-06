#ifndef _GR_DX_CUBETEXTURE_H
#define _GR_DX_CUBETEXTURE_H


#include "DX_common.h"
#include "DX_ContextItem.h"
#include <gr/CubeTexture.h>
#include <gr/SurfaceFormat.h>
#include <lang/String.h>
#include <stdint.h>


namespace gr
{


class DX_Context;


/**
 * DirectX cube texture.
 */
class DX_CubeTexture :
	public CubeTexture,
	public DX_ContextItem
{
public:
	/**
	 * Initializes the texture from file.
	 */
	explicit DX_CubeTexture( DX_Context* context, const lang::String& filename );

	///
	~DX_CubeTexture();

	/**
	 * Called before reseting device.
	 */
	void	deviceLost();

	/**
	 * Called after reseting device.
	 */
	void	deviceReset();

	/**
	 * Returns texture top level surface width in pixels.
	 */
	int		width() const;

	/**
	 * Returns texture top level surface height in pixels.
	 */
	int		height() const;

	/**
	 * Returns surface pixel storage format of this texture.
	 */
	SurfaceFormat 	format() const;

	/**
	 * Returns object string description.
	 */
	lang::String	toString() const;

	/**
	 * Returns IDirect3DCubeTexture
	 */
	IDirect3DCubeTexture9*	texture() const			{return m_tex;}

private:
	IDirect3DCubeTexture9*	m_tex;
	lang::String			m_filename;
	D3DLOCKED_RECT			m_lockedRect;
	uint16_t				m_width;
	uint16_t				m_height;
	uint8_t					m_usageflags;
	SurfaceFormat			m_format;
	LockType				m_locked;

	void	create( const lang::String& filename );
	void	validateDesc( const lang::String& filename, int usageflags );

	DX_CubeTexture( const DX_CubeTexture& );
	DX_CubeTexture& operator=( const DX_CubeTexture& );
};

	
} // gr


#endif // _GR_DX_CUBETEXTURE_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
