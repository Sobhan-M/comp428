#include "hypercube-quicksort.h"
#include <mpi.h>
#include <stdlib.h>
#include <iostream>
#include <math.h>
#include <bitset>
#include <string>
#include <algorithm>
#include <vector>

using std::cout;
using std::endl;
using namespace Sort;
using std::bitset;
using std::string;
using std::vector;
using std::sort;

#define MASTER_RANK 0
#define MAX 1000
#define MIN 0
#define LOCAL_LIST_SIZE 100

void FindAllPivots(int rank, int dim, int dimensions, int* list, int listSize, int& pivot);

int main(int argc, char* argv[])
{
	int rank;
	int size;
	int* list = nullptr;
	int listSize = LOCAL_LIST_SIZE;

	// Initializing stuff.
	cout << "Initializing..." << endl;
	MPI_Init(&argc, &argv);
	MPI_Comm_size(MPI_COMM_WORLD, &size);
	MPI_Comm_rank(MPI_COMM_WORLD, &rank);
	srand(rank * rank * MPI_Wtime());

	// Finding dimensions and the binary ID.
	int dimensions = (int) log2(size);
	string binaryID = IntToBinaryString(rank, dimensions);

	if (rank == MASTER_RANK)
	{
		cout << "MASTER: dimensions = " << dimensions << endl;
	}

	// Randomizing lists.
	cout << "Process " << rank << ": Creating random lists..." << endl;
	RandomList(list, LOCAL_LIST_SIZE, MIN, MAX);

	for (int dim = 0 ; dim < dimensions ; ++dim)
	{
		int pivot;
		FindAllPivots(rank, dim, dimensions, list, listSize, pivot);

		int* smallerList = nullptr;
		int smallerSize;
		int* biggerList = nullptr;
		int biggerSize;

		cout << "Process " << rank << ": Partitioning..." << endl;

		// Partitioning local list.
		Partition(list, listSize, pivot, smallerList, biggerList, smallerSize, biggerSize);

		string tempID = binaryID;
		int* receiveArray = nullptr;
		int receiveSize;

		// Lower half.
		if (binaryID[dim] == '0')
		{
			int pairID = BinaryStringToInt(tempID.replace(dim, 1, "1"));

			cout << "Process " << rank << " / " << binaryID << ": Pairing with " << pairID << " / " << tempID << "..." << endl;

			MPI_Send(&biggerSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD); // Sending array size.
			MPI_Send(biggerList, biggerSize, MPI_INT, pairID, 1, MPI_COMM_WORLD); // Sending actual list.

			cout << "Process " << rank << " / " << binaryID << ": Finished sending to " << pairID << " / " << tempID << "..." << endl;

			MPI_Recv(&receiveSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receiving array size.
			receiveArray = new int[receiveSize];
			MPI_Recv(receiveArray, receiveSize, MPI_INT, pairID, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receiving actual array.

			cout << "Process " << rank << " / " << binaryID << ": Finished receiving from " << pairID << " / " << tempID << "..." << endl;

			delete[] list;
			list = nullptr;

			Join(smallerList, smallerSize, receiveArray, receiveSize, list, listSize);
		}
		// Higher half.
		else if (binaryID[dim] == '1')
		{
			int pairID = BinaryStringToInt(tempID.replace(dim, 1, "0"));

			cout << "Process " << rank << " / " << binaryID << ": Pairing with " << pairID << " / " << tempID << "..." << endl;

			MPI_Recv(&receiveSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receiving array size.
			receiveArray = new int[receiveSize];
			MPI_Recv(receiveArray, receiveSize, MPI_INT, pairID, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receiving actual array.

			cout << "Process " << rank << " / " << binaryID << ": Finished receiving from " << pairID << " / " << tempID << "..." << endl;

			MPI_Send(&smallerSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD); // Sending array size.
			MPI_Send(smallerList, smallerSize, MPI_INT, pairID, 1, MPI_COMM_WORLD); // Sending actual list.

			cout << "Process " << rank << " / " << binaryID << ": Finished sending to " << pairID << " / " << tempID << "..." << endl;

			delete[] list;
			list = nullptr;

			cout << "Process " << rank << ": Joining..." << endl;

			Join(biggerList, biggerSize, receiveArray, receiveSize, list, listSize);
		}

		// Deallocating memory.
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

	// TODO: GatherV into the master.
	int* listOfAllSizes = new int[size];

	if (rank == MASTER_RANK)
	{
		
		for(int i = 0; i < size; ++i)
		{
			if(i = 0)
			{
				listOfAllSizes[i] = listSize;
			}
			else
			{
				MPI_Recv(&listOfAllSizes[i], 1, MPI_INT, i, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
			}
		}
	}
	else if (rank != MASTER_RANK)
	{
		MPI_Send(&listSize, 1, MPI_INT, MASTER_RANK, 0, MPI_COMM_WORLD);
	}

	delete[] list;
	list = nullptr;

	MPI_Finalize();

}

void FindAllPivots(int rank, int dim, int dimensions, int* list, int listSize, int& pivot)
{
	// Special case on the first iteration.
	if (dim == 0)
	{
		if (rank == MASTER_RANK)
		{
			pivot = list[Random(0, listSize)];
		}

		MPI_Bcast(&pivot, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);
		return;
	}

	// Finding all the information with bits.
	string binaryID = IntToBinaryString(rank, dimensions);
	string highBits = binaryID.substr(0, dim);
	string lowBits = binaryID.substr(dim, binaryID.length() - highBits.length());
	int group = BinaryStringToInt(highBits);
	int discriminator = BinaryStringToInt(lowBits);
	int maxDiscriminator = pow(2, dimensions - dim) - 1;

	MPI_Comm newComm;

	// Refer to: https://www.open-mpi.org/doc/current/man3/MPI_Comm_split.3.php
	MPI_Comm_split(MPI_COMM_WORLD, group, discriminator, &newComm);
 
	if (discriminator == 0)
	{
		pivot = list[Random(0, listSize - 1)];
	}

	MPI_Bcast(&pivot, 1, MPI_INT, discriminator, newComm);
	MPI_Comm_free(&newComm);
}