#ifndef FORMATTING_H_
#define FORMATTING_H_

#define MAX_OPTION_LENGTH 32
#include "numberarray.h"

void printNDashes(int n) {
	for (int i = 0; i < n; i++)
		printf("-");
}

void printFormattedOption(const char* option, char command) {
	int offset = MAX_OPTION_LENGTH - strlen(option);
	printf("%s ", option);
	printNDashes(offset);
	printf(" %c", command);
	puts("");
}

void printFormattedTitle(const char* title) {
	puts("");
	printf("%-4s<%s>", "", title);
	puts("");
}

void printMinimumWidthNumber(int number, int minLength) {
	//both 'number' and 'printLength' must be positive in order to produce a defined output.
	//if the length of 'number' is less than 'minLength', 0's are padded.

	int lengthOffset = minLength - findDigits(number);

	for (int i = 0; i < lengthOffset; i++) printf("0");
	printf("%d", number);
}

void printAlignedNumberArray(int* numberArray, int size, int columns) {
	//the elements of numberArray must all be positive in order to produce a defined output.

	int maxValue = findMaxNumber(numberArray, size);
	int maxValueLength = findDigits(maxValue);

	int rows = (size % columns == 0) ? size/columns : size/columns + 1;

	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < columns; j++) {
			int linearIndex = i * columns + j;

			if (linearIndex == size)
				break;
			printMinimumWidthNumber(numberArray[linearIndex], maxValueLength);
			printf(" ");
		}
		printf("\n");
	}
}

#endif