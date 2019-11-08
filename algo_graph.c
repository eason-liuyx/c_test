#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/**
 * Title: division evaluation 399
 *
 * Problem: give an equation A / B = k, A and B are string, k is double float. 
 * According the known equations to solve unknown equation, return result.
 * If not existing, return -1.0.
 *
 * Analysis: actually it is a directed graph problem. To calculate A / Z,
 * we only need to find a path that directed from A to Z.
 * DFS: depth first search, BFS: breadth first search.
 */

/**
 * Note: The returned array must be malloced, assume caller calls free().
 */

struct strgraph {
	double**	arc;
	int	edgenum;
};

struct nodestr {
	char* a;
};

double* calcEquation(char*** equations, int equationsSize, int* eqationsColSize,
		     double* values, int valuesSize, char*** queries,
		     int queriesSize, int* queriesColSize, int* returnSize) {
	struct strgraph graph;
	struct nodestr *stridx;
	double* retArr;
	int edge1, edge2;
	int i, j, k;
	int node;

	if (equationsSize == 0)
		return NULL;

	/* Construct Graph */
	graph.edgenum = equationsSize;
	graph.arc = malloc(2 * equationsSize * sizeof(double*));
	memset(graph.arc, 0, (2 * equationsSize * sizeof(double*)));

	for(i = 0; i < 2 * equationsSize; i++) {
		graph.arc[i] = malloc(2 * equationsSize * sizeof(double));
		memset(graph.arc[i], 0, (2 * equationsSize * sizeof(double)));
	}

	stridx = malloc(2 * equationsSize * sizeof(struct nodestr));
	memset(stridx, 0, (2 * equationsSize * sizeof(struct nodestr)));
	for(i = 0; i < equationsSize; i++) {
		edge1 = edge2 = node;
		for(j = 0; j < node; j++){
			if ((edge1 == node) &&
			    (strcmp(stridx[j].a, equations[i][0]) == 0))
				edge1 = j;

			if ((edge2 == node) &&
			    (strcmp(stridx[j].a, equations[i][1]) == 0))
				edge2 = j;
		}

		if (edge1 == node && edge2 == node) {
			edge2 = node + 1;
			node += 2;
			strcpy(stridx[edge1].a, equations[i][0]);
			strcpy(stridx[edge2].a, equations[i][1]);
		} else if (edge1 == node) {
			node++;
			strcpy(stridx[edge1].a, equations[i][0]);
		} else if (edge2 == node) {
			node++;
			strcpy(stridx[edge2].a, equations[i][1]);
		}

		graph.arc[edge1][edge2] = values[i];
	}

	for(j = 0; j < node; j++){
		for(k = 0; k < node; k++) {
			if(graph.arc[j][k] != 0 && graph.arc[k][j] == 0)
				graph.arc[k][j] = 1/graph.arc[j][k];
		}

		graph.arc[j][j] = 1.0;
	}

	for(i = 0; i < node; i++){
		for(j = 0; j < node; j++) {
			for (k = 0; k < node; k++) {
				if (graph.arc[j][k] == 0 &&
				    (graph.arc[j][i] != 0 &&
				     graph.arc[i][k] != 0))
					graph.arc[j][k] = graph.arc[j][i] *
							  graph.arc[i][k];
			}
		}
	}

	*returnSize = 0;
	retArr = malloc(sizeof(double) * queriesSize);
	for(i = 0; i < queriesSize; i++) {
		edge1 = edge2 = node;
		for(j = 0; j < node; j++) {
			if((edge1 == node) &&
			   (strcmp(stridx[j].a, queries[i][0]) == 0))
				edge1 = j;
			
			if ((edge2 == node) &&
			    (strcmp(stridx[j].a, queries[i][1]) == 0))
				edge2 = j;
		}

		if (edge1 == node || edge2 == node ||
		    graph.arc[edge1][edge2] == 0)
			retArr[*returnSize] = -1;
		else
			retArr[*returnSize] = graph.arc[edge1][edge2];

		*returnSize += 1;
	}

	return retArr;
}

