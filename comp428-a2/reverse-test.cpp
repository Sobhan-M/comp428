#include <iostream>
using namespace std;
void ReverseList(int*& array, int size)
{
	int* tempArray = new int[size];
	for (int i = 0; i < size; ++i)
	{
		tempArray[i] = array[size - 1 - i];
	}

	delete[] array;
	array = tempArray;
}

int main()
{
	int* array = new int[6];
	array[0] = 0;
	array[1] = 1;
	array[2] = 2;
	array[3] = 3;
	array[4] = 4;
	array[5] = 5;

	ReverseList(array, 6);

	for (int i = 0; i < 6; ++i)
	{
		cout << array[i] << " ";
	}
	cout << endl;

	delete[] array;

}