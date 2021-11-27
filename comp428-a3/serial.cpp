#include "functions.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>


#define N 6

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
	matrix = new int*[N];
	for (int i = 0; i < N; ++i)
	{
		matrix[i] = new int[N];
	}

	int statMatrix[N][N] = 
	{
		{0, 4, 5, 2, 3, 4},
		{6, 0, 1, 3, 6, 2},
		{5, 4, 0, 2, 5, 1},
		{3, 3, 4, 0, 6, 6},
		{4, 5, 1, 8, 0, 5},
		{6, 4, 2, 7, 4, 0}
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

	// Deallocation.
	for (int i = 0; i < N; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;

	return 0;
}