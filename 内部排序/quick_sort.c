#include <stdlib.h>
#include <stdio.h>
#include <string.h>

int partion(int a[], int low, int high) 
{
	int pivot = a[low];

	while (low < high)
	{
		while (low < high && pivot <= a[high]) high--;
		a[low] = a[high];

		while (low < high && a[low] <= pivot) low++;
		a[high] = a[low];
	}

	a[low] = pivot;

	return low;
}

void q_sort(int a[], int low, int high)
{
	if (low < high) 
	{
		int pivot = partion(a, low, high);
		q_sort(a, low, pivot - 1);
		q_sort(a, pivot+1, high);
	}
}

void quick_sort(int a[], int n)
{
	q_sort(a, 0, n-1);
}

int main(int n, const char **args)
{
	if (n < 2) 
	{
		fprintf(stderr, "usage: %s numbers...\n", args[0]);
		exit(1);
	}

	int nn = n - 1;
	int *a;

	a = (int*)malloc(sizeof(int) * nn);

	for (int i = 0; i < nn; i++)
	{
		a[i] = atoi(args[i+1]);
	}

	quick_sort(a, n);

	for (int i = 0; i < nn; i++)
	{
		printf("%d ", a[i]);		
	}
	printf("\n");
}