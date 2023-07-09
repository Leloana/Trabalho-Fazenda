#include "FuncLeituras.h"
#include <string.h>
#include <stdlib.h>
#include "arqsvg.h"
#include "aplicacoes.h"
#include <math.h>
#include <time.h>

void LinhaMove(FILE* txt,RadialTree root,int ID,double X,double Y){
    Horta _hortalica = Teste_achaIDNaArvore(root,ID);
    Forma imagem = get_HortaFigura(_hortalica);

    if(_hortalica != NULL){
    removeNoRadialT(&root,getNodeRadialT(root,get_x(imagem),get_y(imagem),0.0000001));
    
    reporta_figura(txt,imagem);
    fprintf(txt,"\nCOORDENADA INICIAL = (%lf,%lf)", get_x(imagem), get_y(imagem));
    
    mvForma(imagem, X,Y);//forma soma as coordenadas antigas às inseridas
   
    fprintf(txt,"\nCOORDENADA FINAL = (%lf,%lf)\n", get_x(imagem), get_y(imagem));
    set_HortX(_hortalica,get_x(imagem));
    set_HortY(_hortalica,get_y(imagem));
    insertRadialT(root,get_x(imagem),get_y(imagem),_hortalica);
    }
    else printf("\nERRO NA ENTRADA DA FUNCAO [*]mv -> ID %d NAO EXISTE\n",ID);
}

double Harvest(FILE* txt,FILE* svg, RadialTree root,Lista Colhidos,int ID, int pas, char* cardial){
    Lista Nos_Colhidos = createLst(-1);
    Horta _Colhehorta = Teste_achaIDNaArvore(root,ID);
    Retangulo Colheitadeira = get_HortaFigura(_Colhehorta);
    set_Colheita(Colheitadeira,true);
    Retangulo Borda;
    int passos = pas-1;
    double Contabilidade = 0.0;
    if(Colheitadeira != NULL && IsColheitadeira(Colheitadeira)){
    double Xo = get_x(Colheitadeira);//X inicial e Y inicial
    double Yo = get_y(Colheitadeira);

    if(strcmp(cardial,"s")==0){//Caso a colheiradeira vá para o norte
        LinhaMove(txt,root,ID,0.0,-passos*get_ret_alt(Colheitadeira));
        Contabilidade += AnaliseDeColheita(txt,root,get_ret_x(Colheitadeira),get_ret_y(Colheitadeira),
        Xo+get_ret_larg(Colheitadeira),Yo+get_ret_alt(Colheitadeira),Nos_Colhidos,Colhidos);
        
        Borda = criaRect(-1,get_ret_x(Colheitadeira),get_ret_y(Colheitadeira),get_ret_larg(Colheitadeira),(1+passos)*get_ret_alt(Colheitadeira), "red", "none");
        set_ret_dasharray(Borda,5);
        escreveRetanguloSvg(svg,Borda);
        killRet(Borda);
    }

    else if(strcmp(cardial,"n")==0){
        LinhaMove(txt,root,ID,0.0,(passos)*get_ret_alt(Colheitadeira));
        Contabilidade += AnaliseDeColheita(txt,root,Xo,Yo,get_ret_x(Colheitadeira)+get_ret_larg(Colheitadeira),
        get_ret_y(Colheitadeira)+get_ret_alt(Colheitadeira),Nos_Colhidos,Colhidos);
        
        Borda = criaRect(-1,Xo,Yo,get_ret_larg(Colheitadeira),(1+passos)*get_ret_alt(Colheitadeira), "red", "none");
        set_ret_dasharray(Borda,5);
        escreveRetanguloSvg(svg,Borda);
        killRet(Borda);
    }
    else if(strcmp(cardial,"o")==0){ 
        LinhaMove(txt,root,ID,passos*get_ret_larg(Colheitadeira),0.0);
        Contabilidade += AnaliseDeColheita(txt,root,Xo,Yo,get_ret_x(Colheitadeira)+get_ret_larg(Colheitadeira),
        get_ret_y(Colheitadeira)+get_ret_alt(Colheitadeira),Nos_Colhidos,Colhidos);

        Borda = criaRect(-1,Xo,Yo,(1+passos)*get_ret_larg(Colheitadeira),get_ret_alt(Colheitadeira), "red", "none");
        set_ret_dasharray(Borda,5);
        escreveRetanguloSvg(svg,Borda);
        killRet(Borda);
    }

    else if(strcmp(cardial,"l")==0){
        LinhaMove(txt,root,ID,-passos*get_ret_larg(Colheitadeira),0.0);
        Contabilidade += AnaliseDeColheita(txt,root,get_ret_x(Colheitadeira),get_ret_y(Colheitadeira),
        Xo+get_ret_larg(Colheitadeira),Yo+get_ret_alt(Colheitadeira),Nos_Colhidos,Colhidos);

        Borda = criaRect(-1,get_ret_x(Colheitadeira),get_ret_y(Colheitadeira),(1+passos)*get_ret_larg(Colheitadeira),get_ret_alt(Colheitadeira), "red", "none");
        set_ret_dasharray(Borda,5);
        escreveRetanguloSvg(svg,Borda);
        killRet(Borda);
    }
    else{
        printf("\nERRO NA ENTRADA DA FUNCAO [*]hrv -> %s E UM COMANDO INVALIDO\n",cardial);
        return 0.0;
    }
    }
    else printf("\nERRO NA ENTRADA DA FUNCAO [*]hrv -> ID %d NAO EXISTE\n",ID);
    if(!isEmptyLst(Nos_Colhidos))fprintf(txt,"\n%d itens foram colhidos!",lengthLst(Nos_Colhidos)-1);
    fprintf(txt,"\nTotal da colheita = %g kg",Contabilidade);
    killLst(Nos_Colhidos);
    return Contabilidade;
}

