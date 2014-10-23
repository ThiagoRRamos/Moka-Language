#ifndef lint
static const char yysccsid[] = "@(#)yaccpar	1.9 (Berkeley) 02/21/93";
#endif

#define YYBYACC 1
#define YYMAJOR 1
#define YYMINOR 9
#define YYPATCH 20121003

#define YYEMPTY        (-1)
#define yyclearin      (yychar = YYEMPTY)
#define yyerrok        (yyerrflag = 0)
#define YYRECOVERING() (yyerrflag != 0)

#define YYPREFIX "yy"

#define YYPURE 0

#line 2 "analisador.y"
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

#line 309 "analisador.y"
#ifdef YYSTYPE
#undef  YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
#endif
#ifndef YYSTYPE_IS_DECLARED
#define YYSTYPE_IS_DECLARED 1
typedef union {
	char cadeia[50];
	int atr, valint;
	float valreal;
	char carac;
   	simbolo simb;
	infoexpressao infoexpr;
	infovariavel infovar;
	quadrupla quad;
   	int nsubscr, tipoexpr, nargs;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 345 "y.tab.c"

/* compatibility with bison */
#ifdef YYPARSE_PARAM
/* compatibility with FreeBSD */
# ifdef YYPARSE_PARAM_TYPE
#  define YYPARSE_DECL() yyparse(YYPARSE_PARAM_TYPE YYPARSE_PARAM)
# else
#  define YYPARSE_DECL() yyparse(void *YYPARSE_PARAM)
# endif
#else
# define YYPARSE_DECL() yyparse(void)
#endif

/* Parameters sent to lex. */
#ifdef YYLEX_PARAM
# define YYLEX_DECL() yylex(void *YYLEX_PARAM)
# define YYLEX yylex(YYLEX_PARAM)
#else
# define YYLEX_DECL() yylex(void)
# define YYLEX yylex()
#endif

/* Parameters sent to yyerror. */
#ifndef YYERROR_DECL
#define YYERROR_DECL() yyerror(const char *s)
#endif
#ifndef YYERROR_CALL
#define YYERROR_CALL(msg) yyerror(msg)
#endif

extern int YYPARSE_DECL();

#define ID 257
#define CADEIA 258
#define CTINT 259
#define CTREAL 260
#define OPAD 261
#define OPMULT 262
#define OPREL 263
#define CTCARAC 264
#define INVAL 265
#define ABPAR 266
#define FPAR 267
#define ABCHAV 268
#define FCHAV 269
#define ABCOL 270
#define FCOL 271
#define PVIRG 272
#define ATRIB 273
#define AND 274
#define NEG 275
#define NOT 276
#define OR 277
#define CARAC 278
#define INT 279
#define REAL 280
#define VERDADE 281
#define FALSO 282
#define VIRG 283
#define CHAMAR 284
#define ENQUANTO 285
#define ESCREVER 286
#define FUNCAO 287
#define LER 288
#define LOGIC 289
#define PARA 290
#define PROCEDIMENTO 291
#define PROGRAMA 292
#define REPETIR 293
#define RETORNAR 294
#define SE 295
#define SENAO 296
#define VAR 297
#define YYERRCODE 256
static const short yylhs[] = {                           -1,
   18,   20,   23,    0,   19,   25,   19,   24,   24,   28,
   26,   15,   15,   15,   15,   27,   30,   27,   29,   32,
   29,   31,   31,   21,   21,   35,   33,   34,   34,   36,
   38,   36,   37,   39,   37,   17,   40,   17,   16,   42,
   22,   41,   41,   43,   43,   43,   43,   43,   43,   43,
   43,   43,   43,   43,   53,   54,   55,   57,   44,   56,
   58,   56,   59,   60,   61,   45,   62,   63,   64,   46,
   65,   66,   67,   68,   69,   70,   71,   47,   72,   48,
    4,   73,    4,   74,   49,    5,   75,    5,   13,   13,
   51,   76,   51,   52,   77,   78,   52,   79,   80,   50,
    1,   81,    1,    6,   82,    6,    7,   83,    7,    8,
   84,    8,    9,   85,    9,   10,   86,   10,   11,   87,
   11,   12,   12,   12,   12,   12,   12,   88,   12,   89,
   12,   12,    3,   90,    3,    2,   91,    2,   14,   92,
   14,
};
static const short yylen[] = {                            2,
    0,    0,    0,    9,    0,    0,    3,    1,    2,    0,
    4,    1,    1,    1,    1,    1,    0,    4,    1,    0,
    5,    1,    3,    0,    2,    0,    4,    1,    1,    6,
    0,    8,    5,    0,    7,    1,    0,    4,    2,    0,
    4,    0,    2,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    1,    0,    0,    0,    0,   10,    0,
    0,    3,    0,    0,    0,    8,    0,    0,    0,   10,
    0,    0,    0,    0,    0,    0,    0,   20,    0,    6,
    1,    0,    4,    0,    6,    1,    0,    4,    1,    1,
    5,    0,    7,    2,    0,    0,    5,    0,    0,    6,
    1,    0,    4,    1,    0,    4,    1,    0,    4,    1,
    0,    3,    1,    0,    4,    1,    0,    4,    1,    0,
    4,    1,    1,    1,    1,    1,    1,    0,    3,    0,
    4,    1,    1,    0,    5,    1,    0,    4,    3,    0,
    5,
};
static const short yydefred[] = {                         1,
    0,    0,    0,    0,    2,    0,    6,   24,    0,    0,
   14,   12,   13,   15,   10,    0,    8,    0,   25,    0,
    0,    9,   40,    4,    0,    0,    0,   28,   29,    0,
    0,   16,   42,    0,    0,    0,   20,   11,   17,    0,
    0,    0,   27,    0,    0,   41,   54,    0,    0,    0,
    0,   71,   67,    0,    0,   44,   43,   45,   46,   47,
   48,   49,   50,   51,   52,   53,    0,    0,    0,    0,
   22,    0,   18,    0,   63,   84,   79,    0,    0,   94,
    0,   55,    0,   99,    0,    0,   33,    0,   36,    0,
   21,    0,    0,    0,    0,    0,    0,    0,    0,  123,
  124,  125,  130,  128,  111,  126,  127,  122,    0,    0,
  107,  110,    0,    0,  119,  132,    0,  134,    0,   30,
    0,   39,    0,   37,   23,    0,    0,    0,   89,    0,
    0,   86,   81,    0,   72,   68,    0,    0,    0,    0,
  105,    0,  108,  117,  114,  120,    0,    0,    0,    0,
   35,    0,   91,    0,    0,    0,    0,   87,    0,   82,
    0,    0,  139,    0,    0,  129,  112,    0,   97,    0,
    0,    0,    0,    0,    0,    0,    0,   32,   38,    0,
  102,   65,   85,    0,   80,    0,    0,    0,    0,  131,
    0,  109,    0,    0,  121,   57,  135,  137,  100,   93,
    0,    0,   88,   83,    0,    0,  141,    0,    0,    0,
   66,    0,    0,   58,    0,   74,    0,    0,    0,   70,
   61,   59,    0,    0,   75,   62,    0,    0,   76,    0,
    0,   77,    0,   78,
};
static const short yydgoto[] = {                          1,
  154,  175,  108,  134,  130,  131,  110,  111,  112,  113,
  114,  115,  132,  116,   88,   89,   90,    2,    8,    6,
   10,   56,   18,   16,    9,   17,   31,   21,   32,   45,
   72,   44,   19,   27,   20,   28,   29,   86,   70,  152,
   40,   33,   57,   58,   59,   60,   61,   62,   63,   64,
   65,   66,  117,  174,  208,  222,  218,  224,   94,  156,
  202,   79,  162,  213,   78,  161,  212,  219,  227,  230,
  233,   96,  186,   95,  184,  127,   81,  142,   67,  119,
  201,  168,  170,  140,  172,  171,  173,  139,  138,  148,
  209,  164,
};
static const short yysindex[] = {                         0,
    0, -272, -197, -214,    0, -170,    0,    0,  -35,    0,
    0,    0,    0,    0,    0,  -35,    0, -162,    0, -279,
 -109,    0,    0,    0,  -35,  -90, -170,    0,    0, -101,
 -232,    0,    0,  -70,  -91, -162,    0,    0,    0, -219,
  -75,  -64,    0,  -59, -109,    0,    0,  -44,  -62,  -48,
  -38,    0,    0,  -57,  -37,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,  -24,  -45,  -33,  -35,
    0, -260,    0,  -32,    0,    0,    0,  -29, -195,    0,
 -123,    0,  -28,    0,  -26,  -35,    0,  -17,    0, -257,
    0,   -4,  -11, -123, -143,  -24,  -24,  -25,  -82,    0,
    0,    0,    0,    0,    0,    0,    0,    0,  -20,  -16,
    0,    0,  -54,   -1,    0,    0, -123,    0,  -10,    0,
 -236,    0,   -8,    0,    0,   -7, -123,  -20,    0, -199,
  -20,    0,    0, -187,    0,    0,    1, -123, -203, -203,
    0,   -6,    0,    0,    0,    0,  -20, -203, -123,   -5,
    0,  -35,    0, -157,  -20,    2,   -2,    0,    3,    0,
    4,    5,    0, -123, -222,    0,    0, -123,    0, -123,
 -203, -203, -203,    6, -126,   11, -191,    0,    0,    7,
    0,    0,    0, -143,    0,  -24, -123, -123, -154,    0,
  -16,    0,   -1,   11,    0,    0,    0,    0,    0,    0,
 -123, -195,    0,    0,  -20,  -20,    0, -195, -203,  -20,
    0,    8,    9,    0,   11,    0,   10,  -22, -123,    0,
    0,    0, -189, -195,    0,    0,  -24,   12,    0, -123,
 -142,    0, -195,    0,
};
static const short yyrindex[] = {                         0,
    0,    0,    0,    0,    0, -263,    0,    0,    0, -255,
    0,    0,    0,    0,    0, -186,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   13,    0,    0, -141,
    0,    0,    0,    0,    0,    0,    0,    0,    0,   21,
    0,  -30,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -110,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -27,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   21,    0,
    0,    0, -221,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -96,    0,    0,    0,    0,    0,  -66,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   14, -165,
    0,    0, -258,  -51,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,   16,    0,    0,
 -139,    0,    0,    0,    0,    0,  -83,    0,    0,    0,
    0,    0,    0,    0,    0,    0,   17,    0,    0,    0,
    0,    0,    0,    0, -105,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -89,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
  -53,    0,  -36, -180,    0,    0,    0,    0,    0,    0,
    0,   21,    0,    0,   15,  -64,    0,   21,    0,  -94,
    0,    0,    0,    0,  -81,    0,    0, -251,    0,    0,
    0,    0,    0,   21,    0,    0,    0,    0,    0,    0,
    0,    0,   21,    0,
};
static const short yygindex[] = {                         0,
  124,    0,  -67,    0,    0,  -79,  121,  120,  151, -145,
  122, -132,  108,    0,   95,  142,  209,    0,  269,    0,
    0,  -14,    0,    0,    0,  281,    0,    0,  253,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -78,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,
};
#define YYTABLESIZE 298
static const short yytable[] = {                         84,
   98,  109,  176,   24,    5,   60,  166,   25,  113,  123,
   91,   26,    3,  113,  128,  113,   60,   60,  113,    3,
   60,   43,   92,    5,  113,  124,  194,    5,  133,  135,
  150,   26,   60,   60,   60,   26,   60,  147,   60,   38,
  195,   60,   60,   60,  190,  133,  124,  155,   23,   46,
   39,  133,   47,   99,  141,  100,  101,    5,  165,    4,
  102,  133,  103,  215,   48,   49,   50,  157,   51,  177,
   52,  104,   23,   53,   54,   55,   47,  106,  107,  159,
  199,    7,  225,  158,  155,  141,  115,  141,   48,   49,
   50,  115,   51,  115,   52,  160,  115,   53,   54,   55,
    7,  104,  115,   15,    7,   23,  104,  205,  206,  180,
   15,  104,  207,   99,  129,  100,  101,  104,  204,   34,
  102,  210,  103,  211,  232,  181,    7,   90,  181,  214,
   19,  104,  105,   99,  141,  100,  101,  106,  107,  223,
  102,   19,  103,   90,  197,  226,   95,   30,   95,   95,
  231,  104,  105,   95,  234,   95,  198,  106,  107,  228,
   92,  101,   92,   92,   95,   95,   35,   92,   37,   92,
   95,   95,  103,  140,   42,  140,  140,  101,   92,   92,
  140,  136,  140,  137,   92,   92,   41,  118,  103,  138,
   68,  140,  140,  136,  133,  133,  133,  140,  140,   71,
  133,  138,   69,   75,  133,  133,  144,  133,  145,  116,
  133,  116,   74,  106,   80,  116,  133,   76,  106,  116,
  116,   85,  116,  106,  118,  116,  118,   77,   82,  106,
  118,  116,   83,   93,  118,  118,   97,  118,   87,  122,
  118,  118,   11,   12,   13,  120,  118,   34,   34,   34,
   31,   31,   31,   14,  125,  126,  141,  143,   34,  136,
  146,   31,  149,  151,  153,  169,  178,  163,  182,  183,
  188,  144,  196,  221,  185,  217,  187,   98,  200,  216,
    5,  220,   64,   56,  229,   96,   73,  189,  191,  192,
  167,  203,  193,  179,  121,   36,   22,   73,
};
static const short yycheck[] = {                         67,
   79,   81,  148,   18,  268,  257,  139,  287,  267,  267,
  271,  291,  268,  272,   94,  274,  268,  269,  277,  292,
  272,   36,  283,  287,  283,  283,  172,  291,   96,   97,
  267,  287,  284,  285,  286,  291,  288,  117,  290,  272,
  173,  293,  294,  295,  267,  267,  283,  127,  268,  269,
  283,  273,  272,  257,  277,  259,  260,  272,  138,  257,
  264,  283,  266,  209,  284,  285,  286,  267,  288,  149,
  290,  275,  268,  293,  294,  295,  272,  281,  282,  267,
  272,  268,  272,  283,  164,  277,  267,  277,  284,  285,
  286,  272,  288,  274,  290,  283,  277,  293,  294,  295,
  287,  267,  283,    9,  291,  268,  272,  187,  188,  267,
   16,  277,  267,  257,  258,  259,  260,  283,  186,   25,
  264,  201,  266,  202,  267,  283,  297,  267,  283,  208,
  272,  275,  276,  257,  277,  259,  260,  281,  282,  219,
  264,  283,  266,  283,  271,  224,  257,  257,  259,  260,
  230,  275,  276,  264,  233,  266,  283,  281,  282,  227,
  257,  267,  259,  260,  275,  276,  257,  264,  270,  266,
  281,  282,  267,  257,  266,  259,  260,  283,  275,  276,
  264,  271,  266,  266,  281,  282,  257,  270,  283,  271,
  266,  275,  276,  283,  261,  262,  263,  281,  282,  259,
  267,  283,  267,  266,  271,  272,  261,  274,  263,  261,
  277,  263,  257,  267,  272,  267,  283,  266,  272,  271,
  272,  267,  274,  277,  261,  277,  263,  266,  266,  283,
  267,  283,  257,  266,  271,  272,  266,  274,  272,  257,
  277,  270,  278,  279,  280,  272,  283,  278,  279,  280,
  278,  279,  280,  289,  259,  267,  277,  274,  289,  285,
  262,  289,  273,  272,  272,  272,  272,  267,  267,  272,
  266,  261,  267,  296,  272,  267,  273,  257,  272,  272,
  268,  272,  267,  267,  273,  272,  272,  164,  168,  170,
  140,  184,  171,  152,   86,   27,   16,   45,
};
#define YYFINAL 1
#ifndef YYDEBUG
#define YYDEBUG 0
#endif
#define YYMAXTOKEN 297
#if YYDEBUG
static const char *yyname[] = {

"end-of-file",0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,"ID","CADEIA","CTINT","CTREAL",
"OPAD","OPMULT","OPREL","CTCARAC","INVAL","ABPAR","FPAR","ABCHAV","FCHAV",
"ABCOL","FCOL","PVIRG","ATRIB","AND","NEG","NOT","OR","CARAC","INT","REAL",
"VERDADE","FALSO","VIRG","CHAMAR","ENQUANTO","ESCREVER","FUNCAO","LER","LOGIC",
"PARA","PROCEDIMENTO","PROGRAMA","REPETIR","RETORNAR","SE","SENAO","VAR",
};
static const char *yyrule[] = {
"$accept : Prog",
"$$1 :",
"$$2 :",
"$$3 :",
"Prog : $$1 PROGRAMA ID PVIRG $$2 Decls SubProgs $$3 CmdComp",
"Decls :",
"$$4 :",
"Decls : VAR $$4 ListDecl",
"ListDecl : Declaracao",
"ListDecl : ListDecl Declaracao",
"$$5 :",
"Declaracao : Tipo $$5 ListElemDecl PVIRG",
"Tipo : INT",
"Tipo : REAL",
"Tipo : CARAC",
"Tipo : LOGIC",
"ListElemDecl : ElemDecl",
"$$6 :",
"ListElemDecl : ListElemDecl VIRG $$6 ElemDecl",
"ElemDecl : ID",
"$$7 :",
"ElemDecl : ID ABCOL $$7 ListDim FCOL",
"ListDim : CTINT",
"ListDim : ListDim VIRG CTINT",
"SubProgs :",
"SubProgs : SubProgs DeclSubProg",
"$$8 :",
"DeclSubProg : $$8 Cabecalho Decls CmdComp",
"Cabecalho : CabFunc",
"Cabecalho : CabProc",
"CabFunc : FUNCAO Tipo ID ABPAR FPAR PVIRG",
"$$9 :",
"CabFunc : FUNCAO Tipo ID ABPAR $$9 ListParam FPAR PVIRG",
"CabProc : PROCEDIMENTO ID ABPAR FPAR PVIRG",
"$$10 :",
"CabProc : PROCEDIMENTO ID ABPAR $$10 ListParam FPAR PVIRG",
"ListParam : Parametro",
"$$11 :",
"ListParam : ListParam VIRG $$11 Parametro",
"Parametro : Tipo ID",
"$$12 :",
"CmdComp : ABCHAV $$12 ListCmd FCHAV",
"ListCmd :",
"ListCmd : ListCmd Comando",
"Comando : CmdComp",
"Comando : CmdSe",
"Comando : CmdEnquanto",
"Comando : CmdRepetir",
"Comando : CmdPara",
"Comando : CmdLer",
"Comando : CmdEscrever",
"Comando : CmdAtrib",
"Comando : ChamaProc",
"Comando : CmdRetornar",
"Comando : PVIRG",
"$$13 :",
"$$14 :",
"$$15 :",
"$$16 :",
"CmdSe : SE ABPAR $$13 Expressao $$14 FPAR $$15 Comando $$16 CmdSenao",
"CmdSenao :",
"$$17 :",
"CmdSenao : SENAO $$17 Comando",
"$$18 :",
"$$19 :",
"$$20 :",
"CmdEnquanto : ENQUANTO ABPAR $$18 Expressao $$19 FPAR $$20 Comando",
"$$21 :",
"$$22 :",
"$$23 :",
"CmdRepetir : REPETIR $$21 Comando ENQUANTO $$22 ABPAR Expressao $$23 FPAR PVIRG",
"$$24 :",
"$$25 :",
"$$26 :",
"$$27 :",
"$$28 :",
"$$29 :",
"$$30 :",
"CmdPara : PARA $$24 ABPAR Variavel $$25 ATRIB Expressao $$26 PVIRG $$27 Expressao PVIRG $$28 Variavel ATRIB $$29 Expressao FPAR $$30 Comando",
"$$31 :",
"CmdLer : LER ABPAR $$31 ListVar FPAR PVIRG",
"ListVar : Variavel",
"$$32 :",
"ListVar : ListVar VIRG $$32 Variavel",
"$$33 :",
"CmdEscrever : ESCREVER ABPAR $$33 ListEscr FPAR PVIRG",
"ListEscr : ElemEscr",
"$$34 :",
"ListEscr : ListEscr VIRG $$34 ElemEscr",
"ElemEscr : CADEIA",
"ElemEscr : Expressao",
"ChamaProc : CHAMAR ID ABPAR FPAR PVIRG",
"$$35 :",
"ChamaProc : CHAMAR ID ABPAR $$35 ListExpr FPAR PVIRG",
"CmdRetornar : RETORNAR PVIRG",
"$$36 :",
"$$37 :",
"CmdRetornar : RETORNAR $$36 Expressao $$37 PVIRG",
"$$38 :",
"$$39 :",
"CmdAtrib : $$38 Variavel $$39 ATRIB Expressao PVIRG",
"ListExpr : Expressao",
"$$40 :",
"ListExpr : ListExpr VIRG $$40 Expressao",
"Expressao : ExprAux1",
"$$41 :",
"Expressao : Expressao OR $$41 ExprAux1",
"ExprAux1 : ExprAux2",
"$$42 :",
"ExprAux1 : ExprAux1 AND $$42 ExprAux2",
"ExprAux2 : ExprAux3",
"$$43 :",
"ExprAux2 : NOT $$43 ExprAux3",
"ExprAux3 : ExprAux4",
"$$44 :",
"ExprAux3 : ExprAux4 OPREL $$44 ExprAux4",
"ExprAux4 : Termo",
"$$45 :",
"ExprAux4 : ExprAux4 OPAD $$45 Termo",
"Termo : Fator",
"$$46 :",
"Termo : Termo OPMULT $$46 Fator",
"Fator : Variavel",
"Fator : CTINT",
"Fator : CTREAL",
"Fator : CTCARAC",
"Fator : VERDADE",
"Fator : FALSO",
"$$47 :",
"Fator : NEG $$47 Fator",
"$$48 :",
"Fator : ABPAR $$48 Expressao FPAR",
"Fator : ChamaFunc",
"Variavel : ID",
"$$49 :",
"Variavel : ID ABCOL $$49 ListSubscr FCOL",
"ListSubscr : ExprAux4",
"$$50 :",
"ListSubscr : ListSubscr VIRG $$50 ExprAux4",
"ChamaFunc : ID ABPAR FPAR",
"$$51 :",
"ChamaFunc : ID ABPAR $$51 ListExpr FPAR",

};
#endif

int      yydebug;
int      yynerrs;

int      yyerrflag;
int      yychar;
YYSTYPE  yyval;
YYSTYPE  yylval;

/* define the initial stack-sizes */
#ifdef YYSTACKSIZE
#undef YYMAXDEPTH
#define YYMAXDEPTH  YYSTACKSIZE
#else
#ifdef YYMAXDEPTH
#define YYSTACKSIZE YYMAXDEPTH
#else
#define YYSTACKSIZE 500
#define YYMAXDEPTH  500
#endif
#endif

#define YYINITSTACKSIZE 500

typedef struct {
    unsigned stacksize;
    short    *s_base;
    short    *s_mark;
    short    *s_last;
    YYSTYPE  *l_base;
    YYSTYPE  *l_mark;
} YYSTACKDATA;
/* variables for the parser stack */
static YYSTACKDATA yystack;
#line 865 "analisador.y"
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
#line 2257 "y.tab.c"

#if YYDEBUG
#include <stdio.h>		/* needed for printf */
#endif

#include <stdlib.h>	/* needed for malloc, etc */
#include <string.h>	/* needed for memset */

/* allocate initial stack or double stack size, up to YYMAXDEPTH */
static int yygrowstack(YYSTACKDATA *data)
{
    int i;
    unsigned newsize;
    short *newss;
    YYSTYPE *newvs;

    if ((newsize = data->stacksize) == 0)
        newsize = YYINITSTACKSIZE;
    else if (newsize >= YYMAXDEPTH)
        return -1;
    else if ((newsize *= 2) > YYMAXDEPTH)
        newsize = YYMAXDEPTH;

    i = data->s_mark - data->s_base;
    newss = (short *)realloc(data->s_base, newsize * sizeof(*newss));
    if (newss == 0)
        return -1;

    data->s_base = newss;
    data->s_mark = newss + i;

    newvs = (YYSTYPE *)realloc(data->l_base, newsize * sizeof(*newvs));
    if (newvs == 0)
        return -1;

    data->l_base = newvs;
    data->l_mark = newvs + i;

    data->stacksize = newsize;
    data->s_last = data->s_base + newsize - 1;
    return 0;
}

#if YYPURE || defined(YY_NO_LEAKS)
static void yyfreestack(YYSTACKDATA *data)
{
    free(data->s_base);
    free(data->l_base);
    memset(data, 0, sizeof(*data));
}
#else
#define yyfreestack(data) /* nothing */
#endif

#define YYABORT  goto yyabort
#define YYREJECT goto yyabort
#define YYACCEPT goto yyaccept
#define YYERROR  goto yyerrlab

int
YYPARSE_DECL()
{
    int yym, yyn, yystate;
#if YYDEBUG
    const char *yys;

    if ((yys = getenv("YYDEBUG")) != 0)
    {
        yyn = *yys;
        if (yyn >= '0' && yyn <= '9')
            yydebug = yyn - '0';
    }
#endif

    yynerrs = 0;
    yyerrflag = 0;
    yychar = YYEMPTY;
    yystate = 0;

#if YYPURE
    memset(&yystack, 0, sizeof(yystack));
#endif

    if (yystack.s_base == NULL && yygrowstack(&yystack)) goto yyoverflow;
    yystack.s_mark = yystack.s_base;
    yystack.l_mark = yystack.l_base;
    yystate = 0;
    *yystack.s_mark = 0;

yyloop:
    if ((yyn = yydefred[yystate]) != 0) goto yyreduce;
    if (yychar < 0)
    {
        if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, reading %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
    }
    if ((yyn = yysindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: state %d, shifting to state %d\n",
                    YYPREFIX, yystate, yytable[yyn]);
#endif
        if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
        {
            goto yyoverflow;
        }
        yystate = yytable[yyn];
        *++yystack.s_mark = yytable[yyn];
        *++yystack.l_mark = yylval;
        yychar = YYEMPTY;
        if (yyerrflag > 0)  --yyerrflag;
        goto yyloop;
    }
    if ((yyn = yyrindex[yystate]) && (yyn += yychar) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yychar)
    {
        yyn = yytable[yyn];
        goto yyreduce;
    }
    if (yyerrflag) goto yyinrecovery;

    yyerror("syntax error");

    goto yyerrlab;

yyerrlab:
    ++yynerrs;

yyinrecovery:
    if (yyerrflag < 3)
    {
        yyerrflag = 3;
        for (;;)
        {
            if ((yyn = yysindex[*yystack.s_mark]) && (yyn += YYERRCODE) >= 0 &&
                    yyn <= YYTABLESIZE && yycheck[yyn] == YYERRCODE)
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: state %d, error recovery shifting\
 to state %d\n", YYPREFIX, *yystack.s_mark, yytable[yyn]);
#endif
                if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
                {
                    goto yyoverflow;
                }
                yystate = yytable[yyn];
                *++yystack.s_mark = yytable[yyn];
                *++yystack.l_mark = yylval;
                goto yyloop;
            }
            else
            {
#if YYDEBUG
                if (yydebug)
                    printf("%sdebug: error recovery discarding state %d\n",
                            YYPREFIX, *yystack.s_mark);
#endif
                if (yystack.s_mark <= yystack.s_base) goto yyabort;
                --yystack.s_mark;
                --yystack.l_mark;
            }
        }
    }
    else
    {
        if (yychar == 0) goto yyabort;
#if YYDEBUG
        if (yydebug)
        {
            yys = 0;
            if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
            if (!yys) yys = "illegal-symbol";
            printf("%sdebug: state %d, error recovery discards token %d (%s)\n",
                    YYPREFIX, yystate, yychar, yys);
        }
#endif
        yychar = YYEMPTY;
        goto yyloop;
    }

yyreduce:
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: state %d, reducing by rule %d (%s)\n",
                YYPREFIX, yystate, yyn, yyrule[yyn]);
