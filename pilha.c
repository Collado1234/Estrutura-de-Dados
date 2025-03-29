#include<stdio.h>
#include<stdlib.h>
#include<stdbool.h>

typedef struct NODE{
    int num;
    struct NODE *prox;
}NODE;

typedef struct STACK{
    NODE* topo;
    int qtd;
}STACK;

NODE* createNode(int num){
    NODE* n = (NODE*) malloc(sizeof(NODE));
    if(n==NULL){
        printf("Erro ao alocar o no\n");
        return NULL;
    }
    n->num = num;
    n->prox = NULL;
    return n;
}

STACK* createStack(){
    STACK* st = (STACK*) malloc(sizeof(STACK));
    if(st==NULL){
        printf("Erro ao alocar pilhazz\n");
        return NULL;
    }
    st->qtd = 0;
    st->topo = NULL;
    printf("Inicializando pilha\n");
    return st;
}

bool isEmpty(STACK *st){
    if(st->topo == NULL){
        return true;
    }else{
        return false;
    }
}

bool isStackNULL(STACK *st){
    if(st == NULL){
        printf("Invalid Stack Pointer\n");
        return true;
    }else{
        return false;
    }
}


void push(STACK *st, int num){
    if(st==NULL){
        printf("Invalid stack pointer\n");
    }
    NODE *new_node = createNode(num);
    if(isEmpty(st)){
        st->topo = new_node;
    }else{
        new_node->prox = st->topo;
        st->topo = new_node;
    }
    st->qtd++;
    return;
}

int pop(STACK *st){
    if(isStackNULL(st)){
        printf("Invalid stack pointer\n");
    }
    if(isEmpty(st)){
        printf("Stack is empty\n");
        return -9999999;
    }
    int pop = st->topo->num;
    NODE *aux = st->topo;  //salvando ponteiro para o No topo
    st->topo = st->topo->prox;
    free(aux);
    st->qtd--;
    return pop;
}

int peek(STACK *st){
    if(isStackNULL(st)){
        return -99999;
    }
    return st->topo->num;
}

void freeStack(STACK* st){
    if(isEmpty(st)) return;
    NODE* aux;
    while(!isEmpty(st)){
        aux = st->topo;
        st->topo = st->topo->prox;
        free(aux);
    }
    printf("Stack was destroyed\n");
    free(st);
}

void viewStack(STACK *st){
    if(isStackNULL(st)) return;
    NODE* aux = st->topo;
    while(aux != NULL){
        printf("%d\n",aux->num);
        aux = aux->prox;
    }
}

int main(){
    STACK *st = createStack();
    for(int i = 0; i < 11; i++){
        push(st,i);
    }
    viewStack(st);
    printf("Top: %d\n", peek(st));
    pop(st);
    pop(st);
    viewStack(st);
    freeStack(st);
}