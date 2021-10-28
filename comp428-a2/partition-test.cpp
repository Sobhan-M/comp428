#include "hypercube-quicksort.h"
#include <stdlib.h>
#include <iostream>

int main()
{
	int list[] = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int pivot = 3;

	std::cout << "List Size: " << 10 << std::endl;

	int* smallerList = nullptr;
	int* biggerList = nullptr;
	int smallerSize;
	int biggerSize;

	std::cout << "Before partitioning." << std::endl;

	Sort::Partition(list, 10, pivot, smallerList, biggerList, smallerSize, biggerSize);

	std::cout << "After partitioning." << std::endl;

	std::cout << "Smaller List Size: " << smallerSize << std::endl;
	std::cout << "Smaller List: ";

	for (int i = 0 ; i < smallerSize ; ++i)
	{
		std::cout << smallerList[i] << " ";
	}

	std::cout << std::endl;

	std::cout << "Bigger List Size: " << biggerSize << std::endl;
	std::cout << "Bigger List: ";

	for (int i = 0 ; i < biggerSize ; ++i)
	{
		std::cout << biggerList[i] << " ";
	} 

	std::cout << std::endl;
	std::cout << "Clearing things up" << std::endl;

	delete[] smallerList;
	delete[] biggerList;
	smallerList = nullptr;
	biggerList = nullptr;

	return 0;

}