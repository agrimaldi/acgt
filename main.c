#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "Utils.h"
#include "Sequence.h"
#include "Cell.h"
#include "Matrix.h"
#include "Tree.h"


int main (int argc, const char * argv[]) {
	
	int task;
	int depth;
	int *readLengths;
	char *s;
	Sequence *ss;
	
/*
tggatggagaggtaaatgcctacgcgatttgcgtgatagagcggatgcgcgttgagtttgtcgttaaggatgggattaaaacggtcaggactgtttgat
*/

	if (argc > 3)
	{
		return ( -1 );
	}
	else if ( argc == 1 || ( argc == 2 && strcmp( argv[1], "consistency" ) == 0 ) )
	{
		task = 1;
	}
	else if ( argc == 2 && strcmp( argv[1], "performance" ) == 0 )
	{
		task = 2;
	}
	
	if (task == 1)
	{
		s = "tggatggagaggtaaatgcctacgcgatttgcgtgatagagcggatgcgcgttgagtttgtcgttaaggatgggattaaaacggtcaggactgtttgat";
		
		int tmp_readL[] = { 3, 4 };
		readLengths = tmp_readL;
		
		depth = 4;
	}
	else
	{
		ss = New_Sequence( NULL );
		
		ss->sequence = Sequence_Random( ss, 1000000 );
		
		s = ss->sequence;
		
		printf( "sequence genereated\n" );
		
		depth = 20;
	}

	
	Node *root = New_Node( '0' );
	
	printf( "Size of a NODE : %ld\n", sizeof( *root ) );
	
	
	Node_Build( root, s, depth, readLengths );
	
	printf("Tree built\n");
	
	
	if (task == 1)
	{
		Node_PrintIndexes( Node_GetReadIndexes( root, "ttt" ) );
	}

//	getchar();
	
	Node_Free( root );	

	if (task == 2)
	{
		Sequence_Free( ss );
	}
	
    return 0;
}
