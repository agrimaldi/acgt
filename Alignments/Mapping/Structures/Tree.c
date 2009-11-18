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
	// Workers baseed tree building.
	
	unsigned int seqLength;
	unsigned int num_readlengths;
	
	num_readlengths = intArrayLength( readLengths );

	seqLength = strlen( targetSequence ) - depth + 1;
	
	modifValues( readLengths, -1 );
	
	dispatch_group_t group = dispatch_group_create();
	
	dispatch_queue_t aQueue = dispatch_queue_create("com.TreeMap.Build.a", NULL);
//	dispatch_queue_t cQueue = dispatch_queue_create("com.TreeMap.Build.c", NULL);
	dispatch_queue_t gQueue = dispatch_queue_create("com.TreeMap.Build.g", NULL);
//	dispatch_queue_t tQueue = dispatch_queue_create("com.TreeMap.Build.t", NULL);
	
	dispatch_block_t blockA = ^
	{
		register unsigned int i;
		register unsigned int cur_depth;
		register unsigned int k;
		char *tmp_read;
		
		if ( ( tmp_read = malloc( ( depth + 1 ) * sizeof( char ) ) ) == NULL )
			return; 
		
		Node *node = This;
		Node *tmp_node = NULL;
		Node *chi_node = NULL;
		
		for ( i = 0; i < seqLength ; ++i )
		{
			if ( targetSequence[ i ] == 'a' || targetSequence[ i ] == 'c' )
			{
				strncpy( tmp_read, targetSequence + i, depth );
				tmp_read[ depth ] = '\0';
				
				for ( cur_depth = 0; cur_depth < depth; ++cur_depth )
				{
					if ( ( chi_node = Node_GetChild( node, tmp_read[ cur_depth ] ) ) != NULL )
					{
						if ( readLengths[ 0 ] <= cur_depth )
						{
							for ( k = num_readlengths; k--; )
							{
								if ( readLengths[ k ] == cur_depth )
								{
									Node_AddPosition( chi_node, i );
									break;
								}
							}
						}
						
						node = chi_node;
					}				
					else
					{
						tmp_node = New_Node( tmp_read[ cur_depth ] );
						
						if ( readLengths[ 0 ] <= cur_depth )
						{
							for ( k = num_readlengths; k--; )
							{
								if ( readLengths[ k ] == cur_depth )
								{
									Node_AddPosition( tmp_node, i );
									break;
								}
							}
						}
						
						Node_AddChild( node, tmp_node );
						
						node = tmp_node;
					}
				}
				
				node = This;
			}
		}
			
		free( tmp_read );
	};
	
