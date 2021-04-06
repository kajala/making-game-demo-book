#include <gr/Rect.h>
#include <lang/Math.h>
#include <config.h>


using namespace lang;


namespace gr
{


Rect::Rect() :
	m_x0( 0 ),
	m_y0( 0 ),
	m_x1( 0 ),
	m_y1( 0 )
{
}

Rect::Rect( int width, int height ) :
	m_x0( 0 ),
	m_y0( 0 ),
	m_x1( width ),
	m_y1( height )
{
}

Rect::Rect( int x0, int y0, int x1, int y1 ) :
	m_x0( x0 ),
	m_y0( y0 ),
	m_x1( x1 ),
	m_y1( y1 )
{
}

void Rect::setLeft( int x )
{
	m_x0 = x;
}

void Rect::setRight( int x )
{
	m_x1 = x;
}

void Rect::setTop( int y )
{
	m_y0 = y;
}

void Rect::setBottom( int y )
{
	m_y1 = y;
}

Rect& Rect::operator&=( const Rect& other )
{
	return *this = *this & other;
}

Rect& Rect::operator|=( const Rect& other )
{
	return *this = *this | other;
}

Rect Rect::operator&( const Rect& other ) const
{
	return Rect( 
		Math::max(m_x0,other.m_x0), Math::max(m_y0,other.m_y0),
		Math::min(m_x1,other.m_x1), Math::min(m_y1,other.m_y1) );
}

Rect Rect::operator|( const Rect& other ) const
{
	return Rect( 
		Math::min(m_x0,other.m_x0), Math::min(m_y0,other.m_y0),
		Math::max(m_x1,other.m_x1), Math::max(m_y1,other.m_y1) );
}


} // gr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