#endif
    yym = yylen[yyn];
    if (yym)
        yyval = yystack.l_mark[1-yym];
    else
        memset(&yyval, 0, sizeof yyval);
    switch (yyn)
    {
case 1:
#line 371 "analisador.y"
	{InicTabSimb ();
				InicCodIntermed ();
				numtemp = 0;}
break;
case 2:
#line 374 "analisador.y"
	{
				escopoatual = InsereSimb (yystack.l_mark[-1].cadeia, IDPROG, NOTVAR, NULL);
				InicCodIntermMod (escopoatual);
          		escopoglobal=escopoatual;}
break;
case 3:
#line 379 "analisador.y"
	{
          			numquadcorrente = 0;
          			modcorrente = codintermed->prox;
          			quadcorrente = modcorrente->listquad;
          			opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			yyval.quad = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);
          		}
break;
case 4:
#line 388 "analisador.y"
	{
          			yyval.quad = GeraQuadrupla (OPEXIT, opndidle, opndidle, opndidle);
          			VerificaInicRef();
          			InterpCodIntermed ();}
break;
case 6:
#line 393 "analisador.y"
	{tab++;}
break;
case 7:
#line 393 "analisador.y"
	{tab--;}
break;
case 10:
#line 397 "analisador.y"
	{ }
