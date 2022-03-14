/*
	BUILD HEAP - BOTTOM-UP vs TOP-DOWN
	HEAP SORT

	Bottom up - applies heapify at each node starting from the last non-leaf at the bottom
			  - sinks the root to the right position
			  - in average case: O(n)

	Top Down - applies push_heap at each element of the array starting with the first
			 - push_heap adds 1 element at the end of the array/heap, then it goes up until it finds
			 the right place by comparing the element to its parent (and the parent of the parent and so on)
			 - avg: O(n log n)

	Heap sort - builds max heap from the input array
			  - at each step: swaps 1st element with the last 
			  => biggest element is at the end in the right place
			  - decrement heap size with 1 + apply heapify at index 0
			  - repeat until there's 2 elements left
			  - complexity: O(n log n)
			  - optimal algorithm

	Worst case for building heap - array sorted ascendently
		for bottom up aproach, worst case is very close to average case
		top down - big difference => bottom up is more efficient
*/

#include <iostream>
#include <stdio.h>
#include <stdlib.h>
#include <windows.h>

#define m 5
#define maximum 10000
#define min_range 10
#define max_range 50000

#include "Profiler.h"
Profiler p("Heap");

int totalBottomUp, totalTopDown;

void copyArr(int* a, int* b, int n)
{
	for (int i = 0; i < n; i++)
		b[i] = a[i];
}

void swap(int* a, int* b)
{
	int aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

void printArr(int* a, int n)
{
	for (int i = 0; i < n; i++)
		printf("%d ", a[i]);
}

int indexOfLargest(int* a, int i, int n)
{
	int j = 2 * i + 1, k = 2 * i + 2;
	int l = i;
	if (j < n && a[i] < a[j])//if a[i] < left child of i
	{
		l = j; //left child is the largest so far
	}
	if (k < n && a[k] > a[l]) //if right child > the largest so far => largest.
	{
		l = k;
	}
	totalBottomUp += 2; //2 comparisons
	return l;
}

void heapify(int* a, int i, int n)
{
	int l;
	l = indexOfLargest(a, i, n);
	if (l != i && l < n)
	{
		swap(&a[i], &a[l]);
		totalBottomUp += 3;
		heapify(a, l, n);
	}
}

void buildHeapBottomUp(int* a, int n)
{
	for (int i = n / 2 - 1; i >= 0; i--)
	{
		heapify(a, i, n);
	}
}

void pushHeap(int* a, int* size, int x)
{
	(*size)++;
	a[*size - 1] = x;
	totalTopDown += 2;
	int i = *size - 1;
	int parent = (i - 1) / 2;
	while (parent >= 0 && a[parent] < a[i])
	{
		swap(&a[i], &a[parent]);
		i = parent;
		parent = (i - 1) / 2;
		totalTopDown += 4; //3 assign + 1 comp
	}
	totalTopDown++; //last comparison in while is not counted inside the loop
}

void buildHeapTopDown(int* a, int n)
{
	int size = 0;
	totalTopDown++;
	for (int i = 0; i < n; i++)
	{
		pushHeap(a, &size, a[i]);
	}
}

void heapSort(int* a, int n)
{
	buildHeapBottomUp(a, n);
	int size = n;
	for (int i = n - 1; i >= 1; i--)
	{
		swap(&a[0], &a[i]);
		size--;
		heapify(a, 0, size);
	}
}

void demoBuildHeapBottomUp()
{
	int a[7],n=7;
	FillRandomArray(a, n, 0, 10);
	printArr(a, n); printf("\n");
	buildHeapBottomUp(a, n);
	printArr(a, n);
}

void demoBuildHeapTopDown()
{
	int a[7], n = 7;
	FillRandomArray(a, n, 0, 10);
	printArr(a, n); printf("\n");
	buildHeapTopDown(a, n);
	printArr(a, n);
}

void demoHeapSort()
{
	int a[10], n = 10;
	FillRandomArray(a, n, 0, 20);
	printArr(a, n); printf("\n");
	heapSort(a, n);
	printArr(a, n);
}

void average()
{
	int a[maximum], aux[maximum];
	for (int i = 0; i < m; i++)
	{
		FillRandomArray(aux, maximum);
		for (int n = 100; n < maximum; n = n + 100)
		{
			totalBottomUp = 0;
			totalTopDown = 0;
			copyArr(aux, a, n);
			buildHeapBottomUp(a, n);
			copyArr(aux, a, n);
			buildHeapTopDown(a, n);
			p.countOperation("BuildHeapBottomUp", n, totalBottomUp);
			p.countOperation("BuildHeapTopDown", n, totalTopDown);
		}
	}
	p.divideValues("BuildHeapBottomUp", m);
	p.divideValues("BuildHeapTopDown", m);
	p.createGroup("BuildHeap", "BuildHeapBottomUp", "BuildHeapTopDown");
	p.showReport();
}

void worst()
{
	int a[maximum];
	for (int n = 100; n < maximum; n = n + 100)
	{
		totalBottomUp = 0;
		FillRandomArray(a, n, min_range, max_range, 0, 1);
		buildHeapBottomUp(a, n);
		p.countOperation("worstBottomUp", n, totalBottomUp);

		totalTopDown = 0;
		FillRandomArray(a, n, min_range, max_range, 0, 1);
		buildHeapTopDown(a, n);
		p.countOperation("worstTopDown", n, totalTopDown);
	}
	p.createGroup("worst", "worstBottomUp", "worstTopDown");
	p.showReport();
}

int main()
{
	demoBuildHeapBottomUp(); printf("\n");
	demoHeapSort(); printf("\n");
	demoBuildHeapTopDown();
	//average();
	//worst();
}