double AnaliseDeColheita(FILE* txt,RadialTree root,double x1, double y1,
 double x2, double y2,Lista Nos_Colhidos,Lista ColhidosTotal){
    double contabilidade = 0.0;
    if(getNodesDentroRegiaoRadialT(root,x1,y1,x2,y2,Nos_Colhidos)){
         //Pega Nodes para remove-los
        Iterador K = createIterator(Nos_Colhidos,false);
        while(!isIteratorEmpty(Nos_Colhidos,K)){
            Node removido = getIteratorNext(Nos_Colhidos,K);
            Horta hortalica = getInfoRadialT(removido);
            Forma checkFarm = get_HortaFigura(hortalica);
            if(!IsColheitadeira(checkFarm) && CheckArea(hortalica,x1,y1,x2,y2)){
                atualizaPeso(hortalica);
                insertLst(ColhidosTotal,hortalica);
                contabilidade += get_HortaP_Atual(hortalica);
                reporta_figura(txt,checkFarm);//reporta no txt
                removeNoRadialT(&root,removido);//remove nó
            }
        }
        killIterator(Nos_Colhidos,K);
    }
    else fprintf(txt,"\nNada foi colhido");
    return contabilidade;
}

void Plague(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double weight,double height,double ratio){
    /*Devo posicionar varias pragas circulares de forma a preencher todo o retangulo dado pelos parametros*/
    int NumCircV = round(height/(2 *ratio));//Aproximo o numero de circulos inteiros que cabem na vertical do retangulo
    int NumCircH = round(weight/(2 * ratio)); //Aproximo o numero de circulos inteiros que cabem na horizontal do retangulo
    Lista Pragas = createLst(-1);
    for(int i = 0; i < NumCircH; i++){
        for(int j = 0; j < NumCircV; j++){
            insertLst(Pragas,criaCirc(-1,(X+ratio)+(i * 2 * ratio),(Y+ratio)+(j * 2 * ratio),ratio,"red","none"));
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

            Node No_atual = getIteratorNext(Atingidos,W);//Lista de nodes
            Horta Atingido = getInfoRadialT(No_atual);//Info do node = HORTA
            if(CheckArea(Atingido,X,Y,X+weight,Y+height)){
            Iterador K = createIterator(Pragas,false);
            double Porcentagem = 0.0;
                while(!isIteratorEmpty(Pragas,K)){
                    Circulo praga_atual = getIteratorNext(Pragas,K);
                    checaDentro75(Atingido,praga_atual,&Porcentagem);
                }
                set_HortPraga(Atingido,get_HortPraga(Atingido)+Porcentagem);
                if(Porcentagem != 0.0){
                        if(get_HortPraga(Atingido) > 0.75){
                        Texto morte = criaTexto(-1,get_x(get_HortaFigura(Atingido)),get_y(get_HortaFigura(Atingido)),"#FF0000","#FF0000","m","x");
                        escreveTextoSvg(svg,morte);
                        removeNoRadialT(&root,No_atual);
                        killTexto(morte); 
                        fprintf(txt,"\nA planta ID = %d morreu de pragas",get_ID(get_HortaFigura(Atingido)));
                    }
                    reporta_figura(txt,get_HortaFigura(Atingido));//info da Horta = FIGURA
                    fprintf(txt,"\n%g Porcento atingida", Porcentagem);
                }
            }
        }
    }
    else fprintf(txt,"\nA praga nao atingiu plantas\n");
    //Inserindo retangulo que demarca a borda
    Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
    set_ret_dasharray(Borda,4.0);
    Circulo ponto = criaCirc(-1,X,Y,ratio,"red","none");
    escreveCirculoSvg(svg,ponto);
    escreveRetanguloSvg(svg,Borda);
    killCirc(ponto);
    killLst(Atingidos);
    Executa_ListaFormas(Pragas);
    killRet(Borda);
}

