#ifndef FORMAS_H
#define FORMAS_H

#include <stdio.h>


typedef void* Forma;

//'T' = texto, 'C' = circulo, 'R' = retangulo, 'L' = linha
char get_type(void* point);
// move a forma para (x+dx:y+dy)
void mvForma(Forma forma, double dx, double dy);
// retorna o id da forma generalizada
int get_ID(void* forma);
// retorna a cor do preenchimento da forma generalizada
char* get_corp(Forma forma);
// retorna a cor da borda da forma generalizada
char* get_corb(Forma forma);
// retorna o x da forma generalizada
double get_x(Forma forma);
// retorna o y da forma generalizada
double get_y(Forma forma);
//seta o x da forma generalizada
void set_x(Forma forma, double x);
//seta cor da borda da forma generalizada
void set_corb(Forma forma, char* corb);
//seta cor do preenchimento da forma generalizada
void set_corp(Forma forma, char* corp);
//seta o ID da forma generalizada
void set_ID(Forma forma, int ID);
//seta coordenada y da ancora da forma
void set_y(Forma forma, double y);
//Elimina forma
void killForma(void* forma);
/*reporta os dados da figura "guarda_forma"*/
void reporta_figura(FILE* txt,void* guarda_forma);

typedef void* Horta;

Horta criaHortalica(Forma figura);

Forma get_HortaFigura(Horta hortalica);

void set_HortaD(Horta hortalica, double Dist);

double get_HortaD(Horta hortalica);

bool IsColheitadeira(Forma forma);

#endif