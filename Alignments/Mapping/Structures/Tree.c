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

#include "Utils.h"
#include "Tree.h"


Node* New_Node( char data )
{
	Node *This = malloc( sizeof( Node ) );
	
	if ( This == NULL )
	{
		return NULL;
	}
	
	Node_Init( This, data );
	
	return This;
}


static int Node_Init( Node *This, char data )
{
	int *positions = (int*) malloc( sizeof( int ) );
	
	Node **children = (Node**) malloc( sizeof( Node* ) );
	
	if ( positions == NULL )
	{
		return NODE_INIT_ERROR;
	}
	
	if ( children == NULL )
	{
		return NODE_INIT_ERROR;
	}
	
	This->data = data;
	This->nchild = 0;
	This->npos = 0;
	This->positions = positions;
	This->children = children;

	return 0;
}


void Node_Free( Node *This )
{
	int i;
	
	if ( This )
	{
		for ( i=0; i<This->nchild; i++ )
		{
			Node_Free( This->children[ i ] );
		}	
	}
	
	free( This->positions );
	free( This->children );	
	
	free( This );
}


int Node_AddChild( Node *This, Node *child )
{
	This->children = (Node**) realloc( This->children, ( This->nchild + 1 ) * sizeof( Node* ) );

	if ( This->children == NULL )
	{
		return CHILDREN_REALLOC_ERROR;
	}
	
	This->children[ This->nchild ] = child;

	This->nchild++;
	
	return 0;
}


Node* Node_GetChild( Node *This, char data )
{
	int i;
	
	for ( i = 0; i < This->nchild; i++ )
	{
		if ( This->children[ i ]->data == data )
		{
			return This->children[ i ];
		}
	}
	
	return NULL;
}


int Node_HasChildren( Node *This )
{
	if ( This->nchild == 0 )
	{
		return 0;
	}
	
	return 1;
}


int Node_AddPosition( Node *This, int position )
{
	This->positions = (int*) realloc( This->positions, ( This->npos + 1 ) * sizeof( int ) );
	
	if ( This->positions == NULL )
	{
		return POSITIONS_REALLOC_ERROR;	
	}
	
	This->positions[ This->npos ] = position;
	
	This->npos++;
	
	return 0;
}


int Node_Build( Node *This, char *targetSequence, int depth, int* readLengths )
{
	register int i;
	register int cur_depth;
	register int k;
	register int seqLength;
	int num_readlengths;
	char *tmp_read;
	
	depth += 1 ;
	
	tmp_read = (char*) malloc( ( depth ) * sizeof( char ) );
	
	if ( tmp_read == NULL )
	{
		return MER_ALLOC_ERROR;
	}
	
	num_readlengths = intArrayLength( readLengths );
	
	seqLength = strlen( targetSequence ) - depth + 1; 
	
	Node *node = This;
	Node *tmp_node = NULL;
	Node *chi_node = NULL;
	
	for ( i = 0; i < seqLength ; i++ )
	{
		strncpy( tmp_read, targetSequence + i, depth);
		
		for ( cur_depth = 0; cur_depth < depth; cur_depth++ )
		{
			if ( ( chi_node = Node_GetChild( node, tmp_read[ cur_depth ] ) ) != NULL )
			{
				for ( k = 0; k < num_readlengths; k++ )
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
				
				for ( k = 0; k < num_readlengths; k++ )
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
	
	return 0;
}


int* Node_GetPositions( Node *This, char *read, int verbose )
{
	register int i;
	
	Node *tmp_node = This;
	
	for ( i = 0; i < strlen( read ); i++ )
	{
		tmp_node = Node_GetChild( tmp_node, read[ i ] );
		
		if ( tmp_node == NULL )
		{
			return NULL;
		}
	}
	
	if ( verbose )
	{
		for ( i = 0; i < tmp_node->npos; i++ )
		{
			printf( "%i\n", tmp_node->positions[ i ] );
		}
	}
	
	return tmp_node->positions;
}
