#include "hypercube-quicksort.h"
#include <stdlib.h>
#include <iostream>
#include <vector>
using std::vector;

int main()
{
	vector<int> list1 = {10,9,4,3,8,3,7};
	vector<int> list2 = {1,4,9,6};
	int* newList = nullptr;
	int newSize;

	Sort::Join(list1.data(), list1.size(), list2.data(), list2.size(), newList, newSize);

	for (int i = 0 ; i < newSize ; ++i)
	{
		std::cout << newList[i] << " ";
	}

	delete[] newList;
	newList = nullptr;

	return 0;
}