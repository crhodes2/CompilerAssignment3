%{

/* Turtle Compiler yacc file
   by Pete Myers and Christian Rhodes
   Due: March 7, 2019
   Assign 3
*/

#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "symtable.h"

TreeNode * root;
extern TreeNodeFactory * factory;

extern FILE * yyin;
extern FILE * yyout;
extern int yylineno;
void yyerror(const char *);

int yylex(void);

%}

%union
{
	TreeNode * node;
	BlockTreeNode * block;
	int value;
	COLOR_TYPE color;
	SymbolTable::Entry * symentry;
}

%token INT

%token HOME
%token FD
%token RT

%token IF
%token IFELSE
%token REPEAT

%token <value> NUMBER
%token <color> COLORNAME
%token <symentry> VARIABLE

%token XCOR
%token YCOR
%token COLOR
%token RANDOM
%token HEADING


%token BK
%token LT
%token PD
%token PU
%token SETC

%type <block> statements
%type <node> statement
%type <node> expression
%type <node> condition
%type <node> function

%left '+' '-'
%left '*' '/'
%nonassoc UMINUS

%%

statements:	statement statements		{
											if ($1 != NULL)
											{
												// there is a statement node
												$$ = factory->CreateBlock();
												$$->AddChild($1);
												if ($2 != NULL)
												{
													$$->AdoptChildren($2);
												}
												root = $$;
											}
											else
											{
												// there is no statement node
												$$ = $2;
											}
										}
	|									{ $$ = NULL; }
	;

statement:	HOME						{ $$ = factory->CreateTurtleCmd(CMD_HOME); }
	|	FD expression					{ $$ = factory->CreateTurtleCmd(CMD_FD, $2); }
	|	RT expression					{ $$ = factory->CreateTurtleCmd(CMD_RT, $2); }
	|   IF '(' condition ')' '[' statements ']' { $$ = factory->CreateIf($3, $6); }
	|	REPEAT expression '[' statements ']'    { $$ = factory->CreateRepeat($2, $4); }
	|	BK expression					{ $$ = factory->CreateTurtleCmd(CMD_BK, $2); }
	|	LT expression					{ $$ = factory->CreateTurtleCmd(CMD_LT, $2); }
	|	PD 								{ $$ = factory->CreateTurtleCmd(CMD_PD); }
	|	PU								{ $$ = factory->CreateTurtleCmd(CMD_PU); }
	|	SETC expression					{ $$ = factory->CreateTurtleCmd(CMD_SETC, $2); }
	|	INT VARIABLE					{ 
										  if($2->type != -1)
										  {
												yyerror("Redeclaration of variable");
												YYABORT;
										  }
										  
										  /* associate type with variable */
										  $2->type = VT_INT;
										  /* construct */
										  $$ = factory->CreateDeclaration(VT_INT, factory->CreateVariable($2)); 
										    
										}
	|	VARIABLE '=' expression			{
										  if($1->type == -1)
										  {
												yyerror("Use of undeclared variable");
												YYABORT;
										  }
										  $$ = factory->CreateAssignment(factory->CreateVariable($1), $3); 
										}
	;

expression:	expression '+' expression	{ $$ = factory->CreateOperator(OT_PLUS, $1, $3); }
	|	expression '-' expression		{ $$ = factory->CreateOperator(OT_MINUS, $1, $3); }
	|	'-' expression %prec UMINUS		{ $$ = factory->CreateOperator(OT_MINUS, factory->CreateNumber(0), $2); }
	|	'(' expression ')'				{ $$ = $2; }
	|	expression '*' expression		{ $$ = factory->CreateOperator(OT_TIMES, $1, $3); }
	|	expression '/' expression		{ $$ = factory->CreateOperator(OT_DIVIDE, $1, $3); }
	|	VARIABLE						{
	
										  if ($1->type == -1)
										  {
												yyerror("Use of undeclared variable");
												YYABORT;
										  }
										  $$ = factory->CreateVariable($1); 
										}
	|	NUMBER							{ $$ = factory->CreateNumber($1); }
	|	COLORNAME						{ $$ = factory->CreateColorName($1); }
	|	function						{ $$ = $1; }
	;

condition: expression '=' expression	{ $$ = factory->CreateOperator(OT_EQUALS, $1, $3); }
	;

function: XCOR							{ $$ = factory->CreateFunction(FT_XCOR); }
	|	YCOR							{ $$ = factory->CreateFunction(FT_YCOR); }
	|	COLOR							{ $$ = factory->CreateFunction(FT_COLOR); }
	|	RANDOM							{ $$ = factory->CreateFunction(FT_RANDOM); }
	|	HEADING							{ $$ = factory->CreateFunction(FT_HEADING); }
	;


%%
