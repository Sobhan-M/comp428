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
	const int TOTAL_POINTS = 10000;
	int numOfPointsInCircle = 0;
	float pi, piNew;

	// Setting up MPI.
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numOfProcessors);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	MPI_Comm_get_parent(&parentComm);
	srand(time(0)+rank*clock());

	countNumInCircle(&numOfPointsInCircle, TOTAL_POINTS);
	pi = 4.0*numOfPointsInCircle/TOTAL_POINTS;

	printf("Child %d: Pi = %f\n", rank, pi);
	MPI_Reduce(&pi, &piNew, 1, MPI_FLOAT, MPI_SUM, MASTER_RANK, parentComm);

	MPI_Finalize();

	return 0;
}