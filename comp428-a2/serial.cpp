#include "hypercube-quicksort.h"
#include <mpi.h>
#include <algorithm>
#include <iostream>
#include <vector>

using std::cout;
using std::endl;
using std::vector;
using namespace Sort;

#define MASTER_RANK 0
#define MAX 1000000
#define MIN 0
#define LOCAL_LIST_SIZE 200000

int main(int argc, char* argv[])
{
	int rank;
	int size;
	int* list = NULL;
	int listSize = LOCAL_LIST_SIZE;
	double startTime;
	double endTime;

	// Initializing stuff.
	// cout << "Initializing..." << endl;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	srand(rank * rank * time(NULL));

	startTime = MPI_Wtime();

	// Randomizing lists.
	cout << "Process " << rank << ": Creating random lists..." << endl;
	RandomList(list, LOCAL_LIST_SIZE, MIN, MAX);

	cout << "Process " << rank << ": Doing local quicksort..." << endl;

	// Local quicksort.
	vector<int> vectorList(list, list + listSize);
	sort(vectorList.begin(), vectorList.end());
	for (int i = 0; i < vectorList.size(); ++i)
	{
		list[i] = vectorList[i];
	}


	// Results.
	if (rank == MASTER_RANK)
	{
		cout << "MASTER: IsSorted(CombinedList) = " << (IsSorted(list, LOCAL_LIST_SIZE) ? "true" : "false") << "..." << endl;

		endTime = MPI_Wtime();
		cout << "MASTER: Time = " << endTime - startTime << " s..." << endl;
	}

	delete[] list;
	list = NULL;

	MPI_Finalize();

}