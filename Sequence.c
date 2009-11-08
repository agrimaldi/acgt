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

#include "Sequence.h"


Sequence* New_Sequence( char *sequence )
{
	Sequence *This = malloc( sizeof( Sequence ) );
	
	if( !This )
	{
		printf( "Sequence Failed to initialize" );
		return NULL;
	}
	
	Sequence_Init( This, sequence );
	
	This->Free = Sequence_Free;
	
	return This;
}


static void Sequence_Init( Sequence *This, char *sequence )
{
	This->sequence = sequence;
	This->complement = NULL;
	
	This->View = Sequence_View;
}


void Sequence_Free( Sequence *This )
{
	
}


void Sequence_Clear( Sequence *This )
{
	
}


void Sequence_View( Sequence *This )
{
	
}
