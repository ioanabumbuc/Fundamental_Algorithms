/*
MERGE K ORDERED LISTS HAVING N ELEMENTS IN TOTAL
Input: k ordered lists with n elements in total - each list has n/k elements (the last one can have n/k + n%k)
Output: 1 ordered list with n elements obtained by merging the input lists

The idea:
	- create a min heap of data structures containing: key(data from input) and index of list
	- at each step: take the min of the heap and put it in the output list
	- min of heap corresponds to some list. if the list is not empty, store at the root of the heap 
		the next element from the list, then heapify from root to heapSize
	- if the corresponding list becomes empty => swap root with last element, decrement heapSize then heapify
	- repeat until heapSize = 0

Algorithm time complexity: O(nlogk)
	- size of heap is k and heapify has O(logk)
	- apply this algorithm n times for all n values => O(nlogk)

Tested average case in two ways:
	1. take k constant with 3 values: 5,10,100. n varies between 100 and 10000. From the resulted graph,
		we can see that when n is very big, it is more efficient to have a bigger value for k
	2. take n = 10.000 constant and k varies between 10 and 500. for big values of k the algorithm is 
		more efficient
*/

#include <stdlib.h>
#include <stdio.h>
#include <iostream>
using namespace std;

#define maxim 10000
#define maxlists 501
#define minrange 0
#define maxrange 50000

#include "Profiler.h"

Profiler p("Merge");

int totalOp;

//LIST OPERATIONS//

typedef struct list {
	int key;
	struct list* next;
}node;

node* lists[maxlists];

void insertLast(int x, node* head)
{
	node* ptr = (node*)malloc(sizeof(node*));
	ptr->key = x;
	ptr->next = NULL;
	node* curr = head;
	totalOp += 4; //3 assignments + 1 comparison
	if (curr == NULL)
	{
		curr->next = ptr;
		totalOp++;
		return;
	}
	while (curr->next != NULL)
	{
		curr = curr->next;
		totalOp += 2; //1 assign + 1 comp
	}
	curr->next = ptr;
	totalOp += 2; //1 assign + 1 comp from while
}

void printList(node* head)
{
	if (head == NULL || head->next == NULL)
	{
		cout << "List is empty" << endl;
		return;
	}
	node* ptr = (node*)malloc(sizeof(node*));
	ptr = head->next;
	while (ptr != NULL)
	{
		cout << ptr->key << " ";
		ptr = ptr->next;
	}
	cout << "\n";
}

node* arrayToList(int* a,int n)
{
	node* head = (node*)malloc(sizeof(node*));
	head->next = NULL;
	totalOp ++;
	for (int i = 0; i < n; i++)
	{
		insertLast(a[i], head);
	}
	return head;
}

void generate_k(int nbOfLists, int elementsForEachList) //k lists each of n/k elements
{
	int a[maxim];
	for (int i = 0; i < nbOfLists; i++)
	{
		FillRandomArray(a, elementsForEachList,minrange,maxrange, 1, 1);
		lists[i] = arrayToList(a, elementsForEachList);
	}
}

//HEAP OPERATIONS//

void swap(int* a, int* b)
{
	int aux = *a;
	*a = *b;
	*b = aux;
}

typedef struct heap {
	int listIndex;
	int key;
};

int indexOfSmallest(heap heap[], int i, int n)
{
	int j = 2 * i + 1, k = 2 * i + 2;
	int s = i;
	if (j < n && heap[j].key < heap[i].key)//if a[i] > left child of i
	{
		s = j; //left child is the smallest so far
	}
	if (k < n && heap[k].key < heap[s].key) //if right child < the largest so far => smallest
	{
		s = k;
	}
	totalOp += 2; //2 comp
	return s;
}

void heapify(heap heap[], int i, int heapSize)
{
	int s;
	s = indexOfSmallest(heap, i, heapSize);
	if (s != i && s < heapSize)
	{
		swap(heap[i], heap[s]);
		totalOp += 3; //3 assign
		heapify(heap, s, heapSize);
	}
}

void buildHeapBottomUp(heap a[], int heapSize)
{
	for (int i = heapSize / 2 - 1; i >= 0; i--)
	{
		heapify(a, i, heapSize);
	}
}

void printHeap(heap a[], int heapSize)
{
	for (int i = 0; i < heapSize; i++)
	{
		cout << a[i].key << " from list " << a[i].listIndex<<endl;
	}
	cout << endl;
}

void createHeap(heap heap[],int heapSize, int nbOfElements) //k lists of n/k elements 
{
	//generate a heap with heapsize = k (nbOfLists)
	//each list has n/k elements
	generate_k(heapSize, nbOfElements);
	for (int i = 0; i < heapSize; i++)
	{
		heap[i].listIndex = i;
		heap[i].key = (lists[i]->next)->key;
		totalOp += 3; //2 assign
	}
	buildHeapBottomUp(heap, heapSize);
}

