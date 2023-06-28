#ifndef LEGEO_H
#define LEGEO_H

#include <stdio.h>
#include "radialtree.h"
#include <stdlib.h>
/*Le cada linha de um arquivo de texto qualquer retornando um char** de cada palavra
de cada linha do arquivo.Deve ser repetido ate que o arquivo se esgote*/
char** LeituraLinha(FILE *arq, char* palavras[],int *n);

/*Le arquivo Geo e baseado em seus comandos gera objetos formas que sao inseridos
na lista "geral", há tambem um comando(ts) que nao gera forma, porem muda a fonte dos
seguintes textos*/
void LerGeo(FILE* geo,RadialTree geral);

void LerQry(FILE* qry,FILE* txt,FILE* svg, RadialTree root);

#endif