#include "GameDynamicObject.h"
#include "GameLevel.h"
#include "GameCamera.h"
#include "GameProjectile.h"
#include <hgr/Mesh.h>
#include <hgr/ResourceManager.h>
#include <lang/Math.h>
#include <lang/Debug.h>
#include <lang/Exception.h>
#include <math/toString.h>
#include <math/InterpolationUtil.h>
#include <config.h>


using namespace gr;
using namespace hgr;
using namespace lua;
using namespace ode;
using namespace lang;
using namespace math;


GameDynamicObject::GameDynamicObject( LuaState* luastate, GameLevel* level,
	Mesh* mesh, ODEObject::GeomType geomtype, ODEObject::MassType masstype, float mass ) :
	GameObject( luastate, level ),
	m_time( 0 )
{
	m_root = mesh;
	m_mesh = mesh;
	mesh->unlink();

	setRigidBodyFromMesh( mesh, geomtype, masstype, mass, TYPE_DYNAMIC,
		TYPE_CHARACTER+TYPE_LEVEL+TYPE_DYNAMIC );
	setAutoDisable( true );
}

GameDynamicObject::~GameDynamicObject()
{
}

void GameDynamicObject::update( float dt )
{
	m_time += dt;
}

void GameDynamicObject::getObjectsToRender( GameCamera* camera )
{
	assert( m_mesh != 0 );

	setMeshTransformFromRigidBody();
	camera->addObjectToRender( m_mesh );
}

int	GameDynamicObject::collided( GameObject* obj, dContact* contacts, int numc )
{
	if ( obj && obj->isType(TYPE_PROJECTILE) )
	{
		GameProjectile* proj = static_cast<GameProjectile*>( obj );
		
		float3 point = float3(contacts[0].geom.pos[0],contacts[0].geom.pos[1],contacts[0].geom.pos[2]);
		float3 normal = float3(contacts[0].geom.normal[0],contacts[0].geom.normal[1],contacts[0].geom.normal[2]);
		bool objnormal = contacts[0].geom.g1 == geom();

		float impulse = proj->impulse();
		if ( !objnormal )
			impulse = -impulse;

		addImpulseAt( point, normal*impulse, 1.f/100.f );
	}
	return numc;
}
