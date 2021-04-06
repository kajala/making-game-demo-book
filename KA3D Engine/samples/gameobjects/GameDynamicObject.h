#ifndef _GAMEOBJECTS_GAMEDYNAMICOBJECT_H
#define _GAMEOBJECTS_GAMEDYNAMICOBJECT_H


#include "GameObject.h"
#include <hgr/Mesh.h>
#include <hgr/Scene.h>


class GameDynamicObject :
	public GameObject
{
public:
	GameDynamicObject( lua::LuaState* luastate, GameLevel* level, hgr::Mesh* mesh, 
		ode::ODEObject::GeomType geomtype, ode::ODEObject::MassType masstype, float mass );

	~GameDynamicObject();

	void		update( float dt );

	void		getObjectsToRender( GameCamera* camera );

	int			collided( GameObject* obj, dContact* contacts, int numc );

private:
	float		m_time;

	GameDynamicObject( const GameDynamicObject& );
	GameDynamicObject& operator=( const GameDynamicObject& );
};


#endif // _GAMEOBJECTS_GAMEDYNAMICOBJECT_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
