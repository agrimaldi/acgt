#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "Cell.h"
#include "Matrix.h"
#include "Tree.h"

char* randseq( int size )
{
	register int r, i;
	char *seq = (char*) malloc( sizeof( char ) * size );
	
	srand( time( NULL ) );
	
	for ( i = 0; i < size - 1; i++ )
	{
		r = rand() % 4;
		
		if ( r == 0 )
		{
			seq[ i ] = 'a';
		}
		else if ( r == 1 )
		{
			seq[ i ] = 'c';
		}
		else if ( r == 2 )
		{
			seq[ i ] = 'g';
		}
		else
		{
			seq[ i ] = 't';
		}
		
		seq[ size ] = '\0';
	}
	
	return seq;
}


int main (int argc, const char * argv[]) {
	
	char *s = randseq( 10000000 );
//	char *s = "tggatggagaggtaaatgcctacgcgatttgcgtgatagagcggatgcgcgttgagtttgtcgttaaggatgggattaaaacggtcaggactgtttgat";
/*
tggatggagaggtaaatgcctacgcgatttgcgtgatagagcggatgcgcgttgagtttgtcgttaaggatgggattaaaacggtcaggactgtttgat
*/
	printf("sequence genereated\n");
//	printf("%s\n", s);

	Node *root = New_Node( '0', NULL );

	root->Build( root, 5, s );
	
	printf("Tree built\n");
	
//	root->GetPositions( root, "ttcgaa", 1 );
	
	getchar();
	
	root->Free( root );
	
	free(s);
	
    return 0;
}
