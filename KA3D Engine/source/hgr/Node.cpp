#include <hgr/Node.h>
#include <gr/Context.h>
#include <lang/Math.h>
#include <lang/Float.h>
#include <lang/Debug.h>
#include <lang/Exception.h>
#include <math/float4x4.h>
#include <config.h>


using namespace gr;
using namespace lang;
using namespace math;


namespace hgr
{


Node::Node() :
	m_modeltm( 1.f ),
	m_flags(NODE_DEFAULTS),
	m_tmindex(-1),
	m_parent(0),
	m_child(0),
	m_next(0),
	m_previous(0),
	m_name()
{
}

Node::Node( const Node& other ) :
	m_modeltm( other.m_modeltm ),
	m_flags( other.m_flags ),
	m_tmindex( other.m_tmindex ),
	m_parent(0),
	m_child(0),
	m_next(0),
	m_previous(0),
	m_name( other.m_name )
{
}

Node::~Node()
{
	unlink();

	while ( m_child != 0 )
		m_child->unlink();
}

void Node::setTransform( const float3x4& transform )
{
	assert( transform.finite() );

	m_modeltm = transform;
}

void Node::setPosition( const float3& position )									
{
	assert( position.finite() );

	m_modeltm.setTranslation( position );
}

void Node::setRotation( const float3x3& rotation )
{
	assert( rotation.finite() );

	m_modeltm.setRotation( rotation );
}

float3x4 Node::worldTransform() const
{
	float3x4 worldtm = m_modeltm;
	for ( Node* parent = m_parent ; parent != 0 ; parent = parent->m_parent )
		worldtm = parent->m_modeltm * worldtm;
	return worldtm;
}

void Node::lookAt( const math::float3& target, const math::float3& up )
{
	assert( Math::abs(up.length()-1.f) < 1e-3f ); // Up direction must be normalized

	// src parent->world space
	float3x4 parenttoworld;
	if ( m_parent != 0 )
		parenttoworld = m_parent->worldTransform();
	else
		parenttoworld = float3x4(1.f);

	// src->world space
	float3x4 sourcetoworld = parenttoworld * transform();

	// src -> target (world space)
	float3 sourcerotz = target - sourcetoworld.translation();
	if ( sourcerotz.lengthSquared() > Float::MIN_VALUE )
	{
		// src->target direction (world space)
		sourcerotz = normalize(sourcerotz);

		// src rotation (world space)
		float3 sourcerotx = cross( up, sourcerotz );
		if ( sourcerotx.lengthSquared() > Float::MIN_VALUE )
			sourcerotx = normalize(sourcerotx);
		else
			sourcerotx = float3(1,0,0);
		float3 sourceroty = cross( sourcerotz, sourcerotx );
		float3x3 sourcerot;
		sourcerot.setColumn( 0, sourcerotx );
		sourcerot.setColumn( 1, sourceroty );
		sourcerot.setColumn( 2, sourcerotz );

		// src world space rotation back to src parent space
		float3x3 parenttoworldrot = parenttoworld.rotation();
		float3x3 rot = sourcerot * parenttoworldrot.transpose();
		setRotation( rot );
	}
}

void Node::lookAt( const Node* target, const float3& up )
{
	lookAt( target->worldTransform().translation(), up );
}

bool Node::hasParent( const Node* other ) const
{
	for ( const Node* parent = this->parent() ; parent != 0 ; parent = parent->parent() )
	{
		if ( other == parent )
			return true;
	}
	return false;
}

void Node::linkTo( Node* parent )
{
	assert( parent );					// parent node must exist
	assert( !parent->hasParent(this) ); // cannot handle cyclic hierarchies
	assert( parent != this );			// immediate cyclic hierarchy

	unlink();

	m_next = parent->m_child;
	if ( m_next ) 
		m_next->m_previous = this;

	m_parent = parent;
	m_parent->m_child = this;
}

void Node::unlink()
{
	if ( m_parent )
	{
		// keep reference for safety (avoid premature destruction)
		P(Node) thisnode = this;
		
		if ( m_parent->m_child == this )
		{
			// this is the first child
			assert( !m_previous );
			m_parent->m_child = m_next;
			if ( m_next )
				m_next->m_previous = 0;
		}
		else
		{
			// this is not the first child
			assert( m_previous );
			if ( m_previous )
				m_previous->m_next = m_next;
			if ( m_next )
				m_next->m_previous = m_previous;
		}

		m_parent = 0;
		m_next = 0;
		m_previous = 0;
	}
}

Node* Node::root() const
{
	Node* root = const_cast<Node*>(this);
	while ( root->parent() )
		root = root->parent();
	return root;
}

Node* Node::next() const
{
	if ( m_child )
		return m_child; // found child
	else if ( m_next ) 
		return m_next; // found sibling
	else 
	{
		// parent done, move to next one
		for ( Node* node = m_parent ; node ; node = node->m_parent )
			if ( node->m_next )
				return node->m_next;
	}

	return 0; // end of hierarchy
}

int Node::depth() const
{
	int d = 0;
	for ( Node* p = m_parent ; 0 != p ; p = p->m_parent )
		++d;
	return d;
}

void Node::setClassId( NodeClassId type )
{
	assert( NODE_OTHER == classId() && "NodeClassId can be set only once per object" );
	m_flags = short( (m_flags & ~int(NODE_CLASS)) + int(type) );
}

Node* Node::getNodeByName( const String& name ) const
{
	Node* node = 0;
	for ( node = root() ; node != 0 ; node = node->next() )
		if ( node->name() == name )
			break;

	if ( node == 0 )
		throwError( Exception( Format("Node named \"{0}\" not found in scene \"{1}\".", name, root()->name()) ) );
	return node;
}

Node* Node::findNodeByName( const String& name ) const
{
	Node* node = 0;
	for ( node = root() ; node != 0 ; node = node->next() )
		if ( node->name() == name )
			break;
	return node;
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
