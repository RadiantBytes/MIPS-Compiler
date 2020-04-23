
/* Strings to be added to .data section of this instance */
typedef struct string_node {
  char string_name[32];
  char string_contents[512];
  struct string_node *next;
} string_node;


struct string_node *add_node(struct string_node *head, struct string_node *node);
int compare_node(struct string_node *n1, struct string_node *n2);
void print_strings_list(struct string_node *head);
void free_list(struct string_node *head);
