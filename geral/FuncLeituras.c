#include "FuncLeituras.h"
#include <string.h>
#include <stdlib.h>
#include "arqsvg.h"
#include "aplicacoes.h"
#include <math.h>

typedef struct Cont{
    double morango;
    double cebola;
    double cenoura;
    double abobora;
    double mato;
    double repolho;
}_contHort;

void LinhaMove(FILE* txt,RadialTree root,int ID,double X,double Y){
    Horta _hortalica = Teste_achaIDNaArvore(root,ID);
    Forma imagem = get_HortaFigura(_hortalica);

    if(_hortalica != NULL){
    removeNoRadialT(root,getNodeRadialT(root,get_x(imagem),get_y(imagem),0.0000001));
    
    reporta_figura(txt,imagem);
    fprintf(txt,"\nCOORDENADA INICIAL = (%lf,%lf)", get_x(imagem), get_y(imagem));
    
    mvForma(imagem, X,Y);//forma soma as coordenadas antigas às inseridas
   
    fprintf(txt,"\nCOORDENADA FINAL = (%lf,%lf)\n", get_x(imagem), get_y(imagem));

    insertRadialT(root,get_x(imagem),get_y(imagem),_hortalica);
    }
    else printf("\nERRO NA ENTRADA DA FUNCAO [*]mv -> ID %d NAO EXISTE\n",ID);
}

