#ifndef _GAMEOBJECTS_GAMEWEAPON_H
#define _GAMEOBJECTS_GAMEWEAPON_H


#include "GameObject.h"
#include <hgr/Mesh.h>
#include <hgr/Lines.h>
#include <hgr/Scene.h>


class GameWeapon :
	public GameObject
{
public:
	GameWeapon( lua::LuaState* luastate, GameLevel* level );
	~GameWeapon();

	void		setOwner( GameObject* obj );

	void		setMesh( lang::String filename );

	void		setParentBone( lang::String bonename );

	void		setPrimaryEmissionBone( lang::String nodename );

	void		setSecondaryEmissionBone( lang::String nodename );

	void		setPrimaryAmmo( lang::String ammoname );

	void		setSecondaryAmmo( lang::String ammoname );

	void		update( float dt );

	void		getObjectsToRender( GameCamera* camera );

	void		setLight( hgr::Light* lt );

	/**
	 * Fixes weapon aim direction.
	 * @param handtm Weapon's parent transform in world space.
	 */
	void		setAim( const math::float3x4& handtm, const math::float3& target, float aimweight );

	void		firePrimary();

	hgr::Node*				root() const		{return m_root;}

	const lang::String&		parentBone() const	{return m_parentBone;}

private:
	struct Ammo
	{
		math::float3	pos;
		math::float3	vel;
		float			time;
	};

	P(hgr::Lines)		m_lines;
	lang::String		m_parentBone;
	lang::String		m_emission1;
	lang::String		m_emission2;
	lang::String		m_ammo1;
	lang::String		m_ammo2;
	math::float3		m_target;
	lang::Array<Ammo>	m_shots;
	math::float3x4		m_bodytm;
	GameObject*			m_owner;
	hgr::Mesh*			m_muzzleflash;
	float				m_muzzleflashVisible;

	GameWeapon( const GameWeapon& );
	GameWeapon& operator=( const GameWeapon& );
};


#endif // _GAMEOBJECTS_GAMEWEAPON_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
