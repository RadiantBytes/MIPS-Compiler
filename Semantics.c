/* Semantics.c
   Support and semantic action routines.

*/

#include <strings.h>
#include <string.h>
#include <stdlib.h>

#include "CodeGen.h"
#include "Semantics.h"
#include "SymTab.h"
#include "IOMngr.h"
#include "LinkedNodes.h"

extern SymTab *table;
string_node *head;
string_node *array_head;

/* doIf jump-point seems to add ahead by one between doRelationOp and doIf,
 Use a buffer to remember the correct if jump-point name */
char ifBuffer[8];
char elseBuffer[8];

char startLoop[8];

int ifBoolBuffer;

int whileReg;

int boolDestReg;

struct ExprRes *res1Buffer;
struct ExprRes *res2Buffer;

int tempReg;


/* Semantics support routines */

void initiateLists() {

	// Initiates list of string declarations
	head = NULL;
	// Initiates list of array declarations
	array_head = NULL;

	return;
}
struct ExprRes *  doIntLit(char * digits)  {

	struct ExprRes *res;

	res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
	res->Reg = AvailTmpReg();
	res->Instrs = GenInstr(NULL,"li",TmpRegName(res->Reg),digits,NULL);

	return res;
}

struct ExprRes *  doRval(char * name)  {

	struct ExprRes *res;

	if (!findName(table, name)) {
		writeIndicator(getCurrentColumnNum());
		writeMessage("Undeclared variable");
	}
	res = (struct ExprRes *) malloc(sizeof(struct ExprRes));
	res->Reg = AvailTmpReg();
	res->Instrs = GenInstr(NULL,"lw",TmpRegName(res->Reg),name,NULL);

	return res;
}

void doInitializeArray() {

	char * temp;
	temp = (char*)malloc(strlen(getBuffer()) * sizeof(char));

	strcpy(temp, getBuffer());

	// Fetch array variable name and size from getBuffer()
	char * arrName;
	arrName = (char*)malloc(strlen(getBuffer()) * sizeof(char));

	int openBracketIndex;
	int closeBracketIndex;
	int nameStartIndex;

	char c;
	int i = 0;
	for(i; i < strlen(temp); i++) {
		c = temp[i];

		if (c == ' ') {
			nameStartIndex = i+1;
		}
		else if (c == '[') {
			openBracketIndex = i;
		}
		else if (c == ']') {
			closeBracketIndex = i;
		}

	}

	// Grab name of array and size from the declaration line of code
	strncpy(arrName, (temp+nameStartIndex), openBracketIndex - nameStartIndex);

	char * tempSize;
	tempSize = (char*)malloc(strlen(getBuffer()) * sizeof(char));

	strncpy(tempSize, (temp+openBracketIndex+1), closeBracketIndex - openBracketIndex-1);

	// Add array name and array size to a linked list node
	string_node *newNode;

	newNode = (string_node*)malloc(sizeof(string_node));

	// Array Name
	strcpy(newNode->string_name, arrName);
	// Array Size
	strcpy(newNode->string_contents, tempSize);

	// Add this newNode to the array list
	newNode->next = NULL;
	array_head = add_node(array_head, newNode);
}

struct InstrSeq *  doIndexAssign(struct ExprRes * Res2) {

		char * temp;
		temp = (char*)malloc(strlen(getBuffer()) * sizeof(char));

		strcpy(temp, getBuffer());

		struct InstrSeq *code;

		// if (!findName(table, name)) {
		// 	writeIndicator(getCurrentColumnNum());
		// 	writeMessage("Undeclared variable");
		// }

		// Fetch array variable name and size from getBuffer()
		char * arrName;
		arrName = (char*)malloc(strlen(getBuffer()) * sizeof(char));

		int openBracketIndex;
		int closeBracketIndex;
		int nameStartIndex;

		char c;
		int i = 0;
		for(i; i < strlen(temp); i++) {
			c = temp[i];


		  if (c == '[') {
				openBracketIndex = i;
			}


		}

		// Grab name of array and size from the declaration line of code
		strncpy(arrName, temp+1, openBracketIndex-1);


