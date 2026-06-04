#include <stdio.h>
#include <stdlib.h>
#include "intVector.h"


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

void concatIntVector(IntVector** dest, const IntVector* a, const IntVector* b) {
	if (a == NULL || b == NULL) {
		*dest = NULL;
		return;
	}

	VecSize newSize = a->size + b->size;

	IntVector* vec = malloc(sizeof(IntVector));
	if (vec == NULL) {
		*dest = NULL;
		return;
	}

	VectorElem* ptr = malloc(newSize * sizeof(VectorElem));
	if (ptr == NULL) {
		free(vec);
		*dest = NULL;
		return;
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

VectorElem popIntVector(IntVector* intVec, bool* success) {
	if (intVec == NULL) {
		if (success != NULL) *success = false;
		return 0;
	}

	if (intVec->size == 0) {
		if (success != NULL) *success = false;
		return 0;
	}
	if (success != NULL) *success = true;
	intVec->size--;

	return intVec->ptr[intVec->size];
}

void appendIntVector(IntVector* intVec, VectorElem value, bool* success) {
	if (intVec == NULL) {		
		if(success!=NULL) *success = false;
		return;
	}

	if (intVec->size < intVec->capacity) {
		intVec->ptr[intVec->size] = value;
		intVec->size++;
	}
	else {
		VecSize newCapacity = (intVec->capacity == 0) ? 4 : intVec->capacity * 2;
		VectorElem* ptr = realloc(intVec->ptr, newCapacity * sizeof(VectorElem));
		if (ptr == NULL) {
			if (success != NULL) *success = false;
			return;
		}
		intVec->capacity = newCapacity;
		intVec->size++;

		ptr[intVec->size - 1] = value;
		intVec->ptr = ptr;
	}

	if (success != NULL) *success = true;
}


void removeIntVector(IntVector* intVec, VecSize index) {
	if (intVec == NULL || index >= intVec->size) return;

	for (VecSize i = index; i < intVec->size - 1; i++) {
		intVec->ptr[i] = intVec->ptr[i + 1];
	}
	intVec->size--;
}

void insertIntVector(IntVector* intVec, VecSize index, VectorElem value, bool* success) {
	if (intVec == NULL) {
		if (success != NULL) *success = false;
		return;
	}

	if (index > intVec->size) {
		if (success != NULL) *success = false;
		return;
	}

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
		if (ptr == NULL) {
			if (success != NULL) *success = false;
			return;
		}

		intVec->capacity = newCapacity;
		intVec->size++;

		intVec->ptr = ptr;

		for (VecSize i = intVec->size - 1; i > index;i--) {
			intVec->ptr[i] = intVec->ptr[i - 1];
		}

		intVec->ptr[index] = value;
	}

	if (success != NULL) *success = true;
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
