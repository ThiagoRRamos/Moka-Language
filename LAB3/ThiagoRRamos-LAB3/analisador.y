%{
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define		MENOR		61
#define		MENIG		62
#define		MAIOR		63
#define		MAIG		64
#define		IGUAL		65
#define		DIFER		66
#define		VEZES		71
#define		DIV			72
#define		REST		73
#define 	MAIS 		81
#define 	MENOS 		82

int tab = 0;
%}
%union {
	char cadeia[50];
	int atr;
	int valint;
	float valreal;
	char carac;
}
%type		<cadeia>		Variavel
%type		<cadeia>		Tipo
%type		<cadeia>		Parametro
%type		<cadeia>		ListParam
%token		<cadeia>		ID
%token		<cadeia>		CADEIA
%token		<valint>		CTINT
%token		<valreal>		CTREAL
%token		<atr>			OPAD
%token		<atr>			OPMULT
%token		<atr>			OPREL
%token		<carac>			CTCARAC
%token		<carac>			INVAL
%token						ABPAR
%token						FPAR
%token						ABCHAV
%token						FCHAV
%token						ABCOL
%token						FCOL
%token						PVIRG
%token						ATRIB
%token						AND
%token						NEG
%token						NOT
%token						OR
%token						CARAC
%token						INT
%token						REAL
%token						VERDADE
%token						FALSO
%token						VIRG
%token						CHAMAR
%token						ENQUANTO
%token						ESCREVER
%token						FUNCAO
%token						LER
%token						LOGIC
%token						PARA
%token						PROCEDIMENTO
%token						PROGRAMA
%token						REPETIR
%token						RETORNAR
%token						SE
%token						SENAO
%token						VAR
%%
Prog		: 	PROGRAMA   ID   PVIRG {printf("programa %s;\n", $2);}  Decls    SubProgs   CmdComp 

Decls 		:	   |   VAR {printf("\nvar\n");tab++;}  ListDecl {tab--;printf("\n");}
				
ListDecl	:	Declaracao    |   ListDecl   Declaracao

Declaracao	: 	Tipo {tabular(); printf("%s ", $1);}   ListElemDecl   PVIRG {printf(";\n");}

Tipo		: 	INT {strcpy($$, "int");}    
			|     REAL {strcpy($$, "real");}   
			|    CARAC {strcpy($$, "carac");}    
			|     LOGIC {strcpy($$, "logic");}
ListElemDecl	: 	ElemDecl      |     ListElemDecl   VIRG {printf(", ");}   ElemDecl

ElemDecl  	:	ID {printf("%s", $1);}  |    ID   ABCOL {printf("%s[", $1);}  ListDim   FCOL  {printf("]");}  

ListDim	: 	CTINT {printf("%d", $1);}   |    ListDim   VIRG   CTINT {printf(", %d", $3);}

SubProgs	:    	   |   SubProgs   DeclSubProg

DeclSubProg	:   	Cabecalho   Decls   CmdComp {printf("\n");}

Cabecalho	:   	CabFunc   |   CabProc

CabFunc	:   	FUNCAO Tipo   ID 	ABPAR  FPAR  PVIRG {printf("funcao %s %s();\n", $2, $3);}
		|		FUNCAO Tipo   ID ABPAR  ListParam   FPAR   PVIRG {printf("funcao %s %s(%s);\n", $2, $3, $5);}

CabProc	:   	PROCEDIMENTO   ID   ABPAR   FPAR   PVIRG {printf("procedimento %s();\n", $2);}
		|	PROCEDIMENTO   ID   ABPAR   ListParam   FPAR   PVIRG {printf("procedimento %s(%s);\n", $2, $4);}

ListParam	:   	Parametro {strcpy($$, $1);}   |   ListParam   VIRG   Parametro {strcpy($$,"");strcat($$,$1);strcat($$,",");strcat($$,$3);}

Parametro	:   	Tipo   ID {strcpy($$,"");strcat($$,$1);strcat($$," ");strcat($$, $2);}

CmdComp	:     	ABCHAV  {tabular (); printf ("\{\n"); tab++;} ListCmd   FCHAV {tab--; tabular (); printf ("}\n");}

ListCmd	:   	   |   ListCmd   Comando

Comando  	:   	CmdComp   |   CmdSe   |   CmdEnquanto   |   CmdRepetir   |   CmdPara   
|   	CmdLer   |   CmdEscrever   |   CmdAtrib   |   ChamaProc   |   CmdRetornar   
|   	PVIRG

CmdSe		:   	SE   ABPAR {tabular();tab++;printf("se(");}  Expressao   FPAR {printf(")\n");}   Comando {tab--;}  CmdSenao

