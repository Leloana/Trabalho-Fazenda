#ifndef _APLICA_H_
#define _APLICA_H_

#define pi 3.14159265359
#include <stdbool.h>
#include "lista.h"
#include "radialtree.h"
#include "formas.h"


void AtualizaDistancia(Info Hortalica, double x, double y, void* Centro);

int OrdenaDistancia (const void* Ponto1, const void* Ponto2);

void IniciandoVetHort(RadialTree t, void* aux);

void retanguloGalhos(Info i, double x, double y, void* aux);

void ListaDeHort(Info i, double x, double y, Lista aux);

void CentroRadialTree(RadialTree t,double* num);





bool ajudaID(Info i,double x,double y,void* ID);

Forma achaIDNaArvore(RadialTree t, int ID);

int Setor(int setores, double xCentro, double yCentro, double a, double b);

bool DentroRegiaoRet(double X,double Y,double x1, double y1, double x2, double y2);

bool FuncGetInfosDentroRet(void* i,double x1,double y1,double x2,double y2);

bool ChecaRetSetor(double xCentro, double yCentro,double x1,double y1,double x2,double y2,int setores,int setorAtual);

void Executa_ListaFormas(Lista executada);
#endif