#include <stdio.h>
#include <stdlib.h>
#include "radialtree.h"
#include <math.h>
#include "aplicacoes.h"
#include <stdbool.h>
#include "arqsvg.h"
#include "Leituras.h"



int main(void){

    printf("\nIniciando Dados...\n");
    RadialTree Arvore = newRadialTree(8 ,0.2);
    Lista Region = createLst(-1);
    ArqSvg svg = abreEscritaSvg("DADOS.svg");
    double ContabilidadeColheita;
    double ContabilidadeNaoColhidos[6] = {0,0,0,0,0,0};
    FILE* geo = fopen("TESTE.geo","r");
    FILE* qry = fopen("ARQ.qry","r");
    FILE* txt = fopen("RELATORIO.txt","w");
    printf("\nConcluido!!\n\n");

    printf("Lendo GEO...\n");
    LerGeo(geo,Arvore);
    printf("\nConcluido!!\n\n");

    printf("Lendo QRY...\n");
    LerQry(qry,txt,svg,Arvore,&ContabilidadeColheita);
    printf("\nConcluido!!\n\n");

    Arvore = ReorganizaRadialT(Arvore);

    FILE* DOT = AbreEscritaDot("arq.DOT");  
    printDotRadialTree(Arvore,"arq.DOT"); 
    FechaEscrita("arq.DOT");

    printf("Escrevendo SVG...\n");
    visitaProfundidadeRadialT(Arvore,escreveGeralSvgArvore,svg);
    printf("\nConcluido!!\n\n");
    

    printf("Finalizando TXT...\n");
    visitaProfundidadeRadialT(Arvore,NaoColhido,ContabilidadeNaoColhidos); 
    fprintf(txt,"\nRestante a ser colhido no proximo semestre:\n");
    fprintf(txt,"Morangos = %g kg\n",ContabilidadeNaoColhidos[0]);
    fprintf(txt,"Cebolas = %g kg\n",ContabilidadeNaoColhidos[1]);
    fprintf(txt,"Cenouras = %g kg\n",ContabilidadeNaoColhidos[2]);
    fprintf(txt,"Aboboras = %g kg\n",ContabilidadeNaoColhidos[3]);
    fprintf(txt,"Repolhos = %g kg\n",ContabilidadeNaoColhidos[4]);
    fprintf(txt,"\nTerreno a ser limpo:\nMatos = %g kg\n",ContabilidadeNaoColhidos[5]);
    printf("\nConcluido!!\n\n");


    printf("Fechando arquivos...\n");

    fechaSvg(svg);
    fclose(geo);
    fclose(qry);
    fclose(txt);
    killLst(Region);


    killRadialTree(Arvore);
    free(Arvore);

    return 0;
}