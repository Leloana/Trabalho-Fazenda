#include "FuncLeituras.h"
#include <string.h>
#include <stdlib.h>
#include "arqsvg.h"
#include "aplicacoes.h"
#include "Retangulo.h"

void LinhaMove(FILE* txt,RadialTree root,int ID,double X,double Y){
    Horta _hortalica = achaIDNaArvore(root,ID);
    Forma imagem = get_HortaFigura(_hortalica);

    if(imagem != NULL){
    removeNoRadialT(root,getNodeRadialT(root,get_x(imagem),get_y(imagem),0.0000001));
    
    reporta_figura(txt,imagem);
    fprintf(txt,"\nCOORDENADA INICIAL = (%lf,%lf)", get_x(imagem), get_y(imagem));
    
    mvForma(imagem, X,Y);//forma soma as coordenadas antigas às inseridas
   
    fprintf(txt,"\nCOORDENADA FINAL = (%lf,%lf)\n", get_x(imagem), get_y(imagem));

    insertRadialT(root,get_x(imagem),get_y(imagem),_hortalica);
    }
    else printf("\nERRO NA ENTRADA DA FUNCAO [*]mv -> ID %d NAO EXISTE\n",ID);
}
//TERMINAR LINHAMOVE e fazer func "achaNaArvore"
void Harvest(FILE* txt,FILE* svg, RadialTree root,int ID, int passos, char* cardial){
    Lista Nos_Colhidos = createLst(-1);
    Horta _Colhehorta = achaIDNaArvore(root,ID);
    Retangulo Colheitadeira = get_HortaFigura(_Colhehorta);
    Retangulo Borda;
    Lista Hortas_Colhidas = createLst(-1);

    if(Colheitadeira != NULL){
    set_Colheita(Colheitadeira, true);//Ativa a flag que sinaliza que um retangulo é colheitadeira
    double Xo = get_x(Colheitadeira);//X inicial e Y inicial
    double Yo = get_y(Colheitadeira);

    if(strcmp(cardial,"n")==0){//Caso a colheiradeira vá para o norte
        LinhaMove(txt,root,ID,0.0,-passos*get_ret_alt(Colheitadeira));
        AnaliseDeColheita(txt,root,get_ret_x(Colheitadeira),get_ret_y(Colheitadeira),
        Xo+get_ret_larg(Colheitadeira),Yo+get_ret_alt(Colheitadeira),Hortas_Colhidas,Nos_Colhidos);
        
        Borda = criaRect(-1,get_ret_x(Colheitadeira),get_ret_y(Colheitadeira),get_ret_larg(Colheitadeira),(1+passos)*get_ret_alt(Colheitadeira), "red", "none");
        set_ret_dasharray(Borda,5);
        escreveRetanguloSvg(svg,Borda);
    }

    else if(strcmp(cardial,"s")==0){
        LinhaMove(txt,root,ID,0.0,(passos)*get_ret_alt(Colheitadeira));
        AnaliseDeColheita(txt,root,Xo,Yo,get_ret_x(Colheitadeira)+get_ret_larg(Colheitadeira),
        get_ret_y(Colheitadeira)+get_ret_alt(Colheitadeira),Hortas_Colhidas,Nos_Colhidos);
        
        Borda = criaRect(-1,Xo,Yo,get_ret_larg(Colheitadeira),(1+passos)*get_ret_alt(Colheitadeira), "red", "none");
        set_ret_dasharray(Borda,5);
        escreveRetanguloSvg(svg,Borda);
    }
    else if(strcmp(cardial,"l")==0){ 
        LinhaMove(txt,root,ID,passos*get_ret_larg(Colheitadeira),0.0);
        AnaliseDeColheita(txt,root,Xo,Yo,get_ret_x(Colheitadeira)+get_ret_larg(Colheitadeira),
        get_ret_y(Colheitadeira)+get_ret_alt(Colheitadeira),Hortas_Colhidas,Nos_Colhidos);

        Borda = criaRect(-1,Xo,Yo,(1+passos)*get_ret_larg(Colheitadeira),get_ret_alt(Colheitadeira), "red", "none");
        set_ret_dasharray(Borda,5);
        escreveRetanguloSvg(svg,Borda);
    }

    else if(strcmp(cardial,"o")==0){
        LinhaMove(txt,root,ID,-passos*get_ret_larg(Colheitadeira),0.0);
        AnaliseDeColheita(txt,root,get_ret_x(Colheitadeira),get_ret_y(Colheitadeira),
        Xo+get_ret_larg(Colheitadeira),Yo+get_ret_alt(Colheitadeira),Hortas_Colhidas,Nos_Colhidos);

        Borda = criaRect(-1,get_ret_x(Colheitadeira),get_ret_y(Colheitadeira),(1+passos)*get_ret_larg(Colheitadeira),get_ret_alt(Colheitadeira), "red", "none");
        set_ret_dasharray(Borda,5);
        escreveRetanguloSvg(svg,Borda);
    }
    else{
        printf("\nERRO NA ENTRADA DA FUNCAO [*]hrv -> %s E UM COMANDO INVALIDO\n",cardial);
        return;
    }
    }
    else printf("\nERRO NA ENTRADA DA FUNCAO [*]hrv -> ID %d NAO EXISTE\n",ID);
    killLst(Nos_Colhidos);
    killLst(Hortas_Colhidas);
}

