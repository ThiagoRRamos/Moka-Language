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

#line 209 "analisador.y"
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
#line 245 "y.tab.c"

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
   29,   31,   31,   21,   21,   33,   34,   34,   35,   37,
   35,   36,   38,   36,   17,   39,   17,   16,   41,   22,
   40,   40,   42,   42,   42,   42,   42,   42,   42,   42,
   42,   42,   42,   52,   53,   54,   56,   43,   55,   57,
   55,   58,   59,   60,   44,   61,   62,   63,   45,   64,
   65,   66,   67,   68,   69,   70,   46,   71,   47,    4,
   72,    4,   73,   48,    5,   74,    5,   13,   13,   50,
   75,   50,   51,   76,   77,   51,   78,   79,   49,    1,
   80,    1,    6,   81,    6,    7,   82,    7,    8,   83,
    8,    9,   84,    9,   10,   85,   10,   11,   86,   11,
   12,   12,   12,   12,   12,   12,   87,   12,   88,   12,
   12,    3,   89,    3,    2,   90,    2,   14,   91,   14,
};
static const short yylen[] = {                            2,
    0,    0,    0,    9,    0,    0,    3,    1,    2,    0,
    4,    1,    1,    1,    1,    1,    0,    4,    1,    0,
    5,    1,    3,    0,    2,    3,    1,    1,    6,    0,
    8,    5,    0,    7,    1,    0,    4,    2,    0,    4,
    0,    2,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    1,    0,    0,    0,    0,   10,    0,    0,
    3,    0,    0,    0,    8,    0,    0,    0,   10,    0,
    0,    0,    0,    0,    0,    0,   20,    0,    6,    1,
    0,    4,    0,    6,    1,    0,    4,    1,    1,    5,
    0,    7,    2,    0,    0,    5,    0,    0,    6,    1,
    0,    4,    1,    0,    4,    1,    0,    4,    1,    0,
    3,    1,    0,    4,    1,    0,    4,    1,    0,    4,
    1,    1,    1,    1,    1,    1,    0,    3,    0,    4,
    1,    1,    0,    5,    1,    0,    4,    3,    0,    5,
};
static const short yydefred[] = {                         1,
    0,    0,    0,    0,    2,    0,    6,   24,    0,    0,
   14,   12,   13,   15,   10,    0,    8,    0,    0,    0,
   25,    0,   27,   28,    0,    9,    0,    0,   39,    4,
    0,    0,    0,   16,    0,    0,   41,   26,   20,   11,
   17,    0,    0,    0,    0,    0,    0,    0,    0,   32,
    0,   35,    0,   40,   53,    0,    0,    0,    0,   70,
   66,    0,    0,   43,   42,   44,   45,   46,   47,   48,
   49,   50,   51,   52,    0,   22,    0,   18,   29,    0,
   38,    0,   36,    0,   62,   83,   78,    0,    0,   93,
    0,   54,    0,   98,   21,    0,    0,   34,    0,    0,
    0,    0,    0,    0,    0,    0,  122,  123,  124,  129,
  127,  110,  125,  126,  121,    0,    0,  106,  109,    0,
    0,  118,  131,    0,  133,    0,   23,   31,   37,    0,
    0,    0,   88,    0,    0,   85,   80,    0,   71,   67,
    0,    0,    0,    0,  104,    0,  107,  116,  113,  119,
    0,    0,    0,   90,    0,    0,    0,    0,   86,    0,
   81,    0,    0,  138,    0,    0,  128,  111,    0,   96,
    0,    0,    0,    0,    0,    0,    0,    0,    0,  101,
   64,   84,    0,   79,    0,    0,    0,    0,  130,    0,
  108,    0,    0,  120,   56,  134,  136,   99,   92,    0,
    0,   87,   82,    0,    0,  140,    0,    0,    0,   65,
    0,    0,   57,    0,   73,    0,    0,    0,   69,   60,
   58,    0,    0,   74,   61,    0,    0,   75,    0,    0,
   76,    0,   77,
};
static const short yydgoto[] = {                          1,
  155,  176,  115,  138,  134,  135,  117,  118,  119,  120,
  121,  122,  136,  123,   51,   52,   53,    2,    8,    6,
   10,   64,   20,   16,    9,   17,   33,   25,   34,   47,
   77,   46,   21,   22,   23,   24,   49,   44,   99,   45,
   37,   65,   66,   67,   68,   69,   70,   71,   72,   73,
   74,  124,  175,  207,  221,  217,  223,  101,  157,  201,
   89,  163,  212,   88,  162,  211,  218,  226,  229,  232,
  103,  185,  102,  183,  131,   91,  146,   75,  126,  200,
  169,  171,  144,  173,  172,  174,  143,  142,  152,  208,
  165,
};
static const short yysindex[] = {                         0,
    0, -279, -243, -220,    0, -230,    0,    0,  -68, -109,
    0,    0,    0,    0,    0,  -68,    0,  -68, -158, -156,
    0, -230,    0,    0, -142,    0, -136, -132,    0,    0,
 -156, -128, -203,    0,  -97, -110,    0,    0,    0,    0,
    0,  -80, -112,  -68, -211,  -61, -142,  -66,  -68,    0,
  -40,    0, -252,    0,    0,  -35,  -51,  -39,  -36,    0,
    0,  -49,  -34,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,  -31,    0, -263,    0,    0, -251,
    0,  -30,    0,  -25,    0,    0,    0,  -21, -168,    0,
 -114,    0,  -22,    0,    0,   -9,  -18,    0,  -68,  -11,
 -114, -127,  -31,  -31,  -26,  -70,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -20,  -16,    0,    0,  -64,
   -2,    0,    0, -114,    0,  -12,    0,    0,    0,   -8,
 -114,  -20,    0, -250,  -20,    0,    0, -249,    0,    0,
   -4, -114, -194, -194,    0,   -7,    0,    0,    0,    0,
  -20, -194, -114,    0, -212,  -20,   -1,   -5,    0,   -3,
    0,    1,    2,    0, -114, -258,    0,    0, -114,    0,
 -114, -194, -194, -194,    3, -177,   10, -266,    4,    0,
    0,    0, -127,    0,  -31, -114, -114, -178,    0,  -16,
    0,   -2,   10,    0,    0,    0,    0,    0,    0, -114,
 -168,    0,    0,  -20,  -20,    0, -168, -194,  -20,    0,
    5,    6,    0,   10,    0,    7,  -24, -114,    0,    0,
    0, -106, -168,    0,    0,  -31,    8,    0, -114, -139,
    0, -168,    0,
};
static const short yyrindex[] = {                         0,
    0,    0,    0,    0,    0, -264,    0,    0,    0,   12,
    0,    0,    0,    0,    0, -261,    0,    0,    0,    0,
    0,   14,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -169,    0,    0,    0,  -42,    0,    0,    0,    0,
    0,  -27,    0,    0,   18,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0, -101,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,   18,    0,
    0,    0, -205,    0,    0,    0,    0,    0,    0,  -87,
    0,    0,    0,    0,    0,  -58,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   11, -227,    0,    0, -223,
  -43,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,   17,    0,    0, -174,    0,    0,    0,    0,    0,
  -74,    0,    0,    0,    0,    0,    0,    0,    0,    0,
   19,    0,    0,    0,    0, -160,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0, -130,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -181,
    0,  -28, -182,    0,    0,    0,    0,    0,    0,    0,
   18,    0,    0,   13,   20,    0,   18,    0, -159,    0,
    0,    0,    0, -107,    0,    0, -247,    0,    0,    0,
    0,    0,   18,    0,    0,    0,    0,    0,    0,    0,
    0,   18,    0,
};
static const short yygindex[] = {                         0,
  113,    0,  -75,    0,    0,  -89,  119,  118,  146, -149,
  120, -138,  108,    0,  175,  194,  245,    0,  273,    0,
    0,  116,    0,    0,    0,  280,    0,    0,  250,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  -88,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,
};
#define YYTABLESIZE 297
static const short yytable[] = {                         94,
  105,  116,  177,    5,  167,  198,    7,   95,  189,   59,
  145,  132,    3,    4,   82,   97,  158,  160,  145,   96,
   59,   59,    5,  193,   59,    7,    5,  137,  139,    7,
   83,   83,  159,  161,  151,  194,   59,   59,   59,  103,
   59,  156,   59,  112,  103,   59,   59,   59,  112,  103,
  112,    5,  166,  112,  179,  103,   29,   54,  214,  112,
   55,  132,  106,  178,  107,  108,    7,  132,   40,  109,
  180,  110,   56,   57,   58,  156,   59,  132,   60,   41,
  111,   61,   62,   63,  114,  105,  113,  114,  206,  114,
  105,  114,   89,  196,  114,  105,  204,  205,   28,   29,
  114,  105,   19,   55,  180,  197,  100,  102,   89,  203,
  209,   29,  210,   19,   32,   56,   57,   58,  213,   59,
   35,   60,  100,  102,   61,   62,   63,  231,  222,  106,
  133,  107,  108,   36,  225,   30,  109,  145,  110,  230,
  135,   39,  106,  233,  107,  108,   38,  111,  112,  109,
  227,  110,  135,  113,  114,   94,   43,   94,   94,   50,
  111,  112,   94,  137,   94,  224,  113,  114,   42,   91,
  145,   91,   91,   94,   94,  137,   91,   18,   91,   94,
   94,   19,  139,   15,  139,  139,   48,   91,   91,  139,
   15,  139,   27,   91,   91,  141,  148,   76,  149,  125,
  139,  139,  132,  132,  132,   79,  139,  139,  132,   11,
   12,   13,  132,  132,   85,  132,   81,  115,  132,  115,
   14,   84,   90,  115,  132,   93,   86,  115,  115,   87,
  115,   92,  117,  115,  117,   33,   33,   33,  117,  115,
  100,   98,  117,  117,  104,  117,   33,  125,  117,  127,
   30,   30,   30,  128,  117,  130,  145,  147,  140,  150,
  153,   30,  164,  154,  170,  181,  182,  187,  184,  195,
  148,  220,  216,  186,   97,  199,  215,  188,  219,    3,
  228,    5,   95,   63,   72,   55,   68,  190,  191,  168,
  202,  192,  129,   80,   31,   26,   78,
};
static const short yycheck[] = {                         75,
   89,   91,  152,  268,  143,  272,  268,  271,  267,  257,
  277,  101,  292,  257,  267,  267,  267,  267,  277,  283,
  268,  269,  287,  173,  272,  287,  291,  103,  104,  291,
  283,  283,  283,  283,  124,  174,  284,  285,  286,  267,
  288,  131,  290,  267,  272,  293,  294,  295,  272,  277,
  274,  272,  142,  277,  267,  283,  268,  269,  208,  283,
  272,  267,  257,  153,  259,  260,  297,  273,  272,  264,
  283,  266,  284,  285,  286,  165,  288,  283,  290,  283,
  275,  293,  294,  295,  267,  267,  281,  282,  267,  272,
  272,  274,  267,  271,  277,  277,  186,  187,  257,  268,
  283,  283,  272,  272,  283,  283,  267,  267,  283,  185,
  200,  268,  201,  283,  257,  284,  285,  286,  207,  288,
  257,  290,  283,  283,  293,  294,  295,  267,  218,  257,
  258,  259,  260,  266,  223,   20,  264,  277,  266,  229,
  271,  270,  257,  232,  259,  260,   31,  275,  276,  264,
  226,  266,  283,  281,  282,  257,  267,  259,  260,  272,
  275,  276,  264,  271,  266,  272,  281,  282,  266,  257,
  277,  259,  260,  275,  276,  283,  264,  287,  266,  281,
  282,  291,  257,    9,  259,  260,  267,  275,  276,  264,
   16,  266,   18,  281,  282,  266,  261,  259,  263,  270,
  275,  276,  261,  262,  263,  272,  281,  282,  267,  278,
  279,  280,  271,  272,  266,  274,  257,  261,  277,  263,
  289,  257,  272,  267,  283,  257,  266,  271,  272,  266,
  274,  266,  261,  277,  263,  278,  279,  280,  267,  283,
  266,  272,  271,  272,  266,  274,  289,  270,  277,  259,
  278,  279,  280,  272,  283,  267,  277,  274,  285,  262,
  273,  289,  267,  272,  272,  267,  272,  266,  272,  267,
  261,  296,  267,  273,  257,  272,  272,  165,  272,  268,
  273,  268,  272,  267,  272,  267,  267,  169,  171,  144,
  183,  172,   99,   49,   22,   16,   47,
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
"DeclSubProg : Cabecalho Decls CmdComp",
"Cabecalho : CabFunc",
"Cabecalho : CabProc",
"CabFunc : FUNCAO Tipo ID ABPAR FPAR PVIRG",
"$$8 :",
"CabFunc : FUNCAO Tipo ID ABPAR $$8 ListParam FPAR PVIRG",
"CabProc : PROCEDIMENTO ID ABPAR FPAR PVIRG",
"$$9 :",
"CabProc : PROCEDIMENTO ID ABPAR $$9 ListParam FPAR PVIRG",
"ListParam : Parametro",
"$$10 :",
"ListParam : ListParam VIRG $$10 Parametro",
"Parametro : Tipo ID",
"$$11 :",
"CmdComp : ABCHAV $$11 ListCmd FCHAV",
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
"$$12 :",
"$$13 :",
"$$14 :",
"$$15 :",
"CmdSe : SE ABPAR $$12 Expressao $$13 FPAR $$14 Comando $$15 CmdSenao",
"CmdSenao :",
"$$16 :",
"CmdSenao : SENAO $$16 Comando",
"$$17 :",
"$$18 :",
"$$19 :",
"CmdEnquanto : ENQUANTO ABPAR $$17 Expressao $$18 FPAR $$19 Comando",
"$$20 :",
"$$21 :",
"$$22 :",
"CmdRepetir : REPETIR $$20 Comando ENQUANTO $$21 ABPAR Expressao $$22 FPAR PVIRG",
"$$23 :",
"$$24 :",
"$$25 :",
"$$26 :",
"$$27 :",
"$$28 :",
"$$29 :",
"CmdPara : PARA $$23 ABPAR Variavel $$24 ATRIB Expressao $$25 PVIRG $$26 Expressao PVIRG $$27 Variavel ATRIB $$28 Expressao FPAR $$29 Comando",
"$$30 :",
"CmdLer : LER ABPAR $$30 ListVar FPAR PVIRG",
"ListVar : Variavel",
"$$31 :",
"ListVar : ListVar VIRG $$31 Variavel",
"$$32 :",
"CmdEscrever : ESCREVER ABPAR $$32 ListEscr FPAR PVIRG",
"ListEscr : ElemEscr",
"$$33 :",
"ListEscr : ListEscr VIRG $$33 ElemEscr",
"ElemEscr : CADEIA",
"ElemEscr : Expressao",
"ChamaProc : CHAMAR ID ABPAR FPAR PVIRG",
"$$34 :",
"ChamaProc : CHAMAR ID ABPAR $$34 ListExpr FPAR PVIRG",
"CmdRetornar : RETORNAR PVIRG",
"$$35 :",
"$$36 :",
"CmdRetornar : RETORNAR $$35 Expressao $$36 PVIRG",
"$$37 :",
"$$38 :",
"CmdAtrib : $$37 Variavel $$38 ATRIB Expressao PVIRG",
"ListExpr : Expressao",
"$$39 :",
"ListExpr : ListExpr VIRG $$39 Expressao",
"Expressao : ExprAux1",
"$$40 :",
"Expressao : Expressao OR $$40 ExprAux1",
"ExprAux1 : ExprAux2",
"$$41 :",
"ExprAux1 : ExprAux1 AND $$41 ExprAux2",
"ExprAux2 : ExprAux3",
"$$42 :",
"ExprAux2 : NOT $$42 ExprAux3",
"ExprAux3 : ExprAux4",
"$$43 :",
"ExprAux3 : ExprAux4 OPREL $$43 ExprAux4",
"ExprAux4 : Termo",
"$$44 :",
"ExprAux4 : ExprAux4 OPAD $$44 Termo",
"Termo : Fator",
"$$45 :",
"Termo : Termo OPMULT $$45 Fator",
"Fator : Variavel",
"Fator : CTINT",
"Fator : CTREAL",
"Fator : CTCARAC",
"Fator : VERDADE",
"Fator : FALSO",
"$$46 :",
"Fator : NEG $$46 Fator",
"$$47 :",
"Fator : ABPAR $$47 Expressao FPAR",
"Fator : ChamaFunc",
"Variavel : ID",
"$$48 :",
"Variavel : ID ABCOL $$48 ListSubscr FCOL",
"ListSubscr : ExprAux4",
"$$49 :",
"ListSubscr : ListSubscr VIRG $$49 ExprAux4",
"ChamaFunc : ID ABPAR FPAR",
"$$50 :",
"ChamaFunc : ID ABPAR $$50 ListExpr FPAR",

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
#line 688 "analisador.y"
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



#line 1208 "y.tab.c"

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
#line 271 "analisador.y"
	{InicTabSimb ();
				InicCodIntermed ();
				numtemp = 0;}
break;
case 2:
#line 274 "analisador.y"
	{printf("programa %s;\n", yystack.l_mark[-1].cadeia);
				escopoatual = InsereSimb (yystack.l_mark[-1].cadeia, IDPROG, NOTVAR, NULL);
				InicCodIntermMod (escopoatual);
				opnd1.tipo = MODOPND;
          		opnd1.atr.modulo = modcorrente;
          		yyval.quad = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);
          		escopoglobal=escopoatual;}
