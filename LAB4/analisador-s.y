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
#define 	FLOAT		3
#define 	CHAR		4

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

%}
%union {
	char cadeia[50];
	int atr, valint;
	float valreal;
	char carac;
	simbolo simb;
	int tipoexpr;
	int nsubscr;
}

%type   	<tipoexpr>    	ChamaFunc
%type   	<nsubscr>    	ListExpr
%type   	<nsubscr>    	ListSubscr
%type		<simb>			Variavel
%type 		<tipoexpr> 		Expressao  ExprAux1 ExprAux2   ExprAux3   ExprAux4   Termo   Fator
%type		<cadeia>		Tipo
%type		<simb>		Parametro
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
Prog		: 	{InicTabSimb ();} PROGRAMA   ID   PVIRG { escopoatual = InsereSimb ($3, IDPROG, NOTVAR, NULL);escopoglobal=escopoatual;}  Decls    SubProgs    CmdComp {ImprimeTabSimb (); VerificaInicRef();}

Decls 		:	   |   VAR {tab++;}  ListDecl {tab--;}
				
ListDecl	:	Declaracao    |   ListDecl   Declaracao

Declaracao	: 	Tipo {tabular(); }   ListElemDecl   PVIRG 

Tipo		: 	INT {strcpy($$, "int"); tipocorrente = INTEGER;}    
			|     REAL {strcpy($$, "real"); tipocorrente = FLOAT;}   
			|    CARAC {strcpy($$, "carac"); tipocorrente = CHAR;}    
			|     LOGIC {strcpy($$, "logic"); tipocorrente = LOGICAL;}
ListElemDecl	: 	ElemDecl      |     ListElemDecl   VIRG    ElemDecl

ElemDecl  	:	ID { AnalisarInsercaoSimb($1, IDVAR, FALSE, escopoatual);}  |    ID   ABCOL {AnalisarInsercaoSimb($1, IDVAR, TRUE, escopoatual);}  ListDim   FCOL    

ListDim	: 	CTINT {ConferirValorDim($1);}   |    ListDim   VIRG   CTINT { ConferirValorDim($3);}

SubProgs	:	|   SubProgs   DeclSubProg

DeclSubProg	:   Cabecalho   Decls   CmdComp { escopoatual = escopoglobal;}

Cabecalho	:   CabFunc   |   CabProc

CabFunc	:   	FUNCAO Tipo   ID 	ABPAR  FPAR  PVIRG
				{AnalisarInsercaoSimb ($3, IDFUNC, FALSE, escopoglobal);
				escopoatual=simb;}
			|		FUNCAO Tipo   ID ABPAR
			{ AnalisarInsercaoSimb ($3, IDFUNC, TRUE, escopoglobal);
			escopoatual=simb;} ListParam  FPAR PVIRG

CabProc	:   PROCEDIMENTO   ID   ABPAR   FPAR   PVIRG
			{AnalisarInsercaoSimb ($2, IDPROC, FALSE, escopoglobal);
			escopoatual=simb;}
			|	PROCEDIMENTO   ID   ABPAR
			{ AnalisarInsercaoSimb ($2, IDPROC, TRUE, escopoglobal);
			escopoatual=simb;} ListParam   FPAR   PVIRG

ListParam	:   	Parametro   |   ListParam   VIRG    Parametro

Parametro	:   	Tipo   ID
			{AnalisarInsercaoSimb($2, IDVAR, FALSE, escopoatual);
			$$ = simb;
			$$->inic = TRUE;
			ConferirValorParam(tipocorrente);}

CmdComp	:     	ABCHAV  ListCmd   FCHAV

ListCmd	:   	   |   ListCmd   Comando

Comando  	:   	CmdComp   |   CmdSe   |   CmdEnquanto   |   CmdRepetir   |   CmdPara   
|   	CmdLer   |   CmdEscrever   |   CmdAtrib   |   ChamaProc   |   CmdRetornar   
|   	PVIRG

CmdSe		:  SE   ABPAR
			{tabular();
				tab++;}  Expressao
			{ConferirExpLogica($4);}  FPAR    Comando {tab--;}  CmdSenao

CmdSenao	:  |   SENAO {tabular();tab++;}  Comando {tab--;}

CmdEnquanto	:   ENQUANTO   ABPAR
				{tabular(); }  Expressao
				{ConferirExpLogica($4);}  FPAR
				{tab++;}  Comando {tab--;}

CmdRepetir	:   	REPETIR {tabular();  tab++;}  Comando   ENQUANTO {tab--; tabular();  }  ABPAR   Expressao {ConferirExpLogica($7);}   FPAR   PVIRG

CmdPara	:   	PARA {tabular();tab++;} ABPAR Variavel {ConferirPara($4);InicializarEReferenciar($4);} ATRIB  Expressao    {ConferirAtribuicao($4, $7);} PVIRG   Expressao   PVIRG {ConferirExpLogica($10);}  Variavel   ATRIB { MesmaVariavel($4,$13);}   Expressao   FPAR    Comando {tab--;}

