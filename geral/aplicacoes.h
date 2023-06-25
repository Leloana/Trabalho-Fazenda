#ifndef _APLICA_H_
#define _APLICA_H_

#define pi 3.14159265359
#include <stdbool.h>
#include "lista.h"


int Setor(int setores, double xCentro, double yCentro, double a, double b);

bool DentroRegiaoRet(double X,double Y,double x1, double y1, double x2, double y2);

bool ChecaRetSetor(double xCentro, double yCentro,double x1,double y1,double x2,double y2,int setores,int setorAtual);

void Executa_ListaFormas(Lista executada);
#endif