break;
case 3:
#line 282 "analisador.y"
	{
          			modcorrente = codintermed->prox;
          			quadcorrente = modcorrente->listquad->prox;
          		}
break;
case 4:
#line 287 "analisador.y"
	{
          			yyval.quad = GeraQuadrupla (OPEXIT, opndidle, opndidle, opndidle);
          			/*GeraQuadrupla (OPEXIT, opndidle, opndidle, opndidle);*/
          			ImprimeTabSimb ();
          			VerificaInicRef();
          			ImprimeQuadruplas ();}
break;
case 6:
#line 294 "analisador.y"
	{printf("\nvar\n");tab++;}
break;
case 7:
#line 294 "analisador.y"
	{tab--;printf("\n");}
break;
case 10:
#line 298 "analisador.y"
	{tabular(); printf("%s ", yystack.l_mark[0].cadeia);}
break;
case 11:
#line 298 "analisador.y"
	{printf(";\n");}
break;
case 12:
#line 300 "analisador.y"
	{strcpy(yyval.cadeia, "int"); tipocorrente = INTEGER;}
break;
case 13:
#line 301 "analisador.y"
	{strcpy(yyval.cadeia, "real"); tipocorrente = FLOAT;}
break;
case 14:
#line 302 "analisador.y"
	{strcpy(yyval.cadeia, "carac"); tipocorrente = CHAR;}
