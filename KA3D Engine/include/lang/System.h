#ifndef _LANG_SYSTEM_H
#define _LANG_SYSTEM_H


namespace lang
{


/** 
 * System class provides running environment related information.
 * 
 * @ingroup lang
 */
class System
{
public:
	/** 
	 * Returns current system time in milliseconds. 
	 */
	static int currentTimeMillis();

private:
	System();
	System( const System& );
	System& operator=( const System& );
};


} // lang


#endif // _LANG_SYSTEM_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
