#include "hypercube-quicksort.h"
#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <bitset>

using std::cout, std::endl;
using namespace Sort;
using std::bitset;

#define MASTER_RANK 0
#define MAX 1000000
#define MIN 0
#define LOCAL_LIST_SIZE 10000

int main(int argc, char* argv[])
{
	int rank;
	int size;
	int* list = nullptr;
	int listSize = LOCAL_LIST_SIZE;

	cout << "Initializing..." << endl;
	
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	srand(rank * rank * MPI_Wtime());

	int dimensions = (int) log2(size);

	if (rank == MASTER_RANK)
	{
		cout << "MASTER: dimensions = " << dimensions << endl;
	}

	cout << "Process " << rank << ": Creating random lists..." << endl;
	RandomList(list, LOCAL_LIST_SIZE, MIN, MAX);

	for (int i = 0 ; i < dimensions ; ++i)
	{
		int pivot;

		if (rank == MASTER_RANK)
		{
			pivot = Random(1, 2);

		}
	}



	MPI_Finalize();

}