/*
 *  Cell.c
 *  SeqAlign
 *
 *  Created by sopo on 21/10/09.
 *  Copyright 2009 UPMC. All rights reserved.
 *
 */

#include <stdlib.h>

#include "Cell.h"


static void Cell_Init( Cell *This, int score, int i, int j, Cell *previous );



Cell* New_Cell( int score, int i, int j, Cell *previous )
{
	Cell *This = malloc( sizeof( Cell ) );
	
	if( This == NULL )
	{
		return NULL;
	}
	
	Cell_Init( This, score, i, j, previous );
		
	return ( This );
}


void Cell_Free( Cell *This )
{
	free( This );
}


void Cell_Update( Cell *This )
{

}


static void Cell_Init( Cell *This, int score, int i, int j, Cell *previous )
{	
	This->score = score;
	This->i = i;
	This->j = j;
	This->previous = previous;
}
