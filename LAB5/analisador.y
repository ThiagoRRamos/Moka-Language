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


#define 	NOTVAR		0
#define 	INTEGER		1
#define 	LOGICAL		2
#define 	FLOAT			3
#define 	CHAR			4

#define		MAXDIMS 	10

int tab = 0;

/*   Definicao dos tipos de identificadores   */

#define 	IDPROG		1
#define 	IDVAR		2
#define 	IDFUNC		3
#define 	IDPROC		4

/* Definicao de constantes para os operadores de quadruplas */

#define		OPOR			1
#define		OPAND	 		2
#define 		OPLT	 		3
#define 		OPLE	 		4
#define		OPGT		   5
#define		OPGE			6
#define		OPEQ			7
#define		OPNE			8
#define		OPMAIS		9
#define		OPMENOS		10
#define		OPMULTIP		11
#define		OPDIV			12
#define		OPRESTO			13
#define		OPMENUN			14
#define		OPNOT			15
#define		OPATRIB			16
#define		OPENMOD			17
#define		NOP				18
#define		OPJUMP			19
#define		OPJF			20
#define		PARAM			21
#define		OPREAD			22
#define		OPWRITE			23
#define		OPCALL			24
#define		OPRETURN		25
#define		OPEXIT			26

/* Definicao de constantes para os tipos de operandos de quadruplas */

#define		IDLEOPND		0
#define		VAROPND		1
#define		INTOPND		2
#define		REALOPND		3
#define		CHAROPND		4
#define		LOGICOPND	5
#define		CADOPND		6
#define		ROTOPND		7
#define		MODOPND		8
#define		FUNCOPND	9

/*   Definicao de outras constantes   */

#define	NCLASSHASH	23
#define	TRUE			1
#define	FALSE			0

/*  Strings para nomes dos tipos de identificadores  */

char *nometipid[5] = {" ", "IDPROG", "IDVAR", "IDFUNC", "IDPROC"};

/*  Strings para nomes dos tipos de variaveis  */

char *nometipvar[5] = {"NOTVAR",
	"INTEGER", "LOGICAL", "FLOAT", "CHAR"
};

/* Strings para operadores de quadruplas */

char *nomeoperquad[27] = {"",
	"OR", "AND", "LT", "LE", "GT", "GE", "EQ", "NE", "MAIS",
	"MENOS", "MULT", "DIV", "RESTO", "MENUN", "NOT", "ATRIB",
	"OPENMOD", "NOP", "JUMP", "JF", "PARAM", "READ", "WRITE",
	"CALL", "RETORNO", "EXIT"
};

/*
	Strings para tipos de operandos de quadruplas
 */

char *nometipoopndquad[10] = {"IDLE",
	"VAR", "INT", "REAL", "CARAC", "LOGIC", "CADEIA", "ROTULO", "MODULO", "FUNCAO"
};

/*    Declaracoes para a tabela de simbolos     */

typedef struct celsimb celsimb;
typedef celsimb *simbolo;
struct celsimb {
	char *cadeia;
	int tid, tvar, ndims, dims[MAXDIMS+1];
	char inic, ref, array;
	simbolo prox, escopo;
};

/*  Variaveis globais para a tabela de simbolos e analise semantica  */

simbolo tabsimb[NCLASSHASH];
simbolo simb;
simbolo escopoatual, escopoglobal;
simbolo ultimachamada;
int tipocorrente;

/* Prototipos das funcoes para a tabela de simbolos e analise semantica */

void InicTabSimb (void);
void ImprimeTabSimb (void);
simbolo InsereSimb (char *, int, int, simbolo);
int hash (char *);
simbolo ProcuraSimb (char *, simbolo);
void DeclaracaoRepetida (char *);
void TipoInadequado (char *);
void NaoDeclarado (char *);
void NaoReferenciado(char *);
void NaoInicializado(char *);
void Incompatibilidade (char *);
void VerificaInicRef();
void AnalisarInsercaoSimb(char *,int tipo, int t, simbolo escopo);

/* Declaracoes para a estrutura do codigo intermediario */

typedef union atribopnd atribopnd;
typedef struct operando operando;
typedef struct celquad celquad;
typedef celquad *quadrupla;
typedef struct celmodhead celmodhead;
typedef celmodhead *modhead;

union atribopnd {
	simbolo simb; int valint; float valfloat;
	char valchar; char vallogic; char *valcad;
	quadrupla rotulo; modhead modulo;
};

struct operando {
	int tipo; atribopnd atr;
};

struct celquad {
	int num, oper; operando opnd1, opnd2, result;
	quadrupla prox;
};

struct celmodhead {
	simbolo modname; modhead prox;
   int modtip;
	quadrupla listquad;
};

/* Variaveis globais para o codigo intermediario */

quadrupla quadcorrente, quadaux, qtemp, qtemp1;
modhead codintermed, modcorrente;
int oper, numquadcorrente;
operando opnd1, opnd2, result, opndaux, opndaux2;
int numtemp;
const operando opndidle = {IDLEOPND, 0};

/* Prototipos das funcoes para o codigo intermediario */

void InicCodIntermed (void);
void InicCodIntermMod (simbolo);
void ImprimeQuadruplas (void);
quadrupla GeraQuadrupla (int, operando, operando, operando);
simbolo NovaTemp (int);
void RenumQuadruplas (quadrupla, quadrupla);

/* Declaracoes para atributos das expressoes e variaveis */

typedef struct infoexpressao infoexpressao;
struct infoexpressao {
	int tipo;
	operando opnd;
};

typedef struct infovariavel infovariavel;
struct infovariavel {
	simbolo simb;
	operando opnd;
};

