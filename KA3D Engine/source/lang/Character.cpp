#include <lang/Character.h>
#include <ctype.h>
#include <config.h>


namespace lang
{


bool Character::isDigit( int cp )
{
	if ( cp < int(0x80) )
		return 0 != isdigit( (char)cp );
	else
		return false;
}

bool Character::isLetter( int cp )
{
	if ( cp < int(0x80) )
		return 0 != isalpha( (char)cp );
	else
		return false;
}

bool Character::isLetterOrDigit( int cp )
{
	return isLetter(cp) || isDigit(cp);
}

bool Character::isLowerCase( int cp )
{
	if ( cp < int(0x80) )
		return 0 != islower( (char)cp );
	else
		return false;
}

bool Character::isUpperCase( int cp )
{
	if ( cp < int(0x80) )
		return 0 != isupper( (char)cp );
	else
		return false;
}

bool Character::isWhitespace( int cp )
{
	if ( cp < int(0x80) )
		return 0 != isspace( (char)cp );
	else
		return false;
}


} // lang

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
