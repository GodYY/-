#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void binary_insertion_sort(int a[], int n)
{
	int temp;

	for (int i = 1; i < n; i++) 
	{
		temp = a[i];
		int low = 0, high = i-1;

		while(low <= high)
		{
			int mid = (low + high) / 2;
			if (temp < a[mid]) {
				high = mid - 1;
			} else {
				low = mid + 1;
			}
		}


		for (int j = i-1; j >= high + 1; j--)
		{
			a[j+1] = a[j];
		}

		a[high+1] = temp;
	}
}

int main(int n, const char **args)
{
	if (n < 2) 
	{
		fprintf(stderr, "usage: %s numbers...\n", args[0]);
		return 1;
	}

	int nn = n - 1;
	int *a;

	a = (int*)malloc(sizeof(int) * nn);

	for (int i = 0; i < nn; i++)
	{
		a[i] = atoi(args[i+1]);
	}

	binary_insertion_sort(a, nn);

	for (int i = 0; i < nn; i++)
	{
		printf("%d ", a[i]);		
	}
	printf("\n");
}