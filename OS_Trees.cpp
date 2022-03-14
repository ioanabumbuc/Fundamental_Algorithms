/*
	ORDER STATISTIC TREE OPERATIONS - BuildTree, OS_Select, OS_Delete
	
Input: ordered array with numbers from 1 to n
Data structure used: node having key, left ptr, right ptr, parent ptr, size field (with nb of children)

BuildTree(array, i, j)
- builds the tree recursively, using a divide and conquer approach
- at each call of the recursive function, creates a node with data from the middle of the arrray
- call function from index i to middle-1
- call function from index middle+1 to j
- when building the tree - set sizes of leaves to 1 and for every non-leaf as the sum of size of left subtree +
	size of right subtree + 1 (for the node itself)

OS_Select(root, i, fixSize)
- returns ith smalles element from the tree, using the information from rank
- if the node has no left children => rank = 1
	otherwise, rank = size of left child + 1 for the node itself
- if rank = i => ith smallest element is found => return it
- if rank > i then continue looking for i on left branch
- if rank < i then look for i-rank on right branch
- fixSize - set to 0 means I dont want to delete and the sizes should remain unchanged
- fixSize = 1 means I want to delete that node so at each calling of the function - decr size
- that way when deleting, the sized don't need to be fixed

OS_Delete(root, ptrToDel)
- deletes the node stored at ptrToDel
- 3 cases covered:
	- when node is leaf -> set the parent's pointer to that node to NULL
	- when node has only one child -> replace that node with its child
	- when node has two children -> replace node with predecessor

Complexity:
BuildTree - O(n)
	apply master theorem: a = 2; b = 2; c = 0 => O(n)
OS_Select - O(log n)
	based on the rank, the algorithm chooses one branch to go at each step
	height of binary tree - log n, considering that the built tree is balanced
OS_Delete - O(log n)
	finding predecessor takes O(log n)
	takes O(1) in the other 2 cases

Perform OS_Select and OS_Delete n times => O(n log n)
From report: BuildTree takes the least time and OS_Select takes most time
*/


#pragma warning(disable:4996)

#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif


#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

using namespace std;

#include "Profiler.h"
Profiler p("OS Trees");

int opB, opS, opD;

#define nrTests 5
#define maxim 10001

typedef struct node {
	int key;
	int sizeChld;
	struct node* left;
	struct node* right;
	struct node* parent;
}nodeB;

node* createNode(int key)
{
	node* newNode = (node*)malloc(sizeof(node));
	opB += 5;
	newNode->key = key;
	newNode->sizeChld = 0;
	newNode->left = NULL;
	newNode->right = NULL;
	newNode->parent = NULL;
	return newNode;
}

node* buildTree(int* a, int i, int j)
{
	if (i > j)
		return NULL;
	int m = (i + j) / 2;
	node* root = createNode(a[m]);
	root->left = buildTree(a, i, m - 1);
	root->right = buildTree(a, m + 1, j);
	if (i == j)
	{
		root->sizeChld = 1;
		opB++;
	}
	else if (root->right == NULL)
	{
		root->left->parent = root;
		root->sizeChld = root->left->sizeChld + 1;
		opB += 2;
	}
	else if (root->left == NULL)
	{
		root->right->parent = root;
		root->sizeChld = root->right->sizeChld + 1;
		opB += 2;
	}
	else
	{
		root->left->parent = root;
		root->right->parent = root;
		root->sizeChld = root->left->sizeChld + root->right->sizeChld + 1;
		opB += 3;
	}
	opB += 2;
	return root;
}

node* OS_Select(node* root, int i, int fixSize)
{
	if (root == NULL)
	{
		return NULL;
	}
	int rank = 1;
	if (root->left != NULL)
	{
		rank = root->left->sizeChld + 1;
		opS++;
	}
	if (fixSize == 1)
	{
		(root->sizeChld)--;
		opS++;
	}
	opS++;
	if (i == rank)
		return root;
	else
	{
		if (i < rank && root->left != NULL)
		{
			opS++;
			return OS_Select(root->left, i, fixSize);
		}
		else
		{
			opS++;
			return OS_Select(root->right, i - rank, fixSize);
		}
	}
}

node* predecessor(node* root)
{
	if (root->left == NULL)
		return NULL;
	node* pred = root->left;
	opD++;
	while (pred->right != NULL)
	{
		pred->sizeChld--;
		pred = pred->right;
		opD++;
	}
	return pred;
}

node* transplant(node* root, node* ptr1, node* ptr2) // ptr2 takes place of ptr1
{
	if (ptr1->parent == NULL)
	{
		opD++;
		return ptr2; //root becomes ptr2
	}
	else if (ptr1->parent->left == ptr1) //ptr1 is left child of its parent
	{
		opD++;
		ptr1->parent->left = ptr2; //ptr2 becomes the left child of ptr1's parent
	}
	else
	{
		opD++;
		ptr1->parent->right = ptr2;
	}
	if (ptr2 != NULL)
	{
		opD++;
		ptr2->parent = ptr1->parent;
	}
	opD += 2;
	return root;
}

