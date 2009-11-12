/*
 *  Sequence.c
 *  SeqAlign
 *
 *  Created by sopo on 01/11/09.
 *  Copyright 2009 UPMC. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>

#include "Sequence.h"



static void Sequence_Init( Sequence *This, char *sequence );



Sequence* New_Sequence( char *sequence )
{
	Sequence *This = malloc( sizeof( Sequence ) );
	
	if( This == NULL )
	{
		return ( NULL );
	}
	
	Sequence_Init( This, sequence );
	
	return ( This );
}



void Sequence_Free( Sequence *This )
{
	if ( This->dynalloc )
	{
		free( This->sequence );
	}

	free( This );
}



void Sequence_View( Sequence *This )
{
	printf( "%s", This->sequence );
}



char* Sequence_Random( Sequence *This, int size )
{
	char *sequence = malloc( ( size + 1 ) * sizeof( char ) );
	
	register int i;
	register int random;
	
	srand( time( NULL ) );
	
	for ( i = 0; i < size ; i++ )
	{
		random = rand() % 4;
		
		if ( random == 0 )
		{
			sequence[ i ] = 'a';
		}
		else if ( random == 1 )
		{
			sequence[ i ] = 'c';
		}
		else if ( random == 2 )
		{
			sequence[ i ] = 'g';
		}
		else
		{
			sequence[ i ] = 't';
		}
		
		sequence[ size ] = '\0';
	}
	
	This->dynalloc = true;
	
	return ( sequence );
}



static void Sequence_Init( Sequence *This, char *sequence )
{
	This->sequence = sequence;
	This->complement = NULL;
}
