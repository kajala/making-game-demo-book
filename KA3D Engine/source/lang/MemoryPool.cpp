#include <lang/MemoryPool.h>
#include <lang/Debug.h>
#include <lang/System.h>
#include <lang/Integer.h>
#include <lang/OutOfMemoryException.h>
#include <algorithm>
#include <string.h>
#include <config.h>


namespace lang
{

	
static const int BLOCK_ALIGNMENT = 4;


MemoryPool::MemoryPool( int size, int maxsize, const char* name ) :
	m_maxSize( maxsize ),
	m_next( 0 ),
	m_maxgctime( 0 )
{
	resize( size );

	if ( 0 == m_maxSize )
		m_maxSize = Integer::MAX_VALUE;

	String::cpy( m_name, sizeof(m_name), name );
}

MemoryPool::~MemoryPool()
{
	Debug::printf( "lang: Destroying MemoryPool \"%s\". Max time spend in compact() was %d ms.\n", m_name, m_maxgctime );

	for ( int i = 0 ; i < m_blocks.size() ; ++i )
	{
		if ( 0 != m_blocks[i] )
			Debug::printf( "ERROR: lang.~MemoryPool: Memory still in use: \"%s\"\n", m_blocks[i] );
		assert( 0 == m_blocks[i] );
	}
}

void MemoryPool::dump() const
{
	Debug::printf( "Dumping memory pool \"%s\":\n", m_name );
	Debug::printf( "(%d blocks, %d bytes, %d byte pool size)\n", m_blocks.size(), m_next, m_mem.size() );
	for ( int i = 0 ; i < m_blocks.size() ; ++i )
	{
		char* p = m_blocks[i];
		if ( p != 0 )
		{
			BlockHeader* header = reinterpret_cast<BlockHeader*>(p)-1;
			Debug::printf( "  block[%d]: size=%d, refs=%d, data='%s'\n", i, header->size, header->refs, m_blocks[i] );
		}
		else
			Debug::printf( "  block[%d]: null\n", i );
	}
	Debug::printf( "\n" );
}

int MemoryPool::allocate( int bytes )
{
	// make sure there is enough space
	int spaceneeded = bytes+BLOCK_ALIGNMENT+(int)sizeof(BlockHeader);
	if ( m_next+spaceneeded > size() )
	{
		compact();
		if ( m_next+spaceneeded > size() && m_next+spaceneeded <= m_maxSize )
		{
			resize( m_mem.size() + spaceneeded );

			if ( m_next+spaceneeded > size() )
			{
				throwError( OutOfMemoryException() );
			}
		}
	}

	// get release memory block index
	int index;
	if ( m_freed.size() > 0 )
	{
		index = m_freed.last();
		m_freed.resize( m_freed.size()-1 );
	}
	else
	{
		index = m_blocks.size();
		m_blocks.add( 0 );
	}
	assert( index >= 0 && index < m_blocks.size() );
	
	// align next block pointer
	m_next = (m_next+BLOCK_ALIGNMENT-1) & ~(BLOCK_ALIGNMENT-1);
	
	// allocate block
	bytes += sizeof(BlockHeader);
	char* mem = m_mem.begin() + m_next;
	BlockHeader* header = reinterpret_cast<BlockHeader*>(mem);
	header->size = bytes;
	header->refs = 1;
	m_blocks[index] = mem + sizeof(BlockHeader);
	m_next += bytes;
	
	return index;
}

void MemoryPool::unalloc( int handle )
{
	assert( m_blocks[handle] );
	assert( (reinterpret_cast<BlockHeader*>(m_blocks[handle])-1)->refs == 0 );
	
	m_freed.add( handle );
	m_blocks[handle] = 0;
}

void MemoryPool::compact()
{
	//Debug::printf( "lang: MemoryPool::compact()\n" );

	// DEBUG: max time spend in compact
	int time = System::currentTimeMillis();

	// sort to ascending memory addresses
	m_gcbuf.resize( m_blocks.size() );
	for ( int i = 0 ; i < m_blocks.size() ; ++i )
		m_gcbuf[i] = i;
	std::sort( m_gcbuf.begin(), m_gcbuf.end(), HandleSorter(this) );
	
	// remove gaps
	m_next = 0;
	for ( int i = 0 ; i < m_gcbuf.size() ; ++i )
	{
		int index = m_gcbuf[i];
		char* src =  m_blocks[index];
		if ( src != 0 )
		{
			BlockHeader* header = reinterpret_cast<BlockHeader*>(src)-1;
			int size = header->size;
			
			// align next block pointer
			m_next = (m_next+BLOCK_ALIGNMENT-1) & ~(BLOCK_ALIGNMENT-1);
			
			char* dst = m_mem.begin() + m_next;
			assert( m_next+size <= m_mem.size() );
			memmove( dst, header, size );
			m_next += size;
			
			m_blocks[index] = dst + sizeof(BlockHeader);
		}
	}

	// DEBUG: max time spend in compact
	time = System::currentTimeMillis() - time;
	if ( m_maxgctime > time )
		m_maxgctime = time;
}

int MemoryPool::blocksAllocated() const
{
	assert( m_blocks.size() >= m_freed.size() );
	return m_blocks.size() - m_freed.size();
}

void MemoryPool::resize( int newsize )
{
	assert( newsize > m_mem.size() );

	// make sure the memory size grows at least 50%
	int minsize = m_mem.size() * 3 >> 1;
	minsize = (minsize+15) & ~15;
	if ( newsize < minsize )
		newsize = minsize;

	m_gcbuf.resize( m_blocks.size() );
	for ( int i = 0 ; i < m_blocks.size() ; ++i )
		m_gcbuf[i] = m_blocks[i] != 0 ? m_blocks[i] - m_mem.begin() : -1;
	
	m_mem.resize( newsize );
	
	m_blocks.clear();
	for ( int i = 0 ; i < m_gcbuf.size() ; ++i )
		m_blocks.add( m_gcbuf[i] != -1 ? m_gcbuf[i] + m_mem.begin() : 0 );
}


} // lang

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
