/*
 *  Tree.c
 *  SeqAlign
 *
 *  Created by sopo on 07/11/09.
 *  Copyright 2009 UPMC. All rights reserved.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include "Utils.h"
#include "Tree.h"


static int
Node_Init(			Node	*This,
					char	data );



Node*
New_Node(			char	data )
{
	Node *This = malloc( sizeof( Node ) );
	
	if ( This == NULL )
	{
		return ( NULL );
	}
	
	Node_Init( This, data );
	
	return ( This );
}



void
Node_Free(			Node	*This )
{
	int i;
	
	if ( This )
	{
		for ( i = 0; i < This->nchild; ++i )
		{
			Node_Free( This->children[ i ] );
		}	
	}
	
	free( This->positions );
	free( This->children );	
	
	free( This );
}



int
Node_AddChild(		Node	*This,
					Node	*child )
{
	This->children = realloc( This->children, ( This->nchild + 1 ) * sizeof( Node* ) );

	if ( This->children == NULL )
	{
		return ( CHILDREN_REALLOC_ERROR );
	}
	
	This->children[ (This->nchild)++ ] = child;
	
	return ( 0 );
}



Node*
Node_GetChild(		Node	*This,
					char	data )
{
	register int i;
	
	for ( i = 0; i < This->nchild; ++i )
	{
		if ( This->children[ i ]->data == data )
		{
			return ( This->children[ i ] );
		}
	}
	
	return ( NULL );
}



int
Node_HasChildren(	Node	*This )
{	
	return ( ( This->nchild == 0 ) ? false : true );
}



int
Node_AddPosition(	Node	*This,
					int		position )
{
	This->positions = realloc( This->positions, ( This->npos + 1 ) * sizeof( int ) );
	
	if ( This->positions == NULL )
	{
		return ( POSITIONS_REALLOC_ERROR );	
	}
	
	This->positions[ (This->npos)++ ] = position;
	
	return ( 0 );
}



int
Node_Build(			Node	*This,
					char	*targetSequence,
					int		depth,
					int		*readLengths )
{
	register int i;
	register int cur_depth;
	register int k;
	register int seqLength;
	int num_readlengths;
	char *tmp_read;
	
	if ( ( tmp_read = malloc( ( ++depth ) * sizeof( char ) ) ) == NULL )
	{
		return ( MER_ALLOC_ERROR );
	}
	
	num_readlengths = intArrayLength( readLengths );
	
	seqLength = strlen( targetSequence ) - depth + 1; 
	
	Node *node = This;
	Node *tmp_node = NULL;
	Node *chi_node = NULL;
	
	for ( i = 0; i < seqLength ; ++i )
	{
		strncpy( tmp_read, targetSequence + i, depth);
		
		for ( cur_depth = 0; cur_depth < depth; ++cur_depth )
		{
			if ( ( chi_node = Node_GetChild( node, tmp_read[ cur_depth ] ) ) != NULL )
			{
				for ( k = 0; k < num_readlengths; ++k )
				{
					if ( readLengths[ k ] == cur_depth )
					{
						Node_AddPosition( node, i );
						break;
					}
				}

				node = chi_node;
			}				
			else
			{
				tmp_node = New_Node( tmp_read[ cur_depth ] );
				
				Node_AddChild( node, tmp_node );
				
				for ( k = 0; k < num_readlengths; ++k )
				{
					if ( readLengths[ k ] == cur_depth )
					{
						Node_AddPosition( node, i );
						break;
					}
				}
				
				node = tmp_node;
			}
		}
		
		node = This;
	}
	
	free( tmp_read );
	
	return ( 0 );
}



Node*
Node_GetReadIndexes(	Node	*This,
						char	*read )
{
	register int i;
	
	Node *tmp_node = This;
	
	for ( i = 0; i < strlen( read ); ++i )
	{
		if ( ( tmp_node = Node_GetChild( tmp_node, read[ i ] ) ) == NULL )
		{
			return ( NULL );
		}
	}
	
	return ( tmp_node );
}



void
Node_PrintIndexes(		Node	*This )
{
	register int i;
	
	for ( i = 0; i < This->npos; ++i )
	{
		printf( "%i\n", This->positions[ i ] );
	}
}



static int
Node_Init(				Node	*This,
						char	data )
{
	int *positions = malloc( sizeof( int ) );
	
	Node **children = malloc( sizeof( Node* ) );
	
	if ( positions == NULL || children == NULL )
	{
		return ( NODE_INIT_ERROR );
	}
	
	This->data = data;
	This->nchild = 0;
	This->npos = 0;
	This->positions = positions;
	This->children = children;
	
	return ( 0 );
}