break;
case 11:
#line 397 "analisador.y"
	{}
break;
case 12:
#line 399 "analisador.y"
	{strcpy(yyval.cadeia, "int"); tipocorrente = INTEGER;}
break;
case 13:
#line 400 "analisador.y"
	{strcpy(yyval.cadeia, "real"); tipocorrente = FLOAT;}
break;
case 14:
#line 401 "analisador.y"
	{strcpy(yyval.cadeia, "carac"); tipocorrente = CHAR;}
break;
case 15:
#line 402 "analisador.y"
	{strcpy(yyval.cadeia, "logic"); tipocorrente = LOGICAL;}
break;
case 17:
#line 403 "analisador.y"
	{}
break;
case 19:
#line 406 "analisador.y"
	{
				AnalisarInsercaoSimb(yystack.l_mark[0].cadeia, IDVAR, FALSE, escopoatual);}
break;
case 20:
#line 409 "analisador.y"
	{
				AnalisarInsercaoSimb(yystack.l_mark[-1].cadeia, IDVAR, TRUE, escopoatual);}
break;
case 21:
#line 410 "analisador.y"
	{}
break;
case 22:
#line 412 "analisador.y"
	{ConferirValorDim(yystack.l_mark[0].valint);}
break;
case 23:
#line 413 "analisador.y"
	{ ConferirValorDim(yystack.l_mark[0].valint);}
