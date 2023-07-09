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

    char *PathEntrada, *PathSaida, *NomeGeo, *NomeQry = "";
    int NsValor; double FdValor;
    int* Ns = &NsValor;
    double* Fd = &FdValor;

    PassaArguemtos(&PathSaida, &PathEntrada, &NomeGeo, &NomeQry, Ns, Fd, argc, argv); //"Preenchendo as strings com seus diretorios"
    printf(" %s \n%s \n%s \n%s \n%d \n%lf \n", PathSaida,PathEntrada, NomeGeo,NomeQry,*Ns,*Fd);
    //Colocando '/' no final das strings que representam os diretorios de entrada e saida, caso nao a tenham
    if(PathSaida[strlen(PathSaida)-1]!='/') strcat(PathSaida,"/");
    if(PathEntrada[strlen(PathEntrada)-1]!='/') strcat(PathEntrada,"/");
    printf("%s \n%s \n",PathSaida,PathEntrada);

    //Removendo uma '/' inicial caso haja
    char* aux = calloc(256,sizeof(char));
    // if (strcmp(NomeQry, "") != 0) {
    //     strcpy(aux,NomeQry);
    //     aux = strchr(aux,'/');
    //     if(aux[0] == '/') memmove(aux, aux + 1, strlen(aux));
    // }
    printf(" %s\n", NomeQry);
    //Gerando nome padrao dos arquivos de saida
    char* NomeArq = CriaNomeArq(NomeGeo, aux);
    NomeArq = ConcatenaPath(PathSaida, NomeArq);

    NomeGeo = ConcatenaPath(PathEntrada, NomeGeo); //Gerando nome do .geo

    printf("\nIniciando Dados...\n");
    
    double ContabilidadeColheita;
    double ContabilidadeNaoColhidos[6] = {0,0,0,0,0,0};
    
    FILE* geo = fopen(NomeGeo,"r");
    strcat(NomeArq, ".svg"); //Gerando nome do .svg
    ArqSvg svg = abreEscritaSvg(NomeArq);
    RadialTree Arvore = newRadialTree(*Ns, *Fd);
    printf("\nConcluido!!\n\n");

    printf("Lendo GEO...\n");
    LerGeo(geo,Arvore);
    fclose(geo);
    printf("\nConcluido!!\n\n");

    if (strcmp(NomeArq, "") != 0) {
        printf("Lendo QRY...\n");
        NomeQry = ConcatenaPath(PathEntrada, NomeQry); //Gerando nome do .qry
        FILE* qry = fopen(NomeQry, "r");

        //Gerando nome do .txt
        strtok(NomeArq, ".");
        strcat(NomeArq, ".txt");
        FILE* txt = fopen(NomeArq, "w");

        LerQry(qry,txt,svg,Arvore,&ContabilidadeColheita); //Lendo o .qry

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
        fclose(qry);
        fclose(txt);
        printf("\nConcluido!!\n\n");
    }

    strtok(NomeArq, ".");
    strcat(NomeArq, ".dot");
    Arvore = ReorganizaRadialT(Arvore);
    FILE* DOT = AbreEscritaDot(NomeArq);  
    printDotRadialTree(Arvore,NomeArq); 
    FechaEscrita(NomeArq);

    printf("Escrevendo SVG...\n");
    visitaProfundidadeRadialT(Arvore,escreveGeralSvgArvore,svg);
    printf("\nConcluido!!\n\n");
    fechaSvg(svg);
    killRadialTree(Arvore);
    free(Arvore);
    return 0;
}