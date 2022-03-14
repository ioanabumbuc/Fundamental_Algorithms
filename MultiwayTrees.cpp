/*
	MULTIWAY TREES REPRESENTATION

input: parent array
transformation T1: from parent array to multiway tree
transformation T2: from multiway tree to binary tree representation of multiway tree

Data structures for R2:
nodeM - key
	  - number of children
	  - array of pointers to children nodes
array "nodes" - array that stores pointers to every node in the structure

idea for T1:
- create nodes and store their address in array
- go through the parent array and at each element connect the parent to the child, using the address stored in nodes[]
- each time a parent is connected to its child, increment number of children in the structure

Data structure for R3:
nodeB - key
	  - child (corresponds to left of BT) - first child in MW
	  - sibling (right of BT) - first sibling to the right

idea for T2 - recursive:
- at each node - add children recursively
- come back and add siblings
- remember position of children in the array of children to know where to continue adding
- when calling the function give as parameter the parent of the node to be able to find its sibling

Complexity - both transformations run in linear time
for T1 - there are two for loops one after the other, visiting all the elements of the parent array twice
	- once to create the nodes and store them in an array of nodes
	- second time to perform operations to link the nodes together
	=> 2n => O(n) time complexity
	- alg uses additional memory - array of n elements
	=> O(n) memory

for T2 - apply master theorem on recursive function
	a = 2  (2 recursive calls)
	b = 2  (each recursive call is performed on the left or right of each node -> divides the data in 2)
	c = 0  (operations outside recursive calls are performed in constant time)
	=> O(n)
*/

#include <stdio.h>
#include <stdlib.h>
#include <iostream>

using namespace std;

#define nrChildren 3
#define maxim 20

typedef struct nodeM {
	int key;
	//int nbChld;
	struct nodeM* child;
	struct nodeM* sibling;
};

typedef struct nodeB {
	int key;
	struct nodeB* child;
	struct nodeB* sibling;
};

nodeM* createNode(int key)
{
	nodeM* newNode = (nodeM*)malloc(sizeof(nodeM));
	newNode->key = key;
	//newNode->nbChld = 0;
	newNode->child = NULL;
	return newNode;
}

nodeB* createNodeB(int key)
{
	nodeB* newNode = (nodeB*)malloc(sizeof(nodeB));
	newNode->key = key;
	newNode->child = NULL;
	newNode->sibling = NULL;
	return newNode;
}

nodeM* nodes[maxim];

int transformT1(int* parent, int size)
{
	int root = -1;
	for (int i = 0; i < size; i++)
	{
		nodes[i] = createNode(i + 1);
	}
	for (int i = 0; i < size; i++)
	{
		if (parent[i] == -1) //found root
		{
			root = i;
		}
		else
		{
			/*
			int nodeParent = parent[i] - 1;
			int nbChildren = nodes[nodeParent]->nbChld;
			nodes[nodeParent]->children[nbChildren] = nodes[i];

			nodes[nodeParent]->child 

			(nodes[nodeParent]->nbChld)++;
			*/
			int nodeParent = parent[i] - 1;
			if (nodes[nodeParent]->child != NULL)
			{
				nodeM* ptr = nodes[nodeParent]->child;
				nodes[nodeParent]->child = nodes[i];
				nodes[i]->child = 
			}
			
		}
	}
	return root;
}

nodeB* transformT2(nodeM* rootM, nodeM* parent, int childPos)
{
	if (rootM == NULL)
		return NULL;
	nodeB* rootB = createNodeB(rootM->key);
	if (rootM->nbChld != 0 && rootM->children[0] != NULL)
	{
		rootB->child = transformT2(rootM->children[0], rootM, 0);
	}
	if (parent != NULL && childPos + 1 < parent->nbChld && parent->children[childPos] == rootM)
	{
		rootB->sibling = transformT2(parent->children[childPos + 1], parent, childPos + 1);
	}
	return rootB;

}


void prettyPrintR2(nodeM* root, int nbSpaces)
{
	if (root == NULL)
		return;
	for (int i = 0; i < nbSpaces; i++)
		cout << " ";
	cout << root->key << endl;
	for (int i = 0; i < nrChildren && i < root->nbChld && root->children[i] != NULL; i++)
	{
		prettyPrintR2(root->children[i], nbSpaces + 5);
	}
}

void printR2(nodeM* root)
{
	if (root == NULL)
		return;
	cout << root->key << " ";
	for (int i = 0; i < root->nbChld; i++)
	{
		printR2(root->children[i]);
	}
}

void prettyPrintR3(nodeB* root, int nbSpaces)
{
	if (root == NULL)
		return;
	for (int i = 0; i < nbSpaces; i++)
		cout << " ";
	cout << root->key << endl;
	prettyPrintR3(root->child, nbSpaces + 5);
	prettyPrintR3(root->sibling, nbSpaces);

}

void printArr(int* arr, int n)
{
	for (int i = 0; i < n; i++)
		cout << arr[i] << " ";
	cout << endl;
}

void demo()
{
	int p[] = { 2,7,5,2,7,7,-1,5,2 };
	int size = sizeof(p) / sizeof(p[0]);
	cout << "Parent array: " << endl;
	printArr(p, size);
	int r = transformT1(p, size);

	nodeM* rootM = nodes[r];
	cout << endl << "Multiway representation (preorder): " << endl;
	//prettyPrintR2(rootM, 0);
	printR2(rootM); cout << endl;
	nodeB* rootB = transformT2(rootM, NULL, 0);
	cout << endl << "Binary representation: " << endl;
	prettyPrintR3(rootB, 0);

}

int main()
{
	demo();

	return 0;
}
