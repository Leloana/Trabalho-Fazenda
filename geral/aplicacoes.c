#include "aplicacoes.h"
#include <stdio.h>
#include "arqsvg.h"

void Executa_ListaFormas(Lista executada){
    Iterador apaga = createIterator(executada,false);
    while(!isIteratorEmpty(executada,apaga))killForma(getIteratorNext(executada,apaga));
    killIterator(executada,apaga);
    killLst(executada);
}

bool ajudaID(Info i,double x,double y,void* ID){
    if(get_ID(get_HortaFigura(i)) == ID)return true;
    else return false;
}

Horta achaIDNaArvore(RadialTree t, int ID){
    Node procurado = procuraNoRadialT(t,ajudaID,(int)ID);
    if(procurado == NULL){
        printf("\nID %d NAO ENCONTRADO\n", ID);
        return NULL;
    }
    return getInfoRadialT(procurado); 
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
    if(X > x2 || X < x1 || Y < y1 || Y > y2)return false;
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
    for(int i=0; i<4;i++){
        if(setorAtual == vetSetores[i]){
            return true;
        }
    }
    if(xCentro == x1 && yCentro <= y2 && yCentro >= y1)return true;//Caso esteja na aresta esquerda
    if(xCentro == x2 && yCentro <= y2 && yCentro >= y1)return true;//Caso esteja na aresta Direita
    if(xCentro == y1 && yCentro <= x2 && yCentro >= x1)return true;//Caso esteja na aresta superior
    if(xCentro == y2 && yCentro <= x2 && yCentro >= x1)return true;//Caso esteja na aresta inferior
    //EM RELACAO AO Y
    //arestas laterais
    //Direita continua decrescente(deve ser menor que o X do centro)
    if((x2 > xCentro) && (setorAtual < vetSetores[0] && setorAtual > vetSetores[3])){
            return true;
        }
    //Crescente
    if((x2 < xCentro) && (setorAtual > vetSetores[0] && setorAtual < vetSetores[3])){
            return true;
        }
    //Direita para tras
    if((x2 > xCentro && y3 > yCentro) && (setorAtual < vetSetores[0] || setorAtual > vetSetores[3])){
            return true;
        }
    //Esquerda continua decrescente(deve ser menor que o X do centro)
    if((x1 > xCentro) && (setorAtual < vetSetores[1] && setorAtual > vetSetores[2])){
            return true;
        }
    //crescente
    if((x1 < xCentro) && (setorAtual > vetSetores[1] && setorAtual < vetSetores[2])){
            return true;
        }
    //Esquerda para tras
    if((x1 > xCentro && y1 > yCentro) && (setorAtual < vetSetores[1] || setorAtual > vetSetores[2])){
            return true;
        }
    //EM RELACAO AO X
    //aresta superior acima do centro
    if( y3 > yCentro ){
        if(setorAtual > vetSetores[0] && setorAtual < vetSetores[1]){
            return true;
        }
    }
    //aresta inferior acima do centro
    if( y4 > yCentro ){
        if(setorAtual > vetSetores[3] && setorAtual < vetSetores[2]){
            return true;
        }
    }
    //aresta superior abaixo do centro
    if( y3 < yCentro){
        if(setorAtual < vetSetores[0] && setorAtual > vetSetores[1]){
            return true;
        }
    }
    //aresta inferior abaixo do centro
    if( y4 < yCentro){
        if(setorAtual > vetSetores[2] && setorAtual < vetSetores[3]){
            return true;
        }
    }
    return false;
} 

void AtualizaDistancia(Info Hortalica, double x, double y, void* Centro){
    double* vetor = (double*)Centro;
    double Distancia = sqrt(pow(vetor[0]-x,2))+(pow(vetor[1]-y,2));
    set_HortaD(Hortalica,Distancia);
}

void CentroRadialTree(RadialTree t,double* num){
    double vertices[4] = {0, 0, 0, 0};
    visitaProfundidadeRadialT(t, retanguloGalhos, vertices); //Delimitando o retangulo

    num[0] = vertices[2] - (vertices[2] - vertices[0])/2; //Definindo a x do centro
    num[1] = vertices[3] - (vertices[3] - vertices[1])/2; //Defininfo o y do centro
}

void retanguloGalhos(Info i, double x, double y, void* aux) {
    double* vetor = (double*)aux;

    //Definindo os pontos extremos de um retangulo, o qual cobre toda uma arvore radial
    if (x < vetor[0]) vetor[0] = x;
    if (y > vetor[1]) vetor[1] = y;
    if (x > vetor[2]) vetor[2] = x;
    if (y < vetor[3]) vetor[3] = y;
}
void IniciandoVetHort(RadialTree t, void* aux) {
    Horta* vetor = (Horta*)aux;
    Lista lista = createLst(-1);
    visitaProfundidadeRadialT(t, ListaDeHort, lista);
    int tam = lengthLst(lista);

    Iterador K = createIterator(lista, false);
    Horta item;

    for (int i=0; i<tam; i++){
        item = getIteratorNext(lista, K);
        vetor[i] = item;
    }
    killIterator(lista, K);
    killLst(lista);
}

void ListaDeHort(Info i, double x, double y, Lista aux){
    insertLst(aux,i);
}

int OrdenaDistancia(const void* hort1, const void* hort2) {

    if (get_HortaD((Horta)hort1) == get_HortaD((Horta)hort2)) return 0;
    else if (get_HortaD((Horta)hort1) > get_HortaD((Horta)hort1)) return 1;
    else return -1;
}