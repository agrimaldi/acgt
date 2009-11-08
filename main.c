#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Sequence.h"
#include "Cell.h"
#include "Matrix.h"
#include "Tree.h"


int main (int argc, const char * argv[]) {
	
/*
tggatggagaggtaaatgcctacgcgatttgcgtgatagagcggatgcgcgttgagtttgtcgttaaggatgggattaaaacggtcaggactgtttgat
*/
//	char *s = "tggatggagaggtaaatgcctcagtacgatcgatcgactagctagctacgatcgtacgtactactatatatattcggcggcgactcatccgctagctacgtacgactgcaatatataaatgactacgacgcgatttgcgtgatagagcggatgcgcgttgagtttgtcgttaaggatgggattaaaacggtcaggactgtttgat";

	Sequence *s = New_Sequence( NULL );
	
	s->sequence = s->Random( s, 500000 );

	printf("sequence genereated\n");
	
	Node *root = New_Node( '0', NULL );

	root->Build( root, 20, s->sequence );
	
	printf("Tree built\n");
	
	root->GetPositions( root, "ttcgaa", 1 );
	
	getchar();

	root->Free( root );	

	s->Free( s );
	
    return 0;
}