break;
case 26:
#line 417 "analisador.y"
	{temretorno = FALSE;}
break;
case 27:
#line 419 "analisador.y"
	{
					if(!temretorno){
						if(escopoatual->tid == IDPROC){
							yyval.quad = GeraQuadrupla (OPRETURN, opndidle, opndidle, opndidle);
						}else{
							Esperado("Eh esperado um retorno em funcoes");
						}
					}
					escopoatual = escopoglobal;}
break;
case 30:
#line 432 "analisador.y"
	{
					AnalisarInsercaoSimb (yystack.l_mark[-3].cadeia, IDFUNC, FALSE, escopoglobal);
					escopoatual=simb;
					InicCodIntermMod (escopoatual);
					opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			yyval.quad = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);}
break;
case 31:
#line 440 "analisador.y"
	{
					AnalisarInsercaoSimb (yystack.l_mark[-1].cadeia, IDFUNC, TRUE, escopoglobal);
					escopoatual=simb;
					InicCodIntermMod (escopoatual);
					opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			yyval.quad = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);}
break;
case 33:
#line 449 "analisador.y"
	{
					AnalisarInsercaoSimb (yystack.l_mark[-3].cadeia, IDPROC, FALSE, escopoglobal);
					escopoatual=simb;
					InicCodIntermMod (escopoatual);
					opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			yyval.quad = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);}
