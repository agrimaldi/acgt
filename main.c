#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>

#include "Utils.h"
#include "Sequence.h"
#include "Cell.h"
#include "Matrix.h"
#include "Tree.h"


int main (int argc, const char * argv[]) {
	
	struct timeval start, end;
	double elapsed;
	
	unsigned int task;
	unsigned int depth;
	unsigned int *readLengths;
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
		
		unsigned int tmp_readL[] = { 3, 4 };
		readLengths = tmp_readL;
		
		depth = 4;
	}
	
	
	else
	{
		ss = New_Sequence( NULL );
		
		ss->sequence = Sequence_Random( ss, 1500000 );
		
		s = ss->sequence;
		
		printf( "sequence genereated\n" );
		
		unsigned int tmp_readL[] = { 35 };
		readLengths = tmp_readL;
		
		depth = 35;
	}
	
	if (task == 2)
	{
		getchar();
	}
	
	
	
	Node *root = New_Node( '0' );
	
	printf( "Size of a NODE : %ld\n", sizeof( *root ) );
	
	gettimeofday( &start, NULL );
	
	Node_Build( root, s, depth, readLengths );
	
	gettimeofday( &end, NULL );
	
	printf("Tree built\n");
	
	
	
	
	
	
	if (task == 1)
	{
		Node_PrintIndexes( Node_GetReadIndexes( root, "ttt" ) );
		printf("\n");
		Node_PrintIndexes( Node_GetReadIndexes( root, "ttga" ) );
		printf("\n");		
		Node_PrintIndexes( Node_GetReadIndexes( root, "atg" ) );
		printf("\n");
		Node_PrintIndexes( Node_GetReadIndexes( root, "atgg" ) );
		printf("\n");
		Node_PrintIndexes( Node_GetReadIndexes( root, "cgt" ) );
		printf("\n");
	}
	
	printf("%f\n", ((end.tv_sec * 1000000 + end.tv_usec)
					 - (start.tv_sec * 1000000 + start.tv_usec))/1000000.0);

	
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
