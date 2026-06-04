#ifndef INTVECTOR_H
#define INTVECTOR_H

#include <stdbool.h>
#include <stddef.h>

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
IntVector* createIntVector(VecSize size, VecSize capacity);
IntVector* createIntVectorInit(VecSize size, VecSize capacity, VectorElem initValue);
void concatIntVector(IntVector** dest, const IntVector* a, const IntVector* b);
IntVector* createIntVectorFromArray(const VectorElem* array, VecSize len);


// Push/Pop
VectorElem popIntVector(IntVector* intVec, bool* success);
void appendIntVector(IntVector* intVec, VectorElem value, bool* success);

// Remove/insert
void removeIntVector(IntVector* intVec, VecSize index);
void insertIntVector(IntVector* intVec, VecSize index, VectorElem value, bool* success);

// Search
int searchIntVector(const IntVector* intVec, VectorElem value, VecSize startFrom);

// Others
void printIntVector(const IntVector* intVec);
void freeIntVector(IntVector* intVec);



#endif // !INTVECTOR_H