#ifndef FUNC_LEIT_
#define FUNC_LEIT_

#include "radialtree.h"
#include <stdio.h>
#include "lista.h"
typedef void* Colheitadeira;

void LinhaMove(FILE* txt,RadialTree root,int ID,double X,double Y);

void Harvest(FILE* txt,FILE* svg, RadialTree root,int ID, int passos, char* cardial);

void AnaliseDeColheita(FILE* txt,RadialTree root,double x1, double y1,
 double x2, double y2, Lista Hortas_Colhidas, Lista Nos_Colhidos);


#endif