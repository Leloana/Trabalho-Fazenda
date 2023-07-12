#include <stdio.h>
#include <stdlib.h>
#include "radialtree.h"
#include <math.h>
#include "aplicacoes.h"
#include <stdbool.h>
#include "arqsvg.h"
#include "Leituras.h"
#include <string.h>


int main(int argc, char*argv[]){

   char *PathEntrada, *PathSaida, *NomeGeo, *NomeQry = NULL;
   int NsValor; double FdValor;
   int* Ns = &NsValor;
   double* Fd = &FdValor;

   PassaArgs(&PathSaida, &PathEntrada, &NomeGeo, &NomeQry, Ns, Fd, argc, argv); //"Preenchendo as strings com seus diretorios"

   //Colocando '/' no final das strings que representam os diretorios de entrada e saida, caso nao a tenham
   if(PathSaida[strlen(PathSaida)-1]!='/') strcat(PathSaida,"/");
   if(PathEntrada[strlen(PathEntrada)-1]!='/') strcat(PathEntrada,"/");

    //Removendo uma '/' inicial caso haja
   char* aux = calloc(256,sizeof(char));
   if (NomeQry != NULL) {
        strcpy(aux,NomeQry);
        // printf(" %s \n", aux);
        aux = strchr(aux,'/');
        // printf(" %s \n", aux);
        if(aux[0] == '/') memmove(aux, aux + 1, strlen(aux));
        // printf(" %s \n", aux);
   }

    //Gerando nome padrao dos arquivos de saida
   char* NomeArq = CriaNomeArq(NomeGeo, aux);
   NomeArq = ConcatenaPath(PathSaida, NomeArq);

    NomeGeo = ConcatenaPath(PathEntrada, NomeGeo); //Gerando nome do .geo
    FILE* geo = fopen(NomeGeo, "r"); //Abrindo arquivo para leitura

    strcat(NomeArq, ".svg"); //Gerando nome do .svg
    ArqSvg svg = abreEscritaSvg(NomeArq);
    RadialTree Arvore = newRadialTree(*Ns, *Fd);
    LerGeo(geo, Arvore); //Lendo .geo

    //Caso tenha arquivo .qry
    if (NomeQry != NULL) {
        double ContabilidadeColheita;
        double ContabilidadeNaoColhidos[6] = {0,0,0,0,0,0};
        NomeQry = ConcatenaPath(PathEntrada, NomeQry); //Gerando nome do .qry
        FILE* qry = fopen(NomeQry, "r");

        //Gerando nome do .txt
        strtok(NomeArq, ".");
        strcat(NomeArq, ".txt");
        FILE* txt = fopen(NomeArq, "w");

        LerQry(qry,txt,svg,Arvore,&ContabilidadeColheita); //Lendo o .qry
        //Contabilidade nao colhido (A parte colhida esta em "Leituras.c")
        visitaProfundidadeRadialT(Arvore,NaoColhido,ContabilidadeNaoColhidos); 
        fprintf(txt,"\nRestante a ser colhido no proximo semestre:\n");
        fprintf(txt,"Morangos = %g kg\n",ContabilidadeNaoColhidos[0]);
        fprintf(txt,"Cebolas = %g kg\n",ContabilidadeNaoColhidos[1]);
        fprintf(txt,"Cenouras = %g kg\n",ContabilidadeNaoColhidos[2]);
        fprintf(txt,"Aboboras = %g kg\n",ContabilidadeNaoColhidos[3]);
        fprintf(txt,"Repolhos = %g kg\n",ContabilidadeNaoColhidos[4]);
        fprintf(txt,"\nTerreno a ser limpo:\nMatos = %g kg\n",ContabilidadeNaoColhidos[5]);
        fclose(qry);
        fclose(txt);
    }


    visitaProfundidadeRadialT(Arvore,escreveGeralSvgArvore,svg);
    fechaSvg(svg);

    strtok(NomeArq, ".");
    strcat(NomeArq, ".dot");
    Arvore = ReorganizaRadialT(Arvore);//debug
    FILE* DOT = AbreEscritaDot(NomeArq);  
    printDotRadialTree(Arvore,NomeArq); 
    FechaEscrita(NomeArq);

    fclose(geo);
    killRadialTree(Arvore);
    free(Arvore);
    return 0;
}