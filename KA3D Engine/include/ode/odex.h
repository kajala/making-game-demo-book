#ifndef _ODE_ODEX_H
#define _ODE_ODEX_H


#define _ODE_ODECPP_H_
#include <ode/ode.h>


namespace math {
	class float3;
	class float3x3;
	class float3x4;}


void dWorldImpulseToForce( dWorldID world, float dt, const math::float3& imp, math::float3* f );

void dBodyGetAngularVel( dBodyID body, math::float3* v );
void dBodyGetLinearVel( dBodyID body, math::float3* v );
void dBodyGetPosition( dBodyID body, math::float3* v );
void dBodyGetRotation( dBodyID body, math::float3x3* v );
void dBodyGetTransform( dBodyID body, math::float3x4* v );
void dBodySetAngularVel( dBodyID body, const math::float3& v );
void dBodySetLinearVel( dBodyID body, const math::float3& v );
void dBodySetPosition( dBodyID body, const math::float3x4& v );
void dBodySetRotation( dBodyID body, const math::float3x3& v );
void dBodySetTransform( dBodyID body, const math::float3x4& v );
void dBodyAddForce( dBodyID body, const math::float3& f );
void dBodyAddTorque( dBodyID body, const math::float3& f );

void dGeomGetPosition( dGeomID geom, math::float3* v );
void dGeomGetRotation( dGeomID geom, math::float3x3* v );
void dGeomGetTransform( dGeomID geom, math::float3x4* v );
void dGeomSetPosition( dGeomID geom, const math::float3x4& v );
void dGeomSetRotation( dGeomID geom, const math::float3x3& v );
void dGeomSetTransform( dGeomID geom, const math::float3x4& f );


#endif // _ODE_ODEX_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