%}
%union {
	char cadeia[50];
	int atr, valint;
	float valreal;
	char carac;
   	simbolo simb;
	infoexpressao infoexpr;
	infovariavel infovar;
	quadrupla quad;
   	int nsubscr, tipoexpr, nargs;
}

%type   	<nsubscr>    	ListExpr
%type   	<nsubscr>    	ListSubscr
%type		<infovar>		Variavel
%type 		<nargs>			ListVar   ListEscr
%type 		<infoexpr> 		Expressao  ExprAux1 ExprAux2 ExprAux3   ExprAux4   Termo   Fator ElemEscr ChamaFunc
%type		<cadeia>		Tipo
%type		<simb>			Parametro
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
Prog		: 	{InicTabSimb ();
				InicCodIntermed ();
				numtemp = 0;} PROGRAMA   ID   PVIRG
				{printf("programa %s;\n", $3);
				escopoatual = InsereSimb ($3, IDPROG, NOTVAR, NULL);
				InicCodIntermMod (escopoatual);
				opnd1.tipo = MODOPND;
          		opnd1.atr.modulo = modcorrente;
          		$<quad>$ = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);
          		escopoglobal=escopoatual;} 
          		Decls    SubProgs
          		{
          			modcorrente = codintermed->prox;
          			quadcorrente = modcorrente->listquad->prox;
          		}
          		CmdComp
          		{
          			$<quad>$ = GeraQuadrupla (OPEXIT, opndidle, opndidle, opndidle);
          			//GeraQuadrupla (OPEXIT, opndidle, opndidle, opndidle);
          			ImprimeTabSimb ();
          			VerificaInicRef();
          			ImprimeQuadruplas ();}

Decls 		:	   |   VAR {printf("\nvar\n");tab++;}  ListDecl {tab--;printf("\n");}
				
ListDecl	:	Declaracao    |   ListDecl   Declaracao

Declaracao	: 	Tipo {tabular(); printf("%s ", $1);}   ListElemDecl   PVIRG {printf(";\n");}

Tipo		: 	INT {strcpy($$, "int"); tipocorrente = INTEGER;}    
			|     REAL {strcpy($$, "real"); tipocorrente = FLOAT;}   
			|    CARAC {strcpy($$, "carac"); tipocorrente = CHAR;}    
			|     LOGIC {strcpy($$, "logic"); tipocorrente = LOGICAL;}
ListElemDecl	: 	ElemDecl      |     ListElemDecl   VIRG {printf(", ");}   ElemDecl

ElemDecl  	:	ID
				{printf("%s", $1);
				AnalisarInsercaoSimb($1, IDVAR, FALSE, escopoatual);}
				|    ID   ABCOL
				{printf("%s[", $1);
				AnalisarInsercaoSimb($1, IDVAR, TRUE, escopoatual);}  ListDim   FCOL  {printf("]");}  

ListDim	: 	CTINT {printf("%d", $1);ConferirValorDim($1);}   |    ListDim   VIRG   CTINT {printf(", %d", $3); ConferirValorDim($3);}

SubProgs	:    	   |   SubProgs   DeclSubProg

DeclSubProg	:   	Cabecalho   Decls   CmdComp {printf("\n"); escopoatual = escopoglobal;}

Cabecalho	:   	CabFunc   |   CabProc

CabFunc	:   	FUNCAO Tipo   ID 	ABPAR  FPAR  PVIRG
				{printf("funcao %s %s();\n", $2, $3);
					AnalisarInsercaoSimb ($3, IDFUNC, FALSE, escopoglobal);
					escopoatual=simb;
					InicCodIntermMod (escopoatual);
					opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			$<quad>$ = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);}
		|		FUNCAO Tipo   ID ABPAR
				{printf("funcao %s %s(", $2, $3);
					AnalisarInsercaoSimb ($3, IDFUNC, TRUE, escopoglobal);
					escopoatual=simb;
					InicCodIntermMod (escopoatual);
					opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			$<quad>$ = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);} ListParam  FPAR PVIRG {printf(");\n");}

CabProc	:   	PROCEDIMENTO   ID   ABPAR   FPAR   PVIRG
				{printf("procedimento %s();\n", $2);
					AnalisarInsercaoSimb ($2, IDPROC, FALSE, escopoglobal);
					escopoatual=simb;
					InicCodIntermMod (escopoatual);
					opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			$<quad>$ = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);}
		|	PROCEDIMENTO   ID   ABPAR
			{printf("procedimento %s(", $2);
			AnalisarInsercaoSimb ($2, IDPROC, TRUE, escopoglobal);
			escopoatual=simb;
			InicCodIntermMod (escopoatual);
			opnd1.tipo = MODOPND;
      		opnd1.atr.modulo = modcorrente;
      		$<quad>$ = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);} ListParam   FPAR   PVIRG {printf(");\n");}

ListParam	:   	Parametro   |   ListParam   VIRG {printf(", ");}   Parametro

Parametro	:   	Tipo   ID {AnalisarInsercaoSimb($2, IDVAR, FALSE, escopoatual); $$ = simb; $$->inic = TRUE; printf("%s %s", $1, $2); ConferirValorParam(tipocorrente);}

CmdComp	:     	ABCHAV
				{tabular ();
					printf ("\{\n");
					tab++;} ListCmd   FCHAV
				{tab--;
					tabular ();
					printf ("}\n");}

ListCmd	:   	   |   ListCmd   Comando

Comando  	:   	CmdComp   |   CmdSe   |   CmdEnquanto   |   CmdRepetir   |   CmdPara   
|   	CmdLer   |   CmdEscrever   |   CmdAtrib   |   ChamaProc   |   CmdRetornar   
|   	PVIRG

