#include<stdio.h>
#include<stdlib.h>
#include<locale.h>

#define M 5
#define N 5

typedef struct No{
    int elemento;
    int linha,coluna;
    struct No* direita;
    struct No* baixo;
}No;

typedef struct Matriz{
    struct No* lin[M];
    struct No* col[N];
}Matriz;

    // Criar um elemento nó de matriz com base nas coordenadas linha e coluhna
No *criarNo(int elemento, int linha, int coluna){
    No* novo_no = (No*) malloc(sizeof(No));
    if(novo_no == NULL){
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

Matriz* inicializarMatriz(){
    Matriz* m = (Matriz*) malloc (sizeof(Matriz));
    if(m == NULL){
        printf("Erro alocação do nó\n");
        return NULL;
    }

    int i = 0;
    for(i=0;i<M;i++){
        m->lin[i] = NULL;
    }
    for(i=0;i<N;i++){
        m->col[i] = NULL;
    }

    return m;
}

void inserirElemento(Matriz* m, int elemento, int linha, int coluna){
    No *novo_no = criarNo(elemento,linha,coluna);
    No *auxCol = m->col[coluna];
    No *auxLin = m->lin[linha];
    No *anterior = NULL; //savepoint

    //Ajustando novo no nas colunas
    if(auxCol==NULL){         //inserir em fila vazia
        m->col[coluna] = novo_no;
    }else{
        while(auxCol != NULL && auxCol->linha < novo_no->coluna){  //posiciona o nó na posição vertical correta
            anterior = auxCol;
            auxCol = auxCol->baixo;
        }

        if(auxCol!=NULL){
            if(auxCol->linha == novo_no){   //POSSIVEL CORREÇÂO PRA != NULL
                printf("Posicao ja esta ocupada\n");
                free(novo_no);
                return;
            }else{
                if(anterior == NULL){  //inserir no inicio
                    novo_no->baixo = auxCol;
                    m->col[coluna] = novo_no;
                }else{
                    anterior->baixo = novo_no; //inserir no meio
                    novo_no->baixo = auxCol;
                }
            }
        }else{
            anterior->baixo = novo_no;  //inserir no final
        }
    }

    anterior = NULL;

    if(auxLin==NULL){
        m->lin[linha] = novo_no;
    }else{
        while(auxLin == NULL && auxLin->coluna < novo_no->coluna){ //posiciona o nó na posição horizontal correta
            anterior = auxLin;
            auxLin = auxLin->direita;
        }

        if(auxLin != NULL){ 
            if(auxLin->coluna == novo_no){//POSSIVEL CORREÇÂO PRA != NULL
                printf("Posicao ja esta ocupada\n");
                free(novo_no);
                return;
            }

            if(anterior == NULL){
                novo_no->direita = auxLin;
                m->lin[linha] = novo_no;
            }else{
                anterior->direita = novo_no;
                novo_no->direita = auxLin;
            }
        }else{
            anterior->direita = novo_no;
        }
    }
}


