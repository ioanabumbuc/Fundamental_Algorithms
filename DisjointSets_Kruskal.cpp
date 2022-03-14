/*
Assignment 8 - DISJOINT SETS & KRUSKAL ALGORITHM

Disjoint sets operations:
- make_set(x) - creates a set with element x
- unify(x,y)  - makes the union of the sets that cointain x and y
			  - unifies based on rank: adds the set with smaller rank to the one with bigger rank; 
				if ranks are equal, increment the rank by 1
- find_Set(x) - returns the representative of the set containing element x

Kruskal algorithm
- generates the minimum spanning tree from a graph
- input: a connected graph with n vertices and n*4 edges, given as adjacency list
	V = n; E = 4*n;
- output: MST
- idea: 
	- generate an array of edges from the adjacency list and sort it using heap - O(ElogE)
	- create sets of all nodes - O(V)
	- for each edge (in increasing order) - O(E) 
		- check if the nodes belong to different sets
		if yes, add the edge to the resulting tree and unify the sets of that nodes
- complexity: O(ElogE)
*/


#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#include "Profiler.h"

Profiler p("DisjointSetsKruskal");

using namespace std;

#define maxim 10001
#define maxWeight 100
#define nrTests 5

int opMS, opFS, opU;

typedef struct set {
	int data;
	int rank;
	struct set* parent;
}node;

typedef struct edge {
	int weight;
	node* u;
	node* v;
}edge;

typedef struct adjList {
	int key;
	int weight;
	struct adjList* next;
}adjList;

adjList* G[maxim];
edge* T[4 * maxim];
edge* edges[4 * maxim];
node* set[maxim];

void addNodeFirst(int index, int key, int weight)
{
	adjList* node = (adjList*)malloc(sizeof(adjList));
	node->key = key;
	node->weight = weight;
	if (G[index] == NULL)
	{
		G[index] = (adjList*)malloc(sizeof(adjList));
		G[index]->key = index;
		G[index]->next = NULL;
		G[index]->weight = 0;
	}
	if (G[index]->next == NULL)
	{
		node->next = NULL;
		G[index]->next = node;
	}
	else
	{
		adjList* aux = G[index]->next;
		node->next = aux;
		G[index]->next = node;
	}
}

node* makeSet(int key)
{
	node* ptr = (node*)malloc(sizeof(node));
	ptr->data = key;
	ptr->parent = ptr;
	ptr->rank = 0;
	opMS += 3;
	return ptr;
}

node* findSet(node* root)
{
	opFS++;
	if (root->parent != root)
	{
		opFS++;
		root->parent = findSet(root->parent);
	}
	return root->parent;
}

void link(node* x, node* y)
{
	opU++;
	if (x->rank > y->rank)
	{
		y->parent = x;
		opU++;
	}
	else
	{
		x->parent = y;
		opU += 2;
		if (x->rank == y->rank)
		{
			y->rank++;
			opU++;
		}
	}
}

void unify(node* x, node* y)
{
	opU++;
	link(findSet(x), findSet(y));
}

/*
void swapEdges(edge* a, edge* b)
{
	edge* temp = a;
	b = a;
	a = temp;
}
*/

void heapify(int i, int n)
{
	int largest = i;
	int j = 2 * i + 1, k = 2 * i + 2; //j left child, k right child
	if (j < n && edges[i]->weight < edges[j]->weight)
	{
		largest = j;
	}
	if (k < n && edges[k]->weight > edges[largest]->weight)
	{
		largest = k;
	}
	if (largest != i && largest < n)
	{
		swap(edges[i], edges[largest]);
		heapify(largest, n);
	}
}

void buildHeapBU(int n)
{
	for (int i = n / 2 - 1; i >= 0; i--)
	{
		heapify(i, n);
	}
}

void heapSortWeights(int n)
{
	buildHeapBU(n);
	int size = n;
	for (int i = n - 1; i >= 1; i--)
	{
		swap(edges[0], edges[i]);
		size--;
		heapify(0, size);
	}
}

