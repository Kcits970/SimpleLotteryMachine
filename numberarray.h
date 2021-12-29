#ifndef NUMBERARRAY_H
#define NUMBERARRAY_H

int* getSequentialNumberArray(int lowerBound, int upperBound) {
	//lowerBound and upperBound are both inclusive.

	int arraySize = upperBound - lowerBound + 1;
	int* numberArray = malloc(arraySize * sizeof(int));

	for (int i = 0; i < arraySize; i++) {
		numberArray[i] = lowerBound + i;
	}

	return numberArray;
}

int compareNumbers(const void* num1, const void* num2) {
	return *(int*) num1 - *(int*) num2;
}

void sortNumberArray(int* numberArray, int size) {
	qsort(numberArray, size, sizeof(int), &compareNumbers);
}

int findMaxNumber(int* numberArray, int size) {
	int max = 0;

	for (int i = 0; i < size; i++)
		if (numberArray[i] >= max)
			max = numberArray[i];

	return max;
}

int findDigits(int n) {
	//this function is defined for positive numbers only: if n is negative, then the output is undefined.

	int digits = 0;

	while (n != 0) {
		n /= 10;
		digits++;
	}

	return digits;
}

int getRandomNumber(int lowerBound, int upperBound) {
	//lowerBound and upperBound are both inclusive.

	return rand() % (upperBound - lowerBound + 1) + lowerBound;
}

void swap(int* num1, int* num2) {
	int temp = *num1;
	*num1 = *num2;
	*num2 = temp;
}

void shuffle(int* numberArray, int size) {
	for (int i = 0; i < size; i++) {
		int randomIndex = getRandomNumber(i, size-1);
		swap(&numberArray[i], &numberArray[randomIndex]);
	}
}

bool areElementsDuplicate(int* numberArray, int size) {
	for (int i = 0; i < size - 1; i++)
		for (int j = i + 1; j < size; j++)
			if (numberArray[i] == numberArray[j])
				return true;

	return false;
}

bool areElementsInRange(int* numberArray, int size, int lowerBound, int upperBound) {
	for (int i = 0; i < size; i++)
		if (numberArray[i] < lowerBound || numberArray[i] > upperBound)
			return false;

	return true;
}

bool checkIfContained(int* numberArray, int size, int number) {
	for (int i = 0; i < size; i++) {
		if (numberArray[i] == number)
			return true;
	}

	return false;
}


int countElementInstances(int* array1, int size1, int* array2, int size2) {
	//returns the number of elements from 'array1' that appear in 'array2'

	int count = 0;
	for (int i = 0; i < size1; i++)
		if (checkIfContained(array2, size2, array1[i])) count++;

	return count;
}

#endif