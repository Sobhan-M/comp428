#include <stdio.h>
#include "functions.h"
#include <stdlib.h>

#define N 4

int main()
{
	int* array = (int*) malloc(sizeof(int)*N*N);

	int** matrix = (int**) malloc(sizeof(int)*N);
	for(int i = 0; i < N; ++i)
	{
		matrix[i] = (int*) malloc(sizeof(int)*N);
	}

	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			matrix[i][j] = N*i+j + 1;
		}
	}

	printf("Printing Matrix\n");
	PrintMatrix(matrix, N);

	MatrixToArray(matrix, N, array);

	printf("Printing Array\n");
	PrintArray(array, N*N);
}