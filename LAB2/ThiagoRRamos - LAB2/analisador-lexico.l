%{
#include 	<string.h>

#define		ID			10
#define		CADEIA 		20
#define		CTCARAC		30
#define		CTINT		40
#define		CTREAL 		50
#define		OPREL		60
#define		MENOR		61
#define		MENIG		62
#define		MAIOR		63
#define		MAIG		64
#define		IGUAL		65
#define		DIFER		66
#define		OPMULT		70
#define		VEZES		71
#define		DIV			72
#define		REST		73
#define		OPAD		80
#define 	MAIS 		81
#define 	MENOS 		82
#define 	ATRIB 		90
#define     ABPAR		91
#define     FPAR		92
#define     ABCOL		93
#define     FCOL		94
#define     ABCHAV		95
#define     FCHAV		96
#define     PVIRG		97
#define     VIRG		98
#define  	INVAL		999

#define		RESERVADO		100
#define		CARAC 			101
#define		INT 			102
#define		REAL 			103
#define		VERDADE			104
#define		CHAMAR 			105
#define		LER 			106
#define		REPETIR			107
#define		ENQUANTO		108
#define		LOGIC 			109
#define		RETORNAR		110
#define		ESCREVER		111
#define		PARA 			112
#define		SE 				113
#define		FALSO 			114
#define		PROCEDIMENTO 	115
#define		SENAO 			116
#define		FUNCAO 			117
#define		PROGRAMA 		118
#define		VAR 			119
union {
	char string[50];
	int atr, valor;
	float valorreal;
	char carac;
} yylval;
%}
delim			[ \t\n\r]
ws				{delim}+

ABPAR			\(
FPAR			\)
ABCOL			\[
FCOL			\]
ABCHAV			\{
FCHAV			\}
PVIRG			;
VIRG			,
ATRIB 			:=

OR 				\|\|
AND				&&
NOT				\!
MAIS			\+
MENOS			\-
VEZES			\*
DIV				\/
REST			\%
MAIOR			\>
MENOR			\<
MAIG			>=
MENIG			<=
IGUAL 			=
DIFER 			!=
OPREL 			{MAIOR}|{MAIG}|{MENOR}|{MENIG}|{IGUAL}|{DIFER}
OPAD 			{MAIS}|{MENOS}
OPMULT  		{VEZES}|{DIV}|{REST}
NEG				~

