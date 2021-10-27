#include "hypercube-quicksort.h"
#include <stdlib.h>
#include <time.h>
#include <iostream>

int main()
{
	srand(time(NULL));

	for (int i = 0 ; i < 10 ; ++i)
	{
		std::cout << Sort::SelectPivot(1, 10) << std::endl;
	}

	return 0;
}