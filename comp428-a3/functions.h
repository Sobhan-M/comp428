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

// Fills the local matrix with the corresponding elements of the global matrix.
void FillLocalMatrix(int** matrix, int** localMatrix, int m, int row, int column);

// Updates the appropriate part of the global matrix with the local content.
void UpdateGlobalMatrix(int** matrix, int** localMatrix, int m, int row, int column);

// Converts a 2D array to a 1D array, because MPI does not work well with 2D dynamic memory. The output and input have a size of n x n.
void MatrixToArray(int** matrix, int n, int* array);

// Converts a 1D array to 2D array, again because MPI does not work well with 2D dynamic memory. The output and input have a size of n x n.
void ArrayToMatrix(int** matrix, int n, int* array);

// Prints the contents of the array.
void PrintArray(int* array, int n);

// Tells you if k is an index in your row range.
bool IsInRow(int k, int row, int m);

// Tells you if k is an index in your column range or not.
bool IsInColumn(int k, int column, int m);

// Tells if k is an index before your row range.
bool IsBeforeRow(int k, int row, int m);

// Tells if k is an index after your row range.
bool IsAfterRow(int k, int row, int m);

// Tells if k is an index before your column range.
bool IsBeforeColumn(int k, int column, int m);

// Tells if k is an index after your column range.
bool IsAfterColumn(int k, int column, int m);
#endif