%{
#include 	<string.h>
#define		ID			10
#define		CTINT		40
#define		CTREAL 		50
#define		OPREL		60
#define		OPMULT		70
#define		OPAD		80

#define 	MAIS 		81
#define 	MENOS 		82
#define		VEZES		71
#define		DIV			72
#define		REST		73
#define		MENOR		61
#define		MENIG		62
#define		MAIOR		63
#define		MAIG		64
#define		IGUAL		65
#define		DIFER		66
#define 	INVAL		99
#define		ABREP 		91
#define		FECHAP 		92
union {
	char string[50];
	int atr, valor;
	float valorreal;
	char carac;
} yylval;
%}
delim		[ \t\n\r]
ws			{delim}+
digito		[0-9]
letra		[A-Za-z]
opad 		(\+)|(\-)
opmult  	[*/%]
oprel 		([<>]=?)|(==)|(<>)
ctint		{digito}+
ctreal		{digito}+\.{digito}*
id			{letra}({letra}|{digito})*
fat			{id}|{ctint}|{ctreal}|(\(expr\))
term		{fat}({opmult}{fat})*
expr		{exprsimpl}|({exprsimpl}{oprel}{exprsimpl})
exprsimpl	{term}({opad}{term})*
%%
{ws}		{ ;}
{id}		{strcpy (yylval.string, yytext); return ID;}
{ctint}		{yylval.valor = atoi(yytext); return CTINT;}
{ctreal}	{yylval.valorreal = atof(yytext); return CTREAL;}
"<"			{yylval.atr = MENOR; return OPREL;}
"<="		{yylval.atr = MENIG; return OPREL;}
">"			{yylval.atr = MAIOR; return OPREL;}
">="		{yylval.atr = MAIG; return OPREL;}
"=="		{yylval.atr = IGUAL; return OPREL;}
"<>"		{yylval.atr = DIFER; return OPREL;}
"+"			{yylval.atr = MAIS; return OPAD;}
"-"			{yylval.atr = MENOS; return OPAD;}
"*"			{yylval.atr = VEZES; return OPMULT;}
"/"			{yylval.atr = DIV; return OPMULT;}
"%"			{yylval.atr = REST; return OPMULT;}
"("			{return ABREP;}
")"			{return FECHAP;}
.			{yylval.carac = yytext[0]; return INVAL;}
%%
main () {
	int i;
	printf ("\n   texto  |   tipo   | atributo \n");
	printf ("--------------------------------\n");
	while (i = yylex ()) {
		printf ("%10s|", yytext, i);
		switch (i) {
			case ID:
				printf ("%10s|%10s","ID", yylval.string); break;
			case CTINT:
				printf ("%10s|%10d","CTINT", yylval.valor); break;
			case CTREAL:
				printf ("%10s|%10f","CTREAL", yylval.valorreal); break;
			case OPREL:
				printf ("%10s|%10d","OPREL", yylval.atr); break;
			case INVAL:
				printf ("%10s|%10c","INVAL", yylval.carac); break;
			case OPAD:
				printf ("%10s|%10d","OPAD", yylval.atr); break; 
			case OPMULT:
				printf ("%10s|%10d","OPMULT", yylval.atr); break;
			case ABREP:
				printf("%10s|%10s","ABREP","");break;
			case FECHAP:
				printf("%10s|%10s","FECHAP","");break;
		}
		printf ("\n");
	}
}