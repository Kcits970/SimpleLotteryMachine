#ifndef MYSTRING_H
#define MYSTRING_H

char* getEmptyString(int characters) {
	return (char*) calloc(characters, sizeof(char));
}

int removeNewline(char* str) {
	char* lnptr = strchr(str, '\n');
	if (lnptr == NULL)
		return 0;
	else
		*lnptr = '\0';

	return 1;
}

bool isNumeric(char c) {
	if (c >= '0' && c <= '9')
		return true;
	return false;
}

int* extractNumbersFromString(char* str, int n) {
	/*
	Extracts and returns exactly n numbers in a given string.
	If the string doesn't contain enough numbers, the function returns NULL.
	*/

	int* numbers = malloc(n * sizeof(int));
	int extractions = 0;

	int strLength = strlen(str);
	for (int i = 0; i < strLength; i++) {
		if (isNumeric(str[i])) {
			char* numberPointer = str + i;
			while (isNumeric(str[i])) i++;

			str[i] = '\0';
			numbers[extractions++] = atoi(numberPointer);

			if (extractions == n) break;
		}
	}

	if (extractions != n) {
		free(numbers);
		return NULL;
	}

	return numbers;
}

#endif