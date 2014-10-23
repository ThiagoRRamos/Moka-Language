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
#define 	OPLT	 		3
#define 	OPLE	 		4
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
#define		INDOP			27
#define		INDEXOP			28
#define		OPATRIBPONT		29
#define		OPCONTAPONT		30

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

char *nomeoperquad[31] = {"",
	"OR", "AND", "LT", "LE", "GT", "GE", "EQ", "NE", "MAIS",
	"MENOS", "MULT", "DIV", "RESTO", "MENUN", "NOT", "ATRIB",
	"OPENMOD", "NOP", "JUMP", "JF", "PARAM", "READ", "WRITE",
	"CALL", "RETORNO", "EXIT", "IND", "INDEX", "ATRIBPONT", "CONTAPONT"
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
	char ponteiro;
	int ponto;
	simbolo prox, escopo, referente;
	int *valint;
	float *valfloat;
	char *valchar, *vallogic;
	simbolo listaparametros[MAXDIMS+1];
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
int numtemp, j;
char temretorno;
const operando opndidle = {IDLEOPND, 0};
FILE *finput;

/* Prototipos das funcoes para o codigo intermediario */

void InicCodIntermed (void);
void InicCodIntermMod (simbolo);
void ImprimeQuadruplas (void);
void doPrint(operando);
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

/* Prototipos das funcoes para o interpretador */

void InterpCodIntermed (void);
void AlocaVariaveis (simbolo);
void AtribuiParametros(int, simbolo);
modhead encontrarMod(simbolo);
void ExecQuadWrite (quadrupla);
void ExecQuadMais (quadrupla);
void ExecQuadMenos (quadrupla);
void ExecQuadVezes (quadrupla);
void ExecQuadDiv (quadrupla);
void ExecQuadLT (quadrupla);
void ExecQuadLE (quadrupla);
void ExecQuadGT (quadrupla);
void ExecQuadGE (quadrupla);
void ExecQuadOR (quadrupla);
void ExecQuadAND (quadrupla);
void ExecQuadEQ (quadrupla);
void ExecQuadNE (quadrupla);
void ExecQuadAtrib (quadrupla);
void ExecQuadRead (quadrupla);
void ExecQuadResto(quadrupla);
void ExecQuadMenun(quadrupla);
void ExecQuadNot(quadrupla);
void ExecQuadInd(quadrupla);
void ExecQuadIndex(quadrupla);
void ExecQuadAtribPont(quadrupla);
void ExecQuadContaPont(quadrupla);
quadrupla ExecQuadCall(quadrupla, int*);
quadrupla ExecQuadReturn(quadrupla);
void VerificarTipoEValorTotal(operando op, int*, int*, float*, char*, char*);

/*	Declaracoes para pilhas de operandos  */

typedef struct nohopnd nohopnd;
struct nohopnd {
	operando opnd;
	nohopnd *prox;
};
typedef nohopnd *pilhaoperando;
pilhaoperando pilhaopnd;
pilhaoperando pilharetornos;

/*  Prototipos das funcoes para pilhas de operandos  */

void EmpilharOpnd (operando, pilhaoperando *);
void DesempilharOpnd (pilhaoperando *);
operando TopoOpnd (pilhaoperando);
void InicPilhaOpnd (pilhaoperando *);
char VaziaOpnd (pilhaoperando);

/*	Declaracoes para pilhas de quadruplas/chamadas  */

typedef struct nohquad nohquad;
struct nohquad {
	quadrupla quad;
	nohquad *prox;
};
typedef nohquad *pilhaquadrupla;
pilhaquadrupla pilhaquad;

/*  Prototipos das funcoes para pilhas de chamadas  */

void EmpilharQuad (quadrupla, pilhaquadrupla *);
void DesempilharQuad (pilhaquadrupla *);
quadrupla TopoQuad (pilhaquadrupla);
void InicPilhaQuad (pilhaquadrupla *);
char VaziaQuad (pilhaquadrupla);

/*	Declaracoes para pilhas de indices  */

typedef struct nohind nohind;
struct nohind {
	int ind;
	nohind *prox;
};
typedef nohind *pilhaindice;
pilhaindice pilhaind;

/*  Prototipos das funcoes para pilhas de chamadas  */

void EmpilharInd (int, pilhaindice *);
void DesempilharInd (pilhaindice *);
int TopoInd (pilhaindice);
void InicPilhaInd (pilhaindice *);
char VaziaInd (pilhaindice);

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
				{
				escopoatual = InsereSimb ($3, IDPROG, NOTVAR, NULL);
				InicCodIntermMod (escopoatual);
          		escopoglobal=escopoatual;} 
          		Decls    SubProgs
          		{
          			numquadcorrente = 0;
          			modcorrente = codintermed->prox;
          			quadcorrente = modcorrente->listquad;
          			opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			$<quad>$ = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);
          		}
          		CmdComp
          		{
          			$<quad>$ = GeraQuadrupla (OPEXIT, opndidle, opndidle, opndidle);
          			VerificaInicRef();
          			InterpCodIntermed ();}

Decls 		:	   |   VAR {tab++;}  ListDecl {tab--;}
				
ListDecl	:	Declaracao    |   ListDecl   Declaracao

Declaracao	: 	Tipo { }   ListElemDecl   PVIRG {}

Tipo		: 	INT {strcpy($$, "int"); tipocorrente = INTEGER;}    
			|     REAL {strcpy($$, "real"); tipocorrente = FLOAT;}   
			|    CARAC {strcpy($$, "carac"); tipocorrente = CHAR;}    
			|     LOGIC {strcpy($$, "logic"); tipocorrente = LOGICAL;}
ListElemDecl	: 	ElemDecl      |     ListElemDecl   VIRG {}   ElemDecl

ElemDecl  	:	ID
				{
				AnalisarInsercaoSimb($1, IDVAR, FALSE, escopoatual);}
				|    ID   ABCOL
				{
				AnalisarInsercaoSimb($1, IDVAR, TRUE, escopoatual);}  ListDim   FCOL  {}  

ListDim	: 	CTINT {ConferirValorDim($1);}   
			|	ListDim   VIRG   CTINT { ConferirValorDim($3);}

SubProgs	:    	   |   SubProgs   DeclSubProg

DeclSubProg	:   	{temretorno = FALSE;}
					Cabecalho   Decls   CmdComp
					{
					if(!temretorno){
						if(escopoatual->tid == IDPROC){
							$<quad>$ = GeraQuadrupla (OPRETURN, opndidle, opndidle, opndidle);
						}else{
							Esperado("Eh esperado um retorno em funcoes");
						}
					}
					escopoatual = escopoglobal;}

Cabecalho	:   	CabFunc   |   CabProc

CabFunc	:   	FUNCAO Tipo   ID 	ABPAR  FPAR  PVIRG
				{
					AnalisarInsercaoSimb ($3, IDFUNC, FALSE, escopoglobal);
					escopoatual=simb;
					InicCodIntermMod (escopoatual);
					opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			$<quad>$ = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);}
		|		FUNCAO Tipo   ID ABPAR
				{
					AnalisarInsercaoSimb ($3, IDFUNC, TRUE, escopoglobal);
					escopoatual=simb;
					InicCodIntermMod (escopoatual);
					opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			$<quad>$ = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);} ListParam  FPAR PVIRG