CmdSenao	:    	   |   SENAO {tabular();tab++;printf("senao\n");}  Comando {tab--;}

CmdEnquanto	:   	ENQUANTO   ABPAR {tabular(); printf("enquanto (");}  Expressao   FPAR {printf(")\n");tab++;}  Comando {tab--;}

CmdRepetir	:   	REPETIR {tabular(); printf("repetir \n"); tab++;}  Comando   ENQUANTO {tab--; tabular(); printf("enquanto("); }  ABPAR   Expressao   FPAR   PVIRG {printf(");\n"); }

CmdPara	:   	PARA  ABPAR Variavel ATRIB {tabular();tab++;printf("para(%s := ", $3);}  Expressao   PVIRG {printf("; ");}  Expressao   PVIRG {printf("; ");}  Variavel   ATRIB {printf("%s := ", $12);}   Expressao   FPAR {printf(")\n");}   Comando {tab--;}

CmdLer	:   	LER   ABPAR  {tabular();printf("ler(");} ListVar   FPAR   PVIRG {printf(");\n");}

ListVar	:   	Variavel {printf("%s", $1);}  |   ListVar   VIRG {printf(", ");}   Variavel {printf("%s", $4);}

CmdEscrever	:   	ESCREVER   ABPAR {tabular();printf("escrever(");}  ListEscr   FPAR   PVIRG {printf(");\n");}

ListEscr	:   	ElemEscr   |   ListEscr   VIRG {printf(", ");}   ElemEscr

ElemEscr	:   	CADEIA {printf("%s", $1);}  |   Expressao
ChamaProc	:   	CHAMAR   ID   ABPAR   FPAR   PVIRG {tabular();printf("chamar %s();\n", $2);}
		|	CHAMAR   ID   ABPAR {tabular();printf("chamar %s(", $2);}  ListExpr   FPAR   PVIRG {printf(");\n");}
CmdRetornar	:   	RETORNAR   PVIRG {tabular (); printf("retornar;\n");} 
			|   RETORNAR {tabular(); printf("retornar ");}  Expressao   PVIRG {printf(";\n");}
CmdAtrib  	:   	Variavel {tabular (); printf("%s := ", $1);}   ATRIB  Expressao   PVIRG {printf(";\n");}
ListExpr	:   	Expressao  |   ListExpr   VIRG {printf(", ");}  Expressao
Expressao  	:   	ExprAux1     |     Expressao   OR {printf(" || ");}   ExprAux1
ExprAux1  	:   	ExprAux2     |     ExprAux1   AND {printf(" && ");}   ExprAux2
ExprAux2  	:   	ExprAux3     |     NOT {printf("!");}   ExprAux3  
ExprAux3  	:   	ExprAux4     |     ExprAux4   OPREL {switch($2){
									case MAIOR: printf(" > ");break;
									case MENOR: printf(" < ");break;
									case MAIG: printf(" >= ");break;
									case MENIG: printf(" <= ");break;
									case IGUAL: printf(" = ");break;
									case DIFER: printf(" != ");break;
								}
								}  ExprAux4
ExprAux4  	:   	Termo     
			|     ExprAux4   OPAD {switch($2){
									case MAIS: printf(" + ");break;
									case MENOS: printf(" - ");break;
								}
								}   Termo
Termo  	:   	Fator   
			|   Termo   OPMULT {switch($2){
									case VEZES: printf(" * ");break;
									case DIV: printf(" / ");break;
									case REST: printf(" %% ");break;
								}
								}   Fator
Fator		:   	Variavel {printf("%s",$1);}  
			|   CTINT {printf("%d", $1);}
			|   CTREAL {printf("%f", $1);}  
			|   CTCARAC   {printf("%c", $1);}
			|   VERDADE   {printf("verdade");}
			|   FALSO   {printf("falso");}
			|   NEG {printf("~");}   Fator
			|   ABPAR {printf("(");}  Expressao   FPAR  {printf(")");}
			|   ChamaFunc
Variavel	:   	ID {strcpy($$,$1);}  |   ID   ABCOL   ListSubscr   FCOL   
ListSubscr	:   	ExprAux4   |   ListSubscr   VIRG   ExprAux4
ChamaFunc	:   	ID   ABPAR    FPAR  {printf("%s()", $1);}
		|	ID   ABPAR  {printf("%s(", $1);} ListExpr   FPAR {printf(")");}
%%
#include "lex.yy.c"
tabular () {
	int i;
	for (i = 1; i <= tab; i++)
   	printf ("\t");
}