break;
case 34:
#line 457 "analisador.y"
	{
			AnalisarInsercaoSimb (yystack.l_mark[-1].cadeia, IDPROC, TRUE, escopoglobal);
			escopoatual=simb;
			InicCodIntermMod (escopoatual);
			opnd1.tipo = MODOPND;
      		opnd1.atr.modulo = modcorrente;
      		yyval.quad = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);}
break;
case 37:
#line 465 "analisador.y"
	{}
break;
case 39:
#line 468 "analisador.y"
	{AnalisarInsercaoSimb(yystack.l_mark[0].cadeia, IDVAR, FALSE, escopoatual);
						yyval.simb = simb;
						yyval.simb->inic = TRUE;
						
						ConferirValorParam(tipocorrente, yyval.simb);}
break;
case 40:
#line 475 "analisador.y"
	{
					
					tab++;}
break;
case 41:
#line 478 "analisador.y"
	{tab--;
					
					}
break;
case 55:
#line 489 "analisador.y"
	{
						tab++;
						}
break;
case 56:
#line 493 "analisador.y"
	{ConferirExpLogica(yystack.l_mark[0].infoexpr.tipo);
						opndaux.tipo = ROTOPND;
						yyval.quad = GeraQuadrupla (OPJF, yystack.l_mark[0].infoexpr.opnd, opndidle, opndaux);
					}
break;
case 57:
#line 497 "analisador.y"
	{}
break;
case 58:
#line 498 "analisador.y"
	{tab--;
						yyval.quad = quadcorrente;
						yystack.l_mark[-3].quad->result.atr.rotulo = GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}
break;
case 59:
#line 501 "analisador.y"
	{
						if (yystack.l_mark[-1].quad->prox != quadcorrente) {
							quadaux = yystack.l_mark[-1].quad->prox;
							yystack.l_mark[-1].quad->prox = quadaux->prox;
							quadaux->prox = yystack.l_mark[-1].quad->prox->prox;
							yystack.l_mark[-1].quad->prox->prox = quadaux;
							RenumQuadruplas (yystack.l_mark[-1].quad, quadcorrente);
						}
					}
break;
case 61:
#line 512 "analisador.y"
	{
							tab++;
							
							opndaux.tipo = ROTOPND;
							yyval.quad = GeraQuadrupla (OPJUMP, opndidle, opndidle,opndaux);}
break;
case 62:
#line 518 "analisador.y"
	{tab--;
							yystack.l_mark[-1].quad->result.atr.rotulo =GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}
break;
case 63:
#line 522 "analisador.y"
	{
						
						yyval.quad = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);}
break;
case 64:
#line 526 "analisador.y"
	{ConferirExpLogica(yystack.l_mark[0].infoexpr.tipo);
					opndaux.tipo = ROTOPND;
					yyval.quad = GeraQuadrupla(OPJF, yystack.l_mark[0].infoexpr.opnd, opndidle, opndaux);}
break;
case 65:
#line 530 "analisador.y"
	{tab++;}
break;
case 66:
#line 532 "analisador.y"
	{tab--;
					yyval.quad = quadcorrente;
					opndaux.tipo = ROTOPND;
					yyval.quad = GeraQuadrupla (OPJUMP, opndidle, opndidle, opndaux);
					yyval.quad->result.atr.rotulo = yystack.l_mark[-5].quad;
					yystack.l_mark[-3].quad->result.atr.rotulo = GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}
break;
case 67:
#line 540 "analisador.y"
	{
						
						tab++;
						yyval.quad = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);}
break;
case 68:
#line 546 "analisador.y"
	{tab--;   }
