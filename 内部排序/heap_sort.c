#include <stdlib.h>
#include <stdio.h>
#include <string.h>

// [s+1..m] 已经成堆，将 [s..m] 调整成堆。
void heap_adjust(int a[], int s, int m)
{
	int rc = a[s];

	for (int i = s * 2 + 1; i < m; i = i * 2 + 1)
	{
		if ((i < m-1) && (a[i] < a[i+1])) i++;
		if (rc >= a[i]) break;
		a[s] = a[i]; s = i;
	}

	a[s] = rc;
}

// 从最后一个非终端节点 n/2 开始，依次向上调整堆。
void heap_create(int a[], int n)
{
	for (int i = (n / 2) - 1; i >= 0; i--)
		heap_adjust(a, i, n);
}

// 1.先建大堆，设 i=n-1, 初始序列为 [0..i]。
// 2.对调 a[0] 和 a[i]。 
// 3.调整 [0..i-1], i--。
// 4.重复2、3，直致 i==0,即堆只剩一个元素。
void heap_sort(int a[], int n)
{
	heap_create(a, n);

	printf("heap create:");
	for (int i = 0; i < n; i++)
		printf(" %d", a[i]);
	printf("\n");

	for (int i = n-1; i > 0; i--)
	{
		int temp = a[i];
		a[i] = a[0];
		a[0] = temp;

		heap_adjust(a, 0, i);
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

	heap_sort(a, nn);

	for (int i = 0; i < nn; i++)
	{
		printf("%d ", a[i]);		
	}
	printf("\n");
}