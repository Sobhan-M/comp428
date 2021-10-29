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
#define LOCAL_LIST_SIZE 10

void FindAllPivots(int rank, int dim, int dimensions, int* list, int listSize, int& pivot);
void PrintListContent(int* array, int size);

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
	srand(rank * rank * time(NULL));

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

			MPI_Recv(&receiveSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receiving array size.
			receiveArray = new int[receiveSize];
			MPI_Recv(receiveArray, receiveSize, MPI_INT, pairID, 1, MPI_COMM_WORLD, MPI_STATUS_IGNORE); // Receiving actual array.

			cout << "Process " << rank << " / " << binaryID << ": Finished pairing with " << pairID << " / " << tempID << "..." << endl;

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

			MPI_Send(&smallerSize, 1, MPI_INT, pairID, 0, MPI_COMM_WORLD); // Sending array size.
			MPI_Send(smallerList, smallerSize, MPI_INT, pairID, 1, MPI_COMM_WORLD); // Sending actual list.

			cout << "Process " << rank << " / " << binaryID << ": Finished pairing with " << pairID << " / " << tempID << "..." << endl;

			delete[] list;
			list = nullptr;

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
	for (int i = 0; i < vectorList.size(); ++i)
	{
		list[i] = vectorList[i];
	}
	cout << "Processor " << rank << ": IsSorted(List) = " << (IsSorted(list, listSize) ? "true" : "false") << "..." << endl;

	// Gathering sizes.
	int* listOfAllSizes = new int[size];
	if (rank == MASTER_RANK)
	{
		cout << "Processor " << rank << ": Gathering sizes of all lists..." << endl;
	}
	MPI_Gather(&listSize, 1, MPI_INT, listOfAllSizes, 1, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

	// Gathering displacement.
	int* displacement = new int[size];
	if (rank == MASTER_RANK)
	{
		cout << "Processor " << rank << ": Finding displacements..." << endl;
	}
	PrefixSum(listOfAllSizes, size, displacement);

	// Gathering all lists.
	int* combinedList = new int[size * LOCAL_LIST_SIZE];
	if (rank == MASTER_RANK)
	{
		cout << "Processor " << rank << ": Gathering all lists..." << endl;
	}
	MPI_Gatherv(list, listSize, MPI_INT, combinedList, listOfAllSizes, displacement, MPI_INT, MASTER_RANK, MPI_COMM_WORLD);

	// Results.
	if (rank == MASTER_RANK)
	{
		cout << "Processor " << rank << ": IsSorted(CombinedList) = " << (IsSorted(combinedList, size * LOCAL_LIST_SIZE) ? "true" : "false") << "..." << endl;
		cout << "Processor " << rank << ": CombinedList = ";
		PrintListContent(combinedList, size * LOCAL_LIST_SIZE);
	}

	delete[] list;
	delete[] listOfAllSizes;
	delete[] displacement;
	delete[] combinedList;
	list = nullptr;
	listOfAllSizes = nullptr;
	displacement = nullptr;
	combinedList = nullptr;

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

void PrintListContent(int* array, int size)
{
	for (int i = 0; i < size; ++i)
	{
		cout << array[i] << " ";
	}
	cout << endl;
}