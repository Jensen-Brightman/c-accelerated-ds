#include <stdio.h>
#include <stdlib.h>

typedef struct {

	int* ptr; // NOTE: should we use a void* ??
	// int itemSize; // Bytes
	int arrayLen; // Number of items in the array
	int itemsLeft; // items left in the array -> array xan be made longer to avoid lots of memory reallocations

} DynamicArray;

DynamicArray createDynamicArray(int itemCount, int additionalSpaces) {
	int arraySize = itemCount + additionalSpaces;

	int* ptr = malloc(arraySize * sizeof(int));
	if (ptr == NULL) return (DynamicArray) { NULL, 0, 0 };

	DynamicArray da = { ptr, itemCount, additionalSpaces };

	return da;
}

DynamicArray createDynamicArrayInitialize(int itemCount, int additionalSpaces, int initialValue) {
	int arraySize = itemCount + additionalSpaces;

	int* ptr = malloc(arraySize * sizeof(int));
	if (ptr == NULL) return (DynamicArray) { NULL, 0, 0 };

	for (int i = 0; i < itemCount; i++) {
		ptr[i] = initialValue;
	}

	DynamicArray da = { ptr, itemCount, additionalSpaces };

	return da;
}

int popDynamicArray(DynamicArray* da) {
	da->arrayLen--;
	da->itemsLeft++;

	return da->ptr[da->arrayLen];
}

void appendDynamicArray(DynamicArray* da, int value) {
	if (da->itemsLeft > 0) {
		da->ptr[da->arrayLen] = value;
		da->arrayLen++;
		da->itemsLeft--;
	}
	else {
		da->arrayLen++;
		int* ptr = realloc(da->ptr, da->arrayLen * sizeof(int));
		if (ptr == NULL) exit(1);

		da->ptr = ptr;
		da->ptr[da->arrayLen - 1] = value;
	}
}

void printDynamicArray(DynamicArray da) {
	printf("[");
	for (int i = 0; i < da.arrayLen; i++) {
		printf("%d", da.ptr[i]);
		if (i != da.arrayLen - 1) {
			printf(", ");
		}
	}
	printf("]\n");
}

void freeDynamicArray(DynamicArray* da) {
	free(da->ptr);
	da->ptr = NULL;
	da->arrayLen = 0;
	da->itemsLeft = 0;
}

DynamicArray concatDynamicArrays(DynamicArray* a, DynamicArray* b) {
	// Create a new DynamicArray, c, given two others, a & b

	DynamicArray c = createDynamicArray(a->arrayLen + b->arrayLen, 0);

	for (int i = 0; i < a->arrayLen; i++) {
		c.ptr[i] = a->ptr[i];
	}

	for (int i = 0; i < b->arrayLen; i++) {
		c.ptr[i + a->arrayLen] = b->ptr[i];
	}

	return c;
}

void removeDynamicArray(DynamicArray* da, int index) {
	// remove an item at a given index

	for (int i = index; i < da->arrayLen - 1;i++) {
		da->ptr[i] = da->ptr[i + 1];
	}

	da->arrayLen--;
	da->itemsLeft++;
}

int searchDynamicArray(DynamicArray* da, int value, int startFrom) {
	for (int i = startFrom; i<da->arrayLen; i++) {
		if (da->ptr[i] == value) {
			return i;
		}
	}
	return -1;
}

void insertDynamicArray(DynamicArray* da, int value, int location) {
	if (location > da->arrayLen + da->itemsLeft - 1) exit(1);

	if (da->itemsLeft > 0) {
		da->arrayLen++;
		da->itemsLeft--;
		for (int i = da->arrayLen-1;i > location;i--) {
			if (i == 0) continue;
			da->ptr[i] = da->ptr[i - 1];
		}
		
		da->ptr[location] = value;
	}
	else
	{
		da->arrayLen++;
		int* ptr = realloc(da->ptr, da->arrayLen * sizeof(int));
		if (ptr == NULL) exit(1);

		da->ptr = ptr;

		for (int i = da->arrayLen - 1;i > location;i--) {
			if (i == 0) continue;
			da->ptr[i] = da->ptr[i - 1];
		}

		da->ptr[location] = value;
	}
}


int main() {

	//DynamicArray da = createDynamicArrayInitialize(10, 4, 5);

	//appendDynamicArray(&da, 9);

	//printf("Valued popped: %d\n", popDynamicArray(&da));

	//printDynamicArray(da);

	//freeDynamicArray(&da);


	//DynamicArray a = createDynamicArrayInitialize(3, 0, 2);
	//DynamicArray b = createDynamicArrayInitialize(2, 0, 4);

	//printDynamicArray(a);
	//printDynamicArray(b);

	//DynamicArray c = concatDynamicArrays(&a, &b);

	//printDynamicArray(c);

	//freeDynamicArray(a);
	//freeDynamicArray(b);
	//freeDynamicArray(c);

	DynamicArray k = createDynamicArray(4, 0);
	k.ptr[0] = 1;
	k.ptr[1] = 2;
	k.ptr[2] = 3;
	k.ptr[3] = 4;

	printDynamicArray(k);

	

	removeDynamicArray(&k, 2); // [1,2,3,4] --> [1,2,4]

	printDynamicArray(k);

	appendDynamicArray(&k, 5);
	appendDynamicArray(&k, 6);
	appendDynamicArray(&k, 7);

	printDynamicArray(k);

	printf("Location of 4: %d\n", searchDynamicArray(&k, 4, 3));

	insertDynamicArray(&k, 10, 3);
	printDynamicArray(k);

	freeDynamicArray(&k);


	return 0;
}