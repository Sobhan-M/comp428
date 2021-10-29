#include "hypercube-quicksort.h"
#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <bitset>
#include <string>

using std::cout, std::endl;
using namespace Sort;
using std::bitset;
using std::string;

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
	string binaryID = bitset<dimensions>(rank);

	if (rank == MASTER_RANK)
	{
		cout << "MASTER: dimensions = " << dimensions << endl;
	}

	cout << "Process " << rank << ": Creating random lists..." << endl;
	RandomList(list, LOCAL_LIST_SIZE, MIN, MAX);

	for (int dim = 0 ; dim < dimensions ; ++dim)
	{
		int pivot; // Use bit to decide how to broadcast.
		// Something like if (dim - 1 >= 0 && your specific binary is this: then...)

		int* smallerList = nullptr;
		int smallerSize;
		int* biggerList = nullptr;
		int biggerSize;

		Partition(list, listSize, pivot, smallerList, biggerList, smallerSize, biggerSize);

		string tempID = binaryID;
		int* receiveArray = nullptr;
		int receiveSize;

		if (binaryID[dim] == "0")
		{
			int pairID = bitset<dimensions>(tempID.replace(dim, 1, "1")).to_ulong();

			MPI_Send(&biggerSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD); // Sending array size.
			MPI_Send(&biggerList, biggerSize, MPI_INT, pairID, 1, MPI_COMM_WORLD); // Sending actual list.

			MPI_Recv(&receiveSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receiving array size.
			MPI_Recv(&receiveArray, receiveSize, MPI_INT, pairID, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE) // Receiving actual array.

			// TODO: Maybe delete[] the original array and the bigger/smaller arrays.
			Join(smallerList, smallerSize, receiveArray, receiveSize, list, listSize);
		}
		else if (binaryID[dim] == "1")
		{
			int pairID = bitset<dimensions>(tempID.replace(dim, 1, "0")).to_ulong();

			MPI_Recv(&receiveSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receiving array size.
			MPI_Recv(&receiveArray, receiveSize, MPI_INT, pairID, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE) // Receiving actual array.

			MPI_Send(&smallerSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD); // Sending array size.
			MPI_Send(&smallerList, smallerSize, MPI_INT, pairID, 1, MPI_COMM_WORLD); // Sending actual list.

			// TODO: Maybe delete[] the original array and the bigger/smaller arrays.
			Join(biggerList, biggerSize, receiveArray, receiveSize, list, listSize);
		}

	}

	// Do local quicksort.



	MPI_Finalize();

}