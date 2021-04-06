#ifndef _HGR_GLOBALS_H
#define _HGR_GLOBALS_H


#include <hgr/ParticleSystem.h>
#include <lang/Array.h>
#include <lang/Hashtable.h>
#include <lang/GlobalStorage.h>


namespace hgr
{


class ResourceManager;


/** 
 * Globals in hgr library.
 * @ingroup hgr
 */
class Globals
{
public:
	/**
	 * Default resource manager.
	 */
	P(ResourceManager)		resourceManager;

	/**
	 * Initializes the globals.
	 */
	static void				init();

	/**
	 * Releases the resources allocated to the globals.
	 */
	static void				cleanup();

	/**
	 * Returns the globals.
	 */
	static Globals&			get();

private:
	Globals();
	~Globals();

	Globals( const Globals& );
	Globals& operator=( const Globals& );
};


inline Globals& Globals::get()
{
	lang::GlobalStorage& gs = lang::GlobalStorage::get(); 
	if ( !gs.hgrGlobals )
		init();
	return *gs.hgrGlobals;
}


} // hgr


#endif // _HGR_GLOBALS_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
