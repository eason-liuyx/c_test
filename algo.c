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

void quick_sort(int* arr, int left, int right) {
	if (left >= right)
		return;

	int i = left;
	int j = right;
	int key = arr[left];
	while (i < j) {
		while (i < j && key <= arr[j]) {
			j--;
		}

		arr[i] = arr[j];

		while (i < j && key >= arr[i]) {
			i++;
		}

		arr[j] = arr[i];
	}

	arr[i] = key;
	quick_sort(arr, left, i - 1);
	quick_sort(arr, i + 1, right);
}

void quick_sort_descend(int* arr, int left, int right) {
	if (left >= right)
		return;

	int i = left;
	int j = right;
	int key = arr[right];
	while (i < j) {
		while (i < j && key <= arr[i]) {
			i++;
		}

		arr[j] = arr[i];

		while (i < j && key >= arr[j]) {
			j--;
		}

		arr[i] = arr[j];
	}

	arr[j] = key;
	quick_sort_descend(arr, left, j - 1);
	quick_sort_descend(arr, j + 1, right);
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

int get_tree_depth(struct TreeNode* root) {
	if (root == NULL)
		return 0;

	//printf("%d", root->val);
	int ldepth = get_tree_depth(root->left);
	int rdepth = get_tree_depth(root->right);

	return ldepth > rdepth ? ldepth + 1 : rdepth + 1;
	/* FIXME: why bug here */
//	return max(ldepth, rdepth) + 1;
}

/* Preorder traversal binary tree */
void pretraversal(struct TreeNode* root, int* depth) {
	if (root) {
		(*depth)++;
		printf("%d", root->val);
		pretraversal(root->left, depth);
		pretraversal(root->right, depth);
	}
}

/* In-order traversal */
void intraversal(struct TreeNode* root, int* depth) {
	if (root) {
		(*depth)++;
		intraversal(root->left, depth);
		printf("%d", root->val);
		intraversal(root->right, depth);
	}
}

void posttraversal(struct TreeNode* root, int* depth) {
	if (root) {
		posttraversal(root->left, depth);
		posttraversal(root->right, depth);
		printf("%d", root->val);
	}
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

void stack_in(struct stack* stack, struct TreeNode *tnode) {
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

struct TreeNode* stack_out(struct stack* stack) {
	if (stack_empty(stack))
		return NULL;

	struct singlenode* snode = stack->top;
	struct TreeNode* tnode = snode->data;

	stack->top = snode->next;
	stack->size -= 1;

	return tnode;
}

struct queue* queue_init() {
	struct queue* queue = (struct queue*)malloc(sizeof(struct queue));
	if (queue == NULL)
		return NULL;

	queue->head = NULL;
	queue->tail = NULL;
	queue->size = 0;

	return queue;
}

int queue_empty(struct queue* queue) {
	if (queue->size == 0)
		return 1;

	return 0;
}

void queue_in(struct queue* queue, struct TreeNode* root) {
	struct singlenode* node =
			 (struct singlenode*)malloc(sizeof(struct singlenode));
	if (node == NULL)
		return;

	node->data = root;
	node->next = NULL;
	if (queue_empty(queue))
		queue->head = node;
	else
		queue->tail->next = node;

	queue->tail = node;
	queue->size += 1;
}

struct TreeNode* queue_out(struct queue* queue) {
	if (queue_empty(queue))
		return NULL;

	struct singlenode* node = queue->head;
	queue->head = node->next;
	queue->size -= 1;
	struct TreeNode* tnode = node->data;

	return tnode;
}

struct TreeNode* queue_pop(struct queue* queue) {
	if (queue_empty(queue))
		return NULL;

	struct TreeNode* tnode = queue->tail->data;

	return tnode;
}

/* BFS: breadth first search */
void bfs(struct TreeNode* root, int* depth) {
	*depth = 0;
	if (root == NULL)
		return;

	int left_depth = 0;
	int right_depth = 0;
	struct TreeNode* tnode;
	struct queue* queue = queue_init();

	queue_in(queue, root);
	while (!queue_empty(queue)) {
		tnode = queue_out(queue);
		printf("%d", tnode->val);
		if (tnode->left) {
			left_depth++;
			queue_in(queue, tnode->left);
		}
		if (tnode->right) {
			right_depth++;
			queue_in(queue, tnode->right);
		}
	}
	*depth = max(left_depth, right_depth) + 1;
}

/* DFS: depth first search */
void dfs(struct TreeNode* root, int* depth) {
	int left_depth = 0;
	int right_depth = 0;
	if (!root)
		return;

	struct TreeNode* tnode;
	struct stack* stack = stack_init();

	stack_in(stack, root);
	while (!stack_empty(stack)) {
		tnode = stack_out(stack);
		printf("%d", tnode->val);

		if (tnode->right) {
			right_depth++;
			stack_in(stack, tnode->right);
		}

		if (tnode->left) {
			left_depth++;
			stack_in(stack, tnode->left);
		}
	}
	*depth = max(left_depth, right_depth) + 1;
}

struct TreeNode* __create_tree(int* data, int index, int n) {
	struct TreeNode* node = NULL;

	/* -1 means NULL */
	if (index < n && data[index] != -1) {
		node = (struct TreeNode*)malloc(sizeof(struct TreeNode));
		if (node == NULL)
			return NULL;

		node->val = data[index];
		/* index determined by layer order */
		node->left = __create_tree(data, 2 * index + 1, n);
		node->right = __create_tree(data, 2 * index + 2, n);
	}
	return node;
}

struct TreeNode* create_tree(struct TreeNode* root) {
	int input[TREE_NODE_MAX_NUM];
	int count;
	int depth;
	int val;
	char ch;
	int i = 0;

	for (i = 0; i < TREE_NODE_MAX_NUM; i++)
	{
		scanf("%d", &input[i]);
		count++;

		ch = getchar();
		if (ch == '\n')
			break;
	}

//		printf("%d", input[i]);
	root = __create_tree(input, 0, count);
	printf("count:%d\n", count);
	printf("\ndepth:%d\n", get_tree_depth(root));
	return root;
}

void traverse_bitree(struct TreeNode* root, int *node_num){
	(*node_num)++;
	if (!root)
		return;

	traverse_bitree(root->left, node_num);
	traverse_bitree(root->right, node_num);
}

void serial_bitree(struct TreeNode* root, int* array){
	if (!root) {
		*array = 0;
		return;
	}

	*array = root->val;
	traverse_bitree(root->left, array + 1);
	traverse_bitree(root->right, array + 2);
}

struct TreeNode** findDuplicateSubtrees(struct TreeNode* root, int* returnSize){
	struct TreeNode** pNode = malloc(sizeof(struct TreeNode*));
	int node_num = 0;

	*pNode = root;
	traverse_bitree(root, &node_num);

	int* array_list = calloc(node_num, sizeof(int));

	serial_bitree(root, array_list);

	return pNode;
}

int** sort_interval(int** intervals, int intervalsSize){
	int tmpstart, tmpend;
	int curstart, nextstart;
	for (int i = 0; i < intervalsSize - 1; i++){
		curstart = intervals[i][0];
		nextstart = intervals[i + 1][0];

		if (curstart > nextstart){
		tmpstart = intervals[i][0];
		tmpend = intervals[i][1];
		intervals[i][0] = intervals[i + 1][0];
		intervals[i][1] = intervals[i + 1][1];
		intervals[i + 1][0] = tmpstart;
		intervals[i + 1][1] = tmpend;
		}
	}
	return intervals;
}

int** merge(int** intervals, int intervalsSize, int* intervalsColSize,
	    int* returnSize, int** returnColumnSizes){
	int isize = intervalsSize;
	int mergecnt = 0;
	int k = 0;
	int i;
	printf("iSize is %d, icolsize is %d\n", isize, *intervalsColSize);
	if (isize == 0) {
		int** array = (int**)malloc(sizeof(int *));
		array[0] = malloc(sizeof(int));
		array[0] = NULL;
		*returnSize = 0;
		int** returnColumnSizes = (int **)malloc(sizeof(int *));
		returnColumnSizes[0] = malloc(sizeof(int));
		returnColumnSizes[0][0] = 2;
		return NULL;
	}

	if (isize == 1) {
		int** array = (int **)malloc(sizeof(int *));
		array[0] = malloc(sizeof(int) * 2);
		array[0] = intervals[0];

		int** returnColumnSizes = (int **)malloc(sizeof(int *));
		returnColumnSizes[0] = malloc(sizeof(int));
		returnColumnSizes[0][0] = 2;

		return array;
	}

	intervals = sort_interval(intervals, isize); //排序

	for (i = 0; i < isize; i++){
		printf("original inter[%d], %d, %d\n", i, intervals[i][0], intervals[i][1]);
	}

	//merge interval
	int curend, nextstart;
	for (i = 0; i < isize - 1; i++){
		curend = intervals[i][1];
		nextstart = intervals[i + 1][0];
		if (curend >= nextstart){
			intervals[i + 1][0] = intervals[i][0];
			intervals[i + 1][1] = max(intervals[i][1], intervals[i + 1][1]);
			intervals[i][0] = -1; // will be deleted
			mergecnt++;
		}
	}

	for (i = 0; i < isize; i++){
		printf("after merge inter[%d], %d, %d\n", i, intervals[i][0], intervals[i][1]);
	}

	int retsz = isize - mergecnt;    // 行数

	//malloc array
	int** array = (int **)malloc(sizeof(int *) * retsz);
	int* retcolsz = (int *)malloc(sizeof(int) * retsz);
	for (int j = 0; j < retsz; j++){
		array[j] = malloc(sizeof(int) * 2);
		retcolsz[j] = 2;
	}

	//fill array
	for (i = 0; i < intervalsSize && k < retsz; i++){
		if (intervals[i][0] != (-1)) {
			array[k][0] = intervals[i][0];
			array[k][1] = intervals[i][1];
			k++;
		}
	}

	*returnSize = retsz;
	*returnColumnSizes = retcolsz;

	for (k = 0; k < retsz; k++) {
		printf("array[%d], %d, %d\n", k, array[k][0], array[k][1]);
		printf("returncolSizes, %d\n", (*returnColumnSizes)[k]);
	}

	printf("returnSize %d\n", *returnSize);

	return array;
}

char * longestPalindrome(char* s){
	int mirroridx1 = 0;
	int mirroridx2 = 0;
	int eq_len1 = 0;
	int eq_len2 = 0;
	int max_len1 = 0;
	int max_len2 = 0;
	int right = 0;
	int slen = strlen(s);

	printf("%d\n", slen);
	/* even case: similar as bb */
	for (int i = 0; i < slen - 1; i++) {
		eq_len1 = 0;

		if (s[i] == s[i + 1]){
			eq_len1 = 1;
			int j = i - 1;
			right = i + 1 + i - j;
			while (j >= 0 && right < slen){
				if (s[j] != s[right])
					break;
				eq_len1++;
				j--;
				right = i + 1 + (i - j);
			}

			if (eq_len1 >= max_len1){
				max_len1 = eq_len1;
				mirroridx1 = i;
			}
		}
	}

	printf("%d\n", max_len1);

	/* odd case: similar as bab */
	for (int i = 0; i < slen - 2; i++){
		eq_len2 = 0;

		if (s[i] == s[i + 2]){
			eq_len2 = 1;
			int j = i - 1;
			right = i + 2 + i - j;
			while (j >= 0 && right < slen){
				if (s[j] != s[right])
				break;

			eq_len2++;
			j--;
			right = i + 2 + i - j;
			}

			if (eq_len2 >= max_len2){
				max_len2 = eq_len2;
				mirroridx2 = i;
			}
		}
	}

	printf("%d\n", max_len2);

	/* even */
	if (max_len1 > max_len2){
		int k;
		char outs[2 * max_len1 + 1];
		for (k = 0; k < (2 * max_len1); k++)
			outs[k] = s[mirroridx1 - (max_len1 - k) + 1];
			outs[k] = '\0'; 
			s = outs;

		return s;
	} else { /* odd */
		int k;
		char outs[2 * max_len2 + 2];
		for (k = 0; k < (2 * max_len2 + 1); k++)
			outs[k] = s[mirroridx2 - (max_len2 - k) + 1];
			outs[k] = '\0';
			s = outs;

		return s;
	}
}

int strlabel(){
	char words[MAX_WD_NUM][MAX_WD_LEN];
	char input[MAX_STR_LEN];
	char *p;
	int bitmap[MAX_STR_LEN + 2] = {0};
	int pos = 0;
	int wd_num;
	int len;

	/* input word count */
	scanf("%d", &wd_num);
	if (wd_num < 1 || wd_num > MAX_WD_NUM) {
		printf("Invalid word num: %d\n", wd_num);
		goto out;
	}

	/* input words */
	for (int i = 0; i < wd_num; i++) {
		scanf("%s", input);
		len = strlen(input);
		if (len < 1 || len > MAX_WD_LEN) {
			printf("Invalid word len:%d\n", len);
			goto out;
		}
		sprintf(words[i], "%s", input);
	}

	/*input string */
	scanf("%s", input);
	len = strlen(input);
	if (len < 1 || len > MAX_STR_LEN) {
		printf("Invalid string len:%d\n", len);
		goto out;
	}

	/* match word in string */
	for (int i = 0; i < wd_num; i++) {
		do {
			p = strstr(input + pos, words[i]);
			if (p) {
				pos = p - input + 1;
				len = strlen(words[i]);
				for (int j = pos; j < pos + len; j++)
					bitmap[j] = 1;

				p = NULL;
			} else {
				pos = 0;
				break;
			}
		} while(1);
	}

	len = strlen(input);
	for (int i = 0; i < len + 2; i++)
		printf("%d", bitmap[i]);

	printf("\n");

	/* output string with label */
	int i = 0;
	do {
		if (bitmap[i] == 0 && bitmap[i + 1] == 1)
			printf("<b>%c", input[i]);
		else if (bitmap[i] == 1 && bitmap[i + 1] == 0)
			printf("</b>%c", input[i]);
		else
			printf("%c", input[i]);
		i++;
	} while(input[i - 1]);

	printf("\n");
	return 0;
out:
	return -1;
}



/**
 * Return an array of arrays of size *returnsize.
 * The sizes of the arrays are returned as *returncolumnsize array.
 * Note: Both returned array and *columnsizes array must be malloced,
 * assume caller calls free().
 */

/* DFS should be selected */

void search(struct TreeNode* root, int sum, int*returnsize, int**
	    returncolumnsizes, int** result, int* temp, int bufsize) {
	if (root == NULL)
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

#if 1
int** pathSum(struct TreeNode* root, int sum, int* returnsize,
	      int** returncolumnsizes) {

	int depth = 0;
	int pathnum = 0;
	int** result = (int**)malloc(sizeof(int*) * 1024);
	(*returncolumnsizes) = (int*)malloc(sizeof(int) * 1024);
	*returnsize = 0;

	if (!root)
		return result;

	int* temp = (int*)malloc(1024 * sizeof(int));
	search(root, sum, returnsize, returncolumnsizes, result, temp, 0);
	return result;
}
#endif

#if 0
int** pathSum(struct TreeNode* root, int sum, int* returnsize,
	      int** returncolumnsizes) {

	int depth = 0;
	int pathnum = 0;
	int i = 0;
	int temp_sum = 0;

	pretraversal(root, &depth);
	pathnum = 2^(depth - 1);

	int** result = (int**)malloc(pathnum * depth * sizeof(int*));
	(*returncolumnsizes) = (int*)malloc(pathnum * sizeof(int));
	*returnsize = 0;

	if (!root)
		return result;

	struct stack* stack = stack_init();

	stack_in(stack, root);
	int* temp_array = (int*)malloc(depth * sizeof(int));
	while (!stack_empty(stack)) {
		struct TreeNode* tnode = stack_out(stack);
		temp_array[i] = tnode->val;
		temp_sum += tnode->val;
		if (temp_sum == sum) {
			stack_in(stack, root);
			i = 0;
			continue;
		}

		if (tnode->left)
			stack_in(stack, tnode->left);

		if (tnode->right)
			stack_in(stack, tnode->right);
	}

	int* temp = (int*)malloc(depth * sizeof(int));
	search(root, sum, returnsize, returncolumnsizes, result, temp, 0);
	return result;
}
#endif

int leastInterval(char* tasks, int tasksSize, int n) {

	int a[UP_ALPHABET_LEN] = {0};

	/* hash tasks into an array of statistical */
	for (int i = 0; i < tasksSize; i++)
		a[tasks[i] - 'A']++;

	quick_sort(a, 0, UP_ALPHABET_LEN - 1);

	int time = 0;
	while (a[UP_ALPHABET_LEN - 1] > 0) {
		int i = 0;
		while (i <= n) {
			if (a[UP_ALPHABET_LEN - 1] == 0)
				break;

			if (i < UP_ALPHABET_LEN &&
			    a[UP_ALPHABET_LEN - 1 - i] > 0)
				a[UP_ALPHABET_LEN - 1 - i]--;

			time++;
			i++;
		}
		quick_sort(a, 0, UP_ALPHABET_LEN - 1);
	}
	return time;
}

char* pushDominoes(char* dominoes) {
	bool flag = false; /* is R waiting for matching */
	int start = 0; /* position of last R */
	int domi_size = strlen(dominoes);

	for (int i = 0; i < domi_size; i++) {
		if (dominoes[i] == 'L') {
			if (flag == false)
				for (int r = i - 1;
				     r >= 0 && dominoes[r] == '.'; r--)
					dominoes[r] = 'L';
			else {
				for (int l = start + 1, r = i - 1;
				     l < i && l < r; l++, r--) {
					dominoes[l] = 'R';
					dominoes[r] = 'L';
				}
				flag = false;
			}
		} else if (dominoes[i] == 'R') {
			if (flag)
				for (int l = start + 1; l < i; l++)
					dominoes[l] = 'R';

			flag = true;
			start = i;
		}
	}

	if (flag)
		for (int l = start + 1; l < domi_size; l++)
			dominoes[l] = 'R';

	return dominoes;
}

/* how to buy one ticket with least cost */
int mincostOneTickets(int* days, int daysSize, int* costs, int costsSize) {
	if (daysSize == 0)
		return 0;

	if (daysSize == 1)
		return costs[0];

	int flag = false; /* whether large then 7 days */
	int cost = 0;
	int tmp = 0;
	int day_start = days[0];
	int duration = 0;
	int days_num = 0; /* record the total days for one ticket */
	int daynum_7 = 0;
	int daynum_30 = 0;
	float avg_day30 = 0;
	float avg_day7 = 0;
	for (int i = 0; i < daysSize; i++) {
		duration = days[i] - day_start;
		if (duration > 30) {
			break;
		}

		if (duration >= 7) {
			daynum_30++;
			continue;
		}

		daynum_30++;
		daynum_7++;
	}

	avg_day30 = (float)costs[2] / daynum_30;
	avg_day7 = (float)costs[1] / daynum_7;
	if (costs[0] <= avg_day7) {
		days_num = 1;
		cost = costs[0];
	} else if (avg_day7 < avg_day30) {
		days_num = daynum_7;
		cost = costs[1];
	} else { 
		days_num = daynum_30;
		cost = costs[2];
	}
out:
	daysSize = daysSize - days_num;
	days = &days[days_num];
	return cost + mincostOneTickets(days, daysSize, costs, costsSize);
}

int mincostTickets(int* days, int daysSize, int* costs, int costsSize) {
	int day_cost[366]; /*day_cost[i][0] buy for 7-day */
	int dayidx = 0;
	int ticketday = costs[0];
	int ticketweek = costs[1];
	int ticketmonth = costs[2];

	day_cost[0] = 0;

	for (int today = 1; today <= 365; today++) {
		if (dayidx >= daysSize) {
			break;
		}

		if (days[dayidx] != today) {
			day_cost[today] = day_cost[today - 1];
			continue;
		}
		dayidx++;
		day_cost[today] = min(day_cost[max(0, today - 1)] + ticketday,
				      day_cost[max(0, today - 7)] + ticketweek);
		day_cost[today] = min(day_cost[today],
				      day_cost[max(0, today - 30)] + ticketmonth);
	}
	return day_cost[days[daysSize - 1]];

#if 0
	/* calc price per day with different cost */
	float p1 = costs[0];
	float p2 = costs[1] / 7;
	float p3 = (float)costs[2] / 30;

	float days_thrd12 = p1 / p2;
	float days_thrd13 = p1 / p3;
	float days_thrd23 = p2 / p3;

	if (p1 < p2 && p1 < p3)
		return p1 * daysSize;

	int in_day7 = 0; /* the interval of two days below 7 */
	int in_day30 = 0;
	int end_day7 = 0; /* which day this ticket ends */ 
	int end_day30 = 0; /* which day this ticket ends */
	int cost7 = 0; /* one ticket per day */
	int cost30 = 0;
	int dp[daysSize]; /* record cost per day */

	for (int i = 0; i < daysSize; i++) {
		for (int j = i; j < i + 7; j++)
			if (day[j] - day[i] < 7) {
				end_day7 = j;
				in_day7++;
			}
		cost7 = in_day7 * costs[0];

		for (int j = i; j < i + 30; j++)
			if (day[j] - day[i] < 30) {
				end_day30 = j;
				in_day30++;
			}
		cost30 = in_day30 * costs[0];

		if (in_day7 * costs[0] > costs[1]) {


		dp[i] = min(dp[i + 1] +  costs[0], dp[i + 7] + costs[1],
			    dp[i + 30] + costs[2]);

	return mincostOneTickets(days, daysSize, costs, costsSize);
#endif
}

static int dist_calc(int* point) {
	return point[0] * point[0] + point[1] * point[1];
}

static void swap(int* point1, int* point2) {
	int tmp1 = point1[0], tmp2 = point1[1];
	point1[0] = point2[0];
	point1[1] = point2[1];
	point2[0] = tmp1;
	point2[1] = tmp2;
}

int** kClosest(int** points, int pointsSize, int* pointColSize, int K,
	       int* returnSize, int** returnColumnSizes) {
	if (K == 1 && pointsSize == 1) {
		int **array = (int**)malloc(sizeof(int*));
		int *retcolsz = (int*)malloc(sizeof(int));
		array[0] = (int*)malloc(sizeof(int) * (*pointColSize));
		array[0] = points[0];
		*returnColumnSizes = pointColSize;
		*returnSize = 1;
		return array;
	}

	int dist[pointsSize];

	for (int i = 0; i < pointsSize; i++)
		dist[i] = dist_calc(points[i]);

	quick_sort(dist, 0, pointsSize - 1);
	int Kth_dist = dist[K - 1];

	int** array = (int**)malloc(sizeof(int*) * K);
	int* retcolsz = (int*)malloc(sizeof(int) * K);

	*returnSize = 0;
	for (int i = 0; i < pointsSize; i++)
		if (dist_calc(points[i]) <= Kth_dist) {
			array[*returnSize] = malloc(sizeof(int) * 2);
			array[*returnSize] = points[i];
			retcolsz[*returnSize] = *pointColSize;
			(*returnSize)++;
		}

	*returnColumnSizes = retcolsz;

	return array;
}

/* size: the size of first part of the region after divided */
void region_divide(int** points, int left, int right, int K) {
	if (left >= right)
		return;

	int i = left;
	int j = right;
	int key = dist_calc(points[left]);

	while (i < j) {
		while (i < j && key < dist_calc(points[j])) j--;
		while (i < j && key > dist_calc(points[i])) i++;
		if (key== dist_calc(points[j]) && key == dist_calc(points[i]))
			break;
		swap(points[i], points[j]);
	}

	if (K <= i - left + 1)
		region_divide(points, left, i, K);
	else
		region_divide(points, i + 1, right, K - (i - left + 1));
}

/* use divide and conqer */
int** kClosest_DC(int** points, int pointsSize, int* pointColSize, int K,
		  int* returnSize, int** returnColumnSizes) {
	if (K == 1 && pointsSize == 1) {
		int **array = (int**)malloc(sizeof(int*));
		int *retcolsz = (int*)malloc(sizeof(int));
		array[0] = (int*)malloc(sizeof(int) * (*pointColSize));
		array[0] = points[0];
		*returnColumnSizes = pointColSize;
		*returnSize = 1;
		return array;
	}

	region_divide(points, 0, pointsSize - 1, K);

	int** array = (int**)malloc(sizeof(int*) * K);
	int* retcolsz = (int*)malloc(sizeof(int) * K);

	*returnSize = 0;
	for (int i = 0; i < K; i++) {
		array[*returnSize] = malloc(sizeof(int) * 2);
		array[*returnSize] = points[i];
		retcolsz[*returnSize] = *pointColSize;
		(*returnSize)++;
	}

	*returnColumnSizes = retcolsz;

	return array;
}

int findAllgenes(int len, char* GeneSeq) {
	if (len <= 2)
		return 0;

	int winlen;
	int start; /* subgene start index */
	int count = 0;
	int pos;

	for (int i = 1; i < len - 1; i++) {
		printf("i = %d   ", i);
		winlen = len - i;
		for (int j = 0; j <= i; j++) {
			char* p = NULL;
			char tmp[winlen + 1]; /* subgene */
			start = j;
			strncpy(tmp, GeneSeq + start, winlen);
			tmp[winlen] = '\0';
			p = strstr(GeneSeq, tmp);
			pos = p - GeneSeq;
			if (pos == start) {
				printf("%s", tmp);
				count++;
				printf(" ");
			}
		}
		printf("\n");
	}

	return count;
}

/* remove val from array nums[] */
int removeElement(int* nums, int numSize, int val) {
	int retsz = numSize;

	for(int i = 0; i < retsz; i++) {
		if (nums[i] == val) {
			for(int j = retsz - 1; j >= i; j--) {
				retsz -= 1;
				if (nums[j] == val)
					continue;
				else {
					nums[i] = nums[j];
					break;
				}
			}
		}
	}

	return retsz;
}

void nextPermutation(int* nums, int numsSize) {
	int has_next = 0;
	for(int j = numsSize - 1; j > 0; j--)
		if (nums[j] > nums[j - 1]) {
			has_next = 1;
			quick_sort_descend(&nums[j - 1], 0, numsSize - j);
			break;
		}

	if (!has_next)
		quick_sort_descend(nums, 0, numsSize - 1);
}

void rotate(int** matrix, int matrixSize, int* matrixColsize) {
	int msize = *matrixColsize;
	for (int i = 0; i < msize; i++) {
		//TODO
	}
}

# if 0
#define MAX_GROUP 15

struct intvs {
	int a; // start value
	int b; // end value
	int calc_flag; // indicate this interval is already added to sum.
	int invalid; // 1: indicate this interval is invalid, 0: valid
};

int calc_intervals(struct intvs *intv, int num){
	int total;
	int nextstart;
	int curend;
	for (int i = 0; i < num; i++) {

		curend = intv[i].b;
		nextstart = intv[i + 1].a;
		if (nextstart <= curend)
			intv[i + 1][1] = intv[i][1] + intv[i + 1][1];

		if (nextstart > curend)
			if (nextstart + intv[i][0] <= intv[i + 1][1])
				intv[i + 1][1] = intv[i][1] + intv[i + 1][1];
}

void merge_intervals(struct intvs *intv, int num) {
	int curend, nextstart;
	for (int i = 0; i < num; i++) {
		curend = intv[i].b;
		nextstart = intv[i + 1].a;

		if (nextstart <= curend) {
			intv[i].invalid = 1;
			intv[i + 1].b = intv[i].b + intv[i + 1].b;
		}
}

void sort_intervals(struct intvs *intv, int num){
	int tmpstart, tmpend;
	int curstart, nextstart;
	for (int i = 0; i < num - 1; i++){
		curstart = intv[i].a;
		nextstart = intv[i + 1].a;

		if (curstart > nextstart){
		tmpstart = intv[i].a;
		tmpend = intv[i].b;
		intv[i].a = intv[i + 1].a;
		intv[i].b = intv[i + 1].b;
		intv[i + 1].a = tmpstart;
		intv[i + 1].b = tmpend;
		}
	}
}

void calcSum() {
	int n;
	int a[MAX_GROUP];
	int b[MAX_GROUP];
	int region[MAX_GROUP * 4][2];
	int *intvs[MAX_GROUP];
	int total;

//	struct intervals *intv = malloc(sizeof(struct intervals) * MAX_GROUP);

	scanf("%d", &n);

	for (int i = 0; i < n; i++) {
		scanf("%d", &a[i]);
	}
	
	for (int i = 0; i < n; i++) {
		scanf("%d", &b[i]);
	}

	/* n is even */
	if (n % 2 == 0) {
		for (int i = 0; i < n / 2; i++) {
			int j = 4 * i;
			region[j][0] = a[i];
			region[j][1] = b[i];
			region[j + 1][0] = a[i + 1];
			region[j + 1][1] = b[i + 1];
			region[j + 2][0] = a[i] + a[i + 1];
			region[j + 2][1] = a[i] + b[i + 1];
			region[j + 3][0] = b[i] + a[i + 1];
			region[j + 3][1] = b[i] + b[i + 1];
		}
	} else {

	merge_intervals(intv, n);
	total = calc_intervals(intv, n);
}

#endif

/* Delete a minimal number to keep a sequence order */
int deleteOrder(void)
{
	int n;
	if (scanf("%d\n", &n) != 1)
		return -1;

	if (n == 2) {
		printf("-1");
		return 0;
	}

	int a[n];
	int flag[n];
	int inc_num = 0;
	int dec_num = 0;
	int sum = 0;
	int ret = -1;

	int i;
	for (i = 0; i < n; i++) {
		int v;
		if (scanf("%d", &v) != 1)
			return -1;

		a[i] = v;

		if (i >= 1) {
			flag[i] = 0;
			if (a[i] > a[i - 1]) {
				flag[i] = 1; //inc
				inc_num++;
			} else if (a[i] < a[i -1]) {
				flag[i] = -1; //dec
				dec_num++;
			} else if (a[i] == a[i -1]) {
				flag[i] = 2;
			}
		}
	}

	if (inc_num >= dec_num) {
		for (i = 1; i < n; i++) {
			if (flag[i] == 2)
				flag[i] = 1;
			sum = sum + flag[i];
		}
	} else {
		for (i = 1; i < n; i++) {
			if (flag[i] == 2)
				flag[i] = -1;
			sum = sum + flag[i];
		}
	}

	if (n == 3) {
		int tmp = 0;
		tmp = (a[0] <= a[1]) ? a[0] : a[1];
		ret = tmp <= a[2] ? tmp : a[2];
	}

	if (sum == (n - 3)) {
		for (i = 1; i < n; i++) {
			if (flag[i] == -1) {
				if (i == 1)
					ret =  (a[i - 1] <= a[i + 1]) ? a[i] : a[i - 1];
				else if (i == (n - 1))
					ret = a[i];
				else {
					if (a[i - 1] <= a[i + 1])
						ret = a[i];
					else if (a[i] >= a[i - 2])
						ret = a[i - 1];
					else
						ret = -1;
				}
			}
		}
	} else if (sum == (3 - n)) {
		for (i = 1; i < n; i++) {
			if (flag[i] == 1) {
				if (i == 1)
					ret = a[i - 1];
				else if (i == (n - 1))
					ret = a[i] <= a[i - 2] ? a[i - 1] : a[i];
				else {
					if (a[i] <= a[i - 2])
						ret = a[i - 1];
					else if (a[i - 1] >= a[i + 1])
						ret = a[i];
					else
						ret = -1;
				}
			}
		}
	}

	printf("%d\n", ret);
	return 0;
}

void floyd(int *a, int n)
{
	int d[n][n];

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			d[i][j] = *(a + i * n + j);
		}
	}

	for (int tmp = 0; tmp < n; tmp++) {
		for (int i = 0; i < n; i++) {
			for (int j = 0; j < n; j++) {
				if ((d[i][tmp] != -1) && (d[tmp][j]!= -1)) {
					if (d[i][j] == -1) {
						d[i][j] = d[i][tmp] + d[tmp][j];
					} else if (d[i][j] > (d[i][tmp] + d[tmp][j])) {
						d[i][j] = d[i][tmp] + d[tmp][j];
					}
				}
				*(a + i * n + j) = d[i][j];
			}
		}
	}
}

void update_floyd(int *p, int n, int a[4])
{
	int d[n][n];

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			d[i][j] = *(p + i * n + j);
		}
	}

	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			d[i][j] = min(d[i][j],
				      min(d[i][a[1]] + a[3] + d[a[2]][j],
					  d[i][a[2]] + a[3] + d[a[1]][j]));
			*(p + i * n + j) = d[i][j];
		}
	}
}

int scenceDistance()
{
	int n;
	scanf("%d", &n);

	int d[n][n];
	int *p;

	int v;
	for (int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &v);
			d[i][j] = v;
		}
	}

	p = &d[0][0];
	floyd(p, n);

	int t; // t is P
	scanf("%d", &t);

	int a[t][4];
	for (int i = 0; i < t; i++) {
		scanf("%d", &v);
		a[i][0] = v;
		if (a[i][0] == 1) {
			scanf("%d", &v);
			a[i][1] = v - 1;
			scanf("%d", &v);
			a[i][2] = v - 1;
		} else {
			scanf("%d", &v);
			a[i][1] = v - 1;
			scanf("%d", &v);
			a[i][2] = v - 1;
			scanf("%d", &v);
			a[i][3] = v;
		}
	}

	int ret;
	for (int k = 0; k < t; k++) {
		if (a[k][0] == 1) {
			ret = d[a[k][1]][a[k][2]];
			printf("%d\n", ret);
		} else {
			update_floyd(p, n, a[k]);
		}
	}

	return 0;
}

bool hasTime(int *a, int n, int cur[2], int door[2], int timeLimit)
{
	if (timeLimit <= 0 && (*(a + cur[0] * n + cur[1]) != 4))
		return false;

	int dis;
	dis = abs(cur[0] - door[0]) + abs(cur[1] - door[1]);
	if (dis > timeLimit)
		return false;

	return true;
}

bool canPass(int *a, int n, int cur[2], int next[2], int pre[2])
{
	if (cur[0] < 0 || cur[0] >= n || cur[1] < 0 || cur[1] >= n)
		return false;

	if (*(a + cur[0] * n + cur[1]) == 1)
		return false;

	if (next[0] == pre[0] && next[1] == pre[1])
		return false;

	return true;
}

bool isBlocked(int *v, int n, int pos[2], int door[2])
{
	int start = min(pos[0], door[0]);
	int end = max(pos[0], door[0]);
	int sum;
	if (end - start > 2) {
		for (int i = start; i < end; i++) {
			sum = 0;
			for (int j = 0; j < n; j++)
				sum = sum + *(v + n * i + j);
			if (sum == 0) {
				printf("%d\n", -1);
				return true;
			}
		}
	}
	start = min(pos[1], door[1]);
	end = max(pos[1], door[1]);
	if (end - start > 2) {
		for (int j = start; j < end; j++) {
			sum = 0;
			for (int i = 0; i < n; i++)
				sum = sum + *(v + n * i + j);
			if (sum == 0) {
				printf("%d\n", -1);
				return true;
			}
		}
	}

	return false;
}
void dfs_light(int *a, int n, int i, int j, int timeLimit, int pre[2],
	       int door[2], int *maxLight, int *Lightnum, int *v)
{
	int cur[2] = {i, j};
	int next[2];

	if (*(a + n * i + j) == 4) {
		*maxLight = max(*maxLight, *Lightnum);
		return;
	}

	*(v + n * i + j) += 1;

	if (*(a + n * i + j) == 2)
		(*Lightnum)++;

	int dir[4][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};

	for (int k = 0; k < 4; k++) {
		next[0] = cur[0] + dir[k][0];
		next[1] = cur[1] + dir[k][1];
		if (hasTime(a, n, cur, door, timeLimit) && canPass(a, n, cur, next, pre))
			dfs_light(a, n, next[0], next[1], timeLimit - 1, cur,
				  door, maxLight, Lightnum, v);
	}

	if (*(a + n * i + j) == 2)
		(*Lightnum)--;
}

int closeLight()
{
	int n;
	scanf("%d", &n);

	int node[n][n];
	int vs[n][n]; //visit
	int *a, *p;
	int pos[2];
	int door[2];
	int maxLight = 0;
	int Lightnum = 0;

	int timeLimit;
	scanf("%d", &timeLimit);

	int v;
	for(int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++) {
			scanf("%d", &v);
			node[i][j] = v;
			vs[i][j] = 0;
			if (v == 3) {
				pos[0] = i;
				pos[1] = j;
			}
			if (v == 4) {
				door[0] = i;
				door[1] = j;
			}
		}
	}

	a = &node[0][0];
	p = &vs[0][0];

	dfs_light(a, n, pos[0], pos[1], timeLimit, pos, door, &maxLight, &Lightnum, p);

	// for debug
	for(int i = 0; i < n; i++) {
		for (int j = 0; j < n; j++)
			printf("%d ", vs[i][j]);

		printf("\n");
	}

	if (isBlocked(p, n, pos, door))
		goto out;

	printf("%d\n", maxLight);
out:
	return 0;
}
