#ifndef INTVECTOR_H
#define INTVECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

typedef int VectorElem;
typedef size_t VecSize;

#define INTVEC_LITERAL(...) \
    createIntVectorFromArray((const VectorElem[]){__VA_ARGS__}, sizeof((const VectorElem[]){__VA_ARGS__}) / sizeof(VectorElem))

typedef struct {
	VectorElem* ptr;
	VecSize size;     // Current number of elements
	VecSize capacity; // Total allocated slots
} IntVector;

// Instantiate methods
__declspec(dllexport) IntVector* createIntVector(VecSize size, VecSize capacity);
__declspec(dllexport) IntVector* createIntVectorInit(VecSize size, VecSize capacity, VectorElem initValue);
__declspec(dllexport) void concatIntVector(IntVector** dest, const IntVector* a, const IntVector* b);
__declspec(dllexport) IntVector* createIntVectorFromArray(const VectorElem* array, VecSize len);


// Push/Pop
__declspec(dllexport) VectorElem popIntVector(IntVector* intVec, bool* success);
__declspec(dllexport) void appendIntVector(IntVector* intVec, VectorElem value, bool* success);

// Remove/insert
__declspec(dllexport) void removeIntVector(IntVector* intVec, VecSize index);
__declspec(dllexport) void insertIntVector(IntVector* intVec, VecSize index, VectorElem value, bool* success);

// Search
__declspec(dllexport) int searchIntVector(const IntVector* intVec, VectorElem value, VecSize startFrom);

// Others
__declspec(dllexport) void printIntVector(const IntVector* intVec);
__declspec(dllexport) void freeIntVector(IntVector* intVec);
__declspec(dllexport) VectorElem getIntVector(IntVector* intVec, VecSize index, bool* success);
__declspec(dllexport) void setIntVector(IntVector* intVec, VecSize index, VectorElem value, bool* success);

#endif // !INTVECTOR_H