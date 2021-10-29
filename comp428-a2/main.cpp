#include "hypercube-quicksort.h"
#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <bitset>
#include <string>
#include <algorithm>
#include <vector>

using std::cout, std::endl;
using namespace Sort;
using std::bitset;
using std::string;
using std::vector;
using std::sort;

#define MASTER_RANK 0
#define MAX 1000000
#define MIN 0
#define LOCAL_LIST_SIZE 10000

void FindAllPivots(int rank, int dim, int dimensions, int* list, int listSize, int& pivot);

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
	string binaryID = bitset<dimensions>(rank).to_string();

	if (rank == MASTER_RANK)
	{
		cout << "MASTER: dimensions = " << dimensions << endl;
	}

	cout << "Process " << rank << ": Creating random lists..." << endl;
	RandomList(list, LOCAL_LIST_SIZE, MIN, MAX);

	for (int dim = 0 ; dim < dimensions ; ++dim)
	{
		int pivot;
		FindAllPivots(rank, dim, dimensions, list, listSize, pivot);
		// TODO:
		// Use bit to decide how to broadcast.
		// after first iteration, we differentiate between the first bits.
		// after second iteration, we differentiate between the two bits.
		// after third ...

		int* smallerList = nullptr;
		int smallerSize;
		int* biggerList = nullptr;
		int biggerSize;

		cout << "Process " << rank << ": Partitioning..." << endl;

		Partition(list, listSize, pivot, smallerList, biggerList, smallerSize, biggerSize);

		string tempID = binaryID;
		int* receiveArray = nullptr;
		int receiveSize;

		if (binaryID[dim] == "0")
		{
			int pairID = bitset<dimensions>(tempID.replace(dim, 1, "1")).to_ulong();

			cout << "Process " << rank << " / " << binaryID << ": Pairing with " << pairID << " / " << tempID << "..." << endl;

			MPI_Send(&biggerSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD); // Sending array size.
			MPI_Send(&biggerList, biggerSize, MPI_INT, pairID, 1, MPI_COMM_WORLD); // Sending actual list.

			MPI_Recv(&receiveSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receiving array size.
			MPI_Recv(&receiveArray, receiveSize, MPI_INT, pairID, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE) // Receiving actual array.

			delete[] list;
			list = nullptr;

			Join(smallerList, smallerSize, receiveArray, receiveSize, list, listSize);
		}
		else if (binaryID[dim] == "1")
		{
			int pairID = bitset<dimensions>(tempID.replace(dim, 1, "0")).to_ulong();

			cout << "Process " << rank << " / " << binaryID << ": Pairing with " << pairID << " / " << tempID << "..." << endl;

			MPI_Recv(&receiveSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receiving array size.
			MPI_Recv(&receiveArray, receiveSize, MPI_INT, pairID, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE) // Receiving actual array.

			MPI_Send(&smallerSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD); // Sending array size.
			MPI_Send(&smallerList, smallerSize, MPI_INT, pairID, 1, MPI_COMM_WORLD); // Sending actual list.

			delete[] list;
			list = nullptr;

			cout << "Process " << rank << ": Joining..." << endl;

			Join(biggerList, biggerSize, receiveArray, receiveSize, list, listSize);
		}

		delete[] smallerList;
		delete[] biggerList;
		smallerList = nullptr;
		biggerList = nullptr;

	}

	cout << "Process " << rank << ": Doing local quicksort..." << endl;

	// Local quicksort.
	vector<int> vectorList(list, list + listSize);
	sort(vectorList.begin(), vectorList.end());
	
	if (rank == MASTER_RANK)
	{
		cout << "Process " << rank << ": ";
		for (int element : vectorList)
		{
			cout << element << " ";
		}
	}

	delete[] list;
	list = nullptr;

	MPI_Finalize();

}

void FindAllPivots(int rank, int dim, int dimensions, int* list, int listSize, int& pivot)
{
	if (dim == 0)
	{
		if (rank == MASTER_RANK)
		{
			pivot = list[Random(0, listSize)];
		}

		MPI_Bcast(pivot, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
		return;
	}

	string binaryID = bitset<dimensions>(rank).to_string();
	string highBits = binaryID.substr(0, dim);
	string lowBits = binaryID.substr(dim, binaryID.length() - highBits.length());
	int group = bitset<dim>(highBits).to_ulong();
	int discriminator = bitset<dimensions - dim>(lowBits).to_ulong(); // What happens if this is an empty string?

	MPI_Comm newComm;

	// Refer to: https://www.open-mpi.org/doc/current/man3/MPI_Comm_split.3.php
	MPI_Comm_split(MPI_COMM_WORLD, group, discriminator, &newComm);

	int tempPivot;

	if (discriminator == 0)
	{
		tempPivot = list[Random(0, listSize)];

		// Figure out the something to be less than. The max discriminator.
		for (int i = 1 ; i < ; ++i)
		{
			MPI_Send(&tempPivot, 1, MPI_INT, i, , newComm);
		}
	}

	if (discriminator != 0)
	{
		MPI_Recv(&tempPivot, 1, MPI_INT, 0, , newComm, MPI_STATUS_IGNORE);
	}

	pivot = newPivot;
}