#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "common.h"

#define _GNU_SOURCE
typedef unsigned long long u64;
typedef unsigned int u32;
typedef unsigned short int u16;
typedef char u8;

typedef unsigned long bitmap_word;

struct data_st {
	int x;
	int y;
	char  w;
	u16 *wrid;
};

union td {
	char str;
	int data;
};

void endian_test () {
	union td db;
	db.data = 0x01020304;
	if (db.str==0x01)
	{
		printf("big endian\n");
	}
	else if (db.str==0x04) {
		printf("little enddian\n");
	}
	else {
		printf("can't know\n");
	}
}

#define roundup(x, y) (                                 \
  {                                                       \
         const typeof(y) __y = y;                        \
          (((x) + (__y - 1)) / __y) * __y;                \
  }                                                       \
 )


union doorbell{
	u64 db64;
	u32 db32[2];
};


/* test the address of a u64 variable is 64bit alignment or not */
void align_addr() {
	u32 doorbell[2];	//u32's addr is 4byte aligned
	doorbell[0] = 0xba98;
	doorbell[1] = 0xfedc;
	printf("*(u64 *)doorbell 0x%llx\n", *(u64 *)doorbell);
	printf("addr of doorbell[0] is 0x%p \n", &(doorbell[0]));
}

void test_types() {

	int a = sizeof(unsigned long);

	printf("unsigned long is %d byte\n", a);
}

void test_struct() {
	struct stdb {
	u32 db1;
	u32 db2;
	} db;
	
	db.db1 = 0xBA98;
	db.db2 = 0xFEDC;

	printf("db. address is 0x%p\n", &db);
	printf("db.db1 address is %p\n", &db.db1);
	printf("db.db2 address is %p\n", &db.db2);
	printf("db.db64 value is 0x%llx\n", *(u64 *)(&db));
}

void test_union() {
	union doorbell {
	u64 db64;
	u32 db32[2];
	} db;
	
	db.db32[0] = 0xBA98;
	db.db32[1] = 0xFEDC;
//	db.db64 = (u64)db.db32[0] << 32 | db.db32[1];

	printf("db.db32 address is %p\n", &db.db32);
	printf("db.db64 address is %p\n", &db.db64);
	printf("db.db32 value is 0x%llx\n", *(u64 *)db.db32);
	printf("db.db64 value is 0x%llx\n", db.db64);
}

#define page_shift 12

void test_owner_bit() {
	u32 head = 29;
	u32 wqe_cnt = 32;
	u32 nreq = 0;
	u16 owner_bit = 0;
	u32 wqe_idx;

	for (nreq = 0; nreq < 128; nreq++) {
		wqe_idx = (head + nreq) & (wqe_cnt - 1);
		owner_bit = ~(((head + nreq) >> 5 ) & 0x1);
		printf("nreq:%d, head:%d, owner1:%d\n", nreq, head, owner_bit & 0x1);
		owner_bit = ~(((wqe_idx) >> 5 ) & 0x1);
		printf("nreq:%d, wqe_idx:%d, owner2:%d\n", nreq, wqe_idx, owner_bit & 0x1);
//		if (owner_bit == 1)
//			printf("nreq:%d, head:%d\n", nreq, head);
	}
}

void test_char() {
	int c;
	int *addr = NULL;
	u64 base = ~0;
	char ch[10] = "HOHOHO";
	struct data_st type_test;
	c = sizeof(*type_test.wrid);
	
	printf("c %d\n", c);
	c = roundup(1, 64);
	printf("c %d\n", c);
	printf("%p\n", addr);
	printf("0x%llx\n", base);

	printf("%s\n", ch);
}

void test_merge(){
	int intervalsSize = 2;
	int intervalsColSize = 2;
 	int returnSize = 0;
	int *returnColumnSizes = NULL;
	int area0[2] = {1, 4};
	int area1[2] = {4, 5};

	int *intervals[intervalsSize];
	intervals[0] = area0;
	intervals[1] = area1;

	merge(intervals, intervalsSize, &intervalsColSize, &returnSize, &returnColumnSizes); 
}

void test_merge_null(){
	int intervalsSize = 1;
	int intervalsColSize = 2;
 	int returnSize = 0;
	int *returnColumnSizes = NULL;
	int area0[2] = {};

	
	int *intervals[intervalsSize];
	intervals[0] = area0;

	merge(intervals, intervalsSize, &intervalsColSize, &returnSize, &returnColumnSizes); 
}

void test_longeststr() {
	//char s[] = "jftaefjfeatsjfia";
	char s[] = "babad";
	char* a;
	a = longestPalindrome(s);
	printf("%s\n", a);
}