double Harvest(FILE* txt,FILE* svg, RadialTree root,Lista Colhidos,int ID, int passos, char* cardial){
    Lista Nos_Colhidos = createLst(-1);
    Horta _Colhehorta = Teste_achaIDNaArvore(root,ID);
    Retangulo Colheitadeira = get_HortaFigura(_Colhehorta);
    Retangulo Borda;
    double Contabilidade = 0.0;
    if(Colheitadeira != NULL && IsColheitadeira(Colheitadeira)){
    double Xo = get_x(Colheitadeira);//X inicial e Y inicial
    double Yo = get_y(Colheitadeira);

    if(strcmp(cardial,"n")==0){//Caso a colheiradeira vá para o norte
        LinhaMove(txt,root,ID,0.0,-passos*get_ret_alt(Colheitadeira));
        Contabilidade += AnaliseDeColheita(txt,root,get_ret_x(Colheitadeira),get_ret_y(Colheitadeira),
        Xo+get_ret_larg(Colheitadeira),Yo+get_ret_alt(Colheitadeira),Nos_Colhidos,Colhidos);
        
        Borda = criaRect(-1,get_ret_x(Colheitadeira),get_ret_y(Colheitadeira),get_ret_larg(Colheitadeira),(1+passos)*get_ret_alt(Colheitadeira), "red", "none");
        set_ret_dasharray(Borda,5);
        escreveRetanguloSvg(svg,Borda);
        killRet(Borda);
    }

    else if(strcmp(cardial,"s")==0){
        LinhaMove(txt,root,ID,0.0,(passos)*get_ret_alt(Colheitadeira));
        Contabilidade += AnaliseDeColheita(txt,root,Xo,Yo,get_ret_x(Colheitadeira)+get_ret_larg(Colheitadeira),
        get_ret_y(Colheitadeira)+get_ret_alt(Colheitadeira),Nos_Colhidos,Colhidos);
        
        Borda = criaRect(-1,Xo,Yo,get_ret_larg(Colheitadeira),(1+passos)*get_ret_alt(Colheitadeira), "red", "none");
        set_ret_dasharray(Borda,5);
        escreveRetanguloSvg(svg,Borda);
        killRet(Borda);
    }
    else if(strcmp(cardial,"l")==0){ 
        LinhaMove(txt,root,ID,passos*get_ret_larg(Colheitadeira),0.0);
        Contabilidade += AnaliseDeColheita(txt,root,Xo,Yo,get_ret_x(Colheitadeira)+get_ret_larg(Colheitadeira),
        get_ret_y(Colheitadeira)+get_ret_alt(Colheitadeira),Nos_Colhidos,Colhidos);

        Borda = criaRect(-1,Xo,Yo,(1+passos)*get_ret_larg(Colheitadeira),get_ret_alt(Colheitadeira), "red", "none");
        set_ret_dasharray(Borda,5);
        escreveRetanguloSvg(svg,Borda);
        killRet(Borda);
    }

    else if(strcmp(cardial,"o")==0){
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
    fprintf(txt,"\n%d itens foram colhidos!",lengthLst(Nos_Colhidos)-1);
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
            if(!IsColheitadeira(checkFarm)){
                atualizaPeso(hortalica);
                insertLst(ColhidosTotal,hortalica);
                contabilidade += get_HortaP_Atual(hortalica);
                reporta_figura(txt,checkFarm);//reporta no txt
                removeNoRadialT(root,removido);//remove nó
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
            Iterador K = createIterator(Pragas,false);
            double Porcentagem = 0.0;
            while(!isIteratorEmpty(Pragas,K)){
                Circulo praga_atual = getIteratorNext(Pragas,K);
                if(checaDentro75(Atingido,praga_atual,&Porcentagem)){

                set_HortPraga(Atingido,get_HortPraga(Atingido)+Porcentagem);
                if(get_HortPraga(Atingido) > 0.75){
                    Texto morte = criaTexto(-1,get_x(get_HortaFigura(Atingido)),get_y(get_HortaFigura(Atingido)),"#FF0000","#FF0000","m","x");
                    escreveTextoSvg(svg,morte);
                    removeNoRadialT(root,No_atual);
                    killTexto(morte); 
                    fprintf(txt,"\nA planta ID = %d morreu de pragas",get_ID(get_HortaFigura(Atingido)));
                    break;
                }
                }
            }
            if(Porcentagem != 0.0){
                reporta_figura(txt,get_HortaFigura(Atingido));//info da Horta = FIGURA
                fprintf(txt,"\n%g Porcento atingida", Porcentagem);
            }
        }
    }
    else printf("\nA praga nao atingiu plantas\n");
    //Inserindo retangulo que demarca a borda
    Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
    set_ret_dasharray(Borda,4.0);
    insertLst(Pragas,Borda);
    fold(Pragas,escreveGeralSvgLista,svg);
    killLst(Atingidos);
    Executa_ListaFormas(Pragas);
}

bool checaDentro75(void* Atingido,void* Praga_Atual,double* acerto){
    Forma Figura = get_HortaFigura(Atingido);
    double PragaX = get_circ_cx(Praga_Atual);
    double PragaY = get_circ_cy(Praga_Atual);
    double PragaR = get_circ_r(Praga_Atual);
    if(get_type(Figura)=='C'){//ABOBORA
        double DistCentros = sqrt(pow(get_x(Figura)-PragaX,2)+pow(get_y(Figura)-PragaY,2));
        double MenorRaio = PragaR;//Precisa-se saber quais sao os raios maiores e menores
        double MaiorRaio = PragaR;
        if(get_circ_r(Figura) > PragaR){
            MenorRaio = PragaR;
            MaiorRaio = get_circ_r(Figura);
        }
        if(DistCentros + MenorRaio < MaiorRaio){//caso a distancia entre centros+Raio, seja menor que MaiorRaio esta dentro
            *acerto += pi*pow(PragaR,2)/pi*pow(get_circ_r(Figura),2);//Acerto recebe % baseado em quanto a praga acertou
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
            //caso o RETANGULO esteja TOTALMENTE DENTRO do CIRCULO
            // else if(PragaX + PragaR >= get_x(Figura)+get_ret_larg(Figura) && PragaX - PragaR <= get_x(Figura) &&
            // PragaY + PragaR >= get_y(Figura)+get_ret_alt(Figura) && PragaY-PragaR <= get_y(Figura)){
            //     (*acerto) =+ 0.1;
            //     return true;
            // }
            //Caso o CIRCULO esteja TOTALMENTE DENTRO do RETANGULO
            else{
                (*acerto) =+ pi*pow(PragaR,2)/get_ret_alt(Figura)*get_ret_larg(Figura);
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
    /*A busca adotada consiste em fixar em uma hortaliça e percorrer por todas as pragas(circulos)
    analisando quantos% da figura foi atingida pelos circulos*/
    if(getNodesDentroRegiaoRadialT(root,X,Y,X+weight,Y+height,Atingidos)){
    Iterador W = createIterator(Atingidos,false);
        while(!isIteratorEmpty(Atingidos,W)){//checa como uma matriz

            Node No_atual = getIteratorNext(Atingidos,W);//Lista de nodes
            Horta Atingido = getInfoRadialT(No_atual);//Info do node = HORTA
            reporta_figura(txt,get_HortaFigura(Atingido));//info da Horta = FIGURA
            Iterador K = createIterator(Curas,false);
            double Porcentagem = 0.0;
            while(!isIteratorEmpty(Curas,K)){
                Circulo inseticida_atual = getIteratorNext(Curas,K);
                checaDentro75(Atingido,inseticida_atual,&Porcentagem);
            }
            set_HortCura(Atingido,get_HortCura(Atingido)+Porcentagem);
            fprintf(txt,"\n%g Porcento atingida", Porcentagem);
        }
    }
    else printf("\nO inseticida nao atingiu plantas\n");
    //Inserindo retangulo que demarca a borda
    Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
    set_ret_dasharray(Borda,5.0);
    insertLst(Curas,Borda);
    fold(Curas,escreveGeralSvgLista,svg);
    killLst(Atingidos);
    Executa_ListaFormas(Curas);
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
            reporta_figura(txt,get_HortaFigura(Atingido));//info da Horta = FIGURA
            Iterador K = createIterator(Adubos,false);
            while(!isIteratorEmpty(Adubos,K)){
                Circulo adubo_atual = getIteratorNext(Adubos,K);
                if(checaDentroAdubo(Atingido,adubo_atual)){
                    set_HortAdubo(Atingido,get_HortAdubo(Atingido)+0.1);//aumenta a area adubada em 10%
                    reporta_figura(txt,get_HortaFigura(Atingido));
                }
            }
        }
    }
    else printf("\nO fertilizante nao atingiu plantas\n");
    //Inserindo retangulo que demarca a borda
    Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
    set_ret_dasharray(Borda,5.0);
    insertLst(Adubos,Borda);
    fold(Adubos,escreveGeralSvgLista,svg);
    killLst(Atingidos);
    Executa_ListaFormas(Adubos);
}

bool checaDentroAdubo(void* atingida, void* adubo){
    Forma Figura = get_HortaFigura(atingida);

    if(get_type(Figura)=='C'){//ABOBORA
        
    }
    else if(get_type(Figura)=='R' && !IsColheitadeira(Figura)){//REPOLHO

    }
    else if(get_type(Figura)=='T'){//Morango,cenoura,cebola,mato
        double distancia = sqrt(pow(get_x(Figura)-get_x(adubo),2)+
        pow(get_y(Figura)-get_y(adubo),2));
        if(distancia <= get_circ_r(adubo))return true;
        else return false; 
    }
    else if(get_type(Figura)=='L'){//mato
        double tamanhoLin = sqrt(pow(get_lin_x1(Figura)-get_lin_x2(adubo),2)+
        pow(get_lin_y1(Figura)-get_lin_y2(adubo),2));

    }
}

void seeding(FILE* txt,FILE* svg, RadialTree root,double X,double Y,double weight,double height,double factor,double Dx, double Dy, int j){
    Lista Atingidos = createLst(-1);
    if(getNodesDentroRegiaoRadialT(root,X,Y,X+weight,Y+height,Atingidos)){
    
    }

    Retangulo Borda = criaRect(-1,X,Y,weight,height,"red","none");
    set_ret_dasharray(Borda,4.0);
    Circulo Ponto = criaCirc(-1,X,Y,1,"red","red");
    Retangulo Paralelo = criaRect(-1,X+Dx,Y+Dy,weight,height,"red","none");
    escreveRetanguloSvg(svg,Borda);
    escreveRetanguloSvg(svg,Paralelo);
    escreveCirculoSvg(svg,Ponto);
    killRet(Borda);
    killRet(Paralelo);
    killCirc(Ponto);
}

void reportaDados(FILE* txt,RadialTree root,int ID){
    Horta busca = Teste_achaIDNaArvore(root,ID);
    if(busca == NULL)return;
    Forma Reportada = get_HortaFigura(busca);

    reporta_figura(txt,Reportada);
}

void ReportaColheitadeiras(FILE* txt,Lista _Colheitadeiras){
    fold(map(_Colheitadeiras,get_HortaFigura),reporta_figura,txt);
}

void setColheitadeira(RadialTree root,int ID,Lista _Colheitadeiras){
    Horta Ret = Teste_achaIDNaArvore(root,ID);
    Retangulo Colheitadeira = get_HortaFigura(Ret);
    set_Colheita(Colheitadeira,true);
    insertLst(_Colheitadeiras,Ret);
}

void atualizaPeso(void* hortalica){
    int adubo = (int)floor(get_HortAdubo(hortalica));
    double degradado = get_HortPraga(hortalica) - get_HortCura(hortalica);
    if(degradado < 0)degradado = 0;//caso tenha curado ela mais que o necessario(degradado<0) nao estara degradada
    double pAtual = get_HortaPeso(hortalica) + get_HortaPeso(hortalica)* 0.1*adubo - get_HortaPeso(hortalica)* degradado;//peso original + 10%do Po a cada 100% adubado - (%praga-%cura)

    set_HortaP_Atual(hortalica, pAtual);
}