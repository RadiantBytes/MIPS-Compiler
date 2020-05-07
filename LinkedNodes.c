/* LinkedNodes.c */
/* This C code manages the Linked Node Lists used in Semantics.c
  that is used to store strings to be printed and loop points */

#include <strings.h>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>

#include "LinkedNodes.h"
#include "Semantics.h"

/* Add a new node to list */
struct string_node *add_node(struct string_node *head, struct string_node *node) {
	struct string_node *prev, *next;
	if (!head) {
		head = node;
	}
	else {
		prev = NULL;
		next = head;
		while (next && compare_node(node,next)>0) {
			prev = next;
			next = next->next;
		}
		if (!next) {
			prev->next = node;
		}
		else {
			if (prev) {
				node->next = prev->next;
				prev->next = node;
			}
			else {
				node->next = head;
				head = node;
			}
		}
	}
  /* DEBUG */
	//printf("\nAdded %s : %s to string_list\n", node->string_name, node->string_contents);
	return head;
}

/* Helper function to place nodes in correct order alphabetically by string_name */
int compare_node(struct string_node *n1, struct string_node *n2) {
	return strcmp(n1->string_name, n2->string_name);
}

/* DEBUG: Print contents of strings_list */
void print_strings_list(struct string_node *head) {
  printf("\n# Printing contents of string list...\n");
  string_node *prev = head;
	string_node *cur = head;

  int i = 1;
	while (cur) {
		prev = cur;
		cur = prev->next;
		printf("%d.\t%s : %s\n",i,prev->string_name, prev->string_contents);
    i++;
	}
}

/* Free list from memory*/
void free_list(struct string_node *head) {
	string_node *prev = head;
	string_node *cur = head;
	while (cur) {
		prev = cur;
		cur = prev->next;
		free(prev);
	}
}
