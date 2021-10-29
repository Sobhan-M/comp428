#include "hypercube-quicksort.h"
#include <iostream>

using namespace std;
using namespace Sort;

int main()
{
	int sortedArray[] = {1, 2, 3, 4, 5, 6, 9, 13};
	int singleUnitArray[1] = {7};
	int unsortedArray[] = {1, 5, 3, 2, 1};

	cout << "IsSorted(sortedArray): " << IsSorted(sortedArray, 8) << endl; // Should be true.
	cout << "IsSorted(singleUnitArray): " << IsSorted(singleUnitArray, 1) << endl; // Should be true.
	cout << "IsSorted(unsortedArray): " << IsSorted(unsortedArray, 5) << endl; // Should be false.

	return 0;
}