bool checaDentro75(void* Atingido,void* Praga_Atual,double* acerto){
    Forma Figura = get_HortaFigura(Atingido);

    double PragaX = get_circ_cx(Praga_Atual);
    double PragaY = get_circ_cy(Praga_Atual);
    double PragaR = get_circ_r(Praga_Atual);
    if(get_type(Figura)=='C'){//ABOBORA
        double DistCentros = sqrt(pow(get_x(Figura)-PragaX,2)+pow(get_y(Figura)-PragaY,2));
        double MenorRaio =  get_circ_r(Figura);//Precisa-se saber quais sao os raios maiores e menores
        double MaiorRaio = PragaR;
        if(get_circ_r(Figura) > PragaR){
            MenorRaio = PragaR;
            MaiorRaio = get_circ_r(Figura);
        }
        if(DistCentros + MenorRaio < MaiorRaio){//caso a distancia entre centros+Raio, seja menor que MaiorRaio esta dentro
            if(MaiorRaio == PragaR)*acerto += 1;
            *acerto += (pi*pow(PragaR,2))/(pi*pow(get_circ_r(Figura),2));//Acerto recebe % baseado em quanto a praga acertou
            return true;
        }
        else return false;
    }
    else if(get_type(Figura)=='R' && !IsColheitadeira(Figura)){//REPOLHO
        if(DentroRegiaoRet(PragaX,PragaY,get_x(Figura),get_y(Figura),
        get_x(Figura)+get_ret_larg(Figura),get_y(Figura)+get_ret_alt(Figura))){//Se o ponto central da praga estiver no retangulo
            //Caso o CIRCULO tenha ALGUMA parte fora do RETANGULO
            if(PragaX + PragaR >= get_x(Figura)+get_ret_larg(Figura) || PragaX - PragaR <= get_x(Figura) ||
            PragaY + PragaR >= get_y(Figura)+get_ret_alt(Figura) || PragaY-PragaR <= get_y(Figura))return false;
            // caso o RETANGULO esteja TOTALMENTE DENTRO do CIRCULO
            else if(PragaX + PragaR >= get_x(Figura)+get_ret_larg(Figura) && PragaX - PragaR <= get_x(Figura) &&
            PragaY + PragaR >= get_y(Figura)+get_ret_alt(Figura) && PragaY-PragaR <= get_y(Figura)){
                (*acerto) += 1;
                return true;
            }
            else{
                (*acerto) += (pi*pow(PragaR,2))/(get_ret_alt(Figura)*get_ret_larg(Figura));
                return true;
            }
        }
        else return false;
    }
    else if(get_type(Figura)=='T'){//Morango,cenoura,cebola,mato
        double distancia = sqrt(pow(get_x(Figura)-PragaX,2)+pow(get_y(Figura)-PragaY,2));
        if(distancia < PragaR){//Se a distancia entre o centro da praga e o texto for menor que o raio esta dentro
            *acerto += 0.1;
            return true;
        }
        else return false; 
    }
    else if(get_type(Figura)=='L'){//mato
        double DistCentro1 = sqrt(pow(get_lin_x1(Figura)-PragaX,2)+
        pow(get_lin_y1(Figura)-PragaY,2));
        double DistCentro2 = sqrt(pow(get_lin_x2(Figura)-PragaX,2)+
        pow(get_lin_y2(Figura)-PragaY,2));
        //Verifico se a distancia entre o centro da praga e as extremidades da reta sao menores que o Diametro se sim esta dentro
        if(DistCentro1+DistCentro2 < 2 * PragaR){
            *acerto += 0.1;
            return true;
        }
        else return false; 
    }
    else{
        printf("\nFIGURA DESCONHECIDA");
        return false;
    }
}

