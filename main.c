#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Utils.h"
#include "Sequence.h"
#include "Cell.h"
#include "Matrix.h"
#include "Tree.h"


int main (int argc, const char * argv[]) {
	
	clock_t start, end;
	double elapsed;
	
	int task;
	int depth;
	int *readLengths;
	char *s;
	Sequence *ss;
	
/*
tggatggagaggtaaatgcctacgcgatttgcgtgatagagcggatgcgcgttgagtttgtcgttaaggatgggattaaaacggtcaggactgtttgat
*/

	if (argc > 4)
	{
		return ( -1 );
	}
	else if ( argc == 1 || ( argc == 2 && strcmp( argv[1], "consistency" ) == 0 ) )
	{
		task = 1;
	}
	else if ( strcmp( argv[1], "performance" ) == 0 )
	{
		if ( argc == 2 || ( argc == 3 && strcmp( argv[ 2 ], "speed" ) == 0 ) )
		{
			task = 3;
		}
		else if ( argc == 3 && strcmp( argv[ 2 ], "memory" ) == 0 )
		{
			task = 2;
		}
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
		
		int tmp_readL[] = { 18, 19, 20 };
		readLengths = tmp_readL;
		
		depth = 20;
	}
	
	
	
	
	
	Node *root = New_Node( '0' );
	
	printf( "Size of a NODE : %ld\n", sizeof( *root ) );
	
	if (task == 3)
		start = clock();
	
	Node_Build( root, s, depth, readLengths );
	
	if (task == 3)
		end = clock();
	
	printf("Tree built\n");
	
	
	
	
	
	
	if (task == 1)
	{
		Node_PrintIndexes( Node_GetReadIndexes( root, "ttt" ) );
		printf("\n");
		Node_PrintIndexes( Node_GetReadIndexes( root, "ttga" ) );
	}
	
	if (task == 3)
	{
		elapsed = ((double) (end - start)) / CLOCKS_PER_SEC;
		printf( "TIME : %f", elapsed );
	}
	printf("%i\n", task);
	if (task == 2)
	{
		getchar();
	}
	
	Node_Free( root );	

	if (task == 2 || task == 3 )
	{
		Sequence_Free( ss );
	}
	
    return 0;
}
