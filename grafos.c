#include<stdio.h>
#include<stdlib.h>

#include "adjacency_list.h"
#include "queue.h"

Graph list_create_graph(int n) {
    int i;
    Graph g = malloc(sizeof(Graph));
    g->n = n;
    g->adj = malloc(n * sizeof(Graph_node));
    for (i = 0; i < n; i++)
        g->adj[i] = NULL;
    return g;
}
void free_list(Graph_node list) {
    if (list != NULL) {
        free_list(list->next);
        free(list);
    }
}
void list_destroy_graph(Graph g) {
    int i;
    for (i = 0; i < g->n; i++)
        free_list(g->adj[i]);
    free(g->adj);
    free(g);
}

Graph_node insert_to_list(Graph_node list, int v) {
    Graph_node new_node = malloc(sizeof(Node));
    new_node->v = v;
    new_node->next = list;
    return new_node;
}

void list_insert_edge(Graph g, int u, int v) {
    g->adj[v] = insert_to_list(g->adj[v], u);
    g->adj[u] = insert_to_list(g->adj[u], v);
}

Graph_node remove_from_list(Graph_node list, int v) {
    if (list == NULL) return NULL;

    if (list->v == v) {
        Graph_node next = list->next;
        free(list);
        return next;
    }

    list->next = remove_from_list(list->next, v);
    return list;
}

void list_remove_edge(Graph g, int u, int v) {
    if (u >= g->n || v >= g->n) return; // Verifica se os vértices são válidos
    g->adj[u] = remove_from_list(g->adj[u], v);
    g->adj[v] = remove_from_list(g->adj[v], u);
}

int list_have_edge(Graph g, int u, int v) {
    for (Graph_node t = g->adj[u]; t != NULL; t = t->next) {
        if (t->v == v)
            return 1;
    }
    return 0;
}

void list_print_edges(Graph g) {
    int u;
    Graph_node t;
    for (u=0; u < g->n; u++)
        for (t = g->adj[u]; t != NULL; t = t->next)
            printf("{%d,%d}\n", u, t->v);
}

void list_rec_search(Graph g, int *components , int comp, int v) {
    Graph_node t;
    components[v] = comp;           

    for (t = g->adj[v]; t != NULL; t = t->next)
        if (components[t->v] == -1)
            list_rec_search(g, components , comp, t->v);
}   

int *list_find_components(Graph g) {
    int s, c = 0, *components = malloc(g->n * sizeof(int));

    for (s = 0; s < g->n; s++)
        components[s] = -1;

    for (s = 0; s < g->n; s++)
        if (components[s] == -1) {
            list_rec_search(g, components , c, s);
            c++;
    }

    return components;
}

void list_in_depth_search(Graph g, int *parent, int p, int v) {
    Graph_node t;
    parent[v] = p;

    for(t = g->adj[v]; t != NULL; t = t->next)
        if (parent[t->v] == -1)
            list_in_depth_search(g, parent, v, t->v);
}

int *list_find_paths(Graph g, int s) {
    int i, *parent = malloc(g->n * sizeof(int));

    for (i = 0; i < g->n; i++)
        parent[i] = -1;

    list_in_depth_search(g, parent, s, s);
    return parent;
}

void list_print_reverse_path(int v, int *parent) {
    printf("%d", v);
    if(parent[v] != v)
    print_reverse_path(parent[v], parent);
}

void list_print_path(int v, int *parent) {
    if(parent[v] != v)
        list_print_path(parent[v], parent);
    printf("%d", v);
}

int *list_width_search(Graph g, int s) { //ARRUMAR
    if (s < 0 || s >= g->n) {
        fprintf(stderr, "Error: Invalid starting vertex.\n");
        return NULL;
    }
    
    int w, v;
    int *parent = malloc(g->n * sizeof(int));
    int *visited = malloc(g->n * sizeof(int));
    Queue *f = create_queue(g->n);

    for (v = 0; v < g->n; v++) {
        parent[v] = -1;
        visited[v] = 0;
    }

    enqueue(f,s);
    parent[s] = s;
    visited[s] = 1;

    while(!empty_queue(f)) {
        v = dequeue(f);
        for (Graph_node aux = g->adj[v]; aux != NULL; aux = aux->next){     //possivel erro
            w = aux->v;
            if (!visited[w]) {
                visited[w] = 1;
                parent[w] = v;
                enqueue(f, w);
            }
        }
    }

    destroy_queue(f);
    free(visited);
    return parent;
}

