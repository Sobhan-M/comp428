#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <mpi.h>
#include "circle-functions.h"
#define TRUE 1
#define FALSE 0

static const float RADIUS = 1;

int main(int argc, char *argv[])
{

	// MPI information.
	const int MASTER_RANK = 0;
	int size;
	int rank;

	// Information about the circle.
	const int TOTAL_POINTS = 10000;
	int pointCount = 0;
	int totalPointCount;
	float pi;

	// Setting up MPI.
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	srand(time(0)+rank);
	// Dividing workload.
	const int POINTS_PER_PROCESSOR = TOTAL_POINTS/size;

	// Starting time.
	clock_t startTime = clock();
	clock_t endTime;

	countNumInCircle(&pointCount, POINTS_PER_PROCESSOR);

	MPI_Reduce(&pointCount, &totalPointCount, 1, MPI_INT, MPI_SUM, MASTER_RANK, MPI_COMM_WORLD);

	if (rank == MASTER_RANK)
	{
		pi = 4.0*totalPointCount/TOTAL_POINTS;
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