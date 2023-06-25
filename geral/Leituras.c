#include "Leituras.h"
#include "Linha.h"
#include "Circulo.h"
#include "Texto.h"
#include "Retangulo.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
// #include "FuncLeituras.h"

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

                // insertRadialT(geral,atoi(aux[1]),(Linha)L);

                for(int i = 0 ; i < 7;i++)free(aux[i]);//da free nas palavras da linha
            }
            else if(strcmp(aux[0],"r") == 0){
                Retangulo R = criaRect(atoi(aux[1]),strtod(aux[2],NULL),strtod(aux[3],NULL),strtod(aux[4],NULL),strtod(aux[5],NULL),aux[6],aux[7]);

                // insertRadialT(geral,atoi(aux[1]),(Retangulo)R);

                for(int i = 0 ; i < 8;i++)free(aux[i]);
            }
            else if(strcmp(aux[0],"c") == 0){
                Circulo C = criaCirc(atoi(aux[1]),strtod(aux[2],NULL),strtod(aux[3],NULL),strtod(aux[4],NULL),aux[5],aux[6]);
                
                // insertRadialT(geral,atoi(aux[1]),(Circulo)C);
                
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
                
                // insertRadialT(geral,atoi(aux[1]), (Texto)T);

                for(int i = 0 ; i < 8;i++)free(aux[i]);
            }
		else {
            printf("\nERRO NA ENTRADA DO GEO [*]%s -> COMANDO INVALIDO\n", aux[0]);  
            free(aux[0]);
        }
        }
    }
}

void LerQry(FILE* qry,FILE* txt,RadialTree root){
    char* palavras[100];
    int n;
    if(qry == NULL)printf(" ERRO AO ABRIR ARQUIVO QRY\n");
    else{

        char ** aux ;
        while(!feof(qry)){//enquanto nao acabar arquivo .qry o banco de dados é manipulado
            aux = LeituraLinha(qry, palavras,&n);
            if(strcmp(aux[0], "mv") == 0){
                fprintf(txt,"\n[*] mv %d %lf %lf", atoi(aux[1]),strtod(aux[2],NULL),strtod(aux[3],NULL));//reporta o comando ao relatorio
                // LinhaMove(txt,root,atoi(aux[1]),strtod(aux[2],NULL),strtod(aux[3],NULL));

                for(int i = 0;i < 4;i++)free(aux[i]);
            }
            else if(strcmp(aux[0], "hvt") == 0){
                fprintf(txt,"\n[*] hvt %d %s", atoi(aux[1]),aux[2]);//reporta o comando ao relatorio


                for(int i = 0;i < 3;i++)free(aux[i]);
            }
            else if(strcmp(aux[0], "ct") == 0){

                for(int i = 0;i < 5;i++)free(aux[i]);
            }
            else if(strcmp(aux[0], "cr") == 0){

                for(int i = 0;i < 5;i++)free(aux[i]);
            }
            else if(strcmp(aux[0], "ad") == 0){

                for(int i = 0;i < 5;i++)free(aux[i]);
            }
            else if(strcmp(aux[0], "st") == 0){

                for(int i = 0;i < 8;i++)free(aux[i]);
            }
            else {
                printf("\nERRO NA ENTRADA DO QRY [*]%s -> COMANDO INVALIDO\n", aux[0]);
                free(aux[0]); 
            } 
        }
    }
}