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

#include <dispatch/dispatch.h>

#include "Utils.h"
#include "Tree.h"


static int
Node_Init( Node *This, char data );



Node* 
New_Node( char data )
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
Node_Free( Node *This )
{
	dispatch_group_t group = dispatch_group_create();
	
	dispatch_queue_t acQueue = dispatch_queue_create("com.TreeMap.Free.a", NULL);
	dispatch_queue_t gtQueue = dispatch_queue_create("com.TreeMap.Free.g", NULL);
	
	dispatch_block_t blockA = ^
	{
		Node_nFree( Node_GetChild( This, 'a' ) );
	};
	
	dispatch_block_t blockC = ^
	{
		Node_nFree( Node_GetChild( This, 'c' ) );
	};
	
	dispatch_block_t blockG = ^
	{
		Node_nFree( Node_GetChild( This, 'g' ) );
	};
	
	dispatch_block_t blockT = ^
	{
		Node_nFree( Node_GetChild( This, 't' ) );
	};
	
	dispatch_group_async(group, acQueue, blockA);
	dispatch_group_async(group, acQueue, blockC);
	dispatch_group_async(group, gtQueue, blockG);
	dispatch_group_async(group, gtQueue, blockT);
	
	dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
	
	dispatch_release(group);	
	
	free( This->positions );
	free( This->children );	
	
	free( This );
}

	
void
Node_nFree( Node *This )
{
	register unsigned int i;
	
	if ( This )
	{
		for ( i = 0; i < This->nchild; ++i )
		{
			Node_nFree( This->children[ i ] );
		}	
	
		free( This->positions );
		free( This->children );	
	}
	
	
	free( This );
}



int
Node_AddChild( Node *This, Node *child )
{
	This->children = realloc( This->children, ( This->nchild + 1 ) * sizeof( Node* ) );

	if ( This->children == NULL )
	{
		exit( CHILDREN_REALLOC_ERROR );
	}
	
	This->children[ (This->nchild)++ ] = child;
	
	return ( 0 );
}



