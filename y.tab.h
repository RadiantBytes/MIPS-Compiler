/* A Bison parser, made by GNU Bison 3.0.4.  */

/* Bison interface for Yacc-like parsers in C

   Copyright (C) 1984, 1989-1990, 2000-2015 Free Software Foundation, Inc.

   This program is free software: you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program.  If not, see <http://www.gnu.org/licenses/>.  */

/* As a special exception, you may create a larger work that contains
   part or all of the Bison parser skeleton and distribute that work
   under terms of your choice, so long as that work isn't itself a
   parser generator using the skeleton or a modified version thereof
   as a parser skeleton.  Alternatively, if you modify or redistribute
   the parser skeleton itself, you may (at your option) remove this
   special exception, which will cause the skeleton and the resulting
   Bison output files to be licensed under the GNU General Public
   License without this special exception.

   This special exception was added by the Free Software Foundation in
   version 2.2 of Bison.  */

#ifndef YY_YY_Y_TAB_H_INCLUDED
# define YY_YY_Y_TAB_H_INCLUDED
/* Debug traces.  */
#ifndef YYDEBUG
# define YYDEBUG 0
#endif
#if YYDEBUG
extern int yydebug;
#endif

/* Token type.  */
#ifndef YYTOKENTYPE
# define YYTOKENTYPE
  enum yytokentype
  {
    Ident = 258,
    IntLit = 259,
    Int = 260,
    Void = 261,
    Bool = 262,
    ArrInt = 263,
    AInt = 264,
    Write = 265,
    IF = 266,
    EQ = 267,
    NOTEQ = 268,
    GTE = 269,
    LTE = 270,
    AND = 271,
    OR = 272,
    WriteLn = 273,
    Read = 274,
    ReadVal = 275,
    PrintSpaces = 276,
    While = 277,
    For = 278,
    ELSE = 279,
    POW = 280,
    ArrAssign = 281,
    True = 282,
    False = 283
  };
#endif
/* Tokens.  */
#define Ident 258
#define IntLit 259
#define Int 260
#define Void 261
#define Bool 262
#define ArrInt 263
#define AInt 264
#define Write 265
#define IF 266
#define EQ 267
#define NOTEQ 268
#define GTE 269
#define LTE 270
#define AND 271
#define OR 272
#define WriteLn 273
#define Read 274
#define ReadVal 275
#define PrintSpaces 276
#define While 277
#define For 278
#define ELSE 279
#define POW 280
#define ArrAssign 281
#define True 282
#define False 283

/* Value type.  */
#if ! defined YYSTYPE && ! defined YYSTYPE_IS_DECLARED

union YYSTYPE
{
#line 22 "ExprEval.y" /* yacc.c:1909  */

  long val;
  char * string;
  struct ExprRes * ExprRes;
  struct InstrSeq * InstrSeq;
  struct BExprRes * BExprRes;
  struct Id *Id;

#line 119 "y.tab.h" /* yacc.c:1909  */
};

typedef union YYSTYPE YYSTYPE;
# define YYSTYPE_IS_TRIVIAL 1
# define YYSTYPE_IS_DECLARED 1
#endif


extern YYSTYPE yylval;

int yyparse (void);

#endif /* !YY_YY_Y_TAB_H_INCLUDED  */
