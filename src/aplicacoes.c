#include "aplicacoes.h"
#include <stdio.h>
#include "arqsvg.h"
#include "FuncLeituras.h"
#include <string.h>

void PassaArgs(char** PathSaida, char** PathEntrada, char** NomeGeo, char** NomeQry, int* Ns, double* Fd, int argc, char** argv){
    int i=1;
    while(i<argc){
        //path da entrada
        if(strcmp("-e",argv[i])==0 && (i+1)!=argc){
            i++;
            *PathEntrada=argv[i];
        }
        //arquivo .geo
        else if(strcmp("-f",argv[i])==0 && (i+1)!=argc){
            i++;
            *NomeGeo=argv[i];
        }
        //path da saida
        else if(strcmp("-o",argv[i])==0 && (i+1)!=argc){
            i++;
            *PathSaida=argv[i];
        }
        //arquivo de consulta
        else if(strcmp("-q",argv[i])==0 && (i+1)!=argc){
            i++;
            *NomeQry=argv[i];
        }
        else if(strcmp("-fd",argv[i])==0 && (i+1)!=argc){
            i++;
            *Fd=strtod(argv[i],NULL);
        }
        else if(strcmp("-ns",argv[i])==0 && (i+1)!=argc){
            i++;
            *Ns=atoi(argv[i]);
        }
        i++;
    }
}

char* CriaNomeArq(char* NomeGeo, char* NomeQry){

    char* nome = calloc(strlen(NomeGeo)+5+strlen(NomeQry),sizeof(char));
    strcpy(nome,NomeGeo);
    strtok(nome,".");

    if (strcmp(NomeQry, "") == 0) return nome;
    
    strcat(nome,"-");
    char* Qry = calloc(strlen(NomeQry)+1,sizeof(char));
    strcpy(Qry,NomeQry);
    strtok(Qry,".");
    strcat(nome,Qry);

    return nome;
}

char* ConcatenaPath (char* Path, char* nome){
    char* aux = calloc(strlen(Path) + strlen(nome) + 5, sizeof(char));
    strcpy(aux, Path); //Copiando path para aux
    strcat(aux, nome); //Copiando nome para aux

    return aux;
}



FILE* AbreEscritaDot(char* fn){
    FILE* dot = fopen(fn, "w");

    fprintf(dot, "digraph RadialTree {\n\n");
    
    fclose(dot);
}


void FechaEscrita(char* fn){
    FILE* DOT = fopen(fn,"a+");
    fprintf(DOT,"\n}");
    fclose(DOT);
}

void Executa_ListaFormas(Lista executada){
    Iterador apaga = createIterator(executada,false);
    while(!isIteratorEmpty(executada,apaga))killForma(getIteratorNext(executada,apaga));
    killIterator(executada,apaga);
    killLst(executada);
}

void Executa_ListaHortas(Lista executada){
    Iterador apaga = createIterator(executada,false);
    while(!isIteratorEmpty(executada,apaga))killHorta(getIteratorNext(executada,apaga));
    killIterator(executada,apaga);
    killLst(executada);
}

bool Teste_ajudaID(Info i,double x,double y){
    if(get_ID(get_HortaFigura(i)) == (int)x)return true;
    else return false;
}

Horta Teste_achaIDNaArvore(RadialTree t, int ID){
    Lista ponto = createLst(-1);

    getInfosAtingidoPontoRadialT(t,ID,0,Teste_ajudaID,ponto);
    if(getFirstLst(ponto) == NULL)return NULL;
    
    Horta hortalica = getLst(ponto,getFirstLst(ponto));
    killLst(ponto);
    return hortalica;
}

int Setor(int setores, double xCentro, double yCentro, double a, double b) {

    double anguloSetor = 2 * pi / setores; // Ângulo do setor circular em radianos

    // Calcula o ângulo do ponto (a, b) em relação ao ponto central (x, y)
    double anguloPonto = atan2(b - yCentro, a - xCentro);
    if (anguloPonto < 0) {
        anguloPonto += 2 * pi; // Ajusta o ângulo para o intervalo [0, 2pi)
    }

    // Calcula o índice do setor em que o ponto está
    int indiceSetor = (int) (anguloPonto / anguloSetor);
    return indiceSetor;
}

bool DentroRegiaoRet(double X,double Y,double x1, double y1, double x2, double y2){
    if(X >= x2 || X <= x1 || Y <= y1 || Y >= y2)return false;
    else return true;
}

