#include <limits.h>

typedef struct edge_graph{
    long u;
    long v;
    int cost;
} edge;

typedef struct list_edge {
    edge e;
    struct list_edge *next;
    struct list_edge *pr;
} list;

typedef struct graph_struct {
    list *l;
    long vertex_cnt;
    long edge_cnt;
} graph;

enum {INF = LLONG_MAX};

graph* init(void);

int add_edge(graph *g, long u, long v, int cost);

int delete_edge(graph *g, long u, long v, int cost);

void deinit (graph *g);

int Ford_Bellman(graph *g, long vertex, long dist[], int *is_neg_cycle);

int check_cycles(graph *g);