int *list_width_search(Graph g, int s) {
    // Verificar se o vértice inicial é válido
    

    int w, v;
    int *parent = malloc(g->n * sizeof(int)); // Vetor para armazenar os pais
    int *visited = malloc(g->n * sizeof(int)); // Vetor de vértices visitados
    Queue *f = create_queue(g->n); // Cria a fila com capacidade igual ao número de vértices

    // Inicializa os vetores de pais e visitados
    for (v = 0; v < g->n; v++) {
        parent[v] = -1;
        visited[v] = 0;
    }

    // Adiciona o vértice inicial na fila
    enqueue(f, s);
    parent[s] = s; // O vértice inicial é seu próprio pai
    visited[s] = 1; // Marca como visitado

    // Realiza a busca em largura
    while (!empty_queue(f)) {
        v = dequeue(f); // Desenfileira um vértice

        // Percorre a lista de adjacência do vértice v
        for (Graph_node aux = g->adj[v]; aux != NULL; aux = aux->next) {
            w = aux->v; // Obtém o índice do vértice adjacente
            if (!visited[w]) { // Verifica se o vértice já foi visitado
                visited[w] = 1; // Marca como visitado
                parent[w] = v; // Define o pai
                enqueue(f, w); // Enfileira o vértice adjacente
            }
        }
    }

    // Libera memória alocada para a fila e o vetor visited
    destroy_queue(f);
    free(visited);

    return parent; // Retorna o vetor de pais
}


#ifndef ADJACENCY_LIST_H
#define ADJACENCY_LIST_H

typedef struct node {
    int v;
    struct node *next;
} Node;

typedef Node *Graph_node;

typedef struct {
    Graph_node *adj;
    int n;
} Adj_list;

typedef Adj_list *Graph;

Graph list_create_graph(int n);
void list_destroy_graph(Graph g);
void list_insert_edge(Graph g, int u, int v);
void list_remove_edge(Graph g, int u, int v);
int list_have_edge(Graph g, int u, int v);
void list_print_edges(Graph g);
void list_rec_search(Graph g, int *components, int comp, int v);
void *list_find_components(Graph g, int *components);
void list_in_depth_search(Graph g, int *parent, int p, int v);
int list_print_reverse_path(int v, int *parent);
void list_print_path(int v, int *parent);
int *list_width_search(Graph g, int s);

#endif

#ifndef QUEUE_H
#define QUEUE_H

typedef struct Node {
    int data;
    struct Node *next;
} Node;

typedef struct Queue {
    Node *front;
    Node *rear;
    int size;
    int capacity; // Campo para armazenar a capacidade máxima
} Queue;

Queue *create_queue(int capacity);
void destroy_queue(Queue *queue);
void enqueue(Queue *queue, int data);
int dequeue(Queue *queue);
int peek(Queue *queue);
int is_empty(Queue *queue);
int get_size(Queue *queue);

#endif

#include <stdio.h>
#include <stdlib.h>
#include "queue.h"

// Cria uma fila com capacidade definida
Queue *create_queue(int capacity) {
    Queue *queue = (Queue *)malloc(sizeof(Queue));
    if (!queue) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    queue->front = queue->rear = NULL;
    queue->size = 0;
    queue->capacity = capacity; // Define a capacidade
    return queue;
}

// Libera toda a memória associada à fila
void destroy_queue(Queue *queue) {
    while (!is_empty(queue)) {
        dequeue(queue);
    }
    free(queue);
}

// Insere um elemento na fila
void enqueue(Queue *queue, int data) {
    if (queue->size == queue->capacity) {
        fprintf(stderr, "Queue is full. Cannot enqueue.\n");
        return;
    }

    Node *new_node = (Node *)malloc(sizeof(Node));
    if (!new_node) {
        fprintf(stderr, "Memory allocation error\n");
        exit(EXIT_FAILURE);
    }
    new_node->data = data;
    new_node->next = NULL;

    if (queue->rear == NULL) {
        queue->front = queue->rear = new_node;
    } else {
        queue->rear->next = new_node;
        queue->rear = new_node;
    }
    queue->size++;
}

// Remove e retorna o elemento da frente da fila
int dequeue(Queue *queue) {
    if (is_empty(queue)) {
        fprintf(stderr, "Queue is empty. Cannot dequeue.\n");
        return -1; // Retorna -1 para indicar falha
    }

    Node *temp = queue->front;
    int data = temp->data;

    queue->front = queue->front->next;
    if (queue->front == NULL) {
        queue->rear = NULL;
    }
    free(temp);
    queue->size--;
    return data;
}

// Retorna o elemento da frente da fila sem removê-lo
int peek(Queue *queue) {
    if (is_empty(queue)) {
        fprintf(stderr, "Queue is empty. Cannot peek.\n");
        return -1; // Retorna -1 para indicar fila vazia
    }
    return queue->front->data;
}

