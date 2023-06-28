#include "FuncLeituras.h"
#include <string.h>
#include <stdlib.h>
#include "arqsvg.h"
#include "aplicacoes.h"

//PRECISA CHECAR SE ARVORE PRECISA SER REFEITA EM TODO COMANDO
//Falta MUITA coisa
void LinhaMove(FILE* txt,RadialTree root,int ID,double X,double Y){
    Lista List_arv = transformaEmLista(root);
    /*
    1- tranformo arvore em uma lista e acho o ponto na lista,
    com base na lista recrio a arvore(basicamente uma busca linear)
    
    2- Tento podar a arvore e reajustar ela (impossivel slk mo trampo)
    */
    Forma imagem = acha(List_arv);
    if(imagem == NULL){
        printf("\nERRO NA ENTRADA DA FUNCAO [*]mv -> ID %d NAO EXISTE\n",ID);
        return;
    }
    else{
    reporta_figura(txt,imagem);
    fprintf(txt,"\nCOORDENADA INICIAL = (%lf,%lf)", get_x(imagem), get_y(imagem));
    mvForma(imagem, X,Y);//forma soma as coordenadas antigas às inseridas
    fprintf(txt,"\nCOORDENADA FINAL = (%lf,%lf)\n\n", get_x(imagem), get_y(imagem));
    }
    root = geraArvore(List_arv);
}
//TERMINAR LINHAMOVE e fazer func "achaNaArvore"
void Harvest(FILE* txt,FILE* svg, RadialTree root,int ID, int passos, char cardial){
    Retangulo Colheitadeira = NULL;
    Lista analises = createLst(-1);
    Colheitadeira = achaIDNaArvore(root,ID);
    if(Colheitadeira == NULL){
        printf("\nERRO NA ENTRADA DA FUNCAO [*]hrv -> ID %d NAO EXISTE\n",ID);
        return;
    }
    set_Colheita(Colheitadeira, true);
    double Xo = get_x(Colheitadeira);
    double Yo = get_y(Colheitadeira);
    /*COLHEITADEIRA COLHE ELEMENTOS POR ONDE ELA PASSA ENTAO EU POSSO:
    1- Criar um retangulo usando os pontos iniciais e finais do retangulo se movimentando
    e analisar a area desse retangulo completamente(GOAT)

    2 - Analisar usando loops a cada passo que o retangulo da colhetadeira da (bullshit)

    ESTE CODIGO E REPETITIVO POR ISSO SO O PRIMEIRO ESTA COMENTADO!!!
    */
    if(strcmp(cardial,"n")==0){
        LinhaMove(txt,root,ID,0.0,-(passos)*get_ret_larg(Colheitadeira));
        /*X,Y,Altura,Largura do retangulo total de colheita
        O retangulo total é composto pelo caminho do retangulo da colheitadeira*/
        if(getNodesDentroRegiaoRadialT(root,Xo,Yo-passos*get_ret_alt(Colheitadeira),Xo+get_ret_larg(Colheitadeira),Yo,analises)){
            /*A funcao deve ser para pegar os nos, pois é necessario remove-los depois, portanto nao
            ser usada a funcao getInfo...*/
            fold(map(analises,getInfoRadialT),reporta_figura,txt);//reporto a lista modificada que so tem infos
            map(analises,removeNoRadialT);//removo usando map
        }
        else fprintf(txt,"\nNada foi colhido");
    fprintf(svg,"\n\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" style=\"fill:%s;stroke:%s\" stroke-dasharray=\"%lf\" />",
    Xo,Yo-passos*get_ret_alt(Colheitadeira), get_ret_larg(Colheitadeira), passos*get_ret_alt(Colheitadeira),"none", "red",5);
    }

    else if(strcmp(cardial,"s")==0){
        LinhaMove(txt,root,ID,0.0,(passos)*get_ret_larg(Colheitadeira));
        if(getNodesDentroRegiaoRadialT(root,Xo,Yo,Xo+get_ret_larg(Colheitadeira),Yo+passos*(get_ret_alt(Colheitadeira)),analises)){
            fold(map(analises,getInfoRadialT),reporta_figura,txt);
            map(analises,removeNoRadialT);
        }
        else fprintf(txt,"\nNada foi colhido");
    fprintf(svg,"\n\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" style=\"fill:%s;stroke:%s\" stroke-dasharray=\"%lf\" />",
    Xo,Yo,get_ret_larg(Colheitadeira), passos*get_ret_alt(Colheitadeira),get_ret_rot(Colheitadeira), "none", "red",5);
    }
    else if(strcmp(cardial,"l")==0){
        LinhaMove(txt,root,ID,passos*get_ret_alt(Colheitadeira),0.0);
        if(getNodesDentroRegiaoRadialT(root,Xo-passos*get_ret_larg(Colheitadeira),Yo,Xo+get_ret_larg(Colheitadeira),Yo+get_ret_alt(Colheitadeira),analises)){
            fold(map(analises,getInfoRadialT),reporta_figura,txt);
            map(analises,removeNoRadialT);
        }
        else fprintf(txt,"\nNada foi colhido");
    fprintf(svg,"\n\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\" transform = \" style=\"fill:%s;stroke:%s\" stroke-dasharray=\"%lf\" />",
    Xo-passos*get_ret_larg(Colheitadeira),Yo,passos*get_ret_larg(Colheitadeira), get_ret_alt(Colheitadeira), "none", "red",5);
    }
    else if(strcmp(cardial,"o")==0){
        LinhaMove(txt,root,ID,-(passos)*get_ret_alt(Colheitadeira),0.0);
        if(getNodesDentroRegiaoRadialT(root,Xo,Yo,Xo+passos*(get_ret_larg(Colheitadeira)),Yo+passos*(get_ret_alt(Colheitadeira)),analises)){
            fold(map(analises,getInfoRadialT),reporta_figura,txt);
            map(analises,removeNoRadialT);
        }
        else fprintf(txt,"\nNada foi colhido");
    fprintf(svg,"\n\t<rect x=\"%lf\" y=\"%lf\" width=\"%lf\" height=\"%lf\"  style=\"fill:%s;stroke:%s\" stroke-dasharray=\"%lf\" />",
    Xo,Yo, passos*get_ret_larg(Colheitadeira), get_ret_alt(Colheitadeira),get_ret_rot(Colheitadeira), "none", "red",5);
    }
    else{
        printf("\nERRO NA ENTRADA DA FUNCAO [*]hrv -> ID %s E UM COMANDO INVALIDO\n",cardial);
        return;
    }
    free(analises);
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
        while(!isIteratorEmpty(Atingidos,W)){
            Node No_atual = getIteratorNext(Atingidos,W);
            Forma Atingido = getInfoRadialT(root,No_atual);
            reporta_figura(txt,Atingido);
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
    fold(Pragas,escreveGeralSvg,svg);
    free(Atingidos);
    Executa_ListaFormas(Pragas);
}
//FALTA FAZER FUNCAO DE CHECAR SE ESTA 75% DENTRO DA PRAGA "checaDentroCura"
void Cure(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double weight,double height,double ratio){
    int NumCircV = round(height/(2 *ratio));//Aproximo o numero de circulos inteiros que cabem na vertical do retangulo
    int NumCircH = round(weight/(2 * ratio)); //Aproximo o numero de circulos inteiros que cabem na horizontal do retangulo
    Lista Pragas = createLst(-1);
    //Inserindo retangulo que demarca a borda
    Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
    set_ret_dasharray(Borda,5.0);
    insertLst(Pragas,Borda);

    for(int i = 0; i < NumCircH; i++){
        for(int j = 0; j < NumCircV; j++){
            Circulo plague = criaCirc(-1,(X+ratio)+(i * 2 * ratio),(Y+ratio)+(j * 2 * ratio),ratio,"yellow","none");
            insertLst(Pragas,plague);
        }
    }
    Lista Atingidos = createLst(-1);
    
    if(getNodesDentroRegiaoRadialT(root,X,Y,X+weight,Y+height,Atingidos)){
    Iterador W = createIterator(Atingidos,false);
        while(!isIteratorEmpty(Atingidos,W)){
            Node No_atual = getIteratorNext(Atingidos,W);
            Forma Atingido = getInfoRadialT(root,No_atual);
            reporta_figura(txt,Atingido);
            Iterador K = createIterator(Pragas,false);

            while(!isIteratorEmpty(Pragas,K)){
                Circulo atual = getIteratorNext(Pragas,K);
                if(checaDentroCura(txt,Atingido,atual)){/*Checa se figura foi atingida ao menos 75% pelos circulos da lista
                    Cura baseado na % PRECISA ESCREVER NO TXT QUANTOS % A FIGURA FOI ATINGIDA*/
                    escreveTextoSvg(svg,criaTexto(-1,get_x(Atingido),get_y(Atingido),"#FF0000","#FF0000","m","x")); 
                    removeNoRadialT(root,No_atual);
                }
            }
        }
    }
    else printf("\nO inseticida nao atingiu plantas\n");
    fold(Pragas,escreveGeralSvg,svg);
    free(Atingidos);
    Executa_ListaFormas(Pragas);
}
//FALTA FAZER FUNCAO DE CHECAR SE ESTA ->>>>COMPLETAMENTE<<<<- DENTRO DA PRAGA "checaDentroAdubo"
void fertilizing(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double weight,double height,double ratio){
    int NumCircV = round(height/(2 *ratio));//Aproximo o numero de circulos inteiros que cabem na vertical do retangulo
    int NumCircH = round(weight/(2 * ratio)); //Aproximo o numero de circulos inteiros que cabem na horizontal do retangulo
    Lista Pragas = createLst(-1);
    //Inserindo retangulo que demarca a borda
    Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
    set_ret_dasharray(Borda,5.0);
    insertLst(Pragas,Borda);

    for(int i = 0; i < NumCircH; i++){
        for(int j = 0; j < NumCircV; j++){
            Circulo plague = criaCirc(-1,(X+ratio)+(i * 2 * ratio),(Y+ratio)+(j * 2 * ratio),ratio,"green","none");
            insertLst(Pragas,plague);
        }
    }

    Lista Atingidos = createLst(-1);
    
    if(getNodesDentroRegiaoRadialT(root,X,Y,X+weight,Y+height,Atingidos)){
    Iterador W = createIterator(Atingidos,false);
        while(!isIteratorEmpty(Atingidos,W)){
            Node No_atual = getIteratorNext(Atingidos,W);
            Forma Atingido = getInfoRadialT(root,No_atual);
            reporta_figura(txt,Atingido);
            Iterador K = createIterator(Pragas,false);

            while(!isIteratorEmpty(Pragas,K)){
                Circulo atual = getIteratorNext(Pragas,K);
                if(checaDentroAdubo(txt,Atingido,atual)){/*Checa se figura foi atingida COMPLETAMENTE pelos circulos da lista
                    Produtividade aumenta 10% se atingida PRECISA ESCREVER NO TXT QUANTOS % A FIGURA FOI ATINGIDA*/
                    escreveTextoSvg(svg,criaTexto(-1,get_x(Atingido),get_y(Atingido),"#FF0000","#FF0000","m","x")); 
                    removeNoRadialT(root,No_atual);
                }
            }
        }
    }
    else printf("\nO fertilizante nao atingiu plantas\n");
    fold(Pragas,escreveGeralSvg,svg);
    free(Atingidos);
    Executa_ListaFormas(Pragas);
}
//A TERMINAR (FALTA INFO DO EVANDRO)
void seeding(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double weight,double height,double Dx, double Dy, int j){
    Lista decoracoes = createLst(-1);
    Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
    set_ret_dasharray(Borda,5.0);
    Circulo Ponto = criaCirc(-1,X,Y,1,"red","red");
    Retangulo Paralelo = criaRect(-1,X+Dx,Y+Dy,weight,height,"red","none");
    insertLst(decoracoes,Borda);
    Lista Atingidos = createLst(-1);
    if(getNodesDentroRegiaoRadialT(root,X,Y,X+weight,Y+height,Atingidos)){

    }

    fold(decoracoes,escreveGeralSvg,svg);
    Executa_ListaFormas(decoracoes);
}

void Dados(FILE* txt,RadialTree root,int ID){
    Forma reportada = achaNaArvore(ID);;
    reporta_figura(txt,reportada);
}

void ReportaColheitadeiras(FILE* txt,RadialTree root){
    /*PERCORRER TODA A ARVORE E CHECAR SE SAO COLHEITADEIRAS
    Usar while na arvore ou sla, tranformar a arvore numa lista*/
}

bool isColheitadeira(Forma analisa){
    if(get_type(analisa)=='R'){
        if(get_Colheita(analisa)){
            return true;
        }
        else return false;
    }
    else return false;
}