		int reg;
		reg = AvailTmpReg();

		code = Res2->Instrs;

		AppendSeq(code,GenInstr(NULL,"la",TmpRegName(reg),arrName,NULL));
		AppendSeq(code,GenInstr(NULL,"sll",TmpRegName(tempReg),TmpRegName(tempReg),"2"));
		AppendSeq(code,GenInstr(NULL,"add",TmpRegName(reg),TmpRegName(reg),TmpRegName(tempReg)));

		// string representation of how to access array index in MIPS
		char *arrTemp;
		arrTemp = (char*)malloc(7 * sizeof(char));
		strcat(arrTemp, "0(");
		strcat(arrTemp, TmpRegName(reg));
		strcat(arrTemp, ")");

		AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(Res2->Reg),arrTemp,NULL));

		//ReleaseTmpReg(Expr->Reg);

		return code;

}

struct ExprRes *  doAdd(struct ExprRes * Res1, struct ExprRes * Res2)  {

	int reg;

	reg = AvailTmpReg();
	boolDestReg = reg;

	AppendSeq(Res1->Instrs,Res2->Instrs);
	AppendSeq(Res1->Instrs,GenInstr(NULL,"add",
				TmpRegName(reg),
				TmpRegName(Res1->Reg),
				TmpRegName(Res2->Reg)));
	ReleaseTmpReg(Res1->Reg);
	ReleaseTmpReg(Res2->Reg);
	Res1->Reg = reg;
	free(Res2);
	return Res1;
}


struct ExprRes *  doSub(struct ExprRes * Res1, struct ExprRes * Res2)  {

	int reg;

	reg = AvailTmpReg();
	AppendSeq(Res1->Instrs,Res2->Instrs);
	AppendSeq(Res1->Instrs,GenInstr(NULL,"sub",
				TmpRegName(reg),
				TmpRegName(Res1->Reg),
				TmpRegName(Res2->Reg)));
	ReleaseTmpReg(Res1->Reg);
	ReleaseTmpReg(Res2->Reg);
	Res1->Reg = reg;
	free(Res2);
	return Res1;
}

struct ExprRes *  doMult(struct ExprRes * Res1, struct ExprRes * Res2)  {

	int reg;

	reg = AvailTmpReg();
	AppendSeq(Res1->Instrs,Res2->Instrs);
	AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",
				TmpRegName(reg),
				TmpRegName(Res1->Reg),
				TmpRegName(Res2->Reg)));
	ReleaseTmpReg(Res1->Reg);
	ReleaseTmpReg(Res2->Reg);
	Res1->Reg = reg;
	free(Res2);
	return Res1;
}


struct ExprRes *  doMod(struct ExprRes * Res1, struct ExprRes * Res2)  {

	int reg;

	reg = AvailTmpReg();
	AppendSeq(Res1->Instrs,Res2->Instrs);
	AppendSeq(Res1->Instrs,GenInstr(NULL,"rem",
				TmpRegName(reg),
				TmpRegName(Res1->Reg),
				TmpRegName(Res2->Reg)));
	ReleaseTmpReg(Res1->Reg);
	ReleaseTmpReg(Res2->Reg);
	Res1->Reg = reg;
	free(Res2);
	return Res1;
}


struct ExprRes *  doDiv(struct ExprRes * Res1, struct ExprRes * Res2)  {

	int reg;

	reg = AvailTmpReg();
	AppendSeq(Res1->Instrs,Res2->Instrs);
	AppendSeq(Res1->Instrs,GenInstr(NULL,"div",
				TmpRegName(reg),
				TmpRegName(Res1->Reg),
				TmpRegName(Res2->Reg)));
	ReleaseTmpReg(Res1->Reg);
	ReleaseTmpReg(Res2->Reg);
	Res1->Reg = reg;
	free(Res2);
	return Res1;
}

struct ExprRes *  doNegative(struct ExprRes * Res1)  {

	int reg;

