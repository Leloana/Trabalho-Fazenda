#include "radialtree.h"
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include "aplicacoes.h"

static RadialTree ReorganizaRadialT(RadialTree t,int num);

typedef struct node{
    
    Info data;
    Node* galhos;
    bool removido;
    double x;
    double y;

}_node;

typedef struct tree{

    _node* raiz;
    int setores;
    double degradacao;
    double x;
    double y;
    int celulas;
    int degeneradas;

}_rTree;

RadialTree newRadialTree(int numSetores, double fd){
    _rTree* aux = calloc(1, sizeof(_rTree));

    aux->x = 0.00;
    aux->y = 0.00;
    aux->setores = numSetores;
    aux->degradacao = fd;
    aux->celulas = 0;
    aux->degeneradas = 0;
    aux->raiz = NULL;

    return aux;
}

Node insertRadialT(RadialTree t, double x, double y, Info i){
    _rTree* Tree = (_rTree*)t;
    _node* raiz = Tree->raiz;
    Tree->celulas ++;
    
    if(raiz == NULL){//Checo se arvore possui algum node
        _node* aux = calloc(1,sizeof(_node));//Crio novo node
        aux->x = x;
        aux->y = y;
        aux->data = i;
        aux->removido = false;
        aux->galhos = calloc(Tree->setores,sizeof(Node));//seto todos os setores do node para nulo
        Tree->raiz = aux;//faço a arvore receber seu primeiro node
        return aux;
    }
    else {//Caso arvore ja tenha seu primeiro node é preciso checar em qual setor deste node o proximo se encontra
        int setor = Setor(Tree->setores,raiz->x,raiz->y,x,y);//checa qual setor esta
        // printf("\nSETOR =  %d\n", setor);//debug

        _rTree aux;//cria arvore auxiliar(serve para funcionar a recursão)
        aux.setores = Tree->setores;
        aux.raiz = raiz->galhos[setor];//raiz da arvore auxiliar recebe o node que queremos colocar o novo node
        raiz->galhos[setor] = insertRadialT(&aux,x,y,i);
        return raiz;
    }
}

Node getNodeRadialT(RadialTree t, double x, double y, double epsilon){
    _rTree* Tree = (_rTree*)t;
    _node* aux = Tree->raiz;

    if (aux == NULL){

    printf("\nERRO: Node %lf %lf nao encontrado!!\n", x,y);
    return NULL;
    }

    else if(fabs(aux->x - x) < epsilon && fabs(aux->y - y) < epsilon) return aux;
    else{
        int setor = Setor(Tree->setores, aux->x, aux->y, x, y);

        _rTree radial_aux;
        radial_aux.setores = Tree->setores;
        radial_aux.degradacao = Tree->degradacao;
        radial_aux.raiz = aux->galhos[setor];

        return getNodeRadialT(&radial_aux, x, y, epsilon);
    }
}

void removeNoRadialT(RadialTree t, Node n){
    _rTree* Tree = (_rTree*)t;

    if(n != NULL){
    _node* aux = (_node*) n;
    aux->removido = true;
    Tree->degeneradas++;
        if((double)Tree->degeneradas/Tree->celulas >= (double)Tree->degradacao){
            int numCels = (int)(Tree->celulas - Tree->degeneradas);

            _rTree* NewTree = (_rTree*)ReorganizaRadialT(Tree, numCels);


            Tree->celulas = NewTree->celulas;
            Tree->degeneradas = 0;
            Tree->x = NewTree->x;
            Tree->y = NewTree->y;
        }
    }
    else printf("REMOCAO INTERROMPIDA!!!");

}

static RadialTree ReorganizaRadialT(RadialTree t,int num){
    _rTree* Tree = (_rTree*)t;
    double centro[2];
    CentroRadialTree(t, centro); //Definindo centro ficticio do retangulo
    visitaProfundidadeRadialT(t, AtualizaDistancia, centro); //Definindo Atualizando a distancia entre os nos e esse ponto

    Horta VetorArvore[num]; //Vetor do conteudo da arvore
    IniciandoVetHort(t, VetorArvore);//Colocando as hortalicas no vetor
    qsort(VetorArvore, num, sizeof(Info), OrdenaDistancia); //Ordenando o vetor com base na funcao OrdenaDistancia

    RadialTree ArvoreAux = newRadialTree(Tree->setores,Tree->degradacao); //Criando arvore auxiliar
    for (int i=0; i < num; i++) {
        insertRadialT(ArvoreAux,get_x(get_HortaFigura(VetorArvore[i])), get_y(get_HortaFigura(VetorArvore[i])), VetorArvore[i]); //Preenchendo arvore auxiliar
    }
    //dar free nos nós

    return ArvoreAux;
}

Info getInfoRadialT(Node n){
    if(n == NULL)return NULL;
    _node* aux = (_node*) n;
    return aux->data;
}