CabProc	:   	PROCEDIMENTO   ID   ABPAR   FPAR   PVIRG
				{
					AnalisarInsercaoSimb ($2, IDPROC, FALSE, escopoglobal);
					escopoatual=simb;
					InicCodIntermMod (escopoatual);
					opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			$<quad>$ = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);}
		|	PROCEDIMENTO   ID   ABPAR
			{
			AnalisarInsercaoSimb ($2, IDPROC, TRUE, escopoglobal);
			escopoatual=simb;
			InicCodIntermMod (escopoatual);
			opnd1.tipo = MODOPND;
      		opnd1.atr.modulo = modcorrente;
      		$<quad>$ = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);} ListParam   FPAR   PVIRG 

ListParam	:   	Parametro   |   ListParam   VIRG {}   Parametro

Parametro	:   	Tipo   ID
					{AnalisarInsercaoSimb($2, IDVAR, FALSE, escopoatual);
						$$ = simb;
						$$->inic = TRUE;
						
						ConferirValorParam(tipocorrente, $$);}

CmdComp	:     	ABCHAV
				{
					
					tab++;} ListCmd   FCHAV
				{tab--;
					
					}

ListCmd	:   	   |   ListCmd   Comando

Comando  	:   	CmdComp   |   CmdSe   |   CmdEnquanto   |   CmdRepetir   |   CmdPara   
|   	CmdLer   |   CmdEscrever   |   CmdAtrib   |   ChamaProc   |   CmdRetornar   
|   	PVIRG

CmdSe		:   	SE   ABPAR
						{
						tab++;
						}
					Expressao
					{ConferirExpLogica($4.tipo);
						opndaux.tipo = ROTOPND;
						$<quad>$ = GeraQuadrupla (OPJF, $4.opnd, opndidle, opndaux);
					}
					FPAR {}   Comando
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
							{
							tab++;
							
							opndaux.tipo = ROTOPND;
							$<quad>$ = GeraQuadrupla (OPJUMP, opndidle, opndidle,opndaux);} 
							Comando
							{tab--;
							$<quad>2->result.atr.rotulo =GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}

CmdEnquanto	:   	ENQUANTO   ABPAR
						{
						
						$<quad>$ = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);}
					Expressao
					{ConferirExpLogica($4.tipo);
					opndaux.tipo = ROTOPND;
					$<quad>$ = GeraQuadrupla(OPJF, $4.opnd, opndidle, opndaux);}
					FPAR
					{tab++;}
					Comando
					{tab--;
					$<quad>$ = quadcorrente;
					opndaux.tipo = ROTOPND;
					$<quad>$ = GeraQuadrupla (OPJUMP, opndidle, opndidle, opndaux);
					$<quad>$->result.atr.rotulo = $<quad>3;
					$<quad>5->result.atr.rotulo = GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}

CmdRepetir	:   	REPETIR
					{
						
						tab++;
						$<quad>$ = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);}
					Comando
					ENQUANTO
					{tab--;   }
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
						}   FPAR   PVIRG

CmdPara	:   	PARA
				{
					tab++;
					}
				ABPAR Variavel
				{
					ConferirPara($4.simb);
					InicializarEReferenciar($4.simb);}
				ATRIB  Expressao
				{ConferirAtribuicao($4.simb, $7.tipo);
				 GeraQuadrupla (OPATRIB, $7.opnd, opndidle, $4.opnd);}
				PVIRG
				{
				$<quad>$ = GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}
				Expressao   PVIRG
				{
					ConferirExpLogica($11.tipo);
					opndaux.tipo = ROTOPND;
					$<quad>$ = GeraQuadrupla(OPJF, $11.opnd, opndidle, opndaux);}
				Variavel   ATRIB
				{
					MesmaVariavel($4.simb,$14.simb);
					$<quad>$ = quadcorrente;}
				Expressao
				FPAR
				{
				$<quad>$ = quadcorrente;}
				Comando
				{tab--;
					quadaux = $<quad>13->prox;
					$<quad>13->prox = $<quad>19->prox;
					quadcorrente->prox = quadaux;
					quadcorrente = $<quad>19;
					quadcorrente->prox = NULL;
					RenumQuadruplas($<quad>13, quadcorrente->prox);
					$<quad>$ = GeraQuadrupla (OPATRIB, $17.opnd, opndidle, $14.opnd);
					opndaux.tipo = ROTOPND;
					quadaux = GeraQuadrupla (OPJUMP, opndidle, opndidle, opndaux);
					quadaux->result.atr.rotulo = $<quad>10;
					quadaux = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);
					$<quad>13->result.atr.rotulo = quadaux;}

CmdLer	:   	LER   ABPAR  {} ListVar   FPAR   PVIRG
				{
					opnd1.tipo = INTOPND;
					opnd1.atr.valint = $4;
					GeraQuadrupla (OPREAD, opnd1, opndidle, opndidle);}

ListVar	:   	Variavel {InicializarEReferenciar($1.simb);
							$$ = 1;
							GeraQuadrupla (PARAM, $1.opnd, opndidle, opndidle);}
				|   ListVar   VIRG {}   Variavel
				{InicializarEReferenciar($4.simb);
					$$ = $1 + 1;
					GeraQuadrupla (PARAM, $4.opnd, opndidle, opndidle);}

CmdEscrever	:   	ESCREVER   ABPAR {}  ListEscr   FPAR   PVIRG
					{
						opnd1.tipo = INTOPND;
						opnd1.atr.valint = $4;
						GeraQuadrupla (OPWRITE, opnd1, opndidle, opndidle);}

ListEscr	:   	ElemEscr
					{
						$$ = 1;
						GeraQuadrupla (PARAM, $1.opnd, opndidle, opndidle);
					}
					|   ListEscr   VIRG {}   ElemEscr {
							$$ = $1 + 1;
							GeraQuadrupla (PARAM, $4.opnd, opndidle, opndidle);
						}

ElemEscr	:   	CADEIA {
						$$.opnd.tipo = CADOPND;
						$$.opnd.atr.valcad = malloc (strlen($1) + 1);
						strcpy ($$.opnd.atr.valcad, $1);}
					|   Expressao
ChamaProc	:   	CHAMAR ID ABPAR  FPAR  PVIRG
					{
						VerificarExistencia($2);
						VerificarTamanhoSemParametros(simb);
						VerificarTipo(simb, IDPROC);
						VerificarRecursividade(escopoatual,simb);
						opndaux.tipo = INTOPND;
						opndaux.atr.valint = 0;
						opndaux2.tipo = FUNCOPND;
						opndaux2.atr.simb = simb;
						GeraQuadrupla  (OPCALL, opndaux2, opndaux, opndidle);}
					|	CHAMAR   ID ABPAR
					{
						
							VerificarExistencia($2);
							ultimachamada=simb;
							$<simb>$ = simb;
							VerificarRecursividade(escopoatual,simb);}  ListExpr   FPAR   PVIRG
					{
						VerificarTamanho($<simb>4,$5);
						VerificarTipo($<simb>4, IDPROC);
						opndaux.tipo = INTOPND;
						opndaux.atr.valint = $<simb>4->ndims;
						opndaux2.tipo = FUNCOPND;
						opndaux2.atr.simb = $<simb>4;
						GeraQuadrupla  (OPCALL, opndaux2, opndaux, opndidle);}
CmdRetornar	:   	RETORNAR   PVIRG
					{
						temretorno = TRUE;
						ConferirRetornoProcedimento(escopoatual);
						GeraQuadrupla  (OPRETURN, opndidle, opndidle, opndidle);} 
					|   RETORNAR
					{
						}  Expressao
					{ConferirRetornoFuncao(escopoatual,$3.tipo);
					temretorno = TRUE;} PVIRG
					{
					GeraQuadrupla  (OPRETURN, $3.opnd, opndidle, opndidle);}
