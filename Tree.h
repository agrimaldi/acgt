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
#define MER_ALLOC_ERROR 3


typedef struct Node Node;


struct Node
{
	/* Attributes */
	char data;
	int nchild;
	int npos;
	int *positions;
	Node *parent;
	Node **children;
	
	/* Methods */
	void ( *Free ) ( Node* );
	int ( *AddChild ) ( Node*, Node* );
	Node* ( *GetChild ) ( Node*, char );
	int ( *HasChildren ) ( Node* );
	int ( *AddPosition ) ( Node*, int );
	int ( *Build ) ( Node*, int, char* );
	int* ( *GetPositions ) ( Node*, char*, int );
};


Node* New_Node( char data, Node *parent );

static int Node_Init( Node *This, char data, Node *parent );

void Node_Free( Node *This );

int Node_AddChild( Node *This, Node *child );

Node* Node_GetChild( Node *This, char data );

int Node_HasChildren( Node *This );

int Node_AddPosition( Node *This, int position );

int Node_Build( Node *This, int merLength, char *targetSequence );

int* Node_GetPositions( Node *This, char *mer, int verbose );


#endif
