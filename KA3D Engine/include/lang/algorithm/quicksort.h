#ifndef _LANG_QUICKSORT_H
#define _LANG_QUICKSORT_H


namespace lang
{


/**
 * Traditional quick sort.
 * Slower than HP/SGI introsort, but generates
 * only a fraction of code so useful on mobile platforms.
 */
template <class T> void quicksort( T* begin, T* end )
{
	const int n = end - begin;
	if ( n > 1 )
	{
		const T v = begin[0];
		int i = 0;
		int j = n;
		for(;;)
		{
			while( begin[++i] < v && i < n ) 
			{
			}

			while( begin[--j] > v ) 
			{
			}

			if( i >= j ) 
				break;

			//swap( begin[i], begin[j] );
			T tmp( begin[i] );
			begin[i] = begin[j];
			begin[j] = tmp;
		}

		//swap( begin[i-1], begin[0] );
		T tmp( begin[i-1] );
		begin[i-1] = begin[0];
		begin[0] = tmp;

		quicksort( begin, begin+i-1 );
		quicksort( begin+i, end-i );
	}
}

/**
 * Traditional quick sort using predicate.
 * Slower than HP/SGI introsort, but generates
 * only a fraction of code so useful on mobile platforms.
 */
template <class T, class C> void quicksort( T* begin, T* end, C comp )
{
	const int n = end - begin;
	if ( n > 1 )
	{
		const T v = begin[0];
		int i = 0;
		int j = n;
		for(;;)
		{
			while( comp(begin[++i],v) && i < n ) 
			{
			}

			--j;
			while( !comp(begin[j],v) && comp(v,begin[j]) ) 
				--j;

			if( i >= j ) 
				break;

			//swap( begin[i], begin[j] );
			T tmp( begin[i] );
			begin[i] = begin[j];
			begin[j] = tmp;
		}

		//swap( begin[i-1], begin[0] );
		T tmp( begin[i-1] );
		begin[i-1] = begin[0];
		begin[0] = tmp;

		quicksort( begin, begin+i-1 );
		quicksort( begin+i, end-i );
	}
}


} // lang


#endif // _LANG_QUICKSORT_H
