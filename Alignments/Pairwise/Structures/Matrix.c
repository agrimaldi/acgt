/*
 *  Matrix.c
 *  SeqAlign
 *
 *  Created by sopo on 22/10/09.
 *  Copyright 2009 UPMC. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>

#include "Cell.h"
#include "Matrix.h"


static void Matrix_Init( Matrix *This, int rows, int cols, int initGapCost );



Matrix* New_Matrix( int nrows, int ncols, int initGapCost )
{
	Matrix *This = malloc( sizeof( Matrix ) );
	
	if ( This == NULL )
	{
		return ( NULL );
	}
	
	Matrix_Init( This, nrows, ncols, initGapCost );
		
	return ( This );
}



void Matrix_Free( Matrix *This )
{
	if ( This )
	{
		Matrix_Clear( This );
	}
	
	free( This->cells );
	free( This );
}


void Matrix_Clear( Matrix *This )
{
	register int i;
	register int j;
	
	i = This->rows - 1;
	j = This->cols - 1;
	
	for ( i = This->rows - 1; i >= 0; --i )
	{
		for ( j = This->cols - 1; j >= 0; --j )
		{
			Cell_Free( This->cells[ i ][ j ] );
		}
		
		free( This->cells[ i ] );
	}
}


void Matrix_View( Matrix *This )
{
	register int i;
	register int j;
	
	for ( i = 0; i < This->rows; ++i )
	{
		for ( j = 0; j < This->cols; ++j )
		{
			printf("(%i, %i) : %i ", This->cells[i][j]->i, This->cells[i][j]->j, This->cells[i][j]->score );
		}
		printf("\n");
	}
}


static void Matrix_Init( Matrix *This, int nrows, int ncols, int initGapCost )
{
	register int i;
	register int j;
	
	Cell ***rows = malloc( nrows * sizeof( Cell** ) );
	
	if( rows != NULL )
	{
		for ( i = 0; i < nrows; ++i )
		{
			if ( ( rows[ i ] = malloc( ncols * sizeof( Cell* ) ) ) != NULL )
			{
				for ( j = 0; j < ncols; ++j )
				{
					if ( i == 0 && j == 0 )
					{
						rows[ i ][ j ] = New_Cell( 0, i, j, NULL );
					}
					else if ( i == 0 )
					{
						rows[ i ][ j ] = New_Cell( j * initGapCost, i, j, rows[ i ][ j - 1 ] );
					}
					else if ( j == 0 )
					{
						rows[ i ][ j ] = New_Cell( i * initGapCost, i, j, rows[ i - 1 ][ j ] );
					}
					else
					{
						rows[ i ][ j ] = New_Cell( 0, i, j, NULL );
					}
				}
			}
			else
			{
				return;
			}
		}
	}
	else
	{
		return;
	}
	
	This->cells = rows;
	This->rows = nrows;
	This->cols = ncols;
}
