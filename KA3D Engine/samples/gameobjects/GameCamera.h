#ifndef _GAMEOBJECTS_GAMECAMERA_H
#define _GAMEOBJECTS_GAMECAMERA_H


#include "GameRay.h"
#include "GameObject.h"
#include <hgr/Camera.h>
#include <lang/Array.h>


class GameCharacter;

namespace gr {
	class Context;}

namespace hgr {
	class Pipe;
	class PipeSetup;
	class Scene;}


class GameCamera :
	public GameObject
{
public:
	GameCamera( lua::LuaState* lua, 
		GameCharacter* target, GameLevel* level );

	~GameCamera();

	void	update( float dt );

	void	render( gr::Context* context );

	void	getObjectsToRender( GameCamera* );

	void	addObjectToRender( hgr::Node* node )	{m_nodes.add(node);}

	void	togglePass( int index );

	const math::float3&		aimPoint() const;

private:
	P(GameCharacter)	m_target;
	P(GameRay)			m_aim;
	P(hgr::Camera)		m_camera;
	P(hgr::PipeSetup)	m_pipeSetup;
	P(hgr::Pipe)		m_defaultPipe;
	P(hgr::Pipe)		m_glowPipe;
	P(hgr::Pipe)		m_spritePipe;
	float				m_time;
	float				m_dt;
	int					m_passes;

	lang::Array<P(hgr::Node)>	m_nodes;

	GameCamera( const GameCamera& );
	GameCamera& operator=( const GameCamera& );
};


#endif // _GAMEOBJECTS_GAMECAMERA_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
