#ifndef HYPERCUBE_QUICKSORT_H
#define HYPERCUBE_QUICKSORT_H

namespace Sort
{
	// This function returns a random pivot between min and max, inclusive.
	int SelectPivot(int min, int max);

	// This function partitions the list into two parts. 
	// The first part is all values <= the pivot. 
	// The second part is all values > the pivot.
	// The input size is used and not modified.
	// The new sizes are modified using the last two parameters respectively.
	void Partition(int* list, int size, int pivot, int* smallerList, int* biggerList, int& smallerSize, int& biggerSize);

	// This function joins the lists together and returns a larger list.
	// At the end of this, both input sizes will be modified and will become equal to the final list.
	int* Join(int* list1, int& size1, int* list2, int& size2);
}

#endif