	reg = AvailTmpReg();
	AppendSeq(Res1->Instrs,GenInstr(NULL,"mul",
				TmpRegName(reg),
				TmpRegName(Res1->Reg),
				"-1"));
	ReleaseTmpReg(Res1->Reg);
	Res1->Reg = reg;
	//free(Res1);
	return Res1;
}

struct ExprRes *  doExponentiation(struct ExprRes * Res1, struct ExprRes * Res2)  {

	int reg;

	reg = AvailTmpReg();

	//AppendSeq(Res1->Instrs,Res2->Instrs);

	char *startExp = GenLabel();
	char *endExp = GenLabel();

	// Loop to complete exponentiationextern struct InstrSeq * doWhileLoop(struct BExprRes * bRes, struct InstrSeq * seq);

	AppendSeq(Res1->Instrs,Res2->Instrs);
	AppendSeq(Res1->Instrs, GenInstr(NULL, "add",TmpRegName(reg), TmpRegName(Res1->Reg), "$zero"));

	// If exponential is a '1', skip this loop entirely to preserve reg value
	AppendSeq(Res1->Instrs, GenInstr(NULL, "beq",TmpRegName(Res2->Reg), "1", endExp));

	AppendSeq(Res1->Instrs, GenInstr(startExp,NULL, NULL, NULL, NULL));
	AppendSeq(Res1->Instrs, GenInstr(NULL, "mul",TmpRegName(reg), TmpRegName(reg), TmpRegName(Res1->Reg)));
	AppendSeq(Res1->Instrs, GenInstr(NULL, "subi",TmpRegName(Res2->Reg), TmpRegName(Res2->Reg), "1"));
	AppendSeq(Res1->Instrs, GenInstr(NULL, "bne",TmpRegName(Res2->Reg), "1", startExp));
	AppendSeq(Res1->Instrs, GenInstr(endExp, NULL,NULL,NULL,NULL));


	Res1->Reg = reg;
	//free(Res1);

	return Res1;
}

struct BExprRes *  doRelationalOp(struct ExprRes * Res1, struct ExprRes * Res2, char * op)  {
	int reg;

	reg = AvailTmpReg();
	AppendSeq(Res1->Instrs,Res2->Instrs);

	struct BExprRes * bRes;
	bRes = (struct BExprRes *) malloc(sizeof(struct BExprRes));
	bRes->Label = GenLabel();

	strcpy(ifBuffer, bRes->Label);

	strcpy(elseBuffer, GenLabel());

	strcpy(startLoop, GenLabel());

	whileReg = reg;

	AppendSeq(Res1->Instrs, GenInstr(startLoop, NULL, NULL, NULL, NULL));

	printf("Label is %s.  ", bRes->Label);

	// determine which MIPS instruction will be used

	// Relational Operators
	if (strcmp(op, ">") == 0)
		AppendSeq(Res1->Instrs,GenInstr(NULL,"sgt",
					TmpRegName(reg),
					TmpRegName(Res1->Reg),
					TmpRegName(Res2->Reg)));
	else if (strcmp(op, ">=") == 0)
		AppendSeq(Res1->Instrs,GenInstr(NULL,"sge",
					TmpRegName(reg),
					TmpRegName(Res1->Reg),
					TmpRegName(Res2->Reg)));

	else if (strcmp(op, "==") == 0)
		AppendSeq(Res1->Instrs,GenInstr(NULL,"seq",
					TmpRegName(reg),
					TmpRegName(Res1->Reg),
					TmpRegName(Res2->Reg)));

	else if (strcmp(op, "!=") == 0)
		AppendSeq(Res1->Instrs,GenInstr(NULL,"sne",
					TmpRegName(reg),
					TmpRegName(Res1->Reg),
					TmpRegName(Res2->Reg)));

	else if (strcmp(op, "<=") == 0)
		AppendSeq(Res1->Instrs,GenInstr(NULL,"sle",
					TmpRegName(reg),
					TmpRegName(Res1->Reg),
					TmpRegName(Res2->Reg)));

	else if (strcmp(op, "<") == 0)
		AppendSeq(Res1->Instrs,GenInstr(NULL,"slt",
					TmpRegName(reg),
					TmpRegName(Res1->Reg),
					TmpRegName(Res2->Reg)));

