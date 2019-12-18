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
	int node_num = 0;

	traverse_bitree(root, &node_num);

	int* array_list = calloc(node_num, sizeof(int));

	serial_bitree(root, array_list);
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

int** merge(int** intervals, int intervalsSize, int* intervalsColSize, int* returnSize, int** returnColumnSizes){
	int mergecnt = 0;
	int k = 0;
	int i;
	printf("intervalsSize is %d, intervalcolsize is %d\n", intervalsSize, *intervalsColSize);
	if (intervalsSize == 1) {
		printf("coming here\n");
		int** array = (int **)malloc(sizeof(int *));
		array[0] = malloc(sizeof(int) * 2);
		array[0] = intervals[0];
		return array;
	}

	intervals = sort_interval(intervals, intervalsSize); //排序

	for (i = 0; i < intervalsSize; i++){
		printf("original inter[%d], %d, %d\n", i, intervals[i][0], intervals[i][1]);
	}

	//merge interval
	int curend, nextstart;
	for (i = 0; i < intervalsSize - 1; i++){
		curend = intervals[i][1];
		nextstart = intervals[i + 1][0];
		if (curend >= nextstart){
			intervals[i + 1][0] = intervals[i][0];
			intervals[i + 1][1] = max(intervals[i][1], intervals[i + 1][1]);
			intervals[i][0] = -1; // will be deleted
			mergecnt++;
		}
	}

	for (i = 0; i < intervalsSize; i++){
		printf("after merge inter[%d], %d, %d\n", i, intervals[i][0], intervals[i][1]);
	}

	int retsz = intervalsSize - mergecnt;    // 行数

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
