#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void _merge(int a[], int s[], int low, int mid, int high)
{
	int i = low, j = mid + 1, k = low;

	while ((i <= mid) && (j <= high))
	{
		if (a[i] <= a[j]) s[k++] = a[i++];
		else s[k++] = a[j++];
	}

	while (i <= mid) s[k++] = a[i++];
	while (j <= high) s[k++] = a[j++];

	for (k = low; k <= high; k++)
		a[k] = s[k];
}

void _merge_sort(int a[], int s[], int low, int high)
{
	if (low < high)
	{
		int mid = (low + high) / 2;
		_merge_sort(a, s, low, mid);
		_merge_sort(a, s, mid+1, high);
		_merge(a, s, low, mid, high);
	}
}

void merge_sort(int a[], int n)
{
	int s[n];
	_merge_sort(a, s, 0, n-1);
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

	merge_sort(a, nn);

	for (int i = 0; i < nn; i++)
	{
		printf("%d ", a[i]);		
	}
	printf("\n");
}