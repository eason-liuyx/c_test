struct treenode {
	int val;
	struct treenode *left;
	struct treenode *right;
};

/* single node, for stack operation */
struct singlenode {
	struct treenode* data;
	struct singlenode* next;
};

struct stack {
	struct singlenode* bottom;
	struct singlenode* top;
	int size;
};

int** pathSum(struct treenode* root, int sum, int* returnsize,
	      int** returncolumnsizes);
bool canPartitionKSubsets(int* nums, int numssize, int k);
double* calcEquation(char*** equations, int equationsSize, int* eqationsColSize,
		     double* values, int valuesSize, char*** queries,
		     int queriesSize, int* queriesColSize, int* returnSize);
char * longestPalindrome(char * s);
int strlabel();
int** merge(int** intervals, int intervalsSize, int* intervalsColSize,
	    int* returnSize, int** returnColumnSizes);
