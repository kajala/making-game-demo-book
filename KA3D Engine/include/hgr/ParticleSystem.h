#ifndef _HGR_PARTICLESYSTEM_H
#define _HGR_PARTICLESYSTEM_H


#include <gr/Primitive.h>
#include <hgr/Visual.h>
#include <math/Domain.h>
#include <lang/Array.h>


namespace gr {
	class Shader;
	class Texture;}

namespace lua {
	class LuaTable;}


namespace hgr
{


class ResourceManager;


/**
 * Particle effect. Can be created directly or from Lua script.
 * 
 * Key features:
 * <ul>
 * <li>Variable number of emitters / single system
 * <li>Variable number of particles / emitter
 * <li>All properties can have different random variable value distribution
 * <li>Elastic, scaling, rotating etc. particle sprites are supported.
 * <li>Per particle transparency, color and animation texture frame are supported.
 * <li>Particles can use also user defined shaders.
 * </ul>
 *
 * Particle systems are heavily based on concept of 'domains'.
 * It is a bit abstract, but understanding it is essential
 * for effective usage of the particle systems. On the benefit
 * side once you understand it, using the particle systems
 * becomes very intuitive and orthogonal, as nearly all properties
 * of particle systems are simple defined by setting
 * appropriate value domains for each property.
 *
 * 'Domains' are source ranges for various random
 * values needed by particle systems.
 * Domains can be sources for scalar values, 
 * 2D and 3D vectors used in the simulation.
 *
 * For example by saying that some particle effect has
 * particle initial position domain set as 'Sphere with
 * radius 4' means that the initial positions of new 
 * particles are randomized inside sphere which has radius 4.
 * If in turn the particle initial velocity domain
 * is set as Point {-50,0,0} then all particles get
 * initial velocity of -50 along negative X-axis.
 *
 * With various domains it is easy to make pretty much
 * any kind of particle effect. For example rain
 * could be done by setting position box above sky,
 * and then velocity Point(0,-1,0) and Gravity=-9.8.
 * In addition, ParticleSpriteElasticity could be
 * set above 0 (say, 0.1) to stretch particle bitmap
 * along movement speed - the system is very flexible.
 *
 * @ingroup hgr
 */
class ParticleSystem :
	public hgr::Visual
{
public:
	/**
	 * Kill mode describes how max count limit is maintained.
	 */
	enum KillType
	{
		/** No items are killed when max limit is reached. */
		KILL_NONE,
		/** Oldest item is killed when max limit is reached. */
		KILL_OLDEST,
		/** Random item is killed when max limit is reached. */
		KILL_RANDOM,
		/** Number of kill modes. */
		KILL_COUNT
	};

	/** 
	 * View mode of particle sprites.
	 * View mode defines how particles are rotated when camera is tilted.
	 */
	enum ViewType
	{
		/**
		 * Particles are facing to camera and bitmaps Y-axis points up in camera space.
		 */
		VIEW_CAMERAUP,
		/**
		 * Particles are facing to camera and bitmaps Y-axis points up in world space.
		 */
		VIEW_WORLDUP,
		/**
		 * Number of view modes.
		 */
		VIEW_COUNT
	};

	/** 
	 * Animation type of particle texture.
	 * Defines how texture frames change over life time of particle effect.
	 */
	enum AnimationType
	{
		/**
		 * Animation frames are used as normal animation sequence.
		 * Framerate defines how often frame is changed to next one.
		 */
		ANIM_LOOP,
		/**
		 * Animation frames are used by particle life time.
		 * Framerate does not affect any way.
		 */
		ANIM_LIFE,
		/**
		 * Animation frames are used randomly.
		 * Framerate defines how often frame is randomized.
		 */
		ANIM_RANDOM,
		/**
		 * Number of animation types.
		 */
		ANIM_COUNT
	};

	/**
	 * Description of a particle system.
	 * Description objects are shared between all instances
	 * of the same particle effect.
	 * Normally this class is not used directly by the user as
	 * particle systems are loaded from scripts.
	 */
	class Description :
		public lang::Object
	{
	public:
		enum
		{
			/** Absolute maximum limit for number of particles. */ 
			MAX_PARTICLES_LIMIT = 65535/6 
		};

		/** Maximum number of simultaneous emissions. */
		int					systemMaxEmissions;

		/** Value domain for number of new emissions / second random (scalar) variable. */
		math::Domain		systemRate;

		/** Value domain for system stop time random (scalar) variable. */
		math::Domain		systemStopTime;

		/** Value domain for system life time random (scalar) variable. */
		math::Domain		systemLifeTime;

		/** Maximum number of particles. See MAX_PARTICLES_LIMIT. */
		int					emissionMaxParticles;

		/** Value domain for emission rate random (scalar) variable. */
		math::Domain		emissionRate;

		/** Value domain for emission stop time random (scalar) variable. */
		math::Domain		emissionStopTime;

		/** Value domain for emission life time random (scalar) variable. */
		math::Domain		emissionLifeTime;

		/** Value domain for emission pivot random (3-vector) variable. */
		math::Domain		emissionPosition;

		/** Value domain for particle life time random (scalar) variable. */
		math::Domain		particleLifeTime;
		
		/** Value domain for particle start velocity random (3-vector) variable. */
		math::Domain		particleStartVelocity;
		
		/** Value domain for particle start position random (3-vector) variable. */
		math::Domain		particleStartPosition;
		
		/** Value domain for particle start size random (scalar) variable. */
		math::Domain		particleStartSize;
		
		/** Value domain for particle end size random (scalar) variable. */
		math::Domain		particleEndSize;
		
		/** Value domain for particle start transparency random (scalar) variable. 1 is opaque. */
		math::Domain		particleStartAlpha;
		
		/** Value domain for particle end transparency random (scalar) variable. 1 is opaque. */
		math::Domain		particleEndAlpha;
		
		/** Value domain for particle start color random (3-vector) variable. (1,1,1) is white. */
		math::Domain		particleStartColor;
		
		/** Value domain for particle end color random (3-vector) variable. (1,1,1) is white. */
		math::Domain		particleEndColor;
		
		/** 
		 * Value domain for particle elasticity random (scalar) variable. 
 		 * Good values 0-0.2, but out-of-range values can create interesting effects. 
		 */
		math::Domain		particleSpriteElasticity;
		
		/** Value domain for particle sprite start rotation (radians) random (scalar) variable. */
		math::Domain		particleSpriteRotation;
		
		/** Value domain for particle sprite start rotation speed (rad/s) random (scalar) variable. */
		math::Domain		particleSpriteStartRotationSpeed;
		
		/** Value domain for particle sprite end rotation speed (rad/s) random (scalar) variable. */
		math::Domain		particleSpriteEndRotationSpeed;
		
		/** Gravity in world space Y-axis direction, m/(s*s). */
		float				gravity;
		
		/** Wind in world space, m/s. */
		math::float3		wind;
		
		/** Texture animation frame change rate, changes/second. */
		float				textureFrameRate;
				
		/** Number of texture frames embedded to the texture bitmap in NxN grid. Must be square of integer. */
		int					textureFrames;
		
		/** Texture (animation) for the particle effect. */
		P(gr::Texture)		texture;
		
		/** Sprite shader used to render particles. */
		P(gr::Shader)		shader;
		
		/** Defines how texture frames change over life time of particle effect. */
		AnimationType		textureAnimation;
		
		/** View mode defines how particles are rotated when camera is tilted. */
		ViewType			textureView;
		
		/** Kill mode describes how max particle limit is maintained. */
		KillType			emissionLimitKill;

		/** Kill mode describes how max emission limit is maintained. */
		KillType			systemLimitKill;

		/** If true then particles are updated even though they are not visible. */
		bool				particleUpdateAlways;

		Description();

	private:
		friend class ParticleSystem;

		// intermediate values cached from the description
		float				textureFrameUV;
		lang::Array<float>	uvcache;
	};

	/**
	 * Loads a particle system from a file.
	 * @param context Rendering context to be used while loading.
	 * @param filename Scene file name relative to current working directory.
	 * @param res Resource manager to load textures and shaders from. 0 for default.
	 * @param texturepath Texture path relative to current working directory.
	 * @param shaderpath Shader path relative to current working directory.
	 * @exception IOException
	 * @exception LuaException
	 * @exception GraphicsException
	 */
	ParticleSystem( gr::Context* context, const lang::String& filename, ResourceManager* res=0,
		const lang::String& texturepath="", const lang::String& shaderpath="" );

	/**
	 * Makes particle system using user provided description object.
	 */
	explicit ParticleSystem( Description* desc );

	/**
	 * Creates copy of particle system. Shares non-instance-specific data.
	 */
	explicit ParticleSystem( const ParticleSystem& other );

	~ParticleSystem();

	/**
	 * Loads particle system from file.
	 * @exception IOException
	 * @exception LuaException
	 * @exception GraphicsException
	 */
	void	load( gr::Context* context, const lang::String& filename, ResourceManager* res,
				const lang::String& texturepath="", const lang::String& shaderpath="" );

	/**
	 * Updates particle animation.
	 * @param dt Time interval to update in seconds.
	 */
	void	update( float dt );

	/** 
	 * Renders this particle system to the active device.
	 * @param context Rendering context.
	 * @param camera Active camera.
	 * @param priority Shader priority level which is to be rendered.
	 */
	void	render( gr::Context* context, Camera* camera, int priority );

	/**
	 * Returns array of shaders used by this visual.
	 * NOTE: Implementation should not clear the list of returned shaders
	 * before adding new ones, since shadesr from multiple Visuals might be 
	 * collected at the same time.
	 */
	void	getShaders( lang::Array<gr::Shader*>& shaders );

	/**
	 * Sets rendering technique of the shaders used by this mesh.
	 */
	void	setTechnique( const char* name );

	/**
	 * Resets particle system to its initial state.
	 */
	void	reset();

	/**
	 * Computes bounding volume of the visual in model space.
	 */
	void	computeBound();

	/**
	 * Sets time (seconds) to delay particle system instance simulation start.
	 * Default delay is 0.
	 * @param time Delay in seconds.
	 */
	void	setDelay( float time );

	/**
	 * Sets velocity of parent node in world space to be taken into account while
	 * computing sprite elasticity effect.
	 */
	void	setParentVelocity( const math::float3& vel );

	/**
	 * Returns description used by this particle system.
	 */
	Description*	description()					{return m_desc;}

	/**
	 * Returns total number of active particles.
	 */
	int				particles() const;

	/**
	 * Returns description used by this particle system.
	 */
	const Description*		description() const		{return m_desc;}

	/**
	 * Converts string to ViewType.
	 * @return VIEW_COUNT if not found.
	 */ 
	static ViewType			toViewType( const char* sz );

	/**
	 * Converts string to AnimationType.
	 * @return ANIMATION_COUNT if not found.
	 */ 
	static AnimationType	toAnimationType( const char* sz );

	/**
	 * Converts string to KillType.
	 * @return KILL_COUNT if not found.
	 */ 
	static KillType			toKillType( const char* sz );

private:
	template <class T> class Integral
	{
	public:
		T		v;
		T		dv;

		void set( math::Domain& startdomain, math::Domain& enddomain, float invlife )
		{
			startdomain.getRandom( &v );
			enddomain.getRandom( &dv );
			dv -= v;
			dv *= invlife;
		}

		void integrate( float dt )
		{
			v += dv*dt;
		}
	};

	class Particle
	{
	public:
		float									time;
		float									life;
		float									rot;
		float									elasticity;
		ParticleSystem::Integral<float>			size;
		ParticleSystem::Integral<math::float3>	color;
		ParticleSystem::Integral<float>			alpha;
		ParticleSystem::Integral<float>			rotspeed;
		int										frame;
		math::float3							velocity;
		math::float3							position;
	};

	class Emission
	{
	public:
		float					time;
		float					stop;
		float					life;
		float					newParticles;
		math::float3			position;
		lang::Array<Particle>	particles;
	};

	P(Description)				m_desc;
	lang::Array<Emission>		m_emissions;
	float						m_time;
	float						m_timeSinceRender;
	float						m_newEmissions;
	float						m_systemStopTime;
	float						m_systemLifeTime;
	float						m_delay;
	math::float3				m_parentVelocity;
	P(gr::Primitive)			m_prim;

	void		restart();
	void		refreshCachedValues();
	static void	getDomain( const lang::String& particlename, lua::LuaTable& tab, const lang::String& domainname, math::Domain* domain );
	static int	log2i( int x );

	template <class T> static void	killOld( lang::Array<T>& array );
	template <class T> static T*	getNew( lang::Array<T>& array, int limit, KillType killtype );

	ParticleSystem& operator=( const ParticleSystem& );
};


} // hgr


#endif // _HGR_PARTICLESYSTEM_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
