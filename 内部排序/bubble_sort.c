#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void bubble_sort(int a[], int n)
{
	int flag = 1;
	int m = n-1;

	while (m > 0 && (flag == 1))
	{
		flag = 0;

		for (int i = 0; i < m; i++)
		{
			if (a[i] > a[i+1]) {
				int temp = a[i];
				a[i] = a[i+1];
				a[i+1] = temp;
				flag = 1;
			}
		}

		m--;
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

	bubble_sort(a, nn);

	for (int i = 0; i < nn; i++)
	{
		printf("%d ", a[i]);		
	}
	printf("\n");
}