//	dispatch_block_t blockC = ^
//	{
//		register int i;
//		register int cur_depth;
//		register int k;
//		char *tmp_read;
//		
//		if ( ( tmp_read = malloc( ( depth + 1 ) * sizeof( char ) ) ) == NULL )
//			return; 
//		
//		Node *node = This;
//		Node *tmp_node = NULL;
//		Node *chi_node = NULL;
//		
//		for ( i = 0; i < seqLength ; ++i )
//		{
//			if ( targetSequence[ i ] == 'c' )
//			{
//				strncpy( tmp_read, targetSequence + i, depth );
//				tmp_read[ depth ] = '\0';
//				
//				for ( cur_depth = 0; cur_depth < depth; ++cur_depth )
//				{
//					if ( ( chi_node = Node_GetChild( node, tmp_read[ cur_depth ] ) ) != NULL )
//					{
//						for ( k = num_readlengths; k--; )
//						{
//							if ( readLengths[ k ] == cur_depth )
//							{
//								Node_AddPosition( chi_node, i );
//								break;
//							}
//						}
//						
//						node = chi_node;
//					}				
//					else
//					{
//						tmp_node = New_Node( tmp_read[ cur_depth ] );
//						
//						for ( k = num_readlengths; k--; )
//						{
//							if ( readLengths[ k ] == cur_depth )
//							{
//								Node_AddPosition( tmp_node, i );
//								break;
//							}
//						}
//						
//						Node_AddChild( node, tmp_node );
//						
//						node = tmp_node;
//					}
//				}
//				
//				node = This;
//			}
//			
//		}
//		
//		free( tmp_read );
//	};
	
	dispatch_block_t blockG = ^
	{
		register unsigned int i;
		register unsigned int cur_depth;
		register unsigned int k;
		char *tmp_read;

		if ( ( tmp_read = malloc( ( depth + 1 ) * sizeof( char ) ) ) == NULL )
			return; 
		
		Node *node = This;
		Node *tmp_node = NULL;
		Node *chi_node = NULL;
		
		for ( i = 0; i < seqLength ; ++i )
		{
			if ( targetSequence[ i ] == 'g' || targetSequence[ i ] == 't' )
			{
				strncpy( tmp_read, targetSequence + i, depth );
				tmp_read[ depth ] = '\0';
				
				for ( cur_depth = 0; cur_depth < depth; ++cur_depth )
				{
					if ( ( chi_node = Node_GetChild( node, tmp_read[ cur_depth ] ) ) != NULL )
					{
						if ( readLengths[ 0 ] <= cur_depth )
						{
							for ( k = num_readlengths; k--; )
							{
								if ( readLengths[ k ] == cur_depth )
								{
									Node_AddPosition( chi_node, i );
									break;
								}
							}
						}
						
						node = chi_node;
					}				
					else
					{
						tmp_node = New_Node( tmp_read[ cur_depth ] );
						
						if ( readLengths[ 0 ] <= cur_depth )
						{
							for ( k = num_readlengths; k--; )
							{
								if ( readLengths[ k ] == cur_depth )
								{
									Node_AddPosition( tmp_node, i );
									break;
								}
							}
						}
						
						Node_AddChild( node, tmp_node );
						
						node = tmp_node;
					}
				}
				
				node = This;
			}
		}
		
		free( tmp_read );
	};
	
//	dispatch_block_t blockT = ^
//	{
//		register int i;
//		register int cur_depth;
//		register int k;
//		char *tmp_read;
//		
//		if ( ( tmp_read = malloc( ( depth + 1 ) * sizeof( char ) ) ) == NULL )
//			return; 
//
//		Node *node = This;
//		Node *tmp_node = NULL;
//		Node *chi_node = NULL;
//		
//		for ( i = 0; i < seqLength ; ++i )
//		{
//			if ( targetSequence[ i ] == 't' )
//			{
//				strncpy( tmp_read, targetSequence + i, depth );
//				tmp_read[ depth ] = '\0';
//				
//				for ( cur_depth = 0; cur_depth < depth; ++cur_depth )
//				{
//					if ( ( chi_node = Node_GetChild( node, tmp_read[ cur_depth ] ) ) != NULL )
//					{
//						for ( k = num_readlengths; k--; )
//						{
//							if ( readLengths[ k ] == cur_depth )
//							{
//								Node_AddPosition( chi_node, i );
//								break;
//							}
//						}
//						
//						node = chi_node;
//					}				
//					else
//					{
//						tmp_node = New_Node( tmp_read[ cur_depth ] );
//						
//						for ( k = num_readlengths; k--; )
//						{
//							if ( readLengths[ k ] == cur_depth )
//							{
//								Node_AddPosition( tmp_node, i );
//								break;
//							}
//						}
//						
//						Node_AddChild( node, tmp_node );
//						
//						node = tmp_node;
//					}
//				}
//				
//				node = This;
//			}
//		}
//			
//		free( tmp_read );
//	};
	
	dispatch_group_async(group, aQueue, blockA);
//	dispatch_group_async(group, cQueue, blockC);
	dispatch_group_async(group, gQueue, blockG);
//	dispatch_group_async(group, tQueue, blockT);
	
	dispatch_group_wait(group, DISPATCH_TIME_FOREVER);
	
	dispatch_release(group);
	
//	dispatch_sync(aQueue, blockA);
//	dispatch_sync(cQueue, blockC);
//	dispatch_sync(gQueue, blockG);
//	dispatch_sync(tQueue, blockT);
	
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