void test_division_evaluation(){
	int eqsize = 2;
	int eqcolsize = 2;
	char* eq[][2]= {{"a","b"}, {"b","c"}};
	int valsize = 2;
	double val[] = {2.0, 3.0};
	int querysize = 5;
	int querycolsize = 2;
	char* query[][2] = {{"a","c"},{"b","a"},{"a","e"},{"a","a"},{"x","x"}};
	int returnsize = 0;
	double* returnarray;

//	returnarray = calcEquation(eq, eqsize, &eqcolsize, val, valsize, query,
//				   querysize, &querycolsize, &returnsize);

	for(int i = 0; i < returnsize; i++)
		printf("returnarray: %f", returnarray[i]);

	free(returnarray);
}


#define ARRAY_SIZE 16
void test_partitionK(){

	int array[ARRAY_SIZE];
	int arraysize = 0;
	int k;
	bool ret;

	for (int i = 0; i < ARRAY_SIZE; i++) {
		scanf("%d", array + i);
		arraysize++;
//		printf("%d", array[i]);
		if (getchar() == '\n')
			break;
	}

	scanf("%d", &k);
	ret = canPartitionKSubsets(array, arraysize, k);
	printf("%s", ret ? "true" : "false");
}

void test_create_tree() {
	struct TreeNode* root;

	create_tree(root);
}

void test_pathsum() {
	struct TreeNode* root;
	int sum = 0;
	int depth = 0;
	int returnsize = 0;
	int* retclnsize;
	int** retarray;

	root = create_tree(root);

	printf("pre:");
	pretraversal(root, &depth); printf(" depth: %d", depth);
	printf("\nin:");
	intraversal(root, &depth); printf(" depth: %d", depth);
	printf("\npost:");
	posttraversal(root, &depth); printf(" depth: %d", depth);
	printf("\ndfs:");
	dfs(root, &depth); printf(" depth: %d", depth);
	printf("\nbfs:");
	bfs(root, &depth); printf(" depth: %d", depth);
	printf("\n");

	scanf("%d", &sum);

	retarray = pathSum(root, sum, &returnsize, &retclnsize);

	for (int i = 0; i < returnsize; i++)
		for (int j = 0; j < retclnsize[i]; j++)
			printf("%d", retarray[i][j]);
}

void test_leastinterval() {
	//char* tasks = "AAAABBDDDDDFFDDAA";
	char* tasks = "AAABBB";
	int tasksize = strlen(tasks);
	int duration = leastInterval(tasks, tasksize, 2);
	printf("need time:%d\n", duration);
}

void test_dominoes() {
	char dominoes[] = ".L.R...LR..L..";
	int len = strlen(dominoes);
	char * ret;
	ret = pushDominoes(dominoes);
	for (int i = 0; i < len; i++)
		printf("%c", ret[i]);
}

void test_mincostTickets()
{
	int days[] = {1, 4, 6, 7, 8, 20};
//	int days[] = {1,2,3,4,5,6,7,8,9,10,30,31};
//	int days[] = {1,4,6,9,10,11,12,13,14,15,16,17,18,20,21,22,23,27,28};
	int dsize = sizeof(days) / sizeof(days[0]);
	int costs[] = {2, 7, 15};
//	int costs[] = {3, 13, 45};
	int costsSize = 3;
	int totalcost = 0;

	totalcost = mincostTickets(days, dsize, costs, costsSize);
	printf("%d\n", totalcost);
}

void test_kclosest()
{
	int a[2] = {6, 10};
	int b[2] = {-3, 3};
	int c[2] = {-2, 5};
	int d[2] = {0, 2};
	int *points[4] = {a, b, c, d};
	int pointsSize = 4;
	int pointColSize = 2;
	int K = 3;
	int retsz;
	int* retcolsize = 0;
	int** retarray;
	retarray = kClosest_DC(points, pointsSize, &pointColSize, K, &retsz, &retcolsize);
	for (int i = 0; i < K; i++)
		printf("{%d, %d}\n", retarray[i][0], retarray[i][1]);

}

void test_findallgene()
{
//     char Geneseq[] = "AGCCT";
//       char Geneseq[] = "AGAGAGAG";
//     char Geneseq[] = "AGCTAGCTAGCT";
       char Geneseq[] = "AGCTACGCT";

       int len = 9;
       int ret;

       ret = findAllgenes(len, Geneseq);
       printf("total subgene num: %d\n", ret);
}

void test_removeelement()
{
//	int nums[] = {3, 2, 2, 3};
	int nums[] = {0, 1, 2, 2, 3, 0, 4, 2};
	int numSize = sizeof(nums) / sizeof(nums[0]);
	int val = 2;
	int ret;

	ret = removeElement(nums, numSize, val);
	printf("Array length after delete elements: %d\n", ret);
}

int main() {

	test_removeelement();

	return 0;
}





