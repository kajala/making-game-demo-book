inline const math::float3& Visual::boundBoxMin() const		
{
	assert(!isBoundWorld()); 
	return m_boundBoxMin;
}

inline const math::float3& Visual::boundBoxMax() const		
{
	assert(!isBoundWorld()); 
	return m_boundBoxMax;
}

inline const math::float3& Visual::boundBoxMinWorld() const		
{
	assert(isBoundWorld()); 
	return m_boundBoxMin;
}

inline const math::float3& Visual::boundBoxMaxWorld() const		
{
	assert(isBoundWorld()); 
	return m_boundBoxMax;
}

inline bool Visual::isBoundInfinity() const
{
	return m_boundBoxMax.x == MAX_BOUND;
}

inline bool Visual::isBoundWorld() const
{
	return 0 != (flags() & NODE_BOUNDWORLDSPACE);
}

inline void Visual::setRenderable( bool enabled )
{
	setFlag( NODE_RENDERABLE, enabled );
}

inline bool Visual::renderable() const
{
	return getFlag( NODE_RENDERABLE );
}

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