void Cure(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double weight,double height,double ratio){
    int NumCircV = round(height/(2 *ratio));//Aproximo o numero de circulos inteiros que cabem na vertical do retangulo
    int NumCircH = round(weight/(2 * ratio)); //Aproximo o numero de circulos inteiros que cabem na horizontal do retangulo
    Lista Curas = createLst(-1);

    for(int i = 0; i < NumCircH; i++){
        for(int j = 0; j < NumCircV; j++){
            Circulo inseticida = criaCirc(-1,(X+ratio)+(i * 2 * ratio),(Y+ratio)+(j * 2 * ratio),ratio,"yellow","none");
            insertLst(Curas,inseticida);
        }
    }
    Lista Atingidos = createLst(-1);
    /*A busca adotada consiste em fixar em uma hortaliça e percorrer por todas as curas(circulos)
    analisando quantos% da figura foi atingida pelos circulos*/
    if(getNodesDentroRegiaoRadialT(root,X,Y,X+weight,Y+height,Atingidos)){
        Iterador W = createIterator(Atingidos,false);
        while(!isIteratorEmpty(Atingidos,W)){//checa como uma matriz

            Node No_atual = getIteratorNext(Atingidos,W);//Lista de nodes
            Horta Atingido = getInfoRadialT(No_atual);//Info do node = HORTA
            Iterador K = createIterator(Curas,false);
            double Porcentagem = 0.0;
            if(CheckArea(Atingido,X,Y,X+weight,Y+height)){
            while(!isIteratorEmpty(Curas,K)){//travo em uma figura e percorro todas as curas acumulando
                Circulo inseticida_atual = getIteratorNext(Curas,K);
                checaDentro75(Atingido,inseticida_atual,&Porcentagem);//acumula curas
            }
            if(get_HortPraga(Atingido) - Porcentagem >= 0.0)set_HortPraga(Atingido,get_HortPraga(Atingido)-Porcentagem);
            else set_HortPraga(Atingido,0);
            if(Porcentagem!= 0.0){
                reporta_figura(txt,get_HortaFigura(Atingido));//info da Horta = FIGURA
                fprintf(txt,"\n%g Porcento atingida", Porcentagem);
            }
            }
        }
    }
    else fprintf(txt,"\nO inseticida nao atingiu plantas\n");
    //Inserindo retangulo que demarca a borda
    Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
    set_ret_dasharray(Borda,4.0);
    Circulo ponto = criaCirc(-1,X,Y,ratio,"yellow","none");
    escreveCirculoSvg(svg,ponto);
    escreveRetanguloSvg(svg,Borda);
    killCirc(ponto);
    killLst(Atingidos);
    Executa_ListaFormas(Curas);
    killRet(Borda);
}