CmdSe		:   	SE   ABPAR
						{tabular();
						tab++;
						printf("se(");}
					Expressao
					{ConferirExpLogica($4.tipo);
						opndaux.tipo = ROTOPND;
						$<quad>$ = GeraQuadrupla (OPJF, $4.opnd, opndidle, opndaux);
					}
					FPAR {printf(")\n");}   Comando
					{tab--;
						$<quad>$ = quadcorrente;
						$<quad>5->result.atr.rotulo = GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}
					CmdSenao {
						if ($<quad>9->prox != quadcorrente) {
							quadaux = $<quad>9->prox;
							$<quad>9->prox = quadaux->prox;
							quadaux->prox = $<quad>9->prox->prox;
							$<quad>9->prox->prox = quadaux;
							RenumQuadruplas ($<quad>9, quadcorrente);
						}
					}

CmdSenao	:    	   |   SENAO
							{tabular();
							tab++;
							printf("senao\n");
							opndaux.tipo = ROTOPND;
							$<quad>$ = GeraQuadrupla (OPJUMP, opndidle, opndidle,opndaux);} 
							Comando
							{tab--;
							$<quad>2->result.atr.rotulo =GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}

CmdEnquanto	:   	ENQUANTO   ABPAR
						{tabular();
						printf("enquanto (");
						$<quad>$ = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);}
					Expressao
					{ConferirExpLogica($4.tipo);
					opndaux.tipo = ROTOPND;
					$<quad>$ = GeraQuadrupla(OPJF, $4.opnd, opndidle, opndaux);}
					FPAR
					{printf(")\n");tab++;}
					Comando
					{tab--;
					$<quad>$ = quadcorrente;
					opndaux.tipo = ROTOPND;
					$<quad>$ = GeraQuadrupla (OPJUMP, opndidle, opndidle, opndaux);
					$<quad>$->result.atr.rotulo = $<quad>3;
					$<quad>5->result.atr.rotulo = GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}

CmdRepetir	:   	REPETIR
					{tabular();
						printf("repetir \n");
						tab++;
						$<quad>$ = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);}
					Comando
					ENQUANTO
					{tab--; tabular(); printf("enquanto("); }
					ABPAR
					Expressao
					{ConferirExpLogica($7.tipo);
						opndaux.tipo = ROTOPND;
						$<quad>$ = GeraQuadrupla(OPJF, $7.opnd, opndidle, opndaux);
						opndaux.tipo = ROTOPND;
						quadaux = GeraQuadrupla(OPJUMP, opndidle, opndidle, opndaux);
						quadaux->result.atr.rotulo = $<quad>2;
						quadaux = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);
						$<quad>$->result.atr.rotulo = quadaux;
						}   FPAR   PVIRG {printf(");\n"); }

CmdPara	:   	PARA
				{tabular();
					tab++;
					printf("para(");} ABPAR Variavel
				{printf(" := ");
					ConferirPara($4.simb);
					InicializarEReferenciar($4.simb);} ATRIB  Expressao
				{ConferirAtribuicao($4.simb, $7.tipo);
				 GeraQuadrupla (OPATRIB, $7.opnd, opndidle, $4.opnd);
				} PVIRG
				{printf("; ");
				$<quad>$ = GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}  Expressao   PVIRG
				{printf("; ");
					ConferirExpLogica($11.tipo);
					opndaux.tipo = ROTOPND;
					$<quad>$ = GeraQuadrupla(OPJF, $11.opnd, opndidle, opndaux);}  Variavel   ATRIB
				{printf(" := ");
					MesmaVariavel($4.simb,$14.simb);
					$<quad>$ = quadcorrente;
				}   Expressao
				FPAR {printf(")\n");
				opndaux.tipo = ROTOPND;
				}
				Comando
				{tab--;
					quadaux = $<quad>16->prox;
					$<quad>16->prox = quadaux->prox;
					quadcorrente->prox = quadaux;
					quadcorrente = quadaux;
					$<quad>$ = GeraQuadrupla (OPATRIB, $17.opnd, opndidle, $14.opnd);
					quadaux = GeraQuadrupla (OPJUMP, opndidle, opndidle, opndaux);
					quadaux->result.atr.rotulo = $<quad>10;
					quadaux = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);
					$<quad>13->result.atr.rotulo = quadaux;}

CmdLer	:   	LER   ABPAR  {tabular();printf("ler(");} ListVar   FPAR   PVIRG
				{printf(");\n");
					opnd1.tipo = INTOPND;
					opnd1.atr.valint = $4;
					GeraQuadrupla (OPREAD, opnd1, opndidle, opndidle);}

ListVar	:   	Variavel {InicializarEReferenciar($1.simb);
							$$ = 1;	
							GeraQuadrupla (PARAM, $1.opnd, opndidle, opndidle);}
				|   ListVar   VIRG {printf(", ");}   Variavel
				{InicializarEReferenciar($4.simb);
					$$ = $1 + 1;
					GeraQuadrupla (PARAM, $4.opnd, opndidle, opndidle);}

CmdEscrever	:   	ESCREVER   ABPAR {tabular();printf("escrever(");}  ListEscr   FPAR   PVIRG
					{printf(");\n");
						opnd1.tipo = INTOPND;
						opnd1.atr.valint = $4;
						GeraQuadrupla (OPWRITE, opnd1, opndidle, opndidle);}

ListEscr	:   	ElemEscr
					{
						$$ = 1;
						GeraQuadrupla (PARAM, $1.opnd, opndidle, opndidle);
					}
					|   ListEscr   VIRG {printf(", ");}   ElemEscr {
							$$ = $1 + 1;
							GeraQuadrupla (PARAM, $4.opnd, opndidle, opndidle);
						}

