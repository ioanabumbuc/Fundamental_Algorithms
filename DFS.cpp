/*
	DEPTH FIRST SEARCH. TOPOLOGICAL SORT. TARJAN'S ALGORITHM

DFS
idea:
	- start with coloring all vertices white, and set all parents to NULL
	- for each white node call DFS_VISIT 
		inside DFS_VISIT at node u:
		- set u->color to gray and set discovery time
		- apply DFS_VISIT for every neighbour of the node u
		- at the end, set u->color to black and set finalizing time
complexity: O(V + E)

Topological Sort
input: graph with V vertexes, E edges
output: a SLL with linear ording of the vertices with the prop that for every edge <u,v>, u is before v in the ordering
constraint: the graph must not have any cycle
idea - modify DFS algorithm
	 - inside DFS_VISIT at node u, after calling the function recursively for every neighbour node
		 - add node u at the front of the SLL

Tarjan Algorithm 
- finds the number of strongly connected components
- also modifies in the node structure the index of the component with the node belongs to

*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
#include <stack>
#include "Profiler.h"
using namespace std;

#define maxim 201
#define nrTests 5

Profiler p("DFS");

typedef struct adjList {
	int key;
	struct adjList* next;
	struct adjList* parent;
	int d, f;
	int color;
};

typedef struct nodeT {
	int key;
	int lowLink;
	int index;
	int onStack;
	int comp;
	struct nodeT* next;
	struct nodeT* parent;
};

enum { black, gray, white };

adjList *G[maxim];
nodeT* T[maxim];
int timp, op;

adjList* createNode(int key)
{
	adjList* node = (adjList*)malloc(sizeof(adjList));
	node->key = key;
	node->parent = NULL;
	node->next = NULL;
	node->d = 0;
	node->f = 0;
	node->color = white;
	op += 6;
	return node;
}

void addNodeFirst(int index, int key)
{
	adjList* node = createNode(key);

	if (G[index] == NULL)
	{
		G[index] = node;
		op += 2;
	}
	else if (G[index]->next == NULL)
	{
		node->next = NULL;
		G[index]->next = node;
		op += 4;
	}
	else
	{
		adjList* aux = G[index]->next;
		node->next = aux;
		G[index]->next = node;
		op += 5;
	}
}

nodeT* createNodeT(int key)
{
	nodeT* Ptr = (nodeT*)malloc(sizeof(nodeT));
	Ptr->key = key;
	Ptr->index = -1;
	Ptr->lowLink = -1;
	Ptr->comp = 0;
	Ptr->onStack = 0;
	Ptr->next = NULL;
	Ptr->parent = NULL;
	
	return Ptr;
}

void addNodeT(int index, int key)
{
	nodeT* node = createNodeT(key);

	if (T[index] == NULL)
	{
		T[index] = node;
	}
	else if (G[index]->next == NULL)
	{
		node->next = NULL;
		T[index]->next = node;
	}
	else
	{
		nodeT* aux = T[index]->next;
		node->next = aux;
		T[index]->next = node;
	}
}

adjList* addFirstSLL(adjList *head, int key)
{
	if (head == NULL)
	{
		op++;
		head = createNode(key);
		return head;
	}
	adjList* ptr = createNode(key);
	adjList* aux = head;
	ptr->next = head;
	head = ptr;
	op += 3;
	return head;
}

void printAdjList(int n)
{
	for (int i = 0; i < n; i++)
	{
		cout << G[i]->key << "-> ";
		adjList* ptr = G[i]->next;
		while (ptr != NULL)
		{
			cout << ptr->key << " ";
			ptr = ptr->next;
		}
		cout << endl;
	}
}


//DFS FUNCTIONS

void DFS_visit(adjList* node)
{
	timp++;
	G[node->key]->color = gray;
	G[node->key]->d = timp;
	adjList* neigh = G[node->key]->next;
	op += 4;
	while (neigh != NULL)
	{
		op++;
		if (G[neigh->key]->color == white)// && neigh->key != node->key)
		{
			G[neigh->key]->parent = G[node->key];
			op++;
			DFS_visit(neigh);
		}
		neigh = neigh->next;
		op++;
	}

	G[node->key]->color = black;
	timp++;
	G[node->key]->f = timp;
	op += 3;
}


void DFS(int n)
{
	for (int i = 0; i < n; i++)
	{
		G[i]->parent = NULL;
		G[i]->color = white;
		op += 2;
	}
	timp = 0;
	op++;
	for (int i = 0; i < n; i++)
	{
		op++;
		if (G[i]->color == white)
		{
			DFS_visit(G[i]);
		}
	}
}

//GENERATING RANDOM GRAPH

void generateRandom(int V, int E)
{
	int u, v,found;
	for (int i = 0; i < V; i++)
	{
		G[i] = createNode(i);
	}
	int i = 0;
	while(i < E)
	{
		u = rand() % V;
		v = rand() % V;
		found = 0;
		adjList* ptr = G[u]->next;
		while (ptr != NULL)
		{
			if (ptr->key == v)
			{
				found = 1;
				break;
			}
			ptr = ptr->next;
		}
		if (found == 0)
		{
			addNodeFirst(u, v);
			i++;
		}
	}
}

//TOPOLOGICAL SORT FUNCTIONS

adjList* visitTop(adjList* head, adjList* node)
{
	timp++;
	G[node->key]->color = gray;
	G[node->key]->d = timp;

	adjList* neigh = G[node->key]->next;
	while (neigh != NULL)
	{
		if (G[neigh->key]->color == white)// && neigh->key != node->key)
		{
			G[neigh->key]->parent = G[node->key];
			head = visitTop(head, neigh);
		}
		neigh = neigh->next;
	}

	G[node->key]->color = black;
	timp++;
	G[node->key]->f = timp;
	head = addFirstSLL(head, node->key);
	return head;
}

adjList* dfsTop(adjList* head, int n)
{
	for (int i = 0; i < n; i++)
	{
		G[i]->parent = NULL;
		G[i]->color = white;
	}
	timp = 0;
	for (int i = 0; i < n; i++)
	{
		if (G[i]->color == white)
		{
			head = visitTop(head, G[i]);
		}
	}
	return head;
}

int tarjan(int V);

adjList* topologicalSort(adjList* head, int n)
{
	int c = tarjan(n);
	if (c == n)
	{
		cout << endl << "Cannot do topologicalSort - graph has cycles" << endl;
		return NULL;
	}
	head = NULL;
	head = dfsTop(head, n);
	return head;
}


//TARJAN ALGORITHM

int indx;
int nrComp;
std::stack<nodeT*> stackT;

void strongConnect(int u)
{
	T[u]->index = indx;
	T[u]->lowLink = indx;
	indx++;
	stackT.push(T[u]);
	T[u]->onStack = 1;
	nodeT* v = T[u];
	nodeT* aux = T[u]->next;
	while (aux != NULL)
	{
		v = T[aux->key];
		if (v->index == -1)
		{
			strongConnect(v->key);
			if (T[u]->lowLink > v->lowLink)
			{
				T[u]->lowLink = v->lowLink;
				aux->lowLink = v->lowLink;
			}
		}
		else if (v->onStack == 1)
		{
			if (T[u]->lowLink > v->index)
			{
				T[u]->lowLink = v->index;
				aux->lowLink = v->index;
			}
		}
		aux = aux->next;
	}
	if (T[u]->lowLink == T[u]->index)
	{
		nrComp++;
		do {
			v = stackT.top();
			stackT.pop();
			int k = v->key;
			v->onStack = 0;
			v->comp = nrComp;
			T[k]->onStack = 0;
			T[k]->comp = nrComp;
		} while (v->key != T[u]->key);
	}
}

int tarjan(int V)
{
	indx = 0;
	nrComp = 0;
	for (int i = 0; i < V; i++)
	{
		T[i] = createNodeT(G[i]->key);
		adjList* aux = G[i]->next;
		while (aux != NULL)
		{
			addNodeT(i, aux->key);
			aux = aux->next;
		}
	}
	for (int i = 0; i < V; i++)
	{
		if (T[i]->index == -1)
		{
			strongConnect(i);
		}
	}
	return nrComp;
}

//DEMO + EVALUATION

void demo()
{
	for (int i = 0; i < 8; i++)
	{
		G[i] = createNode(i);
	}

	addNodeFirst(0, 2);
	addNodeFirst(1, 3);
	addNodeFirst(1, 0);
	addNodeFirst(2, 4);
	addNodeFirst(2, 3);
	addNodeFirst(2, 1);
	addNodeFirst(3, 5);
	addNodeFirst(4, 6);
	addNodeFirst(4, 5);
	addNodeFirst(5, 7);
	addNodeFirst(5, 3);
	addNodeFirst(6, 7);
	addNodeFirst(6, 4);
	//addNodeFirst(7, 0);

	cout << "adjacency list" << endl;
	printAdjList(8);
	
	
	DFS(8);
	cout << endl << "DFS" << endl;
	for (int i = 0; i < 6; i++)
	{
		cout << G[i]->key;// << "-> d= " << G[i]->d << " f= " << G[i]->f << " ";
		if (G[i]->parent != NULL)
			cout << "  parent = " << G[i]->parent->key << endl;
		else
			cout << "  no parent"<< endl;
	}
	
	cout << endl << "Tarjan: "<<endl;
	int c = tarjan(8);
	cout << c << " connected components"<<endl;
	for (int i = 0; i < 8; i++)
	{
		cout << i << " -> " << T[i]->comp << endl;
	}

	cout << endl << "topological sort"<<endl;

	adjList* head = NULL;
	head = addFirstSLL(head, 0);
	head = topologicalSort(head, 8);
	adjList* ptr = head;
	while (ptr != NULL)
	{
		cout << ptr->key << " ";
		ptr = ptr->next;
	}
	cout << endl << endl;

}

void performance()
{
	int V, E;
	V = 100;
	for (int i = 0; i < nrTests; i++)
	{
		for (E = 1000; E < 5000; E += 100)
		{
			op = 0;
			generateRandom(V, E);
			DFS(V);
			p.countOperation("Constant_V", E, op);
		}
		E = 9000;
		for (V = 100; V < 200; V += 10)
		{
			op = 0;
			generateRandom(V, E);
			DFS(V);
			p.countOperation("Constant_E", V, op);
		}
	}
	p.divideValues("Constant_V", nrTests);
	p.divideValues("Constant_E", nrTests);
	p.showReport();
}


int main()
{
	//demo();
	//performance();
}