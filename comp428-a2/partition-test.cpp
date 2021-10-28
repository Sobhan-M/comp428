#include "hypercube-quicksort.h"
#include <stdlib.h>
#include <iostream>
#include <vector>

using std::vector;

int main()
{
	vector<int> list = {1, 2, 3, 4, 5, 6, 7, 8, 9, 10};
	int pivot = 3;

	int* smallerList = nullptr;
	int* biggerList = nullptr;
	int smallerSize;
	int biggerSize;

	Sort::Partition(list.data(), list.size(), pivot, smallerList, biggerList, smallerSize, biggerSize);

	for (int i = 0 ; i < smallerSize ; ++i)
	{
		std::cout << smallerList[i] << " ";
	}

	std::cout << std::endl;

	for (int i = 0 ; i < biggerSize ; ++i)
	{
		std::cout << biggerList[i] << " ";
	} 

	delete[] smallerList;
	delete[] biggerList;
	smallerList = nullptr;
	biggerList = nullptr;

	return 0;

}