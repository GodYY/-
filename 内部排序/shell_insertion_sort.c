#include <stdlib.h>
#include <stdio.h>
#include <string.h>

void shell_insert(int a[], int n, int dk)
{
	for (int i = dk; i < n; i++)
	{
		if (a[i] < a[i-dk])
		{
			int temp = a[i];
			a[i] = a[i-dk];

			int j;
			for (j = i-dk; j > 0 && temp < a[j-dk]; j -= dk)
			{
				a[j] = a[j-dk];
			}

			a[j] = temp;
		}
	}
}

void shell_sort(int a[], int n, int dt[], int t)
{
	for (int i = 0; i < t; i++)
	{
		shell_insert(a, n, dt[i]);
	}
}