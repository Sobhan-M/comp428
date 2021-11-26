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

	for (int i = 0; i < M; ++i)
	{
		for (int j = 0; j < M; ++j)
		{
			localMatrix[i][j] = M*i+j + 1;
		}
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
			globalMatrix[i][j] = 0;
		}
	}

	printf("Printing Global Matrix\n");
	PrintMatrix(globalMatrix, N);

	printf("Printing Local Matrix\n");
	PrintMatrix(localMatrix, M);

	UpdateGlobalMatrix(globalMatrix, localMatrix, M, 1, 1);

	printf("Printing Global Matrix After Update\n");
	PrintMatrix(globalMatrix, N);
}