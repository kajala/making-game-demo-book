#ifndef _LANG_GLOBALSTORAGE_H
#define _LANG_GLOBALSTORAGE_H


namespace hgr {class Globals;}
namespace lang {class Globals;}


namespace lang
{


/** 
 * Storage for global variables. 
 * Dependency to users in the name only.
 * @ingroup lang
 */
class GlobalStorage
{
public:
	hgr::Globals*	hgrGlobals;
	lang::Globals*	langGlobals;
	void*			userGlobals;

	GlobalStorage();

	/** Returns global storage singleton. */
	static GlobalStorage&	get();

	/** Releases global storage singleton. */
	static void				release();

private:
#ifndef __SYMBIAN32__
	static GlobalStorage sm_instance;
#endif
};


#ifndef __SYMBIAN32__
inline GlobalStorage& GlobalStorage::get() 	{return sm_instance;}
#endif


} // lang


#endif // _LANG_GLOBALSTORAGE_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