bool getNodesDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2, Lista L){
    _rTree* Tree = (_rTree*)t;
    _node* raiz = Tree->raiz;
    
//usando visita em profundidade
    if(raiz != NULL){
        if(DentroRegiaoRet(raiz->x,raiz->y,x1,y1,x2,y2) && !raiz->removido){
            // printf("Ponto encontrado no Setor!! %lf %lf   \n", raiz->x,raiz->y);
            insertLst(L,raiz);
        }
        for(int i=0;i<Tree->setores;i++){
        //SE O RETANGULO ESTIVER NESTE SETOR CONTINUE SE NAO VOLTA O LOOP
        if(ChecaRetSetor(raiz->x,raiz->y,x1,y1,x2,y2,Tree->setores,i)){

            _rTree aux;
            aux.raiz = raiz->galhos[i];
            aux.setores = Tree->setores;
            getNodesDentroRegiaoRadialT(&aux,x1,y1,x2,y2,L);
        }
        }
    }
    if(!isEmptyLst(L))return true;
    else return false;
}

bool getInfosDentroRegiaoRadialT(RadialTree t, double x1, double y1, double x2, double y2,FdentroDeRegiao f, Lista L){
    _rTree* Tree = (_rTree*)t;
    _node* raiz = Tree->raiz;
    
//usando visita em profundidade
    if(raiz != NULL){
        if(!raiz->removido && f(raiz->data,x1,x2,y1,y2)){
            // printf("Objeto encontrado no Setor!! %lf %lf \n", raiz->x,raiz->y);
            insertLst(L,raiz->data);
        }
        printf(" \n ");
        for(int i=0;i<Tree->setores ;i++){
        //SE O RETANGULO ESTIVER NESTE SETOR CONTINUE SE NAO VOLTA O LOOP
        if(ChecaRetSetor(raiz->x,raiz->y,x1,y1,x2,y2,Tree->setores,i)){
            printf("\n ");

            _rTree aux;
            aux.raiz = raiz->galhos[i];
            aux.setores = Tree->setores;
            getInfosDentroRegiaoRadialT(&aux,x1,y1,x2,y2,f,L);
        }
        }
    }
    if(!isEmptyLst(L))return true;
    else return false;
}

bool getInfosAtingidoPontoRadialT(RadialTree t, double x, double y, FpontoInternoAInfo f, Lista L){
     _rTree* Tree = (_rTree*)t;
    _node* raiz = Tree->raiz;

   if(raiz != NULL){
        if(f(raiz->data,x,y)  && !raiz->removido){
            insertLst(L,raiz->data);

        }
         for(int i=0;i<Tree->setores;i++){
        //SE O RETANGULO ESTIVER NESTE SETOR CONTINUE SE NAO VOLTA O LOOP
        _rTree aux;
        aux.raiz = raiz->galhos[i];
        aux.setores = Tree->setores;
        getInfosAtingidoPontoRadialT(&aux, x, y, f,L);
        }
   }
   if(!isEmptyLst(L))return true;
    else return false;
    
}

void visitaProfundidadeRadialT(RadialTree t, FvisitaNo f, void *aux){
     _rTree* Tree = (_rTree*)t;
    _node* raiz = Tree->raiz;

    
    if(raiz!=NULL){
        
    if(!raiz->removido)f(raiz->data,raiz->x,raiz->y,aux);

    for(int i=0;i<Tree->setores;i++){

        _rTree Taux;
        Taux.raiz = raiz->galhos[i];
        Taux.setores = Tree->setores;
        visitaProfundidadeRadialT(&Taux,f,aux);
    }
    }

}

void visitaLarguraRadialT(RadialTree t, FvisitaNo f, void *aux) {
    
    _rTree* tree = (_rTree*)t;

    if(tree->raiz!= NULL){
    _node* fila[100]; 
    int comeco = 0;
    int fim = 0;

    fila[fim++] = tree->raiz;

    while (comeco < fim) {
        int tamanhoAtual = fim - comeco;
        
        for (int i = 0; i < tamanhoAtual; i++) {
            _node* noAtual = fila[comeco++];
            if (!(noAtual->removido)) f(noAtual->data, noAtual->x, noAtual->y, aux); //Invoca a funcao f em todos os nos da arvore, exceto nos que foram marcados como removidos
 
            for (int j = 0; j < tree->setores; j++) {
                if (noAtual->galhos[j] != NULL)
                    fila[fim++] = noAtual->galhos[j];
            }
        }
    }
    }
}

Node procuraNoRadialT(RadialTree t, FsearchNo f, void *aux){
    _rTree* Tree = (_rTree*)t;
    _node* raiz = Tree->raiz;

    if (raiz == NULL){
    return NULL;
    }

    else if(f(raiz->data,raiz->x,raiz->y,aux)) return raiz;
    else{
    for(int i=0;i<Tree->setores;i++){

        _rTree Taux;
        Taux.raiz = raiz->galhos[i];
        Taux.setores = Tree->setores;
        return procuraNoRadialT(&Taux,f,aux);
    }
    }
}
