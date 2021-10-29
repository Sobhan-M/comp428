#ifndef HYPERCUBE_QUICKSORT_H
#define HYPERCUBE_QUICKSORT_H

#include<string>

namespace Sort
{
	// This function returns a random pivot between min and max, inclusive.
	int Random(int min, int max);

	// This procedure creates a randomized list of a given length. Where the values are between min and max inclusive.
	// The returned list must be deallocated by the caller.
	void RandomList(int*& list, int size, int min, int max);

	// This function partitions the list into two parts. 
	// The first part is all values <= the pivot. 
	// The second part is all values > the pivot.
	// The input size is used and not modified.
	// The new sizes are modified using the last two parameters respectively.
	// Note that the lists returned must be deallocated by the caller.
	void Partition(int* list, int size, int pivot, int*& smallerList, int*& biggerList, int& smallerSize, int& biggerSize);

	// This function joins the lists together and returns a larger list.
	// Note that the list returned must be deallocated by the caller.
	void Join(int* list1, int size1, int* list2, int size2, int*& newList, int& newSize);

	// This function converts an int into a binary and left joins 0s to match the number of bits.
	std::string IntToBinaryString(int n, int bits);

	// This function converts a binary string into an int.
	int BinaryStringToInt(std::string binary);

	// This function returns true if the array is sorted in ascending order.
	bool IsSorted(int* array, int size);
}

#endif