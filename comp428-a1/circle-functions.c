#include "circle-functions.h"
#include <stdio.h>
#include <stdlib.h>
#define TRUE 1
#define FALSE 0

static const float RADIUS = 1;

int isInCircle(float x, float y)
{
	if (x*x + y*y < RADIUS*RADIUS)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void countNumInCircle(int* circleCount, int iterations)
{
	float randomPointX;
	float randomPointY;

	int i;
	for(i = 0 ; i < iterations ; i++)
	{
		
		randomPointX = ((float)rand())/RAND_MAX;
		randomPointY = ((float)rand())/RAND_MAX;

		if(isInCircle(randomPointX, randomPointY) == TRUE)
		{
			(*circleCount)++;
		}
	}
}
