%{
  #include <stdio.h>
  #include <stdlib.h>
  FILE *yyin;
  int yylex (void);
  void yyerror (char const *s);
  int flag=0;
%}

%token declaration_list
%token DECLBLOCK
%token statement_list
%token NUMBER
%token IDENTIFIER
%token ETOK
%left '+'
%left '*'
%left '/'
%left '-'
%token INT
%token ARRAY
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
%%

program: DECLBLOCK '{' decl_block '}' CODEBLOCK '{' code_block '}' 

decl_block:  INT variables decl_block |

variables:  IDENTIFIER ',' variables
            | IDENTIFIER ';'
	    |     ARRAY ',' variables
	    |     ARRAY ';'


code_block:print ';' code_block 
		| read ';' code_block 
		| expr ';' code_block 
		| IF cond '{' code_block '}' code_block 
		| IF cond '{' code_block '}' ELSE '{' code_block '}' code_block 
		| WHILE cond '{' code_block '}' code_block 
		| FOR forloop '{' code_block '}' code_block 
		| LABEL code_block 
		| GOTO IDENTIFIER call ';' code_block 
		| 


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
