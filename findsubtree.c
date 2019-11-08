#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct TreeNode {
	int val;
	struct TreeNode *left;
	struct TreeNode *right;
};

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