CmdLer	:   	LER   ABPAR  {tabular();} ListVar   FPAR   PVIRG

ListVar	:   	Variavel {InicializarEReferenciar($1);}  |   ListVar   VIRG    Variavel {InicializarEReferenciar($3);}

CmdEscrever	:   	ESCREVER   ABPAR {tabular();}  ListEscr   FPAR   PVIRG

ListEscr	:   	ElemEscr   |   ListEscr   VIRG    ElemEscr

ElemEscr	:   	CADEIA   |   Expressao
ChamaProc	:   	CHAMAR ID ABPAR  FPAR  PVIRG {VerificarExistencia($2);VerificarTamanhoSemParametros(simb);VerificarTipo(simb, IDPROC);}
		|	CHAMAR   ID ABPAR {tabular();VerificarExistencia($2);ultimachamada=simb;}  ListExpr   FPAR   PVIRG {VerificarTamanho(simb,$5); VerificarTipo(simb, IDPROC);}
CmdRetornar	:   	RETORNAR   PVIRG {tabular (); ConferirRetornoProcedimento(escopoatual);} 
			|   RETORNAR {tabular(); }  Expressao  {ConferirRetornoFuncao(escopoatual,$3);} PVIRG 
CmdAtrib  	:   	{tabular ();} Variavel {if  ($2 != NULL) $2->inic = $2->ref = TRUE;}   ATRIB  Expressao   PVIRG { ConferirAtribuicao($2, $5);}
ListExpr	:   	Expressao {$$ = 1;checarTipo(1,$1);}  |   ListExpr   VIRG   Expressao {$$ = $$ + 1;checarTipo($$,$3);}
Expressao  	:   	ExprAux1     |     Expressao   OR  ExprAux1 {ConferirOr($1, $3, &$$);}
ExprAux1  	:   	ExprAux2     |     ExprAux1   AND  ExprAux2 {ConferirAnd($1, $3, &$$);}
ExprAux2  	:   	ExprAux3     |     NOT    ExprAux3  {ConferirNot($2, &$$);}
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
Fator		:   	Variavel 	{if  ($1 != NULL){
									$1->ref  =  TRUE;
									$$ = $1->tvar;
								}}
			|   CTINT { $$ = INTEGER;}
			|   CTREAL { $$ = FLOAT;}  
			|   CTCARAC   { $$ = CHAR;}
			|   VERDADE   { $$ = LOGICAL;}
			|   FALSO   { $$ = LOGICAL;}
			|   NEG    Fator {
				if ($2 != INTEGER && $2 != FLOAT && $2 != CHAR)
					Incompatibilidade  ("Operando improprio para menos unario");
				if ($2 == FLOAT)
					$$ = FLOAT;
				else $$ = INTEGER;
		    }
			|   ABPAR   Expressao   FPAR  { $$ = $2;} 
			|   ChamaFunc {$$ = $1;}
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
						$$ = simb;
						if ($$ != NULL)
               				if ($$->array == TRUE)
								Esperado ("Subscrito\(s)");}  |   ID   ABCOL {
        			simb = ProcuraSimb ($1, escopoatual);
					if (simb == NULL)   simb = ProcuraSimb ($1, escopoglobal);
					if (simb == NULL)   NaoDeclarado ($1);
					else if (simb->tid != IDVAR) TipoInadequado ($1);
         			$<simb>$ = simb;
	}   ListSubscr   FCOL   {$$ = $<simb>3;
		if ($$ != NULL)
			if ($$->array == FALSE)
				NaoEsperado ("Subscrito\(s)");
                 	 else if ($$->ndims != $4)
 				Incompatibilidade ("Numero de subscritos incompativel com declaracao");}
ListSubscr	: ExprAux4 {ConferirSubscr($1);$$ =1;} | ListSubscr   VIRG    ExprAux4 {ConferirSubscr($3);$$ =$1+1;}
ChamaFunc	:   	ID   ABPAR    FPAR  {VerificarExistencia($1);VerificarTamanhoSemParametros(simb);VerificarTipo(simb, IDFUNC);$$=simb->tid;VerificarRecursividade(escopoatual,simb);}
		|	ID ABPAR  {VerificarExistencia($1);ultimachamada=simb;} ListExpr   FPAR {VerificarTamanho(simb,$4);VerificarTipo(simb, IDFUNC); VerificarRecursividade(escopoatual,simb); $$=simb->tid;}
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
				printf("), ");
			}
			printf("\n");
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
		if (((p->tvar == INTEGER || p->tvar == CHAR) && (q == FLOAT || q == LOGICAL)) || (p->tvar == FLOAT && q == LOGICAL) || (p->tvar == LOGICAL && q != LOGICAL))
				Incompatibilidade ("Lado direito de comando de atribuicao improprio");
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
	if(strcmp(simb->cadeia,escopoatual->cadeia) == 0){
		printf("|%s|", escopoatual->cadeia);
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