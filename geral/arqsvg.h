#ifndef ARQSVG__
#define ARQSVG__

#include "Retangulo.h"
#include "Circulo.h"
#include "Linha.h"
#include "Texto.h"
#include "formas.h"

typedef void* ArqSvg;
// abre arquivo svg e escreve inicio do arquivo
ArqSvg abreEscritaSvg(char *fn);
// escreve fim do arquivo svg e fecha arquivo
void fechaSvg(ArqSvg fsvg);
//escreve o circulo especificado no arquivo svg
void escreveCirculoSvg(ArqSvg,Circulo C);
//escreve o retangulo especificado no arquivo svg
void escreveRetanguloSvg(ArqSvg fsvg,Retangulo R);
//escreve a linha especificada no arquivo svg
void escreveLinhaSvg(ArqSvg fsvg,Linha L);
//escreve o texto especificado no arquivo svg
void escreveTextoSvg(ArqSvg fsvg,Texto T);
// analisa as formas recebidas e escreve-as no svg usando uma lista
void escreveGeralSvgLista(ArqSvg fsvg,Forma forma);
// analisa as formas recebidas e escreve-as no svg usando uma Arvore
void escreveGeralSvgArvore(Horta hortalica,double x, double y, void* svg);
#endif