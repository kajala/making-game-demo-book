#include <hgr/ParticleSystem.h>
#include <gr/Shader.h>
#include <gr/Context.h>
#include <gr/Texture.h>
#include <io/PathName.h>
#include <io/IOException.h>
#include <io/FileInputStream.h>
#include <hgr/Camera.h>
#include <hgr/DefaultResourceManager.h>
#include <lua/LuaState.h>
#include <lua/LuaObject.h>
#include <lang/Math.h>
#include <lang/Debug.h>
#include <lang/Float.h>
#include <lang/Profile.h>
#include <lang/algorithm/swap.h>
#include <math/float.h>
#include <math/float2.h>
#include <math/toString.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include <config.h>


using namespace gr;
using namespace io;
using namespace lua;
using namespace lang;
using namespace math;


namespace hgr
{


static const char* const VIEW_NAMES[] =
{
	"CAMERAUP",
	"WORLDUP"
};

static const char* const ANIM_NAMES[] =
{
	"LOOP",
	"LIFE",
	"RANDOM"
};

static const char* const KILL_NAMES[] =
{
	"NONE",
	"OLDEST",
	"RANDOM"
};


ParticleSystem::Description::Description() :
	systemMaxEmissions(0),
	emissionMaxParticles(0),
	gravity(0),
	wind(0,0,0),
	textureFrameRate(0),
	textureFrames(0),
	texture(0),
	shader(0),
	textureAnimation(ANIM_COUNT),
	textureView(VIEW_COUNT),
	emissionLimitKill(KILL_NONE),
	particleUpdateAlways(false),
	textureFrameUV(0),
	uvcache()
{
}


ParticleSystem::ParticleSystem( Context* context, const String& filename, ResourceManager* res,
	const String& texturepath, const String& shaderpath ) :
	m_desc( new Description ),
	m_emissions(),
	m_time( 0.f ),
	m_timeSinceRender( 0.f ),
	m_newEmissions( 0.f ),
	m_systemStopTime( 0.f ),
	m_systemLifeTime( 0.f ),
	m_delay( 0.f ),
	m_parentVelocity( 0, 0, 0 )
{
	load( context, filename, res, texturepath, shaderpath );
}

ParticleSystem::ParticleSystem( const ParticleSystem& other ) :
	Visual( other ),
	m_desc( other.m_desc ),
	m_emissions( other.m_emissions ),
	m_time( other.m_time ),
	m_timeSinceRender( other.m_timeSinceRender ),
	m_newEmissions( other.m_newEmissions ),
	m_systemStopTime( other.m_systemStopTime ),
	m_systemLifeTime( other.m_systemLifeTime ),
	m_delay( other.m_delay ),
	m_parentVelocity( other.m_parentVelocity )
{
	reset();
}

ParticleSystem::ParticleSystem( Description* desc ) :
	m_desc( desc ),
	m_emissions(),
	m_time( 0.f ),
	m_timeSinceRender( 0.f ),
	m_newEmissions( 0.f ),
	m_systemStopTime( 0.f ),
	m_systemLifeTime( 0.f ),
	m_delay( 0.f ),
	m_parentVelocity( 0, 0, 0 )
{
	reset();
}

ParticleSystem::~ParticleSystem()
{
}

void ParticleSystem::load( Context* context, const String& filename, ResourceManager* res,
	const String& texturepath, const String& shaderpath )
{
	setName( filename );

	// make sure we have some resource manager
	if ( !res )
		res = DefaultResourceManager::get( context );

	// compile script
	LuaState lua;
	LuaObject tab( &lua );
	FileInputStream in( filename );
	lua.compile( &in, in.available(), filename, &tab );

	// directories to read data from
	// (char arrays used as memory usage optimization)
	PathName parentpath = PathName(filename).parent();

	char shaderpathsz[PathName::MAXLEN];
	if ( shaderpath != "" )
		shaderpath.get( shaderpathsz, sizeof(shaderpathsz) );
	else
		String::cpy( shaderpathsz, sizeof(shaderpathsz), parentpath.toString() );
	
	char texturepathsz[PathName::MAXLEN];
	if ( texturepath != "" )
		texturepath.get( texturepathsz, sizeof(texturepathsz) );
	else
		String::cpy( texturepathsz, sizeof(texturepathsz), parentpath.toString() );

	// setup max number of emissions
	m_desc->systemMaxEmissions = (int)tab.getNumber("SystemMaxEmissions");
	if ( m_desc->systemMaxEmissions < 1 || m_desc->systemMaxEmissions > Description::MAX_PARTICLES_LIMIT )
		throwError( IOException( Format("Invalid value in \"{0}\": SystemMaxEmissions={1} (out-of-range, max is {2})", filename, (int)tab.getNumber("SystemMaxEmissions"), Description::MAX_PARTICLES_LIMIT) ) );

	// setup emission removal mode
	m_desc->systemLimitKill = toKillType( tab.getString("SystemLimitKill").c_str() );
	if ( m_desc->systemLimitKill == KILL_COUNT )
		throwError( IOException( Format("Invalid value  in \"{0}\": Kill={1}", filename, tab.getString("SystemLimitKill")) ) );

	// setup max number of particles
	m_desc->emissionMaxParticles = (int)tab.getNumber("EmissionMaxParticles");
	if ( m_desc->emissionMaxParticles < 1 || m_desc->emissionMaxParticles >= Description::MAX_PARTICLES_LIMIT )
		throwError( IOException( Format("Invalid value in \"{0}\": MaxParticles={1} (out-of-range, max is {2})", filename, (int)tab.getNumber("MaxParticles"), Description::MAX_PARTICLES_LIMIT) ) );

	// setup particle removal mode
	m_desc->emissionLimitKill = toKillType( tab.getString("EmissionLimitKill").c_str() );
	if ( m_desc->emissionLimitKill == KILL_COUNT )
		throwError( IOException( Format("Invalid value  in \"{0}\": Kill={1}", filename, tab.getString("EmissionLimitKill")) ) );

	// check vertex limits
	const int maxvertcount = m_desc->systemMaxEmissions*m_desc->emissionMaxParticles*6;
	if ( maxvertcount >= 0xFFFF )
		throwError( IOException( Format("Invalid value in \"{0}\": emissions*particles/emission={1} (>limit)", filename, maxvertcount) ) );

	// setup forced update
	m_desc->particleUpdateAlways = tab.getBoolean("ParticleUpdateAlways");

	// setup source value domains
	getDomain( filename, tab, "SystemRate", &m_desc->systemRate );
	getDomain( filename, tab, "SystemStopTime", &m_desc->systemStopTime );
	getDomain( filename, tab, "SystemLifeTime", &m_desc->systemLifeTime );
	getDomain( filename, tab, "EmissionRate", &m_desc->emissionRate );
	getDomain( filename, tab, "EmissionStopTime", &m_desc->emissionStopTime );
	getDomain( filename, tab, "EmissionLifeTime", &m_desc->emissionLifeTime );
	getDomain( filename, tab, "EmissionPosition", &m_desc->emissionPosition );
	getDomain( filename, tab, "ParticleLifeTime", &m_desc->particleLifeTime );
	getDomain( filename, tab, "ParticleStartVelocity", &m_desc->particleStartVelocity );
	getDomain( filename, tab, "ParticleStartPosition", &m_desc->particleStartPosition );
	getDomain( filename, tab, "ParticleStartSize", &m_desc->particleStartSize );
	getDomain( filename, tab, "ParticleEndSize", &m_desc->particleEndSize );
	getDomain( filename, tab, "ParticleStartColor", &m_desc->particleStartColor );
	getDomain( filename, tab, "ParticleEndColor", &m_desc->particleEndColor );
	getDomain( filename, tab, "ParticleStartAlpha", &m_desc->particleStartAlpha );
	getDomain( filename, tab, "ParticleEndAlpha", &m_desc->particleEndAlpha );
	getDomain( filename, tab, "ParticleSpriteElasticity", &m_desc->particleSpriteElasticity );
	getDomain( filename, tab, "ParticleSpriteRotation", &m_desc->particleSpriteRotation );
	getDomain( filename, tab, "ParticleSpriteStartRotationSpeed", &m_desc->particleSpriteStartRotationSpeed );
	getDomain( filename, tab, "ParticleSpriteEndRotationSpeed", &m_desc->particleSpriteEndRotationSpeed );

	// check for mutually exclusive domains
	bool spriteRotationEnabled = !m_desc->particleSpriteRotation.isZero() || !m_desc->particleSpriteStartRotationSpeed.isZero();
	if ( !m_desc->particleSpriteElasticity.isZero() && spriteRotationEnabled )
		throwError( IOException( Format("Invalid combination in \"{0}\": Sprite rotation and elasticity are mutually exclusive", filename) ) );

	// setup world forces
	m_desc->gravity = tab.getNumber("Gravity");
	LuaTable vec3tab = tab.getTable("Wind");
	m_desc->wind.x = vec3tab.getNumber(1);
	m_desc->wind.y = vec3tab.getNumber(2);
	m_desc->wind.z = vec3tab.getNumber(3);

	// setup texture animation
	m_desc->textureFrameRate = tab.getNumber("TextureFramerate");
	if ( !(m_desc->textureFrameRate >= 0.f && m_desc->textureFrameRate <= 100.f) )
		throwError( IOException( Format("Invalid value in \"{0}\": TextureFramerate={1} (out-of-range 0-100)", filename, tab.getString("TextureFramerate")) ) );

	m_desc->textureFrames = (int)tab.getNumber("TextureFrames");
	if ( m_desc->textureFrames < 1 || m_desc->textureFrames != (1<<log2i(m_desc->textureFrames)) )
		throwError( IOException( Format("Invalid value in \"{0}\": TextureFrames={1} (not square of integer)", filename, tab.getString("TextureFrames")) ) );

	m_desc->texture = res->getTexture( PathName(texturepathsz,tab.getString("Texture")).toString() );
	m_desc->shader = res->getShader( PathName(shaderpathsz,tab.getString("Shader")).toString(), Shader::SHADER_DEFAULT );
	m_desc->shader->setTexture( "BASEMAP", m_desc->texture );
	
	m_desc->textureAnimation = toAnimationType( tab.getString("TextureAnimation").c_str() );
	if ( m_desc->textureAnimation == ANIM_COUNT )
		throwError( IOException( Format("Invalid value  in \"{0}\": TextureAnimation={1}", filename, tab.getString("TextureAnimation")) ) );

	m_desc->textureView = toViewType( tab.getString("TextureView").c_str() );
	if ( m_desc->textureView == VIEW_COUNT )
		throwError( IOException( Format("Invalid value  in \"{0}\": ParticleView={1}", filename, tab.getString("ParticleView")) ) );

	reset();
}

void ParticleSystem::update( float dt )
{
	PROFILE(ParticleUpdate);

	// update time
	m_time += dt;

	// delay simulation (start)
	if ( m_time < m_delay )
		return;

	// skip the update if the effect has not been visible for some time,
	// unless updates are forced, e.g. explosion or something like that.
	// note: we cannot skip update if there is no particles, since
	// effects without particles are not rendered in any case
	if ( !m_desc->particleUpdateAlways && 
		m_emissions.size() > 0 &&
		m_timeSinceRender > 1.f &&
		particles() > 0 )
	{
		return;
	}
	
	// update time since particle effect was rendered
	m_timeSinceRender += dt;

	// kill old emissions and particles
	killOld( m_emissions );
	for ( int i = 0 ; i < m_emissions.size() ; ++i )
		killOld( m_emissions[i].particles );

	// create new emissions
	if ( m_systemStopTime < 0.f || m_time-m_delay < m_systemStopTime )
	{
		m_newEmissions += m_desc->systemRate.getRandomFloat() * dt;
		m_newEmissions = clamp( m_newEmissions, 0.f, (float)m_desc->systemMaxEmissions );

		for ( ; m_newEmissions >= 1.f ; m_newEmissions -= 1.f )
		{
			// delete emissions by KillType if max limit reached
			Emission* newitem = getNew( m_emissions, m_desc->systemMaxEmissions, m_desc->systemLimitKill );

			// a new emission can be created?
			if ( newitem != 0 )
			{
				Emission& emission = *newitem;

				emission.time = 0.f;
				emission.stop = m_desc->emissionStopTime.getRandomFloat();
				emission.life = m_desc->emissionLifeTime.getRandomFloat();
				emission.newParticles = 0.f;
				emission.position = m_desc->emissionPosition.getRandomFloat3();
				emission.particles.clear();
			}
		}
	}

	// integrate forces shared between all emissions and particles
	float3 sharedforceintegral( 0.f, 0.f, 0.f );
	sharedforceintegral.y += m_desc->gravity * dt;

	// prepare values shared between all emissions and particles
	const float		frametime		= (m_desc->textureFrameRate > Float::MIN_VALUE ? 1.f / m_desc->textureFrameRate : Float::MAX_VALUE);
	const int		textureframes	= m_desc->textureFrames;
	const float		textureframesf	= (float)textureframes;

	// prepare bounding volume computation
	float3 boundminv( MAX_BOUND, MAX_BOUND, MAX_BOUND );
	float3 boundmaxv( -MAX_BOUND, -MAX_BOUND, -MAX_BOUND );

	// update emissions
	const float3x4 worldtm = worldTransform();
	for ( int k = 0 ; k < m_emissions.size() ; ++k )
	{
		Emission& emission = m_emissions[k];

		// update emission
		emission.time += dt;

		// create new particles
		if ( emission.stop < 0.f || emission.time < emission.stop )
		{
			emission.newParticles += m_desc->emissionRate.getRandomFloat() * dt;
			emission.newParticles = clamp( emission.newParticles, 0.f, (float)m_desc->emissionMaxParticles );

			for ( ; emission.newParticles >= 1.f ; emission.newParticles -= 1.f )
			{
				// delete particles by KillType if max limit reached
				Particle* newitem = getNew( emission.particles, m_desc->emissionMaxParticles, m_desc->emissionLimitKill );

				// a new emission can be created?
				if ( newitem != 0 )
				{
					Particle& particle = *newitem;

					float life = m_desc->particleLifeTime.getRandomFloat();
					assert( life >= Float::MIN_VALUE );
					float invlife = 1.f / life;

					particle.time = 0.f;
					particle.life = life;
					particle.elasticity = m_desc->particleSpriteElasticity.getRandomFloat();
					particle.rot = m_desc->particleSpriteRotation.getRandomFloat();
					particle.rot = Math::toRadians( particle.rot );

					particle.size.set( m_desc->particleStartSize, m_desc->particleEndSize, invlife );
					particle.color.set( m_desc->particleStartColor, m_desc->particleEndColor, invlife );
					particle.alpha.set( m_desc->particleStartAlpha, m_desc->particleEndAlpha, invlife );
					particle.rotspeed.set( m_desc->particleSpriteStartRotationSpeed, m_desc->particleSpriteEndRotationSpeed, invlife );
					particle.rotspeed.v = Math::toRadians( particle.rotspeed.v );
					particle.rotspeed.dv = Math::toRadians( particle.rotspeed.dv );

					particle.frame = 0;
					particle.velocity = m_parentVelocity + worldtm.rotate( m_desc->wind + m_desc->particleStartVelocity.getRandomFloat3() );
					particle.position = worldtm.transform( m_desc->particleStartPosition.getRandomFloat3() + emission.position );
				}
			}
		}

		// update particles
		for ( int i = 0 ; i < emission.particles.size() ; ++i )
		{
			Particle& particle = emission.particles[i];

			// update texture animation
			if ( fmodf(particle.time,frametime)+dt >= frametime )
			{
				switch ( m_desc->textureAnimation )
				{
				case ANIM_LOOP:
					particle.frame = (particle.frame+1) % m_desc->textureFrames;
					break;
				case ANIM_LIFE:
					particle.frame = (int)lerp( 0.f, textureframesf, particle.time/particle.life );
					break;
				case ANIM_RANDOM:
					particle.frame = rand() % textureframes;
					break;
				case ANIM_COUNT:
					assert( false );
					break;
				}
			}

			// integrate values
			particle.time += dt;
			particle.rot += particle.rotspeed.v * dt;
			particle.size.integrate( dt );
			particle.color.integrate( dt );
			particle.alpha.integrate( dt );
			particle.rotspeed.integrate( dt );

			// integrate forces
			particle.velocity += sharedforceintegral;

			// integrate velocity
			particle.position += particle.velocity * dt;

			// update bound volume
			float psize = particle.size.v;
			boundmaxv.x = Math::max( particle.position.x+psize, boundmaxv.x );
			boundmaxv.y = Math::max( particle.position.y+psize, boundmaxv.y );
			boundmaxv.z = Math::max( particle.position.z+psize, boundmaxv.z );
			boundminv.x = Math::min( particle.position.x-psize, boundminv.x );
			boundminv.y = Math::min( particle.position.y-psize, boundminv.y );
			boundminv.z = Math::min( particle.position.z-psize, boundminv.z );
		}
	}

	// finalize bounding volume computation
	setBoundBoxWorld( boundminv, boundmaxv );
}

void ParticleSystem::render( Context* context, Camera* camera, int priority )
{
	if ( priority == m_desc->shader->priority() && m_emissions.size() > 0 )
	{
		PROFILE(renderParticleSystem);

		// mark effect rendered
		m_timeSinceRender = 0.f;

		// get temporary dynamic primitive from rendering context
		const int maxvertices = m_desc->systemMaxEmissions * m_desc->emissionMaxParticles * 6;
		VertexFormat vf;
		vf.addTransformedPosition().addDiffuse().addTextureCoordinate(VertexFormat::DF_V2_32);
		//Primitive* prim = context->getDynamicPrimitive( Primitive::PRIM_TRI, vf, maxvertices, 0 );
		if ( !m_prim ) 
			m_prim = context->createPrimitive( Primitive::PRIM_TRI, vf, maxvertices, 0 );
		Primitive* prim = m_prim;

		// viewport half-width, half-height and center coords
		const float viewdx = float( (context->viewport().right()-context->viewport().left()) >> 1 );
		const float viewdy = float( (context->viewport().bottom()-context->viewport().top()) >> 1 );
		const float viewcx = float( (context->viewport().right()+context->viewport().left()) >> 1 );
		const float viewcy = float( (context->viewport().bottom()+context->viewport().top()) >> 1 );
		const float frameuv = m_desc->textureFrameUV;

		// prepare transforms
		const float3x4& viewtm = camera->cachedViewTransform();
		const float3x4& cameratm = camera->cachedWorldTransform();
		const float4x4& projtm = camera->cachedProjectionTransform();
		const float frontclip = camera->front() + 1e-5f;

		// particle up direction is camera or world?
		float worldangle = 0.f;
		if ( m_desc->textureView == VIEW_WORLDUP )
		{
			float3 camerax( cameratm(0,0), cameratm(1,0), cameratm(2,0) );
			float3 cameray( cameratm(0,1), cameratm(1,1), cameratm(2,1) );
			float3 cameraz( cameratm(0,2), cameratm(1,2), cameratm(2,2) );
			float3 worldy( 0,1,0 );
			worldy = normalize0( worldy - cameraz*dot(worldy,cameraz) );
			if ( worldy.lengthSquared() <= Float::MIN_VALUE )
				worldy = cameray;
			float cosangle = dot(worldy,cameray);
			cosangle = clamp( cosangle, -1.f, 1.f );
			worldangle = Math::acos( cosangle );
			if ( dot(worldy,camerax) < 0.f )
				worldangle = -worldangle;
		}

		// set vertex data
		{
			Primitive::Lock lk( prim, Primitive::LOCK_WRITE );

			// get data pointers
			assert( VertexFormat::DF_V4_32 == prim->vertexFormat().getDataFormat(VertexFormat::DT_POSITIONT) );
			assert( VertexFormat::DF_V4_8 == prim->vertexFormat().getDataFormat(VertexFormat::DT_DIFFUSE) );
			assert( VertexFormat::DF_V2_32 == prim->vertexFormat().getDataFormat(VertexFormat::DT_TEX0) );

			float* vpos;
			int vpospitch;
			prim->getVertexDataPtr( VertexFormat::DT_POSITIONT, reinterpret_cast<uint8_t**>(&vpos), &vpospitch );
			assert( (vpospitch&3) == 0 );
			vpospitch >>= 2;

			uint32_t* vcolor;
			int vcolorpitch;
			prim->getVertexDataPtr( VertexFormat::DT_DIFFUSE, reinterpret_cast<uint8_t**>(&vcolor), &vcolorpitch );
			assert( (vcolorpitch&3) == 0 );
			vcolorpitch >>= 2;

			float* vtexcoord;
			int vtexcoordpitch;
			prim->getVertexDataPtr( VertexFormat::DT_TEX0, reinterpret_cast<uint8_t**>(&vtexcoord), &vtexcoordpitch );
			assert( (vtexcoordpitch&3) == 0 );
			vtexcoordpitch >>= 2;
			const float* const uvcache = m_desc->uvcache.begin();

			// prepare sprite quads
			int vi = 0;
			for ( int k = 0 ; k < m_emissions.size() ; ++k )
			{
				Emission& emission = m_emissions[k];
				Array<Particle>& particles = emission.particles;

				for ( int i = 0 ; i < particles.size() ; ++i )
				{
					Particle& particle = particles[i];

					// particle position in view space
					float3 viewpos;
					viewtm.transform( particle.position, &viewpos );
					if ( viewpos.z < frontclip )
						continue;

					// particle size in screen space
					float particleradius = particle.size.v*.5f;
					float4 projsize = projtm * float4( particleradius, 0.f, viewpos.z, 1.f );
					if ( projsize.w <= 1e-9f ) // ignore particles behind screen
						continue;
					float rhw = 1.f / projsize.w;
					float screenradius = projsize.x * rhw * viewdx;
					float2 dx( screenradius, 0.f );
					float2 dy( 0, screenradius );

					// particle position in screen space
					float4 projpoint = projtm * float4( viewpos.x, viewpos.y, viewpos.z, 1.f );
					projpoint *= rhw;
					projpoint.x = viewcx + projpoint.x * viewdx;
					projpoint.y = viewcy - projpoint.y * viewdy;
					float2 screenpoint( projpoint.x, projpoint.y );

					// particle rotation / elasticity are mutually exclusive
					float spriteangle = worldangle + particle.rot;
					if ( particle.elasticity == 0.f && spriteangle != 0.f )
					{
						float c = Math::cos( spriteangle );
						float s = Math::sin( spriteangle );
						dx = float2( dx.x*c - dx.y*s, dx.x*s + dx.y*c );
						dy = float2( dy.x*c - dy.y*s, dy.x*s + dy.y*c );
					}
					else if ( particle.elasticity != 0.f )
					{
						// camera space points
						float3 viewp[4];
						float3 vel = viewtm.rotate( particle.velocity );

						float3 deltax = vel * particle.elasticity;
						float3 deltay( -deltax.y, deltax.x, 0.f );
						deltay = normalize0(deltay) * particleradius;
						viewp[0] = viewpos - deltax + deltay;
						viewp[1] = viewpos + deltax + deltay;
						viewp[2] = viewpos + deltax - deltay;
						viewp[3] = viewpos - deltax - deltay;

						// screen space points
						bool skip = false;
						float4 screenp[4];
						for ( int n = 0 ; n < 4 ; ++n )
						{
							if ( viewp[n].z < frontclip )
							{
								skip = true;
								break;
							}

							screenp[n] = projtm * float4(viewp[n].x,viewp[n].y,viewp[n].z,1.f);
							
							float hw = screenp[n].w;
							if ( hw <= Float::MIN_VALUE )
							{
								skip = true;
								break;
							}
							float rhw = 1.f / hw;

							screenp[n] *= rhw;
							screenp[n].x = viewcx + screenp[n].x*viewdx;
							screenp[n].y = viewcy - screenp[n].y*viewdy;
						}
						if ( skip )
							continue;

						// adjust projected point and screen space axes
						dx = float2( screenp[2].x-screenp[1].x, screenp[2].y-screenp[1].y )*.5f;
						dy = float2( screenp[1].x-screenp[0].x, screenp[1].y-screenp[0].y )*.5f;
						screenpoint = float2( (screenp[0].x+screenp[2].x)*.5f, (screenp[0].y+screenp[2].y)*.5f );
					}

					// screen rectangle corner positions
					float2 d0 = dx + dy;
					float2 d1 = dx - dy;
					float2 d2 = dx + dy;
					float2 d3 = dy - dx;
					float2 p0 = screenpoint - d0;
					float2 p1 = screenpoint + d1;
					float2 p2 = screenpoint + d2;
					float2 p3 = screenpoint + d3;

					// set vertex positions (as two-triangle quad)
					vpos[0] = p0.x;
					vpos[1] = p0.y;
					vpos[2] = projpoint.z;
					vpos[3] = rhw;
					vpos += vpospitch;
					vpos[0] = p1.x;
					vpos[1] = p1.y;
					vpos[2] = projpoint.z;
					vpos[3] = rhw;
					vpos += vpospitch;
					vpos[0] = p2.x;
					vpos[1] = p2.y;
					vpos[2] = projpoint.z;
					vpos[3] = rhw;
					vpos += vpospitch;
					// tri 0,2,3
					vpos[0] = p0.x;
					vpos[1] = p0.y;
					vpos[2] = projpoint.z;
					vpos[3] = rhw;
					vpos += vpospitch;
					vpos[0] = p2.x;
					vpos[1] = p2.y;
					vpos[2] = projpoint.z;
					vpos[3] = rhw;
					vpos += vpospitch;
					vpos[0] = p3.x;
					vpos[1] = p3.y;
					vpos[2] = projpoint.z;
					vpos[3] = rhw;
					vpos += vpospitch;

					// set color0
					uint32_t color = 
						uint32_t(clamp( particle.color.v.z*255.f, 0.f, 255.f )) +
						(uint32_t(clamp( particle.color.v.y*255.f, 0.f, 255.f )) << 8) +
						(uint32_t(clamp( particle.color.v.x*255.f, 0.f, 255.f )) << 16) +
						(uint32_t(clamp( particle.alpha.v*255.f, 0.f, 255.f )) << 24);
					*vcolor = color;
					vcolor += vcolorpitch;
					*vcolor = color;
					vcolor += vcolorpitch;
					*vcolor = color;
					vcolor += vcolorpitch;
					*vcolor = color;
					vcolor += vcolorpitch;
					*vcolor = color;
					vcolor += vcolorpitch;
					*vcolor = color;
					vcolor += vcolorpitch;

					// set texcoords
					const int frame = particle.frame;
					const int frame2 = frame + frame;
					assert( frame2+1 < m_desc->uvcache.size() );
					const float u0 = uvcache[frame2+0];
					const float v0 = uvcache[frame2+1];
					const float u1 = u0 + frameuv;
					const float v1 = v0 + frameuv;
					vtexcoord[0] = u0;
					vtexcoord[1] = v0;
					vtexcoord += vtexcoordpitch;
					vtexcoord[0] = u1;
					vtexcoord[1] = v0;
					vtexcoord += vtexcoordpitch;
					vtexcoord[0] = u1;
					vtexcoord[1] = v1;
					vtexcoord += vtexcoordpitch;
					vtexcoord[0] = u0;
					vtexcoord[1] = v0;
					vtexcoord += vtexcoordpitch;
					vtexcoord[0] = u1;
					vtexcoord[1] = v1;
					vtexcoord += vtexcoordpitch;
					vtexcoord[0] = u0;
					vtexcoord[1] = v1;
					vtexcoord += vtexcoordpitch;

					vi += 6;
				}
			}
			prim->setVertexRangeEnd( vi );
		}

		// render primitive with shader
		if ( prim->vertexRangeEnd() > 0 )
		{
			Shader::Begin use( m_desc->shader );
			for ( int k = 0 ; k < use.passes() ; ++k )
			{
				Shader::Pass pass( m_desc->shader, k );
				prim->render();
			}
		}
	}
}

void ParticleSystem::getShaders( Array<Shader*>& shaders )
{
	shaders.add( m_desc->shader );
}

int ParticleSystem::particles() const
{
	int count = 0;
	const Emission* end = m_emissions.end();
	for ( const Emission* it = m_emissions.begin() ; it != end ; ++it )
		count += it->particles.size();
	return count;
}

void ParticleSystem::restart()
{
	m_time = 0.f;
	m_timeSinceRender = -Float::MAX_VALUE;
	m_newEmissions = 0.f;
	m_systemStopTime = m_desc->systemStopTime.getRandomFloat();
	m_systemLifeTime = m_desc->systemLifeTime.getRandomFloat();
	m_emissions.clear();
}

ParticleSystem::ViewType ParticleSystem::toViewType( const char* sz )
{
	assert( int(sizeof(VIEW_NAMES)/sizeof(VIEW_NAMES[0])) == VIEW_COUNT );
	for ( int i = 0; i < VIEW_COUNT ; ++i )
		if ( !strcmp(VIEW_NAMES[i],sz) )
			return ViewType(i);
	return VIEW_COUNT;
}

ParticleSystem::AnimationType ParticleSystem::toAnimationType( const char* sz )
{
	assert( int(sizeof(ANIM_NAMES)/sizeof(ANIM_NAMES[0])) == ANIM_COUNT );
	for ( int i = 0; i < ANIM_COUNT ; ++i )
		if ( !strcmp(ANIM_NAMES[i],sz) )
			return AnimationType(i);
	return ANIM_COUNT;
}

ParticleSystem::KillType ParticleSystem::toKillType( const char* sz )
{
	assert( int(sizeof(KILL_NAMES)/sizeof(KILL_NAMES[0])) == KILL_COUNT );
	for ( int i = 0; i < KILL_COUNT ; ++i )
		if ( !strcmp(KILL_NAMES[i],sz) )
			return KillType(i);
	return KILL_COUNT;
}

void ParticleSystem::refreshCachedValues()
{
	assert( m_desc != 0 );

	// allocate space for emissions and particles
	m_emissions.resize( m_desc->systemMaxEmissions );
	for ( int i = 0 ; i < m_emissions.size() ; ++i )
		m_emissions[i].particles.resize( m_desc->emissionMaxParticles );
	m_emissions.clear();

	// setup texture uv cache
	const int frames = m_desc->textureFrames;
	const int framesx = frames > 1 ? log2i( frames ) : 1;
	m_desc->textureFrameUV = 1.f/float(framesx);
	m_desc->uvcache.resize( frames*2 );

	int xframe = 0;
	int yframe = 0;
	for ( int i = 0 ; i < m_desc->uvcache.size() ; i += 2 )
	{
		m_desc->uvcache[i+0] = m_desc->textureFrameUV * (float)xframe;
		m_desc->uvcache[i+1] = m_desc->textureFrameUV * (float)yframe;

		if ( ++xframe >= framesx )
		{
			xframe = 0;
			++yframe;
		}
	}
}

void ParticleSystem::getDomain( const String& particlename, lua::LuaTable& tab, const String& domainname, Domain* domain )
{
	LuaTable domaintab = tab.getTable( domainname );

	// domain type
	if ( !domaintab.isString(1) )
		throwError( IOException( Format("Invalid value in \"{0}\": Domain {1} definition table should start with domain type name", particlename, domainname) ) );
	String domaintypestr = domaintab.getString(1);
	char domaintypesz[64]; 
	domaintypestr.get( domaintypesz, sizeof(domaintypesz) );
	Domain::DomainType domaintype = Domain::toDomainType( domaintypesz );

	// check that correct number of number parameters has been given
	int domainparams = Domain::getParameterCount( domaintype );
	for ( int i = 2 ; i <= domainparams+1 ; ++i )
		if ( !domaintab.isNumber(i) )
			throwError( IOException( Format("Invalid value in \"{0}\": Domain {1} type {2} expects {3} number parameters", particlename, domainname, domaintypestr, domainparams) ) );
	if ( !domaintab.isNil(domainparams+2) )
		throwError( IOException( Format("Invalid value in \"{0}\": Domain {1} type {2} expects {3} number parameters", particlename, domainname, domaintypestr, domainparams) ) );

	// domain parameters
	switch ( domaintype )
	{
	case Domain::DOMAIN_CONSTANT:
		domain->setConstant( domaintab.getNumber(2) );
		break;

	case Domain::DOMAIN_RANGE:
		domain->setRange( domaintab.getNumber(2), domaintab.getNumber(3) );
		break;

	case Domain::DOMAIN_POINT:
		domain->setPoint( float3(domaintab.getNumber(2), domaintab.getNumber(3), domaintab.getNumber(4)) );
		break;

	case Domain::DOMAIN_SPHERE:
		domain->setSphere( float3(domaintab.getNumber(2), domaintab.getNumber(3), domaintab.getNumber(4)), domaintab.getNumber(5), domaintab.getNumber(6) );
		break;

	case Domain::DOMAIN_LINE:
		domain->setLine( float3(domaintab.getNumber(2), domaintab.getNumber(3), domaintab.getNumber(4)), float3(domaintab.getNumber(5), domaintab.getNumber(6), domaintab.getNumber(7)) );
		break;

	case Domain::DOMAIN_BOX:
		domain->setBox( float3(domaintab.getNumber(2), domaintab.getNumber(3), domaintab.getNumber(4)), float3(domaintab.getNumber(5), domaintab.getNumber(6), domaintab.getNumber(7)) );
		break;

	case Domain::DOMAIN_CYLINDER:
		domain->setCylinder( float3(domaintab.getNumber(2), domaintab.getNumber(3), domaintab.getNumber(4)), float3(domaintab.getNumber(5), domaintab.getNumber(6), domaintab.getNumber(7)), domaintab.getNumber(8), domaintab.getNumber(9) );
		break;

	case Domain::DOMAIN_DISK:
		domain->setDisk( float3(domaintab.getNumber(2), domaintab.getNumber(3), domaintab.getNumber(4)), float3(domaintab.getNumber(5), domaintab.getNumber(6), domaintab.getNumber(7)), domaintab.getNumber(8), domaintab.getNumber(9) );
		break;

	case Domain::DOMAIN_RECTANGLE:
		domain->setRectangle( float3(domaintab.getNumber(2), domaintab.getNumber(3), domaintab.getNumber(4)), float3(domaintab.getNumber(5), domaintab.getNumber(6), domaintab.getNumber(7)), float3(domaintab.getNumber(8), domaintab.getNumber(9), domaintab.getNumber(10)) );
		break;

	case Domain::DOMAIN_TRIANGLE:
		domain->setTriangle( float3(domaintab.getNumber(2), domaintab.getNumber(3), domaintab.getNumber(4)), float3(domaintab.getNumber(5), domaintab.getNumber(6), domaintab.getNumber(7)), float3(domaintab.getNumber(8), domaintab.getNumber(9), domaintab.getNumber(10)) );
		break;

	default:
		throw IOException( Format("Invalid domain type (domain was {0} and type {1}) in \"{2}\"", domainname, domaintypestr, particlename) );
	}
}

int ParticleSystem::log2i( int x )
{
	assert( x > 0 );

	int logx = 0;
	for ( ; x > 1 ; x >>= 1 )
		++logx;
	return logx;
}

void ParticleSystem::reset()
{
	refreshCachedValues();
	restart();
}

template <class T> void ParticleSystem::killOld( Array<T>& array )
{
	for ( int i = 0 ; i < array.size() ; ++i )
	{
		if ( array[i].life > 0.f && array[i].time >= array[i].life )
		{
			array[i] = array.last();
			array.resize( array.size()-1 );
			--i;
		}
	}
}

template <class T> T* ParticleSystem::getNew( Array<T>& array, int limit, KillType killtype )
{
	if ( array.size() < limit )
	{
		array.resize( array.size()+1 );
		return &array.last();
	}

	switch ( killtype )
	{
	case KILL_NONE:
		return 0;

	case KILL_OLDEST:{
		float oldage = 0.f;
		int oldix = -1;
		for ( int i = 0 ; i < array.size() ; ++i )
			if ( array[i].time > oldage )
			{
				oldage = array[i].time;
				oldix = i;
			}
		if ( oldix >= 0 )
			return &array[oldix];
		return 0;}

	case KILL_RANDOM:
		return &array[ rand() % array.size() ];

	default:
		return 0;
	}
}

void ParticleSystem::computeBound()
{
	// nothing to compute, ParticleSystem::update keeps up-to-date!
}

void ParticleSystem::setDelay( float time )
{
	assert( time >= 0.f );
	m_delay = time;
}

void ParticleSystem::setParentVelocity( const float3& vel )
{
	m_parentVelocity = vel;
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
