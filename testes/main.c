#include <stdio.h>
#include <stdlib.h>

void UM(int* X){
    X[0]++;
}

void DOIS(int** X){
    UM(*X);
}

int main(){
    int* X[2] = {0,0};

    printf(" %d  %d  ", *X[0],*X[1]);
    DOIS(X);

    printf(" %d  %d  ", *X[0],*X[1]);
} 