void AnaliseDeColheita(FILE* txt,RadialTree root,double x1, double y1,
 double x2, double y2, Lista Hortas_Colhidas, Lista Nos_Colhidos){
    if(getNodesDentroRegiaoRadialT(root,x1,y1,x2,y2,Nos_Colhidos)){
         //Pega Nodes para remove-los
        Hortas_Colhidas = map(Nos_Colhidos,getInfoRadialT);//lista que tem hortaliças
        fold(map(Hortas_Colhidas,getInfoRadialT),reporta_figura,txt);//reporto a lista modificada que so tem geos
        Iterador K = createIterator(Nos_Colhidos,false);
        while(!isIteratorEmpty(Nos_Colhidos,K)){
            Node removido = getIteratorNext(Nos_Colhidos,K);
            Forma checkFarm = get_HortaFigura(getInfoRadialT(removido));
            if(!IsColheitadeira(checkFarm))removeNoRadialT(root,removido);
        }
        killIterator(Nos_Colhidos,K);
    }
    else fprintf(txt,"\nNada foi colhido");
}

//FALTA FAZER FUNCAO DE CHECAR SE ESTA 75% DENTRO DA PRAGA "checaDentro75"
void Plague(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double weight,double height,double ratio){
    /*Devo posicionar varias pragas circulares de forma a preencher todo o retangulo dado pelos parametros*/
    int NumCircV = round(height/(2 *ratio));//Aproximo o numero de circulos inteiros que cabem na vertical do retangulo
    int NumCircH = round(weight/(2 * ratio)); //Aproximo o numero de circulos inteiros que cabem na horizontal do retangulo
    Lista Pragas = createLst(-1);
    //Inserindo retangulo que demarca a borda
    Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
    set_ret_dasharray(Borda,5.0);
    insertLst(Pragas,Borda);

    for(int i = 0; i < NumCircH; i++){
        for(int j = 0; j < NumCircV; j++){
            Circulo plague = criaCirc(-1,(X+ratio)+(i * 2 * ratio),(Y+ratio)+(j * 2 * ratio),ratio,"red","none");
            insertLst(Pragas,plague);
        }
    }
    /*Metodo de busca possivel:
    1 - Checar as hortaliças que estao dentro do retangulo usando a funcao getNodesDentroRegiaoRadialT()

    2 - Guardar o x,y do primeiro node e rodar toda a lista vendo se ele foi atingido(funcao de figura dentro do circulo),
    checar o segundo e assim por diante
    3 - caso tenha sido atingido mais que 75% da figura eliminar o node (removeNoRadialT())
    Tentativa de Implementaçao Abaixo V V V
    */
    Lista Atingidos = createLst(-1);
    /*A busca adotada consiste em fixar em uma hortaliça e percorrer por todas as pragas(circulos)
    analisando quantos% da figura foi atingida pelos circulos*/
    if(getNodesDentroRegiaoRadialT(root,X,Y,X+weight,Y+height,Atingidos)){
    Iterador W = createIterator(Atingidos,false);
        while(!isIteratorEmpty(Atingidos,W)){//checa como uma matriz
            Node No_atual = getIteratorNext(Atingidos,W);
            Horta Atingido = getInfoRadialT(No_atual);
            reporta_figura(txt,get_HortaFigura(Atingido));
            Iterador K = createIterator(Pragas,false);

            while(!isIteratorEmpty(Pragas,K)){
                Circulo atual = getIteratorNext(Pragas,K);
                if(checaDentro75(txt,Atingido,atual)){/*Checa se figura foi atingida ao menos 75% pelos circulos da lista
                            PRECISA ESCREVER NO TXT QUANTOS % A FIGURA FOI ATINGIDA*/
                    escreveTextoSvg(svg,criaTexto(-1,get_x(Atingido),get_y(Atingido),"#FF0000","#FF0000","m","x")); 
                    removeNoRadialT(root,No_atual);
                }
            }
        }
    }
    else printf("\nA praga nao atingiu plantas\n");
    fold(Pragas,escreveGeralSvgLista,svg);
    free(Atingidos);
    Executa_ListaFormas(Pragas);
}
// //FALTA FAZER FUNCAO DE CHECAR SE ESTA 75% DENTRO DA PRAGA "checaDentroCura"
// void Cure(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double weight,double height,double ratio){
//     int NumCircV = round(height/(2 *ratio));//Aproximo o numero de circulos inteiros que cabem na vertical do retangulo
//     int NumCircH = round(weight/(2 * ratio)); //Aproximo o numero de circulos inteiros que cabem na horizontal do retangulo
//     Lista Pragas = createLst(-1);
//     //Inserindo retangulo que demarca a borda
//     Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
//     set_ret_dasharray(Borda,5.0);
//     insertLst(Pragas,Borda);

