#ifndef _LANG_INTERNALERROR_H
#define _LANG_INTERNALERROR_H


namespace lang
{


/** 
 * Throws Exception with 'Internal error at <filename>(<line>): <expr>' message. 
 * @ingroup lang
 */
void internalError( const char* filename, int line, const char* expr );


} // lang


#endif // _LANG_INTERNALERROR_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
