#ifndef _APLICA_H_
#define _APLICA_H_

#define pi 3.14159265359
#include <stdbool.h>
#include "lista.h"
#include "radialtree.h"
#include "formas.h"
#include <math.h>

/*Essa header contem funções gerais , que auxiliam em varias .c diferentes do codigo, como a de encontrar o ID 
da figura*/

//Passa os argumentos que serao pegos do argv da chamada
void PassaArgs(char** PathSaida, char** PathEntrada, char** NomeGeo, char** NomeQry, int* Ns, double* Fd, int argc, char** argv);
//junta o nomeGeo ao NomeQry fazendo as mudanças necessarias
char* CriaNomeArq (char* NomeGeo, char* NomeQry) ;
//Junta Path com nome fazendo as mudanças necessárias
char* ConcatenaPath (char* Path, char* nome);
//inicia a escrita do arquivo dot e retorna o arquivo
FILE* AbreEscritaDot(char* fn);
//Fecha a escrita do arquivo dot
void FechaEscrita(char* fn);
/*Retorna a hortalica indicada por ID ao usar a funcao procuraNoRadialT de "radialtree.h" e a funcao
"ajudaID"*/
Horta Teste_achaIDNaArvore(RadialTree t, int ID);

/*Funcao auxiliar a "achaIDNaArvore, utilizada na funcao procuraNoRadialT de "radialtree.h", esta funcao
retorna verdadeiro caso o ID da informaçao "i" seja igual ao ID referenciado por "ID"*/
bool Teste_ajudaID(Info i,double x,double y);

/*Retorna o setor atual com centro em "xCentro,yCentro" do ponto em "a,b"*/
int Setor(int setores, double xCentro, double yCentro, double a, double b);
/*Retorna verdadeiro caso o ponto dado por "X,Y" esteja dentro do retangulo que tem sua diagonal 
principal dada em "x1,y1,x2,y2"*/
bool DentroRegiaoRet(double X,double Y,double x1, double y1, double x2, double y2);
/*Checa se o retangulo esta no setor dado por "setorAtual", caso afirmativo, retorna verdadeiro e a arvore continua
a busca naquele setor, caso contrario retorna falso e a arvore interrompe a busca naquele setor, sendo "PODADA"*/
bool ChecaRetSetor(double xCentro, double yCentro,double x1,double y1,double x2,double y2,int setores,int setorAtual);
/*Free personalizado para listas com figuras geo*/
void Executa_ListaFormas(Lista executada);
/*Free personalizado para listas com Hortas*/
void Executa_ListaHortas(Lista executada);
//Passa pela arvore radial colocando as Hortas nao colhidas numa lista
void NaoColhido(Info i, double x, double y, void* aux);
//Faz os calculos do total colhido
void TotalColhido(void* aux, Item i);

/*=====Funcoes para reorganizar arvore caso o fator degradaçao atual da arvore supere o valor maximo aceito=====*/

/*Atualiza a distancia da hortalica em relaçao ao centro da arvore que é dado pelo void* Centro*/
void AtualizaDistancia(Info Hortalica, double x, double y, void* Centro);
/*Funcao base para o uso de qsort fornecido pela linguagem C99*/
int OrdenaDistancia (const void* Ponto1, const void* Ponto2);
/*Atribui ao vetor "aux" todas as hortalicas pertencentes a "t" que estao vivas*/
void IniciandoVetHort(RadialTree t, void* aux);
/*Acha, por meio de comparaçoes sucessivas na arvore, os "galhos"(pontos) mais extremos do retangulo imaginario
que forma a arvore, estes pontos serao usados para determinar o "centro" relativo da arvore*/
void retanguloGalhos(Info i, double x, double y, void* aux);
/*funcao auxiliar utilizada na visita por profundidade para gerar vetor que sera usado no qsort*/
void ListaDeHort(Info i, double x, double y, Lista aux);
/*Encontra o "centro" relativo a arvore*/
void CentroRadialTree(RadialTree t,double* num);
#endif