letra			[a-zA-Z]
digito			[0-9]
carac1			\\.|[^\\']
carac2			\\.|[^\\\"]
COMENTARIO		\/\*(.|{delim})*?\*\/
ID				{letra}({letra}|{digito})*
CTINT			{digito}+
CTREAL			{digito}+\.{digito}*([Ee][+-]?{digito}+)?
CTCARAC			'{carac1}'
CADEIA			\"{carac2}*\"

Prog			{programa}{ws}{ID}{ws}{PVIRG}{ws}{Decls}{ws}{SubProgs}{ws}{CmdComp}
Decls			(var {ListDecl})?
ListDecl		{Declaracao}|({ListDecl} {Declaracao})
Declaracao		{Tipo} {ListElemDecl} {PVIRG}
Tipo 			(int)|(real)|(carac)|(logic)
ListElemDecl	{ElemDecl}|({ListElemDecl} {VIRG} {ElemDecl})
ElemDecl 		{ID} | {ID} {ABCOL} {ListDim} {FCOL}
ListDim			{CTINT}|({ListDim} {VIRG} {CTINT})
SubProgs		({SubProgrs} {DeclSubProg})?
DeclSubProg		{Cabecalho} {Decls} {CmdComp}
Cabecalho		{CabFunc}|{CabProc}
CabFunc			(funcao {Tipo} {ID} {ABPAR}{FPAR}{PVIRG})|((funcao {Tipo} {ID} {ABPAR} {ListParam} {FPAR} {PVIRG}))
CabProc			(procedimento ?{Tipo} {ID} {ABPAR}{FPAR}{PVIRG})|((procedimento {Tipo} {ID} {ABPAR} {ListParam} {FPAR} {PVIRG}))
ListParam 		{Parametro}|({ListParam} {VIRG} {Parametro})
Parametro		{Tipo} {ID}
CmdComp			{ABCHAV} {ListCmd} {FCHAV}
ListCmd			({ListCmd} {Comando})?
Comando			{CmdComp}|{CmdSe}|{CmdEnquanto}|{CmdRepetir}|{CmdPara}|{CmdLer}|{CmdEscrever}|{CmdAtrib}|{ChamaProc}|{CmdRetornar}|{PVIRG}
CmdSe 			se {ABPAR} {Expressao} {FPAR} {Comando} {CmdSenao}
CmdSenao		(senao {Comando})?
CmdEnquanto		enquanto {ABPAR} {Expressao} {FPAR} {Comando}
CmdRepetir		repetir {Comando} enquanto {ABPAR} {Expressao} {FPAR} {PVIRG}
CmdPara			para {ABPAR} Variavel {ATRIB} {Expressao} {PVIRG} {Expressao} {PVIRG} {Variavel} {ATRIB} {Expressao} {FPAR} {Comando}
CmdLer			ler {ABPAR} {ListVar} {FPAR} {PVIRG}
ListVar			{Variavel}|({ListVar} {VIRG} {Variavel})
CmdEscrever		escrever {ABPAR} {ListEscr} {FPAR} {PVIRG}
ListEscr		{ElemEscr}|({ListEscr} {VIRG} {ElemEscr})
ElemEscr		{CADEIA}|{Expressao}
ChamaProc		(chamar {ID} {ABPAR} {FPAR} {PVIRG})|(chamar {ID} {ABPAR} {ListExpr} {FPAR} {PVIRG})
CmdRetornar		(retornar {PVIRG})|(retornar {Expressao} {PVIRG})
CmdAtrib		{Variavel} {ATRIB} {Expressao} {PVIRG}
ListExpr		{Expressao}|({ListExpr} {VIRG} {Expressao})
Expressao		{ExprAux1}|({Expressao} {OR} {ExprAux1})
ExprAux1		{ExprAux2}|{ExprAux1} {AND} ExprAux2
ExprAux2		{ExprAux2}|({NOT} {ExprAux3})
ExprAux3		{ExprAux4}|({ExprAux4} {OPREL} {ExprAux4})
ExprAux4		{Termo}|({ExprAux4} {Termo})
Termo			{Fator}|({Termo} {OPMULT} {Fator})
Fator			{Variavel}|{CTINT}|{CTREAL}|{CTCARAC}|(verdade)|(falso)|(NEG Fator)|(ABPAR Expressao FPAR)|{ChamaFunc}
Variavel		{ID}|({ID} {ABCOL} {ListSubscr} {FCOL})
ListSubscr		{ExprAux4}|(ListSubscr {VIRG} {ExprAux4})
ChamaFunc		({ID}{ABPAR}{FPAR})|({ID}{ABPAR}{ListExpr}{FPAR})

%%
{ws}		{ ;}

carac 		{return CARAC;}
int			{return INT;}
real		{return REAL;}
verdade		{return VERDADE;}
chamar 		{return CHAMAR;}
ler 		{return LER;}
repetir 	{return REPETIR;}
enquanto 	{return ENQUANTO;}
logic		{return LOGIC;}
retornar 	{return RETORNAR;}
escrever 	{return ESCREVER;}
para 		{return PARA;}
se 			{return SE;}
falso		{return FALSO;}
procedimento {return PROCEDIMENTO;}
senao 		{return SENAO;}
funcao 		{return FUNCAO;}
programa	{return PROGRAMA;}
var 		{return VAR;}


{ID}		{strcpy (yylval.string, yytext); return ID;}
{CTINT}		{yylval.valor = atoi(yytext); return CTINT;}
{CTREAL}	{yylval.valorreal = atof(yytext); return CTREAL;}
{CADEIA}	{strcpy (yylval.string, yytext); return CADEIA;}
{CTCARAC}	{yylval.carac = yytext; return CTCARAC;}
{MAIOR}		{yylval.atr = MAIOR; return OPREL;}
{MENOR}		{yylval.atr = MENOR; return OPREL;}
{MAIG}		{yylval.atr = MAIG; return OPREL;}
{MENIG}		{yylval.atr = MENIG; return OPREL;}
{IGUAL}		{yylval.atr = IGUAL; return OPREL;}
{DIFER}		{yylval.atr = DIFER; return OPREL;}
{MAIS}		{yylval.atr = MAIS; return OPAD;}
{MENOS}		{yylval.atr = MENOS; return OPAD;}
{VEZES}		{yylval.atr = VEZES; return OPMULT;}
{DIV}		{yylval.atr = DIV; return OPMULT;}
{REST}		{yylval.atr = REST; return OPMULT;}
{ATRIB}		{return ATRIB;}
{ABPAR}		{return ABPAR;}
{FPAR}		{return FPAR;}
{ABCOL}		{return ABCOL;}
{FCOL}		{return FCOL;}
{ABCHAV}	{return ABCHAV;}
{FCHAV}		{return FCHAV;}
{PVIRG}		{return PVIRG;}
{VIRG}		{return VIRG;}
{COMENTARIO} {;}
.			{yylval.carac = yytext[0]; return INVAL;}
%%
main () {
	int i;
	printf ("\n   texto    |   tipo   | atributo \n");
	printf ("----------------------------------\n");
	while (i = yylex ()) {
		printf ("%12s|", yytext, i);
		switch (i) {
			case ID:
				printf ("%10s|%10s","ID", yylval.string); break;
			case ATRIB:
				printf ("%10s|%10s","ATRIB", ""); break;
			case CTINT:
				printf ("%10s|%10d","CTINT", yylval.valor); break;
			case CTREAL:
				printf ("%10s|%6f","CTREAL", yylval.valorreal); break;
			case OPREL:
				printf ("%10s|%10d","OPREL", yylval.atr); break;
			case OPAD:
				printf ("%10s|%10d","OPAD", yylval.atr); break; 
			case OPMULT:
				printf ("%10s|%10d","OPMULT", yylval.atr); break;
			case CTCARAC:
				printf ("%10s|%10c","CTCARAC", yylval.carac); break;
			case CADEIA:
				printf ("%10s|%10c","CADEIA", yylval.carac); break;
			case INVAL:
				printf ("%10s|%10c","INVAL", yylval.carac); break;
			case CARAC:
				printf("%10s|%10s","CARAC","");break;
			case INT:
				printf("%10s|%10s","INT","");break;
			case REAL:
				printf("%10s|%10s","REAL","");break;
			case VERDADE:
				printf("%10s|%10s","VERDADE","");break;
			case CHAMAR:
				printf("%10s|%10s","CHAMAR","");break;
			case LER:
				printf("%10s|%10s","LER","");break;
			case REPETIR:
				printf("%10s|%10s","REPETIR","");break;
			case ENQUANTO:
				printf("%10s|%10s","ENQUANTO","");break;
			case LOGIC:
				printf("%10s|%10s","LOGIC","");break;
			case RETORNAR:
				printf("%10s|%10s","RETORNAR","");break;
			case ESCREVER:
				printf("%10s|%10s","ESCREVER","");break;
			case PARA:
				printf("%10s|%10s","PARA","");break;
			case SE:
				printf("%10s|%10s","SE","");break;
			case FALSO:
				printf("%10s|%10s","FALSO","");break;
			case PROCEDIMENTO:
				printf("%10s|%10s","PROCED","");break;
			case SENAO:
				printf("%10s|%10s","SENAO","");break;
			case FUNCAO:
				printf("%10s|%10s","FUNCAO","");break;
			case PROGRAMA:
				printf("%10s|%10s","PROGRAMA","");break;
			case VAR:
				printf("%10s|%10s","VAR","");break;
			case ABPAR:
				printf("%10s|%10s","ABPAR","");break;
			case FPAR:
				printf("%10s|%10s","FPAR","");break;
			case ABCOL:
				printf("%10s|%10s","ABCOL","");break;
			case FCOL:
				printf("%10s|%10s","FCOL","");break;
			case ABCHAV:
				printf("%10s|%10s","ABCHAV","");break;
			case FCHAV:
				printf("%10s|%10s","FCHAV","");break;
			case PVIRG:
				printf("%10s|%10s","PVIRG","");break;
			case VIRG:
				printf("%10s|%10s","VIRG","");break;
			default:
				printf("%10d|%10s",i,"");break;
		}
		printf ("\n");
	}
}