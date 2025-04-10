#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include<limits.h>

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

/*
    alocar memória para um nó informando as coordenadas de onde será inserido na faculdade
*/
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

/* Alocar memória para caber uma matriz e inializar seus ponteiros como nulo
*/
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

/* Liberar todos os nós e memória alocada da Matriz
*/
void liberarMatriz(Matriz* m) {
    for (int i = 0; i < M; i++) {
        No* atual = m->lin[i];
        while (atual != NULL) {
            No* temp = atual;
            atual = atual->direita;
            free(temp);
        }
    }
    free(m);
}

/* Insere um elemento por vez, cria o nó e insere nas coordenadas
*/
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

/* Remove o elemento informado na coordenada da matriz
*/
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

/* Printa os elementos da MAtriz, colocando 0 nos espaços vazios
*/
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

/* Informa a quantidade de elementos presentes na matriz
*/
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

/* Funcao "privada", destinada a ser utilizada por outras funções, retorna o nó da coordenada
*/
No* _buscarNo(Matriz* m, int linha, int coluna){
    No* noAux = m->lin[linha];
    while(noAux != NULL && noAux->coluna < coluna){
        noAux = noAux->direita;
    }
    if(noAux != NULL && noAux->coluna == coluna){
        return noAux;
    }
    return NULL;
}

/* Recebe as coordenadas da matriz e retorna 
*/
int consulta(Matriz *m, int linha, int coluna){
    No* no_busca = _buscarNo(m,linha,coluna);
    if(no_busca == NULL){
        printf("Não existe elemento nessa posicao\n");
        return INT_MIN;
    }else{
        printf("[%d][%d]: %d\n",no_busca->linha, no_busca->coluna,no_busca->elemento);
        return no_busca->elemento;
    }
}

/* Altera o valor de um no ja existente, se nao existir, cria um e coloca na posicao
*/
void alterar(Matriz* m, int linha, int coluna, int novo_valor){
    No* no_busca = _buscarNo(m,linha,coluna);
    if(no_busca == NULL){
        inserirElemento(m,novo_valor,linha,coluna);
    }else{
        no_busca->elemento = novo_valor;
        printf("Alterado [%d][%d]: %d \n",no_busca->linha,no_busca->coluna,no_busca->elemento);
    }
}

/* Retorna um ponteiro para a matriz transposta da recebida por parâmetro
*/
Matriz* matrizTransposta(Matriz* original){
    Matriz* transposta = inicializarMatriz();

    for(int i = 0; i < M; i++){
        No* aux = original->lin[i];

        while(aux!=NULL){
            inserirElemento(transposta, aux->elemento, aux->coluna, aux->linha);
            aux = aux->direita;
        }
    }
    return transposta;
}

/* Multiplica todos os elementos da matriz por um escalar Alpha
*/
void multiplicarPorEscalar(Matriz *m, int alpha){
    No *noAux;

    for(int i = 0; i < M; i++){
        noAux = m->lin[i];
        while(noAux != NULL){
            noAux->elemento *= alpha;
            noAux = noAux->direita;
        }
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

    printf("%d\n",qtdElementosMatriz(m));

    consulta(m,0,0);
    consulta(m,1,0);
    alterar(m,0,0,8);
    alterar(m,0,1,7);

    printarMatriz(m);

    Matriz* tp = matrizTransposta(m);

    printarMatriz(tp);

    multiplicarPorEscalar(m, 8);
    printarMatriz(m);

return 0;
}