CmdAtrib  	:   	{} Variavel
					{if  ($2.simb != NULL)
						if($2.simb->ponteiro){
							$2.simb->referente->inic = $2.simb->referente->ref = TRUE;
						}else{
							$2.simb->inic = $2.simb->ref = TRUE;
						}
							}  ATRIB  Expressao   PVIRG 
							{
							ConferirAtribuicao($2.simb, $5.tipo);
							if(!$2.simb->ponteiro){
								GeraQuadrupla (OPATRIB, $5.opnd, opndidle, $2.opnd);
							}else{
								GeraQuadrupla (OPATRIBPONT, $5.opnd, opndidle, $2.opnd);
							}
}
ListExpr	:   	Expressao
					{$$ = 1;
						checarTipo(1,$1.tipo);
						GeraQuadrupla (PARAM, $1.opnd, opndidle, opndidle);}
					|   ListExpr   VIRG {}  Expressao
					{$$ = $$ + 1;
						checarTipo($$,$4.tipo);
						GeraQuadrupla (PARAM, $4.opnd, opndidle, opndidle);}
Expressao  	:   	ExprAux1     |     Expressao   OR {} ExprAux1 {ConferirOr($1, $4, &$$);}
ExprAux1  	:   	ExprAux2     |     ExprAux1   AND {} ExprAux2 {ConferirAnd($1, $4, &$$);}
ExprAux2  	:   	ExprAux3     |     NOT {}   ExprAux3  {ConferirNot($3, &$$);}
ExprAux3  	:   	ExprAux4     |     ExprAux4   OPREL {switch($2){
									case MAIOR: break;
									case MENOR: break;
									case MAIG: break;
									case MENIG: break;
									case IGUAL: break;
									case DIFER: break;
								}
								}  ExprAux4 {ConferirOpRel($1, $2, $4, &$$);}
ExprAux4  	:   	Termo     
			|     ExprAux4   OPAD {switch($2){
									case MAIS: break;
									case MENOS: break;
								}
								}   Termo {ConferirOpAd($1, $2, $4, &$$);}
Termo  	:   	Fator   
			|   Termo   OPMULT {switch($2){
									case VEZES: break;
									case DIV: break;
									case REST: break;
								}
								}   Fator {ConferirOpMult($1, $2, $4, &$$);}
