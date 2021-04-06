#ifndef _GAMEOBJECTS_GAMELEVEL_H
#define _GAMEOBJECTS_GAMELEVEL_H


#include "GameObject.h"
#include "GameProjectile.h"
#include <hgr/Scene.h>
#include <hgr/Light.h>
#include <hgr/Lines.h>
#include <hgr/LightSorter.h>
#include <lua/LuaObject.h>
#include <ode/ODEWorld.h>
#include <ode/ODEObject.h>
#include <ode/ODECollisionInterface.h>
#include <lang/Array.h>
#include <math/float3.h>


class GameObject;

namespace gr {
	class Context;}

namespace hgr {
	class ResourceManager;
	class Light;}

namespace Opcode {
	class Model;
	class RayCollider;
	class MeshInterface;
	class CollisionFaces;}


class GameLevel :
	public lua::LuaObject,
	public ode::ODECollisionInterface
{
public:
	GameLevel( gr::Context* context, lua::LuaState* lua, hgr::ResourceManager* res );
	~GameLevel();

	void				update( float dt );

	GameProjectile*		addProjectile( const lang::String& name, 
							const math::float3x4& tm, GameObject* source );

	void				addObject( GameObject* obj );

	void				removeObject( GameObject* obj );

	GameObject*			getObject( int i );

	hgr::LightSorter*	lightSorter();

	void				drawSimulationObjects();

	hgr::Scene*			getSceneToRender( GameCamera* camera, float time, float dt );

	void				removeLines();

	void				blendLights( const math::float3& pos, hgr::Light* lt );

	bool				sampleLightmap( const math::float3& pos, const math::float3& delta, math::float3* color );

	void				serialize( io::DataInputStream* in, io::DataOutputStream* out );

	int					objects() const;

	lang::String		name() const;

	dWorldID			world() const;

	dSpaceID			space() const;

	dGeomID				geom() const;

	int					enabledBodies() const;

	math::float3x4		getStartpoint( const lang::String& name ) const;

	hgr::Lines*			lines() const					{return m_lines;}
	
	gr::Context*		context() const					{return m_context;}

	hgr::ResourceManager*	resourceManager() const;

private:
	class CollisionTriangle
	{
	public:
		int					indices[3];
	};

	class VisualTriangle
	{
	public:
		int					indices[3];
		P(gr::Primitive)	prim;
		int					triangle; // index to the indices, i.e. triangle index * 3
	};

	// visuals
	P(gr::Context)					m_context;
	P(hgr::ResourceManager)			m_resourceManager;
	P(hgr::Scene)					m_scene;
	lang::Array<P(GameObject)>		m_objects;
	hgr::LightSorter				m_lightSorter;
	P(hgr::Lines)					m_lines;

	// physics
	ode::ODEWorld					m_world;
	ode::ODEObject					m_levelObj;
	lang::Array<math::float3>		m_collisionVertices;
	lang::Array<CollisionTriangle>	m_collisionTriangles;

	// lightmap raytracing
	lang::Array<math::float3>		m_visualVertices;
	lang::Array<VisualTriangle>		m_visualTriangles;
	Opcode::CollisionFaces*			m_collisionFaceBuffer;
	Opcode::RayCollider*			m_raycollider;
	Opcode::Model*					m_lightmapCollisionModel;
	Opcode::MeshInterface*			m_lightmapCollisionMeshI;

	void	loadLevel( const lang::String& filename );
	void	createDebugXZGridLevel();
	void	setWorldParams();
	void	validateLevel();
	int		checkCollisions( dGeomID o1, dGeomID o2, dContact* contacts, int maxcontacts );

	GameLevel( const GameLevel& );
	GameLevel& operator=( const GameLevel& );
};


#endif // _GAMEOBJECTS_GAMELEVEL_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
