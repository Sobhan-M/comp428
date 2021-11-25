#include "functions.h"
#include <mpi.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Matrix of size n x n
#define N 24

#define MIN 1
#define MAX 10

#define MASTER 0

int GetRow(int rank, int m)
{
	return (rank * m) % N;
}

int GetColumn(int rank, int m)
{
	return (rank * m) / N;
}

int main(int argc, char* argv[])
{
	int size, rank;
	int row, column;

	// int matrix[N][N];
	int** matrix;
	matrix = (int**) malloc(sizeof(int)*N);
	for(int i = 0; i < N; ++i)
	{
		matrix[i] = (int*) malloc(sizeof(int)*N);
	}

	int* globalArray;
	globalArray = (int*) malloc(sizeof(int)*N*N);

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	srand(rank*rank*time(NULL));

	if (rank == MASTER)
	{
		printf("Processor %d: Creating global matrix.\n", rank);
		RandomMatrix(matrix, N, MIN, MAX);
		InitializeShortestPathMatrix(matrix, N);

		printf("Initial Matrix:\n");
		PrintMatrix(matrix, N);

		MatrixToArray(matrix, N, globalArray);
	}
	
	printf("Processor %d: Broadcasting global matrix.\n", rank);
	MPI_Bcast(globalArray, N*N, MPI_INT, MASTER, MPI_COMM_WORLD);
	printf("Processor %d: Finished broadcasting global matrix.\n", rank);
	ArrayToMatrix(matrix, N, globalArray);

	// The m x m chunk of each processor.
	// int m = N / ((int) sqrt((double) size));
	int m = N / 2;
	row = GetRow(rank, m);
	column = GetColumn(rank, m);

	MPI_Comm rowWorld;
	MPI_Comm columnWorld;

	if (rank == MASTER)
	{
		printf("Processor %d: Creating new worlds.\n", rank);
	}


	MPI_Comm_split(MPI_COMM_WORLD, row, column, &rowWorld);
	MPI_Comm_split(MPI_COMM_WORLD, column, row, &columnWorld);

	int** bufferMatrix;
	bufferMatrix = (int**) malloc(sizeof(int)*m);
	for (int i = 0; i < N; ++i)
	{
		bufferMatrix[i] = (int*) malloc(sizeof(int)*m);
	}

	int* bufferArray;
	bufferArray = (int*) malloc(sizeof(int)*m*m);

	for (int k = 0; k < N; ++k)
	{
		// Note, there's definitely a way to parallelize broadcasting rows and stuff.
		// Probably involves more malloc, so I don't want to do it because memory management
		// is a pain in C.

		if (rank == MASTER)
		{
			printf("Processor %d: Broadcasting across rows.\n", rank);
		}

		// Broadcasting along row.
		for (int i = 0; i < size; ++i)
		{
			if (rank == i)
			{
				FillLocalMatrix(matrix, bufferMatrix, m, row, column);
				MatrixToArray(bufferMatrix, m, bufferArray);
			}

			// If I have the "if" do I even need new worlds?
			if (row == GetRow(i, m))
			{
				MPI_Bcast(bufferArray, m*m, MPI_INT, i, rowWorld);
				ArrayToMatrix(bufferMatrix, m, bufferArray);
				UpdateGlobalMatrix(matrix, bufferMatrix, m, GetRow(i, m), GetColumn(i, m));
			}
		}

		if (rank == MASTER)
		{
			printf("Processor %d: Broadcasting across columns.\n", rank);
		}

		// Broadcasting along column.
		for (int i = 0; i < size; ++i)
		{
			if (rank == i)
			{
				FillLocalMatrix(matrix, bufferMatrix, m, row, column);
				MatrixToArray(bufferMatrix, m, bufferArray);
			}

			// If I have the "if" do I even need new worlds?
			if (column == GetColumn(i, m))
			{
				MPI_Bcast(bufferArray, m*m, MPI_INT, i, columnWorld);
				ArrayToMatrix(bufferMatrix, m, bufferArray);
				UpdateGlobalMatrix(matrix, bufferMatrix, m, GetRow(i, m), GetColumn(i, m));
			}
		}

		if (rank == MASTER)
		{
			printf("Processor %d: Doing local algorithm.\n", rank);
		}

		// Doing the local Floyd algorithm.
		for (int i = 0; i < m; ++i)
		{
			for (int j = 0; j < m; ++j)
			{
				if(matrix[m*row + i][m*column + j] > (matrix[m*row + i][k] + matrix[k][m*column + j]))
				{
					matrix[m*row + i][m*column + j] = matrix[m*row + i][k] + matrix[k][m*column + j];
				}
			}
		}
	}

	if (rank == MASTER)
	{
		printf("Processor %d: Aggregating matrix.\n", rank);
	}

	for (int i = 1; i < size; ++i)
	{
		if (rank == i)
		{
			FillLocalMatrix(matrix, bufferMatrix, m, row, column);
			MatrixToArray(bufferMatrix, m, bufferArray);
			MPI_Send(bufferArray, m*m, MPI_INT, MASTER, 0, MPI_COMM_WORLD);
		}

		if (rank == MASTER)
		{
			MPI_Recv(bufferArray, m*m, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			ArrayToMatrix(bufferMatrix, m, bufferArray);
			UpdateGlobalMatrix(matrix, bufferMatrix, m, GetRow(i, m), GetColumn(i, m));
		}
	}

	if (rank == MASTER)
	{
		printf("Final Matrix:\n");
		PrintMatrix(matrix, N);
	}

	// Deallocate bufferMatrix;

	MPI_Comm_free(&rowWorld);
	MPI_Comm_free(&columnWorld);
	MPI_Finalize();
}

