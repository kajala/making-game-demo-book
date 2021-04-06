#ifndef _HGR_DEFAULTPIPE_H
#define _HGR_DEFAULTPIPE_H


#include <hgr/Pipe.h>
#include <lang/String.h>


namespace hgr
{


class Scene;
class Camera;


/**
 * Render scene to rendering target using specified shader technique.
 * @ingroup hgr
 */
class DefaultPipe :
	public Pipe
{
public:
	/**
	 * Inits resources needed by the pipeline.
	 * @param setup Rendering setup which is shared between all pipes.
	 * @param name Name of the shader technique to be used. Default is "Default".
	 * @param minpriority Minimum priority shader which this pipe renders.
	 * @param maxpriority Maximum priority shader which this pipe renders.
	 * @exception GraphicsException
	 */
	explicit DefaultPipe( PipeSetup* setup,
		const lang::String& name="Default",
		int minpriority=-100, int maxpriority=100 );

	/**
	 * Renders scene to specified rendering target using this pipeline.
	 * @param target Rendering target or 0 if back buffer should be used.
	 * @param context Rendering target context.
	 * @param scene Scene to be rendered.
	 * @param camera Scene camera to use in rendering.
	 */
	void	render( gr::Texture* target, gr::Context* context, Scene* scene, Camera* camera );

private:
	char	m_name[32];
	int		m_minPriority;
	int		m_maxPriority;
};


} // hgr


#endif // _HGR_DEFAULTPIPE_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
