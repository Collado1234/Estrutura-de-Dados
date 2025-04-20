#include<stdio.h>
#include<stdlib.h>
#include<locale.h>
#include<limits.h>
#include<string.h>

typedef struct No{
    struct Pessoa *elemento;
    struct No *prox;
}No;

typedef struct Pessoa{
    char *nome_completo;
    char *cpf;
    int idade;
}Pessoa;

typedef struct TabelaHash{
    No **tabela;
    int tamanho;
}TabelaHash;


// Inicializa um nó da lista com uma Pessoa
No *inicializarNo(Pessoa *pessoa) {
    No *novo_no = (No *)malloc(sizeof(No));
    if (novo_no == NULL) {
        printf("Erro na alocação do nó\n");
        return NULL;
    }
    novo_no->elemento = pessoa;
    novo_no->prox = NULL;
    return novo_no;
}

TabelaHash *inicializarTabela(int tamanho){
    TabelaHash *tabela = (TabelaHash*) malloc(sizeof(TabelaHash));
    if(tabela == NULL){
        printf("Erro alocação da tabela\n");
        return NULL;
    }
    tabela->tamanho = tamanho;
    tabela->tabela = (No**) malloc(tamanho * sizeof(No*));
    if(tabela->tabela == NULL){
        printf("Erro alocação da tabela\n");
        free(tabela);
        return NULL;
    }
    for(int i = 0; i < tamanho; i++){
        tabela->tabela[i] = NULL;
    }
    return tabela;
}

int hash(TabelaHash* hash, char* chave){
    int soma = 0;
    for(int i = 0; chave[i] != '\0'; i++){
        if (chave[i] >= '0' && chave[i] <= '9') {  // só digitos
            soma += chave[i] * (i + 1);
        }
    }
    return soma % hash->tamanho;
}


void inserirTabela(TabelaHash *hash_table, Pessoa *pessoa) {
    if (hash_table == NULL || pessoa == NULL) {
        printf("Erro: tabela ou pessoa nula\n");
        return;
    }

    int pos = hash(hash_table, pessoa->cpf);
    No *novo_no = inicializarNo(pessoa);

    if(novo_no == NULL){
        printf("Erro alocacao No\n");
        return;
    }

    //inserindo No na posicao
    novo_no->prox = hash_table->tabela[pos];
    hash_table->tabela[pos] = novo_no;
    printf("Pessoa inserida\n");
}

void imprimirTabela(TabelaHash *hash_table){
    printf("\n--- Tabela Hash ---\n");
    for(int i = 0; i < hash_table->tamanho; i++){
        printf("[%d]: ",i);
        No *no_aux = hash_table->tabela[i];
        while(no_aux != NULL){
            printf("-> %s (%s, %d)", no_aux->elemento->nome_completo, no_aux->elemento->cpf, no_aux->elemento->idade);
            no_aux = no_aux->prox;
        }
        printf("->NULL\n");
    }
}

void liberarTabela(TabelaHash *hash_table) {
    for (int i = 0; i < hash_table->tamanho; i++) {
        No *atual = hash_table->tabela[i];
        while (atual != NULL) {
            No *temp = atual;
            atual = atual->prox;
            free(temp->elemento->nome_completo);
            free(temp->elemento->cpf);
            free(temp->elemento);
            free(temp);
        }
    }
    free(hash_table->tabela);
    free(hash_table);
}


// Função para criar uma pessoa
Pessoa *criarPessoa(const char *nome, const char *cpf, int idade) {
    Pessoa *nova = (Pessoa *)malloc(sizeof(Pessoa));
    nova->nome_completo = strdup(nome);
    nova->cpf = strdup(cpf);
    nova->idade = idade;
    return nova;
}

Pessoa *buscarPessoa(TabelaHash *hashTable, Pessoa *pessoa_chave){
    int pos  = hash(hashTable,pessoa_chave->cpf);
    
    if(hashTable->tabela[pos] == NULL){
        printf("Pessoa nao encontrada\n");
        return NULL;
    }

    No *no_atual = hashTable->tabela[pos];

    while(no_atual != NULL){
        if(no_atual->elemento->cpf == pessoa_chave->cpf){
            printf("Pessoa encontrada: %s (%s, %d)", no_atual->elemento->nome_completo, no_atual->elemento->cpf, no_atual->elemento->idade);
            return no_atual->elemento;
        }
        no_atual = no_atual->prox;
    }
    printf("Pessoa nao encontrada\n");
    return NULL;
}

int main(){

    TabelaHash *hashTable = inicializarTabela(10);

    Pessoa *p1 = criarPessoa("Fernando Alencar","512.078.488-75",28);
    Pessoa *p2 = criarPessoa("Maria Julia","412.156.884-12",18);
    Pessoa *p3 = criarPessoa("Rennan Collado","109.626.233-89",20);

    inserirTabela(hashTable, p1);
    inserirTabela(hashTable, p2);
    inserirTabela(hashTable, p3);    

    imprimirTabela(hashTable);

    buscarPessoa(hashTable, p3);

    return 0;
}