bool ChecaRetSetor(double xCentro, double yCentro,double x1,double y1,double x2,double y2,int setores,int setorAtual){
    double x3 = x2;
    double y3 = y1;
    double x4 = x1;
    double y4 = y2;
    int vetSetores[4];

    if(DentroRegiaoRet(xCentro,yCentro,x1,y1,x2,y2))return true;

    vetSetores[0] = Setor(setores,xCentro,yCentro,x3,y3);//Canto superior direito
    vetSetores[1] = Setor(setores,xCentro,yCentro,x1,y1);//canto superior esquerdo
    vetSetores[2] = Setor(setores,xCentro,yCentro,x4,y4);//canto inferior esquerdo
    vetSetores[3] = Setor(setores,xCentro,yCentro,x2,y2);//canto inferior direito
    for(int i=0; i<4;i++){
        if(setorAtual == vetSetores[i]){
            return true;
        }
    }

    //NORTE
    if(y2 >= yCentro){
        //EXTREMO NORTE
        if(x1 <= xCentro && x2 >= xCentro){
            if(setorAtual > vetSetores[3] && setorAtual < vetSetores[2])return true;
        }
        //NORDESTE
        if(x2 < xCentro){
            if(setorAtual > vetSetores[3] && setorAtual < vetSetores[1])return true;
        }
        //NOROESTE
        if(x1 > xCentro){
            if(setorAtual > vetSetores[0] && setorAtual < vetSetores[2])return true;
        }
    }
    //SUL
    if(y1 <= yCentro){
        //EXTREMO SUL
        if(x1 <= xCentro && x2 >= xCentro){
            if(setorAtual > vetSetores[1] && setorAtual < vetSetores[0])return true;
        }
        //SUDESTE
        if(x2 < xCentro){
            if(setorAtual > vetSetores[2] && setorAtual < vetSetores[0])return true;
        }
        //SUDOESTE
        if(x1 > xCentro){
            if(setorAtual > vetSetores[1] && setorAtual < vetSetores[3])return true;
        }
    }
    //LESTE(direita)
    if(x1 >= xCentro && y1 >= yCentro && y2 <= yCentro){
        if(setorAtual > vetSetores[2] || setorAtual < vetSetores[1])return true;
    }
    //OESTE(esquerda)
    if(x2 <= xCentro && y1 >= yCentro && y2 <= yCentro){
        if(setorAtual > vetSetores[0] && setorAtual < vetSetores[3])return true;
    }
} 

void AtualizaDistancia(Info Hortalica, double x, double y, void* Centro){
    double* vetor = (double*)Centro;
    double Distancia = sqrt(pow(vetor[0]-x,2)+pow(vetor[1]-y,2));
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
    if (get_HortaD((Horta*)hort1) == get_HortaD((Horta*)hort2)) return 0;
    else if (get_HortaD((Horta*)hort1) > get_HortaD((Horta*)hort2)) return 1;
    else return -1;
}

void NaoColhido(Info i, double x, double y, void* aux){
    atualizaPeso(i);
    char Tipo = get_HortType(i);
    double* Pesos = (double*)aux;
    if(Tipo == 'S')Pesos[0] += get_HortaP_Atual(i);//Peso dos morangos ficam no primiro endereço
    else if(Tipo == 'O')Pesos[1] += get_HortaP_Atual(i);//Peso das Cebolas no segundo assim por diante...
    else if(Tipo == 'C')Pesos[2] += get_HortaP_Atual(i);//...
    else if(Tipo == 'P')Pesos[3] += get_HortaP_Atual(i);//...
    else if(Tipo == 'R' && !IsColheitadeira(get_HortaFigura(i)))Pesos[4] += get_HortaP_Atual(i);//...
    else if(Tipo == 'G')Pesos[5] += get_HortaP_Atual(i);//Peso do mato no quinto endereço
}

void TotalColhido(void* aux, Item i){
    char Tipo = get_HortType(i);
    double* Pesos = (double*)aux;
    if(Tipo == 'S')Pesos[0] += get_HortaP_Atual(i);//Peso dos morangos ficam no primiro endereço
    else if(Tipo == 'O')Pesos[1] += get_HortaP_Atual(i);//Peso das Cebolas no segundo assim por diante...
    else if(Tipo == 'C')Pesos[2] += get_HortaP_Atual(i);//...
    else if(Tipo == 'P')Pesos[3] += get_HortaP_Atual(i);//...
    else if(Tipo == 'R' && !IsColheitadeira(get_HortaFigura(i)))Pesos[4] += get_HortaP_Atual(i);//...
    else if(Tipo == 'G')Pesos[5] += get_HortaP_Atual(i);//Peso do mato no quinto endereço
}