Node*
Node_GetChild( Node *This, char data )
{
	register unsigned int i;
	
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
Node_HasChildren( Node *This )
{	
	return ( ( This->nchild == 0 ) ? false : true );
}



int
Node_AddPosition( Node *This, unsigned int position )
{
	This->positions = realloc( This->positions, ( This->npos + 1 ) * sizeof( int ) );
	
	if ( This->positions == NULL )
	{
		exit( POSITIONS_REALLOC_ERROR );	
	}
	
	This->positions[ (This->npos)++ ] = position;
	
	return ( 0 );
}



int
Node_Build( Node *This, char *targetSequence, unsigned int depth, unsigned int *readLengths )
{	

	dispatch_group_t group = dispatch_group_create();
	
	dispatch_queue_t aQueue = dispatch_queue_create("com.TreeMap.Build.a", NULL);
	dispatch_queue_t cQueue = dispatch_queue_create("com.TreeMap.Build.c", NULL);
	dispatch_queue_t gQueue = dispatch_queue_create("com.TreeMap.Build.g", NULL);
	dispatch_queue_t tQueue = dispatch_queue_create("com.TreeMap.Build.t", NULL);

	
	register unsigned int seqLength;
	register unsigned int num_readlengths;

	modifValues( readLengths, -1 );
	num_readlengths = intArrayLength( readLengths );	
	seqLength = strlen( targetSequence ) - depth + 1;
	
	
	register unsigned int i;
	
	char *atmp_read;
	char *ctmp_read;
	char *gtmp_read;
	char *ttmp_read;
	
	if ( ( atmp_read = malloc( ( depth + 1 ) * sizeof( char ) ) ) == NULL )
		return READ_ALLOC_ERROR;
	
	if ( ( ctmp_read = malloc( ( depth + 1 ) * sizeof( char ) ) ) == NULL )
		return READ_ALLOC_ERROR;
	
	if ( ( gtmp_read = malloc( ( depth + 1 ) * sizeof( char ) ) ) == NULL )
		return READ_ALLOC_ERROR;
	
	if ( ( ttmp_read = malloc( ( depth + 1 ) * sizeof( char ) ) ) == NULL )
		return READ_ALLOC_ERROR;
	
	
	__block Node *root = This;

	__block Node *anode = root;
	__block Node *atmp_node = NULL;
	__block Node *achi_node = NULL;
	
	__block register unsigned int acur_depth;
	__block register unsigned int ak;
	
	__block Node *cnode = root;
	__block Node *ctmp_node = NULL;
	__block Node *cchi_node = NULL;
	
	__block register unsigned int ccur_depth;
	__block register unsigned int ck;
	
	__block Node *gnode = root;
	__block Node *gtmp_node = NULL;
	__block Node *gchi_node = NULL;
	
	__block register unsigned int gcur_depth;
	__block register unsigned int gk;
	
	__block Node *tnode = root;
	__block Node *ttmp_node = NULL;
	__block Node *tchi_node = NULL;
	
	__block register unsigned int tcur_depth;
	__block register unsigned int tk;
	
	
	for ( i = 0; i < seqLength ; ++i )
	{
		if ( targetSequence[ i ] == 'a' )
		{
			dispatch_group_async(group, aQueue, ^
			{
				strncpy( atmp_read, targetSequence + i, depth );
				atmp_read[ depth ] = '\0';
				
				for ( acur_depth = 0; acur_depth < depth; ++acur_depth )
				{
					if ( ( achi_node = Node_GetChild( anode, atmp_read[ acur_depth ] ) ) != NULL )
					{
						if ( readLengths[ 0 ] <= acur_depth )
						{
							for ( ak = num_readlengths; ak--; )
							{
								if ( readLengths[ ak ] == acur_depth )
								{
									Node_AddPosition( achi_node, i );
									break;
								}
							}
						}
						
						anode = achi_node;
					}				
					else
					{
						atmp_node = New_Node( atmp_read[ acur_depth ] );
						
						if ( readLengths[ 0 ] <= acur_depth )
						{
							for ( ak = num_readlengths; ak--; )
							{
								if ( readLengths[ ak ] == acur_depth )
								{
									Node_AddPosition( atmp_node, i );
									break;
								}
							}
						}
						
						Node_AddChild( anode, atmp_node );
						
						anode = atmp_node;
					}
				}
				
				anode = root;
			});
		}
		else if ( targetSequence[ i ] == 'c' )
		{
			dispatch_group_async(group, cQueue, ^
			{
				strncpy( ctmp_read, targetSequence + i, depth );
				ctmp_read[ depth ] = '\0';

				for ( ccur_depth = 0; ccur_depth < depth; ++ccur_depth )
				{
					if ( ( cchi_node = Node_GetChild( cnode, ctmp_read[ ccur_depth ] ) ) != NULL )
					{
						if ( readLengths[ 0 ] <= ccur_depth )
						{
							for ( ck = num_readlengths; ck--; )
							{
								if ( readLengths[ ck ] == ccur_depth )
								{
									Node_AddPosition( cchi_node, i );
									break;
								}
							}
						}
						
						cnode = cchi_node;
					}				
					else
					{
						ctmp_node = New_Node( ctmp_read[ ccur_depth ] );
						
						if ( readLengths[ 0 ] <= ccur_depth )
						{
							for ( ck = num_readlengths; ck--; )
							{
								if ( readLengths[ ck ] == ccur_depth )
								{
									Node_AddPosition( ctmp_node, i );
									break;
								}
							}
						}
						
						Node_AddChild( cnode, ctmp_node );
						
						cnode = ctmp_node;
					}
				}
				
				cnode = root;
			});
		}
		else if ( targetSequence[ i ] == 'g' )
		{
			dispatch_group_async(group, gQueue, ^
			{
				strncpy( gtmp_read, targetSequence + i, depth );
				gtmp_read[ depth ] = '\0';
				 
				for ( gcur_depth = 0; gcur_depth < depth; ++gcur_depth )
				{
					if ( ( gchi_node = Node_GetChild( gnode, gtmp_read[ gcur_depth ] ) ) != NULL )
					{
						if ( readLengths[ 0 ] <= gcur_depth )
						{
							for ( gk = num_readlengths; gk--; )
							{
								if ( readLengths[ gk ] == gcur_depth )
								{
									Node_AddPosition( gchi_node, i );
									break;
								}
							}
						}
						
						gnode = gchi_node;
					}				
					else
					{
						gtmp_node = New_Node( gtmp_read[ gcur_depth ] );
						
						if ( readLengths[ 0 ] <= gcur_depth )
						{
							for ( gk = num_readlengths; gk--; )
							{
								if ( readLengths[ gk ] == gcur_depth )
								{
									Node_AddPosition( gtmp_node, i );
									break;
								}
							}
						}
						
						Node_AddChild( gnode, gtmp_node );
						
						gnode = gtmp_node;
					}
				}
				
				gnode = root;
			});
		}
		else if ( targetSequence[ i ] == 't' )
		{
			dispatch_group_async(group, tQueue, ^
			{
				strncpy( ttmp_read, targetSequence + i, depth );
				ttmp_read[ depth ] = '\0';
				
				for ( tcur_depth = 0; tcur_depth < depth; ++tcur_depth )
				{
					if ( ( tchi_node = Node_GetChild( tnode, ttmp_read[ tcur_depth ] ) ) != NULL )
					{
						if ( readLengths[ 0 ] <= tcur_depth )
						{
							for ( tk = num_readlengths; tk--; )
							{
								if ( readLengths[ tk ] == tcur_depth )
								{
									Node_AddPosition( tchi_node, i );
									break;
								}
							}
						}
						
						tnode = tchi_node;
					}				
					else
					{
						ttmp_node = New_Node( ttmp_read[ tcur_depth ] );
						
						if ( readLengths[ 0 ] <= tcur_depth )
						{
							for ( tk = num_readlengths; tk--; )
							{
								if ( readLengths[ tk ] == tcur_depth )
								{
									Node_AddPosition( ttmp_node, i );
									break;
								}
							}
						}
						
						Node_AddChild( tnode, ttmp_node );
						
						tnode = ttmp_node;
					}
				}
				
				tnode = root;
			});
		}
	}
	
	dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
	
	dispatch_release(group);
	
	free(atmp_read);
	free(ctmp_read);
	free(gtmp_read);
	free(ttmp_read);

	return ( 0 );
}



Node*
Node_GetReadIndexes( Node *This, char *read )
{
	register unsigned int i;
	
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
Node_PrintIndexes( Node *This )
{
	register unsigned int i;
	
	for ( i = 0; i < This->npos; ++i )
	{
		printf( "%i\n", This->positions[ i ] );
	}
}



static int
Node_Init( Node *This, char data )
{
	unsigned int *positions = malloc( sizeof( int ) );
	
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
