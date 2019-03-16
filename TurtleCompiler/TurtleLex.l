
%{

/* Turtle Compiler yacc file
   by Pete Myers and Christian Rhodes
   Due: March 7, 2019
   Assign 3
*/

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "TurtleYacc.h"
#include "symtable.h"

%}

%option noyywrap

%%


"-"?[0-9]+	{ yylval.value = atoi(yytext); return NUMBER; }

black		{ yylval.color = BLACK; return COLORNAME; }
blue		{ yylval.color = BLUE; return COLORNAME; }
white		{ yylval.color = WHITE; return COLORNAME; }
orange		{ yylval.color = ORANGE; return COLORNAME; }
red			{ yylval.color = RED; return COLORNAME; }
yellow		{ yylval.color = YELLOW; return COLORNAME; }
green		{ yylval.color = GREEN; return COLORNAME; }
sky			{ yylval.color = SKY; return COLORNAME; }
magenta		{ yylval.color = MAGENTA; return COLORNAME; }
lime		{ yylval.color = LIME; return COLORNAME; }
cyan		{ yylval.color = CYAN; return COLORNAME; }
brown		{ yylval.color = BROWN; return COLORNAME; }
turquoise	{ yylval.color = TURQUOISE; return COLORNAME; }
violet		{ yylval.color = VIOLET; return COLORNAME; }
pink		{ yylval.color = PINK; return COLORNAME; }
gold		{ yylval.color = GOLD; return COLORNAME; }
silver		{ yylval.color = SILVER; return COLORNAME; }
bronze		{ yylval.color = BRONZE; return COLORNAME; }


"+"			{ return yytext[0]; }
"-"			{ return yytext[0]; }
"*"			{ return yytext[0]; }
"/"			{ return yytext[0]; }
"="			{ return yytext[0];	}

"("			{ return yytext[0]; }
")"			{ return yytext[0]; }
"["			{ return yytext[0]; }
"]"			{ return yytext[0]; }
 
home		{ return HOME; }
fd			{ return FD; }
rt			{ return RT; }

xcor		{ return XCOR; }
ycor		{ return YCOR; }
color		{ return COLOR; }
random		{ return RANDOM; }
heading		{ return HEADING; }

if			{ return IF; }
repeat		{ return REPEAT; }
int			{ return INT; }

bk			{ return BK; }
lt			{ return LT; }
pd			{ return PD; }
pu			{ return PU; }
setc		{ return SETC; }

[a-zA-Z][a-zA-Z0-9]*	{ 
							yylval.symentry = SymbolTable::GetInstance()->Insert(yytext, VARIABLE); 
							return VARIABLE; 
						}

[ \t]		{ /* ignore whitespace */}
\n			{ yylineno++; }
.			{ }

%%

void yyerror(const char * msg)
{
	fprintf(stderr, "ERROR! Line #%d: %s at %s\n", yylineno, msg, yytext);
}

