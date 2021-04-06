inline void GameObject::setAngularVel( const math::float3& vel )
{
	m_obj.setAngularVel( vel );
}

inline void GameObject::setLinearVel( const math::float3& vel )
{
	m_obj.setLinearVel( vel );
}

inline math::float3 GameObject::linearVel() const
{
	math::float3 v;
	m_obj.getLinearVel( &v );
	return v;
}

inline math::float3 GameObject::angularVel() const
{
	math::float3 v;
	m_obj.getAngularVel( &v );
	return v;
}

inline void GameObject::setTransform( const math::float3x4& tm )
{
	m_obj.setTransform( tm );
}

inline void GameObject::setRotation( const math::float3x3& rot )	
{
	m_obj.setRotation( rot );
}

inline void GameObject::setPosition( const math::float3& pos )			
{
	m_obj.setPosition( pos );
}

inline math::float3x3 GameObject::rotation() const
{
	return m_obj.rotation();
}

inline math::float3 GameObject::position() const					
{
	return m_obj.position();
}

inline math::float3x4 GameObject::transform() const
{
	return m_obj.transform();
}

inline dGeomID GameObject::geom() const		
{
	return m_obj.geom();
}

inline dBodyID GameObject::body() const		
{
	return m_obj.body();
}

inline GameObject::Type GameObject::type() const
{
	return (Type)dGeomGetCategoryBits( m_obj.geom() );
}

inline bool GameObject::destroyed() const	
{
	return m_remove;
}

inline bool	GameObject::isType( Type x ) const
{
	return ( dGeomGetCategoryBits(m_obj.geom()) & int(x) ) != 0;
}

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
