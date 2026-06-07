#include <stdio.h>
#include <stdlib.h>
#include "intVector.h"

// ERROR Checks
#define CHK_NULL_PTR(ptr) do {if((ptr)==NULL) return VEC_NULL_PTR_ERR;}while(0)
#define CHK_INDEX(i, upper) do {if((i)>=(upper)) return VEC_RANGE_ERR;}while(0)
#define CHK_INDEX_LESS_THAN(i, upper) do {if((i)>(upper)) return VEC_RANGE_ERR;}while(0)
#define CHK_INDEX_ZERO(i) do {if((i)==0) return VEC_RANGE_ERR;}while(0)
#define CHK_MEM_ALLOC(ptr) do {if((ptr)==NULL) return VEC_ALLOC_MEM_ERR;}while(0)
#define RET_SUCCESS() do {return VEC_SUCCESS;}while(0)


IntVector* createIntVector(VecSize size, VecSize capacity) {
	if (size > capacity) return NULL;

	IntVector* vec = malloc(sizeof(IntVector));
	if (vec == NULL) return NULL;

	VectorElem* ptr = malloc(capacity * sizeof(VectorElem));
	if (ptr == NULL) {
		free(vec);
		return NULL;
	}

	vec->ptr = ptr;
	vec->capacity = capacity;
	vec->size = size;

	return vec;
}

IntVector* createIntVectorInit(VecSize size, VecSize capacity, VectorElem initValue) {
	if (size > capacity) return NULL;

	IntVector* vec = malloc(sizeof(IntVector));
	if (vec == NULL) return NULL;

	VectorElem* ptr = malloc(capacity * sizeof(VectorElem));
	if (ptr == NULL) {
		free(vec);
		return NULL;
	}

	for (VecSize i = 0; i < size; i++) {
		ptr[i] = initValue;
	}

	vec->ptr = ptr;
	vec->capacity = capacity;
	vec->size = size;

	return vec;
}

ERR_CODE concatIntVector(IntVector** dest, const IntVector* a, const IntVector* b) {
	CHK_NULL_PTR(a);
	CHK_NULL_PTR(b);

	VecSize newSize = a->size + b->size;

	IntVector* vec = malloc(sizeof(IntVector));
	if (vec == NULL) {
		*dest = NULL;
		return VEC_ALLOC_MEM_ERR;
	}

	VectorElem* ptr = malloc(newSize * sizeof(VectorElem));
	if (ptr == NULL) {
		free(vec);
		*dest = NULL;
		return VEC_ALLOC_MEM_ERR;
	}

	for (VecSize i = 0; i < a->size;i++) {
		ptr[i] = a->ptr[i];
	}

	for (VecSize i = 0; i < b->size;i++) {
		ptr[i + a->size] = b->ptr[i];
	}

	*dest = vec;
	vec->ptr = ptr;
	vec->size = newSize;
	vec->capacity = newSize;

	RET_SUCCESS();
}

IntVector* createIntVectorFromArray(const VectorElem* array, VecSize len) {
	if (array == NULL || len == 0) return NULL;

	IntVector* vec = malloc(sizeof(IntVector));
	if (vec == NULL) return NULL;

	VectorElem* ptr = malloc(len * sizeof(VectorElem));
	if (ptr == NULL) {
		free(vec);
		return NULL;
	}

	for (VecSize i = 0; i < len;i++) {
		ptr[i] = array[i];
	}

	vec->ptr = ptr;
	vec->size = len;
	vec->capacity = len;

	return vec;
}

ERR_CODE popIntVector(IntVector* intVec, VectorElem* outValue) {
	CHK_NULL_PTR(intVec);
	CHK_NULL_PTR(outValue);
	CHK_INDEX_ZERO(intVec->size);

	intVec->size--;

	*outValue = intVec->ptr[intVec->size];

	RET_SUCCESS();
}

