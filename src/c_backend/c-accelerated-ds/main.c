#include "intVector.h"
#include <stdlib.h>
#include <stdio.h>

int main() {

	printf("# Initializing\n");

	IntVector* vec1 = INTVEC_LITERAL(1, 2, 3, 4, 5);
	printf("vec1 from literal: ");
	printIntVector(vec1);

	IntVector* vec2 = createIntVector(2, 5);
	if (vec2 == NULL) return 1;
	vec2->ptr[0] = 5;
	vec2->ptr[1] = 6;
	printf("vec2: ");
	printIntVector(vec2);

	IntVector* vec3 = createIntVectorInit(3, 3, 99);
	printf("vec3 initialized with '99': ");
	printIntVector(vec3);

	printf("\n#Appending & Resizing\n");

	IntVector* emptyVec = createIntVector(0, 0);
	printf("EmptyVec before append: ");
	printIntVector(emptyVec);

	bool appendSuccess;
	for (int i = 0;i <= 5;i++) {
		appendIntVector(emptyVec, i * 2, &appendSuccess);
		if (appendSuccess) {
			printf("Appended %d: ", i * 2);
			printIntVector(emptyVec);
		}
	}

	printf("\n#Insertion & Removal\n");

	insertIntVector(emptyVec, 2, 999, NULL);
	printf("After inserting 999 at index 2: ");
	printIntVector(emptyVec);

	removeIntVector(emptyVec, 2);
	printf("After Removing item at index 2: ");
	printIntVector(emptyVec);

	printf("\n#Searching\n");

	int foundIndex = searchIntVector(emptyVec, 4, 0);
	printf("Found value 4 starting from index 0: %d\n", foundIndex);

	int missingIndex = searchIntVector(emptyVec, 3, 0);
	printf("Didn't find value 3 starting from index 0: %d\n", missingIndex);

	printf("\n#Popping & safe flags\n");

	bool popSuccess;
	VectorElem poppedValue = popIntVector(emptyVec, &popSuccess);
	if (popSuccess) {
		printf("Safely popped: %d\n", poppedValue);
		printf("After pop: ");
		printIntVector(emptyVec);
	}

	printf("\n#Concatenation\n");

	IntVector* joinedVec = NULL;
	concatIntVector(&joinedVec, vec1, vec3);
	printf("vec1: "); printIntVector(vec1);
	printf("vec3: "); printIntVector(vec3);
	if (joinedVec != NULL) {
		printf("vec1 + vec3:");
		printIntVector(joinedVec);
	}


	printf("\n#Pointer safety guards\n");

	bool guardCheck = true;
	appendIntVector(NULL, 45, &guardCheck);
	printf("NULL appended: %s\n", guardCheck ? "true" : "false");

	printf("\n#Cleanup memory\n");

	freeIntVector(vec1);
	freeIntVector(vec2);
	freeIntVector(vec3);
	freeIntVector(emptyVec);
	freeIntVector(joinedVec);
	printf("All IntVectors freed\n");


	return 0;
}