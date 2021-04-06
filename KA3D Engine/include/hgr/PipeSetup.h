#ifndef _HGR_PIPESETUP_H
#define _HGR_PIPESETUP_H


#include <hgr/LightSorter.h>
#include <lang/Object.h>


namespace gr {
	class Shader;
	class Context;}


namespace hgr
{


class Node;
class Visual;
class Camera;


/**
 * Setups scene ready for rendering.
 * Even if multiple pipes are used, there is only
 * one PipeSetup, which is shared between consuqutive pipes.
 * PipeSetup does all the work which is shared between
 * multiple passes the scene is rendered.
 *
 * @ingroup hgr
 */
class PipeSetup :
	public lang::Object
{
public:
	/** All Nodes in the scene, collected by setup(). */
	lang::Array<Node*>			nodes;
	/** All Visuals in the scene, collected by setup(). */
	lang::Array<Visual*>		visuals;
	/** All Lights in the scene, collected by setup(). */
	LightSorter					lights;
	/** All unique Shaders in the scene, collected by setup(). */
	lang::Array<gr::Shader*>	shaders;
	/** All unique priorities used in the shaders, collected by setup(). */
	lang::Array<int>			priorities;

	/**
	 * Stores reference to rendering context.
	 */
	explicit PipeSetup( gr::Context* context );

	~PipeSetup();

	/**
	 * Called once every frame before actual rendering of pipes.
	 *
	 * Tasks:
	 * <ol>
	   <li>Collects Nodes to 'nodes' array
	   <li>Updates cached node and camera transformations
	   <li>Collects visible Visuals to 'visuals' array
	   <li>Sorts visible Visuals array by ascending distance to camera
	   <li>Collects Lights to 'lights' array
	   <li>Collects unique used Shaders to 'shaders' array
	   </ol>
	 */
	virtual void	setup( Camera* camera );

	/**
	 * Sets the rendering technique to all shaders in 'shaders' array.
	 * Pass 0 to set default technique.
	 * @see setup
	 */
	void			setTechnique( const char* name );

	/**
	 * Returns true if setup() has been called in this frame.
	 */
	bool			valid() const;

	/**
	 * Returns rendering context used.
	 */
	gr::Context*	context() const;

	/**
	 * Collects all nodes from the hierarchy to array.
	 * Helper function used by setup().
	 */
	static void		getNodes( Node* scene, lang::Array<Node*>& nodes );

	/**
	 * Collects all lights from node array.
	 * Helper function used by setup().
	 */
	static void		getLights( const lang::Array<Node*>& nodes, LightSorter& lights );

	/**
	 * Collects unique shaders from visuals to array.
	 * Helper function used by setup().
	 */
	static void		getShaders( const lang::Array<Visual*>& visuals, lang::Array<gr::Shader*>& shaders );

	/**
	 * Collects unique priorities from shaders to array.
	 * Helper function used by setup().
	 */
	static void		getPriorities( const lang::Array<gr::Shader*>& shaders, lang::Array<int>& priorities );

private:
	/** Last frame when setup() was called. */
	int				m_frameCounter;
	P(gr::Context)	m_context;

	PipeSetup( const PipeSetup& );
	PipeSetup& operator=( const PipeSetup& );
};


} // hgr


#endif // _HGR_PIPESETUP_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
