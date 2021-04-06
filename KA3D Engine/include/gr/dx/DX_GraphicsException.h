#ifndef _GR_DX_GRAPHICSEXCEPTION_H
#define _GR_DX_GRAPHICSEXCEPTION_H


#include <gr/GraphicsException.h>


namespace gr
{


/** DirectX Graphics exception. */
class DX_GraphicsException :
	public GraphicsException
{
public:
	/** Creates an exception with the specified error description. */
	DX_GraphicsException( const lang::Format& msg )				: GraphicsException(msg) {}
};


} // gr


#endif // _GR_DX_GRAPHICSEXCEPTION_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
