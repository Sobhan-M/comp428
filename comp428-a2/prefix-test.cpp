#include <iostream>
#include "hypercube-quicksort.h"
using namespace std;
using namespace Sort;

int main()
{
	int* array = new int[5];
	int* prefixSum = nullptr;

	array[0] = 2;
	array[1] = 1;
	array[2] = 6;
	array[3] = 1;
	array[4] = 4;

	PrefixSum(array, 5, prefixSum);

	cout << "Array: ";
	for (int i = 0; i < 5; ++i)
	{
		cout << array[i] << " ";
	}
	cout << endl;

	cout << "PrefixSum: ";
	for (int i = 0; i < 5; ++i)
	{
		cout << prefixSum[i] << " ";
	}
	cout << endl;

	delete[] array;
	delete[] prefixSum;
}