void fertilizing(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double weight,double height,double ratio){
    int NumCircV = round(height/(2 *ratio));//Aproximo o numero de circulos inteiros que cabem na vertical do retangulo
    int NumCircH = round(weight/(2 * ratio)); //Aproximo o numero de circulos inteiros que cabem na horizontal do retangulo
    Lista Adubos = createLst(-1);

    for(int i = 0; i < NumCircH; i++){
        for(int j = 0; j < NumCircV; j++){
            Circulo adubo = criaCirc(-1,(X+ratio)+(i * 2 * ratio),(Y+ratio)+(j * 2 * ratio),ratio,"green","none");
            insertLst(Adubos,adubo);
        }
    }

    Lista Atingidos = createLst(-1);
    
    if(getNodesDentroRegiaoRadialT(root,X,Y,X+weight,Y+height,Atingidos)){
        Iterador W = createIterator(Atingidos,false);
        while(!isIteratorEmpty(Atingidos,W)){//checa como uma matriz

            Node No_atual = getIteratorNext(Atingidos,W);//Lista de nodes
            Horta Atingido = getInfoRadialT(No_atual);//Info do node = HORTA
            if(CheckArea(Atingido,X,Y,X+weight,Y+height)){
                Iterador K = createIterator(Adubos,false);
                double final = 0.0;
                while(!isIteratorEmpty(Adubos,K)){
                    Circulo adubo_atual = getIteratorNext(Adubos,K);
                    checaDentro75(Atingido,adubo_atual, &final);
                }
                if(final > 0.0){
                    reporta_figura(txt,get_HortaFigura(Atingido));
                    set_HortAdubo(Atingido,get_HortAdubo(Atingido)+final);
                }
                fprintf(txt,"\n%g Porcento atingida", final);
            }
        }
    }
    else fprintf(txt,"\nO fertilizante nao atingiu plantas\n");
    //Inserindo retangulo que demarca a borda
    Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
    set_ret_dasharray(Borda,5.0);
    escreveRetanguloSvg(svg,Borda);
    Circulo ponto = criaCirc(-1,X,Y,ratio,"green","none");
    escreveCirculoSvg(svg,ponto);
    killCirc(ponto);
    killLst(Atingidos);
    Executa_ListaFormas(Adubos);
    killRet(Borda);
}

bool CheckArea(void* atual, double x1, double y1, double x2, double y2){
    Forma Figura = (Forma)atual;
    
    if(get_type(Figura)=='R'){
        if(get_ret_x(Figura)+get_ret_larg(Figura) > x2)return false;
        if(get_ret_y(Figura)+get_ret_alt(Figura) > y2)return false;
        if(strcmp(get_ret_corp(Figura),"none")==0)return false;
        else return true;
    }
    else if(get_type(Figura)=='C'){
        if(get_circ_cx(Figura)+get_circ_r(Figura) > x2)return false;
        if(get_circ_cx(Figura)-get_circ_r(Figura) < x1)return false;
        if(get_circ_cy(Figura)+get_circ_r(Figura) > y2)return false;
        if(get_circ_cy(Figura)-get_circ_r(Figura) < y1)return false;
        else return true;
    }
    else if(get_type(Figura)=='L'){
        if(get_lin_x2(Figura) > x2 || get_lin_x2(Figura) < x1) return false;
        if(get_lin_y2(Figura) > y2 || get_lin_y2(Figura) < y1) return false;
        else return true;
    }
    else return true;//Textos sempre estao dentro do getnodesregiaoradialT
}

