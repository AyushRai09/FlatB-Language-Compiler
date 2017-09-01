%{
#include "parser.tab.h"
#include "file.h"
	void yyerror (char const *s) 
	{
		fprintf(stderr, "Error | Statement number: %d\n%s\n",yylineno,s);
		flag=1;
	}
%}

%%
"codeblock" 			return CODEBLOCK;
"declblock"                 	return DECLBLOCK;
"declaration_list"		return declaration_list;
"for" 				return FOR;
"while" 			return WHILE;
"and" 				return AND;
"or" 				return OR;
"int"           {
			printf("Token type: datatype->int, Lexeme/Token Value: %s\n", yytext);  
			return INT;
		}
 [a-zA-Z][a-zA-Z0-9]":"    {
				printf("Label detected with the name: %s\n", yytext);  
				return LABEL;
 			}
	 			
"if" 		return IF;
"else" 		return ELSE;
"print" 	return PRINT;
"read" 			return READ;
[0-9][0-9]*	{  
			printf("Token type: Number, Lexeme/Token Value: %s\n", yytext);  
			return NUMBER; 
                }
"goto" 		{ 
			printf("goto detected\n");
			return GOTO;
		}
 [a-zA-Z][a-zA-Z0-9]*"["[0-9a-z][0-9a-z]*"]" {
			printf("Token type:integer array, Lexeme/Token Value: %s\n", yytext);  
			return ARRAY;
 			}
 \"(\\.|[^\\"])*\"      {
				printf("Token type:string, Lexeme/Token Value: %s\n",yytext);
				return STRING;
			}

[a-zA-Z][a-zA-Z0-9]* {  
			printf("Token type: Identifier, Lexeme/Token Value: %s\n", yytext);  
			return IDENTIFIER; 
		     }

">=" 		return GTE;
"<=" 		return LTE;
"==" 		return EE;
">" 		return GT;
"<" 		return LT;
"+"		return '+';
"-"		return '-';
"*"		return '*';
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

  
	   
