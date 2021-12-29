#define _CRT_SECURE_NO_WARNINGS
#define VERSION_ID 2

typedef enum {false=0, true=1} bool;

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "userinput.h"
#include "ticket.h"
#include "linkedlist.h"
#include "user.h"
#include "formatting.h"

USER* user = NULL;

void printMainOptions() {
	printFormattedTitle("MAIN OPTIONS");
	printFormattedOption("PURCHASE TICKET", 'A');
	printFormattedOption("BULK PURCHASE TICKETS", 'B');
	printFormattedOption("SELL TICKET", 'C');
	printFormattedOption("SELL ALL TICKETS", 'D');
	printFormattedOption("VIEW TICKET DETAILS", 'E');
	printFormattedOption("VIEW ALL TICKETS", 'F');
	printFormattedOption("VIEW USER STATUS", 'G');
	printFormattedOption("EXIT", 'Q');
}

TICKET_TYPE getLotteryTypeSelection() {
	printFormattedOption("AUTO TICKET", 'A');
	printFormattedOption("MANUAL TICKET", 'B');

	if (getCharacterInput("SELECT A TYPE: ") == 'A')
		return TYPE_AUTO;
	else
		return TYPE_MANUAL;
}

int getNthTicketSelection() {
	printFormattedTitle("TICKET SELECTION PROMPT");
	printTicketList(user);

	return getNumberInput("SELECT A TICKET: ", 1, INT_MAX);
}

void promptPurchase() {
	printFormattedTitle("TICKET PURCHASE PROMPT");

	if (!canPurchase(user, DEFAULT_TICKET_PRICE)) {
		printf("UNABLE TO PURCHASE TICKET: NOT ENOUGH CASH\n");
		return;
	}

	addCash(user, -DEFAULT_TICKET_PRICE);
	TICKET* ticket = getTicket(getLotteryTypeSelection());
	addTicket(user, ticket);
	printf("USER NOW HAS $%d\n", user->cash);

	getCharacterInput(">>");
}

void promptBulkPurchase() {
	printFormattedTitle("TICKET BULK PURCHASE PROMPT");

	int numOfTickets = getNumberInput("ENTER THE PREFERRED NUMBER OF TICKETS: ", 1, MAX_LIST_SIZE);
	if (!canPurchase(user, numOfTickets * DEFAULT_TICKET_PRICE)) {
		printf("UNABLE TO PURCHASE %d TICKET(S): NOT ENOUGH CASH\n", numOfTickets);
		return;
	}

	addCash(user, -DEFAULT_TICKET_PRICE * numOfTickets);
	int numOfAutoTickets = getNumberInput("ENTER THE PREFERRED NUMBER OF AUTO TICKETS: ", 0, numOfTickets);
	int numOfManualTickets = numOfTickets - numOfAutoTickets;
	printf("GENERATING %d AUTO TICKET(S), %d MANUAL TICKET(S)\n", numOfAutoTickets, numOfManualTickets);

	for (int i = 0; i < numOfAutoTickets; i++) {
		addTicket(user, getTicket(TYPE_AUTO));
	}

	for (int i = 0; i < numOfManualTickets; i++) {
		addTicket(user, getTicket(TYPE_MANUAL));
	}
	printf("USER NOW HAS $%d\n", user->cash);

	getCharacterInput(">>");
}

void promptTicketSale() {
	int n = getNthTicketSelection();
	TICKET* ticket = getNthTicket(user, n);

	printFormattedTitle("TICKET SALE PROMPT");
	if (ticket != NULL) {
		int worth = ticket->worth;
		sellNthTicket(user, n);
		addCash(user, worth);

		printf("TICKET SOLD FOR $%d\n", worth);
		printf("USER NOW HAS $%d\n", user->cash);
	}
	else
		printf("UNABLE TO FIND TICKET AT INDEX %d\n", n);

	getCharacterInput(">>");
}

void promptAllTicketSale() {
	printFormattedTitle("TICKET SALE PROMPT");

	if (user->ticketList->size == 0)
		printf("NO TICKETS ARE AVAILABLE TO SELL\n");
	else {
		for (TICKET* ticket = getNthTicket(user, 1); ticket != NULL; ticket = getNthTicket(user, 1)) {
			int worth = ticket->worth;
			sellNthTicket(user, 1);
			addCash(user, worth);
			printf("TICKET SOLD FOR $%d\n", worth);
		}

		printf("USER NOW HAS $%d\n", user->cash);
	}

	getCharacterInput(">>");
}

void promptTicketDetails() {
	int n = getNthTicketSelection();
	TICKET* ticket = getNthTicket(user, n);

	printFormattedTitle("TICKET DETAILS");
	if (ticket != NULL)
		printTicketDetails(ticket);
	else
		printf("UNABLE TO FIND TICKET AT INDEX %d\n", n);

	getCharacterInput(">>");
}

void promptAllTickets() {
	printFormattedTitle("LIST OF ALL TICKETS");
	printTicketList(user);

	getCharacterInput(">>");
}

void promptUserDetails() {
	printFormattedTitle("USER STATUS");
	printUserStatus(user);

	getCharacterInput(">>");
}

void promptMainMenu() {
	char optionSelection = 0;
	bool exitMainPrompt = false;

	while (!exitMainPrompt) {
		printMainOptions();
		optionSelection = getCharacterInput(">>");

		switch (optionSelection) {
		case 'A': promptPurchase(); break;
		case 'B': promptBulkPurchase(); break;
		case 'C': promptTicketSale(); break;
		case 'D': promptAllTicketSale(); break;
		case 'E': promptTicketDetails(); break;
		case 'F': promptAllTickets(); break;
		case 'G': promptUserDetails(); break;
		case 'Q': exit(0);
		default:
			printf("'%c' IS NOT AN IDENTIFIABLE COMMAND\n", optionSelection);
			break;
		}
	}
}

int main(void) {
	printf(">>LOTTERY MACHINE: VERSION %d\n", VERSION_ID);

	generateChosenTicket();
	user = createNewUser();
	promptMainMenu();

	return 0;
}