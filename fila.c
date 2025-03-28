#include<stdio.h>
#include<stdlib.h>

typedef struct NO{
    int num;
    struct NO* prox;
    struct NO* ant;
}NO;

typedef struct FILA{
    NO* primeiro;
    NO* ultimo;
    int tam;
}FILA;

NO* inicializarNO(int num){
    NO* n = (NO*) malloc(sizeof(NO));
    if(n==NULL){
        printf("\nErro ao alocar o NO\n");
        return NULL;
    }
    n->num = num;
    n->prox = NULL;
    n->ant = NULL;
    return n;
}

FILA* inicializarFila(){
    FILA* f = (FILA*) malloc(sizeof(FILA));
    if(f==NULL){
        printf("\nErro na alocação da fila\n");
        return NULL;
    }
    f->primeiro = NULL;
    f->ultimo = NULL;
    f->tam = 0;
    return f;
}

void inserirFila(FILA *f,int num){
    if(f==NULL){
        printf("\nNo de fila inválido\n");
        return;
    }
    NO *novo_no = inicializarNO(num);
    if(f->primeiro==NULL){
        f->primeiro = novo_no;
        f->ultimo = novo_no;
        f->tam++;
        printf("Primeiro NO inserido na fila\n");
        return;
    }
    f->ultimo->prox = novo_no;
    novo_no->ant = f->ultimo;
    f->ultimo = novo_no;
    f->tam++;

    return;
}

void percorrerListaInicio(FILA *f){
    if(f==NULL){
        printf("\nNo de fila inválido\n");
        return;
    }
    NO* noAux = f->primeiro;
    int i = 0;
    while(noAux != NULL){
        printf("%d:%d \n",i,noAux->num);
        noAux = noAux->prox;
        i++;
    }
    printf("\n");
    return;
}

void percorrerListaFinal(FILA *f){
    if(f==NULL){
        printf("\nNo de fila inválido\n");
        return;
    }
    NO* noAux = f->ultimo;
    while(noAux != NULL){
        printf("%d ", noAux->num);
        noAux = noAux->ant;
    }
    printf("\n");
    return;
}

void removerElemento(FILA *f, int chave){
    if(f == NULL){
        printf("\nNo de fila inválido\n");
        return;
    }

    // Remover o primeiro nó
    if (f->primeiro != NULL && f->primeiro->num == chave) {
        NO* temp = f->primeiro;
        f->primeiro = f->primeiro->prox;
        if (f->primeiro != NULL) {
            f->primeiro->ant = NULL;
        } else {
            f->ultimo = NULL;  // Se a fila ficar vazia
        }
        free(temp);
        f->tam--;
        printf("Chave %d removida da fila\n", chave);
        return;
    }

    // Remover o último nó
    if (f->ultimo != NULL && f->ultimo->num == chave) {
        NO* temp = f->ultimo;
        f->ultimo = f->ultimo->ant;
        if (f->ultimo != NULL) {
            f->ultimo->prox = NULL;
        } else {
            f->primeiro = NULL;  // Se a fila ficar vazia
        }
        free(temp);
        f->tam--;
        printf("Chave %d removida da fila\n", chave);
        return;
    }

    // Remover do meio
    NO* noAux = f->primeiro;
    while(noAux != NULL){
        if(noAux->num == chave){
            noAux->ant->prox = noAux->prox;
            if(noAux->prox != NULL) {
                noAux->prox->ant = noAux->ant;
            }
            free(noAux);
            f->tam--;
            printf("Chave %d removida da fila\n", chave);
            return;
        }
        noAux = noAux->prox;
    }

    printf("Chave %d não encontrada na fila\n", chave);
}


NO* buscarNo(FILA *f, int chave){
    if(f==NULL){
        printf("\nNo de fila inválido\n");
        return NULL;
    }

    NO* noAux = f->primeiro;
    while(noAux != NULL){
        if(noAux->num == chave){
            printf("No encontrado\n");
            return noAux;
        }
        noAux = noAux->prox;
    }
    printf("Chave nao encontrada\n");
    return NULL;
}

void liberarFila(FILA *f) {
    NO* atual = f->primeiro;
    while (atual != NULL) {
        NO* prox = atual->prox;
        free(atual);
        atual = prox;
    }
    free(f);
}

void insercaoOrdenada(FILA *f, int novoNum) {
    if (f == NULL) {
        printf("\nNo de fila inválido\n");
        return;
    }
    NO* novoNO = inicializarNO(novoNum);
    
    if (f->primeiro == NULL) {  // Fila vazia
        f->primeiro = novoNO;
        f->ultimo = novoNO;
        f->tam++;
        return;
    }

    // Inserção no início
    if (novoNum < f->primeiro->num) {
        novoNO->prox = f->primeiro;
        f->primeiro->ant = novoNO;
        f->primeiro = novoNO;
        f->tam++;
        return;
    }

    // Inserção no meio ou final
    NO* noAux = f->primeiro;
    while (noAux->prox != NULL && novoNum >= noAux->num) {
        noAux = noAux->prox;
    }

    // Inserir no meio
    if (noAux->prox != NULL) {
        novoNO->prox = noAux->prox;
        novoNO->ant = noAux;
        noAux->prox->ant = novoNO;
        noAux->prox = novoNO;
    }
    // Inserir no final
    else {
        f->ultimo->prox = novoNO;
        novoNO->ant = f->ultimo;
        f->ultimo = novoNO;
    }

    f->tam++;
}

void inserirFilaIndex(FILA* f, int num, int Index){
    if(f==NULL){
        printf("\nNo de fila inválido\n");
        return;
    }
    if(Index>f->tam || Index<0){
        printf("Index invalido, tente um index na faixa (%d - %d)", 0,f->tam-1);
        return;
    }
    NO* novoNo = inicializarNO(num);
    if(novoNo == NULL){
        printf("Erro alocacao\n");
        return;  // Caso a alocação falhe
    }

    if(Index==0){
        printf("Insercao no index 0\n");
        f->primeiro->ant = novoNo;
        novoNo->prox = f->primeiro;
        f->primeiro = novoNo;
        f->tam++;
        return;
    }
    if(Index==f->tam){ 
        f->ultimo->prox = novoNo;
        novoNo->ant = f->ultimo;
        f->ultimo = novoNo;
        f->tam++;
        return;
    }

    //identificando qual o caminho mais rápido e posicionando o noAux no ponto correto
    NO *noAux = NULL;
    if(Index < f->tam/2){
        noAux = f->primeiro;
        for(int i=Index;i>0; i--){
            noAux = noAux->prox;
        }
    }else{
        noAux = f->ultimo;
        for(int i = f->tam-Index-1; i > 0; i--){
            noAux = noAux->ant;
        }
    }

    // Inserção no meio
    novoNo->prox = noAux;
    novoNo->ant = noAux->ant;

    noAux->ant->prox = novoNo;
    noAux->ant = novoNo;

    f->tam++;

    return;

}

int main (){
    FILA *f = inicializarFila();
    for(int i = 10; i > 0; i--){
        inserirFila(f,i);
    }

    percorrerListaInicio(f);

    inserirFilaIndex(f,110,0);
    inserirFilaIndex(f,100,1);
    inserirFilaIndex(f,150,4);
    inserirFilaIndex(f,200,10);
    inserirFilaIndex(f,250,13);
    printf("%d\n",f->tam);
    inserirFilaIndex(f,190,f->tam);

    percorrerListaInicio(f);
    
    return 0;
}