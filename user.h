#ifndef USER_H_
#define USER_H_

#include "ticket.h"
#include "linkedlist.h"

typedef struct {
	int cash;
	int ticketsBought;
	int ticketsSold;
	LINKED_LIST* ticketList;
} USER;

USER* createNewUser() {
	USER* user = malloc(sizeof(USER));
	user->cash = 100;
	user->ticketsBought = 0;
	user->ticketsSold = 0;
	user->ticketList = createEmptyList();
	return user;
}

void addCash(USER* user, int amount) {
	user->cash += amount;
}

void addTicket(USER* user, TICKET* ticket) {
	user->ticketsBought++;
	 
	if (ticket != NULL) {
		appendAtStart(user->ticketList, ticket, &deleteTicket);
		printf("NEW TICKET ADDED\n");
	}
	else
		printf("FAILED TO ADD NEW TICKET\n");
}

void printTicketList(USER* user) {
	printAllValues(user->ticketList, "TICKET", &printAsTicket);
}

TICKET* getNthTicket(USER* user, int n) {
	return (TICKET*) getValueAt(user->ticketList, n - 1);
}

bool sellNthTicket(USER* user, int n) {
	bool removed = removeNodeAt(user->ticketList, n - 1, &deleteTicket);
	if (removed) user->ticketsSold++;

	return removed;
}

bool canPurchase(USER* user, int price) {
	if (user->cash < price)
		return false;
	else
		return true;
}

void printUserStatus(USER* user) {
	printf("CASH: $%d\n", user->cash);
	printf("NUMBER OF TICKETS BOUGHT: %d\n", user->ticketsBought);
	printf("NUMBER OF TICKETS SOLD: %d\n", user->ticketsSold);
	printf("NUMBER OF OWNED TICKETS: %d\n", user->ticketList->size);
}

#endif