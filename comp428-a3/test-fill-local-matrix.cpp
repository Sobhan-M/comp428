#include <stdio.h>
#include "functions.h"
#include <stdlib.h>

#define M 2
#define N 4

int main()
{
	int** localMatrix = (int**) malloc(sizeof(int)*M);
	for(int i = 0; i < M; ++i)
	{
		localMatrix[i] = (int*) malloc(sizeof(int)*M);
	}

	int** globalMatrix = (int**) malloc(sizeof(int)*N);
	for(int i = 0; i < N; ++i)
	{
		globalMatrix[i] = (int*) malloc(sizeof(int)*N);
	}

	for (int i = 0; i < N; ++i)
	{
		for (int j = 0; j < N; ++j)
		{
			globalMatrix[i][j] = N*i+j;
		}
	}

	printf("Printing Global Matrix\n");
	PrintMatrix(globalMatrix, N);

	FillLocalMatrix(globalMatrix, localMatrix, M, 1, 1);

	printf("Printing Local Matrix\n");
	PrintMatrix(localMatrix, M);
}