inline Node* Node::parent() const
{
	return m_parent;
}
	
inline Node* Node::firstChild() const
{
	return m_child;
}
	
inline Node* Node::getNextChild( const Node* child ) const
{
	assert( child->m_parent == this );
	return child->m_next;
}

inline const math::float3x4& Node::transform() const
{
	return m_modeltm;
}

inline void Node::setName( const lang::String& name )
{
	m_name = name;
}

inline const lang::String& Node::name() const
{
	return m_name;
}

inline math::float3x3 Node::rotation() const
{
	return m_modeltm.rotation();
}

inline math::float3 Node::position() const
{
	return m_modeltm.translation();
}

inline Node::NodeClassId Node::classId() const		
{
	return NodeClassId(m_flags & int(NODE_CLASS));
}

inline void Node::setFlags( int flags )			
{
	m_flags = (short)flags;
}

inline void Node::setFlag( NodeFlags flag, bool enabled )
{
	if ( enabled )
		m_flags = (short)( int(m_flags) | flag );
	else
		m_flags = (short)( int(m_flags) & ~flag );
}

inline bool Node::getFlag( NodeFlags flag ) const
{
	return 0 != (flag & int(m_flags));
}

inline int Node::flags() const					
{
	return m_flags;
}

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
