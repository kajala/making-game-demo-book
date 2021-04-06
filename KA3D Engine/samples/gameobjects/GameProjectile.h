#ifndef _GAMEOBJECTS_GAMEPROJECTILE_H
#define _GAMEOBJECTS_GAMEPROJECTILE_H


#include "GameObject.h"
#include <hgr/Mesh.h>
#include <hgr/Scene.h>


class GameProjectile :
	public GameObject
{
public:
	GameProjectile( lua::LuaState* luastate, GameLevel* level );
	~GameProjectile();

	void		reset();

	void		setTimeToLive( float time );

	void		setSource( GameObject* obj );

	void		setMesh( lang::String filename );

	void		setSpeed( float speed );

	void		setDamage( float damage );

	void		setParticle( lang::String filename );

	void		update( float dt );

	void		getObjectsToRender( GameCamera* camera );

	void		setLight( hgr::Light* lt );

	int			collided( GameObject* obj, dContact* contacts, int numc );

	float		impulse() const;

	float		speed() const		{return m_speed;}

private:
	float					m_time;
	float					m_timeToLive;
	P(hgr::ParticleSystem)	m_particle;
	GameObject*				m_source;
	float					m_speed;
	float					m_damage;

	GameProjectile( const GameProjectile& );
	GameProjectile& operator=( const GameProjectile& );
};


#endif // _GAMEOBJECTS_GAMEPROJECTILE_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
