/*
 *  Cell.h
 *  SeqAlign
 *
 *  Created by sopo on 21/10/09.
 *  Copyright 2009 UPMC. All rights reserved.
 *
 */

#ifndef DEF_CELL
#define DEF_CELL


typedef struct Cell Cell;


struct Cell
{
	/* Attributes */
	int i;
	int j;
	int score;
	Cell *previous;
};


Cell* New_Cell(		int		score,
					int		i,
					int		j,
					Cell	*previous );


void Cell_Free(		Cell* );


void Cell_Update(	Cell* );



#endif
