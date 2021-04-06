#include <io/PropertyParser.h>
#include <io/IOException.h>
#include <lang/Debug.h>
#include <ctype.h>
#include <string.h>
#include <config.h>


using namespace lang;


namespace io
{


PropertyParser::ConstIterator::ConstIterator( const PropertyParser* parser, int pos, int line ) :
	m_parser( parser ),
	m_pos( pos ),
	m_line( line )
{
	if ( parser != 0 )
		this->operator++();
}

PropertyParser::ConstIterator& PropertyParser::ConstIterator::operator++()
{
	assert( m_parser != 0 );

	if ( m_parser->skipComments(m_pos,m_line) )
	{
		m_parser->parsePair( m_pos, m_line, m_parser->m_temp );
	}
	else
	{
		m_parser = 0;
		m_pos = 0;
	}
	return *this;
}

bool PropertyParser::ConstIterator::operator!=( const ConstIterator& other ) const
{
	return m_pos != other.m_pos || m_parser != other.m_parser;
}

const char* PropertyParser::ConstIterator::key() const
{
	const Array<char>& x = m_parser->m_temp;
	return x.begin();
}

const char* PropertyParser::ConstIterator::value() const
{
	const Array<char>& x = m_parser->m_temp;
	return x.begin() + strlen(x.begin()) + 1;
}


PropertyParser::PropertyParser() :
	m_buf(),
	m_len( 0 )
{
}

PropertyParser::PropertyParser( const String& buf, const String& name ) :
	m_buf( buf ),
	m_len( buf.length() ),
	m_name( name )
{
}

void PropertyParser::reset( const String& buf, const String& name )
{
	m_buf = buf;
	m_len = buf.length();
	m_name = name;
	m_temp.clear();
}

inline char PropertyParser::get( int pos ) const
{
	assert( pos >= 0 && pos <= m_len );
	return pos < m_len ? m_buf.charAt(pos) : 0;
}

const char* PropertyParser::getString( const char* key ) const
{
	assert( hasKey(key) );
	if ( this->get(key,m_temp) )
		return m_temp.begin();
	return 0;
}

bool PropertyParser::getBoolean( const char* key ) const
{
	assert( hasKey(key) );
	
	bool haskey = this->get( key, m_temp );
	toLowerCase( m_temp );

	if ( !haskey )
		throwError( IOException(Format("Failed to parse boolean, no key {0} in \"{1}\"", key, m_name)) );
	else if ( !strcmp(&m_temp[0],"enabled") || !strcmp(&m_temp[0],"true") || !strcmp(&m_temp[0],"1") || !strcmp(&m_temp[0],"yes") )
		return true;
	else if ( !strcmp(&m_temp[0],"disabled") || !strcmp(&m_temp[0],"false") || !strcmp(&m_temp[0],"0") || !strcmp(&m_temp[0],"no") )
		return false;
	else
		throwError( IOException(Format("Failed to parse boolean {0} from \"{1}\"", key, m_name)) );
	return false;
}

bool PropertyParser::hasKey( const char* key ) const
{
	return this->get( key, m_temp );
}

bool PropertyParser::get( const char* key, Array<char>& x ) const
{
	x.resize( 128 );
	x.clear();
	m_temp.clear();

	int line = 1;
	for ( int pos = 0 ; skipComments(pos,line) ; )
	{
		parsePair( pos, line, x );
		if ( !strcmp(x.begin(),key) )
		{
			x.remove( 0, strlen(key)+1 );
			return true;
		}
	}

	x.clear();
	return false;
}

void PropertyParser::parsePair( int& pos, int& line, Array<char>& x ) const
{
	// empty output buffer
	x.clear();

	// parse key
	skipSpace( pos, line );
	parseKey( pos, line, x );

	// parse value
	skipSpace( pos, line );
	char ch = get(pos);
	if ( ch != '=' )
		throwError( IOException(Format("Missing \"{0}=<value>\" on line {1} of property set \"{2}\":\n{3}", x.begin(), line, m_name, m_buf)) );
	skipSpace( ++pos, line );
	parseValue( pos, line, x );
}

void PropertyParser::parseKey( int& pos, int& /*line*/, Array<char>& value ) const
{
	// read until whitespace or '=', eol or eof
	for ( char ch=get(pos) ; 
		ch != 0 && !isspace(ch) && ch != '=' ; 
		ch=get(++pos) )
	{
		if ( ch > 0 )
			ch = (char)tolower( ch );
		value.add( ch );
	}

	value.add( 0 );
}

void PropertyParser::parseValue( int& pos, int& /*line*/, Array<char>& value ) const
{
	// read until eol or eof
	for ( char ch = get(pos) ; ch != 0 && ch != '\n' && ch != ';' ; ch = get(++pos) )
		value.add( ch );

	// trim trailing whitespace
	while ( value.size() > 0 && isspace(value.last()) )
		value.resize( value.size()-1 );

	value.add( 0 );
}

bool PropertyParser::skipLine( int& pos, int& line ) const
{
	while ( pos < m_len && get(pos) != '\n' )
		++pos;
	++line;
	return pos < m_len;
}

bool PropertyParser::skipSpace( int& pos, int& line ) const
{
	for ( char ch = get(pos) ; isspace(ch) ; ch = get(++pos) )
	{
		if ( ch == '\n' )
			++line;
	}
	return get(pos) != 0;
}

bool PropertyParser::skipComments( int& pos, int& line ) const
{
	while ( skipSpace(pos,line) )
	{
		if ( isComment(pos) )
			skipLine( pos, line );
		else if ( !isspace(get(pos)) )
			break;
	}
	return get(pos) != 0;
}

bool PropertyParser::isComment( int pos ) const
{
	return pos+1 < m_len && get(pos) == '-' && get(pos+1) == '-';
}

PropertyParser::ConstIterator PropertyParser::begin() const
{
	return ConstIterator( this, 0, 1 );
}

PropertyParser::ConstIterator PropertyParser::end() const
{
	return ConstIterator( 0, 0, -1 );
}

void PropertyParser::toLowerCase( Array<char>& temp )
{
	// ignore case of boolean 
	for ( int i = 0 ; i < temp.size() ; ++i )
	{
		if ( temp[i] > 0 )
			temp[i] = (char)tolower( temp[i] );
	}
}


} // io

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