break;
case 15:
#line 303 "analisador.y"
	{strcpy(yyval.cadeia, "logic"); tipocorrente = LOGICAL;}
break;
case 17:
#line 304 "analisador.y"
	{printf(", ");}
break;
case 19:
#line 307 "analisador.y"
	{printf("%s", yystack.l_mark[0].cadeia);
				AnalisarInsercaoSimb(yystack.l_mark[0].cadeia, IDVAR, FALSE, escopoatual);}
break;
case 20:
#line 310 "analisador.y"
	{printf("%s[", yystack.l_mark[-1].cadeia);
				AnalisarInsercaoSimb(yystack.l_mark[-1].cadeia, IDVAR, TRUE, escopoatual);}
break;
case 21:
#line 311 "analisador.y"
	{printf("]");}
break;
case 22:
#line 313 "analisador.y"
	{printf("%d", yystack.l_mark[0].valint);ConferirValorDim(yystack.l_mark[0].valint);}
break;
case 23:
#line 313 "analisador.y"
	{printf(", %d", yystack.l_mark[0].valint); ConferirValorDim(yystack.l_mark[0].valint);}
break;
case 26:
#line 317 "analisador.y"
	{printf("\n"); escopoatual = escopoglobal;}
break;
case 29:
#line 322 "analisador.y"
	{printf("funcao %s %s();\n", yystack.l_mark[-4].cadeia, yystack.l_mark[-3].cadeia);
					AnalisarInsercaoSimb (yystack.l_mark[-3].cadeia, IDFUNC, FALSE, escopoglobal);
					escopoatual=simb;
					InicCodIntermMod (escopoatual);
					opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			yyval.quad = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);}
