/*
 *  Sequence.h
 *  SeqAlign
 *
 *  Created by sopo on 01/11/09.
 *  Copyright 2009 UPMC. All rights reserved.
 *
 */

#ifndef SEQUENCE_H
#define SEQUENCE_H


typedef struct Sequence Sequence;


struct Sequence
{
	/* Attributes */
	char *sequence;
	Sequence *complement;
	int dynalloc;
};


Sequence* New_Sequence( char *sequence );


void Sequence_Free(		Sequence	*This );

void Sequence_View(		Sequence	*This );

char* Sequence_Random(	Sequence	*This,
						int			size );


#endif
