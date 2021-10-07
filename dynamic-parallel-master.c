#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>
#include "circle-functions.h"
#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[])
{

	// MPI information.
	const int MASTER_RANK = 0;
	const char* CHILD_COMMAND = "dynamic-parallel-child";
	int numOfParents;
	int rank;
	int numOfChildren;

	MPI_Comm childComm;

	float pi = 0;
	float piNew = 0;

	int shouldLoop = TRUE;
	const int CONVERGENCE_ITERATIONS = 3;
	int convergenceNum = 0;
	// Setting up MPI.
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numOfParents);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	srand(time(0)+rank);

	// Starting time.
	clock_t startTime = clock();
	clock_t endTime;

	if (argc != 2)
	{
		printf("usage: %s <number of workers>\n", argv[0]);
	}
	else
	{
		numOfChildren = atoi(argv[1]);
	}

	while(shouldLoop)
	{
		MPI_Comm_spawn(CHILD_COMMAND, argv, numOfChildren, MPI_INFO_NULL, MASTER_RANK, MPI_COMM_SELF, &childComm, MPI_ERRCODES_IGNORE);
		MPI_Reduce(&pi, &piNew, 1, MPI_FLOAT, MPI_SUM, MPI_ROOT, childComm);
		piNew /= numOfChildren;

		if (abs((piNew - pi)/pi) < 0.0001)
		{
			convergenceNum++;
			if (convergenceNum == CONVERGENCE_ITERATIONS)
			{
				shouldLoop = FALSE;
			}
		}
		else
		{
			convergenceNum = 0;
		}
		pi = piNew;
		printf("%f\n", pi);
	}

	MPI_Finalize();
	endTime = clock();

	if (rank == MASTER_RANK)
	{
		printf("PI = %f \n", pi);
		printf("Total Time = %f ms \n", (double)(endTime - startTime)/CLOCKS_PER_SEC);
	}

	return 0;
}