break;
case 69:
#line 549 "analisador.y"
	{ConferirExpLogica(yystack.l_mark[0].infoexpr.tipo);
						opndaux.tipo = ROTOPND;
						yyval.quad = GeraQuadrupla(OPJF, yystack.l_mark[0].infoexpr.opnd, opndidle, opndaux);
						opndaux.tipo = ROTOPND;
						quadaux = GeraQuadrupla(OPJUMP, opndidle, opndidle, opndaux);
						quadaux->result.atr.rotulo = yystack.l_mark[-5].quad;
						quadaux = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);
						yyval.quad->result.atr.rotulo = quadaux;
						}
break;
case 71:
#line 560 "analisador.y"
	{
					tab++;
					}
break;
case 72:
#line 564 "analisador.y"
	{
					ConferirPara(yystack.l_mark[0].infovar.simb);
					InicializarEReferenciar(yystack.l_mark[0].infovar.simb);}
break;
case 73:
#line 568 "analisador.y"
	{ConferirAtribuicao(yystack.l_mark[-3].infovar.simb, yystack.l_mark[0].infoexpr.tipo);
				 GeraQuadrupla (OPATRIB, yystack.l_mark[0].infoexpr.opnd, opndidle, yystack.l_mark[-3].infovar.opnd);}
break;
case 74:
#line 571 "analisador.y"
	{
				yyval.quad = GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}
break;
case 75:
#line 574 "analisador.y"
	{
					ConferirExpLogica(yystack.l_mark[-1].infoexpr.tipo);
					opndaux.tipo = ROTOPND;
					yyval.quad = GeraQuadrupla(OPJF, yystack.l_mark[-1].infoexpr.opnd, opndidle, opndaux);}
break;
case 76:
#line 579 "analisador.y"
	{
					MesmaVariavel(yystack.l_mark[-11].infovar.simb,yystack.l_mark[-1].infovar.simb);
					yyval.quad = quadcorrente;}
break;
case 77:
#line 584 "analisador.y"
	{
				yyval.quad = quadcorrente;}
break;
case 78:
#line 587 "analisador.y"
	{tab--;
					quadaux = yystack.l_mark[-7].quad->prox;
					yystack.l_mark[-7].quad->prox = yystack.l_mark[-1].quad->prox;
					quadcorrente->prox = quadaux;
					quadcorrente = yystack.l_mark[-1].quad;
					quadcorrente->prox = NULL;
					RenumQuadruplas(yystack.l_mark[-7].quad, quadcorrente->prox);
					yyval.quad = GeraQuadrupla (OPATRIB, yystack.l_mark[-3].infoexpr.opnd, opndidle, yystack.l_mark[-6].infovar.opnd);
					opndaux.tipo = ROTOPND;
					quadaux = GeraQuadrupla (OPJUMP, opndidle, opndidle, opndaux);
					quadaux->result.atr.rotulo = yystack.l_mark[-10].quad;
					quadaux = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);
					yystack.l_mark[-7].quad->result.atr.rotulo = quadaux;}
break;
case 79:
#line 601 "analisador.y"
	{}
break;
case 80:
#line 602 "analisador.y"
	{
					opnd1.tipo = INTOPND;
					opnd1.atr.valint = yystack.l_mark[-2].nargs;
					GeraQuadrupla (OPREAD, opnd1, opndidle, opndidle);}
break;
case 81:
#line 607 "analisador.y"
	{InicializarEReferenciar(yystack.l_mark[0].infovar.simb);
							yyval.nargs = 1;
							GeraQuadrupla (PARAM, yystack.l_mark[0].infovar.opnd, opndidle, opndidle);}
break;
case 82:
#line 610 "analisador.y"
	{}
break;
case 83:
#line 611 "analisador.y"
	{InicializarEReferenciar(yystack.l_mark[0].infovar.simb);
					yyval.nargs = yystack.l_mark[-3].nargs + 1;
					GeraQuadrupla (PARAM, yystack.l_mark[0].infovar.opnd, opndidle, opndidle);}
break;
case 84:
#line 615 "analisador.y"
	{}
break;
case 85:
#line 616 "analisador.y"
	{
						opnd1.tipo = INTOPND;
						opnd1.atr.valint = yystack.l_mark[-2].nargs;
						GeraQuadrupla (OPWRITE, opnd1, opndidle, opndidle);}
break;
case 86:
#line 622 "analisador.y"
	{
						yyval.nargs = 1;
						GeraQuadrupla (PARAM, yystack.l_mark[0].infoexpr.opnd, opndidle, opndidle);
					}
break;
case 87:
#line 626 "analisador.y"
	{}
break;
case 88:
#line 626 "analisador.y"
	{
							yyval.nargs = yystack.l_mark[-3].nargs + 1;
							GeraQuadrupla (PARAM, yystack.l_mark[0].infoexpr.opnd, opndidle, opndidle);
						}
break;
case 89:
#line 631 "analisador.y"
	{
						yyval.infoexpr.opnd.tipo = CADOPND;
						yyval.infoexpr.opnd.atr.valcad = malloc (strlen(yystack.l_mark[0].cadeia) + 1);
						strcpy (yyval.infoexpr.opnd.atr.valcad, yystack.l_mark[0].cadeia);}
break;
case 91:
#line 637 "analisador.y"
	{
						VerificarExistencia(yystack.l_mark[-3].cadeia);
						VerificarTamanhoSemParametros(simb);
						VerificarTipo(simb, IDPROC);
						VerificarRecursividade(escopoatual,simb);
						opndaux.tipo = INTOPND;
						opndaux.atr.valint = 0;
						opndaux2.tipo = FUNCOPND;
						opndaux2.atr.simb = simb;
						GeraQuadrupla  (OPCALL, opndaux2, opndaux, opndidle);}
break;
case 92:
#line 648 "analisador.y"
	{
						
							VerificarExistencia(yystack.l_mark[-1].cadeia);
							ultimachamada=simb;
							yyval.simb = simb;
							VerificarRecursividade(escopoatual,simb);}
break;
case 93:
#line 654 "analisador.y"
	{
						VerificarTamanho(yystack.l_mark[-3].simb,yystack.l_mark[-2].nsubscr);
						VerificarTipo(yystack.l_mark[-3].simb, IDPROC);
						opndaux.tipo = INTOPND;
						opndaux.atr.valint = yystack.l_mark[-3].simb->ndims;
						opndaux2.tipo = FUNCOPND;
						opndaux2.atr.simb = yystack.l_mark[-3].simb;
						GeraQuadrupla  (OPCALL, opndaux2, opndaux, opndidle);}
break;
case 94:
#line 663 "analisador.y"
	{
						temretorno = TRUE;
						ConferirRetornoProcedimento(escopoatual);
						GeraQuadrupla  (OPRETURN, opndidle, opndidle, opndidle);}
break;
case 95:
#line 668 "analisador.y"
	{
						}
break;
case 96:
#line 670 "analisador.y"
	{ConferirRetornoFuncao(escopoatual,yystack.l_mark[0].infoexpr.tipo);
					temretorno = TRUE;}
break;
case 97:
#line 672 "analisador.y"
	{
					GeraQuadrupla  (OPRETURN, yystack.l_mark[-2].infoexpr.opnd, opndidle, opndidle);}
break;
case 98:
#line 674 "analisador.y"
	{}
break;
case 99:
#line 675 "analisador.y"
	{if  (yystack.l_mark[0].infovar.simb != NULL)
						if(yystack.l_mark[0].infovar.simb->ponteiro){
							yystack.l_mark[0].infovar.simb->referente->inic = yystack.l_mark[0].infovar.simb->referente->ref = TRUE;
						}else{
							yystack.l_mark[0].infovar.simb->inic = yystack.l_mark[0].infovar.simb->ref = TRUE;
						}
							}