break;
case 30:
#line 330 "analisador.y"
	{printf("funcao %s %s(", yystack.l_mark[-2].cadeia, yystack.l_mark[-1].cadeia);
					AnalisarInsercaoSimb (yystack.l_mark[-1].cadeia, IDFUNC, TRUE, escopoglobal);
					escopoatual=simb;
					InicCodIntermMod (escopoatual);
					opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			yyval.quad = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);}
break;
case 31:
#line 336 "analisador.y"
	{printf(");\n");}
break;
case 32:
#line 339 "analisador.y"
	{printf("procedimento %s();\n", yystack.l_mark[-3].cadeia);
					AnalisarInsercaoSimb (yystack.l_mark[-3].cadeia, IDPROC, FALSE, escopoglobal);
					escopoatual=simb;
					InicCodIntermMod (escopoatual);
					opnd1.tipo = MODOPND;
          			opnd1.atr.modulo = modcorrente;
          			yyval.quad = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);}
break;
case 33:
#line 347 "analisador.y"
	{printf("procedimento %s(", yystack.l_mark[-1].cadeia);
			AnalisarInsercaoSimb (yystack.l_mark[-1].cadeia, IDPROC, TRUE, escopoglobal);
			escopoatual=simb;
			InicCodIntermMod (escopoatual);
			opnd1.tipo = MODOPND;
      		opnd1.atr.modulo = modcorrente;
      		yyval.quad = GeraQuadrupla (OPENMOD, opnd1, opndidle, opndidle);}
