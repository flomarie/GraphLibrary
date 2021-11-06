#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include "graph_lib.h"

graph* init(void) {
    graph *g =(graph *)malloc(sizeof(graph));
    if (g == NULL) {
        return NULL;
    }
    g->l = NULL;
    g->vertex_cnt = 0;
    g->edge_cnt = 0;
    return g;
    /*В случае неудачи NULL*/
}

int add_edge(graph *g, long u, long v, int cost) {
    //в случае неудачи 1, в случае успеха 0
    if (u < 0 || u >= (g->vertex_cnt) || v < 0 || v >= (g->vertex_cnt)) {
    	printf("Vertex index is out of a range\n");
    	return 1;
    }
    list *l = (list *)malloc(sizeof(list));
    if (l == NULL) {
    	printf("Unable to allocate memory\n");
        return 1;
    }    
    (l->e).u = u;
    (l->e).v = v;
    (l->e).cost = cost;
    if (g->edge_cnt == 0) {
        g->l = l;
        l->pr = l;
        l->next = l;
    }
    else {
        l->pr = (g->l)->pr;
        ((g->l)->pr)->next = l;
        (g->l)->pr = l;
        l->next = (g->l);
    }
    (g->edge_cnt)++;
    return 0;
}

int edge_cmp(edge e1, edge e2) {
    return (e1.u == e2.u) && (e1.v == e2.v) && (e1.cost == e2.cost);
}

list * is_in_graph(graph *g, edge e) {
//NULL если нет ребра, иначе list с этим ребром
    list *curr = g->l;
    for (long long i = 0; i < g->edge_cnt; i++) {
        if (edge_cmp(curr->e, e)) {
            return curr;
        }
        curr = curr->next;
    }
    return NULL;
}

int delete_edge(graph *g, long u, long v, int cost) {
//если ребра нет в графе, то возвращается 1, иначе 0
    edge e = {u, v, cost};
    list *l = is_in_graph(g, e);
    if (l != NULL) {
        if ((g->l) == l) {
            if (g->edge_cnt == 1) {
                g->l = NULL;
            }
            else {
                ((g->l)->next)->pr = (g->l)->pr;
                ((g->l)->pr)->next = (g->l)->next;
                (g->l) = (g->l)->next;
            }
        }
        else {
            (l->pr)->next = l->next;
            (l->next)->pr = l->pr;
        }
        g->edge_cnt--;
        free(l);
        return 0;
    }
    else {
        return 1;
    }
}

void deinit (graph *g) {
    list *curr_l = g->l, *next_l;
    for (long i = 0; i < g->edge_cnt; i++) {
        next_l = curr_l->next;
        free(curr_l);
        curr_l = next_l;
    }
    free(g);
}

int Ford_Bellman(graph *g, long vertex, long *dist, int *is_neg_cycle) {
    //размер dist должен быть равен количеству вершин в графе
    //все длины путей должны помещаться в long
    long long inf = INF;
    for (long i = 0; i < g->vertex_cnt; i++) {
        dist[i] = inf;
    }
    if (vertex < 0 || vertex >= g->vertex_cnt) {
    	printf("Vertex index is out of a range, unable to calculate distances\n");
    	return 1;
    }
    dist[vertex] = 0;
    int is_changed = 0;
    for (long i = 0; i < g->vertex_cnt; i++) {
        list *curr = g->l;
        is_changed = 0;
        for (long j = 0; j < g->edge_cnt; j++) {
            if (dist[(curr->e).u] < inf) {
                if ((dist[(curr->e).u] + (curr->e).cost) < dist[(curr->e).v]) {
                    dist[(curr->e).v] = dist[(curr->e).u] + (curr->e).cost;
                    is_changed = 1;
                }
            }
            curr = curr->next;
        }
    }
    *is_neg_cycle = is_changed;
    return 0;
}


static char is_cycle = 0;

void dfs(graph *g, long vertex, char col[]) {
    col[vertex] = 1;
    list *curr = g->l;
    for (long i = 0; i < g->edge_cnt; i++) {
        if ((curr->e).u == vertex) {
            if (col[(curr->e).v] == 1) {
                is_cycle = 1;
            }
            else if (col[(curr->e).v] == 0) {
                dfs(g, (curr->e).v, col);
            }
        }
        curr = curr->next;
    }
    col[vertex] = 2;
}

int check_cycles(graph *g) {
//возвращает -1 в случае ошибки, 0 если нет циклов, 1 если есть
    if(g->edge_cnt == 0) {
        return 0;
    }
    char *col = (char *)malloc(g->vertex_cnt);
    if (col == NULL) {
        printf("Unable to allocate memory for temporary array\n");
        return -1;
    }
    
    for (long i = 0; i < g->vertex_cnt; i++) {
        col[i] = 0;
    }
    is_cycle = 0;
    dfs(g, 0, col);
    free(col);
    return is_cycle;
}

