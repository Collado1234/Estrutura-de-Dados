#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

// Estrutura para um nó da pilha
typedef struct NODE{
    int num;             // Dado armazenado no nó
    struct NODE *prox;   // Ponteiro para o próximo nó
}NODE;

// Estrutura para a pilha
typedef struct STACK{
    NODE* topo; // Ponteiro para o topo da pilha
    int qtd;    // Quantidade de elementos na pilha
}STACK;

// Cria um novo nó com o valor fornecido
NODE* createNode(int num){
    NODE* n = (NODE*) malloc(sizeof(NODE));
    if(n == NULL){
        printf("Erro ao alocar o nó\n");
        return NULL;
    }
    n->num = num;
    n->prox = NULL;
    return n;
}

// Cria uma nova pilha vazia
STACK* createStack(){
    STACK* st = (STACK*) malloc(sizeof(STACK));
    if(st == NULL){
        printf("Erro ao alocar pilha\n");
        return NULL;
    }
    st->qtd = 0;
    st->topo = NULL;
    printf("Inicializando pilha\n");
    return st;
}

// Verifica se a pilha está vazia
bool isEmpty(STACK *st){
    return (st->topo == NULL);
}

// Verifica se a pilha é inválida (ponteiro nulo)
bool isStackNULL(STACK *st){
    if(st == NULL){
        printf("Invalid Stack Pointer\n");
        return true;
    }
    return false;
}

// Insere um elemento no topo da pilha
void push(STACK *st, int num){
    if(st == NULL){
        printf("Invalid stack pointer\n");
        return;
    }
    NODE *new_node = createNode(num);
    if(new_node == NULL) return;

    if(isEmpty(st)){
        st->topo = new_node;  // Se a pilha estiver vazia, o novo nó é o topo
    } else {
        new_node->prox = st->topo; // O novo nó aponta para o topo atual
        st->topo = new_node;       // Atualiza o topo da pilha
    }
    st->qtd++; // Incrementa a quantidade de elementos na pilha
}

// Remove e retorna o elemento do topo da pilha
int pop(STACK *st){
    if(isStackNULL(st)){
        printf("Invalid stack pointer\n");
        return -9999999; // Valor de erro
    }
    if(isEmpty(st)){
        printf("Stack is empty\n");
        return -9999999; // Valor de erro
    }
    int pop = st->topo->num; // Armazena o valor do topo
    NODE *aux = st->topo;    // Salva o ponteiro para o nó do topo
    st->topo = st->topo->prox; // Atualiza o topo para o próximo nó
    free(aux); // Libera a memória do nó removido
    st->qtd--; // Decrementa a quantidade de elementos
    return pop;
}

// Retorna o valor do topo da pilha sem removê-lo
int peek(STACK *st){
    if(isStackNULL(st)){
        return -99999; // Valor de erro
    }
    return st->topo->num;
}

// Libera toda a memória alocada para a pilha
void freeStack(STACK* st){
    if(isStackNULL(st)) return;
    
    NODE* aux;
    while(!isEmpty(st)){ // Enquanto a pilha não estiver vazia
        aux = st->topo;  // Salva o nó do topo
        st->topo = st->topo->prox; // Atualiza o topo
        free(aux); // Libera o nó removido
    }
    printf("Stack was destroyed\n");
    free(st); // Libera a estrutura da pilha
}

// Exibe os elementos da pilha do topo até a base
void viewStack(STACK *st){
    if(isStackNULL(st)) return;
    
    NODE* aux = st->topo;
    while(aux != NULL){
        printf("%d\n", aux->num);
        aux = aux->prox;
    }
}

// Função principal para testar a pilha
int main(){
    STACK *st = createStack();
    
    // Adicionando elementos à pilha
    for(int i = 0; i < 11; i++){
        push(st, i);
    }

    // Exibindo a pilha
    viewStack(st);
    printf("Top: %d\n", peek(st));

    // Removendo elementos da pilha
    pop(st);
    pop(st);

    // Exibindo a pilha após remoções
    viewStack(st);

    // Liberando a memória da pilha
    freeStack(st);

    return 0;
}