void initializeSets(int n)
{
	for (int i = 0; i < n; i++)
	{
		set[i] = makeSet(-1);
		opMS -= 3;
	}
}

void makeEdgesArr(int n)
{
	int i, edgect = 0, key, w;
	initializeSets(n);
	for (i = 0; i < n; i++)
	{
		adjList* ptr = G[i];
		key = ptr->key;
		if (set[key]->data == -1) //node doesn't already exist
		{
			set[key] = makeSet(key);
		}
		node* u = set[key];
		while (ptr->next != NULL && edgect < 4*n)
		{
			ptr = ptr->next;
			w = ptr->weight;
			key = ptr->key;
			if (key > i) //to not have duplicates - only count an edge the first time it appears in the adjacency list
			{
				if (set[key]->data == -1)
				{
					set[key] = makeSet(key);
				}
				node* v = set[key];
				edges[edgect] = (edge*)malloc(sizeof(edge));
				edges[edgect]->u = u;
				edges[edgect]->v = v;
				edges[edgect]->weight = w;
				edgect++;
			}
		}
	}
}

void printHeapSort(int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << edges[i]->u->data << " " << edges[i]->v->data << " " << edges[i]->weight << endl;
	}
}

void printEdgesArr(int n)
{
	for (int i = 0; i < 4 * n; i++)
	{
		cout << edges[i]->u->data << " " << edges[i]->v->data << " w " << edges[i]->weight << endl;
	}
}

void printAdjList(int n)
{
	for (int i = 0; i < n; i++)
	{
		adjList* ptr = G[i];
		cout << ptr->key << " ";
		ptr = ptr->next;
		while (ptr != NULL)
		{
			cout << ptr->key << " ";
			ptr = ptr->next;
		}
		cout << endl;
	}
}

void printResult(int size)
{
	int minCost = 0;
	for (int i = 0; i < size; i++)
	{
		cout <<T[i]->u->data << " " << T[i]->v->data << " w " << T[i]->weight << endl;
		minCost += T[i]->weight;
	}
	cout <<endl<< "Cost of MST= " << minCost<<endl<<endl;
}

int kruskal(int n)
{
	int size = 0;
	makeEdgesArr(n);
	heapSortWeights(n*4);
	for (int i = 0; i < n*4; i++)
	{
		node* u = edges[i]->u;
		node* v = edges[i]->v;
		if (findSet(u) != findSet(v))
		{
			T[size] = edges[i];
			unify(u, v);
			size++;
		}
	}
	return size;
}


void generateRandomGraph(int n)
{
	int a[maxim], random;
	FillRandomArray(a, n, 0, n - 1, 1);

	G[0] = (adjList*)malloc(sizeof(adjList));
	G[0]->key = 0;
	G[0]->weight = 0;
	G[0]->next = NULL;
	G[n - 1] = (adjList*)malloc(sizeof(adjList));
	G[n - 1]->key = n - 1;
	G[n - 1]->weight = 0;
	G[n - 1]->next = NULL;

	int random2 = rand() % maxWeight;
	addNodeFirst(0, n - 1, random2);

	for (int i = 0; i < n - 1; i++)
	{
		G[i + 1] = (adjList*)malloc(sizeof(adjList));
		G[i + 1]->key = i + 1;
		G[i + 1]->weight = 0;
		G[i + 1]->next = NULL;

		random = rand() % maxWeight;
		addNodeFirst(i, i + 1, random);
		addNodeFirst(i + 1, i, random);
	}
	addNodeFirst(n - 1, 0, random2);
	for (int i = 0; i < 4; i++)
	{
		adjList* ptr = G[i];
		for (int j = i + 2; j < n; j++)
		{
			if (!(i == 0 && j == n - 1))
			{
				random = rand() % maxWeight;
				int u = a[i], v = a[j];
				if (u != v - 1 && v != u - 1)
				{
					addNodeFirst(u, v, random);
					addNodeFirst(v, u, random);
				}
				else
				{
					do {
						u++;
					} while (u == v - 1 || v == u - 1);
					addNodeFirst(u, v, random);
					addNodeFirst(v, u, random);
				}
			}
		}
	}
}