	ifBoolBuffer = reg;

	res1Buffer = Res1;
	res2Buffer = Res2;

	bRes->Instrs = Res1->Instrs;
	ReleaseTmpReg(Res1->Reg);
	ReleaseTmpReg(Res2->Reg);
	free(Res1);
	free(Res2);
	return bRes;

	// Boolean Operators
	// ReleaseTmpReg(Res1->Reg);
	// ReleaseTmpReg(Res2->Reg);
	//
	//
	// Res1->Reg = reg;
	// free(Res2);
	// return Res1;
}

struct ExprRes *  doBoolNot(struct ExprRes * Res)  {

	int reg;

	reg = AvailTmpReg();
	AppendSeq(Res->Instrs,NULL);
	AppendSeq(Res->Instrs,GenInstr(NULL,"xor",
				TmpRegName(reg),
				TmpRegName(Res->Reg),"1"));
	ReleaseTmpReg(Res->Reg);
	Res->Reg = reg;

	return Res;
}

struct ExprRes *  doBoolOp(struct ExprRes * Res1, struct ExprRes * Res2, char * op)  {

	int reg;

	reg = AvailTmpReg();
	AppendSeq(Res1->Instrs,Res2->Instrs);

	if (strcmp(op, "&&") == 0)
		AppendSeq(Res1->Instrs,GenInstr(NULL,"and",
					TmpRegName(reg),
					TmpRegName(Res1->Reg),
					TmpRegName(Res2->Reg)));

	else if (strcmp(op, "||") == 0)
		AppendSeq(Res1->Instrs,GenInstr(NULL,"or",
					TmpRegName(reg),
					TmpRegName(Res1->Reg),
					TmpRegName(Res2->Reg)));

	ReleaseTmpReg(Res1->Reg);
	ReleaseTmpReg(Res2->Reg);
	Res1->Reg = reg;
	free(Res2);
	return Res1;
}

struct InstrSeq * doPrint(struct ExprRes * Expr) {

	struct InstrSeq *code;

	code = Expr->Instrs;

	AppendSeq(code,GenInstr(NULL,"li","$v0","1",NULL));
	AppendSeq(code,GenInstr(NULL,"move","$a0",TmpRegName(Expr->Reg),NULL));
	AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

	AppendSeq(code,GenInstr(NULL,"li","$v0","4",NULL));
	AppendSeq(code,GenInstr(NULL,"la","$a0","_nl",NULL));
	AppendSeq(code,GenInstr(NULL,"syscall",NULL,NULL,NULL));

	ReleaseTmpReg(Expr->Reg);
	free(Expr);

	return code;
}

struct InstrSeq * doReadIn(char *name)  {

	//printf("name : %s. ", name);
	struct InstrSeq *code;
	struct ExprRes *Expr = malloc(sizeof(struct ExprRes));

	Expr->Reg = AvailTmpReg();

	Expr->Instrs = GenInstr(NULL, "li", "$v0", "5", NULL);
	AppendSeq(Expr->Instrs,GenInstr(NULL, "syscall", NULL, NULL, NULL));
	AppendSeq(Expr->Instrs,GenInstr(NULL, "move", TmpRegName(Expr->Reg), "$v0", NULL));
	AppendSeq(Expr->Instrs,GenInstr(NULL, "sw", TmpRegName(Expr->Reg), name, NULL));


	return Expr->Instrs;

}

struct InstrSeq * doPrintLn() {

	// string_node *newNode;
	//
	// newNode = (string_node*)malloc(sizeof(string_node));
	//
	// strcpy(newNode->string_name, "newline");
	// strcpy(newNode->string_contents, "ln338");
	//
	// newNode->next = NULL;
	// head = add_node(head, newNode);

	struct ExprRes *Expr = malloc(sizeof(struct ExprRes));

	Expr->Reg = AvailTmpReg();

