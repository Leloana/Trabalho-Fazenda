#include <stdio.h>
#include "Circulo.h"
#include "Retangulo.h"
#include "Texto.h"
#include "Linha.h"
#include "formas.h"

char get_type(void* point){

    char circulo = get_circ_type(point);
    char retangulo = get_ret_type(point);
    char linha = get_lin_type(point);
    char texto = get_text_type(point);

    if(circulo == 'C')return circulo;
    else if(retangulo == 'R')return retangulo;
    else if(linha == 'L')return linha;
    else if(texto == 'T')return texto;
    else return 'W';//nao é forma conhecida
}

int get_ID(void* forma){
    if(get_type(forma) == 'C')return get_circ_ID(forma);
    else if(get_type(forma) == 'R')return get_ret_ID(forma);
    else if(get_type(forma) == 'L')return get_lin_ID(forma);
    else if(get_type(forma) == 'T')return get_text_ID(forma);
    else return -999;//ID nao encontrado
 }

void set_ID(Forma forma, int ID){
    if(get_type(forma) == 'C')set_circ_ID(forma,ID);
    else if(get_type(forma) == 'R')set_ret_ID(forma,ID);
    else if(get_type(forma) == 'L')set_lin_ID(forma,ID);
    else if(get_type(forma) == 'T')set_text_ID(forma,ID);
}

void mvForma(Forma forma, double dx, double dy){

    if(get_type(forma) == 'C'){
        set_circ_cx(forma, (get_circ_cx(forma) + dx));
        set_circ_cy(forma, (get_circ_cy(forma) + dy));
    }

    else if(get_type(forma) == 'R'){
        set_ret_x(forma, (get_ret_x(forma) + dx));
        set_ret_y(forma, (get_ret_y(forma) + dy));
    }

    else if(get_type(forma) == 'L'){
        set_lin_x1(forma, (get_lin_x1(forma) + dx));
        set_lin_x2(forma, (get_lin_x2(forma) + dx));
        set_lin_y1(forma, (get_lin_y2(forma) + dy));
        set_lin_y2(forma, (get_lin_y2(forma) + dy));
    }

    else if(get_type(forma) == 'T'){
        set_text_x(forma, (get_text_x(forma) + dx));
        set_text_y(forma, (get_text_y(forma) + dy));
    }
    
}

char* get_corp(Forma forma){
    if(get_type(forma) == 'C')return get_circ_corp(forma);
    else if(get_type(forma) == 'R')return  get_ret_corp(forma);
    else if(get_type(forma) == 'L')return  get_lin_corp(forma);
    else if(get_type(forma) == 'T')return get_text_corp(forma);
    else return "none";//cor nao encontrada
}

void set_corp(Forma forma, char* corp){
    if(get_type(forma) == 'C')set_circ_corp(forma,corp);
    else if(get_type(forma) == 'R')set_ret_corp(forma,corp);
    else if(get_type(forma) == 'L')set_lin_cor(forma,corp);
    else if(get_type(forma) == 'T')set_text_cor(forma,corp);
}

char* get_corb(Forma forma){
    if(get_type(forma) == 'C')return get_circ_corb(forma);
    else if(get_type(forma) == 'R')return  get_ret_corb(forma);
    else if(get_type(forma) == 'L')return  0;
    else if(get_type(forma) == 'T')return get_text_corb(forma);
    else return "none";//cor nao encontrada
}

void set_corb(Forma forma, char* corb){
    if(get_type(forma) == 'C')set_circ_corb(forma,corb);
    else if(get_type(forma) == 'R')set_ret_corb(forma,corb);
    else if(get_type(forma) == 'L')set_lin_cor(forma,corb);
    else if(get_type(forma) == 'T')set_text_cor(forma,corb);
}

double get_x(Forma forma){
    if(get_type(forma) == 'C')return get_circ_cx(forma);
    else if(get_type(forma) == 'R')return  get_ret_x(forma);
    else if(get_type(forma) == 'L')return  get_lin_x1(forma);
    else if(get_type(forma) == 'T')return get_text_x(forma);
    else return -999;//valor nao encontrado
}

void set_x(Forma forma, double x){
     if(get_type(forma) == 'C')set_circ_cx(forma,x);
    else if(get_type(forma) == 'R')set_ret_x(forma,x);
    else if(get_type(forma) == 'T')set_text_x(forma,x);
}

void set_y(Forma forma, double y){
     if(get_type(forma) == 'C')set_circ_cy(forma,y);
    else if(get_type(forma) == 'R')set_ret_y(forma,y);
    else if(get_type(forma) == 'T')set_text_y(forma,y);
}

double get_y(Forma forma){
    if(get_type(forma) == 'C')return get_circ_cy(forma);
    else if(get_type(forma) == 'R')return  get_ret_y(forma);
    else if(get_type(forma) == 'L')return  get_lin_y1(forma);
    else if(get_type(forma) == 'T')return get_text_y(forma);
    else return -999;//valor nao encontrado
}

void killForma(void* forma){
    if(get_type(forma) == 'C')killCirc(forma);
    else if(get_type(forma) == 'R')killRet(forma);
    else if(get_type(forma) == 'L')killLinha(forma);
    else if(get_type(forma) == 'T')killTexto(forma);
}

void reporta_figura(FILE* txt,void* guarda_forma){
    if(guarda_forma == NULL)return;
    if(get_type(guarda_forma) == 'T'){
                fprintf(txt,"\nTexto ID=%d x= %lf y= %lf corp= %s corb= %s rotacao= %lf ancora= %s texto= %s",get_text_ID(guarda_forma),
                                get_text_x(guarda_forma),get_text_y(guarda_forma),get_text_corp(guarda_forma),
                                get_text_corb(guarda_forma),get_text_rot(guarda_forma), get_text_ancora(guarda_forma), get_text_texto(guarda_forma));
                }
                
                else if(get_type(guarda_forma) == 'R'){
                    fprintf(txt,"\nRetangulo ID=%d x= %lf y= %lf altura= %lf largura=%lf rotacao= %lf corp= %s corb = %s",get_ret_ID(guarda_forma),
                                get_ret_x(guarda_forma),get_ret_y(guarda_forma),get_ret_alt(guarda_forma),get_ret_larg(guarda_forma),
                                get_ret_rot(guarda_forma), get_ret_corp(guarda_forma), get_ret_corb(guarda_forma));
                }

                else if(get_type(guarda_forma) == 'L'){
                    fprintf(txt,"\nLinha ID=%d x1= %lf y1= %lf x2= %lf y2=%lf rotaçao= %lf corp= %s",get_lin_ID(guarda_forma),
                                get_lin_x1(guarda_forma),get_lin_y1(guarda_forma),get_lin_x2(guarda_forma),
                                get_lin_y2(guarda_forma),get_lin_rot(guarda_forma), get_lin_corp(guarda_forma));
                }
                else if(get_type(guarda_forma) == 'C'){
                    fprintf(txt,"\nCirculo ID=%d cx= %lf cy= %lf r= %lf rotacao= %lf corp= %s corb = %s",get_circ_ID(guarda_forma),
                                get_circ_cx(guarda_forma),get_circ_cy(guarda_forma),get_circ_r(guarda_forma),
                                get_circ_rot(guarda_forma), get_circ_corp(guarda_forma), get_circ_corb(guarda_forma));
                }
                else printf("FORMA NAO IDENTIFICADA PARA REPORTAR\n");
}