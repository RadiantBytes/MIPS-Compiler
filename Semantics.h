/* Semantics.h
   The action and supporting routines for performing semantics processing.
*/

/* Semantic Records */
struct IdList {
  struct SymEntry * TheEntry;
  struct IdList * Next;
};

struct ExprRes {
  int Reg;
  struct InstrSeq * Instrs;
};

struct ExprResList {
	struct ExprRes *Expr;
	struct ExprResList * Next;
};

struct BExprRes {
  char * Label;
  struct InstrSeq * Instrs;
};


/* Semantics Actions */
void initiateLists();
extern struct ExprRes *  doIntLit(char * digits);
extern struct ExprRes *  doRval(char * name);
extern struct InstrSeq *  doAssign(char * name,  struct ExprRes * Res1);
extern struct InstrSeq * doBoolAssign(char boolVal);
void doInitializeArray();
extern struct InstrSeq *  doIndexAssign(struct ExprRes * Res2);
extern struct ExprRes *  doAdd(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doSub(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct ExprRes *  doPlusPlus(struct ExprRes * Res1);
extern struct ExprRes *  doMult(struct ExprRes * Res1,  struct ExprRes * Res2);
extern struct ExprRes *  doDiv(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct ExprRes *  doMod(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct ExprRes *  doNegative(struct ExprRes * Res1);
extern struct ExprRes *  doExponentiation(struct ExprRes * Res1, struct ExprRes * Res2);
extern struct BExprRes * doRelationalOp(struct ExprRes * Res1, struct ExprRes * Res2, char * op);
extern struct ExprRes *  doBoolNot(struct ExprRes * Res);
extern struct ExprRes *  doBoolOp(struct ExprRes * Res1, struct ExprRes * Res2, char * op);
extern struct InstrSeq *  doPrint(struct ExprRes * Expr);
extern struct InstrSeq *  doReadIn(char *name);
extern struct InstrSeq *  doPrintLn();
extern struct InstrSeq * doPrintSpaces(struct ExprRes * Expr);
extern struct InstrSeq * doIf(struct BExprRes *bRes, struct InstrSeq * seq);
extern struct InstrSeq * doIfElse(struct BExprRes * bRes, struct InstrSeq * seq, struct InstrSeq * seq2);
extern struct InstrSeq * doWhileLoop(struct BExprRes * bRes, struct InstrSeq * seq);
extern struct InstrSeq * doForLoop();
extern struct InstrSeq * doFunction(struct InstrSeq * ident, struct InstrSeq * params, struct InstrSeq * stmtSeq, char *functName);
extern struct InstrSeq * doDeclFunct(struct InstrSeq * seq);
extern struct InstrSeq * doFunctCall(struct InstrSeq * seq);
extern void	Finish(struct InstrSeq *Code);
// Node *createString(char *varName, char *string);
// void queueString(Node **link, Node *newNode);
// void printAsciiStrings(Node *head);