	Expr->Instrs = GenInstr(NULL, "li", "$v0", "4", NULL);
	AppendSeq(Expr->Instrs,GenInstr(NULL, "la", "$a0", "_nl", NULL));
	AppendSeq(Expr->Instrs,GenInstr(NULL, "syscall", NULL, NULL, NULL));

	return Expr->Instrs;

}

struct InstrSeq * doPrintSpaces(struct ExprRes * Expr) {

	 string_node *newNode;

	 newNode = (string_node*)malloc(sizeof(string_node));

	 // MIPS .data string name
	 strcpy(newNode->string_name, "add_space");
	 // Contents of the MIPS string
	 strcpy(newNode->string_contents, " ");

	 newNode->next = NULL;
	 head = add_node(head, newNode);

	//Expr->Reg = AvailTmpReg();

	//AppendSeq(Res1->Instrs,Res2->Instrs);

	char *startSpaces = GenLabel();

	// Loop and print spaces one at a time
	AppendSeq(Expr->Instrs,GenInstr(startSpaces,NULL, NULL, NULL, NULL));

	AppendSeq(Expr->Instrs,GenInstr(NULL, "li", "$v0", "4", NULL));
	AppendSeq(Expr->Instrs,GenInstr(NULL, "la", "$a0", "add_space", NULL));
	AppendSeq(Expr->Instrs,GenInstr(NULL, "syscall", NULL, NULL, NULL));
	AppendSeq(Expr->Instrs, GenInstr(NULL, "subi",TmpRegName(Expr->Reg), TmpRegName(Expr->Reg), "1"));
	AppendSeq(Expr->Instrs, GenInstr(NULL, "bgtz",TmpRegName(Expr->Reg), startSpaces, NULL));


	//free(Res1);

	return Expr->Instrs;

}


struct InstrSeq * doAssign(char *name, struct ExprRes * Expr) {

	struct InstrSeq *code;


	if (!findName(table, name)) {
		writeIndicator(getCurrentColumnNum());
		writeMessage("Undeclared variable");
	}

	code = Expr->Instrs;

	AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(Expr->Reg), name,NULL));

	res1Buffer = Expr;
	tempReg = Expr->Reg;

	ReleaseTmpReg(Expr->Reg);
	free(Expr);

	return code;
}

struct InstrSeq * doBoolAssign(char boolVal) {

	struct ExprRes *Expr = malloc(sizeof(struct ExprRes));

	char * temp;
	temp = (char*)malloc(strlen(getBuffer()) * sizeof(char));

	strcpy(temp, getBuffer());

	struct InstrSeq *code;

	// if (!findName(table, name)) {
	// 	writeIndicator(getCurrentColumnNum());
	// 	writeMessage("Undeclared variable");
	// }

	// Fetch array variable name and size from getBuffer()
	char * varName;
	varName = (char*)malloc(strlen(getBuffer()) * sizeof(char));

	int equalsIndex;
	int closeBracketIndex;
	int nameStartIndex;

	char c;
	int i = 0;
	for(i; i < strlen(temp); i++) {
		c = temp[i];


		if (c == '=') {
			equalsIndex = i;
		}


	}

	// Grab name of array and size from the declaration line of code
	strncpy(varName, temp+1, equalsIndex-1);

	printf("varName is %s. ", varName);
	printf("boolVal is %c", boolVal);

	AppendSeq(code,GenInstr(NULL,"sw",TmpRegName(Expr->Reg),varName,NULL));

	ReleaseTmpReg(Expr->Reg);
	free(Expr);

	return code;
}

extern struct InstrSeq * doIf(struct BExprRes * bRes, struct InstrSeq * seq) {
	struct InstrSeq * seq2;
	seq2 = bRes->Instrs;

	struct ExprRes *Expr = malloc(sizeof(struct ExprRes));

	Expr->Reg = ifBoolBuffer;

	//Changed beq to bne 4-14-2020 11:44AM
	//AppendSeq(seq2, GenInstr(NULL, "beq", TmpRegName(Expr->Reg), "$0", end_label));
	AppendSeq(seq2, GenInstr(NULL, "bne", TmpRegName(ifBoolBuffer), "1", ifBuffer));

	AppendSeq(seq2, seq);
	AppendSeq(seq2, GenInstr(ifBuffer, NULL, NULL, NULL, NULL));
	free(bRes);
	return seq2;
}

