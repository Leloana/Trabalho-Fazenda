#ifndef FUNC_LEIT_
#define FUNC_LEIT_

/*Header para ser usada em "Leituras.c", aqui estao todos os comandos do arquivo qry*/

#include "radialtree.h"
#include <stdio.h>
#include "lista.h"
typedef void* Colheitadeira;
/*Move a figura identificada por "ID" para a sua coordenada somada com as coordenadas "X" e "Y"
para mover o nó atual que contem a informaçao que deseja ser movida é REMOVIDO e RECRIADO em sua nova posição
assim a arvore é refeita apenas se o fator de degradaçao atual da arvore passe seu valor maximo*/
void LinhaMove(FILE* txt,RadialTree root,int ID,double X,double Y);

/*A colheitadeira identificada por "ID" avança sua largura caso vá para o leste ou oeste e avança
sua altura caso vá para o norte ou sul, andando na direçao determinada por "cardial" a quantidade determinada
por "passos", a funçao seleciona os nós de toda a area percorrida usando a funcao getNodesRegiaoRadialT
e os remove, reportando seus atributos e fazendo a contagem final da colheita*/
double Harvest(FILE* txt,FILE* svg, RadialTree root,Lista Colhidos,int ID, int passos, char* cardial);
/*Analise de colheita é a funcao que faz o movimento da Colheitadeira usando a funcao LinhaMove e "colhe as figuras"*/
double AnaliseDeColheita(FILE* txt,RadialTree root,double x1, double y1,
 double x2, double y2,Lista Nos_Colhidos,Lista ColhidosTotal);
/*Na funçao plague circulos sao posicionados preenchendo um retangulo dado por "X,Y,height e weight", os circulos possuem
raio = "ratio" e sao os circulos que definem a porcentagem em que uma horta foi atingida, a funçao funciona usando
getNodesRegiaoRadialT que é dado pelo retangulo dos parametros, logo em seguida a contabilidade dos acertos é feita
percorrendo a lista de "pragas"(circulos) e a lista de Nodes atingidos, comparando suas area com a funcao checaDentro75
o valor é somado ao peso atual da hortaliça e em caso de "morte" a hortaliça é removida*/
void Plague(FILE* txt,FILE* svg, RadialTree root,double X,double Y,
double weight,double height,double ratio);
/*Essa funcao calcula quanto uma praga "Atual" atingiu uma horta "Atingido", retornando este valor em uma porcentagem
dada por "acerto", esse valor é repassado e escrito no txt fora desta funçao*/
bool checaDentro75(void* Atingido,void* Atual,double* acerto);
/*Acontece semelhante a "Plague", porem o valor repassado pela funcao "checaDentro75" é somado ao peso atual da hortaliça*/
void Cure(FILE* txt,FILE* svg, RadialTree root,double X,double Y,
double weight,double height,double ratio);
//funcao de semear a area
void seeding(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double factor,
double weight,double height,double Dx, double Dy, int j);
/*Pega um vetor de inteiros e multiplica o valor do vetor pelo fator de multiplicação, aproxima o resultado
para baixo e baseado no valor cria replicas da planta dentro da area delimitada por x1,y1,x2,y2.
A posição das réplicas é aleatoria*/
void sementesGeradas(FILE*txt,RadialTree root,Lista atingidos,int quantidade[7],
double factor, double x1,double y1,double x2,double y2,int j);
/*Reporta os dados da figura de identificado "ID", ela utiliza a funcao "procuraNoRadialT" passando o ID
desejado como parametro do void* */
void reportaDados(FILE* txt,RadialTree root,int ID);
/*Usa a mesma funcao que a "reportaDados" para achar o ID, quando acha-o indica que o retangulo do ID é uma Colheitadeira
ao usar a funcao "set_Colheita" como true*/
void setColheitadeira(RadialTree root,int ID,Lista _Colheitadeiras);
/*usa a funcao de visita em profundidade de "radialtree.h" passando a funcao "RepColhe" como parametro*/
void ReportaColheitadeiras(FILE* txt,Lista _Colheitadeiras);
//Fertiliza area
void fertilizing(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double weight,double height,double ratio);
//Checa se a figura atual esta completamente dentro da area de x1,y1,x2,y2.
bool CheckArea(void* atual, double x1, double y1, double x2, double y2);
//Atualiza o peso das hortalicas baseando-se na % praguejada e no adubo
void atualizaPeso(void* hortalica);

#endif