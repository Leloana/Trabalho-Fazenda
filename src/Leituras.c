#include "Leituras.h"
#include "Linha.h"
#include "Circulo.h"
#include "Texto.h"
#include "Retangulo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "formas.h"
#include "aplicacoes.h"

#include "FuncLeituras.h"

char** LeituraLinha(FILE *arq, char* palavras[],int *n)
{
    char linha[100];
    char *token;
    const char *delim = " \t\n";
    int i = 0;
    *n = i;
    // lê uma linha do arquivo
    if (fgets(linha, 100, arq) != NULL)
    {
        // separa a primeira palavra da linha
        token = strtok(linha, delim);
        // enquanto houver palavras na linha
        while (token != NULL)
        {
            // aloca memória para armazenar a palavra
            palavras[i] = (char *)malloc(strlen(token) + 1);
            // copia a palavra para a variável
            strcpy(palavras[i], token);
            // incrementa o contador de palavras
            i++;
            // separa a próxima palavra da linha
            token = strtok(NULL, delim);
        }
        // atualiza o número de palavras na linha
        *n = i;
    }
    else
    {
        // atualiza o número de palavras na linha
        *n = 0;
    }
    return palavras;
}

void LerGeo(FILE* geo,RadialTree geral){
    char* palavras[100];
    int n;
    char family[50] = "sans (sans-serif)";//seta padrao de ts
    char weight[50] = "normal";//caso a func ts seja chamada o valor setado muda
    char size[50] = "16px";
    if(geo == NULL)printf(" ERRO AO ABRIR ARQUIVO GEO\n");
    else{
        char ** aux ;
        while(!feof(geo)){//enquanto nao acabar o geo as figuras sao geradas
            aux = LeituraLinha(geo,palavras,&n);//le a linha do geo

            if(strcmp(aux[0],"l") == 0){
                Linha L = criaLinha(atoi(aux[1]),strtod(aux[2],NULL),strtod(aux[3],NULL),strtod(aux[4],NULL),strtod(aux[5],NULL), aux[6]);
                Horta hortalica = criaHortalica(L);
                insertRadialT(geral,strtod(aux[2],NULL),strtod(aux[3],NULL),hortalica);

                for(int i = 0 ; i < 7;i++)free(aux[i]);//da free nas palavras da linha
            }
            else if(strcmp(aux[0],"r") == 0){
                Retangulo R = criaRect(atoi(aux[1]),strtod(aux[2],NULL),strtod(aux[3],NULL),strtod(aux[4],NULL),strtod(aux[5],NULL),aux[6],aux[7]);
                Horta hortalica = criaHortalica(R);
                insertRadialT(geral,strtod(aux[2],NULL),strtod(aux[3],NULL),hortalica);

                for(int i = 0 ; i < 8;i++)free(aux[i]);
            }
            else if(strcmp(aux[0],"c") == 0){
                Circulo C = criaCirc(atoi(aux[1]),strtod(aux[2],NULL),strtod(aux[3],NULL),strtod(aux[4],NULL),aux[5],aux[6]);
                Horta hortalica = criaHortalica(C);
                insertRadialT(geral,strtod(aux[2],NULL),strtod(aux[3],NULL),hortalica);

                for(int i = 0 ; i < 7;i++)free(aux[i]);
            }
            else if(strcmp(aux[0],"ts")==0){
                strncpy(family,aux[1],50);
                strncpy(weight,aux[2],50);
                strncpy(size,aux[3],50);
                for(int i = 0 ; i < 4;i++)free(aux[i]);
            }
            else if(strcmp(aux[0],"t") == 0){
                Texto T = criaTexto(atoi(aux[1]),strtod(aux[2],NULL),strtod(aux[3],NULL),aux[4],aux[5],aux[6], aux[7]);
                set_text_Style(T, family,weight,size);
                Horta hortalica = criaHortalica(T);
                insertRadialT(geral,strtod(aux[2],NULL),strtod(aux[3],NULL),hortalica);

                for(int i = 0 ; i < 8;i++)free(aux[i]);
            }
            else {
                printf("\nERRO NA ENTRADA DO GEO [*]%s -> COMANDO INVALIDO\n", aux[0]);
            } 
        }
    }
}

