#include <hgr/DefaultResourceManager.h>
#include <io/PathName.h>
#include <gr/Context.h>
#include <hgr/Globals.h>
#include <lang/String.h>
#include <config.h>


using namespace gr;
using namespace io;
using namespace lang;


namespace hgr
{


DefaultResourceManager::DefaultResourceManager( Context* context ) :
	m_context( context )
{
}

DefaultResourceManager::~DefaultResourceManager()
{
}

Shader* DefaultResourceManager::getShader( const String& name, int flags )
{
	return m_context->createShader( name, flags );
}

Texture* DefaultResourceManager::getTexture( const String& filename )
{
	Texture* obj = m_textures[filename];
	if ( obj == 0 )
		return m_textures[filename] = m_context->createTexture( filename );
	else
		return obj;
}

CubeTexture* DefaultResourceManager::getCubeTexture( const String& filename )
{
	CubeTexture* obj = m_cubeTextures[filename];
	if ( obj == 0 )
		return m_cubeTextures[filename] = m_context->createCubeTexture( filename );
	else
		return obj;
}

ParticleSystem* DefaultResourceManager::getParticleSystem( const String& filename,
	const String& texturepath, const String& shaderpath )
{
	String basename = PathName(filename).basename();

	ParticleSystem* obj = m_particles[basename];
	if ( obj == 0 )
		return m_particles[basename] = new ParticleSystem( m_context, filename, this, texturepath, shaderpath );
	else
		return new ParticleSystem( *obj );
}

ResourceManager* DefaultResourceManager::get( Context* context )
{
	ResourceManager* res = Globals::get().resourceManager;
	if ( !res )
		Globals::get().resourceManager = res = new DefaultResourceManager( context );
	assert( res != 0 );
	return res;
}

void DefaultResourceManager::set( ResourceManager* res )
{
	Globals::get().resourceManager = res;
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