//     for(int i = 0; i < NumCircH; i++){
//         for(int j = 0; j < NumCircV; j++){
//             Circulo plague = criaCirc(-1,(X+ratio)+(i * 2 * ratio),(Y+ratio)+(j * 2 * ratio),ratio,"yellow","none");
//             insertLst(Pragas,plague);
//         }
//     }
//     Lista Atingidos = createLst(-1);
    
//     if(getNodesDentroRegiaoRadialT(root,X,Y,X+weight,Y+height,Atingidos)){
//     Iterador W = createIterator(Atingidos,false);
//         while(!isIteratorEmpty(Atingidos,W)){
//             Node No_atual = getIteratorNext(Atingidos,W);
//             Forma Atingido = getInfoRadialT(root,No_atual);
//             reporta_figura(txt,Atingido);
//             Iterador K = createIterator(Pragas,false);

//             while(!isIteratorEmpty(Pragas,K)){
//                 Circulo atual = getIteratorNext(Pragas,K);
//                 if(checaDentroCura(txt,Atingido,atual)){/*Checa se figura foi atingida ao menos 75% pelos circulos da lista
//                     Cura baseado na % PRECISA ESCREVER NO TXT QUANTOS % A FIGURA FOI ATINGIDA*/
//                     escreveTextoSvg(svg,criaTexto(-1,get_x(Atingido),get_y(Atingido),"#FF0000","#FF0000","m","x")); 
//                     removeNoRadialT(root,No_atual);
//                 }
//             }
//         }
//     }
//     else printf("\nO inseticida nao atingiu plantas\n");
//     fold(Pragas,escreveGeralSvg,svg);
//     free(Atingidos);
//     Executa_ListaFormas(Pragas);
// }
// //FALTA FAZER FUNCAO DE CHECAR SE ESTA ->>>>COMPLETAMENTE<<<<- DENTRO DA PRAGA "checaDentroAdubo"
// void fertilizing(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double weight,double height,double ratio){
//     int NumCircV = round(height/(2 *ratio));//Aproximo o numero de circulos inteiros que cabem na vertical do retangulo
//     int NumCircH = round(weight/(2 * ratio)); //Aproximo o numero de circulos inteiros que cabem na horizontal do retangulo
//     Lista Pragas = createLst(-1);
//     //Inserindo retangulo que demarca a borda
//     Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
//     set_ret_dasharray(Borda,5.0);
//     insertLst(Pragas,Borda);

