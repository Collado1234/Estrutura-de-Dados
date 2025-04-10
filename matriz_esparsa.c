#include <stdio.h>
#include <stdlib.h>
#include <locale.h>

#define M 5
#define N 5

typedef struct No {
    int elemento;
    int linha, coluna;
    struct No* direita;
    struct No* baixo;
} No;

typedef struct Matriz {
    struct No* lin[M];
    struct No* col[N];
} Matriz;

No* criarNo(int elemento, int linha, int coluna) {
    No* novo_no = (No*) malloc(sizeof(No));
    if (novo_no == NULL) {
        printf("Erro alocação do nó\n");
        return NULL;
    }
    novo_no->elemento = elemento;
    novo_no->linha = linha;
    novo_no->coluna = coluna;
    novo_no->direita = NULL;
    novo_no->baixo = NULL;
    return novo_no;
}

Matriz* inicializarMatriz() {
    Matriz* m = (Matriz*) malloc(sizeof(Matriz));
    if (m == NULL) {
        printf("Erro alocação da matriz\n");
        return NULL;
    }

    for (int i = 0; i < M; i++) m->lin[i] = NULL;
    for (int i = 0; i < N; i++) m->col[i] = NULL;

    return m;
}

void inserirElemento(Matriz* m, int elemento, int linha, int coluna) {
    No* novo_no = criarNo(elemento, linha, coluna);
    if (!novo_no) return;

    // --- INSERIR NA COLUNA ---
    No *auxCol = m->col[coluna], *antCol = NULL;
    while (auxCol != NULL && auxCol->linha < linha) {
        antCol = auxCol;
        auxCol = auxCol->baixo;
    }

    if (auxCol != NULL && auxCol->linha == linha) {
        printf("Posição [%d][%d] já ocupada\n", linha, coluna);
        free(novo_no);
        return;
    }

    if (antCol == NULL) {
        novo_no->baixo = m->col[coluna];
        m->col[coluna] = novo_no;
    } else {
        novo_no->baixo = auxCol;
        antCol->baixo = novo_no;
    }

    // --- INSERIR NA LINHA ---
    No *auxLin = m->lin[linha], *antLin = NULL;
    while (auxLin != NULL && auxLin->coluna < coluna) {
        antLin = auxLin;
        auxLin = auxLin->direita;
    }

    if (auxLin != NULL && auxLin->coluna == coluna) {
        printf("Posição [%d][%d] já ocupada\n", linha, coluna);
        free(novo_no); // já inserido na coluna, teria que remover de lá, mas mantemos simples por enquanto
        return;
    }

    if (antLin == NULL) {
        novo_no->direita = m->lin[linha];
        m->lin[linha] = novo_no;
    } else {
        novo_no->direita = auxLin;
        antLin->direita = novo_no;
    }
}

void removerElemento(Matriz* m, int linha, int coluna) {
    No *auxCol = m->col[coluna], *antCol = NULL;
    No *auxLin = m->lin[linha], *antLin = NULL;

    while (auxCol != NULL && auxCol->linha < linha) {
        antCol = auxCol;
        auxCol = auxCol->baixo;
    }

    while (auxLin != NULL && auxLin->coluna < coluna) {
        antLin = auxLin;
        auxLin = auxLin->direita;
    }

    if (auxCol == NULL || auxLin == NULL || auxCol->linha != linha || auxLin->coluna != coluna) {
        printf("Elemento [%d][%d] não encontrado\n", linha, coluna);
        return;
    }

    // Remover da coluna
    if (antCol == NULL)
        m->col[coluna] = auxCol->baixo;
    else
        antCol->baixo = auxCol->baixo;

    // Remover da linha
    if (antLin == NULL)
        m->lin[linha] = auxLin->direita;
    else
        antLin->direita = auxLin->direita;

    // Liberação única
    free(auxCol); // ou auxLin — são o mesmo nó
    printf("Elemento [%d][%d] removido\n", linha, coluna);
}

void printarMatriz(Matriz* m){
    No* auxLin;
    printf("\n");
    for(int i = 0; i < M; i++){
        auxLin = m->lin[i];

        for(int j = 0; j < N; j++){
            if(auxLin != NULL && j == auxLin->coluna){
                printf("%d ", auxLin->elemento);
                auxLin = auxLin->direita;
            } else {
                printf("0 ");
            }
        }
        printf("\n");
    }
}

int qtdElementosMatriz(Matriz* m){
    No *auxLin, *auxCol;
    int contX=0,contY=0;

    for(int i = 0; i < M; i++){
        auxLin = m->lin[i];

        while(auxLin != NULL){
            contX++;
            //printf("[%d][%d]\n", auxLin->linha,auxLin->coluna);
            auxLin = auxLin->direita;
        }
    }

    for(int i = 0; i < N; i++){
        auxCol = m->col[i];
        
        while(auxCol != NULL){
            contY++;
            //printf("[%d][%d]\n", auxCol->linha,auxCol->coluna);
            auxCol = auxCol->baixo;
        }
    }

    if(contX == contY){  
        return contX;
    }else{
        return -1;
    }
}


int main(){
    Matriz* m = inicializarMatriz();
    inserirElemento(m, 5, 0, 0);
    inserirElemento(m, 5, 0, 3);
    inserirElemento(m, 5, 1, 1);
    inserirElemento(m, 5, 2, 2);
    inserirElemento(m, 5, 3, 3);
    inserirElemento(m, 5, 4, 4);

    printarMatriz(m);

    //removerElemento(m,1,1);

    printarMatriz(m);

    printf("%d",qtdElementosMatriz(m));

return 0;
}






