#ifndef _ANIM_H
#define _ANIM_H


#include <gr/Context.h>
#include <gr/Texture.h>
#include <hgr/Console.h>
#include <lang/Array.h>
#include <lang/String.h>


class Anim :
	public lang::Object
{
public:
	Anim( gr::Context* context, hgr::Console* console, 
		const lang::String& filenamefmt, int firstframe, int framecount );

	void	render( int x, int y, int frameindex );

	int		width() const;

	int		height() const;
	
	int		frames() const;

private:
	P(gr::Context)			m_context;
	P(gr::Texture)			m_tex;
	P(hgr::Console)			m_console;
	int						m_w;
	int						m_h;
	int						m_frames;
	lang::Array<gr::Rect>	m_rects;
};


#endif // _ANIM_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
