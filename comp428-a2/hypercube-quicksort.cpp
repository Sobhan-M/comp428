#include "hypercube-quicksort.h"
#include <stdlib.h>

int Sort::SelectPivot(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

void Sort::Partition(int* list, int size, int pivot, int* smallerList, int* biggerList, int& smallerSize, int& biggerSize)
{
	// Counting the sizes.
	smallerSize = 0;
	
	for (int i = 0 ; i < size ; ++i)
	{
		if (list[i] <= pivot)
		{
			++smallerSize;
		}	
	}

	biggerSize = size - smallerSize;

	// Filling new arrays.
	smallerList = new int[smallerSize];
	biggerList = new int[biggerSize];

	int smallIndex = 0;
	int bigIndex = 0;
	for (int i = 0 ; i < size ; ++i)
	{
		if (list[i] <= pivot)
		{
			smallerList[smallIndex++] = list[i];
		}
		else
		{
			biggerList[bigIndex++] = list[i];
		}
	}
}

void Sort::Join(int* list1, int size1, int* list2, int size2, int* newList, int& newSize)
{
	// Joining arrays.
	newSize = size1 + size2;
	newList = new int[newSize];

	for (int i = 0 ; i < size1 ; ++i)
	{
		newList[i] = list1[i];
	}

	for (int i = 0 ; i < size2 ; ++i)
	{
		newList[size1 + i] = list2[i];
	}
}