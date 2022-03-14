/*
Quick Sort:
	- Best case: each partition divides the array into 2 equal parts
		=>O(nlogn)
	- Average case: very close to best case
	- Worst case: depends on the taken pivot. In my case => pivot is the last element => worst case is the sorted array
		Each partition divides the array into 1 element and the rest
		=>O(n^2)

Quick Select - Selects ith smallest element from an unordered array
	- Uses partition algorithm from Quick Sort, but only one recursive call
	- Best case: O(n) - the element is found after one partition 
	- Averge case: O(n)
	- Worst case: O(n^2) - same case as quick sort

Quick Sort vs Heap Sort 
	- both are O(nlogn) in the average case
	- however, quick sort is more efficient on big data
*/


#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "Profiler.h"
Profiler p("Quick");

#define m 5
#define minim 0
#define maxim 10000

using namespace std;

int opQuick, opHeap;

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
		cout << a[i] << " ";
}

void copyArr(int* dest, int* source, int n)
{
	for (int i = 0; i < n; i++)
		dest[i] = source[i];
		
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
	opHeap += 2; //2 comparisons
	return l;
}

void heapify(int* a, int i, int n)
{
	int l;
	l = indexOfLargest(a, i, n);
	if (l != i && l < n)
	{
		swap(&a[i], &a[l]);
		opHeap += 3;
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

int partition(int* a, int p, int r)
{
	int i, j;
	i = p - 1;
	for (j = p; j <= r - 1; j++)
	{
		if (a[j] <= a[r])
		{
			i++;
			swap(&a[i], &a[j]);
			opQuick += 3;
		}
		opQuick++;
	}
	swap(&a[i + 1], &a[r]);
	opQuick += 3;
	return i + 1;
}

int randomizedPartition(int* a, int p, int r)
{
	int x = rand() % (r - p) + p;
	swap(&a[r], &a[x]);
	opQuick += 3;
	return partition(a, p, r);
}


void quickSort(int* a, int p, int r)
{
	if (p < r)
	{
		int q;
		q = partition(a, p, r);
		//q = randomizedPartition(a, p, r);
		quickSort(a, p, q - 1);
		quickSort(a, q + 1, r);
	}
}

void demoQuickSort()
{
	cout <<endl << "QuickSort: " << endl;
	int a[10];
	int n = 10;
	FillRandomArray(a, n, 0, 15);
	printArr(a, n); 
	cout << endl;
	quickSort(a, 0, n-1);
	printArr(a, n); 
	cout << endl;

}

int quickSelect(int* a, int p, int r, int i)
{
	if (p == r)
		return a[p];
	int q = randomizedPartition(a, p, r);
	int k = q - p + 1;
	if (i == k)
		return a[q];
	else if (i < k)
		return quickSelect(a, p, q - 1, i);
	else
		return quickSelect(a, q + 1, r, i - k);
}

void demoQuickSelect()
{
	cout << endl << "Quick Select: " << endl;
	int a[10], n = 10;
	FillRandomArray(a, n, 0, 15,1);
	printArr(a, n);
	cout << endl<<"pe pozitia 5 se afla ";
	cout << quickSelect(a, 0, n - 1, 5);
	quickSort(a, 0, n - 1);
	cout << endl;
	printArr(a, n);
}

void average()
{
	int a[maxim],aux[maxim];
	for (int i = 0; i < m; i++)
	{
		for (int n = 100; n < maxim; n = n + 100)
		{
			opHeap = 0; 
			opQuick = 0;
			FillRandomArray(aux, n);
			copyArr(a, aux, n);
			heapSort(a, n);
			copyArr(a, aux, n);
			quickSort(a, 0, n-1);
			p.countOperation("Heap", n, opHeap);
			p.countOperation("Quick", n, opQuick);
		}
	}
	p.divideValues("Heap", m);
	p.divideValues("Quick", m);
	p.createGroup("Heap_vs_Quick", "Heap", "Quick");
	p.showReport();
}

void avg_vs_worst()
{
	int a[maxim];
	for (int n = 100; n < maxim; n = n + 100)
	{
		opQuick = 0;
		FillRandomArray(a, n);
		quickSort(a, 0, n - 1);
		p.countOperation("Average_Quick", n, opQuick);
		opQuick = 0;
		quickSort(a, 0, n - 1);
		p.countOperation("Worst_Quick", n, opQuick);
	}
	p.showReport();
}

int main()
{
	//demoQuickSort();
	//demoQuickSelect();
	//average();
	avg_vs_worst(); 
}