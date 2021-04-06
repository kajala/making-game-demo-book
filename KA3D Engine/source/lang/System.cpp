#include <lang/System.h>
#include <lang/pp.h>

#ifdef PLATFORM_WIN32
	#define WIN32_LEAN_AND_MEAN
	#include <windows.h>
	#include <mmsystem.h>
	#pragma comment( lib, "winmm" )
#endif

#include <time.h>
#include <stdint.h>
#include <config.h>


namespace lang
{


int System::currentTimeMillis()
{
#ifdef WIN32

	LARGE_INTEGER freq;
	if ( QueryPerformanceFrequency(&freq) )
	{
		LARGE_INTEGER cur;
		QueryPerformanceCounter( &cur );
		if ( freq.QuadPart >= 1000 )
		{
			__int64 msdiv = __int64(freq.QuadPart) / __int64(1000);
			__int64 c = __int64(cur.QuadPart) / msdiv;
			return (int)c;
		}
	}

	return (int)timeGetTime();

#elif defined(PLATFORM_PS2)
	
	return 0;
	
#else

	uint64_t time = (uint64_t)clock() * 1000 / CLOCKS_PER_SEC;
	return (int)time;

#endif
}


} // lang

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