break;
case 34:
#line 353 "analisador.y"
	{printf(");\n");}
break;
case 36:
#line 355 "analisador.y"
	{printf(", ");}
break;
case 38:
#line 357 "analisador.y"
	{AnalisarInsercaoSimb(yystack.l_mark[0].cadeia, IDVAR, FALSE, escopoatual); yyval.simb = simb; yyval.simb->inic = TRUE; printf("%s %s", yystack.l_mark[-1].cadeia, yystack.l_mark[0].cadeia); ConferirValorParam(tipocorrente);}
break;
case 39:
#line 360 "analisador.y"
	{tabular ();
					printf ("\{\n");
					tab++;}
break;
case 40:
#line 363 "analisador.y"
	{tab--;
					tabular ();
					printf ("}\n");}
break;
case 54:
#line 374 "analisador.y"
	{tabular();
						tab++;
						printf("se(");}
break;
case 55:
#line 378 "analisador.y"
	{ConferirExpLogica(yystack.l_mark[0].infoexpr.tipo);
						opndaux.tipo = ROTOPND;
						yyval.quad = GeraQuadrupla (OPJF, yystack.l_mark[0].infoexpr.opnd, opndidle, opndaux);
					}
break;
case 56:
#line 382 "analisador.y"
	{printf(")\n");}
break;
case 57:
#line 383 "analisador.y"
	{tab--;
						yyval.quad = quadcorrente;
						yystack.l_mark[-3].quad->result.atr.rotulo = GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}
break;
case 58:
#line 386 "analisador.y"
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
case 60:
#line 397 "analisador.y"
	{tabular();
							tab++;
							printf("senao\n");
							opndaux.tipo = ROTOPND;
							yyval.quad = GeraQuadrupla (OPJUMP, opndidle, opndidle,opndaux);}
break;
case 61:
#line 403 "analisador.y"
	{tab--;
							yystack.l_mark[-1].quad->result.atr.rotulo =GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}
break;
case 62:
#line 407 "analisador.y"
	{tabular();
						printf("enquanto (");
						yyval.quad = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);}
break;
case 63:
#line 411 "analisador.y"
	{ConferirExpLogica(yystack.l_mark[0].infoexpr.tipo);
					opndaux.tipo = ROTOPND;
					yyval.quad = GeraQuadrupla(OPJF, yystack.l_mark[0].infoexpr.opnd, opndidle, opndaux);}
break;
case 64:
#line 415 "analisador.y"
	{printf(")\n");tab++;}
break;
case 65:
#line 417 "analisador.y"
	{tab--;
					yyval.quad = quadcorrente;
					opndaux.tipo = ROTOPND;
					yyval.quad = GeraQuadrupla (OPJUMP, opndidle, opndidle, opndaux);
					yyval.quad->result.atr.rotulo = yystack.l_mark[-5].quad;
					yystack.l_mark[-3].quad->result.atr.rotulo = GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}
break;
case 66:
#line 425 "analisador.y"
	{tabular();
						printf("repetir \n");
						tab++;
						yyval.quad = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);}
break;
case 67:
#line 431 "analisador.y"
	{tab--; tabular(); printf("enquanto("); }
break;
case 68:
#line 434 "analisador.y"
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
case 69:
#line 442 "analisador.y"
	{printf(");\n"); }
break;
case 70:
#line 445 "analisador.y"
	{tabular();
					tab++;
					printf("para(");}
break;
case 71:
#line 448 "analisador.y"
	{printf(" := ");
					ConferirPara(yystack.l_mark[0].infovar.simb);
					InicializarEReferenciar(yystack.l_mark[0].infovar.simb);}
break;
case 72:
#line 451 "analisador.y"
	{ConferirAtribuicao(yystack.l_mark[-3].infovar.simb, yystack.l_mark[0].infoexpr.tipo);
				 GeraQuadrupla (OPATRIB, yystack.l_mark[0].infoexpr.opnd, opndidle, yystack.l_mark[-3].infovar.opnd);
				}
