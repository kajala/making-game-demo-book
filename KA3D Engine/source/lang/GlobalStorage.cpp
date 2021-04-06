#include <lang/GlobalStorage.h>
#ifdef __SYMBIAN32__
#include <e32std.h>
#endif
#include <string.h>
#include <config.h>


namespace lang
{


#ifndef __SYMBIAN32__
GlobalStorage GlobalStorage::sm_instance;
#endif


GlobalStorage::GlobalStorage()
{
	// a bit ugly but this class contains only regular pointers
	// and we don't want to end up forgetting to initialize some pointer
	memset( this, 0, sizeof(GlobalStorage) );
}

#ifdef __SYMBIAN32__

GlobalStorage& GlobalStorage::get()
{
	GlobalStorage* gs = reinterpret_cast<GlobalStorage*>( Dll::Tls() );
	if ( !gs )
	{
		gs = new GlobalStorage;
		Dll::SetTls( gs );
	}
	return *gs;
}

void GlobalStorage::release()
{
	GlobalStorage* gs = reinterpret_cast<GlobalStorage*>( Dll::Tls() );
	if ( gs != 0 )
	{
		delete gs;
		Dll::SetTls( 0 );
	}
}

#else

void GlobalStorage::release()
{
}

#endif


} // lang

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
