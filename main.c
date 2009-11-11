#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"
#include "Sequence.h"
#include "Cell.h"
#include "Matrix.h"
#include "Tree.h"


int main (int argc, const char * argv[]) {
	
/*
tggatggagaggtaaatgcctacgcgatttgcgtgatagagcggatgcgcgttgagtttgtcgttaaggatgggattaaaacggtcaggactgtttgat
*/
//	char *s = "tggatggagaggtaaatgcctacgcgatttgcgtgatagagcggatgcgcgttgagtttgtcgttaaggatgggattaaaacggtcaggactgtttgat";
	Sequence *s = New_Sequence( NULL );
	
	s->sequence = s->Random( s, 2000000 );
	
	printf( "sequence genereated\n" );
	
	Node *root = New_Node( '0' );
	
	printf( "Size of a NODE : %ld\n", sizeof( *root ) );
	
	int readLengths[] = { 18, 19, 20 };
	
	Node_Build( root, s->sequence, 20, readLengths );
	
	printf("Tree built\n");
	
//	Node_PrintIndexes( Node_GetReadIndexes( root, "ttt" ) );

//	getchar();
	
	Node_Free( root );	
	
	s->Free( s );
	
    return 0;
}
