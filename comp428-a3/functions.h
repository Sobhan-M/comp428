#ifndef FUNCTIONS_H
#define FUNCTIONS_H

// This function returns a random pivot between min and max, inclusive.
int Random(int min, int max);

// This procedure creates a randomized list of a given length. Where the values are between min and max inclusive.
// The input list must be allocated by you.
void RandomList(int* list, int size, int min, int max);

// This procedure creates a randomized n x n matrix. Where the values are between min and max inclusive.
// The input list must be allocated by you.
void RandomMatrix(int** list, int n, int min, int max);

// Makes the diagonal zero.
void InitializeShortestPathMatrix(int** matrix, int n);

// Prints the contents of the n x n matrix.
void PrintMatrix(int** matrix, int n);

#endif