break;
case 100:
#line 682 "analisador.y"
	{
							ConferirAtribuicao(yystack.l_mark[-4].infovar.simb, yystack.l_mark[-1].infoexpr.tipo);
							if(!yystack.l_mark[-4].infovar.simb->ponteiro){
								GeraQuadrupla (OPATRIB, yystack.l_mark[-1].infoexpr.opnd, opndidle, yystack.l_mark[-4].infovar.opnd);
							}else{
								GeraQuadrupla (OPATRIBPONT, yystack.l_mark[-1].infoexpr.opnd, opndidle, yystack.l_mark[-4].infovar.opnd);
							}
}
break;
case 101:
#line 691 "analisador.y"
	{yyval.nsubscr = 1;
						checarTipo(1,yystack.l_mark[0].infoexpr.tipo);
						GeraQuadrupla (PARAM, yystack.l_mark[0].infoexpr.opnd, opndidle, opndidle);}
break;
case 102:
#line 694 "analisador.y"
	{}
break;
case 103:
#line 695 "analisador.y"
	{yyval.nsubscr = yyval.nsubscr + 1;
						checarTipo(yyval.nsubscr,yystack.l_mark[0].infoexpr.tipo);
						GeraQuadrupla (PARAM, yystack.l_mark[0].infoexpr.opnd, opndidle, opndidle);}
break;
case 105:
#line 698 "analisador.y"
	{}
break;
case 106:
#line 698 "analisador.y"
	{ConferirOr(yystack.l_mark[-3].infoexpr, yystack.l_mark[0].infoexpr, &yyval.infoexpr);}
break;
case 108:
#line 699 "analisador.y"
	{}
break;
case 109:
#line 699 "analisador.y"
	{ConferirAnd(yystack.l_mark[-3].infoexpr, yystack.l_mark[0].infoexpr, &yyval.infoexpr);}
break;
case 111:
#line 700 "analisador.y"
	{}
break;
case 112:
#line 700 "analisador.y"
	{ConferirNot(yystack.l_mark[0].infoexpr, &yyval.infoexpr);}
break;
case 114:
#line 701 "analisador.y"
	{switch(yystack.l_mark[0].atr){
									case MAIOR: break;
									case MENOR: break;
									case MAIG: break;
									case MENIG: break;
									case IGUAL: break;
									case DIFER: break;
								}
								}
break;
case 115:
#line 709 "analisador.y"
	{ConferirOpRel(yystack.l_mark[-3].infoexpr, yystack.l_mark[-2].atr, yystack.l_mark[0].infoexpr, &yyval.infoexpr);}
break;
case 117:
#line 711 "analisador.y"
	{switch(yystack.l_mark[0].atr){
									case MAIS: break;
									case MENOS: break;
								}
								}
break;
case 118:
#line 715 "analisador.y"
	{ConferirOpAd(yystack.l_mark[-3].infoexpr, yystack.l_mark[-2].atr, yystack.l_mark[0].infoexpr, &yyval.infoexpr);}
break;
case 120:
#line 717 "analisador.y"
	{switch(yystack.l_mark[0].atr){
									case VEZES: break;
									case DIV: break;
									case REST: break;
								}
								}
break;
case 121:
#line 722 "analisador.y"
	{ConferirOpMult(yystack.l_mark[-3].infoexpr, yystack.l_mark[-2].atr, yystack.l_mark[0].infoexpr, &yyval.infoexpr);}
break;
case 122:
#line 723 "analisador.y"
	{
						if  (yystack.l_mark[0].infovar.simb != NULL)  {
               				yystack.l_mark[0].infovar.simb->ref  =  TRUE;
                  			yyval.infoexpr.tipo = yystack.l_mark[0].infovar.simb->tvar;
                  			yyval.infoexpr.opnd = yystack.l_mark[0].infovar.opnd;
                  			if(yystack.l_mark[0].infovar.simb->ponteiro){
                  				yyval.infoexpr.tipo = VAROPND;
                  				yyval.infoexpr.opnd.atr.simb = NovaTemp(yystack.l_mark[0].infovar.simb->tvar);
                  				opndaux.tipo = VAROPND;
                  				opndaux.atr.simb = yystack.l_mark[0].infovar.simb;
                  				GeraQuadrupla(OPCONTAPONT,opndaux,opndidle,yyval.infoexpr.opnd);
                  			}
               			}
				}
break;
case 123:
#line 737 "analisador.y"
	{
						yyval.infoexpr.tipo = INTEGER;
						yyval.infoexpr.opnd.tipo = INTOPND;
						yyval.infoexpr.opnd.atr.valint = yystack.l_mark[0].valint;}
break;
case 124:
#line 741 "analisador.y"
	{
						yyval.infoexpr.tipo = FLOAT;
						yyval.infoexpr.opnd.tipo = REALOPND;
						yyval.infoexpr.opnd.atr.valfloat = yystack.l_mark[0].valreal;}
break;
case 125:
#line 745 "analisador.y"
	{
							yyval.infoexpr.tipo = CHAR;
							yyval.infoexpr.opnd.tipo = CHAROPND;
							yyval.infoexpr.opnd.atr.valchar = yystack.l_mark[0].carac;}
break;
case 126:
#line 749 "analisador.y"
	{
							yyval.infoexpr.tipo = LOGICAL;
							yyval.infoexpr.opnd.tipo = LOGICOPND;
							yyval.infoexpr.opnd.atr.vallogic = 1;}
break;
case 127:
#line 753 "analisador.y"
	{
							yyval.infoexpr.tipo = LOGICAL;
							yyval.infoexpr.opnd.tipo = LOGICOPND;
							yyval.infoexpr.opnd.atr.vallogic = 0;}
break;
case 128:
#line 757 "analisador.y"
	{}
break;
case 129:
#line 757 "analisador.y"
	{
				if (yystack.l_mark[0].infoexpr.tipo != INTEGER && yystack.l_mark[0].infoexpr.tipo != FLOAT && yystack.l_mark[0].infoexpr.tipo != CHAR)
					Incompatibilidade  ("Operando improprio para menos unario");
				if (yystack.l_mark[0].infoexpr.tipo == FLOAT)
					yyval.infoexpr.tipo = FLOAT;
				else
					yyval.infoexpr.tipo = INTEGER;
				yyval.infoexpr.opnd.tipo = VAROPND;
				yyval.infoexpr.opnd.atr.simb = NovaTemp (yyval.infoexpr.tipo);
          		GeraQuadrupla  (OPMENUN, yystack.l_mark[0].infoexpr.opnd, opndidle, yyval.infoexpr.opnd);
		    }
break;
case 130:
#line 768 "analisador.y"
	{}
break;
case 131:
#line 769 "analisador.y"
	{
					yyval.infoexpr.tipo = yystack.l_mark[-1].infoexpr.tipo;
					yyval.infoexpr.opnd = yystack.l_mark[-1].infoexpr.opnd;}