void seeding(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double weight,double height,double factor,double Dx, double Dy, int j){
    Lista Atingidos = createLst(-1);
    int hortalicas[7] = {0,0,0,0,0,0,0};
   if(getNodesDentroRegiaoRadialT(root,X,Y,X+weight,Y+height,Atingidos)){
        Iterador W = createIterator(Atingidos,false);
        while(!isIteratorEmpty(Atingidos,W)){//checa como uma matriz
            Node No_atual = getIteratorNext(Atingidos,W);//Lista de nodes
            Horta Planta = getInfoRadialT(No_atual);//Info do node = HORTA
            if(CheckArea(Planta,X,Y,X+weight,Y+height)){

            if(!IsColheitadeira(get_HortaFigura(Planta)))reporta_figura(txt,get_HortaFigura(Planta));//info da Horta = FIGURA
            if(get_HortType(Planta) == 'S')hortalicas[0]++;//Morangos na area
            else if(get_HortType(Planta) == 'O')hortalicas[1]++;//Cebolas na area
            else if(get_HortType(Planta) == 'C')hortalicas[2]++;//Cenouras na area
            else if(get_HortType(Planta) == 'P')hortalicas[3]++;//Aboboras na area
            else if(get_HortType(Planta) == 'R' && !IsColheitadeira(get_HortaFigura(Planta)))hortalicas[4]++;//Repolhos na area
            else if(get_HortType(Planta) == 'G'){//Matos em geral na area
                if(get_type(get_HortaFigura(Planta)) == 'T')hortalicas[5]++;//Mato texto "#"
                else hortalicas[6]++;//Mato linha
            }
            }
        }
        fprintf(txt,"\n\nSementes geradas:");
        sementesGeradas(txt,root,Atingidos,hortalicas,factor,X+Dx,Y+Dy,X+Dx+weight,Y+Dy+height,j);
    }
    else fprintf(txt,"\nNenhuma planta estava na area de soltar sementes\n");

    Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
    set_ret_dasharray(Borda,4.0);
    Circulo Ponto = criaCirc(-1,X,Y,2,"red","red");
    Retangulo Paralelo = criaRect(-1,X+Dx,Y+Dy,weight,height,"red","none");
    Circulo PontoParalelo = criaCirc(-1,X+Dx,Y+Dy,2,"red","red");
    set_ret_dasharray(Paralelo,4.0);
    escreveRetanguloSvg(svg,Borda);
    escreveRetanguloSvg(svg,Paralelo);
    escreveCirculoSvg(svg,Ponto);
    escreveCirculoSvg(svg,PontoParalelo);
    killRet(Borda);
    killRet(Paralelo);
    killCirc(Ponto);
    killLst(Atingidos);
}

