#ifndef _DX_INCLUDEMANAGER_H
#define _DX_INCLUDEMANAGER_H


#include <io/PathName.h>
#include <lang/String.h>


namespace gr
{


/**
 * Class for loading shader #include files.
 */
class DX_IncludeManager :
	public ID3DXInclude
{
public:
	DX_IncludeManager( const io::PathName& path );

    STDMETHOD(Open)( D3DXINCLUDE_TYPE includetype, LPCSTR filename, LPCVOID parentdata, LPCVOID* data, UINT* bytes );
    STDMETHOD(Close)( LPCVOID data );

private:
	io::PathName m_path;
};


} // gr


#endif // _DX_INCLUDEMANAGER_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
