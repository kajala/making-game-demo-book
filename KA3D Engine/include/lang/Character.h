#ifndef _LANG_CHARACTER_H
#define _LANG_CHARACTER_H


namespace lang
{


/** 
 * Character type information.
 * 
 * @ingroup lang
 */
class Character
{
public:
	/** Returns true if the code point is a Unicode digit. */
	static bool		isDigit( int cp );

	/** Returns true if the code point is a Unicode letter. */
	static bool		isLetter( int cp );

	/** Returns true if the code point is a Unicode letter or digit. */
	static bool		isLetterOrDigit( int cp );

	/** Returns true if the code point is a Unicode lower-case. */
	static bool		isLowerCase( int cp );

	/** Returns true if the code point is a Unicode upper-case. */
	static bool		isUpperCase( int cp );

	/** Returns true if the code point is a Unicode whitespace. */
	static bool		isWhitespace( int cp );
};


} // lang


#endif // _LANG_CHARACTER_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
