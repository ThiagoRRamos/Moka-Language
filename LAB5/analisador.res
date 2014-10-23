programa teste;

var
	int a, resultado;

funcao int fibonnaci(int x);

var
	int p1, p2, cont, temp;

{
	para(cont := 0; cont < x; cont := cont + 1)
		{
			temp := p2;
			p2 := p1 + p2;
			p1 := temp;
		}
	retornar p1;
}

procedimento m(int k, real l);
{
	retornar;
}

{
	escrever(""Valor para calcular"");
	ler(a);
	resultado := fibonnaci(a);
	escrever(""Resultado eh"", resultado);
}


   TABELA  DE  SIMBOLOS:

Classe 0:
  (p1, IDVAR, INTEGER, 1, 1, fibonnaci)
Classe 1:
  (temp, IDVAR, INTEGER, 1, 1, fibonnaci)
  (p2, IDVAR, INTEGER, 1, 1, fibonnaci)
Classe 4:
  (##1, IDVAR, INTEGER, 1, 1, fibonnaci)
Classe 5:
  (##2, IDVAR, INTEGER, 1, 1, fibonnaci)
  (x, IDVAR, INTEGER, 1, 1, fibonnaci)
  (a, IDVAR, INTEGER, 1, 1, teste)
Classe 6:
  (##3, IDVAR, INTEGER, 1, 1, fibonnaci)
Classe 7:
  (##4, IDVAR, INTEGER, 1, 1, teste)
Classe 13:
  (resultado, IDVAR, INTEGER, 1, 1, teste)
Classe 15:
  (k, IDVAR, INTEGER, 1, 0, m)
Classe 16:
  (l, IDVAR, FLOAT, 1, 0, m)
Classe 17:
  (m, IDPROC, EH ARRAY
	ndims = 2, dimensoes:  INTEGER  FLOAT, teste)
  (fibonnaci, IDFUNC, INTEGER, EH ARRAY
	ndims = 1, dimensoes:  INTEGER, teste)
Classe 20:
  (teste, IDPROG)
Classe 22:
  (cont, IDVAR, INTEGER, 1, 1, fibonnaci)


***** Identificador nao referenciado: k *****



***** Identificador nao referenciado: l *****



Quadruplas do modulo teste:

	   1) OPENMOD, (MODULO, teste), (IDLE), (IDLE)
	   3) PARAM, (CADEIA, "Valor para calcular"), (IDLE), (IDLE)
	   4) WRITE, (INT, 1), (IDLE), (IDLE)
	   5) PARAM, (VAR, a), (IDLE), (IDLE)
	   6) READ, (INT, 1), (IDLE), (IDLE)
	   7) PARAM, (VAR, a), (IDLE), (IDLE)
	   8) CALL, (FUNCAO, fibonnaci), (INT, 1), (VAR, ##4)
	   9) ATRIB, (VAR, ##4), (IDLE), (VAR, resultado)
	  10) PARAM, (CADEIA, "Resultado eh"), (IDLE), (IDLE)
	  11) PARAM, (VAR, resultado), (IDLE), (IDLE)
	  12) WRITE, (INT, 2), (IDLE), (IDLE)
	  13) EXIT, (IDLE), (IDLE), (IDLE)

Quadruplas do modulo fibonnaci:

	   1) OPENMOD, (MODULO, fibonnaci), (IDLE), (IDLE)
	   2) ATRIB, (INT, 0), (IDLE), (VAR, cont)
	   3) NOP, (IDLE), (IDLE), (IDLE)
	   4) LT, (VAR, cont), (VAR, x), (VAR, ##1)
	   5) JF, (VAR, ##1), (IDLE), (ROTULO, 13)
	   7) ATRIB, (VAR, p2), (IDLE), (VAR, temp)
	   8) MAIS, (VAR, p1), (VAR, p2), (VAR, ##3)
	   9) ATRIB, (VAR, ##3), (IDLE), (VAR, p2)
	  10) ATRIB, (VAR, temp), (IDLE), (VAR, p1)
	   6) MAIS, (VAR, cont), (INT, 1), (VAR, ##2)
	  11) ATRIB, (VAR, ##2), (IDLE), (VAR, cont)
	  12) JUMP, (IDLE), (IDLE), (ROTULO, 3)
	  13) NOP, (IDLE), (IDLE), (IDLE)
	  14) RETORNO, (VAR, p1), (IDLE), (IDLE)

Quadruplas do modulo m:

	   1) OPENMOD, (MODULO, m), (IDLE), (IDLE)
	   2) RETORNO, (IDLE), (IDLE), (IDLE)
