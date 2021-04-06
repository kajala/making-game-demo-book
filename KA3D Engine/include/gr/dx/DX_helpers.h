#ifndef _GR_HELPERS_H
#define _GR_HELPERS_H


#include "DX_toString.h"
#include <lang/Debug.h>
#include <stdint.h>


namespace gr
{


/**
 * Reports error of failed DirectX operation. Does nothing if operation succeeded.
 * @return HRESULT
 */
inline HRESULT DX_reportError( HRESULT hr, const char* op )
{
	if ( D3D_OK != hr )
		::lang::Debug::printf( "ERROR: gr/dx: DX9 operation %d failed: %s\n", op, toString(hr) );
	return hr;
}

/** 
 * Releases COM interface and sets pointer 0. 
 */
template <class T> inline void DX_release( T*& p )
{
	if ( p )
	{
		p->Release();
		p = 0;
	}
}

/** 
 * Helper class for automatically releasing COM interface at the end of scope.
 */
class DX_AutoRelease
{
public:
	/**
	 * Does not increase reference count.
	 */
	DX_AutoRelease( IUnknown* i )		: m_i(i) {}

	/**
	 * Releases COM interface.
	 */
	~DX_AutoRelease()					{if (m_i) m_i->Release();}

	/**
	 * Cancels automatic release of the COM interface.
	 */
	void		cancel()				{m_i=0;}

private:
	IUnknown*	m_i;

	DX_AutoRelease( const DX_AutoRelease& );
	DX_AutoRelease& operator=( const DX_AutoRelease& );
};


} // gr


/** Tries the DirectX function which returns HRESULT and prints error if it fails. */
#define DX_TRY(OP) DX_reportError( OP, #OP )


#endif // _GR_HELPERS_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
