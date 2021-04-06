#ifndef _LANG_SWAP_H
#define _LANG_SWAP_H


namespace lang
{


/** 
 * Swaps two items. 
 */
template <class T> void swap( T& a, T& b )
{
	T tmp( a );
	a = b;
	b = tmp;
}


} // lang


#endif // _LANG_SWAP_H