struct InstrSeq * doIfElse(struct BExprRes * bRes, struct InstrSeq * seq, struct InstrSeq * seq2) {
	struct InstrSeq * seq3;
	seq3 = bRes->Instrs;

	struct ExprRes *Expr = malloc(sizeof(struct ExprRes));

	Expr->Reg = ifBoolBuffer;

	//Changed beq to bne 4-14-2020 11:44AM
	//AppendSeq(seq2, GenInstr(NULL, "beq", TmpRegName(Expr->Reg), "$0", end_label));
	AppendSeq(seq3, GenInstr(NULL, "bne", TmpRegName(ifBoolBuffer), "1", elseBuffer));

	AppendSeq(seq3, seq);
	// If the if is true, branch to after the if-else clause
	AppendSeq(seq3, GenInstr(NULL, "b", ifBuffer, NULL, NULL));

	// Jump-point to else clause
	AppendSeq(seq3, GenInstr(elseBuffer, NULL, NULL, NULL, NULL));
	//Append from ELSE statement
	AppendSeq(seq3, seq2);
	// Make a branch point after the if-else clause
	AppendSeq(seq3, GenInstr(ifBuffer, NULL, NULL, NULL, NULL));
	free(bRes);
	return seq3;
}

struct InstrSeq * doWhileLoop(struct BExprRes * bRes, struct InstrSeq * seq) {

	struct InstrSeq * seq2;
	seq2 = bRes->Instrs;

	struct ExprRes *Expr = malloc(sizeof(struct ExprRes));

	Expr->Reg = ifBoolBuffer;

	char *endLoop = GenLabel();

	struct BExprRes *redoIf = malloc(sizeof(struct BExprRes));

	AppendSeq(seq2, GenInstr(NULL, "bne", TmpRegName(Expr->Reg), "1", elseBuffer));
	AppendSeq(seq2, seq);
	AppendSeq(seq2, GenInstr(NULL, "move", TmpRegName(boolDestReg), TmpRegName(whileReg), NULL));
	AppendSeq(seq2, GenInstr(NULL, "j", startLoop, NULL, NULL));
	AppendSeq(seq2, GenInstr(elseBuffer, NULL, NULL, NULL, NULL));

	free(Expr);
	free(bRes);
	return seq2;

}

struct InstrSeq * doForLoop() {
	printf("Test");
}

struct InstrSeq * doFunction(struct InstrSeq * ident, struct InstrSeq * params, struct InstrSeq * stmtSeq, char *functName) {
	printf("In doFunction");

	printf("\n%s.", functName);


	return NULL;
}

struct InstrSeq * doDeclFunct(struct InstrSeq * seq) {

	struct InstrSeq * seq2;

	char * temp;
	temp = (char*)malloc(strlen(getBuffer()) * sizeof(char));

	strcpy(temp, getBuffer());

	char * functName;
	functName = (char*)malloc(strlen(getBuffer()) * sizeof(char));

	int openParensIndex;
	int nameStartIndex;

	char c;
	int i = 0;
	for(i; i < strlen(temp); i++) {
		c = temp[i];

		if (c == '(') {
			openParensIndex = i;
		}
		if (c == ' ') {
			nameStartIndex = i;
		}


	}

	// Grab name of array and size from the declaration line of code
	strncpy(functName, temp+nameStartIndex+1, openParensIndex-5);

	seq2 = GenInstr(functName, NULL, NULL, NULL, NULL);
	//AppendSeq(seq2, seq);

	return seq2;
}

struct InstrSeq * doFunctCall(struct InstrSeq * seq) {
	struct InstrSeq * seq2;

	char * temp;
	temp = (char*)malloc(strlen(getBuffer()) * sizeof(char));

	strcpy(temp, getBuffer());

	char * functName;
	functName = (char*)malloc(strlen(getBuffer()) * sizeof(char));

