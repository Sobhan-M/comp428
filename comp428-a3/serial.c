#include "functions.h"
#include <stdio.h>


#define MATRIX_LENGTH 4

#define MIN 1
#define MAX 10

int main()
{
	int** matrix = malloc(sizeof(int)*MATRIX_LENGTH);
	for (int i = 0; i < MATRIX_LENGTH; ++i)
	{
		matrix[i] = malloc(sizeof(int)*MATRIX_LENGTH);
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
				if (matrix[i][j] > matrix[i][k] + matrix[k][i])
				{
					matrix[i][j] = matrix[i][k] + matrix[k][i];
				}
			}
		}
	}

	printf("Final Matrix:\n");
	PrintMatrix(matrix, MATRIX_LENGTH);

	return 0;
}