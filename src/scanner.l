%{
#include <bits/stdc++.h>
#include "ClassDefs.h"
#include "parser.tab.h"
#include "file.h"
#define YY_DECL extern "C" int yylex()
extern union Node yylval;
using namespace std;
	void yyerror (char const *s)
	{
		fprintf(stderr, "Error | Statement number: %d\n%s\n",yylineno,s);
		flag=1;
	}
%}

%%
"codeblock" 			{yylval.value = strdup(yytext);return CODEBLOCK;}
"declblock"                 	{yylval.value = strdup(yytext);return DECLBLOCK;}
"declaration_list"		return declaration_list;
"for" 				{yylval.value = strdup(yytext);return FOR;}
"while" 			{yylval.value = strdup(yytext);return WHILE;}
"and" 				{yylval.value = strdup(yytext);return AND;}
"or" 					{yylval.value = strdup(yytext);return OR;}
"int"           {
							yylval.value = strdup(yytext);
							printf("Token type: datatype->int, Lexeme/Token Value: %s\n", yytext);
							return INT;
								}
 [a-zA-Z][a-zA-Z0-9]":"    {
 					yylval.value = strdup(yytext);
					printf("Label detected with the name: %s\n", yytext);
					return LABEL;
 													}

"if" 		{yylval.value = strdup(yytext);return IF;}
"else" 	{yylval.value = strdup(yytext);return ELSE;}
"print" {yylval.value = strdup(yytext);return PRINT;}
"read" 	{yylval.value = strdup(yytext);return READ;}
[0-9][0-9]*	{
				yylval.number = stoi(yytext)	;
				printf("Token type: Number, Lexeme/Token Value: %s\n", yytext);
				return NUMBER;
            }
"goto" 		{
				yylval.value = strdup(yytext);
			printf("goto detected\n");
			return GOTO;
		}
 [a-zA-Z][a-zA-Z0-9]*"["[0-9a-z][0-9a-z]*"]" {
 			yylval.value = strdup(yytext);
			printf("Token type:integer array, Lexeme/Token Value: %s\n", yytext);
			return ARRAY;
 			}
 \"(\\.|[^\\"])*\"      {
 				yylval.value = strdup(yytext);
				printf("Token type:string, Lexeme/Token Value: %s\n",yytext);
				return STRING;
			}

[a-zA-Z][a-zA-Z0-9]* {
			yylval.value = strdup(yytext);
			printf("Token type: Identifier, Lexeme/Token Value: %s\n", yytext);
			return IDENTIFIER;
		     }

">=" 		{yylval.value = strdup(yytext);return GTE;}
"<=" 		{yylval.value = strdup(yytext);return LTE;}
"==" 		{yylval.value = strdup(yytext);return EE;}
">" 		{yylval.value = strdup(yytext);return GT;}
"<" 		{yylval.value = strdup(yytext);return LT;}
"+"		{yylval.value = strdup(yytext);return ADD;}
"-"		{yylval.value = strdup(yytext);return SUB;}
"*"		{yylval.value = strdup(yytext);return MUL;}
"/"   {yylval.value = strdup(yytext);return DIV;}
"{"		return '{';
"}"		return '}';
"," 		return ',';

";" 		{
		yylineno++;
		return ';';
		}
"=" 		return '=';

[ \t]		{ /* Do nothing */ }
.		{
		  printf("Unexpected token encountered: %s\n", yytext);
		  return ETOK;
		}
%%