ElemEscr	:   	CADEIA {printf("\"%s\"", $1);
						$$.opnd.tipo = CADOPND;
						$$.opnd.atr.valcad = malloc (strlen($1) + 1);
						strcpy ($$.opnd.atr.valcad, $1);}
					|   Expressao
ChamaProc	:   	CHAMAR ID ABPAR  FPAR  PVIRG
					{tabular();
						printf("chamar %s();\n", $2);
						VerificarExistencia($2);
						VerificarTamanhoSemParametros(simb);
						VerificarTipo(simb, IDPROC);}
					|	CHAMAR   ID ABPAR
					{tabular();
						printf("chamar %s(", $2);
							VerificarExistencia($2);
							ultimachamada=simb;}  ListExpr   FPAR   PVIRG
					{printf(");\n");
						VerificarTamanho(simb,$5);
						VerificarTipo(simb, IDPROC);
						opndaux.tipo = INTOPND;
						opndaux.atr.valint = simb->ndims;
						opndaux2.tipo = FUNCOPND;
						opndaux2.atr.simb = simb;
						GeraQuadrupla  (OPCALL, opndaux2, opndaux, opndidle);}
CmdRetornar	:   	RETORNAR   PVIRG
					{tabular ();
						printf("retornar;\n");
						ConferirRetornoProcedimento(escopoatual);
						GeraQuadrupla  (OPRETURN, opndidle, opndidle, opndidle);} 
					|   RETORNAR
					{tabular();
						printf("retornar ");}  Expressao
					{ConferirRetornoFuncao(escopoatual,$3.tipo);} PVIRG
					{printf(";\n");
					GeraQuadrupla  (OPRETURN, $3.opnd, opndidle, opndidle);}
CmdAtrib  	:   	{tabular ();} Variavel
					{if  ($2.simb != NULL)
							$2.simb->inic = $2.simb->ref = TRUE;
							printf(" := ");}  ATRIB  Expressao   PVIRG 
							{printf(";\n");
							ConferirAtribuicao($2.simb, $5.tipo);
							GeraQuadrupla (OPATRIB, $5.opnd, opndidle, $2.opnd);
}
ListExpr	:   	Expressao
					{$$ = 1;
						checarTipo(1,$1.tipo);
						GeraQuadrupla (PARAM, $1.opnd, opndidle, opndidle);}
					|   ListExpr   VIRG {printf(", ");}  Expressao
					{$$ = $$ + 1;
						checarTipo($$,$4.tipo);
						GeraQuadrupla (PARAM, $4.opnd, opndidle, opndidle);}
Expressao  	:   	ExprAux1     |     Expressao   OR {printf(" || ");} ExprAux1 {ConferirOr($1, $4, &$$);}
ExprAux1  	:   	ExprAux2     |     ExprAux1   AND {printf(" && ");} ExprAux2 {ConferirAnd($1, $4, &$$);}
ExprAux2  	:   	ExprAux3     |     NOT {printf("!");}   ExprAux3  {ConferirNot($3, &$$);}
ExprAux3  	:   	ExprAux4     |     ExprAux4   OPREL {switch($2){
									case MAIOR: printf(" > ");break;
									case MENOR: printf(" < ");break;
									case MAIG: printf(" >= ");break;
									case MENIG: printf(" <= ");break;
									case IGUAL: printf(" = ");break;
									case DIFER: printf(" != ");break;
								}
								}  ExprAux4 {ConferirOpRel($1, $2, $4, &$$);}
ExprAux4  	:   	Termo     
			|     ExprAux4   OPAD {switch($2){
									case MAIS: printf(" + ");break;
									case MENOS: printf(" - ");break;
								}
								}   Termo {ConferirOpAd($1, $2, $4, &$$);}
Termo  	:   	Fator   
			|   Termo   OPMULT {switch($2){
									case VEZES: printf(" * ");break;
									case DIV: printf(" / ");break;
									case REST: printf(" %% ");break;
								}
								}   Fator {ConferirOpMult($1, $2, $4, &$$);}
Fator		:   	Variavel  {
						if  ($1.simb != NULL)  {
               				$1.simb->ref  =  TRUE;
                  			$$.tipo = $1.simb->tvar;
                  			$$.opnd = $1.opnd;
               			}
				}
			|   CTINT {printf("%d", $1);
						$$.tipo = INTEGER;
						$$.opnd.tipo = INTOPND;
						$$.opnd.atr.valint = $1;}
			|   CTREAL {printf("%f", $1);
						$$.tipo = FLOAT;
						$$.opnd.tipo = REALOPND;
						$$.opnd.atr.valfloat = $1;}  
			|   CTCARAC   {printf("%c", $1);
							$$.tipo = CHAR;
							$$.opnd.tipo = CHAROPND;
							$$.opnd.atr.valchar = $1;}
			|   VERDADE   {printf("verdade");
							$$.tipo = LOGICAL;
							$$.opnd.tipo = LOGICOPND;
							$$.opnd.atr.vallogic = 1;}
			|   FALSO   {printf("falso");
							$$.tipo = LOGICAL;
							$$.opnd.tipo = LOGICOPND;
							$$.opnd.atr.vallogic = 0;}
			|   NEG {printf("~");}   Fator {
				if ($3.tipo != INTEGER && $3.tipo != FLOAT && $3.tipo != CHAR)
					Incompatibilidade  ("Operando improprio para menos unario");
				if ($3.tipo == FLOAT)
					$$.tipo = FLOAT;
				else
					$$.tipo = INTEGER;
				$$.opnd.tipo = VAROPND;
				$$.opnd.atr.simb = NovaTemp ($$.tipo);
          		GeraQuadrupla  (OPMENUN, $3.opnd, opndidle, $$.opnd);
		    }
			|   ABPAR {printf("(");}  Expressao   FPAR
					{printf(")");
					$$.tipo = $3.tipo;
					$$.opnd = $3.opnd;} 
			|   ChamaFunc
				{$$.tipo = $1.tipo;
				$$.opnd.tipo = VAROPND;
				$$.opnd.atr.simb = NovaTemp ($$.tipo);
				opndaux.tipo = INTOPND;
				opndaux.atr.valint = $1.opnd.atr.simb->ndims;
				GeraQuadrupla  (OPCALL, $1.opnd, opndaux, $$.opnd);}
