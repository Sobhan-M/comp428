#include <stdio.h>
#include "functions.h"
#include <stdlib.h>
#include <time.h>

#define N 4
#define MIN 0
#define MAX 10

int main()
{
	srand(time(NULL));

	int** matrix = (int**) malloc(sizeof(int)*N);
	for(int i = 0; i < N; ++i)
	{
		matrix[i] = (int*) malloc(sizeof(int)*N);
	}

	RandomMatrix(matrix, N, MIN, MAX);

	printf("Printing Matrix\n");
	PrintMatrix(matrix, N);
}