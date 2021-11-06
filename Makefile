a.out: graph_lib.o main.o
	gcc graph_lib.o main.o

graph_lib: graph_lib.c graph_lib.h
	gcc graph_lib.c -c

main: main.c graph_lib.h
	gcc main.c -c

clean:
	rm -f graph_lib.o main.o
	