Variavel	:   	ID {printf("%s",$1);
						simb = ProcuraSimb ($1, escopoatual);
						if (simb == NULL){
							simb = ProcuraSimb ($1, escopoglobal);
						}
						if (simb == NULL){
							NaoDeclarado ($1);
						}
						else if (simb->tid != IDVAR)
							TipoInadequado ($1);
						$$.simb = simb;
						if ($$.simb != NULL)
               				if ($$.simb->array == TRUE)
								Esperado ("Subscrito\(s)");
						$$.opnd.tipo = VAROPND;
                  		$$.opnd.atr.simb = $$.simb;}  |
					ID   ABCOL {
	 					printf ("%s[", $1);
	        			simb = ProcuraSimb ($1, escopoatual);
						if (simb == NULL)   simb = ProcuraSimb ($1, escopoglobal);
						if (simb == NULL)   NaoDeclarado ($1);
						else if (simb->tid != IDVAR) TipoInadequado ($1);
	         			$<simb>$ = simb;
					}   ListSubscr   FCOL   { printf ("] ");  $$.simb = $<simb>3;
		if ($$.simb != NULL)
			if ($$.simb->array == FALSE)
				NaoEsperado ("Subscrito\(s)");
                 	 else if ($$.simb->ndims != $4)
 				Incompatibilidade ("Numero de subscritos incompativel com declaracao");}
ListSubscr	: ExprAux4 {ConferirSubscr($1.tipo);$$ =1;} | ListSubscr   VIRG {printf(",");}   ExprAux4 {ConferirSubscr($4.tipo);$$ =$1+1;}
ChamaFunc	:   	ID   ABPAR    FPAR
			{printf("%s()", $1);
				VerificarExistencia($1);
				VerificarTamanhoSemParametros(simb);
				VerificarTipo(simb, IDFUNC);
				$<simb>$ = simb;
				$$.tipo=simb->tid;
				$$.opnd.tipo = FUNCOPND;
				$$.opnd.atr.simb = simb;
				VerificarRecursividade(escopoatual,simb);}
			|	ID ABPAR
			{printf("%s(", $1);
				VerificarExistencia($1);
				ultimachamada=simb;
				$<simb>$ = simb;} ListExpr   FPAR
			{printf(")");
				VerificarTamanho($<simb>3,$4);
				VerificarTipo($<simb>3, IDFUNC);
				VerificarRecursividade(escopoatual,$<simb>3);
				$$.tipo=$<simb>3->tvar;
				$$.opnd.tipo = FUNCOPND;
				$$.opnd.atr.simb = $<simb>3;
			}
%%
#include "lex.yy.c"

tabular () {
	int i;
	for (i = 1; i <= tab; i++)
   	printf ("\t");
}

/*  InicTabSimb: Inicializa a tabela de simbolos   */

void InicTabSimb () {
	int i;
	for (i = 0; i < NCLASSHASH; i++) 
		tabsimb[i] = NULL;
}

/*
	ProcuraSimb (cadeia): Procura cadeia na tabela de simbolos;
	Caso ela ali esteja, retorna um ponteiro para sua celula;
	Caso contrario, retorna NULL.
 */

simbolo ProcuraSimb (char *cadeia, simbolo escopo) {
	simbolo s; int i;
	i = hash (cadeia);
	for (s = tabsimb[i]; (s!=NULL);  s = s->prox){
		if(strcmp(cadeia, s->cadeia)==0 && strcmp(s->escopo->cadeia, escopo->cadeia)==0){
			return s;
		}
	}
	return NULL;
}

/*
	InsereSimb (cadeia, tid, tvar): Insere cadeia na tabela de
	simbolos, com tid como tipo de identificador e com tvar como
	tipo de variavel; Retorna um ponteiro para a celula inserida
 */

simbolo InsereSimb (char *cadeia, int tid, int tvar, simbolo escopo) {
	int i; simbolo aux, s;
	i = hash (cadeia); aux = tabsimb[i];
	s = tabsimb[i] = (simbolo) malloc (sizeof (celsimb));
	s->cadeia = (char*) malloc ((strlen(cadeia)+1) * sizeof(char));
	s->escopo = escopo;
	strcpy (s->cadeia, cadeia);
	s->tid = tid;		s->tvar = tvar;
	s->inic = FALSE;	s->ref = FALSE;
	s->prox = aux;	return s;
}

/*
	hash (cadeia): funcao que determina e retorna a classe
	de cadeia na tabela de simbolos implementada por hashing
 */

int hash (char *cadeia) {
	int i, h;
	for (h = i = 0; cadeia[i]; i++) {h += cadeia[i];}
	h = h % NCLASSHASH;
	return h;
}

/* ImprimeTabSimb: Imprime todo o conteudo da tabela de simbolos  */

