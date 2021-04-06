#ifndef _LANG_GLOBALS_H
#define _LANG_GLOBALS_H


#include <lang/MemoryPool.h>
#include <lang/GlobalStorage.h>


namespace lang
{


/** 
 * Globals in lang library.
 * @ingroup lang
 */
class Globals
{
public:
	/** String pool used by the String class implementation. */
	lang::MemoryPool	stringPool;

	/** Temporary buffer used by the String::c_str() implementation. */
	char				cstrBuffer[2000];

	/** Temporary buffer index used by the String::c_str() implementation. */
	int					cstrBufferIndex;

	/**
	 * Initializes the globals.
	 */
	static void			init();

	/**
	 * Releases the resources allocated to the globals.
	 */
	static void			cleanup();

	/**
	 * Returns the globals.
	 */
	static Globals&		get();

private:
	Globals();
	~Globals();
};


inline Globals& Globals::get()
{
	lang::GlobalStorage& gs = lang::GlobalStorage::get(); 
	if ( !gs.langGlobals )
		init();
	return *gs.langGlobals;
}


} // lang


#endif // _LANG_GLOBALS_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
