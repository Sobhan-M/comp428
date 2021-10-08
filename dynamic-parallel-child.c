#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "circle-functions.h"
#define TRUE 1
#define FALSE 0

int main(int argc, char *argv[])
{

	// MPI information.
	const int MASTER_RANK = 0;
	int numOfProcessors;
	int rank;
	MPI_Comm parentComm;

	// Information about the circle.
	int iterations;
	int numOfPointsInCircle = 0;
	float pi, piNew;

	// Looping information.
	int shouldLoop = TRUE;

	// Setting up MPI.
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numOfProcessors);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_get_parent(&parentComm);
	srand(time(0)+rank*clock());

	while(shouldLoop)
	{
		MPI_Bcast(&iterations, 1, MPI_INT, MASTER_RANK, parentComm);
		printf("Child %d: Iterations = %d\n", rank, iterations);

		countNumInCircle(&numOfPointsInCircle, iterations);
		pi = 4.0*numOfPointsInCircle/iterations;
		numOfPointsInCircle = 0;

		printf("Child %d: Pi = %f\n", rank, pi);
		MPI_Reduce(&pi, &piNew, 1, MPI_FLOAT, MPI_SUM, MASTER_RANK, parentComm);

		MPI_Bcast(&shouldLoop, 1, MPI_INT, MASTER_RANK, parentComm);
		printf("Child %d: Should Loop = %d\n", rank, shouldLoop);
	}

	MPI_Finalize();

	return 0;
}