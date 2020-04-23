#include "SymTab.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>


// Create a hash key of a SymEntry (name, attribute) pair
// Modulus this value by SymTab size to determine
// a hash bucket value.
// http://www.cse.yorku.ca/~oz/hash.html
unsigned long getHashKey(char *name);

// Keep track of the SymTab's current's hash bucket
int symTabCurrBucket = 0;

struct SymTab * createSymTab(int size) {

	struct SymTab *newSymTab = malloc(sizeof(SymTab));

	newSymTab->contents = malloc(size * sizeof(SymEntry*));

	// Initialize all SymTab contents to NULL
	int i;
	for (i = 0; i < size; i++) {

		newSymTab->contents[i] = NULL;
	}

	newSymTab->size = size;

	printf("\n##A new SymTab has been created!\n");
	return newSymTab;
}

void destroySymTab(SymTab *table) {

	if (!table) {
		return;
	}

	struct SymEntry *temp = malloc(sizeof(SymEntry));
	struct SymEntry *nextTemp = malloc(sizeof(SymEntry));

	int i;
	for (i = 0; i < table->size; i++) {

		temp = table->contents[i];

		while (temp) {
			nextTemp = temp->next;
			free(temp->name);

			free(temp);
			temp = nextTemp;

		}

		free(nextTemp);
	}

	free(table->contents);
	free(table);

	printf("\n#SymTab destroyed\n");
}

int isEmpty(SymTab * table) {
	int i;
	for (i = 0; i < table->size; i++) {

		if (table->contents[i] != NULL)
			return 0;

	}
	return 1;
}

int enterName(SymTab * table, char *name) {

	// check if the name already exists in the SymTab
	if (findName(table, name) == 1) {
		return 0;
	}


	// if the name does not already exist in the SymTab, create it
	struct SymEntry *newSymEntry = malloc(sizeof(SymEntry));

	// hashing key for the new entry
	unsigned long key;
	key = getHashKey(name);

	int bucketNum;
	bucketNum = key % table->size;

	// Set newSymEntry->name to the name parameter
	newSymEntry->name = malloc(strlen(name) + 1);

	strcpy(newSymEntry->name, name);

	// Initialize newSymEntry->attribute to NULL
	newSymEntry->attribute = NULL;

	// Link the newSymEntry to other SymEntrys saved in SymTab
	newSymEntry->next = table->contents[bucketNum];

	table->contents[bucketNum] = newSymEntry;

	table->current = newSymEntry;
	symTabCurrBucket = bucketNum;

	//printf("#Added %s successfully into hash bucket %d\n", name, bucketNum);
	return 1;
}

int findName(SymTab *table, char *name) {

	struct SymEntry *temp = malloc(sizeof(SymEntry));

	// search the SymTab for name, starting at the first pair in the SymTab

	int i;
	for (i = 0; i < table->size; i++) {

		temp = table->contents[i];

		// check all pairs within this bucket
		while (temp != NULL) {
			if (strcmp(temp->name, name) == 0) {
				table->current = temp;
				return 1;
			}
			else {
				temp = temp->next;
			}
		}
	}

	// if reached, the name is not in the SymTab
	return 0;
}

int hasCurrent(SymTab *table) {

	return ((table->current->name != NULL) && (table->current->attribute != NULL));
}

void setCurrentAttr(SymTab *table, void * attr) {

	table->current->attribute = attr;

}

void * getCurrentAttr(SymTab *table) {

	return table->current->attribute;

}

char * getCurrentName(SymTab *table) {

	return table->current->name;
}

int startIterator(SymTab *table) {

	// Find the first SymEntry in the table; Set it to current
	int i;
	for (i = 0; i < table->size; i++) {
		if (table->contents[i] != NULL) {
			table->current = table->contents[i];
			symTabCurrBucket = i;

			//printf("\n##SymTab current reset to %s\n", table->current->name);
			return 1;
		}
	}

	return 0;
}

int nextEntry(SymTab *table) {

	// check if the current's next exists in the current bucket
	if (table->current->next != NULL) {
		table->current = table->current->next;
		//printf("\n#SymTab current set to %s\n", table->current->name);

		return 1;
	}

	// else, check the next bucket(s) in the SymTab
	else {

		int i;
		for (i = symTabCurrBucket + 1; i < table->size; i++) {

			if (table->contents[i] != NULL) {
				table->current = table->contents[i];
				symTabCurrBucket = i;

				//printf("\n#SymTab current set to %s\n", table->current->name);

				return 1;
			}
		}
	}

	// if reached, all (name, attribute) pairs have been visited
	return 0;

}

unsigned long getHashKey(char *name) {

	unsigned long hash = 5381;
	int c;

	while (c = *name++)
		hash = ((hash << 5) + hash) + c; /* hash * 33 + c */

	return hash;

}

//  int main() {
//
//  	SymTab * table;
//
//  	table = createSymTab(5);
//
// // DEBUG
//  	printf("The size of the new SymTab is %d\n", table->size);
//
//  	enterName(table, "Red");
//  	enterName(table, "Blue");
//  	enterName(table, "Green");
//  	enterName(table, "Yellow");
//  	enterName(table, "Pink");
//  	enterName(table, "Purple");
//  	enterName(table, "Orange");
//  	enterName(table, "Black");
//
//  	enterName(table, "White");
// 	enterName(table, "White");
// 	enterName(table, "White");
//
//
//
//  	int i = startIterator(table);
//
// 	int count = 0;
//
// 	while (count != 10) {
//  		i = nextEntry(table);
//  		count++;
// 	}
//
//
// 	printf("\ngetCurrentName returns %s\n", getCurrentName(table));
// 	printf("(This should print 'yellow')\n");
//
//  	i = startIterator(table);
//
// 	printf("\ngetCurrentName returns %s\n", getCurrentName(table));
// 	printf("(This should print the first entry in hash bucket 0, Green)\n");
//
//
//  	setCurrentAttr(table, "setExampleAttr");
//
//  	printf("\ngetCurrentAttr returns %s\n", (char *)getCurrentAttr(table));
//
// 	printf("\nhasCurrent returns %d\n", hasCurrent(table));
//
// 	printf("\nfindName searching for 'White' returns %d\n", findName(table, "White"));
//
// 	printf("\nfindName searching for 'NotInTable' returns %d\n", findName(table, "NotInTable"));
//
//  	// NOTE: enterName does not like char, use string
//
//  	printf("\n\n##Testing more stuff...\n\n");
//
// 	printf("findName before adding ';' returns %d\n", findName(table, ";"));
//
// 	enterName(table, ";");
//
//  	printf("findName after adding ';' returns %d\n", findName(table, ";"));
//
// 	destroySymTab(table);
//
//  	return 0;
//  }