void sementesGeradas(FILE*txt,RadialTree root,Lista atingidos,int quantidade[7],double factor, double x1,double y1,double x2,double y2,int j){
    srand(time(NULL));
    int ID = j;
    for(int i = 0; i < 7 ; i ++){
        int plantasGeradas = floor(quantidade[i] * factor);
        for(int j = 0; j < plantasGeradas ; j++){
            double newX = x1 + (double)(rand()%(int)(x2-x1));//maior que x1 menor que x2
            double newY = y1 + (double)(rand()%(int)(y2-y1));//maior que x1 menor que x2
            if(i==0){
                Texto morango = criaTexto(ID,newX,newY,"none","#FFAAEE","m","*");
                reporta_figura(txt,morango);
                insertRadialT(root,newX,newY,criaHortalica(morango));
            }
            else if(i==1){
                Texto cebola = criaTexto(ID,newX,newY,"none","#6600FF","m","@");
                reporta_figura(txt,cebola);
                insertRadialT(root,newX,newY,criaHortalica(cebola));
            }
            else if(i==2){
                Texto cenoura = criaTexto(ID,newX,newY,"none","#D4AA00","m","%");
                reporta_figura(txt,cenoura);
                insertRadialT(root,newX,newY,criaHortalica(cenoura));
            }
            else if(i==3){
               Iterador K = createIterator(atingidos,false);
               Horta Planta;
               while(!isIteratorEmpty(atingidos,K)){
                    Node No_atual = getIteratorNext(atingidos,K);//Lista de nodes
                    Planta = getInfoRadialT(No_atual);
                    if(get_HortType(Planta) == 'P')break;
               } 
               Circulo abobora = criaCirc(ID,newX,newY,7,"#803300","#FF6600");
               reporta_figura(txt,abobora);
               insertRadialT(root,newX,newY,criaHortalica(abobora));
               killIterator(atingidos,K);
            }
            else if(i==4){
               Iterador K = createIterator(atingidos,false);
               Horta Planta;
               while(!isIteratorEmpty(atingidos,K)){
                    Node No_atual = getIteratorNext(atingidos,K);//Lista de nodes
                    Planta = getInfoRadialT(No_atual);
                    if(get_HortType(Planta) == 'R' && !IsColheitadeira(get_HortaFigura(Planta)))break;
               } 
                Retangulo repolho = criaRect(ID,newX,newY,get_ret_larg(get_HortaFigura(Planta)),get_ret_alt(get_HortaFigura(Planta)),"#447821","#71C837");
                reporta_figura(txt,repolho);
                insertRadialT(root,newX,newY,criaHortalica(repolho));
                killIterator(atingidos,K);
            }
            else if(i==5){
                Texto mato = criaTexto(ID,newX,newY,"black","#D38D5F","m","#");
                reporta_figura(txt,mato);
                insertRadialT(root,newX,newY,criaHortalica(mato));
            }
            else if(i==6){
               Iterador K = createIterator(atingidos,false);
               Horta Planta;
               while(!isIteratorEmpty(atingidos,K)){
                    Node No_atual = getIteratorNext(atingidos,K);//Lista de nodes
                    Planta = getInfoRadialT(No_atual);
                    if(get_type(get_HortaFigura(Planta))=='L')break;
               }
               double distanciaX = (get_lin_x1(get_HortaFigura(Planta)) - get_lin_x2(get_HortaFigura(Planta)));
               double distanciaY = (get_lin_y2(get_HortaFigura(Planta)) - get_lin_y1(get_HortaFigura(Planta)));
               Texto matoLinha = criaLinha(ID,newX,newY,newX+distanciaX,newY+distanciaY,"#008000");
               reporta_figura(txt,matoLinha);
               insertRadialT(root,newX,newY,criaHortalica(matoLinha));
               killIterator(atingidos,K);
            }
            ID++;
        }
    }
}

void reportaDados(FILE* txt,RadialTree root,int ID){
    Horta busca = Teste_achaIDNaArvore(root,ID);
    if(busca == NULL)return;
    Forma Reportada = get_HortaFigura(busca);

    reporta_figura(txt,Reportada);
}

void ReportaColheitadeiras(FILE* txt,Lista _Colheitadeiras){
    Lista aux = map(_Colheitadeiras,get_HortaFigura);
    fold(aux,reporta_figura,txt);
    killLst(aux);
}

void setColheitadeira(RadialTree root,int ID,Lista _Colheitadeiras){
    Horta Ret = Teste_achaIDNaArvore(root,ID);
    Retangulo Colheitadeira = get_HortaFigura(Ret);
    set_Colheita(Colheitadeira,true);
    insertLst(_Colheitadeiras,Ret);
}

void atualizaPeso(void* hortalica){
    int adubo = (int)floor(get_HortAdubo(hortalica));//floor esta aproximando 1 para 0
    double pAtual = get_HortaPeso(hortalica) + get_HortaPeso(hortalica)* 0.1*adubo - get_HortaPeso(hortalica)* get_HortPraga(hortalica);//peso original + 10%do Po a cada 100% adubado - (%praga-%cura)
    //apos aplicar mudancas é necessario resetar hortalica
    set_HortAdubo(hortalica,0);
    set_HortPraga(hortalica,0);

    set_HortaP_Atual(hortalica, pAtual);
}