// Verifica se a fila está vazia
int is_empty(Queue *queue) {
    return queue->size == 0;
}

// Retorna o tamanho atual da fila
int get_size(Queue *queue) {
    return queue->size;
}




#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

#include "adjacency_list.h"
#include "queue.h"
#include "priority_queue.h"

Graph list_create_graph(int n) {
    int i;
    Graph g = malloc(sizeof(Graph));
    g->n = n;
    g->adj = malloc(n * sizeof(Graph_node));
    for (i = 0; i < n; i++)
        g->adj[i] = NULL;
    return g;
}
void free_list(Graph_node list) {
    if (list != NULL) {
        free_list(list->next);
        free(list);
    }
}
void list_destroy_graph(Graph g) {
    int i;
    for (i = 0; i < g->n; i++)
        free_list(g->adj[i]);
    free(g->adj);
    free(g);
}

Graph_node insert_to_list(Graph_node list, int v, int weight) {
    Graph_node new_node = malloc(sizeof(Node));
    new_node->v = v;
    new_node->next = list;
    new_node->weight = weight;
    return new_node;
}


void list_insert_edge(Graph g, int u, int v, int weight) {
    g->adj[v] = insert_to_list(g->adj[v], u, weight);
    g->adj[u] = insert_to_list(g->adj[u], v, weight);
}

Graph_node remove_from_list(Graph_node list, int v) {
    if (list == NULL) return NULL;

    if (list->v == v) {
        Graph_node next = list->next;
        free(list);
        return next;
    }

    list->next = remove_from_list(list->next, v);
    return list;
}

void list_remove_edge(Graph g, int u, int v) {
    if (u >= g->n || v >= g->n) return; // Verifica se os vértices são válidos
    g->adj[u] = remove_from_list(g->adj[u], v);
    g->adj[v] = remove_from_list(g->adj[v], u);
}

int list_have_edge(Graph g, int u, int v) {
    for (Graph_node t = g->adj[u]; t != NULL; t = t->next) {
        if (t->v == v)
            return 1;
    }
    return 0;
}

void list_print_edges(Graph g) {
    int u;
    Graph_node t;
    for (u=0; u < g->n; u++)
        for (t = g->adj[u]; t != NULL; t = t->next)
            printf("{%d,%d}\n", u, t->v);
}

void list_rec_search(Graph g, int *components , int comp, int v) {
    Graph_node t;
    components[v] = comp;           

    for (t = g->adj[v]; t != NULL; t = t->next)
        if (components[t->v] == -1)
            list_rec_search(g, components , comp, t->v);
}   

int *list_find_components(Graph g) {
    int s, c = 0, *components = malloc(g->n * sizeof(int));

    for (s = 0; s < g->n; s++)
        components[s] = -1;

    for (s = 0; s < g->n; s++)
        if (components[s] == -1) {
            list_rec_search(g, components , c, s);
            c++;
    }

    return components;
}

void list_in_depth_search(Graph g, int *parent, int p, int v) {
    Graph_node t;
    parent[v] = p;

    for(t = g->adj[v]; t != NULL; t = t->next)
        if (parent[t->v] == -1)
            list_in_depth_search(g, parent, v, t->v);
}

int *list_find_paths(Graph g, int s) {
    int i, *parent = malloc(g->n * sizeof(int));

    for (i = 0; i < g->n; i++)
        parent[i] = -1;

    list_in_depth_search(g, parent, s, s);
    return parent;
}

void list_print_reverse_path(int v, int *parent) {
    printf("%d", v);
    if(parent[v] != v)
    print_reverse_path(parent[v], parent);
}

void list_print_path(int v, int *parent) {
    if(parent[v] != v)
        list_print_path(parent[v], parent);
    printf("%d", v);
}

int *list_width_search(Graph g, int s) { //ARRUMAR
    if (s < 0 || s >= g->n) {
        fprintf(stderr, "Error: Invalid starting vertex.\n");
        return NULL;
    }
    
    int w, v;
    int *parent = malloc(g->n * sizeof(int));
    int *visited = malloc(g->n * sizeof(int));
    Queue *f = create_queue(g->n);

    for (v = 0; v < g->n; v++) {
        parent[v] = -1;
        visited[v] = 0;
    }

    enqueue(f,s);
    parent[s] = s;
    visited[s] = 1;

    while(!empty_queue(f)) {
        v = dequeue(f);
        for (Graph_node aux = g->adj[v]; aux != NULL; aux = aux->next){     //possivel erro
            w = aux->v;
            if (!visited[w]) {
                visited[w] = 1;
                parent[w] = v;
                enqueue(f, w);
            }
        }
    }

    destroy_queue(f);
    free(visited);
    return parent;
}

