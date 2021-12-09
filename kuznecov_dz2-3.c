#include <stdio.h>
#include <stdlib.h>

#define MAX(x, y) ((x>=y) ? (x) : (y))
#define SWAP(x, y) {int t; t = x; x = y; y = t; }

int is_tree(int **array, int N);
int edge_sum(int* array, int N);
int edge_multi(int* array, int N);

int main(void) {
	FILE* file_graph;
	int top = 0, first_top = 0, second_top = 0, check = 0, max_top = 1, error = 0;
	char get_symbol;
	int** arr = (int**)calloc(max_top, sizeof(int*));
	arr[0] = (int*)calloc(max_top, sizeof(int));

	file_graph = fopen("graph.dot", "w");
	fprintf(file_graph, "graph g{\n");

	printf("Enter the edges connected to the graph. Example:\n1--2\n2--3\nEnter \"e\" for exit. Your graph:\n");

	while ((get_symbol = getchar()) != 'e') {

		fputc(get_symbol, file_graph);

		if ((get_symbol <= '9') && (get_symbol >= '0'))
			top = top * 10 + (int)(get_symbol - '0');

		if (get_symbol == '-') 
			check++;

		if (check == 2) {
			first_top = top;
			top = 0;
			check = 0;
		}

		if (get_symbol == '\n') {
			second_top = top;
			top = 0;
			if (first_top > second_top) 
				SWAP(first_top, second_top);
		}

		if ((first_top == 0) && (second_top != 0))
			second_top = 0;

		if ((first_top != 0) && (second_top != 0)) {

			if (max_top < MAX(first_top, second_top)) {
				arr = (int**)realloc(arr, sizeof(int*) * MAX(first_top, second_top));

				for (int i = 0; i < MAX(first_top, second_top); i++) {
					if (!arr[i]) 
						arr[i] = (int*)calloc(MAX(first_top, second_top), sizeof(int));       
					else {
						arr[i] = (int*)realloc(arr[i], sizeof(int) * MAX(first_top, second_top));
						for (int j = max_top; j < MAX(first_top, second_top); j++) arr[i][j] = 0;
					}
					max_top = MAX(first_top, second_top);
				}
			}
			arr[second_top - 1][first_top - 1] += 1;
			first_top = 0;
			second_top = 0;
		}
	}

	fprintf(file_graph, "}");
	
	error = is_tree(arr, max_top);

	if (error) 
		printf("This graph isn't a tree!\n");
	else 
		printf("This graph is a tree!\n");

	for (int i = 0; i < max_top; i++) 
		free(arr[i]);
	free(arr);
	fclose(file_graph);
	system("dot graph.dot -Tpng -o graph.png");

	return 0;
}


int is_tree(int **array, int N) {                 //проверка на дерево
	int error = 0;
	int check_sum = 0;

	for (int i = 0; i < N; i++) {
		error += array[i][i] ? 1 : 0;
		error += edge_multi(array[i], N);
		check_sum += edge_sum(array[i], N);
		if (error)
			return error;
	}

	if (check_sum != (N - 1))             
		error += 1;
	return error;
}

int edge_sum(int* array, int N) {                //проверка суммы ребер из вершины
	int sum = 0;
	for (int i = 0; i < N; i++)
		sum += array[i];

	return sum;
}

int edge_multi(int* array, int N) {              //проверка каждого ребра из вершины, чтобы исключить кратность
	for (int i = 0; i < N; i++) 
		if (array[i] > 1) 
			return 1;
	return 0;
}
