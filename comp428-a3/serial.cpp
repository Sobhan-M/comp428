#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define N 4

#define MIN 1
#define MAX 10

void StaticArrToDynamic(int** dynamic, int matrix[N][N])
{	
	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			dynamic[i][j] = matrix[i][j];
		}
	}
}

int main()
{
	srand(time(NULL));

	int** matrix;
	matrix = (int**) malloc(sizeof(int)*N);
	for (int i = 0; i < N; ++i)
	{
		matrix[i] = (int*) malloc(sizeof(int)*N);
	}

	int statMatrix[N][N] = 
	{
		{0, 1, 2, 3},
		{4, 0, 5, 6},
		{7, 8, 0 ,9},
		{1, 2, 3, 0}
	};
	StaticArrToDynamic(matrix, statMatrix);

	// RandomMatrix(matrix, N, MIN, MAX);
	// InitializeShortestPathMatrix(matrix, N);

	printf("Initial Matrix:\n");
	PrintMatrix(matrix, N);

	for (int k = 0; k < N; ++k)
	{
		for (int i = 0; i < N; ++i)
		{
			for (int j = 0; j < N; ++j)
			{
				if (matrix[i][j] > matrix[i][k] + matrix[k][j])
				{
					matrix[i][j] = matrix[i][k] + matrix[k][j];
				}
			}
		}
	}

	printf("Final Matrix:\n");
	PrintMatrix(matrix, N);

	// Does not work for some reason:
	// free(matrix[0]);
	free(matrix[1]);
	free(matrix[2]);
	free(matrix[3]);
	free(matrix);

	return 0;
}