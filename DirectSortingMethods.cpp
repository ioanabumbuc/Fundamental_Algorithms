#include <stdlib.h>
#include <stdio.h>
#include <iostream>

#define maxim 10000
#define minv 1
#define maxv 1000000
#define nrtests 5

#include "Profiler.h"
Profiler p("Sorting");

/*
Average case 
- tested 5 times for each algorithm and reported the average
- input data is the same for every algorithm such that the results are accurate
- for generating the input array i used the function FillRandomArray from profiler.h
- assignments: n^2
- comparisons: n^2
- total: n^2
=> All three methods have generate graphs with the same functions
=> however, insertion and selection sort are faster, while bubble sort performs about 10 times more operations
than the other two.

////////////////////////////////////////////////////////////////////////////////

Best case:
- it takes a sorted array as input for all 3 sorting methods
- assingments: constant for selection and bubble sort and linear (n) for insertion sort
- comparisons: n for insertion and bubble, n^2 for selection
- total: n for insertion and bubble , n^2 for selection sort
=> selection sort is the slowest in the best case
=> bubble sort is faster then insertion

////////////////////////////////////////////////////////////////////////////////

Worst case:
- it takes a descending array as input for bubble and insertion sort
- for selection sort: the first value of the array is the maximum value, and the rest of the array, 
from index 1 to n, is ascending
- assignments: n for selection, n^2 for insertion and bubble
- comparisons: n^2 for all 3
- total: n^2 for all 3
=> all 3 algorithms are O(n^2)
=> considering the total number of operations, the best is selection, then insertion, then bubble sort.

*/



void swap(int* a, int* b)
{
	int aux;
	aux = *a;
	*a = *b;
	*b = aux;
}

void printArr(int* a, int n)
{
	printf("\n");
	for (int i = 0; i < n; i++)
		printf("%d ", a[i]);
}

void insertionSort(int* a, int n)
{
	Operation assign = p.createOperation("insertion assignments", n);
	Operation comp = p.createOperation("insertion comparisons", n);
	int i, j, aux;
	for (i = 1; i < n; i++)
	{
		aux = a[i];
		j = i;
		assign.count();
		while (j > 0 && aux < a[j - 1])
		{
			a[j] = a[j - 1];
			j--;
			comp.count();
			assign.count();
		}
		comp.count();
		if (j != i)
		{
			a[j] = aux;
			assign.count();
		}
	}
}

void demoInsertion()
{
	int a[] = { 5,9,2,8,11,23,6,45,3 };
	int n = sizeof(a) / sizeof(a[0]);
	insertionSort(a, n);
	printArr(a, n);
}

void selectionSort(int* a, int n)
{
	Operation assign = p.createOperation("selection assignments", n);
	Operation comp = p.createOperation("selection comparisons", n);
	int i, j, index;
	for (i = 0; i < n; i++)
	{
		index = i;
		for (j = i + 1; j < n; j++)
		{
			if (a[j] < a[index])
			{
				index = j;
				//assign.count();
			}
			comp.count();
		}
		if (i != index)
		{
			swap(&a[i], &a[index]);
			assign.count(3);
		}
	}
}

void demoSelection()
{
	int a[] = { 5,9,2,8,11,23,6,45,3 };
	int n = sizeof(a) / sizeof(a[0]);
	selectionSort(a, n);
	printArr(a, n);
}

void bubbleSort(int* a, int n)
{
	Operation assign = p.createOperation("bubble assignments", n);
	Operation comp = p.createOperation("bubble comparisons", n);
	int i, swapped, aux;
	aux = n;
	assign.count();
	do
	{
		swapped = 0;
		for (i = 0; i < aux - 1; i++)
		{
			if (a[i] > a[i + 1])
			{
				swap(&a[i], &a[i + 1]);
				swapped = 1;
				assign.count(3);
			}
			comp.count();
		}
		aux--;
		assign.count();
	} while (swapped != 0);

}

void demoBubble()
{
	int a[] = { 5,9,2,8,11,23,6,45,3 };
	int n = sizeof(a) / sizeof(a[0]);
	bubbleSort(a, n);
	printArr(a, n);
}

void copyArr(int* a, int* b, int n)
{
	for (int i = 0; i < n; i++)
		b[i] = a[i];
}

void average()
{
	int a[maxim],aux[maxim];
	for (int i = 0; i < nrtests; i++)
	{
		FillRandomArray(aux, maxim);
		for (int n = 100; n < maxim; n=n+100)
		{
			copyArr(aux, a, n);
			insertionSort(a, n);
			copyArr(aux, a, n);
			selectionSort(a, n);
			copyArr(aux, a, n);
			bubbleSort(a, n);
		}
	}
	p.divideValues("insertion assignments", nrtests);
	p.divideValues("insertion comparisons", nrtests);
	p.divideValues("selection assignments", nrtests);
	p.divideValues("selection comparisons", nrtests);
	p.divideValues("bubbles assignments", nrtests);
	p.divideValues("bubble comparisons", nrtests);

	p.addSeries("insertion total", "insertion assignments", "insertion comparisons");
	p.addSeries("selection total", "selection assignments", "selection comparisons");
	p.addSeries("bubble total", "bubble assignments", "bubble comparisons");

	p.createGroup("average assignments", "insertion assignments", "selection assignments","bubble assignments");
	p.createGroup("average comparisons", "insertion comparisons", "selection comparisons", "bubble comparisons");
	p.createGroup("average total", "insertion total", "selection total", "bubble total");
	p.showReport();	
}

void best()
{
	int a[maxim];
	for (int n = 100; n < maxim; n=n+100)
	{
		FillRandomArray(a, n, minv, maxv, 0, 1);
		insertionSort(a, n);
		FillRandomArray(a, n, minv, maxv, 0, 1);
		selectionSort(a, n);
		FillRandomArray(a, n, minv, maxv, 0, 1);
		bubbleSort(a, n);
	}
	p.addSeries("insertion total", "insertion assignments", "insertion comparisons");
	p.addSeries("selection total", "selection assignments", "selection comparisons");
	p.addSeries("bubble total", "bubble assignments", "bubble comparisons");

	p.createGroup("best assignments", "insertion assignments", "selection assignments", "bubble assignments");
	p.createGroup("best comparisons", "insertion comparisons", "selection comparisons", "bubble comparisons");
	p.createGroup("best total", "insertion total", "selection total", "bubble total");
	p.showReport();
}

void worst()
{
	int a[maxim];
	for (int n = 100; n < maxim; n = n + 100)
	{
		FillRandomArray(a, n, minv, maxv, 0, 2);
		insertionSort(a, n);
		FillRandomArray(a, n, minv, maxv - 1, 0, 1);
		a[0] = maxv;
		selectionSort(a, n);
		FillRandomArray(a, n, minv, maxv, 0, 2);
		bubbleSort(a, n);
	}
	p.addSeries("insertion total", "insertion assignments", "insertion comparisons");
	p.addSeries("selection total", "selection assignments", "selection comparisons");
	p.addSeries("bubble total", "bubble assignments", "bubble comparisons");

	p.createGroup("worst assignments", "insertion assignments", "selection assignments", "bubble assignments");
	p.createGroup("worst comparisons", "insertion comparisons", "selection comparisons", "bubble comparisons");
	p.createGroup("worst total", "insertion total", "selection total", "bubble total");
	p.showReport();
}

int main()
{
	//demoInsertion();
	//demoSelection();
	//demoBubble();
	
	//average();
	//best();
	//worst();
	return 0;
}