int *dijkstra(Graph g, int s) {
    int v, *parent = malloc(g->n * sizeof(int));
    Graph_node t;
    p_queue h = create_p_queue(g->n);
    for (v = 0; v < g->n; v++) {
        parent[v] = -1;
        insert(h, v, INT_MAX);
    }
    parent[s] = s;
    decrease_priority(h, s, 0);
    while (!is_empty(h)) {
        v = extract_min(h);
        if (priority(h, v) != INT_MAX)
            for (t = g->adj[v]; t != NULL; t = t->next)
                if (priority(h, v) + t->weight < priority(h, t->v)) {
                    decrease_priority(h, t->v, priority(h, v) + t->weight);
                    parent[t->v] = v;
                }
    }
    return parent;
}


int degree_ListAdj(Graph g, int u){
    Graph_node aux;
    int degree = 0;
    for(aux = g->adj[u]; aux != NULL; aux = aux->next){
        degree++;
    }
    return degree;
}

int most_popular_List(Graph g){
    int u, max, max_degree, current_degree;

    max = 0;
    max_degree = degree_ListAdj(g, max);
    for( u = 1; u < g->n; u++) {
        current_degree = degree_ListAdj(g, u);
        if(current_degree > max_degree){
            max = u;
            max_degree = current_degree;
        }
    }
    return max;
}

/*
 * Função que imprime vizinhos dos vizinhos do vértice
 * de forma não repetida
*/
void print_recommendations(Graph g, int u){
    Graph_node aux, rec_aux;
    int *recommended = malloc(g->n * sizeof(int));  
    int i;
    for (i = 0; i < g->n; i++) {
        recommended[i] = 0;
    }

    for (aux = g->adj[u]; aux != NULL; aux = aux->next) {
        int v = aux->v;

        for (rec_aux = g->adj[v]; rec_aux != NULL; rec_aux = rec_aux->next) {
            int w = rec_aux->v;

            if (w != u) {
                if (!recommended[w]) {  //verifica se já não foi recomendado
                    printf("Recommendation: %d\n", w);  
                    recommended[w] = 1;  
                }
            }
        }
    }

    free(recommended);  // Libera a memória alocada para o vetor de recomendados
}

int rec_search_list(Graph g,int* visited, int v, int t){
    int w;
    Graph_node aux;

    if(v == t){
        return 1;
    }
    visited[v] = 1;
    for(aux = g->adj[v]; aux != NULL ; aux = aux->next){
        w = aux->v;
        if(!visited[w]){
            if(rec_search_list(g, visited, w, t)){
                return 1;
            }
        }

    }
    return 0;
}

int has_path_List(Graph g, int start, int end){
    int founded_flag, i;
    int *visited = malloc(g->n * sizeof(int));

    for (i = 0 ; i < g->n ; i++){
        visited[i] = 0;
    }

    founded_flag = rec_search_list(g,visited,start,end);
    free(visited);

    return founded_flag;
}



#include<stdio.h>
#include<stdlib.h>

#include "adjacency_matrix.h"
#include "queue.h"

graph matrix_create_graph(int n) {
    int i, j;

    graph g = malloc(sizeof(Adj_matrix));
    g->n = n;
    g->adj = malloc(n*sizeof(int *));
    for(i=0; i<n; i++) {
        g->adj[i] = malloc(n * sizeof(int));
    }
    for (i = 0; i < n; i++)
        for (j = 0; j < n; j++)
            g->adj[i][j] = 0;
    return g;
}

void matrix_destroy_matrix_graph(graph g) {
    int i;
    for (i = 0; i < g->n; i++)
        free(g->adj[i]);
    free(g->adj);
    free(g);
}

void matrix_insert_edge(graph g, int u, int v, int weight) {
    g->adj[u][v] = weight;
    g->adj[v][u] = weight;
}
void matrix_remove_edge(graph g, int u, int v) {
    g->adj[u][v] = 0;
    g->adj[v][u] = 0;
}
int matrix_have_edge(graph g, int u, int v) {
    return g->adj[u][v];
}

void matrix_print_edges(graph g) {
    int u, v;
    for (u = 0; u < g->n; u++)
        for (v = u+1; v < g->n; v++)
            if (g->adj[u][v])
                printf("{%d,%d}\n", u, v);
}

graph matrix_read_graph() {
    int n, m, i, u, v, w;
    graph g;
    scanf("%d %d", &n, &m);
    g = matrix_create_graph(n);
    for (i = 0; i < m; i++) {
        scanf("%d %d %d", &u, &v, &w);
        matrix_insert_edge(g, u, v, w);
    }
    return g;
}

