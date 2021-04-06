#ifndef _GR_SORTBUFFER_H
#define _GR_SORTBUFFER_H


#include <lang/Array.h>
#include <stdint.h>


namespace gr
{


class SortGreater
{
public:
	explicit SortGreater( const float* z ) : m_z(z) {}

	inline bool operator()( int a, int b ) const
	{
		return m_z[a] > m_z[b];
	}

private:
	const float* m_z;
};

class SortLess
{
public:
	explicit SortLess( const float* z ) : m_z(z) {}

	inline bool operator()( int a, int b ) const
	{
		return m_z[a] < m_z[b];
	}

private:
	const float* m_z;
};

/*
 * Temporary buffer used by the gr platform dependent implementations
 * to sort polygons.
 */
class SortBuffer
{
public:
	SortBuffer();
	~SortBuffer();

	void reset( int intbuffersize, int floatbuffersize );

	uint16_t*	intBuffer()			{return m_intBuffer;}
	float*		floatBuffer()		{return m_floatBuffer;}

private:
	lang::Array<uint8_t>	m_buf;
	uint16_t*				m_intBuffer;
	float*					m_floatBuffer;

	SortBuffer( const SortBuffer& );
	SortBuffer& operator=( const SortBuffer& );
};


} // gr


#endif // _GR_SORTBUFFER_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