void demoSets()
{
	node* set[20];
	for (int i = 0; i < 10; i++)
	{
		set[i] = makeSet(i);
	}
	cout << "initial sets: " << endl;
	for (int i = 0; i < 10; i++)
	{
		cout << set[i]->data << " ";
	}
	cout << endl << "unify sets 3 with 1; 6 with 2; 4 with 5 with 9";
	unify(set[3], set[1]);
	unify(set[6], set[2]);
	unify(set[5], set[4]);
	unify(set[4], set[9]);
	cout << endl << "new sets: " << endl;
	int newline = 0;
	for (int i = 0; i < 10; i++)
	{
		newline = 0;
		for (int j = 0; j < 10; j++)
		{
			if (findSet(set[j])->data == set[i]->data)
			{
				cout << set[j]->data << " ";
				newline = 1;
			}
		}
		if (newline)
			cout << endl;
	}
	cout << endl << "find representative for element 9: ";
	cout << findSet(set[9])->data << endl << endl;
}

void demoKruskal()
{
	int n = 5;
	for (int i = 0; i < 5; i++)
	{
		G[i] = (adjList*)malloc(sizeof(adjList));
		G[i]->key = i;
		G[i]->weight = 0;
		G[i]->next = NULL;
	}
	addNodeFirst(0, 1, 1);
	addNodeFirst(0, 2, 2);
	addNodeFirst(0, 4, 5);
	addNodeFirst(0, 3, 4);
	addNodeFirst(1, 0, 1);
	addNodeFirst(1, 2, 4);
	addNodeFirst(2, 0, 2);
	addNodeFirst(2, 1, 4);
	addNodeFirst(2, 3, 2);
	addNodeFirst(2, 4, 3);
	addNodeFirst(3, 0, 4);
	addNodeFirst(3, 2, 2);
	addNodeFirst(3, 4, 6);
	addNodeFirst(4, 0, 5);
	addNodeFirst(4, 2, 3);
	addNodeFirst(4, 3, 6);

	int size = 0;
	makeEdgesArr(n);
	cout << "edges: " << endl;
	printEdgesArr(2);
	heapSortWeights(8);
	for (int i = 0; i < 8; i++)
	{
		node* u = edges[i]->u;
		node* v = edges[i]->v;
		if (findSet(u) != findSet(v))
		{
			T[size] = edges[i];
			unify(u, v);
			size++;
		}
	}

	int minCost = 0;
	cout << endl << "Kruskal MST: " << endl;
	printResult(size);
}

void demoRandomGraph()
{
	int n = 100;
	generateRandomGraph(n);
	int size = kruskal(n);
	printResult(size);
}

void demo()
{
	cout << "Set operations: " << endl;
	demoSets();
	cout << "Kruskal demo: "<<endl<<endl;
	demoKruskal();
}

void evaluation()
{
	int n;
	for (int i = 0; i < nrTests; i++)
	{
		for (n = 100; n < maxim; n += 100)
		{
			opU = 0; opMS = 0; opFS = 0;
			generateRandomGraph(n);
			kruskal(n);
			p.countOperation("MakeSet_Operations", n, opMS);
			p.countOperation("Unify_Operations", n, opU);
			p.countOperation("FindSet_Operations", n, opFS);
		}
	}
	p.divideValues("MakeSet_Operations", nrTests);
	p.divideValues("FindSet_Operations", nrTests);
	p.divideValues("Unify_Operations", nrTests);
	p.createGroup("Set_Operations", "MakeSet_Operations", "FindSet_Operations", "Unify_Operations");

	p.showReport();
}

int main()
{
	demo();
	//evaluation();
}