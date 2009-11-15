/*
 *  Utils.c
 *  SeqAlign
 *
 *  Created by sopo on 09/11/09.
 *  Copyright 2009 UPMC. All rights reserved.
 *
 */


#include "Utils.h"


int
intArrayLength(		int		*array )
{
	return ( sizeof( array ) / sizeof( int ) );
}


void
modifValues(		int		*array,
					int		value )
{
	register unsigned int l_array;
	register unsigned int i;
	
	l_array = intArrayLength( array );
	
	for ( i = 0; i < l_array; ++i )
	{
		array[ i ] += value;
	}
}