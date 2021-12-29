#ifndef TICKET_H_
#define TICKET_H_

#define TICKET_SIZE 6
#define TICKET_NUMBERS_LOWER_BOUND 1
#define TICKET_NUMBERS_UPPER_BOUND 45
#define TICKET_NUMBERS_CARDINALITY TICKET_NUMBERS_UPPER_BOUND - TICKET_NUMBERS_LOWER_BOUND + 1

#define DEFAULT_TICKET_PRICE 5
#define TICKET_WORTH_1ST 913900
#define TICKET_WORTH_2ND 3906
#define TICKET_WORTH_3RD 82
#define TICKET_WORTH_4TH DEFAULT_TICKET_PRICE

typedef enum { TYPE_AUTO, TYPE_MANUAL } TICKET_TYPE;

#include <time.h>
#include "numberarray.h"
#include "formatting.h"
#include "mystring.h"

typedef struct {
	int size;
	int* numbers;
	time_t time;
	int rank;
	int worth;
} TICKET;

TICKET* chosenTicket = NULL;
void generateChosenTicket() {
	srand(time(NULL));

	TICKET* getAutomatedTicket();
	chosenTicket = getAutomatedTicket();
	chosenTicket->rank = 1;
	chosenTicket->worth = TICKET_WORTH_1ST;
}

TICKET* getEmptyTicket() {
	TICKET* ticket = malloc(sizeof(TICKET));
	ticket->size = TICKET_SIZE;
	ticket->numbers = malloc(TICKET_SIZE * sizeof(int));

	return ticket;
}

void deleteTicket(TICKET* ticket) {
	free(ticket->numbers);
	free(ticket);
}

TICKET* getAutomatedTicket() {
	TICKET* ticket = getEmptyTicket();

	int* numberArray = getSequentialNumberArray(TICKET_NUMBERS_LOWER_BOUND, TICKET_NUMBERS_UPPER_BOUND);
	shuffle(numberArray, TICKET_NUMBERS_CARDINALITY);

	for (int i = 0; i < ticket->size; i++)
		ticket->numbers[i] = numberArray[i];
	sortNumberArray(ticket->numbers, TICKET_SIZE);

	free(numberArray);
	return ticket;
}

TICKET* getManualTicket() {
	int* numberArray = getSequentialNumberArray(TICKET_NUMBERS_LOWER_BOUND, TICKET_NUMBERS_UPPER_BOUND);
	printFormattedTitle("TICKET SHEET");
	printAlignedNumberArray(numberArray, TICKET_NUMBERS_CARDINALITY, 7);

	printf("SELECT %d UNIQUE NUMBERS: ", TICKET_SIZE);
	getInputInDefaultBuffer("");

	int* numbers = extractNumbersFromString(defaultBuffer, TICKET_SIZE);
	if (numbers == NULL) {
		printf("FALSE TICKET: THE INPUT DOES NOT CONTAIN ENOUGH NUMBERS\n");
		return NULL;
	}
	else if (!areElementsInRange(numbers, TICKET_SIZE, TICKET_NUMBERS_LOWER_BOUND, TICKET_NUMBERS_UPPER_BOUND)) {
		printf("FALSE TICKET: INPUTS ARE NOT WITHIN RANGE\n");
		free(numbers);
		return NULL;
	}
	else if (areElementsDuplicate(numbers, TICKET_SIZE)) {
		printf("FALSE TICKET: THE INPUT CONTAINS DUPLICATE NUMBERS\n");
		free(numbers);
		return NULL;
	}
	else {
		TICKET* ticket = getEmptyTicket();
		sortNumberArray(numbers, TICKET_SIZE);
		ticket->numbers = numbers;
		return ticket;
	}
}


void evaluateWorth(TICKET* ticket) {
	int matches = countElementInstances(ticket->numbers, ticket->size, chosenTicket->numbers, chosenTicket->size);
	ticket->rank = TICKET_SIZE - matches + 1;

	switch (ticket->rank) {
	case 1: ticket->worth = TICKET_WORTH_1ST; break;
	case 2: ticket->worth = TICKET_WORTH_2ND; break;
	case 3: ticket->worth = TICKET_WORTH_3RD; break;
	case 4: ticket->worth = TICKET_WORTH_4TH; break;
	default: ticket->worth = 0; break;
	}
}

TICKET* getTicket(TICKET_TYPE type) {
	TICKET* ticket = NULL;

	if (type == TYPE_AUTO)
		ticket = getAutomatedTicket();
	else
		ticket = getManualTicket();

	if (ticket == NULL) return NULL;

	time(&ticket->time);
	evaluateWorth(ticket);
	return ticket;
}

void printAsTicket(void* voidPtr) {
	TICKET* ticket = (TICKET*) voidPtr;
	int printLength = findDigits(TICKET_NUMBERS_UPPER_BOUND);

	for (int i = 0; i < ticket->size; i++) {
		printMinimumWidthNumber(ticket->numbers[i], printLength);
		printf(" ");
	}

	printf("(RANK %d, WORTH $%d)\n", ticket->rank, ticket->worth);
}

void printTicketDetails(TICKET* ticket) {
	printAsTicket(ticket);

	printf("GENERATED AT: %s", asctime(localtime(&ticket->time)));
	printf("TICKET RANK: %d\n", ticket->rank);
	printf("MATCHING NUMBERS: ");

	if (ticket->rank == TICKET_SIZE + 1)
		printf("NONE\n");
	else {
		for (int i = 0; i < ticket->size; i++) {
			if (checkIfContained(chosenTicket->numbers, chosenTicket->size, ticket->numbers[i]))
				printf("%d ", ticket->numbers[i]);
		}

		printf("\n");
	}
}

#endif