#include <stdio.h>
#include "functions.h"
#include <stdlib.h>

#define N 4

int main()
{
	int* array = (int*) malloc(sizeof(int)*N*N);

	for (int i = 0; i < N*N; ++i)
	{
		array[i] = i + 1;
	}

	int** matrix = (int**) malloc(sizeof(int)*N);
	for(int i = 0; i < N; ++i)
	{
		matrix[i] = (int*) malloc(sizeof(int)*N);
	}

	printf("Printing Array\n");
	PrintArray(array, N*N);
	
	ArrayToMatrix(matrix, N, array);

	printf("Printing Matrix\n");
	PrintMatrix(matrix, N);
}