void LerQry(FILE* qry,FILE* txt,FILE* svg,RadialTree root,double* contabilidade){
    char* palavras[100];
    int n;
    Lista Colhido = createLst(-1);
    Lista Colheitadeiras = createLst(-1);
    if(qry == NULL)printf(" ERRO AO ABRIR ARQUIVO QRY\n");
    else{
        char ** aux ;
        while(!feof(qry)){//enquanto nao acabar arquivo .qry o banco de dados é manipulado
            aux = LeituraLinha(qry, palavras,&n);
            if(strcmp(aux[0], "mv") == 0){//move figuras
                fprintf(txt,"\n\n[*] mv %d %g %g", atoi(aux[1]),strtod(aux[2],NULL),strtod(aux[3],NULL));//reporta o comando ao relatorio
                LinhaMove(txt,root,atoi(aux[1]),strtod(aux[2],NULL),strtod(aux[3],NULL));

                for(int i = 0;i < 4;i++)free(aux[i]);
            }
            else if(strcmp(aux[0], "hvt") == 0){//Colhe
                fprintf(txt,"\n\n[*] hvt %d %d %s", atoi(aux[1]), atoi(aux[2]),aux[3]);//reporta o comando ao relatorio
                *contabilidade += Harvest(txt,svg,root,Colhido,atoi(aux[1]),atoi(aux[2]),aux[3]);

                for(int i = 0;i < 4;i++)free(aux[i]);
            }
            else if(strcmp(aux[0], "cl") == 0){//Seta colheiradeiras
                fprintf(txt,"\n\n[*] cl %d ", atoi(aux[1]));
                setColheitadeira(root,atoi(aux[1]),Colheitadeiras);

                for(int i = 0;i < 2;i++)free(aux[i]);
            }
            else if(strcmp(aux[0], "ct") == 0){//Praga
                fprintf(txt,"\n\n[*] ct %g %g %g %g %g ", strtod(aux[1],NULL),strtod(aux[2],NULL),strtod(aux[3],NULL),strtod(aux[4],NULL),strtod(aux[5],NULL));
                Plague(txt,svg,root,strtod(aux[1],NULL),strtod(aux[2],NULL),strtod(aux[3],NULL),strtod(aux[4],NULL),strtod(aux[5],NULL));

                for(int i = 0;i < 6;i++)free(aux[i]);
            }
            else if(strcmp(aux[0], "cr") == 0){//Cura hortalicas
                 fprintf(txt,"\n\n[*] cr %g %g %g %g %g ", strtod(aux[1],NULL),strtod(aux[2],NULL),strtod(aux[3],NULL),strtod(aux[4],NULL),strtod(aux[5],NULL));
                 Cure(txt,svg,root,strtod(aux[1],NULL),strtod(aux[2],NULL),strtod(aux[3],NULL),strtod(aux[4],NULL),strtod(aux[5],NULL));

                for(int i = 0;i < 5;i++)free(aux[i]);
            }
            else if(strcmp(aux[0], "ad") == 0){//aduba hortalicas
                fprintf(txt,"\n\n[*] ad %g %g %g %g %g ", strtod(aux[1],NULL),strtod(aux[2],NULL),strtod(aux[3],NULL),strtod(aux[4],NULL),strtod(aux[5],NULL));
                fertilizing(txt,svg,root,strtod(aux[1],NULL),strtod(aux[2],NULL),strtod(aux[3],NULL),strtod(aux[4],NULL),strtod(aux[5],NULL));
                for(int i = 0;i < 5;i++)free(aux[i]);
            }
            else if(strcmp(aux[0], "st") == 0){//dispersa sementes
                fprintf(txt,"\n\n[*] ad %g %g %g %g %g ", strtod(aux[1],NULL),strtod(aux[2],NULL),strtod(aux[3],NULL),strtod(aux[4],NULL),strtod(aux[5],NULL));
                seeding(txt,svg,root,strtod(aux[1],NULL),strtod(aux[2],NULL),strtod(aux[3],NULL),strtod(aux[4],NULL),
                strtod(aux[5],NULL),strtod(aux[6],NULL),strtod(aux[7],NULL),atoi(aux[8]));

                for(int i = 0;i < 8;i++)free(aux[i]);
            }
            else if(strcmp(aux[0], "d?") == 0){//reporta figura i
                fprintf(txt,"\n\n[*] d? %d ", atoi(aux[1]));
                reportaDados(txt,root,atoi(aux[1]));

                for(int i = 0;i < 2;i++)free(aux[i]);
            }
            else if(strcmp(aux[0], "c?") == 0){//reporta TODAS colheitadeiras
                fprintf(txt,"\n\n[*] c?");
                fprintf(txt,"\nExiste(m) %d Colheitadeira(s):\n", lengthLst(Colheitadeiras));
                ReportaColheitadeiras(txt,Colheitadeiras);

                free(aux[0]);
            }
            else {
                printf("\nERRO NA ENTRADA DO QRY [*]%s -> COMANDO INVALIDO\n", aux[0]);
            } 
        }
        if(!isEmptyLst(Colhido)){
            //Contabilidade Colhido
            double ContabilidadeColhidos[6] = {0,0,0,0,0,0};
            fold(Colhido,TotalColhido,ContabilidadeColhidos);
            fprintf(txt,"\n\nPanorama final da safra 2023 1 semestre:\n");
            fprintf(txt,"Morangos = %g kg\n",ContabilidadeColhidos[0]);
            fprintf(txt,"Cebolas = %g kg\n",ContabilidadeColhidos[1]);
            fprintf(txt,"Cenouras = %g kg\n",ContabilidadeColhidos[2]);
            fprintf(txt,"Aboboras = %g kg\n",ContabilidadeColhidos[3]);
            fprintf(txt,"Repolhos = %g kg\n",ContabilidadeColhidos[4]);
            fprintf(txt,"\nTerreno limpo:\nMatos = %g kg\n",ContabilidadeColhidos[5]);
        }
        else fprintf(txt,"\n\nNao houve Colheitas.\n");
    }
    killLst(Colheitadeiras);
    Executa_ListaHortas(Colhido);
}