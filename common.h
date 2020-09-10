#define MAX_WD_NUM	64
#define MAX_WD_LEN	16
#define MAX_STR_LEN	512

#define UP_ALPHABET_LEN 26

#define TREE_NODE_MAX_NUM 1024
#define TREE_NODE_MAX_VALUE 0xF

#define max(a, b)  (a) > (b) ? (a) : (b)
#define min(a, b)  (a) < (b) ? (a) : (b)

struct TreeNode {
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
};

/* single node, for stack operation */
struct singlenode {
	struct TreeNode* data;
	struct singlenode* next;
};

struct stack {
	struct singlenode* bottom;
	struct singlenode* top;
	int size;
};

struct queue {
	struct singlenode* head;
	struct singlenode* tail;
	int size;
};

void nextPermutation(int* nums, int numsSize);
int removeElement(int* nums, int numSize, int val);
int findAllgenes(int len, char* GeneSeq);
int** kClosest(int** points, int pointsSize, int* pointColSize, int K,
	       int* returnSize, int** returnColumnSizes);
int** kClosest_DC(int** points, int pointsSize, int* pointColSize, int K,
		  int* returnSize, int** returnColumnSizes);
int mincostTickets(int* days, int daysSize, int* costs, int costsSize);
char* pushDominoes(char* dominoes);
int leastInterval(char* tasks, int tasksSize, int n);
void quick_sort(int* arr, int left, int right);
void pretraversal(struct TreeNode* root, int* depth);
void intraversal(struct TreeNode* root, int* depth);
void posttraversal(struct TreeNode* root, int* depth);
void bfs(struct TreeNode* root, int* depth);
void dfs(struct TreeNode* root, int* depth);
struct TreeNode* create_tree(struct TreeNode* root);
int** pathSum(struct TreeNode* root, int sum, int* returnsize,
	      int** returncolumnsizes);
bool canPartitionKSubsets(int* nums, int numssize, int k);
double* calcEquation(char*** equations, int equationsSize, int* eqationsColSize,
		     double* values, int valuesSize, char*** queries,
		     int queriesSize, int* queriesColSize, int* returnSize);
char * longestPalindrome(char * s);
int strlabel();
int** merge(int** intervals, int intervalsSize, int* intervalsColSize,
	    int* returnSize, int** returnColumnSizes);
int deleteOrder(void);
int scenceDistance();
int closeLight();
int carryNum();
void calcFobric();
int mergeChar();
void collectCoins ();
void fillNum();
void fillPacket();
void cityRoad();
int jumpGame();
void dig_gold();
int strangeGame();
int strangeGame2();
