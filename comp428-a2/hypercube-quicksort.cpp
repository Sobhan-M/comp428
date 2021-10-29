#include "hypercube-quicksort.h"
#include <stdlib.h>
#include <string>

int Sort::Random(int min, int max)
{
	return (rand() % (max - min + 1)) + min;
}

void Sort::RandomList(int*& list, int size, int min, int max)
{
	list = new int[size];
	for (int i = 0; i < size; ++i)
	{
		list[i] = Sort::Random(min, max);
	}
}

void Sort::Partition(int* list, int size, int pivot, int*& smallerList, int*& biggerList, int& smallerSize, int& biggerSize)
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

void Sort::Join(int* list1, int size1, int* list2, int size2, int*& newList, int& newSize)
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

std::string Sort::IntToBinaryString(int n, int bits) 
{
	std::string tempOutput = "";
	while(true)
	{
		// 4 = 100
		if (n == 0)
		{
			tempOutput += "0";
			break;
		}
		else if (n == 1)
		{
			tempOutput += "1";
			break;
		}
		else if (n%2 == 0)
		{
			tempOutput += "0";
			n /= 2; 
		}
		else if (n%2 == 1)
		{
			tempOutput += "1";
			n /= 2; 
		}
	}

	// Reversing.
	std::string output = "";
	for (int i = tempOutput.length() - 1; i >= 0 ; --i)
	{
		output += tempOutput[i];
	}

	while (bits - output.length() > 0)
	{
		output = "0" + output;
	}

	return output;
}

int Sort::BinaryStringToInt(std::string binary)
{
	int output = 0;
	int power = 1;
	for (int i = binary.length() - 1 ; i >= 0 ; --i)
	{
		if (binary[i] == '0')
		{
			output += 0*power;
		}
		else if (binary[i] == '1')
		{
			output += 1*power;
		}
		power = power*2;
	}

	return output;
}

bool Sort::IsSorted(int* array, int size)
{
	for (int i = 0; i < size - 1; ++i)
	{
		if (array[i] > array[i+1])
		{
			return false;
		}
	}
	
	return true;
}

void Sort::ReverseList(int*& array, int size)
{
	int* tempArray = new int[size];
	for (int i = 0; i < size; ++i)
	{
		tempArray[i] = array[size - 1 - i];
	}

	delete[] array;
	array = tempArray;
}

void Sort::PrefixSum(int* array, int size, int*& outputArray)
{
	outputArray = new int[size];
	for (int i = 0; i < size; ++i)
	{
		int sum = 0;
		for (int j = 0; j < i; ++j)
		{
			sum += array[j];
		}
		outputArray[i] = sum;
	}
}
