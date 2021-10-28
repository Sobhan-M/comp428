#include "hypercube-quicksort.h"
#include <stdlib.h>
#include <iostream>

int main()
{
	int list1[] = {10,9,4,3,8,3,7};
	int list2[] = {1,4,9,6};

	std::cout << "List 1 Size: " << 7 << std::endl;
	std::cout << "List 2 Size: " << 4 << std::endl;

	int* newList = nullptr;
	int newSize;

	std::cout << "Beginning to join." << std::endl;

	Sort::Join(list1, 7, list2, 4, newList, newSize);

	std::cout << "After joining." << std::endl;
	std::cout << "The New List Size: " << newSize << std::endl;
	std::cout << "The New List: ";

	for (int i = 0 ; i < newSize ; ++i)
	{
		std::cout << newList[i] << " ";
	}

	std::cout <<  std::endl << "Clearing the heap." << std::endl;

	delete[] newList;
	newList = nullptr;

	return 0;
}