break;
case 73:
#line 454 "analisador.y"
	{printf("; ");
				yyval.quad = GeraQuadrupla (NOP, opndidle, opndidle, opndidle);}
break;
case 74:
#line 456 "analisador.y"
	{printf("; ");
					ConferirExpLogica(yystack.l_mark[-1].infoexpr.tipo);
					opndaux.tipo = ROTOPND;
					yyval.quad = GeraQuadrupla(OPJF, yystack.l_mark[-1].infoexpr.opnd, opndidle, opndaux);}
break;
case 75:
#line 460 "analisador.y"
	{printf(" := ");
					MesmaVariavel(yystack.l_mark[-11].infovar.simb,yystack.l_mark[-1].infovar.simb);
					yyval.quad = quadcorrente;
				}
break;
case 76:
#line 464 "analisador.y"
	{printf(")\n");
				opndaux.tipo = ROTOPND;
				}
break;
case 77:
#line 468 "analisador.y"
	{tab--;
					quadaux = yystack.l_mark[-4].quad->prox;
					yystack.l_mark[-4].quad->prox = quadaux->prox;
					quadcorrente->prox = quadaux;
					quadcorrente = quadaux;
					yyval.quad = GeraQuadrupla (OPATRIB, yystack.l_mark[-3].infoexpr.opnd, opndidle, yystack.l_mark[-6].infovar.opnd);
					quadaux = GeraQuadrupla (OPJUMP, opndidle, opndidle, opndaux);
					quadaux->result.atr.rotulo = yystack.l_mark[-10].quad;
					quadaux = GeraQuadrupla(NOP, opndidle, opndidle, opndidle);
					yystack.l_mark[-7].quad->result.atr.rotulo = quadaux;}
break;
case 78:
#line 479 "analisador.y"
	{tabular();printf("ler(");}
break;
case 79:
#line 480 "analisador.y"
	{printf(");\n");
					opnd1.tipo = INTOPND;
					opnd1.atr.valint = yystack.l_mark[-2].nargs;
					GeraQuadrupla (OPREAD, opnd1, opndidle, opndidle);}
break;
case 80:
#line 485 "analisador.y"
	{InicializarEReferenciar(yystack.l_mark[0].infovar.simb);
							yyval.nargs = 1;	
							GeraQuadrupla (PARAM, yystack.l_mark[0].infovar.opnd, opndidle, opndidle);}
break;
case 81:
#line 488 "analisador.y"
	{printf(", ");}
break;
case 82:
#line 489 "analisador.y"
	{InicializarEReferenciar(yystack.l_mark[0].infovar.simb);
					yyval.nargs = yystack.l_mark[-3].nargs + 1;
					GeraQuadrupla (PARAM, yystack.l_mark[0].infovar.opnd, opndidle, opndidle);}
break;
case 83:
#line 493 "analisador.y"
	{tabular();printf("escrever(");}
break;
case 84:
#line 494 "analisador.y"
	{printf(");\n");
						opnd1.tipo = INTOPND;
						opnd1.atr.valint = yystack.l_mark[-2].nargs;
						GeraQuadrupla (OPWRITE, opnd1, opndidle, opndidle);}
break;
case 85:
#line 500 "analisador.y"
	{
						yyval.nargs = 1;
						GeraQuadrupla (PARAM, yystack.l_mark[0].infoexpr.opnd, opndidle, opndidle);
					}
break;
case 86:
#line 504 "analisador.y"
	{printf(", ");}
break;
case 87:
#line 504 "analisador.y"
	{
							yyval.nargs = yystack.l_mark[-3].nargs + 1;
							GeraQuadrupla (PARAM, yystack.l_mark[0].infoexpr.opnd, opndidle, opndidle);
						}
break;
case 88:
#line 509 "analisador.y"
	{printf("\"%s\"", yystack.l_mark[0].cadeia);
						yyval.infoexpr.opnd.tipo = CADOPND;
						yyval.infoexpr.opnd.atr.valcad = malloc (strlen(yystack.l_mark[0].cadeia) + 1);
						strcpy (yyval.infoexpr.opnd.atr.valcad, yystack.l_mark[0].cadeia);}
break;
case 90:
#line 515 "analisador.y"
	{tabular();
						printf("chamar %s();\n", yystack.l_mark[-3].cadeia);
						VerificarExistencia(yystack.l_mark[-3].cadeia);
						VerificarTamanhoSemParametros(simb);
						VerificarTipo(simb, IDPROC);}
break;
case 91:
#line 521 "analisador.y"
	{tabular();
						printf("chamar %s(", yystack.l_mark[-1].cadeia);
							VerificarExistencia(yystack.l_mark[-1].cadeia);
							ultimachamada=simb;}
break;
case 92:
#line 525 "analisador.y"
	{printf(");\n");
						VerificarTamanho(simb,yystack.l_mark[-2].nsubscr);
						VerificarTipo(simb, IDPROC);
						opndaux.tipo = INTOPND;
						opndaux.atr.valint = simb->ndims;
						opndaux2.tipo = FUNCOPND;
						opndaux2.atr.simb = simb;
						GeraQuadrupla  (OPCALL, opndaux2, opndaux, opndidle);}