ERR_CODE appendIntVector(IntVector* intVec, VectorElem value) {
	CHK_NULL_PTR(intVec);

	if (intVec->size < intVec->capacity) {
		intVec->ptr[intVec->size] = value;
		intVec->size++;
	}
	else {
		VecSize newCapacity = (intVec->capacity == 0) ? 4 : intVec->capacity * 2;
		VectorElem* ptr = realloc(intVec->ptr, newCapacity * sizeof(VectorElem));
		CHK_MEM_ALLOC(ptr);

		intVec->capacity = newCapacity;
		intVec->size++;

		ptr[intVec->size - 1] = value;
		intVec->ptr = ptr;
	}

	RET_SUCCESS();
}


ERR_CODE removeIntVector(IntVector* intVec, VecSize index) {
	CHK_NULL_PTR(intVec);
	CHK_INDEX(index, intVec->size);

	for (VecSize i = index; i < intVec->size - 1; i++) {
		intVec->ptr[i] = intVec->ptr[i + 1];
	}
	intVec->size--;

	RET_SUCCESS();
}

ERR_CODE insertIntVector(IntVector* intVec, VecSize index, VectorElem value) {
	CHK_NULL_PTR(intVec);
	CHK_INDEX_LESS_THAN(index, intVec->size);

	if (intVec->size < intVec->capacity) {
		intVec->size++;

		for (VecSize i = intVec->size - 1; i > index;i--) {
			intVec->ptr[i] = intVec->ptr[i - 1];
		}

		intVec->ptr[index] = value;
	}
	else {
		VecSize newCapacity = (intVec->capacity == 0) ? 4 : intVec->capacity * 2;
		VectorElem* ptr = realloc(intVec->ptr, newCapacity * sizeof(VectorElem));
		CHK_MEM_ALLOC(ptr);

		intVec->capacity = newCapacity;
		intVec->size++;

		intVec->ptr = ptr;

		for (VecSize i = intVec->size - 1; i > index;i--) {
			intVec->ptr[i] = intVec->ptr[i - 1];
		}

		intVec->ptr[index] = value;
	}

	RET_SUCCESS();
}


int searchIntVector(const IntVector* intVec, VectorElem value, VecSize startFrom) {
	if (intVec == NULL || startFrom >= intVec->size) {
		return -1;
	}

	for (VecSize i = startFrom; i < intVec->size;i++) {
		if (intVec->ptr[i] == value) {
			return i;
		}
	}
	return -1;
}

void printIntVector(const IntVector* intVec) {
	if (intVec == NULL) {
		printf("[null IntVector ptr]\n");
		return;
	}

	printf("[");
	for (VecSize i = 0; i < intVec->size; i++) {
		printf("%d", intVec->ptr[i]);
		if (i != intVec->size - 1) {
			printf(", ");
		}
	}
	printf("]");
	printf("  %zu/", intVec->size);
	printf("%zu\n", intVec->capacity);
}

void freeIntVector(IntVector* intVec) {
	if (intVec == NULL) return;

	free(intVec->ptr);
	free(intVec);
}

char* errorToStr(ERR_CODE errorCode) {
	switch (errorCode) {
		case 0:  return "VEC_SUCCESS";
		case 1:  return "VEC_NULL_PTR_ERR";
		case 2:  return "VEC_ALLOC_MEM_ERR";
		case 3:  return "VEC_RANGE_ERR";
		default: return "VEC_UNKNWON_ERR";
	}
}


ERR_CODE getIntVector(const IntVector* intVec, VecSize index, VectorElem* outValue) {
	CHK_NULL_PTR(intVec);
	CHK_NULL_PTR(outValue);
	CHK_INDEX(index, intVec->size);

	*outValue = intVec->ptr[index];

	RET_SUCCESS();
}
ERR_CODE setIntVector(IntVector* intVec, VecSize index, VectorElem value) {
	CHK_NULL_PTR(intVec);
	CHK_INDEX(index, intVec->size);

	intVec->ptr[index] = value;

	RET_SUCCESS();
}