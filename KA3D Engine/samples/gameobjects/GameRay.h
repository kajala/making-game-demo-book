#ifndef _GAMEOBJECTS_GAMERAY_H
#define _GAMEOBJECTS_GAMERAY_H


#include "GameObject.h"
#include <hgr/Mesh.h>
#include <hgr/Lines.h>
#include <hgr/Scene.h>


/**
 * Helper class for ray collision checking. 
 */
class GameRay :
	public GameObject
{
public:
	GameRay( lua::LuaState* luastate, GameLevel* level, int collides );
	~GameRay();

	void	setRay( const math::float3& origin, const math::float3& dir, float maxlen );

	void	setIgnoreObject( GameObject* ignore );

	int		collided( GameObject* obj, dContact* contacts, int numc );

	const math::float3&		endPoint() const	{return m_endPoint;}

private:
	math::float3	m_endPoint;
	GameObject*		m_ignore;

	GameRay( const GameRay& );
	GameRay& operator=( const GameRay& );
};


#endif // _GAMEOBJECTS_GAMERAY_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