void ImprimeTabSimb () {
	int i; simbolo s;
	printf ("\n\n   TABELA  DE  SIMBOLOS:\n\n");
	for (i = 0; i < NCLASSHASH; i++)
		if (tabsimb[i]) {
			printf ("Classe %d:\n", i);
			for (s = tabsimb[i]; s!=NULL; s = s->prox){
				printf ("  (%s, %s", s->cadeia,  nometipid[s->tid]);
				if (s->tid == IDVAR){
					printf (", %s, %d, %d", nometipvar[s->tvar], s->inic, s->ref);
					if (s->array == TRUE) {
						int j;
						printf (", EH ARRAY\n\tndims = %d, dimensoes:", s->ndims);
						for (j = 1; j <= s->ndims; j++){
                  			printf ("  %d", s->dims[j]);
                  		}
					}
				}
				if(s->tid == IDFUNC){
					printf(", %s", nometipvar[s->tvar]);
					if (s->array == TRUE) {
						int j;
						printf (", EH ARRAY\n\tndims = %d, dimensoes:", s->ndims);
						for (j = 1; j <= s->ndims; j++){
                  			printf ("  %s", nometipvar[s->dims[j]]);
                  		}
					}
				}
				if(s->tid == IDPROC ){
					if (s->array == TRUE) {
						int j;
						printf (", EH ARRAY\n\tndims = %d, dimensoes:", s->ndims);
						for (j = 1; j <= s->ndims; j++){
                  			printf ("  %s", nometipvar[s->dims[j]]);
                  		}
					}
				}
				if(s->escopo)
					printf(", %s)\n", s->escopo->cadeia);
				else
					printf(")\n");
			}
		}
}

/*  Mensagens de erros semanticos  */

void DeclaracaoRepetida (char *s) {
	printf ("\n\n***** Declaracao Repetida: %s *****\n\n", s);
}

void NaoDeclarado (char *s) {
	printf ("\n\n***** Identificador Nao Declarado: %s *****\n\n", s);
}

void TipoInadequado (char *s) {
	printf ("\n\n***** Identificador de Tipo Inadequado: %s *****\n\n", s);
}

void NaoInicializado (char *s) {
	printf ("\n\n***** Identificador nao inicializado: %s *****\n\n", s);
}

void NaoReferenciado (char *s) {
	printf ("\n\n***** Identificador nao referenciado: %s *****\n\n", s);
}

void Incompatibilidade (char *s) {
	printf ("\n\n***** Incompatibilidade: %s *****\n\n", s);
}

void Esperado (char *s) {
		printf ("\n\n***** Esperado: %s *****\n\n", s);
}

void NaoEsperado (char *s) {
	printf ("\n\n***** Nao Esperado: %s *****\n\n", s);
}

void Recursividade (char *s) {
	printf ("\n\n***** Recursividade: %s *****\n\n", s);
}

void VerificaInicRef(){
	int i; simbolo s;
	for (i = 0; i < NCLASSHASH; i++)
		if (tabsimb[i]) {
			for (s = tabsimb[i]; s!=NULL; s = s->prox){
				if (s->tid == IDVAR){
					if(!s->inic)
						NaoInicializado(s->cadeia);
					if(!s->ref)
						NaoReferenciado(s->cadeia);
				}
			}
		}	
}

void AnalisarInsercaoSimb(char* n, int tipo, int t, simbolo escopo){
	if  (ProcuraSimb (n, escopo)  !=  NULL)
		DeclaracaoRepetida (n);
	else
		simb = InsereSimb (n,  tipo,  tipocorrente, escopo);
		simb->array = t;
		if(t){
			simb->ndims = 0;
		}
}

void ConferirOpMult(infoexpressao p, int s, infoexpressao q, infoexpressao *pointer){
	switch (s) {
	     case VEZES: case DIV:
	        if (p.tipo != INTEGER && p.tipo != FLOAT && p.tipo != CHAR || q.tipo != INTEGER && q.tipo!=FLOAT && q.tipo!=CHAR)
	               Incompatibilidade("Operando improprio para operador aritmetico multiplicacao/divisao");
	        if (p.tipo == FLOAT || q.tipo == FLOAT){
	        	pointer->tipo = FLOAT;
	        }else{
	        	pointer->tipo = INTEGER;
	        }
	        pointer->opnd.tipo = VAROPND;  
			pointer->opnd.atr.simb = NovaTemp (pointer->tipo);
			if (s == VEZES)  
			       GeraQuadrupla   (OPMULTIP, p.opnd, q.opnd, pointer->opnd);
			else  GeraQuadrupla  (OPDIV, p.opnd, q.opnd, pointer->opnd);
	        break;
	     case REST:
	        if (p.tipo != INTEGER && p.tipo != CHAR ||  q.tipo != INTEGER && q.tipo != CHAR){
	               Incompatibilidade("Operando improprio para operador resto");
	        }
	        pointer->tipo = INTEGER;
	        pointer->opnd.tipo = VAROPND;  
			pointer->opnd.atr.simb = NovaTemp (pointer->tipo);
			GeraQuadrupla (OPRESTO, p.opnd, q.opnd, pointer->opnd);
	        break;
	     }
}

