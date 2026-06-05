#include "intVector.h"
#include <stdlib.h>
#include <stdio.h>

int main() {

	IntVector* myVector = INTVEC_LITERAL(1, 2, 3, 4, 5);

	printIntVector(myVector);
	
	ERR_CODE errorRes = 0;
	VectorElem result = -1;
	

	errorRes = getIntVector(myVector, 4, &result);

	printf("ERR Code: %s\n", errorToStr(errorRes));
	printf("Result: %d\n", result);

	printIntVector(myVector);



	freeIntVector(myVector);


	return 0;
}