break;
case 93:
#line 534 "analisador.y"
	{tabular ();
						printf("retornar;\n");
						ConferirRetornoProcedimento(escopoatual);
						GeraQuadrupla  (OPRETURN, opndidle, opndidle, opndidle);}
break;
case 94:
#line 539 "analisador.y"
	{tabular();
						printf("retornar ");}
break;
case 95:
#line 541 "analisador.y"
	{ConferirRetornoFuncao(escopoatual,yystack.l_mark[0].infoexpr.tipo);}
break;
case 96:
#line 542 "analisador.y"
	{printf(";\n");
					GeraQuadrupla  (OPRETURN, yystack.l_mark[-2].infoexpr.opnd, opndidle, opndidle);}
break;
case 97:
#line 544 "analisador.y"
	{tabular ();}
break;
case 98:
#line 545 "analisador.y"
	{if  (yystack.l_mark[0].infovar.simb != NULL)
							yystack.l_mark[0].infovar.simb->inic = yystack.l_mark[0].infovar.simb->ref = TRUE;
							printf(" := ");}
break;
case 99:
#line 548 "analisador.y"
	{printf(";\n");
							ConferirAtribuicao(yystack.l_mark[-4].infovar.simb, yystack.l_mark[-1].infoexpr.tipo);
							GeraQuadrupla (OPATRIB, yystack.l_mark[-1].infoexpr.opnd, opndidle, yystack.l_mark[-4].infovar.opnd);
}
break;
case 100:
#line 553 "analisador.y"
	{yyval.nsubscr = 1;
						checarTipo(1,yystack.l_mark[0].infoexpr.tipo);
						GeraQuadrupla (PARAM, yystack.l_mark[0].infoexpr.opnd, opndidle, opndidle);}
break;
case 101:
#line 556 "analisador.y"
	{printf(", ");}
break;
case 102:
#line 557 "analisador.y"
	{yyval.nsubscr = yyval.nsubscr + 1;
						checarTipo(yyval.nsubscr,yystack.l_mark[0].infoexpr.tipo);
						GeraQuadrupla (PARAM, yystack.l_mark[0].infoexpr.opnd, opndidle, opndidle);}
break;
case 104:
#line 560 "analisador.y"
	{printf(" || ");}
break;
case 105:
#line 560 "analisador.y"
	{ConferirOr(yystack.l_mark[-3].infoexpr, yystack.l_mark[0].infoexpr, &yyval.infoexpr);}
break;
case 107:
#line 561 "analisador.y"
	{printf(" && ");}
break;
case 108:
#line 561 "analisador.y"
	{ConferirAnd(yystack.l_mark[-3].infoexpr, yystack.l_mark[0].infoexpr, &yyval.infoexpr);}
break;
case 110:
#line 562 "analisador.y"
	{printf("!");}
break;
case 111:
#line 562 "analisador.y"
	{ConferirNot(yystack.l_mark[0].infoexpr, &yyval.infoexpr);}
break;
case 113:
#line 563 "analisador.y"
	{switch(yystack.l_mark[0].atr){
									case MAIOR: printf(" > ");break;
									case MENOR: printf(" < ");break;
									case MAIG: printf(" >= ");break;
									case MENIG: printf(" <= ");break;
									case IGUAL: printf(" = ");break;
									case DIFER: printf(" != ");break;
								}
								}
break;
case 114:
#line 571 "analisador.y"
	{ConferirOpRel(yystack.l_mark[-3].infoexpr, yystack.l_mark[-2].atr, yystack.l_mark[0].infoexpr, &yyval.infoexpr);}
break;
case 116:
#line 573 "analisador.y"
	{switch(yystack.l_mark[0].atr){
									case MAIS: printf(" + ");break;
									case MENOS: printf(" - ");break;
								}
								}
break;
case 117:
#line 577 "analisador.y"
	{ConferirOpAd(yystack.l_mark[-3].infoexpr, yystack.l_mark[-2].atr, yystack.l_mark[0].infoexpr, &yyval.infoexpr);}
break;
case 119:
#line 579 "analisador.y"
	{switch(yystack.l_mark[0].atr){
									case VEZES: printf(" * ");break;
									case DIV: printf(" / ");break;
									case REST: printf(" %% ");break;
								}
								}
break;
case 120:
#line 584 "analisador.y"
	{ConferirOpMult(yystack.l_mark[-3].infoexpr, yystack.l_mark[-2].atr, yystack.l_mark[0].infoexpr, &yyval.infoexpr);}
break;
case 121:
#line 585 "analisador.y"
	{
						if  (yystack.l_mark[0].infovar.simb != NULL)  {
               				yystack.l_mark[0].infovar.simb->ref  =  TRUE;
                  			yyval.infoexpr.tipo = yystack.l_mark[0].infovar.simb->tvar;
                  			yyval.infoexpr.opnd = yystack.l_mark[0].infovar.opnd;
               			}
				}
break;
case 122:
#line 592 "analisador.y"
	{printf("%d", yystack.l_mark[0].valint);
						yyval.infoexpr.tipo = INTEGER;
						yyval.infoexpr.opnd.tipo = INTOPND;
						yyval.infoexpr.opnd.atr.valint = yystack.l_mark[0].valint;}
