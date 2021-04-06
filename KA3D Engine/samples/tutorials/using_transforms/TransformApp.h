// Note: In actual application you should
// include individual headers for optimal
// compilation performance. This example includes
// everything just for convenience.
#include <framework/App.h>
#include <gr/all.h>
#include <lang/all.h>
#include <math/all.h>


class TransformApp : 
	public framework::App
{
public:
	TransformApp( gr::Context* context );
	
	void update( float dt, gr::Context* context );

private:
	P(gr::Primitive)				m_prim;
	float							m_time;
};

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
