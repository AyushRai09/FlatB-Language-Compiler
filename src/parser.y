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
char* num2str(int a)
{
  string ans;
  char * out;
  int i;
  while(a!=0)
  {
    ans+=(char)a%10;
    a=a/10;
  }

  int len=ans.length();
  out=(char*)malloc(sizeof(char)*len);
  for(i=len-1;i>=0;i--)
  {
    out[len-i]=ans[i];
  }
  return out;
}
%}

%start program
%token declaration_list
%token <value> ADD
%token <value> SUB
%token <value> MUL
%token <value> DIV
%token DECLBLOCK
%token statement_list
%token <number> NUMBER
%token <value> IDENTIFIER
%token ETOK
%token <value> INT
%token <value> ARRAY
%token PRINT
%token <value> STRING
%token CODEBLOCK
%token <value> READ
%token <value>IF
%token <value>ELSE
%token <value>COMPARE
%token <value> EE
%token <value>GT
%token <value>LT
%token <value>GTE
%token <value>LTE
%token <value>AND
%token <value>OR
%token <value>WHILE
%token <value>FOR
%token <value>LABEL
%token GOTO
%left ADD
%left MUL
%left DIV
%left SUB

%type <prog> program
%type <fields> decl_blocks
%type <field> decl_block
%type <vars> variables
%type <var> variable
%type <codes> code_blocks
%type <code> code_block
%type <exprnewsts> exprnew
%type <arthm> arithmetic
%type <exp> expr
%type <conds> conds
%type <cond> cond
%type <value> compare
%type <value> andor
%type <fors> forloop
%type <number> inc
%type <called> call
%type <pr> thingps
%type <rd> thingrs
%type <value> thingr
%type <value> thingp
%%

program: DECLBLOCK '{' decl_blocks '}' CODEBLOCK '{' code_blocks '}' {
  $$=new Prog($3,$7);
  start=$$;
};

decl_blocks:{ $$ = new fieldDecls();}
          | decl_blocks decl_block ';' { $$->push_back($2); };

decl_block: INT variables {$$=new fieldDecl($2);};

variables: variable { $$=new Vars();$$->push_back($1);}
          | variables ',' variable {
            $$->push_back($3);
          };

variable:  IDENTIFIER {$$ = new Var(string("Identifier"),string($1));}
          | ARRAY  {$$=new Var(string("Array"),string($1));};

code_blocks: { $$=new fieldCodes(); }
          | code_blocks code_block {$$->push_back($2);};

code_block:PRINT thingps ';' {$$=$2;}
		| READ thingrs ';'       {$$=$2;}
		| expr ';' {$$=$1;}
		| IF conds '{' code_blocks '}'  {$$=new ifelsest($2,$4,NULL);}
		| IF conds '{' code_blocks '}' ELSE '{' code_blocks '}'  {$$=new ifelsest($2,$4,$8);}
		| WHILE conds '{' code_blocks '}' {$$=new whilest($2,$4);}
		| FOR forloop '{' code_blocks '}'  {$$=new forst($2,$4);}
    | LABEL code_blocks GOTO IDENTIFIER call ';' {$$=new gotost($2,$4,$5);}


thingps:thingp {$$=new thingpsst();$$->push_back(string($1));}
      | thingps ',' thingp {$$->push_back(string($3));};
thingp:  IDENTIFIER {$$=$1;}
		| NUMBER        {$$=num2str($1);}
		| ARRAY         {$$=$1;}
		| STRING        {$$=$1;};


thingrs:thingr  {$$=new thingrsst();$$->push_back(string($1));}
      | thingrs ',' thingr {$$->push_back($3);};
thingr: IDENTIFIER {$$=$1;}
		| NUMBER       {$$=num2str($1);}
		| ARRAY        {$$=$1;};

expr: IDENTIFIER '=' exprnew {$$=new expr($1,$3);}
		| ARRAY '=' exprnew {$$=new expr($1,$3);} ;

exprnew: arithmetic  {$$=new exprnewst($1); }
		| IDENTIFIER {$$=new exprnewst($1);}
		| NUMBER    {$$=new exprnewst($1);}
		| ARRAY     {$$=new exprnewst($1);};

arithmetic: exprnew ADD exprnew {$$=new arithmeticst($1,string($2),$3);}
		| exprnew SUB exprnew   {$$=new arithmeticst($1,string($2),$3);}
		| exprnew DIV exprnew   {$$=new arithmeticst($1,string($2),$3);}
		| exprnew MUL exprnew    {$$=new arithmeticst($1,string($2),$3);};

conds: {$$=new condsst();}  | conds andor cond {$$->push_back($3,$2);};
cond: exprnew compare exprnew {$$=new condst($1,$2,$3);};
compare: GT {$$=$1;}
	| LT  {$$=$1;}
	| GTE  {$$=$1;}
	| LTE  {$$=$1;}
	| EE  {$$=$1;};


andor: {$$="";}
    | AND {$$=$1;}
	  | OR {$$=$1;};

forloop: IDENTIFIER '=' NUMBER ','  NUMBER inc {$$=new forloopinit($1,$3,$5,$6);};
inc:      {$$=1;}
    |','  NUMBER {$$=$2;};

call: {$$=NULL;}
    | IF conds  {$$=new callst($2);}
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
  {
		printf("Successfully parsed without any errors\n");
    start->traverse();
    start->codegen();
    start->codeDump();
  }
}
