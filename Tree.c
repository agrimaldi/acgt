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

#include "Tree.h"


Node* New_Node( char data, Node *parent )
{
	Node *This = malloc( sizeof( Node ) );
	
	if ( !This )
	{
		printf( "Tree Failed to initialize" );
		return NULL;
	}
	
	Node_Init( This, data, parent );
	
	This->Free = Node_Free;
	
	return This;
}


static int Node_Init( Node *This, char data, Node *parent )
{
	int *positions = (int*) malloc( sizeof( int ) );
	
	Node **children = (Node**) malloc( sizeof( Node* ) );
	
	if ( !positions )
	{
		printf( "Positions Failed to initialize" );
		return NODE_INIT_ERROR;
	}
	
	if ( !children )
	{
		printf( "Children Failed to initialize" );
		return NODE_INIT_ERROR;
	}
	
	This->data = data;
	This->nchild = 0;
	This->npos = 0;
	This->positions = positions;
	This->parent = parent;
	This->children = children;
	
	This->AddChild = Node_AddChild;
	This->GetChild = Node_GetChild;
	This->HasChildren = Node_HasChildren;
	This->AddPosition = Node_AddPosition;
	This->Build = Node_Build;
	This->GetPositions = Node_GetPositions;
	
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

	if ( This->children != NULL )
	{
		This->children[ This->nchild ] = child;

		This->children[ This->nchild ]->parent = This;

		This->nchild++;
	}
	else
	{
		return CHILDREN_REALLOC_ERROR;
	}

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
	if ( This->nchild > 0 )
	{
		return 1;
	}
	
	return 0;
}


int Node_AddPosition( Node *This, int position )
{
	This->positions = (int*) realloc( This->positions, ( This->npos + 1 ) * sizeof( int ) );
	
	if ( This->positions != NULL )
	{
		This->positions[ This->npos ] = position;
		
		This->npos++;
	}
	else
	{
		return POSITIONS_REALLOC_ERROR;
	}
	
	return 0;
}


void Node_Build( Node *This, int merLength, char *targetSequence )
{
	register int i;
	register int j;
	register int seqLength;
	char *mer;
	
	mer = (char*) malloc( ( merLength + 1 ) * sizeof( char ) );
	
	seqLength = strlen( targetSequence ) - merLength + 1; 
	
	Node *node = This;
	Node *tmp_node = NULL;
	
	for ( i = 0; i < seqLength ; i++ )
	{
		strncpy( mer, targetSequence + i, merLength);
		
		for ( j = 0; j < merLength; j++ )
		{
			if ( node->GetChild( node, mer[ j ] ) == NULL )
			{
				tmp_node = New_Node( mer[ j ], NULL );
				
				node->AddChild( node, tmp_node );
				
				node->AddPosition( node, i );
				
				node = tmp_node;
			}
			else
			{
				node->AddPosition( node, i );
				
				node = node->GetChild( node, mer[ j ] );
			}
		}
		
		node = This;
	}
	
	free( mer );
}


int* Node_GetPositions( Node *This, char *mer, int verbose )
{
	register int i;
	
	Node *tmp_node = This;
	
	for ( i = 0; i < strlen( mer ); i++ )
	{
		if ( ( tmp_node = tmp_node->GetChild( tmp_node, mer[ i ] ) ) == NULL )
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
