#ifndef _HGR_DEFAULTRESOURCEMANAGER_H
#define _HGR_DEFAULTRESOURCEMANAGER_H


#include <gr/Texture.h>
#include <gr/CubeTexture.h>
#include <hgr/ParticleSystem.h>
#include <hgr/ResourceManager.h>
#include <lang/Hashtable.h>


namespace gr {
	class Context;
	class Texture;}

namespace lang {
	class String;}


namespace hgr
{


class ParticleSystem;


/**
 * Default rendering resource manager.
 *
 * Manages resources in following way:
 * <ul>
 * <li>Shares texture by matching pathname (i.e. C:/mydata/tex.bmp != C:/tex.bmp)
 * <li>Shares particles by matching basename (i.e. C:/flare.prt == C:/data/flare.prt)
 * <li>Shaders are not shared (i.e. C:/my.fx != C:/my.fx)
 * </ul>
 *
 * @ingroup hgr
 */
class DefaultResourceManager :
	public ResourceManager
{
public:
	/**
	 * Creates resource manager using specified rendering context.
	 */
	explicit DefaultResourceManager( gr::Context* context );

	~DefaultResourceManager();

	/**
	 * Gets shader by name.
	 * @exception Exception
	 */
	gr::Shader*			getShader( const lang::String& name, int flags );

	/** 
	 * Gets texture by filename. 
	 * @exception Exception
	 */
	gr::Texture*		getTexture( const lang::String& filename );

	/** 
	 * Gets cube texture by filename. 
	 * @exception Exception
	 */
	gr::CubeTexture*	getCubeTexture( const lang::String& filename );

	/** 
	 * Gets particle system by filename. 
	 * @exception Exception
	 */
	ParticleSystem*		getParticleSystem( const lang::String& filename,
							const lang::String& texturepath,
							const lang::String& shaderpath );

	/**
	 * Returns default resource manager.
	 */
	static ResourceManager* get( gr::Context* context );

	/**
	 * Sets user-defined default resource manager.
	 */
	static void				set( ResourceManager* res );

private:
	P(gr::Context)											m_context;
	lang::Hashtable<lang::String,P(gr::Texture)>			m_textures;
	lang::Hashtable<lang::String,P(gr::CubeTexture)>		m_cubeTextures;
	lang::Hashtable<lang::String,P(hgr::ParticleSystem)>	m_particles;

	DefaultResourceManager( const DefaultResourceManager& );
	DefaultResourceManager& operator=( const DefaultResourceManager& );
};


} // hgr


#endif // _HGR_DEFAULTRESOURCEMANAGER_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
