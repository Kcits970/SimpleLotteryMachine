#ifndef LINKEDLIST_H_
#define LINKEDLIST_H_

#define MAX_LIST_SIZE 4

typedef struct node {
	void* value;
	struct node* prev;
	struct node* next;
} LIST_NODE;

LIST_NODE* createNewNode(void* value) {
	LIST_NODE* node = malloc(sizeof(LIST_NODE));
	node->value = value;
	node->prev = NULL;
	node->next = NULL;

	return node;
}

void deleteNode(LIST_NODE* node, void (*del)(void*)) {
	(*del)(node->value);
	free(node);
}

typedef struct list {
	LIST_NODE* header;
	LIST_NODE* terminal;
	int size;
} LINKED_LIST;


LINKED_LIST* createEmptyList() {
	LINKED_LIST* list = malloc(sizeof(LINKED_LIST));
	list->header = NULL;
	list->terminal = NULL;
	list->size = 0;

	return list;
}

void printAllValues(LINKED_LIST* list, const char* prefix, void (*print)(void*)) {
	int index = 0;

	for (LIST_NODE* currentNode = list->header; currentNode != NULL; currentNode = currentNode->next) {
		printf("%s %-3.3d: ", prefix, ++index);
		(*print)(currentNode->value);
	}
}

LIST_NODE* getNodeAt(LINKED_LIST* list, int index) {
	if (index < 0 || index > list->size - 1) return NULL;

	LIST_NODE* currentNode;
	int currentNodeIndex = 0;

	for (currentNode = list->header; currentNode != NULL; currentNode = currentNode->next)
		if (currentNodeIndex++ == index) break;

	return currentNode;
}

void* getValueAt(LINKED_LIST* list, int index) {
	LIST_NODE* node = getNodeAt(list, index);

	if (node != NULL)
		return node->value;
	else
		return NULL;
}

bool removeHeaderNode(LINKED_LIST* list, void (*del)(void*)) {
	if (list->header == NULL) return false;

	LIST_NODE* originalHeader = list->header;
	list->header = originalHeader->next;

	if (list->header == NULL)
		list->terminal = NULL;
	else
		list->header->prev = NULL;

	deleteNode(originalHeader, del);
	(list->size)--;

	return true;
}

bool removeTerminalNode(LINKED_LIST* list, void (*del)(void*)) {
	if (list->terminal == list->header)
		return removeHeaderNode(list, del);
	else {
		LIST_NODE* originalTerminal = list->terminal;
		list->terminal = originalTerminal->prev;
		list->terminal->next = NULL;

		deleteNode(originalTerminal, del);
		(list->size)--;

		return true;
	}
}

bool removeNodeAt(LINKED_LIST* list, int index, void (*del)(void*)) {
	if (index < 0 || index > list->size - 1)
		return false;
	else if (index == 0)
		return removeHeaderNode(list, del);
	else if (index == list->size - 1)
		return removeTerminalNode(list, del);
	else {
		LIST_NODE* nodeToRemove = getNodeAt(list, index);
		nodeToRemove->prev->next = nodeToRemove->next;
		nodeToRemove->next->prev = nodeToRemove->prev;

		deleteNode(nodeToRemove, del);
		(list->size)--;

		return true;
	}
}

void clearList(LINKED_LIST* list, void (*del)(void*)) {
	while (removeHeaderNode(list, del));
}

void appendToBlankList(LINKED_LIST* list, void* value) {
	LIST_NODE* header = createNewNode(value);

	list->header = header;
	list->terminal = header;
	(list->size)++;
}

void appendAtStart(LINKED_LIST* list, void* value, void(*del)(void*)) {
	//new values are appended at the start of the given list.
	//function pointer del is needed in case the list exceeds the specified max size.

	if (list->size == MAX_LIST_SIZE)
		removeNodeAt(list, list->size - 1, del);

	if (list->size == 0)
		appendToBlankList(list, value);
	else {
		LIST_NODE* newHeader = createNewNode(value);
		LIST_NODE* originalHeader = list->header;

		list->header = newHeader;
		newHeader->next = originalHeader;
		originalHeader->prev = newHeader;

		(list->size)++;
	}
}

#endif