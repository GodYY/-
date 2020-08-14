#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void select_sort(int a[], int n)
{
	for (int i = 0; i < n - 1; i++)
	{
		int k = i;

		for (int j = i + 1; j < n; j++)
		{
			if (a[j] < a[k]) k = j;
		}

		if (k != i)
		{
			int temp = a[i];
			a[i] = a[k];
			a[k] = temp;
		}
	}
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

	select_sort(a, nn);

	for (int i = 0; i < nn; i++)
	{
		printf("%d ", a[i]);		
	}
	printf("\n");
}