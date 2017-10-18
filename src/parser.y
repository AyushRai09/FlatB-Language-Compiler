%{
#include <bits/stdc++.h>
#include "ClassDefs.h"
int flag=0;
extern "C" int yylex();
extern "C" int yyparse();
extern "C" FILE *yyin;
extern "C" int line_num;
extern union Node yylval;
extern "C" int errors;
void yyerror(const char *s);
class Prog* start = NULL;
int errors=0;
%}

%start program
%token declaration_list
%token DECLBLOCK
%token statement_list
%token NUMBER
%token <value> IDENTIFIER
%token ETOK
%left '+'
%left '*'
%left '/'
%left '-'
%token INT
%token <value> ARRAY
%token PRINT
%token STRING
%token CODEBLOCK
%token READ
%token IF
%token ELSE
%token COMPARE
%token EE
%token GT
%token LT
%token GTE
%token LTE
%token AND
%token OR
%token WHILE
%token FOR
%token LABEL
%token GOTO

%type <prog> program
%type <fields> decl_blocks
%type <field> decl_block
%type <vars> variables
%type <var> variable
%type <codes> code_blocks
%type <code> code_block
%%

program: DECLBLOCK '{' decl_blocks '}' CODEBLOCK '{' code_blocks '}' {
  $$=new Prog($3,$7);
  start=$$;
};

decl_blocks: { $$ = new fieldDecls(); }
           | decl_blocks decl_block ';' { $$->push_back($2); };
decl_block:  | decl_block INT variables {$$=new fieldDecl($3);};

variables: variable { $$=new Vars();$$->push_back($1);}
           | variables ',' variable {$$->push_back($3);};

variable:  IDENTIFIER {$$=new Var(string("Identifier"),$1); cout << $$<<"\n";}
           | ARRAY  {$$=new Var(string("Array"),$1);  cout << $$<<"\n";};

code_blocks: { $$=new fieldCodes(); }
            | code_blocks code_block ';' {$$->push_back($2);};

code_block:print
		| read
		| expr
		| IF cond '{' code_block '}'
		| IF cond '{' code_block '}' ELSE '{' code_block '}'
		| WHILE cond '{' code_block '}'
		| FOR forloop '{' code_block '}'
		| LABEL
		| GOTO IDENTIFIER call


print: PRINT things
things:  IDENTIFIER
		| NUMBER
		| ARRAY
		| IDENTIFIER ',' things
		| NUMBER ',' things
		| ARRAY ',' things
		| STRING ',' things


read: READ thingsr
thingsr: IDENTIFIER
		| NUMBER
		| ARRAY
		| IDENTIFIER ',' thingsr
		| NUMBER ',' thingsr
		| ARRAY ',' thingsr

expr: IDENTIFIER '=' exprnew
		| ARRAY '=' exprnew

exprnew: arithmetic
		| IDENTIFIER
		| NUMBER
		| ARRAY

arithmetic: exprnew '+' exprnew
		| exprnew '-' exprnew
		| exprnew '/' exprnew
		| exprnew '*' exprnew

cond: exprnew compare exprnew andor cond |
compare: GT
	| LT
	| GTE
	| LTE
	| EE
andor: AND
	| OR
	|

forloop: IDENTIFIER '=' NUMBER ','  NUMBER inc
inc: ','  NUMBER
	|

call: IF cond |

%%


int main(int argc, char *argv[])
{
	if (argc == 1 ) {
		fprintf(stderr, "Correct usage: bcc filename\n");
		exit(1);
	}

	if (argc > 2) {
		fprintf(stderr, "Passing more arguments than necessary.\n");
		fprintf(stderr, "Correct usage: bcc filename\n");
	}

	yyin = fopen(argv[1], "r");

	yyparse();
	if(!flag)
		printf("Successfully parsed without any errors\n");
}
