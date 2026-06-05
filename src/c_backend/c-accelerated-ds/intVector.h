#ifndef INTVECTOR_H
#define INTVECTOR_H

#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdint.h>

typedef int VectorElem;
typedef size_t VecSize;
typedef uint8_t ERR_CODE;

#define INTVEC_LITERAL(...) \
    createIntVectorFromArray((const VectorElem[]){__VA_ARGS__}, sizeof((const VectorElem[]){__VA_ARGS__}) / sizeof(VectorElem))

// ERROR Codes
#define VEC_SUCCESS         0 // No error
#define VEC_NULL_PTR_ERR    1 // Null ptr error
#define VEC_ALLOC_MEM_ERR   2 // Allocation or memory error
#define VEC_RANGE_ERR       3 // Bound or index error

typedef struct {
	VectorElem* ptr;
	VecSize size;     // Current number of elements
	VecSize capacity; // Total allocated slots
} IntVector;

// Instantiate methods
__declspec(dllexport) IntVector* createIntVector(VecSize size, VecSize capacity);
__declspec(dllexport) IntVector* createIntVectorInit(VecSize size, VecSize capacity, VectorElem initValue);
__declspec(dllexport) ERR_CODE concatIntVector(IntVector** dest, const IntVector* a, const IntVector* b);
__declspec(dllexport) IntVector* createIntVectorFromArray(const VectorElem* array, VecSize len);


// Push/Pop
__declspec(dllexport) ERR_CODE popIntVector(IntVector* intVec, VectorElem* outValue);
__declspec(dllexport) ERR_CODE appendIntVector(IntVector* intVec, VectorElem value);

// Remove/insert
__declspec(dllexport) ERR_CODE removeIntVector(IntVector* intVec, VecSize index);
__declspec(dllexport) ERR_CODE insertIntVector(IntVector* intVec, VecSize index, VectorElem value);

// Search
__declspec(dllexport) int searchIntVector(const IntVector* intVec, VectorElem value, VecSize startFrom);

// Others
__declspec(dllexport) void printIntVector(const IntVector* intVec);
//__declspec(dllexport) char* stringifyIntVector(const IntVector* intVec); [NOTE] NOT IMPLEMENTED
__declspec(dllexport) void freeIntVector(IntVector* intVec);
__declspec(dllexport) char* errorToStr(ERR_CODE errorCode);

// Get & Set
__declspec(dllexport) ERR_CODE getIntVector(const IntVector* intVec, VecSize index, VectorElem* outValue);
__declspec(dllexport) ERR_CODE setIntVector(IntVector* intVec, VecSize index, VectorElem value);

#endif // !INTVECTOR_H