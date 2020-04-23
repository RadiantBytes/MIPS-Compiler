%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "IOMngr.h"
#include "SymTab.h"
#include "Semantics.h"
#include "CodeGen.h"

extern int yylex();	/* The next token function. */
extern char *yytext;   /* The matched token text.  */
extern int yyleng;      /* The token text length.   */
extern int yyparse();
extern int yyerror(char *);
void dumpTable();

extern SymTab *table;

%}


%union {
  long val;
  char * string;
  struct ExprRes * ExprRes;
  struct InstrSeq * InstrSeq;
  struct BExprRes * BExprRes;
  struct Id *Id;
}

%type <string> Id
%type <ExprRes> Factor
%type <ExprRes> Term
%type <ExprRes> Expr
%type <InstrSeq> StmtSeq
%type <InstrSeq> Stmt
%type <InstrSeq> ReadVars
%type <InstrSeq> RVar
%type <InstrSeq> ExprVal
%type <BExprRes> BExpr
%type <InstrSeq> ExprList


%token Ident
%token IntLit
%token Int
%token Write
%token IF
%token EQ
%token NOTEQ
%token GTE
%token LTE
%token AND
%token OR
%token WriteLn
%token Read
%token ReadVal
%token PrintSpaces
%token While
%token ELSE
%token POW


%%

Prog            :	     Declarations StmtSeq					        	{Finish($2); } ;
Declarations    :	     Dec Declarations						         	  { };
Declarations  	:		                                          { };
Dec		         	:	     Int Ident                              {enterName(table, yytext); }';'	{};
StmtSeq     		:	     Stmt StmtSeq							            	{$$ = AppendSeq($1, $2); } ;
StmtSeq	      	:		 	                                        {$$ = NULL;} ;
Stmt	       		:	     Write '(' ExprList ')' ';'							{$$ = $3; };
Stmt            :      WriteLn ';'                            {$$ = doPrintLn(); } ;
Stmt            :      PrintSpaces '(' Expr ')' ';'           {$$ = doPrintSpaces($3);};
Stmt            :      Read '(' ReadVars ')' ';'              {$$ = $3;};
Stmt		      	:	     Id '=' Expr ';'							        	{$$ = doAssign($1, $3);} ;
Stmt	       		:	     IF '(' BExpr ')' '{' StmtSeq '}' 			{$$ = doIf($3, $6);};
Stmt	       		:	     IF '(' BExpr ')' '{' StmtSeq '}' ELSE '{' StmtSeq '}' 	 {$$ = doIfElse($3, $6, $10);};
Stmt            :      While '(' BExpr ')' '{' StmtSeq '}'    {$$ = doWhileLoop($3, $6);};
ReadVars        :      RVar ',' ReadVars                      {$$ = AppendSeq($1, $3); };
ReadVars        :      RVar                                   {$$ = $1;};
RVar            :      Id                                     {$$ = doReadIn($1);};
ExprList        :      ExprVal ',' ExprList                   {$$ = AppendSeq($1, $3);} ;
ExprList        :      ExprVal                                {$$ = $1;};
ExprVal         :      Expr                                   {$$ = doPrint($1);};
Expr	       		:	     Expr '+' Term								          {$$ = doAdd($1, $3); } ;
Expr	       		:	     Expr '-' Term								          {$$ = doSub($1, $3); } ;
Expr	       		:	     Term									                  {$$ = $1; } ;
Term	         	:	     Term '*' Factor							        	{$$ = doMult($1, $3); } ;
Term	         	:	     Term '/' Factor							        	{$$ = doDiv($1, $3); } ;
Term	         	:	     Term '%' Factor							        	{$$ = doMod($1, $3); } ;
Term            :      POW '(' Term ',' Expr ')'              {$$ = doExponentiation($3, $5);};
Term	         	:      Factor								                	{$$ = $1; } ;
BExpr           :      Term '>' Factor                        {$$ = doRelationalOp($1, $3, ">"); } ;
BExpr           :      Term '<' Factor                        {$$ = doRelationalOp($1, $3, "<"); } ;
BExpr           :      Expr EQ Expr                           {$$ = doRelationalOp($1, $3, "=="); } ;
BExpr           :      Expr NOTEQ Expr                        {$$ = doRelationalOp($1, $3, "!="); } ;
BExpr           :      Expr GTE Expr                          {$$ = doRelationalOp($1, $3, ">="); } ;
BExpr           :      Expr LTE Expr                          {$$ = doRelationalOp($1, $3, "<="); } ;
Term            :      Term AND Factor                        {$$ = doBoolOp($1, $3, "&&"); } ;
Term            :      Term OR Factor                         {$$ = doBoolOp($1, $3, "||"); } ;
Term            :      '!'Term                                {$$ = doBoolNot($2); } ;
Factor          :      '(' Expr ')'                           {$$ = $2;}
Factor          :      '-'Factor                              {$$ = doNegative($2); };
Factor	       	:	     IntLit									                {$$ = doIntLit(yytext); };
Factor	       	:	     Ident									                {$$ = doRval(yytext); };
Id		          : 	   Ident									                {$$ = strdup(yytext);};

%%

int yyerror(char *s)  {
  writeIndicator(getCurrentColumnNum());
  writeMessage("Illegal Character in YACC");
  return 1;
}
