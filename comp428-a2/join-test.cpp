#include "hypercube-quicksort.h"
#include <stdlib.h>
#include <iostream>

int main()
{
	int list1[7] = {10,9,4,3,8,3,7};
	int list2[4] = {1,4,9,6};
	int* newList;
	int newSize;

	Sort::Join(list1, 7, list2, 4, newList, newSize);

	for (int i = 0 ; i < newSize ; ++i)
	{
		std::cout << newList[i] << " ";
	}

	return 0;
}