#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <mpi.h>
#include "functions.h"
#define TRUE 1
#define FALSE 0

// Returns the rate of change, where 1 is 100%. Can be both positive and negative.
float rateOfChange(float new, float old);

int main(int argc, char *argv[])
{

	// MPI information.
	const int MASTER_RANK = 0;
	const char* CHILD_COMMAND = "child";
	int numOfParents;
	int rank;
	int numOfChildren;

	MPI_Comm childComm;

	float pi = 0;
	float piNew = 0;

	int shouldLoop = TRUE;
	const int CONVERGENCE_ITERATIONS = 3;
	const float CONVERGENCE_THRESHOLD = 0.001;
	int convergenceNum = 0;
	int iterations = 1000;
	const int ITERATIONS_INCREMENT = 1000;

	// Setting up MPI.
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &numOfParents);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	srand(time(0)+rank);

	// Starting time.
	double startTime = MPI_Wtime();
	double endTime;

	if (argc != 2)
	{
		printf("usage: %s <number of workers>\n", argv[0]);
	}
	else
	{
		numOfChildren = atoi(argv[1]);
	}

	MPI_Comm_spawn(CHILD_COMMAND, argv, numOfChildren, MPI_INFO_NULL, MASTER_RANK, MPI_COMM_SELF, &childComm, MPI_ERRCODES_IGNORE);
	while(shouldLoop)
	{
		MPI_Bcast(&iterations, 1, MPI_INT, MPI_ROOT, childComm);
		MPI_Reduce(&pi, &piNew, 1, MPI_FLOAT, MPI_SUM, MPI_ROOT, childComm);
		piNew /= numOfChildren;

		if (rateOfChange(piNew, pi) < CONVERGENCE_THRESHOLD && rateOfChange(piNew, pi) > -CONVERGENCE_THRESHOLD)
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
			iterations += ITERATIONS_INCREMENT;
		}

		pi = piNew;
		printf("Aggregate Pi = %f\n", pi);

		MPI_Bcast(&shouldLoop, 1, MPI_INT, MPI_ROOT, childComm);
	}
	endTime = MPI_Wtime();
	MPI_Finalize();
	

	if (rank == MASTER_RANK)
	{
		printf("PI = %f \n", pi);
		printf("Total Time = %f s \n", (endTime - startTime));
	}

	return 0;
}

float rateOfChange(float new, float old)
{
	return (new - old)/old;
}