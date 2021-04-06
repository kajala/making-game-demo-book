#ifndef _LANG_DEBUG_H
#define _LANG_DEBUG_H


#include <lang/Formattable.h>


namespace lang
{


/** 
 * Provides support for debug output. Default implementation writes the output to
 * stdout and to Debug Output on Win32 platform.
 *
 * On Windows, debug output can be viewed with IDE debugger or stand-alone 
 * debug output viewer -- there are multiple available for free on Internet. This kind
 * of debug output has several benefits over traditional log files:
 * <ol>
 * <li>You get debug output visible already while running application without file access / reload issues.
 * <li>You always get up-to-date output, instead of accidentally looking at old debug output files.
 * <li>Debug output does not clutter up hard disk unless debug output viewer is active and logging enabled from the viewer.
 * <li>Debug output can be filtered, color coded, etc., marked, depending on debug output viewer application's features.
 * <li>Debug output viewer can provide support for remote debug output viewing on remote PC.
 * </ol>
 * 
 * @ingroup lang
 */
class Debug
{
public:
	/** Outputs a log message. printf compatible arguments. */
	static void			printf( const char* fmt, ... );

	/** Strips parent paths from full filename. */
	static const char*	stripSourceFilename( const char* fname );
};


} // lang


#ifndef DBG_TRACE
#define DBG_TRACE() lang::Debug::printf( "%s(%d)\n", lang::Debug::stripSourceFilename(__FILE__), __LINE__ )
#endif // DBG_TRACE

#ifndef DBG_OUT
#define DBG_OUT(A) lang::Debug::printf( "%s @ %s(%d)\n", (A), lang::Debug::stripSourceFilename(__FILE__), __LINE__ )
#endif // DBG_OUT

#ifndef DBG_ONCE
#define DBG_ONCE(A) {static bool s_once = true; if ( s_once && !(s_once=false) ) A;}
#endif // DBG_ONCE


#endif // _LANG_DEBUG_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
