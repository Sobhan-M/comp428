#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "circle-functions.h"
#define TRUE 1
#define FALSE 0



// Scatter sends the info to everyone. Needs to be called by everyone.
// Gather is like reduce, but minus the operation.
// Each process has dedicated memory, but some variables must be global.
// You need uniform chunks.

int main()
{
	const int TOTAL_POINTS = 10000;
	int numOfPointsInCircle = 0;
	float randomPointX;
	float randomPointY;
	float pi;
	clock_t startTime = clock();
	clock_t endTime;

	srand(time(0));

	int i;
	for(i = 0 ; i < TOTAL_POINTS ; i++)
	{
		
		randomPointX = ((float)rand())/RAND_MAX;
		randomPointY = ((float)rand())/RAND_MAX;

		if(isInCircle(randomPointX, randomPointY) == TRUE)
		{
			numOfPointsInCircle++;
		}
	}

	pi = 4.0*numOfPointsInCircle/TOTAL_POINTS;
	endTime = clock();

	printf("PI = %f \n", pi);
	printf("Total Time = %f ms \n", (double)(endTime - startTime)/CLOCKS_PER_SEC);

	return 0;
}