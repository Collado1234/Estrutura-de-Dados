#include<stdio.h>
#include<stdlib.h>

typedef struct no{
    struct no* dir;
    struct no* esq;
    int valor;
}no;

typedef struct arvore_bin{ //desnecessario, pode usar a arvore como so raiz
    no* raiz;
}arvore_bin;

no* criar_no(int valor){
    no *n = (no*) malloc (sizeof(no));
    if(n == NULL) printf ("Erro alocacão no\n"); return NULL;

    n->dir = NULL;
    n->esq = NULL;
    n->valor = valor;

    return n;
}

arvore_bin* criar_arvore(){
    arvore_bin *arv = (arvore_bin*) malloc (sizeof(arvore_bin));
    if(arv == NULL) printf ("Erro alocacão no\n"); return NULL;

    arv->raiz = NULL;

    return arv;
}

void inserir_novo_no(arvore_bin *arv, int valor){
    if(arv == NULL) printf ("Erro, arvore nula\n"); return;

    no* aux = arv->raiz;
    no* pai;

    if(aux == NULL){  //Se for inserir na raiz
        arv->raiz = criar_no(valor);
        return;
    }

    while(aux != NULL){
        pai = aux;
        if(valor < aux->valor){
            aux = aux->esq;
        }else if(aux->valor < valor){
            aux = aux->dir;
        }else{
            printf("Valor ja existe na arvore\n");
            return;
        }
    }


}