void ConferirOpAd(infoexpressao p, int s, infoexpressao q, infoexpressao *pointer){
	if (p.tipo != INTEGER && p.tipo != FLOAT && p.tipo != CHAR || q.tipo != INTEGER && q.tipo!=FLOAT && q.tipo!=CHAR)
           Incompatibilidade("Operando improprio para operador aritmetico de adicao/subtracao");
    if (p.tipo == FLOAT || q.tipo == FLOAT){
    	pointer->tipo = FLOAT;
    }else{
    	pointer->tipo = INTEGER;
    }
    pointer->opnd.tipo = VAROPND;  
	pointer->opnd.atr.simb = NovaTemp (pointer->tipo);
	if(s == MAIS)
		GeraQuadrupla (OPMAIS, p.opnd, q.opnd, pointer->opnd);
	else if(s == MENOS)
		GeraQuadrupla (OPMENOS, p.opnd, q.opnd, pointer->opnd);
}

void ConferirOpRel(infoexpressao p, int s, infoexpressao q, infoexpressao *pointer){
	switch(s){
	case IGUAL: case DIFER:
		if((p.tipo == LOGICAL && q.tipo!=LOGICAL)||(q.tipo == LOGICAL && p.tipo!=LOGICAL)){
			Incompatibilidade("Operando improprio para operador de igual/diferente");
		}
		pointer->opnd.tipo = VAROPND;  
		pointer->opnd.atr.simb = NovaTemp (pointer->tipo);
		if(s == IGUAL){
			GeraQuadrupla (OPEQ, p.opnd, q.opnd, pointer->opnd);
		}else if(s == DIFER){
			GeraQuadrupla (OPNE, p.opnd, q.opnd, pointer->opnd);
		}
		break;
	case MAIOR: case MENOR: case MENIG: case MAIG:
		if (p.tipo != INTEGER && p.tipo != FLOAT && p.tipo != CHAR || q.tipo != INTEGER && q.tipo!=FLOAT && q.tipo!=CHAR){
           Incompatibilidade("Operando improprio para operador de maior/menor/maig/menig");
        }
        pointer->opnd.tipo = VAROPND;  
		pointer->opnd.atr.simb = NovaTemp (pointer->tipo);
		if(s == MAIOR){
			GeraQuadrupla (OPGT, p.opnd, q.opnd, pointer->opnd);
		}else if(s == MENOR){
			GeraQuadrupla (OPLT, p.opnd, q.opnd, pointer->opnd);
		}else if(s == MAIG){
			GeraQuadrupla (OPGE, p.opnd, q.opnd, pointer->opnd);
		}else if(s == MENIG){
			GeraQuadrupla (OPLE, p.opnd, q.opnd, pointer->opnd);
		}
		break;
	}
	pointer->tipo = LOGICAL;
}

void ConferirNot(infoexpressao p, infoexpressao *pointer){
	if(p.tipo!=LOGICAL){
		Incompatibilidade("Operando improprio para operador de negacao");
	}
	pointer->tipo = LOGICAL;
	GeraQuadrupla (OPNOT, p.opnd, opndidle, pointer->opnd);
}

void ConferirAnd(infoexpressao p, infoexpressao q, infoexpressao *pointer){
	if(p.tipo!=LOGICAL||q.tipo!=LOGICAL){
		Incompatibilidade("Operando improprio para operador de AND");
	}
	pointer->tipo = LOGICAL;
	GeraQuadrupla (OPAND, p.opnd, q.opnd, pointer->opnd);
}

void ConferirOr(infoexpressao p, infoexpressao q, infoexpressao *pointer){
	if(p.tipo!=LOGICAL||q.tipo!=LOGICAL){
		Incompatibilidade("Operando improprio para operador de OR");
	}
	pointer->tipo = LOGICAL;
	GeraQuadrupla (OPOR, p.opnd, q.opnd, pointer->opnd);
}

void ConferirAtribuicao(simbolo p, int q){
	if (p != NULL){
		if (((p->tvar == INTEGER || p->tvar == CHAR) && (q == FLOAT || q == LOGICAL)) || (p->tvar == FLOAT && q == LOGICAL) || (p->tvar == LOGICAL && q != LOGICAL)){
			printf("%s %s %s",p->cadeia, nometipvar[p->tvar], nometipvar[q]);
			Incompatibilidade ("Lado direito de comando de atribuicao improprio");
		}		
	}
}

void ConferirRetornoProcedimento(simbolo s){
	if(s==NULL){
		return;
	}
	if(s->tid == IDFUNC){
		Esperado("Funcao precisa de variavel sendo retornada");
	}
}

void ConferirRetornoFuncao(simbolo s, int q){
	if(s==NULL){
		return;
	}
	if(s->tid == IDPROC){
		NaoEsperado("Procedimento nao aceita variavel sendo retornada");
	}
	int p = s->tvar;
	if (((p == INTEGER || p == CHAR) && (q == FLOAT || q == LOGICAL)) || (p == FLOAT && q == LOGICAL) || (p == LOGICAL && q != LOGICAL))
				Incompatibilidade ("Tipos incompativeis para retorno");
}

void ConferirExpLogica(int p){
	if(p!=LOGICAL){
		Incompatibilidade ("Expressao deveria ser logica");
	}
}

void ConferirPara(simbolo p){
	if(p!=NULL && p->tvar!=INTEGER && p->tvar!=CHAR){
		Incompatibilidade ("Variavel inicializada do PARA deveria ser inteira ou caractere");
	}
}

void InicializarEReferenciar(simbolo s){
	s->inic = s->ref = TRUE;
}

void ConferirValorDim(int v){
	if (v <= 0)
		Esperado("Valor inteiro positivo");
    simb->ndims++; simb->dims[simb->ndims] = v;
}

void ConferirSubscr(int t){
		if (t != INTEGER && t != CHAR){
				printf("Tipo inadequado para subscrito: %s", nometipvar[t]);
        	    Incompatibilidade ("Tipo inadequado para subscrito");
       	}
}

void ConferirValorParam(int p){
    escopoatual->ndims++; escopoatual->dims[escopoatual->ndims] = p;
}

