#include "functions.h"
#include <stdlib.h>
#include <stdio.h>


int Random(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

void RandomList(int* list, int size, int min, int max)
{
	for (int i = 0; i < size; ++i)
	{
		list[i] = Random(min, max);
	}
}

void RandomMatrix(int** list, int n, int min, int max)
{
	for (int i = 0; i < n; ++i)
	{
		RandomList(list[i], n, min, max);
	}
}

void InitializeShortestPathMatrix(int** matrix, int n)
{
	for (int i = 0; i < n; ++i)
	{
		matrix[i][i] = 0;
	}
}

void PrintMatrix(int** matrix, int n)
{
	for (int i = 0; i < n; ++i)
	{
		for (int j = 0; j < n; ++j)
		{
			printf("%d\t", matrix[i][j]);
		}
		printf("\n");
	}
}