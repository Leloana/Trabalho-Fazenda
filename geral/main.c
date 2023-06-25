#include <stdio.h>
#include <stdlib.h>
#include "radialtree.h"
#include <math.h>
#include "aplicacoes.h"
#include <stdbool.h>
#include "arqsvg.h"

bool checkF(Item i,double x1,double y1,double x2,double y2){
    return true;
}

int main(void){
    RadialTree Arvore = newRadialTree(4,20);
    Lista Region = createLst(-1);

    Circulo Circ = criaCirc(0,0.0,0.0,3,"blue","red");
    Retangulo Rect = criaRect(1,-3.0,-3.0,10,10,"red","orange");

    printf("\nInserindo...");
    insertRadialT(Arvore,get_x(Circ),get_y(Circ),Circ);
    insertRadialT(Arvore,get_x(Rect),get_y(Rect),Rect);
    printf("Inserido.");

    
    printf("\n\nescreveArvore:\n");
    escreveArvore(Arvore);

    printf("\n\nREMOVENDO:\n");
    removeNoRadialT(Arvore,getNodeRadialT(Arvore,4,-4,0.000000000000001));

    printf("\n\nescreveArvore:\n");
    escreveArvore(Arvore);

    printf("\n\ngetInfosDentroRegiaoRadialT:\n");
    getInfosDentroRegiaoRadialT(Arvore,-4,-1,4,-4,checkF,Region);

    printf("\nTAMANHO LISTA: %d ",lengthLst(Region));

    printf("\nDENTRO DO RETANGULO: ");
    Iterador K = createIterator(Region,false);
    while(!isIteratorEmpty(Region,K)){
        printf(" %c ", get_type(getIteratorNext(Region,K)));
    }

    Executa_ListaFormas(Region);
    killIterator(Region,K);
    free(Arvore);

    //  printf("\n\ngetNodesDentroRegiaoRadialT:\n");
    // getNodesDentroRegiaoRadialT(Arvore,-5,-1,-1,-4,Nodes);

    // printf("\nTAMANHO LISTA: %d ",lengthLst(Nodes));

    // printf("\nDENTRO DO RETANGULO: ");
    // Iterador J = createIterator(Nodes,false);
    // while(!isIteratorEmpty(Nodes,J)){
    //     printf(" %d ", getInfoRadialT(Arvore, getIteratorNext(Nodes,J)));
    // }

    printf("\n\n");
    return 0;
}