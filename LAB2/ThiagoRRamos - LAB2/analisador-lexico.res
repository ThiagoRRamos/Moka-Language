
   texto    |   tipo   | atributo 
----------------------------------
      arroba|        ID|    arroba
          :=|     ATRIB|          
       entra|        ID|     entra
           +|      OPAD|        81
           2|     CTINT|         2
           a|        ID|         a
           >|     OPREL|        63
           3|     CTINT|         3
           b|        ID|         b
          :=|     ATRIB|          
        13.2|    CTREAL|13.200000
           *|    OPMULT|        71
       7.2E4|    CTREAL|72000.000000
           -|      OPAD|        82
          91|     CTINT|        91
    enquanto|  ENQUANTO|          
           (|     ABPAR|          
           a|        ID|         a
           >|     OPREL|        63
           3|     CTINT|         3
           )|      FPAR|          
           {|    ABCHAV|          
         arb|        ID|       arb
           =|     OPREL|        65
           2|     CTINT|         2
           }|     FCHAV|          
    retornar|  RETORNAR|          
           ;|     PVIRG|          
    retornar|  RETORNAR|          
        arpa|        ID|      arpa
           ;|     PVIRG|          
procedimento|    PROCED|          
      lancar|        ID|    lancar
           (|     ABPAR|          
           )|      FPAR|          
           ;|     PVIRG|          
    contador|        ID|  contador
          :=|     ATRIB|          
    contador|        ID|  contador
           *|    OPMULT|        71
           2|     CTINT|         2
           +|      OPAD|        81
           1|     CTINT|         1
           )|      FPAR|          
           b|        ID|         b
          :=|     ATRIB|          
           b|        ID|         b
           /|    OPMULT|        72
         1.5|    CTREAL|1.500000
      funcao|    FUNCAO|          
         int|       INT|          
    fatorial|        ID|  fatorial
           (|     ABPAR|          
         int|       INT|          
           n|        ID|         n
           )|      FPAR|          
         var|       VAR|          
         int|       INT|          
           a|        ID|         a
           ,|      VIRG|          
           b|        ID|         b
           ,|      VIRG|          
           i|        ID|         i
           ;|     PVIRG|          
       logic|     LOGIC|          
     loucura|        ID|   loucura
           ;|     PVIRG|          
        real|      REAL|          
       reeee|        ID|     reeee
           ;|     PVIRG|          
       carac|     CARAC|          
         coc|        ID|       coc
           ;|     PVIRG|          
           {|    ABCHAV|          
           a|        ID|         a
          :=|     ATRIB|          
           1|     CTINT|         1
           ;|     PVIRG|          
           i|        ID|         i
          :=|     ATRIB|          
           0|     CTINT|         0
           ;|     PVIRG|          
    enquanto|  ENQUANTO|          
           (|     ABPAR|          
           i|        ID|         i
           <|     OPREL|        61
           n|        ID|         n
           )|      FPAR|          
           {|    ABCHAV|          
           i|        ID|         i
          :=|     ATRIB|          
           i|        ID|         i
           +|      OPAD|        81
           1|     CTINT|         1
           ;|     PVIRG|          
         int|       INT|          
        temp|        ID|      temp
          :=|     ATRIB|          
           a|        ID|         a
           +|      OPAD|        81
           b|        ID|         b
           ;|     PVIRG|          
           b|        ID|         b
          :=|     ATRIB|          
           a|        ID|         a
           ;|     PVIRG|          
           a|        ID|         a
          :=|     ATRIB|          
        temp|        ID|      temp
           ;|     PVIRG|          
           }|     FCHAV|          
    retornar|  RETORNAR|          
           a|        ID|         a
           ;|     PVIRG|          
           }|     FCHAV|          
