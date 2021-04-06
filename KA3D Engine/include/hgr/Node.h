#ifndef _HGR_NODE_H
#define _HGR_NODE_H


#include <lang/String.h>
#include <lang/Object.h>
#include <math/float3x4.h>


namespace gr {
	class Context;}


namespace hgr
{


class Camera;
class SceneInputStream;
class SceneOutputStream;


/**
 * Base class for all objects in scene graph. Node contains model
 * transformation and object hierarchy information.
 * Note that even though nodes are reference counted, 
 * references to parent are weak, i.e. parent can be destroyed
 * even though child is still referenced by the user. In that case
 * the child is automatically unlinked from the parent.
 * @ingroup hgr
 */
class Node :
	public lang::Object
{
public:
	/*
	 * Node class identifier. Used by the implementation to 
	 * avoid depending on RTTI, since this class might 
	 * be need to be ported to some platforms which do not 
	 * support run-time type information.
	 */
	enum NodeClassId
	{
		/** Node is either plain node or unknown derived class. Default type. */
		NODE_OTHER	= (0<<4),
		/** Node is Camera. */
		NODE_CAMERA	= (1<<4),
		/** Node is Light. */
		NODE_LIGHT	= (2<<4),
		/** Node is Visual. */
		NODE_VISUAL	= (3<<4),
		/** Node is Scene. */
		NODE_SCENE	= (4<<4)
	};

	/*
	 * Internal node state flags.
	 */
	enum NodeFlags
	{
		/** Node enabled flag. Exact usage semantics is dependent on derived class. */
		NODE_ENABLED				= 1, // bit 0
		/** Node renderable flag. Exact usage semantics is dependent on derived class. */
		NODE_RENDERABLE				= 2, // bit 1
		/** Set only for Light class instances. */
		NODE_LIGHTTYPE				= (3<<2), // bits 2:3
		/** Shift needed to access light type. */
		NODE_LIGHTTYPE_SHIFT		= 2,
		/** NodeClassId of this Node. Default is NODE_OTHER. */
		NODE_CLASS					= (15<<4), // bits 4:7
		/** Set if Node's bounding volumes are stored in world space coordinates. */
		NODE_BOUNDWORLDSPACE		= (1<<8),
		/** Default type flags for Node. */
		NODE_DEFAULTS				= NODE_ENABLED + NODE_OTHER,
	};

	/** 
	 * Creates non-parented node with identity as model transformation. 
	 */
	Node();

	~Node();

	/** 
	 * Copy by value. Clones children. 
	 */
	Node( const Node& other );

	/** 
	 * Sets name of this node. 
	 */
	void					setName( const lang::String& name );

	/** 
	 * Sets model-to-parent node transform. Rotation can have scaling. 
	 */
	void					setTransform( const math::float3x4& transform );

	/** 
	 * Sets model-to-parent node position. 
	 */
	void					setPosition( const math::float3& position );

	/** 
	 * Sets model-to-parent node rotation. Rotation can have scaling. 
	 */
	void					setRotation( const math::float3x3& rotation );

	/** 
	 * Makes this node rotation to point at other node in world space.
	 * @param target Node to look at.
	 * @param up Up direction in world space.
	 */
	void					lookAt( const Node* target, const math::float3& up = math::float3(0,1,0) );

	/** 
	 * Makes this node rotation to point at target position in world space.
	 * @param target World space position to look at.
	 * @param up Up direction in world space.
	 */
	void					lookAt( const math::float3& target, const math::float3& up = math::float3(0,1,0) );

	/** 
	 * Links this as a child to parent node. 
	 * Increments reference counts of both nodes.
	 */
	void					linkTo( Node* parent );

	/** 
	 * Unlinks this from parent() node. 
	 * Decrements reference counts of both nodes.
	 */
	void					unlink();

	/*
	 * Sets node transform index. Used by rendering implementation 
	 * to keep track of cached world transforms.
	 */
	void					setTransformIndex( int  );

	/** 
	 * Returns name of this node. 
	 */
	const lang::String&		name() const;

	/** 
	 * Returns current transform in parent space. Rotation can have scaling. 
	 */
	const math::float3x4&	transform() const;

	/** 
	 * Returns current rotation in parent space. Rotation can have scaling. 
	 */
	math::float3x3			rotation() const;

	/** 
	 * Returns current position in parent space. Rotation can have scaling. 
	 */
	math::float3			position() const;
	
	/** 
	 * Calculates current model-to-world transform. 
	 */
	math::float3x4			worldTransform() const;

	/**
	 * Returns cached (model-to-world) transform. 
	 * Can be used only during rendering.
	 */
	const math::float3x4&	getCachedTransform( gr::Context* context ) const;

	/** 
	 * Returns the parent of this node. Returns 0 if no parent set. 
	 */
	Node*					parent() const;
	
	/** 
	 * Returns the first child of this node. Returns 0 if no children. 
	 */
	Node*					firstChild() const;
	
	/** 
	 * Returns the next child of this node. Returns 0 if none. 
	 */
	Node*					getNextChild( const Node* child ) const;

	/** 
	 * Returns the root of the node hierarchy. 
	 */
	Node*					root() const;

	/** 
	 * Returns true if this node has the other node as some grand parent. 
	 */
	bool					hasParent( const Node* other ) const;

	/**
	 * Returns depth of the hierarchy at specified node.
	 * If the node has no parent then the depth if 0, if single parent then depth is 1, etc.
	 */
	int						depth() const;

	/** 
	 * Iterates through the scene graph hierarchy. 
	 * This function can be used to enumerate all objects in the scene graph.
	 * Iteration is performed in child-first-then-sibling order.
	 * Note that to iterate throught the scene graph the return
	 * value of this function must be used to ask for next node.
	 */
	Node*					next() const;

	/**
	 * Finds recursively node with specified name from the node hierarchy.
	 * @exception Exception If node not found.
	 */
	Node*					getNodeByName( const lang::String& name ) const;

	/**
	 * Tries to find node recursively with specified name from the node hierarchy.
	 * @return 0 if node not found.
	 */
	Node*					findNodeByName( const lang::String& name ) const;

	/*
	 * Returns type of the node. Used by the implementation to 
	 * avoid depending on RTTI.
	 */
	NodeClassId				classId() const;

protected:
	/*
	 * Sets class id of the node.
	 * Warning: This is not RTTI checked.
	 */
	void	setClassId( NodeClassId type );

	/*
	 * Sets node flags. Node flags are internal representation
	 * method of some node options. This method, along with flags(),
	 * is used only for loading/saving of nodes data.
	 */
	void	setFlags( int flags );

	/*
	 * Sets a single flag enabled/disabled.
	 */
	void	setFlag( NodeFlags flag, bool enabled );

	/* 
	 * Returns node flags. Node flags are internal representation
	 * method of some node options. This method, along with setFlags,
	 * is used only for loading/saving of node data.
	 */
	int		flags() const;

	/*
	 * Gets a single flag enabled/disabled state.
	 */
	bool	getFlag( NodeFlags flag ) const;

private:
	friend class Camera;
	friend class SceneInputStream;
	friend class SceneOutputStream;

	math::float3x4					m_modeltm;
	mutable short					m_flags;
	mutable short					m_tmindex;
	Node*							m_parent;
	P(Node)							m_child;
	P(Node)							m_next;
	Node*							m_previous;
	lang::String					m_name;

	Node& operator=( const Node& );
};


#include <hgr/Node.inl>


} // hgr


#endif // _HGR_NODE_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
