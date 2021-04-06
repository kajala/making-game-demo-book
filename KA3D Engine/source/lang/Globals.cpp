#include <lang/Globals.h>
#include <config.h>


namespace lang
{


Globals::Globals() :
	stringPool(10000,0,"String"),
	cstrBufferIndex(0)
{
}

Globals::~Globals()
{
}

void Globals::init()
{
	cleanup();
	GlobalStorage::get().langGlobals = new Globals;
}

void Globals::cleanup()
{
	delete GlobalStorage::get().langGlobals;
	GlobalStorage::get().langGlobals = 0;
	GlobalStorage::release();
}


} // lang

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