//     for(int i = 0; i < NumCircH; i++){
//         for(int j = 0; j < NumCircV; j++){
//             Circulo plague = criaCirc(-1,(X+ratio)+(i * 2 * ratio),(Y+ratio)+(j * 2 * ratio),ratio,"green","none");
//             insertLst(Pragas,plague);
//         }
//     }

//     Lista Atingidos = createLst(-1);
    
//     if(getNodesDentroRegiaoRadialT(root,X,Y,X+weight,Y+height,Atingidos)){
//     Iterador W = createIterator(Atingidos,false);
//         while(!isIteratorEmpty(Atingidos,W)){
//             Node No_atual = getIteratorNext(Atingidos,W);
//             Forma Atingido = getInfoRadialT(root,No_atual);
//             reporta_figura(txt,Atingido);
//             Iterador K = createIterator(Pragas,false);

//             while(!isIteratorEmpty(Pragas,K)){
//                 Circulo atual = getIteratorNext(Pragas,K);
//                 if(checaDentroAdubo(txt,Atingido,atual)){/*Checa se figura foi atingida COMPLETAMENTE pelos circulos da lista
//                     Produtividade aumenta 10% se atingida PRECISA ESCREVER NO TXT QUANTOS % A FIGURA FOI ATINGIDA*/
//                     escreveTextoSvg(svg,criaTexto(-1,get_x(Atingido),get_y(Atingido),"#FF0000","#FF0000","m","x")); 
//                     removeNoRadialT(root,No_atual);
//                 }
//             }
//         }
//     }
//     else printf("\nO fertilizante nao atingiu plantas\n");
//     fold(Pragas,escreveGeralSvg,svg);
//     free(Atingidos);
//     Executa_ListaFormas(Pragas);
// }
// //A TERMINAR (FALTA INFO DO EVANDRO)
// void seeding(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double weight,double height,double Dx, double Dy, int j){
//     Lista decoracoes = createLst(-1);
//     Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
//     set_ret_dasharray(Borda,5.0);
//     Circulo Ponto = criaCirc(-1,X,Y,1,"red","red");
//     Retangulo Paralelo = criaRect(-1,X+Dx,Y+Dy,weight,height,"red","none");
//     insertLst(decoracoes,Borda);
//     Lista Atingidos = createLst(-1);
//     if(getNodesDentroRegiaoRadialT(root,X,Y,X+weight,Y+height,Atingidos)){

//     }

//     fold(decoracoes,escreveGeralSvg,svg);
//     Executa_ListaFormas(decoracoes);
// }

// void Dados(FILE* txt,RadialTree root,int ID){
//     Forma reportada = achaNaArvore(ID);;
//     reporta_figura(txt,reportada);
// }

// void ReportaColheitadeiras(FILE* txt,RadialTree root){
//     /*PERCORRER TODA A ARVORE E CHECAR SE SAO COLHEITADEIRAS
//     Usar while na arvore ou sla, tranformar a arvore numa lista*/
// }
