#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define MATRIX_LENGTH 4

#define MIN 1
#define MAX 10

int main()
{
	srand(time(NULL));

	int** matrix;
	matrix = (int**) malloc(sizeof(int)*MATRIX_LENGTH);
	for (int i = 0; i < MATRIX_LENGTH; ++i)
	{
		matrix[i] = (int*) malloc(sizeof(int)*MATRIX_LENGTH);
	}

	RandomMatrix(matrix, MATRIX_LENGTH, MIN, MAX);
	InitializeShortestPathMatrix(matrix, MATRIX_LENGTH);

	printf("Initial Matrix:\n");
	PrintMatrix(matrix, MATRIX_LENGTH);

	for (int k = 0; k < MATRIX_LENGTH; ++k)
	{
		for (int i = 0; i < MATRIX_LENGTH; ++i)
		{
			for (int j = 0; j < MATRIX_LENGTH; ++j)
			{
				if (matrix[i][j] > matrix[i][k] + matrix[k][j])
				{
					matrix[i][j] = matrix[i][k] + matrix[k][j];
				}
			}
		}
	}

	printf("Final Matrix:\n");
	PrintMatrix(matrix, MATRIX_LENGTH);

	// Does not work for some reason:
	// free(matrix[0]);
	free(matrix[1]);
	free(matrix[2]);
	free(matrix[3]);
	free(matrix);

	return 0;
}