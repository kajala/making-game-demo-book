#ifndef _IMG_TEST_H
#define _IMG_TEST_H


namespace lang {
	class String;}


namespace img
{


/**
 * Performs img library internal tests.
 * @param datapath Path to data/ directory, where the test images are.
 */
void test( const lang::String& datapath );


} // img


#endif // _IMG_TEST_H

// Copyright (C) 2005 Jani Kajala. All rights reserved. Consult your license regarding permissions and restrictions.
