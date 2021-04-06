#ifndef _GR_GRAPHICSEXCEPTION_H
#define _GR_GRAPHICSEXCEPTION_H


#include <lang/Exception.h>


namespace gr
{


/** 
 * Graphics library exception. 
 * @ingroup gr
 */
class GraphicsException :
	public lang::Exception
{
public:
	/** Creates an exception with the specified error description. */
	GraphicsException( const lang::Format& msg )			: Exception(msg) {}
};


} // gr


#endif // _GR_GRAPHICSEXCEPTION_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
