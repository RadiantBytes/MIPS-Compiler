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
for															{return For;}
int															{return Int;}
void 														{return Void;}
Bool 														{return Bool;}
Int 														{return ArrInt;}
true     												{return True;}
false 													{return False;}
array														{return ArrAssign;}
aint														{return AInt;}
read														{return Read;}
print														{return Write;}
printline												{return WriteLn;}
printspaces											{return PrintSpaces;}
pow															{return POW;}
{letter}({letter}|{digit})*			{return Ident;}
{digit}{digit}*									{return IntLit;}
\=															{return '=';}
\+															{return '+';}
\+\+														{return Incr;}
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
\|															{return '|';}
\;															{return ';';}
\{															{return '{';}
\}															{return '}';}
\(															{return '(';}
\)															{return ')';}
\[															{return '[';}
\]															{return ']';}
\_															{return '_';}
\~															{return '~';}

\#															{
																 char * temp;
																 temp = (char*)malloc(strlen(getBuffer()) * sizeof(char));
																 strcpy(temp, getBuffer());
															   if (temp[0] == '#') {
																	setBuffer(getCurrentLineNum()+1);
																 }
																}

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
