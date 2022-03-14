
/*
HASH TABLE IMPLEMENTATION WITH OPEN ADRESSING AND QUADRATIC PROBING

quadratic probing - hash function of the form
	h(k,i) = (h'(k) + c1 x i + c2 x i^2) mod m
When inserting - try first i=0 and calculate hash
			   - if the resulted place is empty => put the element in the table
			   - otherwise, increment i until a place is found or until the index is out of range
When searching - do the same and check if the value wanted is at that place
			   - stop if an empty slot is reached => it means that the element is not in the table

Filling factor = nb of inserted elements / total nb of elements

Average effort for searching an element that isn't in the hash table increases when FF increases. That is because
	search function stops when it reaches an empty spot. When the table is almost full, it means there are
	very few occasions for the algorithm to stop when reaching an empty spot
The average complexity to find an element (that exists in the table) is O(1) => efficient
	- this is seen in the output table at Average Effort When Found
	- otherwise, it takes few checks to find the wanted element
*/


#pragma warning(disable:4996)
#ifdef _MSC_VER
#define _CRT_SECURE_NO_WARNINGS
#endif

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

using namespace std;

#include "Profiler.h"
Profiler p("HashTable");

#define maxim 10000
#define N 9973
#define nrTests 5
#define min_range 0
#define max_range 50000
#define len 3
#define m 3000
#define tSize 5

int op;
int opForMaxEff;
double outputTable[5][5];

typedef struct {
	int id;
	char name[30];
}entry;


int h(int key, int i, int n) 
{
	return(key + 227 * i + 491 * i * i) % n;
}

int insert(entry table[], int key, const char *name, int n)
{
	int i = 0;
	do {
		int j = h(key, i, n);
		if (strcmp(table[j].name, "nil") == 0)
		{
			strcpy(table[j].name, name);
			table[j].id = key;
			return j;
		}
		else
		{
			i++;
		}
	} while (i < n);
	return -1;
}

int search(entry table[], int key, int n) 
{
	int i = 0,j;
	do {
		j = h(key, i, n);
		op++;
		opForMaxEff++;
		if (table[j].id == key && strcmp(table[j].name,"nil") != 0)
			return j;
		i++;
	} while (strcmp(table[j].name, "nil") != 0 && i < n);
	return -1;
}

void initializeTable(entry table[], int n)
{
	for (int i = 0; i < n; i++)
	{
		strcpy(table[i].name, "nil");
		table[i].id = 0;
	}
}

char* randomString(int length)
{
	const char* string = "qwertyuiopasdfghjklzxcvbnmQWERTYUIOPASDFGHJKLZXCVBNM";
	int size = strlen(string);
	char* randomString = (char*)malloc((length+1) * sizeof(char));

	int a = 0;
	for (int i = 0; i < length; i++)
	{
		a = rand() % size;
		randomString[i] = string[a];
	}
	randomString[length] = '\0';
	return randomString;
}

void demo()
{
	entry table[10];
	int n = 10;
	initializeTable(table, n);
	int keys[6];
	FillRandomArray(keys, 6,0,100,true);
	cout << "random elements to insert: ";
	for (int i = 0; i < 6; i++)
		cout << keys[i] << " ";
	cout << 15 << endl;
	for (int i = 0; i < 6; i++)
	{
		int x = keys[i];
		insert(table, x, randomString(len), n);
	}
	insert(table, 15, randomString(len), n);
	cout << endl << "Values in the hash table: " << endl;
	for (int i = 0; i < n; i++)
	{
		cout << table[i].id << " " << table[i].name << endl;
	}
	int found = search(table, 15, n);
	if (found >= 0)
		cout << "Element with key 15 is at index " << found << " with name " << table[found].name << endl;
	else
		cout << "Element with key 15 is not in the table" << endl;
	found = search(table, 30, n);
	if (found >= 0)
		cout << "Elemenet with key 30 is at index " << found << " with name " << table[found].name << endl;
	else
		cout << "Element with key 30 is not in the table";
}

void evaluation()
{
	double alpha[] = { 0.8,0.85,0.9,0.95,0.99 };
	int alphaS = sizeof(alpha) / sizeof(alpha[0]);
	entry table[N];
	for (int i = 0; i < alphaS; i++)
	{
		int maxEffFound = 0;
		int maxEffNotFound = 0;
		outputTable[i][0] = alpha[i];
		int n = alpha[i] * N;
		int size = maxim + m / 2 + 1; //maxim - nr of elements inserted ; m/2 - nr of elements to search that aren't in table
		int temp[maxim + m / 2 + 1];
		for (int t = 0; t < nrTests; t++)
		{
			initializeTable(table, N);
			FillRandomArray(temp, size, min_range, max_range, true);
			for (int j = 0; j < n; j++)
			{
				char* s = randomString(len);
				insert(table, temp[j], s, N);
			}
			op = 0;
			for (int j = 0; j < m / 2; j++)
			{
				int index = rand() % n;
				opForMaxEff = 0;
				search(table, temp[index], N);
				if (opForMaxEff >= maxEffFound)
				{
					maxEffFound = opForMaxEff;
				}
			}
			outputTable[i][1] += double(op) / double(m/2); //average effort for found
			op = 0;
			for (int j = maxim + 1; j < maxim + m/2 + 1; j++)
			{
				opForMaxEff = 0;
				search(table, temp[j], N);
				if (opForMaxEff >= maxEffNotFound)
				{
					maxEffNotFound = opForMaxEff;
				}
			}
			
			outputTable[i][3] += double(op) / double(m/2);
		}
		outputTable[i][2] = maxEffFound;
		outputTable[i][4] = maxEffNotFound;
	}

	cout << "FF   avgEffFound   maxEffFound avgEffNotFound maxEffNotFound"<<endl;
	for (int i = 0; i < tSize; i++)
	{
		for (int j = 0; j < tSize; j++)
		{
			if(j == 0)
				printf("%.2f       ", outputTable[i][j]);
			else if ( j == 1 || j == 3)
			{
				outputTable[i][j] /= nrTests;
				printf("%.3f       ", outputTable[i][j]);
			}
			else
			{
				cout << outputTable[i][j] << "       ";
			}
		}
		cout << endl;
	}
}

int main()
{
	//demo();
	evaluation();
}