#include "functions.h"
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>

// Matrix of size n x n
#define N 36

#define MIN 1
#define MAX 60

#define MASTER 0

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

int GetRow(int rank, int m)
{
	return rank % (N/m);
}

int GetColumn(int rank, int m)
{
	return (rank * m) / N;
}

int Sqrt(int n)
{
	if (n == 1)
		return 1;
	else if (n == 4)
		return 2;
	else if (n == 9)
		return 3;
	else if (n == 16)
		return 4;
	else if (n == 25)
		return 5;
	else if (n == 36)
		return 6;
	else if (n == 49)
		return 7;
	else if (n == 64)
		return 8;
	else if (n > 64)
	{
		printf("Error! Only supports up to n = 64!\n");
		exit(1);
	}
	else
	{
		printf("Error! Only supports perfect squares!\n");
		exit(1);
	}
}

int main(int argc, char* argv[])
{
	int size, rank;
	int row, column;
	double startTime, endTime;

	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);

	// int matrix[N][N];
	int** matrix = new int*[N];
	for (int i = 0; i < N; ++i)
	{
		matrix[i] = new int[N];
	}

	int* globalArray = new int[N*N];

	srand(time(NULL));

	if (rank == MASTER)
	{
		startTime = MPI_Wtime();

		// printf("Processor %d: Creating global matrix.\n", rank);
		// RandomMatrix(matrix, N, MIN, MAX);
		
		int statMatrix[N][N] = 
		{
			{0, 1, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 5, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 20, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0 },
			{6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 6, 0, 3, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 14}, 
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{25, 0, 0, 10, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0, 0, 0, 0, 0,0,0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 2, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 3, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 6, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 5, 0, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 4, 0, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 0, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 0, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 3, 0, 1, 0, 0 },
			{0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 1, 0 }
		};
		StaticArrToDynamic(matrix, statMatrix);

		InitializeShortestPathMatrix(matrix, N);

		printf("Initial Matrix:\n");
		PrintMatrix(matrix, N);

		MatrixToArray(matrix, N, globalArray);
	}
	
	// printf("Processor %d: Receiving global matrix.\n", rank);
	MPI_Bcast(globalArray, N*N, MPI_INT, MASTER, MPI_COMM_WORLD);
	ArrayToMatrix(matrix, N, globalArray);

	// The m x m chunk of each processor.
	// int m = N / ((int) sqrt((double) size));
	int m = N / Sqrt(size);
	row = GetRow(rank, m);
	column = GetColumn(rank, m);
	// printf("Process %d: (row, column) = (%d, %d)\n", rank, row, column);

	MPI_Comm rowWorld;
	MPI_Comm columnWorld;

	// if (rank == MASTER)
	// {
	// 	printf("Processor %d: Creating new worlds.\n", rank);
	// }

	MPI_Comm_split(MPI_COMM_WORLD, row, column, &rowWorld);
	MPI_Comm_split(MPI_COMM_WORLD, column, row, &columnWorld);

	// if (rank == MASTER)	
	// {
	// 	printf("Processor %d: Creating local buffers.\n", rank);
	// }
	int** bufferMatrix = new int*[m];
	for (int i = 0; i < m; ++i)
	{
		bufferMatrix[i] = new int[m];
	}

	int* bufferArray = new int[m*m];

	for (int k = 0; k < N; ++k)
	{
		// Note, there's definitely a way to parallelize broadcasting rows and stuff.
		// Probably involves more malloc, so I don't want to do it because memory management
		// is a pain in C.

		// if (rank == MASTER)
		// {
		// 	printf("Processor %d: Broadcasting across rows.\n", rank);
		// }

		// Broadcasting along row.
		for (int i = 0; i < size; ++i)
		{
			if (rank == i)
			{
				// printf("Processor %d: Preparing to broadcast to row %d\n", rank, row);
				FillLocalMatrix(matrix, bufferMatrix, m, row, column);
				MatrixToArray(bufferMatrix, m, bufferArray);
				// printf("Processor %d: Finished preparing to broadcast to row %d\n", rank, row);
			}

			// If I have the "if" do I even need new worlds?
			if (row == GetRow(i, m))
			{
				// printf("Processor %d: Receiving in row %d from process %d\n", rank, row, i);
				MPI_Bcast(bufferArray, m*m, MPI_INT, GetColumn(i, m), rowWorld);
				ArrayToMatrix(bufferMatrix, m, bufferArray);
				UpdateGlobalMatrix(matrix, bufferMatrix, m, GetRow(i, m), GetColumn(i, m));
				// printf("Processor %d: Finished receiving in row %d from process %d\n", rank, row, i);
			}
		}

		// if (rank == MASTER)
		// {
		// 	printf("Processor %d: Broadcasting across columns.\n", rank);
		// }

		// Broadcasting along column.
		for (int i = 0; i < size; ++i)
		{
			if (rank == i)
			{
				// printf("Processor %d: Preparing to broadcast to column %d\n", rank, column);
				FillLocalMatrix(matrix, bufferMatrix, m, row, column);
				MatrixToArray(bufferMatrix, m, bufferArray);
				// printf("Processor %d: Finished preparing to broadcast to column %d\n", rank, column);
			}

			// If I have the "if" do I even need new worlds?
			if (column == GetColumn(i, m))
			{
				// printf("Processor %d: Receiving in column %d from process %d\n", rank, column, i);
				MPI_Bcast(bufferArray, m*m, MPI_INT, GetRow(i, m), columnWorld);
				ArrayToMatrix(bufferMatrix, m, bufferArray);
				UpdateGlobalMatrix(matrix, bufferMatrix, m, GetRow(i, m), GetColumn(i, m));
				// printf("Processor %d: Finished receiving in column %d from process %d\n", rank, column, i);
			}
		}

		// if (rank == MASTER)
		// {
		// 	printf("Processor %d: Doing local algorithm.\n", rank);
		// }

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

	// if (rank == MASTER)
	// {
	// 	printf("Processor %d: Aggregating matrix.\n", rank);
	// }

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
		IsEqualToAnswer(matrix);

		endTime = MPI_Wtime();
		printf("Total Time: %f seconds.\n", (endTime - startTime));
	}

	// Deallocation.
	for (int i = 0; i < N; ++i)
	{
		delete[] matrix[i];
	}
	delete[] matrix;

	delete[] globalArray;

	for (int i = 0; i < m; ++i)
	{
		delete[] bufferMatrix[i];
	}
	delete[] bufferMatrix;

	delete[] bufferArray;

	MPI_Comm_free(&rowWorld);
	MPI_Comm_free(&columnWorld);
	MPI_Finalize();
}

