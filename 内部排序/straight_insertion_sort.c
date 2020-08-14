#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void straight_insertion_sort(int a[], int n)
{
	int temp;

	for (int i = 1; i < n; i++)
	{
		// printf("%d begin\n", i);
		if (a[i] < a[i-1]) 
		{
			temp = a[i];
			a[i] = a[i-1];

			int j;
			for (j = i-1; j > 0 && temp < a[j-1]; j--)
			{

				a[j] = a[j-1];
			}

			a[j] = temp;
		}
		// printf("%d end\n", i);
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

	straight_insertion_sort(a, nn);

	for (int i = 0; i < nn; i++)
	{
		printf("%d ", a[i]);		
	}
	printf("\n");
}