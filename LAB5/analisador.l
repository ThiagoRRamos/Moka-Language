%{
void comentario(void);
%}

delim			[ \t\n\r]
ws				{delim}+

letra			[a-zA-Z]
digito			[0-9]
carac1			\\.|[^\\']
carac2			\\.|[^\\\"]
id				{letra}({letra}|{digito})*
ctint			{digito}+
ctreal			{digito}+\.{digito}*([Ee][+-]?{digito}+)?
ctcarac			'{carac1}'
cadeia			\"{carac2}*\"

%%
{ws}		{ ;}

"carac" 		{return CARAC;}
"int"			{return INT;}
"real"			{return REAL;}
"verdade"		{return VERDADE;}
"chamar" 		{return CHAMAR;}
"ler" 			{return LER;}
"repetir" 		{return REPETIR;}
"enquanto" 		{return ENQUANTO;}
"logic"			{return LOGIC;}
"retornar" 		{return RETORNAR;}
"escrever" 		{return ESCREVER;}
"para" 			{return PARA;}
"se" 			{return SE;}
"falso"			{return FALSO;}
"procedimento" 	{return PROCEDIMENTO;}
"senao" 		{return SENAO;}
"funcao" 		{return FUNCAO;}
"programa"		{return PROGRAMA;}
"var" 			{return VAR;}


{id}			{strcpy (yylval.cadeia, yytext); return ID;}
{ctint}			{yylval.valint = atoi(yytext); return CTINT;}
{ctreal}		{yylval.valreal = atof(yytext); return CTREAL;}
{cadeia}		{strcpy (yylval.cadeia, yytext); return CADEIA;}
{ctcarac}		{yylval.carac = yytext[1]; return CTCARAC;}

"||"			{return OR;}
"&&"			{return AND;}
"!"				{return NOT;}
"~"				{return NEG;}
">"				{yylval.atr = MAIOR; return OPREL;}
"<"				{yylval.atr = MENOR; return OPREL;}
">="			{yylval.atr = MAIG; return OPREL;}
"<="			{yylval.atr = MENIG; return OPREL;}
"="				{yylval.atr = IGUAL; return OPREL;}
"!="			{yylval.atr = DIFER; return OPREL;}
"+"				{yylval.atr = MAIS; return OPAD;}
"-"				{yylval.atr = MENOS; return OPAD;}
"*"				{yylval.atr = VEZES; return OPMULT;}
"/"				{yylval.atr = DIV; return OPMULT;}
"%"				{yylval.atr = REST; return OPMULT;}
":="			{return ATRIB;}
"("				{return ABPAR;}
")"				{return FPAR;}
"["				{return ABCOL;}
"]"				{return FCOL;}
"{"				{return ABCHAV;}
"}"				{return FCHAV;}
";"				{return PVIRG;}
","				{return VIRG;}
"/*" 			{comentario();}
.				{yylval.carac = yytext[0]; return INVAL;}
%%
void comentario() {
	char c;
	int estado;
	estado = 1;
	while(estado!=3){
		switch(estado){
			case 1:
				c = input();
				if(c == EOF){
					estado = 3;
				}else if (c == '*'){
					estado = 2;
				}
				break;
			case 2:
				c = input();
				if(c == EOF||c == '/'){
					estado = 3;
				}else if (c == '*'){
					estado = 2;
				}else{
					estado = 1;
				}
				break;
		}
	}
}