Fator		:   	Variavel  {
						if  ($1.simb != NULL)  {
               				$1.simb->ref  =  TRUE;
                  			$$.tipo = $1.simb->tvar;
                  			$$.opnd = $1.opnd;
                  			if($1.simb->ponteiro){
                  				$$.tipo = VAROPND;
                  				$$.opnd.atr.simb = NovaTemp($1.simb->tvar);
                  				opndaux.tipo = VAROPND;
                  				opndaux.atr.simb = $1.simb;
                  				GeraQuadrupla(OPCONTAPONT,opndaux,opndidle,$$.opnd);
                  			}
               			}
				}
			|   CTINT {
						$$.tipo = INTEGER;
						$$.opnd.tipo = INTOPND;
						$$.opnd.atr.valint = $1;}
			|   CTREAL {
						$$.tipo = FLOAT;
						$$.opnd.tipo = REALOPND;
						$$.opnd.atr.valfloat = $1;}  
			|   CTCARAC   {
							$$.tipo = CHAR;
							$$.opnd.tipo = CHAROPND;
							$$.opnd.atr.valchar = $1;}
			|   VERDADE   {
							$$.tipo = LOGICAL;
							$$.opnd.tipo = LOGICOPND;
							$$.opnd.atr.vallogic = 1;}
			|   FALSO   {
							$$.tipo = LOGICAL;
							$$.opnd.tipo = LOGICOPND;
							$$.opnd.atr.vallogic = 0;}
			|   NEG {}   Fator {
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
			|   ABPAR {}  Expressao   FPAR
					{
					$$.tipo = $3.tipo;
					$$.opnd = $3.opnd;} 
			|   ChamaFunc
				{$$.tipo = $1.tipo;
				$$.opnd.tipo = VAROPND;
				$$.opnd.atr.simb = NovaTemp ($$.tipo);
				opndaux.tipo = INTOPND;
				opndaux.atr.valint = $1.opnd.atr.simb->ndims;
				GeraQuadrupla  (OPCALL, $1.opnd, opndaux, $$.opnd);}
Variavel	:   	ID {
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
                  		$$.opnd.atr.simb = $$.simb;
	         			$<simb>$ = simb;
					}   ListSubscr   FCOL
					{
					$$.simb = $<simb>3;
					$$.opnd.tipo = VAROPND;
					$$.opnd.atr.simb = NovaTemp ($$.simb->tvar);
					$$.opnd.atr.simb->ponteiro = TRUE;
					$$.opnd.atr.simb->referente = $<simb>3;
					if($<simb>3)
						$<simb>3->ref = TRUE;
					opndaux.tipo = VAROPND;
					opndaux.atr.simb = $$.simb;
					opndaux2.tipo = INTOPND;
					opndaux2.atr.valint = $$.simb->ndims;
					GeraQuadrupla(INDEXOP,opndaux,opndaux2,$$.opnd);
					if ($$.simb != NULL)
						if ($$.simb->array == FALSE)
							NaoEsperado ("Subscrito\(s)");
			                 	 else if ($$.simb->ndims != $4)
			 				Incompatibilidade ("Numero de subscritos incompativel com declaracao");
			 		$$.simb = $$.opnd.atr.simb;}
ListSubscr	:	ExprAux4
				{ConferirSubscr($1.tipo);
					$$ =1;
					GeraQuadrupla (INDOP, $1.opnd, opndidle, opndidle);}
				| ListSubscr   VIRG
				{}   ExprAux4
				{ConferirSubscr($4.tipo);
					$$ =$1+1;
					GeraQuadrupla (INDOP, $4.opnd, opndidle, opndidle);}
ChamaFunc	:   ID   ABPAR    FPAR
				{
					VerificarExistencia($1);
					VerificarTamanhoSemParametros(simb);
					VerificarTipo(simb, IDFUNC);
					$<simb>$ = simb;
					if(simb){
						$$.tipo=simb->tvar;
					}
					$$.opnd.tipo = FUNCOPND;
					$$.opnd.atr.simb = simb;
					VerificarRecursividade(escopoatual,simb);}
			|	ID ABPAR
			{
				VerificarExistencia($1);
				ultimachamada=simb;
				$<simb>$ = simb;} ListExpr   FPAR
			{
				VerificarTamanho($<simb>3,$4);
				VerificarTipo($<simb>3, IDFUNC);
				VerificarRecursividade(escopoatual,$<simb>3);
				if($<simb>3)
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
	pointer->tipo = LOGICAL;
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
}

void ConferirNot(infoexpressao p, infoexpressao *pointer){
	if(p.tipo!=LOGICAL){
		Incompatibilidade("Operando improprio para operador de negacao");
	}
	pointer->tipo = LOGICAL;
	pointer->opnd.tipo = VAROPND;  
	pointer->opnd.atr.simb = NovaTemp (pointer->tipo);
	GeraQuadrupla (OPNOT, p.opnd, opndidle, pointer->opnd);
}

void ConferirAnd(infoexpressao p, infoexpressao q, infoexpressao *pointer){
	if(p.tipo!=LOGICAL||q.tipo!=LOGICAL){
		Incompatibilidade("Operando improprio para operador de AND");
	}
	pointer->tipo = LOGICAL;
	pointer->opnd.tipo = VAROPND;  
	pointer->opnd.atr.simb = NovaTemp (pointer->tipo);
	GeraQuadrupla (OPAND, p.opnd, q.opnd, pointer->opnd);
}

void ConferirOr(infoexpressao p, infoexpressao q, infoexpressao *pointer){
	if(p.tipo!=LOGICAL||q.tipo!=LOGICAL){
		Incompatibilidade("Operando improprio para operador de OR");
	}
	pointer->tipo = LOGICAL;
	pointer->opnd.tipo = VAROPND;  
	pointer->opnd.atr.simb = NovaTemp (pointer->tipo);
	GeraQuadrupla (OPOR, p.opnd, q.opnd, pointer->opnd);
}

void ConferirAtribuicao(simbolo p, int q){
	if (p != NULL){
		if (((p->tvar == INTEGER || p->tvar == CHAR) && (q == FLOAT || q == LOGICAL)) || (p->tvar == FLOAT && q == LOGICAL) || (p->tvar == LOGICAL && q != LOGICAL)){
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
		return;
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

void ConferirValorParam(int p, simbolo s){
    escopoatual->ndims++;
    escopoatual->dims[escopoatual->ndims] = p;
    escopoatual->listaparametros[escopoatual->ndims] = s;
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
	if(!s1 || !s2)
		return;
	if(strcmp(s1->cadeia,s2->cadeia))
		Incompatibilidade ("Variavel atualizada nao eh a mesma inicializada");
}

void VerificarTipo(simbolo s1, int tipo){
	if(!s1)
		return;
	if(s1->tid!=tipo){
		TipoInadequado(s1->cadeia);
	}
}

void VerificarRecursividade(simbolo escopoatual,simbolo simb){
	if(simb == NULL || escopoatual == NULL){
		return;
	}
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
				case VAROPND:
					printf (", %s", o.atr.simb->cadeia);
					break;
				case INTOPND: printf (", %d", o.atr.valint); break;
				case REALOPND: printf (", %g", o.atr.valfloat); break;
				case CHAROPND: printf (", %c", o.atr.valchar); break;
				case LOGICOPND:
					printf (", %d", o.atr.vallogic);
					break;
				case CADOPND: printf (", %s", o.atr.valcad); break;
				case ROTOPND:
					printf (", %d", o.atr.rotulo->num);
					break;
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

void InterpCodIntermed () {
	quadrupla quad, quadprox;
	char encerra;
	char condicao;
	simbolo chamadaatual;
	int parametros = 0;
	finput = fopen ("entrada", "r");
	printf ("\n\nINTERPRETADOR:\n");
	InicPilhaOpnd (&pilhaopnd);
	InicPilhaQuad (&pilhaquad);
	InicPilhaQuad (&pilharetornos);
	InicPilhaInd(&pilhaind);
	encerra = FALSE;
	quad = codintermed->prox->listquad->prox;
	while (! encerra) {
		//printf ("\n%4d) %s\n", quad->num,nomeoperquad[quad->oper]);
		quadprox = quad->prox;
		switch (quad->oper) {
			case OPEXIT: encerra = TRUE; break;
			case OPENMOD:
				AlocaVariaveis (quad->opnd1.atr.modulo->modname);
				AtribuiParametros(parametros, chamadaatual);
				break;
			case PARAM: EmpilharOpnd (quad->opnd1, &pilhaopnd);  break;
			case OPWRITE:   ExecQuadWrite (quad);  break;
			case OPMAIS:  ExecQuadMais (quad);  break;
			case OPMENOS:  ExecQuadMenos (quad);  break;
			case OPMULTIP:  ExecQuadVezes (quad);  break;
			case OPDIV:  ExecQuadDiv (quad);  break;
			case OPATRIB:  ExecQuadAtrib (quad);  break;
			case OPJUMP:
				quadprox = quad->result.atr.rotulo;
				break;
			case OPJF:
				if (quad->opnd1.tipo == LOGICOPND)
					condicao = quad->opnd1.atr.vallogic;
				if (quad->opnd1.tipo == VAROPND)
					condicao = *(quad->opnd1.atr.simb->vallogic);
				if (! condicao){
					quadprox = quad->result.atr.rotulo;
				}
				break;
			case OPLT:  ExecQuadLT (quad); break;
			case OPLE:  ExecQuadLE (quad); break;
			case OPGT:  ExecQuadGT (quad); break;
			case OPGE:  ExecQuadGE (quad); break;
			case OPOR:	ExecQuadOR (quad); break;
			case OPAND:	ExecQuadAND (quad); break;
			case OPEQ:	ExecQuadEQ (quad); break;
			case OPNE:	ExecQuadNE (quad); break;
			case OPRESTO: ExecQuadResto(quad); break;
			case OPMENUN: ExecQuadMenun(quad); break;
			case OPNOT: ExecQuadNot(quad); break;
			case OPREAD:   ExecQuadRead (quad);  break;
			case OPCALL:
				quadprox = ExecQuadCall(quad, &parametros);
				chamadaatual = quad->opnd1.atr.simb;
				break;
			case OPRETURN: quadprox = ExecQuadReturn(quad); break;
			case INDOP: ExecQuadInd(quad); break;
			case INDEXOP: ExecQuadIndex(quad); break;
			case OPCONTAPONT: ExecQuadContaPont(quad);break;
			case OPATRIBPONT: ExecQuadAtribPont(quad); break;
		}
		if (! encerra) quad = quadprox;
	}
	printf ("\n");                 
}

void AlocaVariaveis (simbolo escopo) {
	simbolo s; int nelemaloc, i, j;
	//printf ("\n\t\tAlocando as variaveis de %s:", escopo->cadeia);
	for (i = 0; i < NCLASSHASH; i++)
	       if (tabsimb[i]) {
	              for (s = tabsimb[i]; s != NULL; s = s->prox){
	                     if (s->tid == IDVAR && !strcmp(s->escopo->cadeia,escopo->cadeia)) {
	                            nelemaloc = 1;
	                            if (s->array)
	                                   for (j = 1; j <= s->ndims; j++)
	                                   		nelemaloc *= s->dims[j];
	                            switch (s->tvar) {
	                                   case INTEGER:
	                                          s->valint = malloc (nelemaloc * sizeof (int)); break;
	                                   case FLOAT:
	                                          s->valfloat = malloc (nelemaloc * sizeof (float)); break;
	                                   case CHAR:
	                                          s->valchar = malloc (nelemaloc * sizeof (char)); break;
	                                   case LOGICAL:
	                                          s->vallogic = malloc (nelemaloc * sizeof (char)); break;
	                           }
	                           //printf ("\n\t\t\t%s: %d elemento(s) alocado(s) ", s->cadeia, nelemaloc);
	                   }
	      	 }
	      }
}

void DesalocaVariaveis(simbolo escopo){
	simbolo s; int i, j;
	//printf ("\n\t\tDesalocando as variaveis de %s:", escopo->cadeia);
	for (i = 0; i < NCLASSHASH; i++)
	       if (tabsimb[i]) {
	              for (s = tabsimb[i]; s != NULL; s = s->prox){
	                     if (s->tid == IDVAR && !strcmp(s->escopo->cadeia,escopo->cadeia)) {
	                            switch (s->tvar) {
	                                   case INTEGER:
	                                          free(s->valint); break;
	                                   case FLOAT:
	                                          free(s->valfloat); break;
	                                   case CHAR:
	                                          free(s->valchar); break;
	                                   case LOGICAL:
	                                          free(s->vallogic); break;
	                           }
	                           //printf ("\n\t\t\t%s: Elemento(s) desalocado(s) ", s->cadeia);
	                   }
	      	 }
	      }
}

void EmpilharOpnd (operando x, pilhaoperando *P) {
	nohopnd *temp;
	temp = *P;   
	*P = (nohopnd *) malloc (sizeof (nohopnd));
	(*P)->opnd = x; (*P)->prox = temp;
}

void DesempilharOpnd (pilhaoperando *P) {
	nohopnd *temp;
	if (! VaziaOpnd (*P)) {
		temp = *P;  *P = (*P)->prox; free (temp);
	}
	else  printf ("\n\tDelecao em pilha vazia\n");
}

operando TopoOpnd (pilhaoperando P) {
	if (! VaziaOpnd (P))  return P->opnd;
	else  printf ("\n\tTopo de pilha vazia\n");
}

void InicPilhaOpnd (pilhaoperando *P) { 
	*P = NULL;
}

char VaziaOpnd (pilhaoperando P) {
	if  (P == NULL)  return 1;  
	else return 0; 
}

void ExecQuadWrite (quadrupla quad) {
	int i;  operando opndaux;  pilhaoperando pilhaopndaux;

	//printf ("\n\t\tEscrevendo: \n\n");
	InicPilhaOpnd (&pilhaopndaux);
	for (i = 1; i <= quad->opnd1.atr.valint; i++) {
		EmpilharOpnd (TopoOpnd (pilhaopnd), &pilhaopndaux);
		DesempilharOpnd (&pilhaopnd);
	}
	for (i = 1; i <= quad->opnd1.atr.valint; i++) {
		opndaux = TopoOpnd (pilhaopndaux);
		DesempilharOpnd (&pilhaopndaux);
		switch (opndaux.tipo) {
			case INTOPND:
				printf ("%d", opndaux.atr.valint); break;
			case REALOPND:
				printf ("%g", opndaux.atr.valfloat); break;
			case CHAROPND:
				printf ("%c", opndaux.atr.valchar); break;
			case LOGICOPND:
				if (opndaux.atr.vallogic == 1) printf ("VERDADE");
				else printf ("FALSO");
				break;
			case CADOPND:
				for(j=1;opndaux.atr.valcad[j]!='"';j++){

				}
				printf ("%.*s", j-1, opndaux.atr.valcad+1); 
				break ;
			case VAROPND:
			switch (opndaux.atr.simb->tvar) {
				case INTEGER:
					printf ("%d", *(opndaux.atr.simb->valint));break;
				case FLOAT:
					printf ("%g", 
					     *(opndaux.atr.simb->valfloat));break;
				case LOGICAL:
					if (*(opndaux.atr.simb->vallogic) == 1)
						printf ("VERDADE"); 
						else printf ("FALSO"); break;
				case CHAR:
					printf ("%c", 
					    *(opndaux.atr.simb->valchar)); break;
			}
			break;
		}
	}
	printf ("\n");
}

void determinarTipo(operando orig, int *novot, int *valint, float *valfloat){
	switch (orig.tipo) {
		case INTOPND:
			*novot = INTOPND;  *valint = orig.atr.valint;  break;
		case REALOPND:
			*novot = REALOPND;  *valfloat = orig.atr.valfloat; break;
		case CHAROPND:
			*novot = INTOPND;  *valint = orig.atr.valchar;  break;
		case VAROPND:
			switch (orig.atr.simb->tvar) {
				case INTEGER:
					*novot = INTOPND;
					*valint = *(orig.atr.simb->valint);  break;
				case FLOAT:
					*novot = REALOPND;
					*valfloat=*(orig.atr.simb->valfloat);break;
				case CHAR:
					*novot = INTOPND;
					*valint = *(orig.atr.simb->valchar); break;
			}
			break;
	}
}

void ExecQuadOR(quadrupla quad){
	char vallogic1, vallogic2;
	switch(quad->opnd1.tipo){
		case LOGICOPND:
			vallogic1 = quad->opnd1.atr.vallogic;
			break;
		case VAROPND:
			vallogic1 = *(quad->opnd1.atr.simb->vallogic);
			break;
	}
	switch(quad->opnd2.tipo){
		case LOGICOPND:
			vallogic2 = quad->opnd2.atr.vallogic;
			break;
		case VAROPND:
			vallogic2 = *(quad->opnd2.atr.simb->vallogic);
			break;
	}
	*(quad->result.atr.simb->vallogic) = vallogic1 || vallogic2;
}

void ExecQuadAND(quadrupla quad){
	char vallogic1, vallogic2;
	switch(quad->opnd1.tipo){
		case LOGICOPND:
			vallogic1 = quad->opnd1.atr.vallogic;
			break;
		case VAROPND:
			vallogic1 = *(quad->opnd1.atr.simb->vallogic);
			break;
	}
	switch(quad->opnd2.tipo){
		case LOGICOPND:
			vallogic2 = quad->opnd2.atr.vallogic;
			break;
		case VAROPND:
			vallogic2 = *(quad->opnd2.atr.simb->vallogic);
			break;
	}
	*(quad->result.atr.simb->vallogic) = vallogic1 && vallogic2;
}

void ExecQuadMais (quadrupla quad) {
	int tipo1, tipo2, valint1, valint2;
	float valfloat1, valfloat2;
	determinarTipo(quad->opnd1,&tipo1,&valint1,&valfloat1);
	determinarTipo(quad->opnd2,&tipo2,&valint2,&valfloat2);
	switch (quad->result.atr.simb->tvar) {
		case INTEGER:
			*(quad->result.atr.simb->valint) = valint1 + valint2;
			break;
		case FLOAT:
			if (tipo1 == INTOPND && tipo2 == INTOPND)
				*(quad->result.atr.simb->valfloat) = valint1 + valint2;
			if (tipo1 == INTOPND && tipo2 == REALOPND)
				*(quad->result.atr.simb->valfloat) = valint1 + valfloat2;
			if (tipo1 == REALOPND && tipo2 == INTOPND)
				*(quad->result.atr.simb->valfloat) = valfloat1 + valint2;
			if (tipo1 == REALOPND && tipo2 == REALOPND)
				*(quad->result.atr.simb->valfloat) = valfloat1 + valfloat2;
			break;
	}
}

void ExecQuadMenos (quadrupla quad) {
	int tipo1, tipo2, valint1, valint2;
	float valfloat1, valfloat2;
	determinarTipo(quad->opnd1,&tipo1,&valint1,&valfloat1);
	determinarTipo(quad->opnd2,&tipo2,&valint2,&valfloat2);
	switch (quad->result.atr.simb->tvar) {
		case INTEGER:
			*(quad->result.atr.simb->valint) = valint1 - valint2;
			break;
		case FLOAT:
			if (tipo1 == INTOPND && tipo2 == INTOPND)
				*(quad->result.atr.simb->valfloat) = valint1 - valint2;
			if (tipo1 == INTOPND && tipo2 == REALOPND)
				*(quad->result.atr.simb->valfloat) = valint1 - valfloat2;
			if (tipo1 == REALOPND && tipo2 == INTOPND)
				*(quad->result.atr.simb->valfloat) = valfloat1 - valint2;
			if (tipo1 == REALOPND && tipo2 == REALOPND)
				*(quad->result.atr.simb->valfloat) = valfloat1 - valfloat2;
			break;
	}
}

void ExecQuadVezes (quadrupla quad) {
	int tipo1, tipo2, valint1, valint2;
	float valfloat1, valfloat2;
	determinarTipo(quad->opnd1,&tipo1,&valint1,&valfloat1);
	determinarTipo(quad->opnd2,&tipo2,&valint2,&valfloat2);
	switch (quad->result.atr.simb->tvar) {
		case INTEGER:
			*(quad->result.atr.simb->valint) = valint1 * valint2;
			break;
		case FLOAT:
			if (tipo1 == INTOPND && tipo2 == INTOPND)
				*(quad->result.atr.simb->valfloat) = valint1 * valint2;
			if (tipo1 == INTOPND && tipo2 == REALOPND)
				*(quad->result.atr.simb->valfloat) = valint1 * valfloat2;
			if (tipo1 == REALOPND && tipo2 == INTOPND)
				*(quad->result.atr.simb->valfloat) = valfloat1 * valint2;
			if (tipo1 == REALOPND && tipo2 == REALOPND)
				*(quad->result.atr.simb->valfloat) = valfloat1 * valfloat2;
			break;
	}
}

void ExecQuadDiv (quadrupla quad) {
	int tipo1, tipo2, valint1, valint2;
	float valfloat1, valfloat2;
	determinarTipo(quad->opnd1,&tipo1,&valint1,&valfloat1);
	determinarTipo(quad->opnd2,&tipo2,&valint2,&valfloat2);
	switch (quad->result.atr.simb->tvar) {
		case INTEGER:
			*(quad->result.atr.simb->valint) = valint1 / valint2;
			break;
		case FLOAT:
			if (tipo1 == INTOPND && tipo2 == INTOPND)
				*(quad->result.atr.simb->valfloat) = valint1 / valint2;
			if (tipo1 == INTOPND && tipo2 == REALOPND)
				*(quad->result.atr.simb->valfloat) = valint1 / valfloat2;
			if (tipo1 == REALOPND && tipo2 == INTOPND)
				*(quad->result.atr.simb->valfloat) = valfloat1 / valint2;
			if (tipo1 == REALOPND && tipo2 == REALOPND)
				*(quad->result.atr.simb->valfloat) = valfloat1 / valfloat2;
			break;
	}
}

void ExecQuadEQ(quadrupla quad){
	int tipo1, valint1;
	float valfloat1;
	char valchar1, vallogic1;
	int tipo2, valint2;
	float valfloat2;
	char valchar2, vallogic2;
	char reslogic;
	VerificarTipoEValorTotal(quad->opnd1, &tipo1, &valint1, &valfloat1, &valchar1, &vallogic1);
	VerificarTipoEValorTotal(quad->opnd2, &tipo2, &valint2, &valfloat2, &valchar2, &vallogic2);
	switch(tipo1){
		case INTOPND:
			switch(tipo2){
				case INTOPND:
					reslogic = valint1 == valint2;
					break;
				case REALOPND:
					reslogic = valint1 == valfloat2;
					break;
				case CHAROPND:
					reslogic = valint1 == valchar2;
					break;
			}
			break;
		case REALOPND:
			switch(tipo2){
				case INTOPND:
					reslogic = valfloat1 == valint2;
					break;
				case REALOPND:
					reslogic = valfloat1 == valfloat2;
					break;
				case CHAROPND:
					reslogic = valfloat1 == valchar2;
					break;
			}
			break;
		case CHAROPND:
			switch(tipo2){
				case INTOPND:
					reslogic = valchar1 == valint2;
					break;
				case REALOPND:
					reslogic = valchar1 == valfloat2;
					break;
				case CHAROPND:
					reslogic = valchar1 == valchar2;
					break;
			}
			break;
		case LOGICOPND:
			reslogic = vallogic1 == vallogic2;
			break;
	}
	*(quad->result.atr.simb->vallogic) = reslogic;
}

void ExecQuadNE(quadrupla quad){
	int tipo1, valint1;
	float valfloat1;
	char valchar1, vallogic1;
	int tipo2, valint2;
	float valfloat2;
	char valchar2, vallogic2;
	char reslogic;
	VerificarTipoEValorTotal(quad->opnd1, &tipo1, &valint1, &valfloat1, &valchar1, &vallogic1);
	VerificarTipoEValorTotal(quad->opnd2, &tipo2, &valint2, &valfloat2, &valchar2, &vallogic2);
	switch(tipo1){
		case INTOPND:
			switch(tipo2){
				case INTOPND:
					reslogic = valint1 != valint2;
					break;
				case REALOPND:
					reslogic = valint1 != valfloat2;
					break;
				case CHAROPND:
					reslogic = valint1 != valchar2;
					break;
			}
			break;
		case REALOPND:
			switch(tipo2){
				case INTOPND:
					reslogic = valfloat1 != valint2;
					break;
				case REALOPND:
					reslogic = valfloat1 != valfloat2;
					break;
				case CHAROPND:
					reslogic = valfloat1 != valchar2;
					break;
			}
			break;
		case CHAROPND:
			switch(tipo2){
				case INTOPND:
					reslogic = valchar1 != valint2;
					break;
				case REALOPND:
					reslogic = valchar1 != valfloat2;
					break;
				case CHAROPND:
					reslogic = valchar1 != valchar2;
					break;
			}
			break;
		case LOGICOPND:
			reslogic = vallogic1 != vallogic2;
			break;
	}
	*(quad->result.atr.simb->vallogic) = reslogic;
}

void VerificarTipoEValorTotal(operando op, int *tipo, int *valint, float *valfloat, char *valchar, char *vallogic){
	switch (op.tipo) {
		case INTOPND:
			*tipo = INTOPND;
			*valint = op.atr.valint; break;
		case REALOPND:
			*tipo = REALOPND;
			*valfloat = op.atr.valfloat; break;
		case CHAROPND:
			*tipo = CHAROPND;
			*valchar = op.atr.valchar; break;
		case LOGICOPND:
			*tipo = LOGICOPND;
			*vallogic = op.atr.vallogic; break;
		case VAROPND:
			switch (op.atr.simb->tvar) {
				case INTEGER:
					*tipo = INTOPND;
					*valint = *(op.atr.simb->valint); break;
				case FLOAT:
					*tipo = REALOPND;
					*valfloat=*(op.atr.simb->valfloat);break;
				case CHAR:
					*tipo = CHAROPND;
					*valchar=*(op.atr.simb->valchar);break;
				case LOGICAL:
					*tipo = LOGICOPND;
					*vallogic = *(op.atr.simb->vallogic);
					break;
			}
			break;
	}
}

void executarAtribuicao(operando *origem, simbolo destino){
	int tipo1, valint1;
	float valfloat1;
	char valchar1, vallogic1;
	VerificarTipoEValorTotal(*origem, &tipo1, &valint1, &valfloat1, &valchar1, &vallogic1);
	switch (destino->tvar) {
		case INTEGER:
			if (tipo1 == INTOPND)  *(destino->valint) = valint1;
			if (tipo1 == CHAROPND)*(destino->valint)=valchar1;
			break;
		case CHAR:
			if (tipo1 == INTOPND) *(destino->valchar) = valint1;
			if (tipo1==CHAROPND)*(destino->valchar)=valchar1;
			break;
		case LOGICAL:  *(destino->vallogic) = vallogic1;
			break;
		case FLOAT:
			if (tipo1 == INTOPND)
				*(destino->valfloat) = valint1;
			if (tipo1 == REALOPND)
				*(destino->valfloat) = valfloat1;
			if (tipo1 == CHAROPND)
				*(destino->valfloat) = valchar1;
			break;
	}
}

void executarAtribuicaoPont(operando *origem, simbolo destino, int pont){
	int tipo1, valint1;
	float valfloat1;
	char valchar1, vallogic1;
	VerificarTipoEValorTotal(*origem, &tipo1, &valint1, &valfloat1, &valchar1, &vallogic1);
	switch (destino->tvar) {
		case INTEGER:
			if (tipo1 == INTOPND)
				destino->valint[pont] = valint1;
			if (tipo1 == CHAROPND)
				destino->valint[pont] = valchar1;
			break;
		case CHAR:
			if (tipo1 == INTOPND)
				destino->valchar[pont] = valint1;
			if (tipo1==CHAROPND)
				destino->valchar[pont]= valchar1;
			break;
		case LOGICAL:
			destino->vallogic[pont] = vallogic1;
			break;
		case FLOAT:
			if (tipo1 == INTOPND)
				destino->valfloat[pont] = valint1;
			if (tipo1 == REALOPND)
				destino->valfloat[pont] = valfloat1;
			if (tipo1 == CHAROPND)
				destino->valfloat[pont] = valchar1;
			break;
	}
}

void ExecQuadAtrib (quadrupla quad) {
	executarAtribuicao(&(quad->opnd1), quad->result.atr.simb);
}

void ExecQuadLT (quadrupla quad) {
	int tipo1, tipo2, valint1, valint2;
	float valfloat1, valfloat2;
	determinarTipo(quad->opnd1,&tipo1,&valint1,&valfloat1);
	determinarTipo(quad->opnd2,&tipo2,&valint2,&valfloat2);
	if (tipo1 == INTOPND && tipo2 == INTOPND){
		*(quad->result.atr.simb->vallogic) = valint1 < valint2;
	}
	if (tipo1 == INTOPND && tipo2 == REALOPND){
		*(quad->result.atr.simb->vallogic) = valint1 < valfloat2;
	}
	if (tipo1 == REALOPND && tipo2 == INTOPND){
		*(quad->result.atr.simb->vallogic) = valfloat1 < valint2;
	}
	if (tipo1 == REALOPND && tipo2 == REALOPND){
		*(quad->result.atr.simb->vallogic) = valfloat1 < valfloat2;
	}
}

void ExecQuadLE (quadrupla quad) {
	int tipo1, tipo2, valint1, valint2;
	float valfloat1, valfloat2;
	determinarTipo(quad->opnd1,&tipo1,&valint1,&valfloat1);
	determinarTipo(quad->opnd2,&tipo2,&valint2,&valfloat2);
	if (tipo1 == INTOPND && tipo2 == INTOPND){
		*(quad->result.atr.simb->vallogic) = valint1 <= valint2;
	}
	if (tipo1 == INTOPND && tipo2 == REALOPND){
		*(quad->result.atr.simb->vallogic) = valint1 <= valfloat2;
	}
	if (tipo1 == REALOPND && tipo2 == INTOPND){
		*(quad->result.atr.simb->vallogic) = valfloat1 <= valint2;
	}
	if (tipo1 == REALOPND && tipo2 == REALOPND){
		*(quad->result.atr.simb->vallogic) = valfloat1 <= valfloat2;
	}
}

void ExecQuadGT (quadrupla quad) {
	int tipo1, tipo2, valint1, valint2;
	float valfloat1, valfloat2;
	determinarTipo(quad->opnd1,&tipo1,&valint1,&valfloat1);
	determinarTipo(quad->opnd2,&tipo2,&valint2,&valfloat2);
	if (tipo1 == INTOPND && tipo2 == INTOPND){
		*(quad->result.atr.simb->vallogic) = valint1 > valint2;
	}
	if (tipo1 == INTOPND && tipo2 == REALOPND){
		*(quad->result.atr.simb->vallogic) = valint1 > valfloat2;
	}
	if (tipo1 == REALOPND && tipo2 == INTOPND){
		*(quad->result.atr.simb->vallogic) = valfloat1 > valint2;
	}
	if (tipo1 == REALOPND && tipo2 == REALOPND){
		*(quad->result.atr.simb->vallogic) = valfloat1 > valfloat2;
	}
}

void ExecQuadGE (quadrupla quad) {
	int tipo1, tipo2, valint1, valint2;
	float valfloat1, valfloat2;
	determinarTipo(quad->opnd1,&tipo1,&valint1,&valfloat1);
	determinarTipo(quad->opnd2,&tipo2,&valint2,&valfloat2);
	if (tipo1 == INTOPND && tipo2 == INTOPND){
		*(quad->result.atr.simb->vallogic) = valint1 >= valint2;
	}
	if (tipo1 == INTOPND && tipo2 == REALOPND){
		*(quad->result.atr.simb->vallogic) = valint1 >= valfloat2;
	}
	if (tipo1 == REALOPND && tipo2 == INTOPND){
		*(quad->result.atr.simb->vallogic) = valfloat1 >= valint2;
	}
	if (tipo1 == REALOPND && tipo2 == REALOPND){
		*(quad->result.atr.simb->vallogic) = valfloat1 >= valfloat2;
	}
}

void ExecQuadRead (quadrupla quad) {
	int i;  operando opndaux;  pilhaoperando pilhaopndaux;
	InicPilhaOpnd (&pilhaopndaux);
	for (i = 1; i <= quad->opnd1.atr.valint; i++) {
		EmpilharOpnd (TopoOpnd (pilhaopnd), &pilhaopndaux);
		DesempilharOpnd (&pilhaopnd);
	}
	for (i = 1; i <= quad->opnd1.atr.valint; i++) {
		opndaux = TopoOpnd (pilhaopndaux);
		DesempilharOpnd (&pilhaopndaux);
		simbolo s = opndaux.atr.simb;
		int c = 0;
		if(s->ponteiro){
			c = s->ponto;
			s = s->referente;
		}
      	switch (s->tvar) {
      		case INTEGER:
         			fscanf (finput, "%d", s->valint+c);
         			break;
        		case FLOAT:
         			fscanf (finput, "%g", s->valfloat+c);
         			break;
         		case LOGICAL:
         			fscanf (finput, "%d", s->vallogic+c);
         			break;
        		case CHAR:
         			fscanf (finput, "%c", s->valchar+c);
         			break;
     		}
	}
}

void ExecQuadResto(quadrupla quad){
	int tipo1, valint1, tipo2, valint2;
	char valchar1, valchar2;
	int res;
	VerificarTipoEValorTotal(quad->opnd1, &tipo1, &valint1, NULL, &valchar1, NULL);
	VerificarTipoEValorTotal(quad->opnd2, &tipo2, &valint2, NULL, &valchar2, NULL);
	if(tipo1 == INTOPND){
		if(tipo2 == INTOPND){
			res = valint1 % valint2;
		}else{
			res = valint1 % valchar2;
		}
	}else{
		if(tipo2 == INTOPND){
			res = valchar1 % valint2;
		}else{
			res = valchar1 % valchar2;
		}
	}
	switch (quad->result.atr.simb->tvar){
		case INTEGER:
			*(quad->result.atr.simb->valint) = res;
			break;
		case FLOAT:
			*(quad->result.atr.simb->valchar) = res;
			break;
	}
}

void ExecQuadMenun(quadrupla quad){
	int tipo1, valint1;
	float valfloat1;
	char valchar1;
	VerificarTipoEValorTotal(quad->opnd1, &tipo1, &valint1, &valfloat1, &valchar1, NULL);
	switch(quad->result.atr.simb->tvar){
		case INTEGER:
			switch(tipo1){
				case INTOPND:
					*(quad->result.atr.simb->valint) = -valint1;
					break;
				case REALOPND:
					*(quad->result.atr.simb->valint) = -valfloat1;
					break;
				case CHAROPND:
					*(quad->result.atr.simb->valint) = -valchar1;
					break;
			}
			break;
		case FLOAT:
			switch(tipo1){
				case INTOPND:
					*(quad->result.atr.simb->valfloat) = -valint1;
					break;
				case REALOPND:
					*(quad->result.atr.simb->valfloat) = -valfloat1;
					break;
				case CHAROPND:
					*(quad->result.atr.simb->valfloat) = -valchar1;
					break;
			}
			break;
		case CHAR:
			switch(tipo1){
				case INTOPND:
					*(quad->result.atr.simb->valchar) = -valint1;
					break;
				case REALOPND:
					*(quad->result.atr.simb->valchar) = -valfloat1;
					break;
				case CHAROPND:
					*(quad->result.atr.simb->valchar) = -valchar1;
					break;
			}
			break;
	}
}

void ExecQuadNot(quadrupla quad){
	char vallogic1;
	switch(quad->opnd1.tipo){
		case LOGICOPND:
			vallogic1 = quad->opnd1.atr.vallogic;
			break;
		case VAROPND:
			vallogic1 = *(quad->opnd1.atr.simb->vallogic);
			break;
	}
	*(quad->result.atr.simb->vallogic) = !vallogic1;
}

quadrupla ExecQuadCall(quadrupla quad, int *destparam){
	EmpilharQuad(quad,&pilhaquad);
	modhead r = encontrarMod(quad->opnd1.atr.simb);
	*destparam = quad->opnd2.atr.valint;
	if(quad->result.tipo != IDLEOPND){
		EmpilharOpnd(quad->result,&pilharetornos);
	}
	return r->listquad->prox;
}

quadrupla ExecQuadReturn(quadrupla quad){
	quadrupla res = TopoQuad(pilhaquad);
	DesempilharQuad(&pilhaquad);
	if(quad->opnd1.tipo != IDLEOPND){
		operando op = TopoOpnd(pilharetornos);
		DesempilharOpnd(&pilharetornos);
		executarAtribuicao(&quad->opnd1,op.atr.simb);
	}
	DesalocaVariaveis(res->opnd1.atr.simb);
	return res->prox;
}

void AtribuiParametros(int quant, simbolo chamadaatual){
	if(quant == 0){
		return;
	}
	int i;
	operando opndaux;
	pilhaoperando pilhaopndaux;

	InicPilhaOpnd (&pilhaopndaux);
	for (i = 1; i <= quant; i++) {
		EmpilharOpnd (TopoOpnd (pilhaopnd), &pilhaopndaux);
		DesempilharOpnd (&pilhaopnd);
	}
	for (i = 1; i <= quant; i++) {
		opndaux = TopoOpnd (pilhaopndaux);
		DesempilharOpnd (&pilhaopndaux);
      	executarAtribuicao(&opndaux, chamadaatual->listaparametros[i]);
	}
}

/* Implementacoes da pilha de quadruplas */

void EmpilharQuad (quadrupla x, pilhaquadrupla *P) {
	nohquad *temp;
	temp = *P;   
	*P = (nohquad *) malloc (sizeof (nohquad));
	(*P)->quad = x; (*P)->prox = temp;
}

void DesempilharQuad (pilhaquadrupla *P) {
	nohquad *temp;
	if (! VaziaQuad (*P)) {
		temp = *P;  *P = (*P)->prox; free (temp);
	}
	else  printf ("\n\tDelecao em pilha vazia\n");
}

quadrupla TopoQuad (pilhaquadrupla P) {
	if (! VaziaQuad (P))  return P->quad;
	else  printf ("\n\tTopo de pilha vazia\n");
}

void InicPilhaQuad (pilhaquadrupla *P) { 
	*P = NULL;
}

char VaziaQuad (pilhaquadrupla P) {
	if  (P == NULL)  return 1;  
	else return 0; 
}

modhead encontrarMod(simbolo s){
	modhead aux;
	for(aux=codintermed->prox;strcmp(aux->modname->cadeia,s->cadeia);aux = aux->prox){
	}
	return aux;
}

void ExecQuadInd(quadrupla quad){
	if(quad->opnd1.tipo==INTOPND){
		EmpilharInd(quad->opnd1.atr.valint,&pilhaind);
	}else{
		EmpilharInd(*(quad->opnd1.atr.simb->valint),&pilhaind);
	}
}

void ExecQuadIndex(quadrupla quad){
	simbolo s = quad->opnd1.atr.simb;
	int l = 0;
	int i;
	pilhaindice pilhaindaux;
	InicPilhaInd (&pilhaindaux);
	for (i = 1; i <= quad->opnd2.atr.valint; i++) {
		EmpilharInd (TopoInd (pilhaind), &pilhaindaux);
		DesempilharInd (&pilhaind);
	}
	for(i=1; i <= quad->opnd2.atr.valint;i++){
		l*=s->dims[i];
		int v = TopoInd(pilhaindaux);
		DesempilharInd(&pilhaindaux);
		l+=v;
	}
	simbolo novo = quad->result.atr.simb;
	novo->ponto = l;
	novo->referente = s;
}

void ExecQuadAtribPont(quadrupla quad){
	simbolo re = quad->result.atr.simb->referente;
	int p = quad->result.atr.simb->ponto;
	executarAtribuicaoPont(&(quad->opnd1), re, p);
}

void ExecQuadContaPont(quadrupla quad){
	simbolo s1 = quad->opnd1.atr.simb;
	simbolo refe = s1->referente;
	simbolo dest = quad->result.atr.simb;
	switch(refe->tvar){
		case INTEGER:
			*(dest->valint) = refe->valint[s1->ponto];
			break;
		case FLOAT:
			(*dest->valfloat) = refe->valfloat[s1->ponto];
			break;
		case CHAR:
			(*dest->valchar) = refe->valchar[s1->ponto];
			break;
		case LOGICAL:
			(*dest->vallogic) = refe->vallogic[s1->ponto];
			break;
	}
}

/* Implementacoes da pilha de indices */

void EmpilharInd (int x, pilhaindice *P) {
	nohind *temp;
	temp = *P;   
	*P = (nohind *) malloc (sizeof (nohind));
	(*P)->ind = x; (*P)->prox = temp;
}

void DesempilharInd (pilhaindice *P) {
	nohind *temp;
	if (! VaziaInd (*P)) {
		temp = *P;  *P = (*P)->prox; free (temp);
	}
	else  printf ("\n\tDelecao em pilha vazia\n");
}

int TopoInd (pilhaindice P) {
	if (! VaziaInd (P))  return P->ind;
	else  printf ("\n\tTopo de pilha vazia\n");
}

void InicPilhaInd (pilhaindice *P) { 
	*P = NULL;
}

char VaziaInd (pilhaindice P) {
	if  (P == NULL)  return 1;  
	else return 0; 
}