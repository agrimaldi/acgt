/*
 *  Tree.h
 *  SeqAlign
 *
 *  Created by sopo on 07/11/09.
 *  Copyright 2009 UPMC. All rights reserved.
 *
 */

#ifndef TREE_H
#define TREE_H

#define NODE_INIT_ERROR -1
#define CHILDREN_REALLOC_ERROR 1
#define POSITIONS_REALLOC_ERROR 2
#define READ_ALLOC_ERROR 3


typedef struct Node Node;


struct Node
{
	Node **children;
	unsigned int *positions;
	unsigned int nchild;
	unsigned int npos;
	char data;
};


Node*
New_Node(					char			data );


void
Node_Free(					Node			*This );


void
Node_nFree(					Node			*This );


int
Node_AddChild(				Node			*This,
							Node			*child );


Node*
Node_GetChild(				Node			*This,
							char			data );


int
Node_HasChildren(			Node			*This );


int
Node_AddPosition(			Node			*This,
							unsigned int	position );


int
Node_Build(					Node			*This,
							char			*targetSequence,
							unsigned int	depth,
							unsigned int	*readLengths );


Node*
Node_GetReadIndexes(		Node			*This,
							char			*read );


void
Node_PrintIndexes(			Node	*This );


#endif
