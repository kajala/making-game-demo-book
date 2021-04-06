inline const math::float3x4& Camera::cachedWorldTransform() const
{
	return m_worldtm;
}

inline const math::float3x4& Camera::cachedViewTransform() const
{
	return m_viewtm;
}

inline const math::float4x4& Camera::cachedProjectionTransform() const
{
	return m_projtm;
}

inline const math::float4x4& Camera::cachedViewProjectionTransform() const
{
	return m_viewprojtm;
}

inline const math::float4x4& Camera::cachedScreenViewProjectionTransform() const
{
	return m_screenviewprojtm;
}

inline const math::float3x4& Camera::getCachedWorldTransform( Node* node ) const
{
	return m_worldTransformCache[ node->m_tmindex ];
}

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
