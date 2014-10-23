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

#line 88 "analisador.y"
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
	int tipoexpr;
	int nsubscr;
} YYSTYPE;
#endif /* !YYSTYPE_IS_DECLARED */
#line 122 "y.tab.c"

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
   15,   17,    0,   16,   21,   16,   20,   20,   24,   22,
   12,   12,   12,   12,   23,   26,   23,   25,   28,   25,
   27,   27,   18,   18,   29,   30,   30,   31,   33,   31,
   32,   34,   32,   14,   35,   14,   13,   37,   19,   36,
   36,   38,   38,   38,   38,   38,   38,   38,   38,   38,
   38,   38,   48,   49,   50,   52,   39,   51,   53,   51,
   54,   55,   56,   40,   57,   58,   59,   41,   60,   61,
   62,   63,   64,   65,   66,   42,   68,   43,   67,   69,
   67,   71,   44,   70,   73,   70,   72,   72,   46,   74,
   46,   47,   75,   76,   47,   77,   78,   45,    2,   79,
    2,    5,   80,    5,    6,   81,    6,    7,   82,    7,
    8,   83,    8,    9,   84,    9,   10,   85,   10,   11,
   11,   11,   11,   11,   11,   86,   11,   87,   11,   11,
    4,   88,    4,    3,   89,    3,    1,   90,    1,
};
static const short yylen[] = {                            2,
    0,    0,    8,    0,    0,    3,    1,    2,    0,    4,
    1,    1,    1,    1,    1,    0,    4,    1,    0,    5,
    1,    3,    0,    2,    3,    1,    1,    6,    0,    8,
    5,    0,    7,    1,    0,    4,    2,    0,    4,    0,
    2,    1,    1,    1,    1,    1,    1,    1,    1,    1,
    1,    1,    0,    0,    0,    0,   10,    0,    0,    3,
    0,    0,    0,    8,    0,    0,    0,   10,    0,    0,
    0,    0,    0,    0,    0,   20,    0,    6,    1,    0,
    4,    0,    6,    1,    0,    4,    1,    1,    5,    0,
    7,    2,    0,    0,    5,    0,    0,    6,    1,    0,
    4,    1,    0,    4,    1,    0,    4,    1,    0,    3,
    1,    0,    4,    1,    0,    4,    1,    0,    4,    1,
    1,    1,    1,    1,    1,    0,    3,    0,    4,    1,
    1,    0,    5,    1,    0,    4,    3,    0,    5,
};
static const short yydefred[] = {                         1,
    0,    0,    0,    0,    2,    0,    5,   23,    0,    0,
   13,   11,   12,   14,    9,    0,    7,   38,    0,    0,
    3,   24,    0,   26,   27,    0,    8,   40,    0,    0,
    0,    0,    0,   15,    0,    0,    0,   25,   19,   10,
   16,   39,   52,    0,    0,    0,    0,   69,   65,    0,
    0,   42,   41,   43,   44,   45,   46,   47,   48,   49,
   50,   51,    0,    0,    0,    0,    0,    0,    0,   61,
   82,   77,    0,    0,   92,    0,   53,    0,   97,    0,
    0,   31,    0,   34,    0,   21,    0,   17,    0,    0,
    0,    0,    0,    0,    0,  121,  122,  123,  128,  126,
  109,  124,  125,  130,  120,    0,    0,  105,  108,    0,
    0,  117,    0,  132,    0,   28,    0,   37,    0,   35,
   20,    0,    0,    0,    0,   87,    0,    0,   84,   79,
    0,   70,   66,    0,    0,    0,    0,  103,    0,  106,
  115,  112,  118,    0,    0,    0,    0,   33,    0,   22,
   89,    0,    0,    0,    0,   85,    0,   80,    0,    0,
  137,    0,    0,  127,  110,    0,   95,    0,    0,    0,
    0,    0,    0,    0,    0,   30,   36,    0,  100,   63,
   83,    0,   78,    0,    0,    0,    0,  129,    0,  107,
    0,    0,  119,   55,  133,  135,   98,   91,    0,    0,
   86,   81,    0,    0,  139,    0,    0,    0,   64,    0,
    0,   56,    0,   72,    0,    0,    0,   68,   59,   57,
    0,    0,   73,   60,    0,    0,   74,    0,    0,   75,
    0,   76,
};
static const short yydgoto[] = {                          1,
  104,  152,  173,  105,  127,  107,  108,  109,  110,  111,
  112,   83,   84,   85,    2,    8,    6,   10,   52,   16,
    9,   17,   33,   26,   34,   68,   87,   67,   22,   23,
   24,   25,   81,   66,  149,   35,   28,   53,   54,   55,
   56,   57,   58,   59,   60,   61,   62,  113,  172,  206,
  220,  216,  222,   90,  154,  200,   74,  160,  211,   73,
  159,  210,  217,  225,  228,  231,  131,   92,  184,  128,
   91,  129,  182,  124,   76,  139,   63,  115,  199,  166,
  168,  137,  170,  169,  171,  136,  135,  145,  207,  162,
};
static const short yysindex[] = {                         0,
    0, -279, -243, -215,    0, -234,    0,    0, -172, -245,
    0,    0,    0,    0,    0, -172,    0,    0, -172, -203,
    0,    0, -234,    0,    0, -167,    0,    0, -112, -181,
 -159, -105, -252,    0, -217, -119,  -76,    0,    0,    0,
    0,    0,    0,  -86,  -64,  -58,  -54,    0,    0,  -55,
  -48,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,  -43,  -46,  -49, -172,  -75, -167,  -39,    0,
    0,    0,  -38, -193,    0, -177,    0,  -26,    0,  -40,
 -172,    0,  -24,    0, -262,    0, -259,    0,  -25, -177,
 -249,  -43,  -43,  -44,  -88,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,  -30,  -21,    0,    0, -122,
  -13,    0, -177,    0,  -15,    0, -173,    0,  -12,    0,
    0,    3,   -9, -177,  -30,    0,  -30, -127,    0,    0,
 -123,    0,    0,   -8, -177,  -83,  -83,    0,   -7,    0,
    0,    0,    0,  -30,  -83, -177,   -5,    0, -172,    0,
    0, -114,  -30,   -3,   -4,    0,   -2,    0,   -1,    5,
    0, -177, -218,    0,    0, -177,    0, -177,  -83,  -83,
  -83,    2, -209,   12, -191,    0,    0,    4,    0,    0,
    0, -249,    0,  -43, -177, -177, -108,    0,  -21,    0,
  -13,   12,    0,    0,    0,    0,    0,    0, -177, -193,
    0,    0,  -30,  -30,    0, -193,  -83,  -30,    0,    6,
    7,    0,   12,    0,    8,  -19, -177,    0,    0,    0,
  -84, -193,    0,    0,  -43,    9,    0, -177, -125,    0,
 -193,    0,
};
static const short yyrindex[] = {                         0,
    0,    0,    0,    0,    0, -231,    0,    0,    0,    0,
    0,    0,    0,    0,    0, -221,    0,    0,    0,    0,
    0,    0,   11,    0,    0,    0,    0,    0,    0,    0,
    0, -169,    0,    0,   18,    0,  -28,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0, -144,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,  -23,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   18,    0,    0,    0, -137,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, -109,    0,
    0,    0,    0,    0,  -67,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,   13, -219,    0,    0, -148,
  -52,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,   14,    0,  -80,    0,    0,    0,
    0,    0,    0,  -96,    0,    0,    0,    0,    0,    0,
    0,    0,    0,   16,    0,    0,    0,    0,    0,    0,
    0,    0,  -77,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0, -187,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0, -149,    0,
  -37,  -29,    0,    0,    0,    0,    0,    0,    0,   18,
    0,    0,   15,   17,    0,   18,    0,  -70,    0,    0,
    0,    0,  -82,    0,    0, -250,    0,    0,    0,    0,
    0,   18,    0,    0,    0,    0,    0,    0,    0,    0,
   18,    0,
};
static const short yygindex[] = {                         0,
    0,  124,    0,  -63,  -74,  122,  121,  153, -142,  123,
 -130,  220,  142,  212,    0,  271,    0,    0,   -6,    0,
    0,  279,    0,    0,  228,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,  -73,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
    0,  115,    0,    0,    0,    0,    0,    0,    0,    0,
    0,    0,    0,    0,    0,    0,    0,    0,    0,    0,
};
#define YYTABLESIZE 297
static const short yytable[] = {                         79,
   94,  106,  174,   21,  119,  164,   58,   95,  126,   96,
   97,  121,    3,    4,   98,  125,   99,   58,   58,   40,
  120,   58,   18,  122,   38,  100,  101,  192,  130,  132,
   41,  102,  103,   58,   58,   58,    4,   58,  144,   58,
  193,   19,   58,   58,   58,   20,    6,  102,  188,  153,
   18,   42,  102,   30,   43,    4,    5,  102,  138,    4,
  163,  195,    7,  102,  213,    6,   44,   45,   46,    6,
   47,  175,   48,  196,   18,   49,   50,   51,   43,   95,
  197,   96,   97,  134,   37,  138,   98,  153,   99,   32,
   44,   45,   46,  147,   47,  134,   48,  100,  101,   49,
   50,   51,   18,  102,  103,   11,   12,   13,   18,  120,
  203,  204,   93,   18,   93,   93,   14,  104,  111,   93,
  202,   93,  104,  111,  208,  111,  209,  104,  111,  131,
   93,   93,  212,  104,  111,  131,   93,   93,  141,  155,
  142,  230,  221,  157,   36,  131,   64,   90,  224,   90,
   90,  138,  178,  229,   90,  156,   90,  232,  205,  158,
  138,  226,  138,  138,   39,   90,   90,  138,  179,  138,
   69,   90,   90,   95,  179,   96,   97,  134,  138,  138,
   98,  114,   99,   86,  138,  138,   88,  223,  136,   99,
   65,  100,  138,  131,  131,  131,  101,  102,  103,  131,
  136,   70,   88,  131,  131,   99,  131,   71,  114,  131,
  114,   72,  101,   78,  114,  131,   75,   77,  114,  114,
   80,  114,   82,  116,  114,  116,   89,   93,   15,  116,
  114,  116,  118,  116,  116,   15,  116,  113,   29,  116,
  133,  123,  113,  114,  113,  116,  138,  113,  143,   32,
   32,   32,  140,  113,   29,   29,   29,  146,  161,  148,
   32,  150,  151,  180,  167,   29,  176,  181,  194,  183,
  186,  185,  141,  215,   96,  198,  219,  214,    4,  218,
   62,  227,   54,   67,   94,  187,   71,  189,  190,  165,
  177,  191,  117,   31,   27,   88,  201,
};
static const short yycheck[] = {                         63,
   74,   76,  145,   10,  267,  136,  257,  257,  258,  259,
  260,  271,  292,  257,  264,   90,  266,  268,  269,  272,
  283,  272,  268,  283,   31,  275,  276,  170,   92,   93,
  283,  281,  282,  284,  285,  286,  268,  288,  113,  290,
  171,  287,  293,  294,  295,  291,  268,  267,  267,  124,
  268,  269,  272,  257,  272,  287,  272,  277,  277,  291,
  135,  271,  297,  283,  207,  287,  284,  285,  286,  291,
  288,  146,  290,  283,  268,  293,  294,  295,  272,  257,
  272,  259,  260,  271,  266,  277,  264,  162,  266,  257,
  284,  285,  286,  267,  288,  283,  290,  275,  276,  293,
  294,  295,  272,  281,  282,  278,  279,  280,  268,  283,
  185,  186,  257,  283,  259,  260,  289,  267,  267,  264,
  184,  266,  272,  272,  199,  274,  200,  277,  277,  267,
  275,  276,  206,  283,  283,  273,  281,  282,  261,  267,
  263,  267,  217,  267,  257,  283,  266,  257,  222,  259,
  260,  277,  267,  228,  264,  283,  266,  231,  267,  283,
  257,  225,  259,  260,  270,  275,  276,  264,  283,  266,
  257,  281,  282,  257,  283,  259,  260,  266,  275,  276,
  264,  270,  266,  259,  281,  282,  267,  272,  271,  267,
  267,  275,  277,  261,  262,  263,  267,  281,  282,  267,
  283,  266,  283,  271,  272,  283,  274,  266,  261,  277,
  263,  266,  283,  257,  267,  283,  272,  266,  271,  272,
  267,  274,  272,  261,  277,  263,  266,  266,    9,  267,
  283,  272,  257,  271,  272,   16,  274,  267,   19,  277,
  285,  267,  272,  270,  274,  283,  277,  277,  262,  278,
  279,  280,  274,  283,  278,  279,  280,  273,  267,  272,
  289,  259,  272,  267,  272,  289,  272,  272,  267,  272,
  266,  273,  261,  267,  257,  272,  296,  272,  268,  272,
  267,  273,  267,  267,  272,  162,  272,  166,  168,  137,
  149,  169,   81,   23,   16,   68,  182,
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
"Prog : $$1 PROGRAMA ID PVIRG $$2 Decls SubProgs CmdComp",
"Decls :",
"$$3 :",
"Decls : VAR $$3 ListDecl",
"ListDecl : Declaracao",
"ListDecl : ListDecl Declaracao",
"$$4 :",
"Declaracao : Tipo $$4 ListElemDecl PVIRG",
"Tipo : INT",
"Tipo : REAL",
"Tipo : CARAC",
"Tipo : LOGIC",
"ListElemDecl : ElemDecl",
"$$5 :",
"ListElemDecl : ListElemDecl VIRG $$5 ElemDecl",
"ElemDecl : ID",
"$$6 :",
"ElemDecl : ID ABCOL $$6 ListDim FCOL",
"ListDim : CTINT",
"ListDim : ListDim VIRG CTINT",
"SubProgs :",
"SubProgs : SubProgs DeclSubProg",
"DeclSubProg : Cabecalho Decls CmdComp",
"Cabecalho : CabFunc",
"Cabecalho : CabProc",
"CabFunc : FUNCAO Tipo ID ABPAR FPAR PVIRG",
"$$7 :",
"CabFunc : FUNCAO Tipo ID ABPAR $$7 ListParam FPAR PVIRG",
"CabProc : PROCEDIMENTO ID ABPAR FPAR PVIRG",
"$$8 :",
"CabProc : PROCEDIMENTO ID ABPAR $$8 ListParam FPAR PVIRG",
"ListParam : Parametro",
"$$9 :",
"ListParam : ListParam VIRG $$9 Parametro",
"Parametro : Tipo ID",
"$$10 :",
"CmdComp : ABCHAV $$10 ListCmd FCHAV",
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
"$$11 :",
"$$12 :",
"$$13 :",
"$$14 :",
"CmdSe : SE ABPAR $$11 Expressao $$12 FPAR $$13 Comando $$14 CmdSenao",
"CmdSenao :",
"$$15 :",
"CmdSenao : SENAO $$15 Comando",
"$$16 :",
"$$17 :",
"$$18 :",
"CmdEnquanto : ENQUANTO ABPAR $$16 Expressao $$17 FPAR $$18 Comando",
"$$19 :",
"$$20 :",
"$$21 :",
"CmdRepetir : REPETIR $$19 Comando ENQUANTO $$20 ABPAR Expressao $$21 FPAR PVIRG",
"$$22 :",
"$$23 :",
"$$24 :",
"$$25 :",
"$$26 :",
"$$27 :",
"$$28 :",
"CmdPara : PARA $$22 ABPAR Variavel $$23 ATRIB Expressao $$24 PVIRG $$25 Expressao PVIRG $$26 Variavel ATRIB $$27 Expressao FPAR $$28 Comando",
"$$29 :",
"CmdLer : LER ABPAR $$29 ListVar FPAR PVIRG",
"ListVar : Variavel",
"$$30 :",
"ListVar : ListVar VIRG $$30 Variavel",
"$$31 :",
"CmdEscrever : ESCREVER ABPAR $$31 ListEscr FPAR PVIRG",
"ListEscr : ElemEscr",
"$$32 :",
"ListEscr : ListEscr VIRG $$32 ElemEscr",
"ElemEscr : CADEIA",
"ElemEscr : Expressao",
"ChamaProc : CHAMAR ID ABPAR FPAR PVIRG",
"$$33 :",
"ChamaProc : CHAMAR ID ABPAR $$33 ListExpr FPAR PVIRG",
"CmdRetornar : RETORNAR PVIRG",
"$$34 :",
"$$35 :",
"CmdRetornar : RETORNAR $$34 Expressao $$35 PVIRG",
"$$36 :",
"$$37 :",
"CmdAtrib : $$36 Variavel $$37 ATRIB Expressao PVIRG",
"ListExpr : Expressao",
"$$38 :",
"ListExpr : ListExpr VIRG $$38 Expressao",
"Expressao : ExprAux1",
"$$39 :",
"Expressao : Expressao OR $$39 ExprAux1",
"ExprAux1 : ExprAux2",
"$$40 :",
"ExprAux1 : ExprAux1 AND $$40 ExprAux2",
"ExprAux2 : ExprAux3",
"$$41 :",
"ExprAux2 : NOT $$41 ExprAux3",
"ExprAux3 : ExprAux4",
"$$42 :",
"ExprAux3 : ExprAux4 OPREL $$42 ExprAux4",
"ExprAux4 : Termo",
"$$43 :",
"ExprAux4 : ExprAux4 OPAD $$43 Termo",
"Termo : Fator",
"$$44 :",
"Termo : Termo OPMULT $$44 Fator",
"Fator : Variavel",
"Fator : CTINT",
"Fator : CTREAL",
"Fator : CTCARAC",
"Fator : VERDADE",
"Fator : FALSO",
"$$45 :",
"Fator : NEG $$45 Fator",
"$$46 :",
"Fator : ABPAR $$46 Expressao FPAR",
"Fator : ChamaFunc",
"Variavel : ID",
"$$47 :",
"Variavel : ID ABCOL $$47 ListSubscr FCOL",
"ListSubscr : ExprAux4",
"$$48 :",
"ListSubscr : ListSubscr VIRG $$48 ExprAux4",
"ChamaFunc : ID ABPAR FPAR",
"$$49 :",
"ChamaFunc : ID ABPAR $$49 ListExpr FPAR",

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
#line 329 "analisador.y"
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
						printf (", EH ARRAY\n\tndims = %d, dimensoes:[", s->ndims);
						for (j = 1; j <= s->ndims; j++){
                  			printf ("  %d", s->dims[j]);
                  		}
                  		printf("]");
					}
				}
				if(s->tid == IDFUNC){
					printf(", %s", nometipvar[s->tvar]);
					if (s->array == TRUE) {
						int j;
						printf (", TEM PARAMETROS\n\tndims = %d, dimensoes:[", s->ndims);
						for (j = 1; j <= s->ndims; j++){
                  			printf ("  %s", nometipvar[s->dims[j]]);
                  		}
                  		printf("]");
					}
				}
				if(s->tid == IDPROC ){
					if (s->array == TRUE) {
						int j;
						printf (", TEM PARAMETROS\n\tndims = %d, dimensoes:[", s->ndims);
						for (j = 1; j <= s->ndims; j++){
                  			printf ("  %s", nometipvar[s->dims[j]]);
                  		}
                  		printf("]");
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

void ConferirOpMult(int p, int s, int q, int *pointer){
	switch (s) {
	     case VEZES: case DIV:
	        if (p != INTEGER && p != FLOAT && p != CHAR || q != INTEGER && q!=FLOAT && q!=CHAR)
	               Incompatibilidade("Operando improprio para operador aritmetico multiplicacao/divisao");
	        if (p == FLOAT || q == FLOAT){
	        	*pointer = FLOAT;
	        }else{
	        	*pointer = INTEGER;
	        }
	        break;
	     case REST:
	        if (p != INTEGER && p != CHAR ||  q != INTEGER && q != CHAR){
	               Incompatibilidade("Operando improprio para operador resto");
	        }
	        *pointer = INTEGER;
	        break;
	     }
}

void ConferirOpAd(int p, int s, int q, int *pointer){
	if (p != INTEGER && p != FLOAT && p != CHAR || q != INTEGER && q!=FLOAT && q!=CHAR)
           Incompatibilidade("Operando improprio para operador aritmetico de adicao/subtracao");
    if (p == FLOAT || q == FLOAT){
    	*pointer = FLOAT;
    }else{
    	*pointer = INTEGER;
    }
}

void ConferirOpRel(int p, int s, int q, int *pointer){
	switch(s){
	case IGUAL: case DIFER:
		if((p == LOGICAL && q!=LOGICAL)||(q == LOGICAL && p!=LOGICAL)){
			Incompatibilidade("Operando improprio para operador de igual/diferente");
		}
		break;
	case MAIOR: case MENOR: case MENIG: case MAIG:
		if (p != INTEGER && p != FLOAT && p != CHAR || q != INTEGER && q!=FLOAT && q!=CHAR){
           Incompatibilidade("Operando improprio para operador de maior/menor/maig/menig");
        }
		break;
	}
	*pointer = LOGICAL;
}

void ConferirNot(int p, int *pointer){
	if(p!=LOGICAL){
		Incompatibilidade("Operando improprio para operador de negacao");
	}
	*pointer = LOGICAL;
}

void ConferirAnd(int p, int q, int *pointer){
	if(p!=LOGICAL||q!=LOGICAL){
		Incompatibilidade("Operando improprio para operador de AND");
	}
	*pointer = LOGICAL;
}

void ConferirOr(int p, int q, int *pointer){
	if(p!=LOGICAL||q!=LOGICAL){
		Incompatibilidade("Operando improprio para operador de OR");
	}
	*pointer = LOGICAL;
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
				NaoEsperado ("Funcao/Procedimento nao espera parametros\(s)");
            else if (simb->ndims != val)
 				Incompatibilidade ("Numero de parametros incompativel com declaracao");
}

void VerificarTamanhoSemParametros(simbolo simb){
	if (simb != NULL)
			if (simb->array == TRUE)
				Esperado ("Parametro\(s)");
}

void MesmaVariavel(simbolo s1, simbolo s2){
	if(s1 && s2){
	if(strcmp(s1->cadeia,s2->cadeia))
		Incompatibilidade ("Variavel atualizada nao eh a mesma inicializada");
	}
}

void VerificarTipo(simbolo s1, int tipo){
	if(s1){
		if(s1->tid!=tipo){
			TipoInadequado(s1->cadeia);
		}
	}
}

void VerificarRecursividade(simbolo escopoatual,simbolo simb){
	if(simb){
		if(!strcmp(simb->cadeia,escopoatual->cadeia)){
			Recursividade("Procedimento/funcao chamando ela mesma");
		}
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
#line 957 "y.tab.c"

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
#line 148 "analisador.y"
	{InicTabSimb ();}
break;
case 2:
#line 149 "analisador.y"
	{
					escopoatual = InsereSimb (yystack.l_mark[-1].cadeia, IDPROG, NOTVAR, NULL);
					escopoglobal=escopoatual;}
break;
case 3:
#line 152 "analisador.y"
	{ImprimeTabSimb ();
					VerificaInicRef();}
break;
case 5:
#line 155 "analisador.y"
	{tab++;}
break;
case 6:
#line 155 "analisador.y"
	{tab--;}
break;
case 9:
#line 159 "analisador.y"
	{tabular(); }
break;
case 10:
#line 159 "analisador.y"
	{}
break;
case 11:
#line 161 "analisador.y"
	{strcpy(yyval.cadeia, "int"); tipocorrente = INTEGER;}
break;
case 12:
#line 162 "analisador.y"
	{strcpy(yyval.cadeia, "real"); tipocorrente = FLOAT;}
break;
case 13:
#line 163 "analisador.y"
	{strcpy(yyval.cadeia, "carac"); tipocorrente = CHAR;}
break;
case 14:
#line 164 "analisador.y"
	{strcpy(yyval.cadeia, "logic"); tipocorrente = LOGICAL;}
break;
case 16:
#line 165 "analisador.y"
	{}
break;
case 18:
#line 168 "analisador.y"
	{
					AnalisarInsercaoSimb(yystack.l_mark[0].cadeia, IDVAR, FALSE, escopoatual);}
break;
case 19:
#line 169 "analisador.y"
	{AnalisarInsercaoSimb(yystack.l_mark[-1].cadeia, IDVAR, TRUE, escopoatual);}
break;
case 20:
#line 169 "analisador.y"
	{}
break;
case 21:
#line 171 "analisador.y"
	{ConferirValorDim(yystack.l_mark[0].valint);}
break;
case 22:
#line 171 "analisador.y"
	{ ConferirValorDim(yystack.l_mark[0].valint);}
break;
case 25:
#line 175 "analisador.y"
	{ escopoatual = escopoglobal;}
break;
case 28:
#line 179 "analisador.y"
	{AnalisarInsercaoSimb (yystack.l_mark[-3].cadeia, IDFUNC, FALSE, escopoglobal);escopoatual=simb;}
break;
case 29:
#line 180 "analisador.y"
	{ AnalisarInsercaoSimb (yystack.l_mark[-1].cadeia, IDFUNC, TRUE, escopoglobal);escopoatual=simb;}
break;
case 31:
#line 182 "analisador.y"
	{AnalisarInsercaoSimb (yystack.l_mark[-3].cadeia, IDPROC, FALSE, escopoglobal);escopoatual=simb;}
break;
case 32:
#line 183 "analisador.y"
	{ AnalisarInsercaoSimb (yystack.l_mark[-1].cadeia, IDPROC, TRUE, escopoglobal);escopoatual=simb;}
break;
case 35:
#line 185 "analisador.y"
	{}
break;
case 37:
#line 187 "analisador.y"
	{AnalisarInsercaoSimb(yystack.l_mark[0].cadeia, IDVAR, FALSE, escopoatual); yyval.simb = simb; yyval.simb->inic = TRUE;  ConferirValorParam(tipocorrente);}
break;
case 38:
#line 189 "analisador.y"
	{tabular ();  tab++;}
break;
case 39:
#line 189 "analisador.y"
	{tab--; tabular (); }
break;
case 53:
#line 197 "analisador.y"
	{tabular();tab++;}
break;
case 54:
#line 197 "analisador.y"
	{ConferirExpLogica(yystack.l_mark[0].tipoexpr);}
break;
case 55:
#line 197 "analisador.y"
	{}
break;
case 56:
#line 197 "analisador.y"
	{tab--;}
break;
case 59:
#line 199 "analisador.y"
	{tabular();tab++;}
break;
case 60:
#line 199 "analisador.y"
	{tab--;}
break;
case 61:
#line 201 "analisador.y"
	{tabular(); }
break;
case 62:
#line 201 "analisador.y"
	{ConferirExpLogica(yystack.l_mark[0].tipoexpr);}
break;
case 63:
#line 201 "analisador.y"
	{tab++;}
break;
case 64:
#line 201 "analisador.y"
	{tab--;}
break;
case 65:
#line 203 "analisador.y"
	{tabular();  tab++;}
break;
case 66:
#line 203 "analisador.y"
	{tab--; tabular();  }
break;
case 67:
#line 203 "analisador.y"
	{ConferirExpLogica(yystack.l_mark[0].tipoexpr);}
break;
case 69:
#line 205 "analisador.y"
	{tabular();tab++;}
break;
case 70:
#line 205 "analisador.y"
	{ConferirPara(yystack.l_mark[0].simb);InicializarEReferenciar(yystack.l_mark[0].simb);}
break;
case 71:
#line 205 "analisador.y"
	{ConferirAtribuicao(yystack.l_mark[-3].simb, yystack.l_mark[0].tipoexpr);}
break;
case 72:
#line 205 "analisador.y"
	{}
break;
case 73:
#line 205 "analisador.y"
	{ConferirExpLogica(yystack.l_mark[-1].tipoexpr);}
break;
case 74:
#line 205 "analisador.y"
	{ MesmaVariavel(yystack.l_mark[-11].simb,yystack.l_mark[-1].simb);}
break;
case 75:
#line 205 "analisador.y"
	{}
break;
case 76:
#line 205 "analisador.y"
	{tab--;}
break;
case 77:
#line 207 "analisador.y"
	{tabular();}
break;
case 79:
#line 209 "analisador.y"
	{InicializarEReferenciar(yystack.l_mark[0].simb);}
break;
case 80:
#line 209 "analisador.y"
	{}
break;
case 81:
#line 209 "analisador.y"
	{InicializarEReferenciar(yystack.l_mark[0].simb);}
break;
case 82:
#line 211 "analisador.y"
	{tabular();}
break;
case 85:
#line 213 "analisador.y"
	{}
break;
case 87:
#line 215 "analisador.y"
	{}
break;
case 89:
#line 217 "analisador.y"
	{tabular();
				VerificarExistencia(yystack.l_mark[-3].cadeia);
				VerificarTamanhoSemParametros(simb);
				VerificarRecursividade(escopoatual,simb);
				VerificarTipo(simb, IDPROC);
			}
break;
case 90:
#line 224 "analisador.y"
	{tabular();
				
				VerificarExistencia(yystack.l_mark[-1].cadeia);
				VerificarRecursividade(escopoatual,simb);
				ultimachamada=simb;
				yyval.simb = simb;}
break;
case 91:
#line 231 "analisador.y"
	{
				VerificarTamanho(yystack.l_mark[-3].simb,yystack.l_mark[-2].nsubscr);
				VerificarTipo(yystack.l_mark[-3].simb, IDPROC);}
break;
case 92:
#line 234 "analisador.y"
	{tabular (); ConferirRetornoProcedimento(escopoatual);}
break;
case 93:
#line 235 "analisador.y"
	{tabular(); }
break;
case 94:
#line 235 "analisador.y"
	{ConferirRetornoFuncao(escopoatual,yystack.l_mark[0].tipoexpr);}
break;
case 95:
#line 235 "analisador.y"
	{}
break;
case 96:
#line 236 "analisador.y"
	{tabular ();}
break;
case 97:
#line 236 "analisador.y"
	{if  (yystack.l_mark[0].simb != NULL) yystack.l_mark[0].simb->inic = yystack.l_mark[0].simb->ref = TRUE;}
break;
case 98:
#line 236 "analisador.y"
	{ ConferirAtribuicao(yystack.l_mark[-4].simb, yystack.l_mark[-1].tipoexpr);}
break;
case 99:
#line 237 "analisador.y"
	{yyval.nsubscr = 1;checarTipo(1,yystack.l_mark[0].tipoexpr);}
break;
case 100:
#line 237 "analisador.y"
	{}
break;
case 101:
#line 237 "analisador.y"
	{yyval.nsubscr = yyval.nsubscr + 1;checarTipo(yyval.nsubscr,yystack.l_mark[0].tipoexpr);}
break;
case 103:
#line 238 "analisador.y"
	{}
break;
case 104:
#line 238 "analisador.y"
	{ConferirOr(yystack.l_mark[-3].tipoexpr, yystack.l_mark[0].tipoexpr, &yyval.tipoexpr);}
break;
case 106:
#line 239 "analisador.y"
	{}
break;
case 107:
#line 239 "analisador.y"
	{ConferirAnd(yystack.l_mark[-3].tipoexpr, yystack.l_mark[0].tipoexpr, &yyval.tipoexpr);}
break;
case 109:
#line 240 "analisador.y"
	{}
break;
case 110:
#line 240 "analisador.y"
	{ConferirNot(yystack.l_mark[0].tipoexpr, &yyval.tipoexpr);}
break;
case 112:
#line 241 "analisador.y"
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
case 113:
#line 249 "analisador.y"
	{ConferirOpRel(yystack.l_mark[-3].tipoexpr, yystack.l_mark[-2].atr, yystack.l_mark[0].tipoexpr, &yyval.tipoexpr);}
break;
case 115:
#line 251 "analisador.y"
	{switch(yystack.l_mark[0].atr){
									case MAIS: break;
									case MENOS: break;
								}
								}
break;
case 116:
#line 255 "analisador.y"
	{ConferirOpAd(yystack.l_mark[-3].tipoexpr, yystack.l_mark[-2].atr, yystack.l_mark[0].tipoexpr, &yyval.tipoexpr);}
break;
case 118:
#line 257 "analisador.y"
	{switch(yystack.l_mark[0].atr){
									case VEZES: break;
									case DIV: break;
									case REST: break;
								}
								}
break;
case 119:
#line 262 "analisador.y"
	{ConferirOpMult(yystack.l_mark[-3].tipoexpr, yystack.l_mark[-2].atr, yystack.l_mark[0].tipoexpr, &yyval.tipoexpr);}
break;
case 120:
#line 263 "analisador.y"
	{if  (yystack.l_mark[0].simb != NULL){
									yystack.l_mark[0].simb->ref  =  TRUE;
									yyval.tipoexpr = yystack.l_mark[0].simb->tvar;
								}}
break;
case 121:
#line 267 "analisador.y"
	{ yyval.tipoexpr = INTEGER;}
break;
case 122:
#line 268 "analisador.y"
	{ yyval.tipoexpr = FLOAT;}
break;
case 123:
#line 269 "analisador.y"
	{ yyval.tipoexpr = CHAR;}
break;
case 124:
#line 270 "analisador.y"
	{ yyval.tipoexpr = LOGICAL;}
break;
case 125:
#line 271 "analisador.y"
	{ yyval.tipoexpr = LOGICAL;}
break;
case 126:
#line 272 "analisador.y"
	{}
break;
case 127:
#line 272 "analisador.y"
	{
				if (yystack.l_mark[0].tipoexpr != INTEGER &&
					yystack.l_mark[0].tipoexpr != FLOAT && yystack.l_mark[0].tipoexpr != CHAR)
	Incompatibilidade  ("Operando improprio para menos unario");
				if (yystack.l_mark[0].tipoexpr == FLOAT) yyval.tipoexpr = FLOAT;
				else yyval.tipoexpr = INTEGER;
		    }
break;
case 128:
#line 279 "analisador.y"
	{}
break;
case 129:
#line 279 "analisador.y"
	{ yyval.tipoexpr = yystack.l_mark[-1].tipoexpr;}
break;
case 130:
#line 280 "analisador.y"
	{yyval.tipoexpr = yystack.l_mark[0].tipoexpr;}
break;
case 131:
#line 281 "analisador.y"
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
						yyval.simb = simb;
						if (yyval.simb != NULL)
               				if (yyval.simb->array == TRUE)
								Esperado ("Subscrito\(s)");}
break;
case 132:
#line 295 "analisador.y"
	{
 					
        			simb = ProcuraSimb (yystack.l_mark[-1].cadeia, escopoatual);
					if (simb == NULL)   simb = ProcuraSimb (yystack.l_mark[-1].cadeia, escopoglobal);
					if (simb == NULL)   NaoDeclarado (yystack.l_mark[-1].cadeia);
					else if (simb->tid != IDVAR) TipoInadequado (yystack.l_mark[-1].cadeia);
         			yyval.simb = simb;
	}
break;
case 133:
#line 302 "analisador.y"
	{   yyval.simb = yystack.l_mark[-2].simb;
		if (yyval.simb != NULL)
			if (yyval.simb->array == FALSE)
				NaoEsperado ("Subscrito\(s)");
                 	 else if (yyval.simb->ndims != yystack.l_mark[-1].nsubscr)
 				Incompatibilidade ("Numero de subscritos incompativel com declaracao");}
break;
case 134:
#line 308 "analisador.y"
	{ConferirSubscr(yystack.l_mark[0].tipoexpr);yyval.nsubscr =1;}
break;
case 135:
#line 308 "analisador.y"
	{}
break;
case 136:
#line 308 "analisador.y"
	{ConferirSubscr(yystack.l_mark[0].tipoexpr);yyval.nsubscr =yystack.l_mark[-3].nsubscr+1;}
break;
case 137:
#line 310 "analisador.y"
	{
				VerificarExistencia(yystack.l_mark[-2].cadeia);
				VerificarTamanhoSemParametros(simb);
				VerificarTipo(simb, IDFUNC);
				yyval.tipoexpr=simb->tvar;
				yyval.simb = simb;
				VerificarRecursividade(escopoatual,simb);}
break;
case 138:
#line 318 "analisador.y"
	{
				VerificarExistencia(yystack.l_mark[-1].cadeia);
				ultimachamada=simb;
				yyval.simb = simb;}
break;
case 139:
#line 323 "analisador.y"
	{
				VerificarTamanho(yystack.l_mark[-2].simb,yystack.l_mark[-1].nsubscr);
				VerificarTipo(yystack.l_mark[-2].simb, IDFUNC);
				VerificarRecursividade(escopoatual,yystack.l_mark[-2].simb);
				yyval.tipoexpr=yystack.l_mark[-2].simb->tvar;}
break;
#line 1639 "y.tab.c"
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