	int openParensIndex;
	int nameStartIndex;

	char c;
	int i = strlen(temp)-1;
	for(i; i >= 0; i--) {
		c = temp[i];

		if (c == '(') {
			openParensIndex = i;
			printf("found open at %d", i);
		}
		if ((c == ' ') || (i == 0)) {
			nameStartIndex = i;
			printf("found start at %d", i);
		}


	}

	// Grab name of array and size from the declaration line of code
	strncpy(functName, temp+nameStartIndex+1, openParensIndex-1);

	printf("Funct is .%s.", functName);

	seq2 = GenInstr(NULL, "j", functName, NULL, NULL);

	return seq2;
}

/*

   extern struct InstrSeq * doIf(struct ExprRes *res1, struct ExprRes *res2, struct InstrSeq * seq) {
   struct InstrSeq *seq2;
   char * label;
   label = GenLabel();
   AppendSeq(res1->Instrs, res2->Instrs);
   AppendSeq(res1->Instrs, GenInstr(NULL, "bne", TmpRegName(res1->Reg), TmpRegName(res2->Reg), label));
   seq2 = AppendSeq(res1->Instrs, seq);
   AppendSeq(seq2, GenInstr(label, NULL, NULL, NULL, NULL));
   ReleaseTmpReg(res1->Reg);
   ReleaseTmpReg(res2->Reg);
   free(res1);
   free(res2);
   return seq2;
   }

*/
	void
Finish(struct InstrSeq *Code)
{ struct InstrSeq *code;
	//struct SymEntry *entry;
	int hasMore;
	struct Attr * attr;


	code = GenInstr(NULL,".data",NULL,NULL,NULL);

	/*
	 * Add information to .data section from node lists
	 */

	// Add array .space info to .data section in MIPS
	// Concurrently free the string_nodes after AppendSeq
	string_node *prev = array_head;
	string_node *cur = array_head;
	char *sizeTemp;

	while (cur) {
		prev = cur;
		cur = prev->next;

		// Put double-quotes around string_contents
		sizeTemp = malloc(strlen(prev->string_contents) + 2);

		int arrSize = atoi(prev->string_contents);
		arrSize = arrSize * 4;

		sprintf(sizeTemp, "%d", arrSize);

		// Append string declaration
		AppendSeq(code, GenInstr(prev->string_name,".space",sizeTemp,NULL,NULL));

		free(prev);
	}


	// Add string(s) to .data section in MIPS
	// Concurrently free the string_nodes after AppendSeq
	prev = head;
	cur = head;

	char *temp;
	while (cur) {
		prev = cur;
		cur = prev->next;

		// Put double-quotes around string_contents
		temp = malloc(strlen(prev->string_contents) + 2);

		strcpy(temp,"\"");
		strcat(temp, prev->string_contents);
		strcat(temp, "\"");

		// Append string declaration
		AppendSeq(code, GenInstr(prev->string_name,".asciiz",temp,NULL,NULL));

		free(prev);
	}

	AppendSeq(code,GenInstr(NULL,".text",NULL,NULL,NULL));
	//AppendSeq(code,GenInstr(NULL,".align","2",NULL,NULL));
	AppendSeq(code,GenInstr(NULL,".globl","main",NULL,NULL));
	AppendSeq(code, GenInstr("main",NULL,NULL,NULL,NULL));
	AppendSeq(code,Code);
	AppendSeq(code, GenInstr(NULL, "li", "$v0", "10", NULL));
	AppendSeq(code, GenInstr(NULL,"syscall",NULL,NULL,NULL));
	AppendSeq(code,GenInstr(NULL,".data",NULL,NULL,NULL));
	AppendSeq(code,GenInstr(NULL,".align","4",NULL,NULL));
	AppendSeq(code,GenInstr("_nl",".asciiz","\"\\n\"",NULL,NULL));

	hasMore = startIterator(table);
	while (hasMore) {
		AppendSeq(code,GenInstr((char *) getCurrentName(table),".word","0",NULL,NULL));
		hasMore = nextEntry(table);
	}

	WriteSeq(code);

	return;
}
