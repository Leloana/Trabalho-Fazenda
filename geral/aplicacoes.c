#include "aplicacoes.h"
#include "radialtree.h"
#include <stdio.h>
#include <math.h>
#include "arqsvg.h"

void Executa_ListaFormas(Lista executada){
    Iterador apaga = createIterator(executada,false);
    while(!isIteratorEmpty(executada,apaga))killForma(getIteratorNext(executada,apaga));
    killIterator(executada,apaga);
    killLst(executada);
}

int Setor(int setores, double xCentro, double yCentro, double a, double b) {

    double distancia = sqrt(pow(xCentro-a, 2) + pow(yCentro-b, 2)); //distancia entre o ponto central e o ponto (a,b)
    double xr1, yr1, xr2, yr2;
    double angulo = 360/setores * pi/180;
    if(setores == 1){
        return 0;
    }
    else if(setores == 2){
        if( yCentro < b )return 0;
        else return 1;
    }
    else{
    for (int i=0; i<setores; i++) {
        //Definindo pontos das retas auxiliares 1 e 2
        xr1 = xCentro + distancia * cos(angulo * i);
        yr1 = yCentro + distancia * sin(angulo * i);
        xr2 = xCentro + distancia * cos(angulo * (i+1));
        yr2 = yCentro + distancia * sin(angulo * (i+1));

        double distanciaRetas = sqrt(pow(xr1 - xr2, 2) + pow(yr1 - yr2, 2)); //Distancia entre os pontos mais distantes de cada reta auxiliar em relação ao ponto central
        double distanciaR1 = sqrt(pow(a - xr1, 2) + pow(b - yr1, 2)); //Distancia entre o ponto (a,b) e do ponto mais distante da reta 1 em relação ao ponto central
        double distanciaR2 = sqrt(pow(a - xr2, 2) + pow(b - yr2, 2)); //Distancia entre o ponto (a,b) e do ponto mais distante da reta 2 em relação ao ponto central

        if (distanciaR1 <= distanciaRetas && distanciaR2 <= distanciaRetas) return i;
    }
    }
}

bool DentroRegiaoRet(double X,double Y,double x1, double y1, double x2, double y2){
    if(X >= x2 || X <= x1 || Y >= y1 || Y <= y2)return false;
    else return true;
}

bool ChecaRetSetor(double xCentro, double yCentro,double x1,double y1,double x2,double y2,int setores,int setorAtual){
    double x3 = x2;
    double y3 = y1;
    double x4 = x1;
    double y4 = y2;
    int vetSetores[4];

    vetSetores[0] = Setor(setores,xCentro,yCentro,x3,y3);
    vetSetores[1] = Setor(setores,xCentro,yCentro,x1,y1);
    vetSetores[2] = Setor(setores,xCentro,yCentro,x4,y4);
    vetSetores[3] = Setor(setores,xCentro,yCentro,x2,y2);
    printf(" \n %d -> SETORES DOS VERTICES: ", setorAtual);
    for(int i=0; i<4;i++)printf(" %d ", vetSetores[i]);
    for(int i=0; i<4;i++){
        if(setorAtual == vetSetores[i]){
            printf(" P ");
            return true;
        }
    }
    //EM RELACAO AO Y
    //arestas laterais
    //Direita continua decrescente(deve ser menor que o X do centro)
    if((x2 > xCentro) && (setorAtual < vetSetores[0] && setorAtual > vetSetores[3])){
            printf("===> Ad");
            return true;
        }
    //Crescente
    if((x2 < xCentro) && (setorAtual > vetSetores[0] && setorAtual < vetSetores[3])){
            printf("===> Ac");
            return true;
        }
    //Direita para tras
    if((x2 > xCentro && y3 > yCentro) && (setorAtual < vetSetores[0] || setorAtual > vetSetores[3])){
            printf("===> A1");
            return true;
        }
    //Esquerda continua decrescente(deve ser menor que o X do centro)
    if((x1 > xCentro) && (setorAtual < vetSetores[1] && setorAtual > vetSetores[2])){
            printf("===> Bd");
            return true;
        }
    //crescente
    if((x1 < xCentro) && (setorAtual > vetSetores[1] && setorAtual < vetSetores[2])){
            printf("===> Bc");
            return true;
        }
    //Esquerda para tras
    if((x1 > xCentro && y1 > yCentro) && (setorAtual < vetSetores[1] || setorAtual > vetSetores[2])){
            printf("===> B1");
            return true;
        }
    //EM RELACAO AO X
    //aresta superior acima do centro
    if( y3 > yCentro ){
        if(setorAtual > vetSetores[0] && setorAtual < vetSetores[1]){
            printf("===> 1");
            return true;
        }
    }
    //aresta inferior acima do centro
    if( y4 > yCentro ){
        if(setorAtual > vetSetores[3] && setorAtual < vetSetores[2]){
            printf("===> 2");
            return true;
        }
    }
    //aresta superior abaixo do centro
    if( y3 < yCentro){
        if(setorAtual < vetSetores[0] && setorAtual > vetSetores[1]){
            printf("===> 3");
            return true;
        }
    }
    //aresta inferior abaixo do centro
    if( y4 < yCentro){
        if(setorAtual > vetSetores[2] && setorAtual < vetSetores[3]){
            printf("===> 4");
            return true;
        }
    }
    printf(" FALSE");
    return false;
} 

// RadialTree ReorganizaArvore(RadialTree root){
//     // 1 - Percorrer toda a arvore procurando os pontos mais distantes
//     double Xmax, Xmin, Ymax, Ymin;
//     /*
//     enquanto a arvore nao acabar
//         se Xatual > Xmax
//             Xmax = Xatual
        
//         se Xatual < Xmin
//             Xmin = Xatual

//         se Yatual > Ymax
//             Ymax = Yatual
        
//         se Yatual > Ymin
//             Ymin = Yatual
//     */
//     // 2 - Baseado nos pontos achar meio imaginario
//     double XMeio = (Xmax + Xmin) / 2; 
//     double YMeio = (Ymax + Ymin) / 2; 
//     /*
//      3 - Percorrer novamente a arvore toda e alocando num vetor cada nó
//      baseado na distancia do nó em relacao ao ponto imaginario (Xmeio,Ymeio)
//     */
//     void** armazena;

//     // 4 - Usar esse vetor para recriar nova arvore
//     // RadialTree nova = newRadialTree( ,0.0);
//     while(armazena != NULL){ 
//         int i = 0;
//         // insertRadialT(nova, , , *(armazena + i));
//         i++;
//     }

//     // return nova;
// }