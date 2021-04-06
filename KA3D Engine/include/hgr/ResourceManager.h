#ifndef _HGR_RESOURCEMANAGER_H
#define _HGR_RESOURCEMANAGER_H


#include <lang/Object.h>
#include <lang/String.h>


namespace gr {
	class Shader;
	class Context;
	class Texture;
	class CubeTexture;}

namespace lang {
	class String;}


namespace hgr
{


class ParticleSystem;


/**
 * Abstract base class for managing and sharing rendering resources.
 *
 * @ingroup hgr
 */
class ResourceManager :
	public lang::Object
{
public:
	/**
	 * Gets shader by name.
	 * @param name (Base) name of the shader.
	 * @param flags Shader compilation flags. See Shader::Flags.
	 * @exception Exception
	 */
	virtual gr::Shader*			getShader( const lang::String& name, int flags ) = 0;

	/** 
	 * Gets texture by filename. 
	 * @param name Name of the texture.
	 * @exception Exception
	 */
	virtual gr::Texture*		getTexture( const lang::String& filename ) = 0;

	/** 
	 * Gets cube texture by filename. 
	 * @param name Name of the texture.
	 * @exception Exception
	 */
	virtual gr::CubeTexture*	getCubeTexture( const lang::String& filename ) = 0;

	/** 
	 * Gets particle system by filename. 
	 * @param name Name of the particle system.
	 * @exception Exception
	 */
	virtual ParticleSystem*		getParticleSystem( const lang::String& filename,
									const lang::String& texturepath="",
									const lang::String& shaderpath="" ) = 0;
};


} // hgr


#endif // _HGR_RESOURCEMANAGER_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
