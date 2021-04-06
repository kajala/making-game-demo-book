// Note: In actual application you should
// include individual headers for optimal
// compilation performance. This example includes
// everything just for convenience.
#include <framework/App.h>
#include <gr/all.h>
#include <hgr/all.h>
#include <lang/all.h>
#include <math/all.h>


class CharacterAnimationApp : 
	public framework::App
{
public:
	CharacterAnimationApp( gr::Context* context );
	
	void	keyDown( KeyType key );

	void	update( float dt, gr::Context* context );

private:
	enum
	{ 
		IDLE_COUNT=2
	};

	enum State
	{
		STATE_STAND,
		STATE_WALK_FORWARD,
		STATE_STEP_LEFT,
		STATE_STEP_RIGHT,
		STATE_JUMP_UP,
		STATE_JUMP_OUT,
		STATE_IDLE1,
		STATE_IDLE2,
		STATE_COUNT
	};

	enum TransitionState
	{
		TRANSITION_NONE,
		TRANSITION_BLEND,
	};

	P(hgr::PipeSetup)				m_pipeSetup;
	P(hgr::Pipe)					m_defaultPipe;
	P(hgr::Pipe)					m_glowPipe;

	P(hgr::Scene)					m_scene;
	P(hgr::Scene)					m_character;
	P(hgr::Camera)					m_camera;
	P(hgr::TransformAnimationSet)	m_anims[STATE_COUNT];
	float							m_bipHeight;

	float							m_time;
	State							m_state;
	TransitionState					m_transitionState;
	State							m_transitionTarget;
	float							m_transitionTime;
	float							m_transitionStart;
	float							m_transitionLength;
	int								m_idleAnimIndex;

	void							animateCharacter( float dt );
	void							animateCamera( float dt );
	void							render( gr::Context* context );
	State							selectIdle();
	void							rotateAnimation( hgr::TransformAnimationSet* animset, const math::float3x3& rot );
	hgr::TransformAnimationSet*		getAnimation( State bodystate ) const;
	bool							isAnimationEnd( float dt ) const;

	P(hgr::TransformAnimationSet)	loadCharacterAnimation( gr::Context* context, const lang::String& filename, hgr::TransformAnimation::BehaviourType endbehaviour=hgr::TransformAnimation::BEHAVIOUR_REPEAT );
	P(hgr::Lines)					createXZGrid( gr::Context* context, float r, float dr );

	void							evaluateTransitionTarget();

	static bool						isStep( State bodystate );
	static bool						isJump( State bodystate );
	static bool						isIdle( State bodystate );
	static bool						isRepeated( State bodystate );
	static bool						needsTimeReset( State bodystate );
	static const char*				toString( State bodystate );
};

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