node* OS_Delete(node* ptrToDel, node* root)
{
	if (ptrToDel->left == NULL && ptrToDel->right == NULL) //case of leaf
	{
		if (ptrToDel->parent == NULL)
		{
			opD++;
			return NULL;
		}
		if (ptrToDel->parent->left == ptrToDel)
		{
			opD++;
			ptrToDel->parent->left = NULL;
		}
		else
		{
			opD++;
			ptrToDel->parent->right = NULL;
		}
		opD += 3;
		root->parent = NULL;
		return root;
	}
	else if (ptrToDel->left == NULL)
	{
		root = transplant(root, ptrToDel, ptrToDel->right);
	}
	else if (ptrToDel->right == NULL)
	{
		root = transplant(root, ptrToDel, ptrToDel->left);
	}
	else
	{
		node* pred = predecessor(ptrToDel);
		opD++;
		if (pred->parent != ptrToDel)
		{
			root = transplant(root, pred, pred->left);
			pred->left = ptrToDel->left;
			pred->left->parent = pred;
			opD += 2;
		}
		root = transplant(root, ptrToDel, pred);
		pred->right = ptrToDel->right;
		pred->right->parent = pred;
		int aux = ptrToDel->sizeChld;
		ptrToDel->sizeChld = pred->sizeChld;
		pred->sizeChld = aux;
		opD += 5;
	}
	opD += 4;
	root->parent = NULL;
	return root;
}


void preorder(node* root, FILE* pf)
{
	if (root == NULL)
		return;
	fprintf(pf, "%d ", root->key);
	preorder(root->left, pf);
	preorder(root->right, pf);
}

void prettyPrint(node* root, int spaces, FILE* pf)
{
	if (root == NULL)
		return;
	for (int i = 0; i < spaces; i++)
		fprintf(pf, " ");
	fprintf(pf, "%d \n", root->key);
	prettyPrint(root->left, spaces + 5, pf);
	prettyPrint(root->right, spaces + 5, pf);
}

void demo()
{
	FILE* pf = fopen("demo.txt", "w");
	ofstream fout("demo.txt");

	int a[] = { 1,2,3,4,5,6,7,8,9,10,11 };
	int n = sizeof(a) / sizeof(a[0]);
	//BuildTree
	node* root = buildTree(a, 0, n - 1);
	fprintf(pf, "Preorder of built treee\n");
	preorder(root, pf);
	fprintf(pf, "\nPretty print\n");
	prettyPrint(root, 0, pf);

	//OS_Select:
	fprintf(pf, "\n2nd smallest element: ");
	node* ptr = OS_Select(root, 2, 0);
	fprintf(pf, "%d", ptr->key);
	//OS_Delete
	fprintf(pf, "\n\nDelete leaf with key 2:\n");
	root = OS_Delete(OS_Select(root, 2, 1), root);
	prettyPrint(root, 0, pf); preorder(root, pf);
	fprintf(pf, "\n\nDelete 3rd smallest node (key 4) having one child\n");
	root = OS_Delete(OS_Select(root, 3, 1), root);
	prettyPrint(root, 0, pf); preorder(root, pf);
	fprintf(pf, "\n\nDelete 7th smallest node (key 9) with 2 children \n");
	root = OS_Delete(OS_Select(root, 7, 1), root);
	prettyPrint(root, 0, pf); preorder(root, pf);
	fprintf(pf, "\n\nDelete root (key 6) with 2 children\n");
	root = OS_Delete(OS_Select(root, 4, 1), root);
	prettyPrint(root, 0, pf);
	preorder(root, pf);
}


void evaluation()
{
	int a[maxim + 2];
	for (int i = 0; i < nrTests; i++)
	{
		for (int n = 101; n <= maxim; n += 100)
		{
			opS = 0; opD = 0; opB = 0;
			for (int j = 0; j < n; j++)
				a[j] = j;
			node* root = buildTree(a, 1, n - 1);
			root->parent = NULL;
			int size = n - 1; //100
			while (size > 1)
			{
				int x = rand() % size + 1;
				root = OS_Delete(root, OS_Select(root, x,1));
				size--;
			}
			p.countOperation("OS_Select", n, opS);
			p.countOperation("OS_Delete", n, opD);
			p.countOperation("BuildTree", n, opB);
			//p.divideValues("OS_Select", n);
			//p.divideValues("OS_Delete", n);
			//p.divideValues("BuildTree", n);
		}
	}
	p.divideValues("OS_Select", nrTests);
	p.divideValues("OS_Delete", nrTests);
	p.divideValues("BuildTree", nrTests);
	p.createGroup("OS_Operations", "BuildTree", "OS_Select", "OS_Delete");
	p.showReport();
}

int main()
{
	//demo();
	evaluation();
	return 0;
}