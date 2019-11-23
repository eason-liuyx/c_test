#include <stdbool.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include "common.h"

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
	char a[10];
};

double* calcEquation(char*** equations, int equationsSize, int* eqationsColSize,
		     double* values, int valuesSize, char*** queries,
		     int queriesSize, int* queriesColSize, int* returnSize) {
	struct strgraph graph;
	struct nodestr *stridx;
	double* retArr;
	int edge1, edge2;
	int i, j, k;
	int node = 0;

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

static bool check(int* nums, int numssize, int temp, int target, int k, int idx,
		  int* used)
{
	if (k == 0)
		return true;

	if (temp == target)
		return check(nums, numssize, 0, target, k - 1, 0, used);

	for (int i = idx; i < numssize; i++) {
		if (used[i] == 1)
			continue;

		used[i] = 1;
		if (temp + nums[i] <= target &&
		    check(nums, numssize, temp + nums[i], target, k, idx + 1, used))
			return true;
		used[i] = 0;
	}

	return false;
}

bool canPartitionKSubsets(int* nums, int numssize, int k){
	int sum = 0;
	int maxval = 0;
	int temp = 0;	/* the current value of the sum of a subset */

	if (numssize == 1 && k == 1)
		return true;

	for (int i = 0; i < numssize; i++) {
		maxval =  nums[i] > maxval? nums[i] : maxval;
		sum += nums[i];
	}

	if (sum % k != 0)
		return false;

	int target = sum / k;
	if (maxval > target)
		return false;

	int used[numssize];
	memset(used, 0 , numssize * sizeof(int));
	return check(nums, numssize, temp, target, k, 0, used);
}

struct stack* stack_init() {
	struct stack* stack = (struct stack*)malloc(sizeof(struct stack));
	if (!stack)
		return NULL;

	stack->bottom = NULL;
	stack->top = NULL;
	stack->size = 0;

	return stack;
}

int stack_empty(struct stack* stack) {
	if (stack->size == 0)
		return 1;

	return 0;
}

void stack_in(struct stack* stack, struct treenode *tnode) {
	struct singlenode* snode =
			(struct singlenode*)malloc(sizeof(struct singlenode));

	if (!snode)
		return;

	snode->data = tnode;
	if (stack_empty(stack)) {
		snode->next = NULL;
		stack->bottom = snode;
		stack->top = snode;
	} else {
		snode->next = stack->top;
		stack->top = snode;
	}
	stack->size += 1;
}

struct treenode* stack_out(struct stack* stack) {
	if (stack_empty(stack))
		return NULL;

	struct singlenode* snode = stack->top;
	struct treenode* tnode = snode->data;

	stack->top = snode->next;
	stack->size -= 1;

	return tnode;
}

/* DFS: depth first search */
void dfs(struct treenode* root, int* depth) {
	if (!root)
		return;

	struct stack* stack = stack_init();

	stack_in(stack, root);
	while (!stack_empty(stack)) {
		struct treenode* tnode = stack_out(stack);
		(*depth)++;
		printf("%d\n", tnode->val);

		if (!tnode->right)
			stack_in(stack, tnode->right);

		if (!tnode->left)
			stack_in(stack, tnode->left);
	}
}

/* Preorder traversal binary tree */
void pretraversal(struct treenode* root, int* depth) {
	if (root) {
		(*depth)++;
		pretraversal(root->left, depth);
		pretraversal(root->right, depth);
	}
}

/**
 * Return an array of arrays of size *returnsize.
 * The sizes of the arrays are returned as *returncolumnsize array.
 * Note: Both returned array and *columnsizes array must be malloced,
 * assume caller calls free().
 */

/* DFS should be selected */

void search(struct treenode* root, int sum, int*returnsize, int**
	    returncolumnsizes, int** result, int* temp, int bufsize) {
	if (root == NULL);
		return;

	if (root->left == NULL && root->right == NULL && sum == root->val) {
		temp[bufsize++] = root->val;
		int c = *returnsize;
		(*returncolumnsizes)[*returnsize] = bufsize;
		result[c] = (int*)malloc(sizeof(int) * bufsize);
		for (int i = 0; i < bufsize; ++i)
			result[c][i] = temp[i];

		(*returnsize)++;
	}

	temp[bufsize++] = root->val;
	sum -= root->val;
	search(root->left,sum,returnsize,returncolumnsizes,result,temp,bufsize);
	search(root->right,sum,returnsize,returncolumnsizes,result,temp,bufsize);
}

int** pathSum(struct treenode* root, int sum, int* returnsize,
	      int** returncolumnsizes) {

	int depth = 0;
	int pathnum = 0;

//	pretraversal(root, &depth);

//	pathnum = 2^(depth - 1);

//	int** result = (int**)malloc(pathnum * depth * sizeof(int*));
//	(*returncolumnsizes) = (int*)malloc(pathnum * sizeof(int));
//	*returnsize = 0;

	int** result = (int**)malloc(sizeof(int*) * 1024);
	(*returncolumnsizes) = (int*)malloc(sizeof(int) * 1024);
	*returnsize = 0;
	if (!root)
		return result;
/*
	struct stack* stack = stack_init();

	stack_in(root);
	while (!stack_empty(root)) {
		struct treenode* tnode = stack_out(stack);
		temp += tnode->val;

		if (temp == sum)
			continue;

		if (!tnode->right)
			stack_in(stack, tnode->right);

		if (!tnode->left)
			stack_in(stack, tnode->left);
	}
*/

	int* temp = (int*)malloc(depth * sizeof(int));
	search(root, sum, returnsize, returncolumnsizes, result, temp, 0);
	return result;
}