int matrix_degree(graph g, int u) {
    int v, degree = 0;
    for (v = 0; v < g->n; v++)
        if (g->adj[u][v])
            degree++;
    return degree;
}

int matrix_most_popular(graph g) {
    int u, max, max_degree , current_degree;
    max = 0;
    max_degree = matrix_degree(g, 0);
    for (u = 1; u < g->n; u++) {
        current_degree = matrix_degree(g, u);
        if (current_degree > max_degree) {
            max_degree = current_degree;
            max = u;
        }
    }
    return max;
}

void matrix_print_recommendations(graph g, int u) {
    int v, w;
    for (v=0; v < g->n; v++) {
        if (g->adj[u][v]) {
            for (w = 0; w < g->n; w++) {
                if (g->adj[v][w] && w != u && !g->adj[u][w])
                    printf("%d\n", w);
            }
        }
    }
}

int matrix_path_exists(graph g, int s, int t) {
    int found , i, *visited = malloc(g->n * sizeof(int));
    for (i=0; i < g->n; i++)
        visited[i] = 0;
    found = matrix_rec_search(g, visited , s, t);
    free(visited);
    return found;
}

int matrix_rec_search(graph g, int *visited , int v, int t) {
    int w;
    if (v == t)
        return 1;
    visited[v] = 1;
    for (w=0; w < g->n; w++)
        if (g->adj[v][w] && !visited[w])
            if (matrix_rec_search(g, visited , w, t))
                return 1;
    return 0;
}

int *matrix_width_search(graph g, int s){ // trocar para matrix_BFS
    int *parent = malloc(g->n * sizeof(int));
    int *visited = malloc(g->n * sizeof(int));
    int w,v;
    Queue *f = create_queue(g->n);

    for(v = 0; v < g->n ; v++){
        parent[v] = -1;
        visited[v] = 0;
    }

    enqueue(f,s);
    parent[s] = s;
    visited[s] = 1;

    while(!empty_queue(f)){
        v = dequeue(f);

        for(w = 0; w < g->n; w++){
            if(g->adj[v][w] != -1 && !visited[w]){
                visited[w] = 1; 
                parent[w] = v; 
                enqueue(f, w); 
            }
        }
    }
    destroy_queue(f);
    free(visited);

    return parent; 
}

void matrix_in_depth_search(graph g, int* parent, int p, int v){
    parent[v] = p;
    int w;
    for(w = 0; w < g->n; w++){
        if(g->adj[v][w] != 0 && parent[w] == -1){
            matrix_in_depth_search(g, parent, v, w);
        }
    }
}

int *matrix_find_paths(graph g, int s){
    int* parent = malloc (g->n * sizeof(int));
    int i;

    for(i = 0; i < g->n; i++){
        parent[i] = -1;
    }
    matrix_in_depth_search(g,parent,s,s);
    return parent;
}

void matrix_recursive_visit(graph g, int *components, int count_comp, int v){
    components[v] = count_comp;

    for(int w = 0; w < g->n; w++){
        if(g->adj[v][w] == 1 && components[w] == -1){
            matrix_recursive_visit(g,components,count_comp,w);
        }
    }
}

int *matrix_find_components(graph g){
    int s, count_components = 0;
    int * components = malloc (g->n * sizeof(int));
    for(s = 0; s < g->n; s++){
        components[s] = -1;
    }

    for(s = 0; s < g->n; s++){
        if(components[s] == -1){
            matrix_recursive_visit(g,components,count_components,s);
            count_components++;
        }
    }
    return components;
}


#ifndef ADJACENCY_MATRIX_H
#define ADJACENCY_MATRIX_H

typedef struct {
    int **adj;
    int n;
}Adj_matrix;

typedef Adj_matrix *graph;

graph matrix_create_graph(int n);
void matrix_destroy_matrix_graph(graph g);
void matrix_insert_edge(graph g, int u, int v, int weight);
void matrix_remove_edge(graph g, int u, int v);
int matrix_have_edge(graph g, int u, int v);
void matrix_print_edges(graph g);
graph matrix_read_graph();
int matrix_degree(graph g, int u);
int matrix_most_popular(graph g);
void matrix_print_recommendations(graph g, int u);
int matrix_path_exists(graph g, int s, int t);
int matrix_rec_search(graph g, int *visited , int v, int t);
int *matrix_width_search(graph g, int s);       //largura
int *matrix_find_paths(graph g, int s);         //profundidade
int *matrix_find_components(graph g);

#endif
