#include <stdio.h>
#include <stdlib.h>
#include "radialtree.h"
#include <math.h>
#include "aplicacoes.h"
#include <stdbool.h>
#include "arqsvg.h"
#include "Leituras.h"

bool checkF(Item i,double x1,double y1,double x2,double y2){
    return true;
}


int main(void){
    printf("\nIniciando Dados...\n");
    RadialTree Arvore = newRadialTree(4,0.4);
    Lista Region = createLst(-1);
    ArqSvg svg = abreEscritaSvg("DADOS.svg");
    // ArqSvg REMO = abreEscritaSvg("REMOVIDO.svg");
    FILE* geo = fopen("TESTE.geo","r");
    FILE* qry = fopen("ARQ.qry","r");
    FILE* txt = fopen("RELATORIO.txt","w");
    printf("\nConcluido!!\n\n");

    LerGeo(geo,Arvore);
    LerQry(qry,txt,svg,Arvore);

    printf("Escrevendo...\n");
    visitaProfundidadeRadialT(Arvore,escreveGeralSvgArvore,svg);
    printf("\nConcluido!!\n\n");

    fechaSvg(svg);
    // fechaSvg(REMO);
    fclose(geo);
    fclose(qry);
    fclose(txt);
    return 0;
}