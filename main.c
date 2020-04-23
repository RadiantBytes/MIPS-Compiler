#include <stdio.h>
#include "SymTab.h"
#include "IOMngr.h"
#include "Semantics.h"
#include "LinkedNodes.h"

extern int yyparse();

SymTab *table;
//SymTab *ProcSymTab;
//struct SymEntry *entry;
//int inProc =0;
FILE *aFile;

int main(int argc, char * argv[]) {
	table = createSymTab(33);
	//assumes there is a listing file
	openFiles(argv[1], argv[2]);
	if (argc == 4)
		aFile = fopen(argv[3], "w");
	else
		aFile = stdout;


	// Initiate string_node linked list
	initiateLists();


	struct ExprRes *res1Buffer = malloc(sizeof(struct ExprRes));
	struct ExprRes *res2Buffer = malloc(sizeof(struct ExprRes));

	yyparse();
}
