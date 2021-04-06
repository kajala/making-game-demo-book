#ifndef _HGR_TRANSFORMANIMATION_H
#define _HGR_TRANSFORMANIMATION_H


#include <hgr/KeyframeSequence.h>
#include <lang/Object.h>
#include <stdint.h>


namespace lang {
	class String;}

namespace math {
	class float3;
	class float3x3;
	class float3x4;
	class quaternion;}


namespace hgr
{


/**
 * Key frame based affine transformation animation container.
 * @ingroup hgr
 */
class TransformAnimation :
	public lang::Object
{
public:
	/** 
	 * Animation playback end behaviour.
	 */
	enum BehaviourType
	{
		/** Animation loops to start after reaching end. */
		BEHAVIOUR_REPEAT,
		/** Animation changes direction at the ends. */
		BEHAVIOUR_OSCILLATE,
		/** Animation resets to start frame at the end. */
		BEHAVIOUR_RESET,
		/** Animation stops at the last frame. */
		BEHAVIOUR_STOP,
		/** Number of supported behaviours. */
		BEHAVIOUR_COUNT
	};

	/**
	 * Creates a transform animation with space for specified animation channels.
	 * @param endbehaviour Animation playback end behaviour.
	 * @param poskeys Number of position key frames.
	 * @param rotkeys Number of rotation key frames.
	 * @param sclkeys Number of scale key frames.
	 * @param poskeyrate Sample rate of position keys.
	 * @param rotkeyrate Sample rate of rotation keys.
	 * @param sclkeyrate Sample rate of scale keys.
	 */
	TransformAnimation( BehaviourType endbehaviour,
		int poskeys, int rotkeys, int sclkeys,
		int poskeyrate, int rotkeyrate, int sclkeyrate );

	/**
	 * Creates a transform animation with specified animation channels.
	 * @param endbehaviour Animation playback end behaviour.
	 * @param poskeys Position key frames.
	 * @param rotkeys Rotation key frames.
	 * @param sclkeys Scale key frames.
	 * @param poskeyrate Sample rate of position keys.
	 * @param rotkeyrate Sample rate of rotation keys.
	 * @param sclkeyrate Sample rate of scale keys.
	 */
	TransformAnimation( BehaviourType endbehaviour,
		KeyframeSequence* poskeys, KeyframeSequence* rotkeys, KeyframeSequence* sclkeys,
		int poskeyrate, int rotkeyrate, int sclkeyrate );

	///
	~TransformAnimation();

	/** 
	 * Evaluates transform at specified time. 
	 */
	void	eval( float time, math::float3* pos, math::quaternion* rot, math::float3* scl );

	/** 
	 * Evaluates transform at specified time. 
	 */
	void	eval( float time, math::float3x4* tm );

	/**
	 * Sets animation playback end behaviour type.
	 */
	void	setEndBehaviour( BehaviourType endbehaviour );

	/**
	 * Sets position key at specified index.
	 */
	void	setPositionKey( int i, const math::float3& v );

	/**
	 * Sets rotation key at specified index.
	 */
	void	setRotationKey( int i, const math::quaternion& v );

	/**
	 * Sets scale key at specified index.
	 */
	void	setScaleKey( int i, const math::float3& v );

	/**
	 * Removes redundant key frames from the animation
	 * and packs animation keys to 16-bit integer components.
	 */
	void	optimize();

	/**
	 * Rotates transform animation keys.
	 */
	void	rotate( const math::float3x3& rot );

	/**
	 * Scales transform animation (by altering scale and position data scaling factors).
	 */
	void	scale( float s );

	/**
	 * Gets position key at specified index.
	 */
	math::float3		getPositionKey( int i ) const;

	/**
	 * Gets rotation key at specified index.
	 */
	math::quaternion	getRotationKey( int i ) const;

	/**
	 * Gets scaling key at specified index.
	 */
	math::float3		getScaleKey( int i ) const;

	/** 
	 * Returns number of position keys. 
	 */
	int		positionKeys() const;

	/** 
	 * Returns number of rotation keys. 
	 */
	int		rotationKeys() const;
	
	/** 
	 * Returns number of scale keys. 
	 */
	int		scaleKeys() const;

	/** 
	 * Returns sample rate of position keys. 
	 */
	int		positionKeyRate() const;

	/** 
	 * Returns sample rate of rotation keys. 
	 */
	int		rotationKeyRate() const;
	
	/** 
	 * Returns sample rate of scale keys. 
	 */
	int		scaleKeyRate() const;

	/**
	 * Returns animation playback end behaviour.
	 */
	BehaviourType	endBehaviour() const;

	/**
	 * Returns linear velocity at specified time in the animation,
	 * calculated from two consequtive position keys.
	 */
	math::float3	getLinearVelocity( float time ) const;

	/**
	 * Returns distance moved between two position keys in the animation.
	 */
	math::float3	getPositionDelta( float time0, float time1 ) const;

	/**
	 * Returns position keyframe sequence if any.
	 */
	KeyframeSequence*	positionKeyframeSequence() const	{return m_pos;}

	/**
	 * Returns rotation keyframe sequence if any.
	 */
	KeyframeSequence*	rotationKeyframeSequence() const	{return m_rot;}

	/**
	 * Returns scale keyframe sequence if any.
	 */
	KeyframeSequence*	scaleKeyframeSequence() const		{return m_scl;}

	/**
	 * Converts end behavior string to enum.
	 * @param str Either REPEAT, OSCILLATE, RESET or STOP.
	 * @exception GraphicsException
	 */
	static BehaviourType	toBehaviour( const lang::String& str );

private:
	P(KeyframeSequence)	m_pos;
	P(KeyframeSequence)	m_rot;
	P(KeyframeSequence)	m_scl;
	uint8_t				m_poskeyrate;
	uint8_t				m_rotkeyrate;
	uint8_t				m_sclkeyrate;
	uint8_t				m_endbehaviour;

	TransformAnimation( const TransformAnimation& );
	TransformAnimation& operator=( const TransformAnimation& );
};


} // hgr


#endif // _HGR_TRANSFORMANIMATION_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