break;
case 123:
#line 596 "analisador.y"
	{printf("%f", yystack.l_mark[0].valreal);
						yyval.infoexpr.tipo = FLOAT;
						yyval.infoexpr.opnd.tipo = REALOPND;
						yyval.infoexpr.opnd.atr.valfloat = yystack.l_mark[0].valreal;}
break;
case 124:
#line 600 "analisador.y"
	{printf("%c", yystack.l_mark[0].carac);
							yyval.infoexpr.tipo = CHAR;
							yyval.infoexpr.opnd.tipo = CHAROPND;
							yyval.infoexpr.opnd.atr.valchar = yystack.l_mark[0].carac;}
break;
case 125:
#line 604 "analisador.y"
	{printf("verdade");
							yyval.infoexpr.tipo = LOGICAL;
							yyval.infoexpr.opnd.tipo = LOGICOPND;
							yyval.infoexpr.opnd.atr.vallogic = 1;}
break;
case 126:
#line 608 "analisador.y"
	{printf("falso");
							yyval.infoexpr.tipo = LOGICAL;
							yyval.infoexpr.opnd.tipo = LOGICOPND;
							yyval.infoexpr.opnd.atr.vallogic = 0;}
break;
case 127:
#line 612 "analisador.y"
	{printf("~");}
break;
case 128:
#line 612 "analisador.y"
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
case 129:
#line 623 "analisador.y"
	{printf("(");}
break;
case 130:
#line 624 "analisador.y"
	{printf(")");
					yyval.infoexpr.tipo = yystack.l_mark[-1].infoexpr.tipo;
					yyval.infoexpr.opnd = yystack.l_mark[-1].infoexpr.opnd;}
break;
case 131:
#line 628 "analisador.y"
	{yyval.infoexpr.tipo = yystack.l_mark[0].infoexpr.tipo;
				yyval.infoexpr.opnd.tipo = VAROPND;
				yyval.infoexpr.opnd.atr.simb = NovaTemp (yyval.infoexpr.tipo);
				opndaux.tipo = INTOPND;
				opndaux.atr.valint = yystack.l_mark[0].infoexpr.opnd.atr.simb->ndims;
				GeraQuadrupla  (OPCALL, yystack.l_mark[0].infoexpr.opnd, opndaux, yyval.infoexpr.opnd);}
break;
case 132:
#line 634 "analisador.y"
	{printf("%s",yystack.l_mark[0].cadeia);
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
case 133:
#line 650 "analisador.y"
	{
	 					printf ("%s[", yystack.l_mark[-1].cadeia);
	        			simb = ProcuraSimb (yystack.l_mark[-1].cadeia, escopoatual);
						if (simb == NULL)   simb = ProcuraSimb (yystack.l_mark[-1].cadeia, escopoglobal);
						if (simb == NULL)   NaoDeclarado (yystack.l_mark[-1].cadeia);
						else if (simb->tid != IDVAR) TipoInadequado (yystack.l_mark[-1].cadeia);
	         			yyval.simb = simb;
					}
break;
case 134:
#line 657 "analisador.y"
	{ printf ("] ");  yyval.infovar.simb = yystack.l_mark[-2].simb;
		if (yyval.infovar.simb != NULL)
			if (yyval.infovar.simb->array == FALSE)
				NaoEsperado ("Subscrito\(s)");
                 	 else if (yyval.infovar.simb->ndims != yystack.l_mark[-1].nsubscr)
 				Incompatibilidade ("Numero de subscritos incompativel com declaracao");}
break;
case 135:
#line 663 "analisador.y"
	{ConferirSubscr(yystack.l_mark[0].infoexpr.tipo);yyval.nsubscr =1;}
break;
case 136:
#line 663 "analisador.y"
	{printf(",");}
break;
case 137:
#line 663 "analisador.y"
	{ConferirSubscr(yystack.l_mark[0].infoexpr.tipo);yyval.nsubscr =yystack.l_mark[-3].nsubscr+1;}
break;
case 138:
#line 665 "analisador.y"
	{printf("%s()", yystack.l_mark[-2].cadeia);
				VerificarExistencia(yystack.l_mark[-2].cadeia);
				VerificarTamanhoSemParametros(simb);
				VerificarTipo(simb, IDFUNC);
				yyval.simb = simb;
				yyval.infoexpr.tipo=simb->tid;
				yyval.infoexpr.opnd.tipo = FUNCOPND;
				yyval.infoexpr.opnd.atr.simb = simb;
				VerificarRecursividade(escopoatual,simb);}
break;
case 139:
#line 675 "analisador.y"
	{printf("%s(", yystack.l_mark[-1].cadeia);
				VerificarExistencia(yystack.l_mark[-1].cadeia);
				ultimachamada=simb;
				yyval.simb = simb;}
break;
case 140:
#line 679 "analisador.y"
	{printf(")");
				VerificarTamanho(yystack.l_mark[-2].simb,yystack.l_mark[-1].nsubscr);
				VerificarTipo(yystack.l_mark[-2].simb, IDFUNC);
				VerificarRecursividade(escopoatual,yystack.l_mark[-2].simb);
				yyval.infoexpr.tipo=yystack.l_mark[-2].simb->tvar;
				yyval.infoexpr.opnd.tipo = FUNCOPND;
				yyval.infoexpr.opnd.atr.simb = yystack.l_mark[-2].simb;
			}
break;
#line 2103 "y.tab.c"
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
