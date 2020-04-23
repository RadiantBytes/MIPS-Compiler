%{
	//#include "yaccExample.h"
#include "IOMngr.h"
#include "y.tab.h"

#define YY_INPUT(buf,result,max_size) \
	{ int c = getNextSourceChar(); \
		result = (c == EOF) ? YY_NULL : (buf[0] = c, 1); \
	}

%}


letter [A-Za-z]
digit [0-9]

%%
if															{return IF;}
else														{return ELSE;}
while														{return While;}
int															{return Int;}
read														{return Read;}
print														{return Write;}
printline												{return WriteLn;}
printspaces											{return PrintSpaces;}
pow															{return POW;}
{letter}({letter}|{digit})*			{return Ident;}
{digit}{digit}*									{return IntLit;}
\=															{return '=';}
\+															{return '+';}
\*															{return '*';}
\-															{return '-';}
\/															{return '/';}
\%															{return '%';}
\^															{return '^';}
\>															{return '>';}
\<															{return '<';}
\,															{return ',';}
\=\=														{return EQ;}
\!\=														{return NOTEQ;}
\>\=														{return GTE;}
\<\=														{return LTE;}
\!															{return '!';}
\&\&														{return AND;}
\|\|														{return OR;}
\;															{return ';';}
\{															{return '{';}
\}															{return '}';}
\(															{return '(';}
\)															{return ')';}

[ ]															{}
\t															{}
\r															{}
\n															{}
[\00]           								{}

.					{writeIndicator(getCurrentColumnNum());
						writeError("Illegal Character in lex\n");}

%%

	int yywrap () {
		return 1;
	}
