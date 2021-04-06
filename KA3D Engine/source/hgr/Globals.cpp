#include <hgr/Globals.h>
#include <hgr/DefaultResourceManager.h>
#include <config.h>


namespace hgr
{


Globals::Globals()
{
}

Globals::~Globals()
{
}

void Globals::init()
{
	cleanup();
	lang::GlobalStorage::get().hgrGlobals = new Globals;
}

void Globals::cleanup()
{
	delete lang::GlobalStorage::get().hgrGlobals;
	lang::GlobalStorage::get().hgrGlobals = 0;
}


} // hgr

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
