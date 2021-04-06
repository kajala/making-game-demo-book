// Note: In actual application you should
// include individual headers for optimal
// compilation performance. This example includes
// everything just for convenience.
#include <framework/App.h>
#include <gr/all.h>
#include <lang/all.h>


class DrawTriangleApp : 
	public framework::App
{
public:
	DrawTriangleApp( gr::Context* context );
	
	void update( float dt, gr::Context* context );

private:
	P(gr::Primitive) m_prim;
};

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
