#ifndef _HGR_GLOWPIPE_H
#define _HGR_GLOWPIPE_H


#include <hgr/Pipe.h>


namespace hgr
{


class Scene;
class Camera;


/**
 * Post-processing glow effect.
 * @ingroup hgr
 */
class GlowPipe :
	public Pipe
{
public:
	/**
	 * Inits resources needed by glow effect.
	 * @param setup Rendering setup which is shared between all pipes.
	 * @exception GraphicsException
	 */
	explicit GlowPipe( PipeSetup* setup );

	/**
	 * Renders (only) glow to specified rendering target.
	 * @param target Rendering target or 0 if back buffer should be used.
	 * @param context Rendering target context.
	 * @param scene Scene to be rendered.
	 * @param camera Scene camera to use in rendering.
	 */
	void	render( gr::Texture* target, gr::Context* context, Scene* scene, Camera* camera );

private:
	P(gr::Texture)				m_rtt;
	P(gr::Texture)				m_rttBlurH;
	P(gr::Texture)				m_rttBlur;
	P(gr::Texture)				m_rttNewBlur;
	P(gr::Texture)				m_rttOldBlur;
	P(gr::Shader)				m_add;
	P(gr::Shader)				m_trail;
	P(gr::Shader)				m_mulA;
	P(gr::Shader)				m_blurH;
	P(gr::Shader)				m_blurV;
	P(gr::Primitive)			m_overlay;
};


} // hgr


#endif // _HGR_GLOWPIPE_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