void VerificarExistencia(char* s){
	simb = ProcuraSimb (s, escopoatual);
	if (simb == NULL){
		simb = ProcuraSimb (s, escopoglobal);
	}
	if (simb == NULL){
		NaoDeclarado (s);
	}
}

void VerificarTamanho(simbolo simb,int val){
	if (simb != NULL)
			if (simb->array == FALSE)
				NaoEsperado ("Subscrito\(s)");
            else if (simb->ndims != val)
 				Incompatibilidade ("Numero de parametros incompativel com declaracao");
}

void VerificarTamanhoSemParametros(simbolo simb){
	if (simb != NULL)
			if (simb->array == TRUE)
				Esperado ("Parametro\(s)");
}

void MesmaVariavel(simbolo s1, simbolo s2){
	if(strcmp(s1->cadeia,s2->cadeia))
		Incompatibilidade ("Variavel atualizada nao eh a mesma inicializada");
}

void VerificarTipo(simbolo s1, int tipo){
	if(s1->tid!=tipo){
		TipoInadequado(s1->cadeia);
	}
}

void VerificarRecursividade(simbolo escopoatual,simbolo simb){
	if(!strcmp(simb->cadeia,escopoatual->cadeia)){
		Recursividade("Procedimento/funcao chamando ela mesma");
	}
}

void checarTipo(int pos,int tipo){
	if(!ultimachamada->array){
		return; //Ja seremos avisados mais tarde, nao precisamos agora
	}
	if(pos > ultimachamada->ndims){
		return; //Mesma coisa
	}
	int esp = ultimachamada->dims[pos];
	if (((esp == INTEGER || esp == CHAR) && (tipo == FLOAT || tipo == LOGICAL)) || (esp== FLOAT && tipo == LOGICAL) || (esp == LOGICAL && tipo != LOGICAL))
		Incompatibilidade("Parametro incompativel com tipo");
}

/* Funcoes para o codigo intermediario */

void InicCodIntermed () {
	modcorrente = codintermed = malloc (sizeof (celmodhead));
   modcorrente->listquad = NULL;
	modcorrente->prox = NULL;
}

void InicCodIntermMod (simbolo simb) {
	modcorrente->prox = malloc (sizeof (celmodhead));
	modcorrente = modcorrente->prox;
	modcorrente->prox = NULL;
	modcorrente->modname = simb;
	modcorrente->modtip = simb->tid;
	modcorrente->listquad = malloc (sizeof (celquad));
	quadcorrente = modcorrente->listquad;
	quadcorrente->prox = NULL;
	numquadcorrente = 0;
	quadcorrente->num = numquadcorrente;
}

quadrupla GeraQuadrupla (int oper, operando opnd1, operando opnd2,
	operando result) {
	quadcorrente->prox = malloc (sizeof (celquad));
	quadcorrente = quadcorrente->prox;
	quadcorrente->oper = oper;
	quadcorrente->opnd1 = opnd1;
	quadcorrente->opnd2 = opnd2;
	quadcorrente->result = result;
	quadcorrente->prox = NULL;
	numquadcorrente ++;
   quadcorrente->num = numquadcorrente;
   return quadcorrente;
}

simbolo NovaTemp (int tip) {
	simbolo simb; int temp, i, j;
	char nometemp[10] = "##", s[10] = {0};

	numtemp ++; temp = numtemp;
	for (i = 0; temp > 0; temp /= 10, i++)
		s[i] = temp % 10 + '0';
	i --;
	for (j = 0; j <= i; j++)
		nometemp[2+i-j] = s[j];
	simb = InsereSimb (nometemp, IDVAR, tip, escopoatual);
	simb->inic = simb->ref = TRUE;
  	simb->array = FALSE;
	return simb;
}

void ImprimeQuadruplas () {
	modhead p;
	quadrupla q;
	for (p = codintermed->prox; p != NULL; p = p->prox) {
		printf ("\n\nQuadruplas do modulo %s:\n", p->modname->cadeia);
		for (q = p->listquad->prox; q != NULL; q = q->prox) {
			printf ("\n\t%4d) %s", q->num, nomeoperquad[q->oper]);
			printf (", (%s", nometipoopndquad[q->opnd1.tipo]);
			doPrint(q->opnd1);
			printf (")");
			printf (", (%s", nometipoopndquad[q->opnd2.tipo]);
			doPrint(q->opnd2);
			printf (")");
			printf (", (%s", nometipoopndquad[q->result.tipo]);
			doPrint(q->result);
			printf (")");
		}
	}
   printf ("\n");
}

void doPrint(operando o){
	switch (o.tipo) {
				case IDLEOPND: break;
				case VAROPND: printf (", %s", o.atr.simb->cadeia); break;
				case INTOPND: printf (", %d", o.atr); break;
				case REALOPND: printf (", %g", o.atr.valfloat); break;
				case CHAROPND: printf (", %c", o.atr.valchar); break;
				case LOGICOPND: printf (", %d", o.atr.vallogic); break;
				case CADOPND: printf (", %s", o.atr.valcad); break;
				case ROTOPND: printf (", %d", o.atr.rotulo->num); break;
				case MODOPND: printf(", %s", o.atr.modulo->modname->cadeia);break;
				case FUNCOPND: printf(", %s", o.atr.simb->cadeia); break;
			}
}

void RenumQuadruplas (quadrupla quad1, quadrupla quad2) {
	quadrupla q; int nquad;
	for (q = quad1->prox, nquad = quad1->num; q != quad2; q = q->prox) {
      nquad++;
		q->num = nquad;
	}
}



