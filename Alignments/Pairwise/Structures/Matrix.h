/*
 *  Matrix.h
 *  SeqAlign
 *
 *  Created by sopo on 22/10/09.
 *  Copyright 2009 UPMC. All rights reserved.
 *
 */

#ifndef MATRIX_H
#define MATRIX_H


typedef struct Matrix Matrix;


struct Matrix
{
	/* Attributes */
	int score;
	int rows;
	int cols;
	Cell *backtrack;
	Cell ***cells;	
};


Matrix* New_Matrix(		int		rows,
						int		cols,
						int		initGapCost );


void Matrix_Free(		Matrix	*This );


void Matrix_Clear(		Matrix	*This );


void Matrix_View(		Matrix	*This );


void Matrix_Fill(		Matrix	*This,
						char	*seq1,
						char	*seq2 );


void Matrix_Backtrack(	Matrix	*This );


#endif