break;
case 132:
#line 773 "analisador.y"
	{yyval.infoexpr.tipo = yystack.l_mark[0].infoexpr.tipo;
				yyval.infoexpr.opnd.tipo = VAROPND;
				yyval.infoexpr.opnd.atr.simb = NovaTemp (yyval.infoexpr.tipo);
				opndaux.tipo = INTOPND;
				opndaux.atr.valint = yystack.l_mark[0].infoexpr.opnd.atr.simb->ndims;
				GeraQuadrupla  (OPCALL, yystack.l_mark[0].infoexpr.opnd, opndaux, yyval.infoexpr.opnd);}
break;
case 133:
#line 779 "analisador.y"
	{
						simb = ProcuraSimb (yystack.l_mark[0].cadeia, escopoatual);
						if (simb == NULL){
							simb = ProcuraSimb (yystack.l_mark[0].cadeia, escopoglobal);
						}
						if (simb == NULL){
							NaoDeclarado (yystack.l_mark[0].cadeia);
						}
						else if (simb->tid != IDVAR)
							TipoInadequado (yystack.l_mark[0].cadeia);
						yyval.infovar.simb = simb;
						if (yyval.infovar.simb != NULL)
               				if (yyval.infovar.simb->array == TRUE)
								Esperado ("Subscrito\(s)");
						yyval.infovar.opnd.tipo = VAROPND;
                  		yyval.infovar.opnd.atr.simb = yyval.infovar.simb;}
break;
case 134:
#line 795 "analisador.y"
	{
	 					
	        			simb = ProcuraSimb (yystack.l_mark[-1].cadeia, escopoatual);
						if (simb == NULL){
							simb = ProcuraSimb (yystack.l_mark[-1].cadeia, escopoglobal);
						}
						if (simb == NULL){
							NaoDeclarado (yystack.l_mark[-1].cadeia);
						}
						else if (simb->tid != IDVAR)
							TipoInadequado (yystack.l_mark[-1].cadeia);
						yyval.infovar.simb = simb;
                  		yyval.infovar.opnd.atr.simb = yyval.infovar.simb;
	         			yyval.simb = simb;
					}
break;
case 135:
#line 810 "analisador.y"
	{
					yyval.infovar.simb = yystack.l_mark[-2].simb;
					yyval.infovar.opnd.tipo = VAROPND;
					yyval.infovar.opnd.atr.simb = NovaTemp (yyval.infovar.simb->tvar);
					yyval.infovar.opnd.atr.simb->ponteiro = TRUE;
					yyval.infovar.opnd.atr.simb->referente = yystack.l_mark[-2].simb;
					if(yystack.l_mark[-2].simb)
						yystack.l_mark[-2].simb->ref = TRUE;
					opndaux.tipo = VAROPND;
					opndaux.atr.simb = yyval.infovar.simb;
					opndaux2.tipo = INTOPND;
					opndaux2.atr.valint = yyval.infovar.simb->ndims;
					GeraQuadrupla(INDEXOP,opndaux,opndaux2,yyval.infovar.opnd);
					if (yyval.infovar.simb != NULL)
						if (yyval.infovar.simb->array == FALSE)
							NaoEsperado ("Subscrito\(s)");
			                 	 else if (yyval.infovar.simb->ndims != yystack.l_mark[-1].nsubscr)
			 				Incompatibilidade ("Numero de subscritos incompativel com declaracao");
			 		yyval.infovar.simb = yyval.infovar.opnd.atr.simb;}
break;
case 136:
#line 830 "analisador.y"
	{ConferirSubscr(yystack.l_mark[0].infoexpr.tipo);
					yyval.nsubscr =1;
					GeraQuadrupla (INDOP, yystack.l_mark[0].infoexpr.opnd, opndidle, opndidle);}
break;
case 137:
#line 834 "analisador.y"
	{}
break;
case 138:
#line 835 "analisador.y"
	{ConferirSubscr(yystack.l_mark[0].infoexpr.tipo);
					yyval.nsubscr =yystack.l_mark[-3].nsubscr+1;
					GeraQuadrupla (INDOP, yystack.l_mark[0].infoexpr.opnd, opndidle, opndidle);}
break;
case 139:
#line 839 "analisador.y"
	{
					VerificarExistencia(yystack.l_mark[-2].cadeia);
					VerificarTamanhoSemParametros(simb);
					VerificarTipo(simb, IDFUNC);
					yyval.simb = simb;
					if(simb){
						yyval.infoexpr.tipo=simb->tvar;
					}
					yyval.infoexpr.opnd.tipo = FUNCOPND;
					yyval.infoexpr.opnd.atr.simb = simb;
					VerificarRecursividade(escopoatual,simb);}
break;
case 140:
#line 851 "analisador.y"
	{
				VerificarExistencia(yystack.l_mark[-1].cadeia);
				ultimachamada=simb;
				yyval.simb = simb;}
break;
case 141:
#line 855 "analisador.y"
	{
				VerificarTamanho(yystack.l_mark[-2].simb,yystack.l_mark[-1].nsubscr);
				VerificarTipo(yystack.l_mark[-2].simb, IDFUNC);
				VerificarRecursividade(escopoatual,yystack.l_mark[-2].simb);
				if(yystack.l_mark[-2].simb)
					yyval.infoexpr.tipo=yystack.l_mark[-2].simb->tvar;
				yyval.infoexpr.opnd.tipo = FUNCOPND;
				yyval.infoexpr.opnd.atr.simb = yystack.l_mark[-2].simb;
			}
break;
#line 3205 "y.tab.c"
    }
    yystack.s_mark -= yym;
    yystate = *yystack.s_mark;
    yystack.l_mark -= yym;
    yym = yylhs[yyn];
    if (yystate == 0 && yym == 0)
    {
#if YYDEBUG
        if (yydebug)
            printf("%sdebug: after reduction, shifting from state 0 to\
 state %d\n", YYPREFIX, YYFINAL);
#endif
        yystate = YYFINAL;
        *++yystack.s_mark = YYFINAL;
        *++yystack.l_mark = yyval;
        if (yychar < 0)
        {
            if ((yychar = YYLEX) < 0) yychar = 0;
#if YYDEBUG
            if (yydebug)
            {
                yys = 0;
                if (yychar <= YYMAXTOKEN) yys = yyname[yychar];
                if (!yys) yys = "illegal-symbol";
                printf("%sdebug: state %d, reading %d (%s)\n",
                        YYPREFIX, YYFINAL, yychar, yys);
            }
#endif
        }
        if (yychar == 0) goto yyaccept;
        goto yyloop;
    }
    if ((yyn = yygindex[yym]) && (yyn += yystate) >= 0 &&
            yyn <= YYTABLESIZE && yycheck[yyn] == yystate)
        yystate = yytable[yyn];
    else
        yystate = yydgoto[yym];
#if YYDEBUG
    if (yydebug)
        printf("%sdebug: after reduction, shifting from state %d \
to state %d\n", YYPREFIX, *yystack.s_mark, yystate);
#endif
    if (yystack.s_mark >= yystack.s_last && yygrowstack(&yystack))
    {
        goto yyoverflow;
    }
    *++yystack.s_mark = (short) yystate;
    *++yystack.l_mark = yyval;
    goto yyloop;

yyoverflow:
    yyerror("yacc stack overflow");

yyabort:
    yyfreestack(&yystack);
    return (1);

yyaccept:
    yyfreestack(&yystack);
    return (0);
}
