#include <stdio.h>
#include "graph_lib.h"
#include <string.h>
#include <stdlib.h>

enum {BUF_SIZE = 256};

static const char *func_name[] = {"add", "del", "cyc", "f-b", "wrt"};

int read_file(FILE *f) {
    graph *g = init();
    if (g == NULL) {
        printf("Unable to initialize the graph\n");
        return -1;
    }
    else {
        fscanf(f, "%ld", &(g->vertex_cnt));
        if (g->vertex_cnt < 0) {
            printf("Invalid vertex count\n");
            deinit(g);
            return -1;
        }
        char buf[BUF_SIZE];
        while (fgets(buf, BUF_SIZE, f) != NULL) {
            if (strncmp(buf, func_name[0], 3) == 0) {
                //функция add
                char *next_d;
                long u = strtol(buf + 3, &next_d, 10);
                long v = strtol(next_d, &next_d, 10);
                int cost = strtol(next_d, &next_d, 10);
                int err = add_edge(g, u, v, cost);
                if (err) {
                    printf("Unable do add edge %ld %ld %d\n", u, v, cost);
                }
                else {
                    printf("Edge %ld %ld %d successfully added\n", u, v, cost);
                }
            }
            if (strncmp(buf, func_name[1], 3) == 0) {
            	//функция del
            	char *next_d;
                long edge_ind;
                long u = strtol(buf + 3, &next_d, 10);
                long v = strtol(next_d, &next_d, 10);
                int cost = strtol(next_d, &next_d, 10);
                int err = delete_edge(g, u, v, cost);
                if (err) {
                	printf("There is no edge %ld %ld %d in the graph\n", 
                	u, v, cost);
                }
                else {
                    printf("Edge %ld %ld %d successfully deleted\n", \
                    u, v, cost);
                }
            }
            if (strncmp(buf, func_name[2], 3) == 0) {
                //проверка на ацикличность
                int check_cyc = check_cycles(g);
                if (check_cyc == -1) {
                    deinit(g);
                    return -1;
                }
                if (!check_cyc) {
                    printf("There is NO cycles in the graph\n");
                }
                else {
                    printf("There is cycles in the graph\n");
                }
            }
            
            if (strncmp(buf, func_name[3], 3) == 0) {
            	//алгоритм Форда-Беллмана
            	long vertex = strtol(buf + 3, NULL, 10);
            	long *dist = (long *)malloc(sizeof(long) * g->vertex_cnt);
            	if (dist == NULL) {
                    printf("Unable to allocate distance array\n");
                    deinit(g);
                    return -1;
		        }
		        int is_neg_cycle;
            	int err = Ford_Bellman(g, vertex, dist, &is_neg_cycle);
            	if (err == 0) {
                	if (is_neg_cycle) {
                	    printf("There is a negative cycle in the graph\n");
                	}
                	else {
                	    printf("Distances from vertex %ld to all others\n", vertex);
                	    for (int i = 0; i < g->vertex_cnt; i++) {
                	        if (dist[i] != INF) {
                	            printf("%ld ", dist[i]);
                	        }
                	        else {
                	            printf("INF ");
                	        }
                	    }
                	    printf("\n");
                	}
            	}
            	free(dist);
            }
            if (strncmp(buf, func_name[4], 3) == 0) {
                //распечатка ребер графа
                printf("Current list of edges:\n");
                list *curr = g->l;
                for (int i = 0; i < g->vertex_cnt; i++) {
                    printf("%ld %ld %d\n", (curr->e).u, (curr->e).v, (curr->e).cost);
                    curr = curr->next;
                }
            }
        }
        deinit(g);
    }
    return 0;
}



int
main(int argc, char *argv[]) {
    FILE *graph_file = fopen(argv[1], "r");
    if (read_file(graph_file) != 0) {
    	printf("Programm finished due to errors\n");
    }    
    fclose(graph_file);
}