void deleteRootFromHeap(heap heap[], int *n)
{
	swap(heap[0], heap[(*n) - 1]);
	(*n)--;
	totalOp += 4; //4 assign
	heapify(heap, 0, *n);
}

//MERGE 2 LISTS//

void merge(node* node1, node* node2, node* node3)
{
	node1 = node1->next;
	node2 = node2->next;
	node* curr = node3;
	while (node1 != NULL && node2 != NULL)
	{
		if (node1->key <= node2->key)
		{
			insertLast(node1->key, node3);
			node1 = node1->next;
			curr = curr->next;
		}
		else
		{
			insertLast(node2->key, node3);
			node2 = node2->next;
			curr = curr->next;
		}
	}
	if (node1 != NULL)
	{
		curr->next = node1;
	}
	if (node2 != NULL)
	{
		curr->next = node2;
	}
}

//MERGE ALGORITHM//

node* mergekSortedLists(int k, int n) //k lists, n total nr of elements
{
	heap heap[maxim];
	node* output = (node*)malloc(sizeof(node*));
	output->next = NULL;
	int heapSize = k;
	int nbOfElements = n / k;
	totalOp += 3; // 3 assign
	createHeap(heap, heapSize, nbOfElements);

	for (int i = 0; i < k; i++)
	{
		lists[i] = lists[i]->next; //set the pointer from head of list to 1st actual element
		totalOp++;
	}

	while (heapSize)
	{
		int min = heap[0].key; //take min from root of heap and insert it in output list
		insertLast(min, output);
		int indexOfMin = heap[0].listIndex;
		totalOp += 3; //2 assign + 1 comp
		if (lists[indexOfMin] != NULL && lists[indexOfMin] -> next != NULL)
		{
			lists[indexOfMin] = lists[indexOfMin]->next; 
			heap[0].key = lists[indexOfMin]->key;
			//put in heap[0] the next value from the corresponding list
			totalOp += 2; //2 assign
			heapify(heap, 0, heapSize);
			//heapify from index 0 to have the minimum in heap[0]
		}
		else
		{
			deleteRootFromHeap(heap, &heapSize);
		}
	}
	return output;
}

void demo()
{
	cout << "Merge 2 random sorted lists" << endl;
	int n = 5;
	int a[5];
	FillRandomArray(a, n, 0, 20, 1, 1);
	node* head1 = arrayToList(a, n);
	cout << "list 1:  "; printList(head1);
	FillRandomArray(a, n, 0, 20, 1, 1);
	node* head2 = arrayToList(a, n);
	cout << "list 2:  "; printList(head2);
	node* head3 = (node*)malloc(sizeof(node*));
	head3->next = NULL;
	merge(head1, head2, head3);
	cout << "merged:  "; printList(head3);

	cout << endl << "Merge 4 random sorted lists with 16 elements in total" << endl;
	node* output = (node*)malloc(sizeof(node*));
	output = mergekSortedLists(4, 16);
	printList(output);
}

void averagekConstant()
{
	int k[] = { 5,10,100 };
	int a[maxim];
	for (int j = 0; j < 3; j++)
	{
		for (int n = 100; n < maxim; n += 100)
		{
			totalOp = 0;
			int size = n / k[j];
			for (int i = 0; i < k[j]; i++)
			{
				FillRandomArray(a, size, minrange, maxrange, 0, 1);
				lists[i] = arrayToList(a, size);
			}
			node* output = mergekSortedLists(k[j], n);
			free(output);
			if (j == 0)
				p.countOperation("Average_k1_5", n, totalOp);
			else if (j == 1)
				p.countOperation("Average_k2_10", n, totalOp);
			else
				p.countOperation("Average_k3_100", n, totalOp);
		}
	}
	p.createGroup("k_Constant", "Average_k1_5", "Average_k2_10", "Average_k3_100");
	p.showReport();
}
void averagenConstant()
{
	int n = maxim;
	int a[maxim];
	for (int k = 10; k <= 500; k += 10)
	{
		totalOp = 0;
		int size = n / k;
		for (int i = 0; i < k-1; i++)
		{
			FillRandomArray(a, size, minrange, maxrange, 0, 1);
			lists[i] = arrayToList(a, size);
		}
		int sizeOfLast = n / k + n % k;
		FillRandomArray(a, sizeOfLast, minrange, maxrange, 0, 1);
		lists[k - 1] = arrayToList(a, sizeOfLast);

		node* output = mergekSortedLists(k, n);
		free(output);
		p.countOperation("Constant_n", k, totalOp);
	}
	p.showReport();
}

int main()
{	
	//demo();
	//averagekConstant();
	//averagenConstant();
	return 0;
}