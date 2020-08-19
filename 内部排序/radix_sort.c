// 最低位优先链式基数排序算法
#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>


#define MAXNUM_KEY 	8				// 关键字最大数量
#define RADIX 		10				// 基数
#define MAX_SPACE 	10000			// 静态链表最大空间

typedef int keyType;
typedef const char *cType;

typedef struct {
	keyType 	keys[MAXNUM_KEY];	// 关键字
	cType 		content;			// 内容
	int			next;				// next下标
} slCell;

typedef struct {
	slCell 	r[MAX_SPACE];			// 记录
	int 	keyNum;					// 关键字数量
	int 	recNum;					// 记录数量
} slList;

typedef int arrType[RADIX];			// 指针数组


int ord(keyType key)
{
	return (int)(key - '0');	
}

void distribute(slCell r[], int i, arrType *f, arrType *e) 
{
	// 静态链表 r 中记录已按（keys[0]..keys[i-1]）有序。
	// 本算法按照 keys[i] 建立 RADIX 个子表, 每个子表的 keys[i] 相同。

	int j;

	for (j = 0; j < RADIX; j++) (*f)[j] = 0;

	for (int p = r[0].next; p; p = r[p].next)
	{
		j = ord(r[p].keys[i]);
		if (!(*f)[j]) (*f)[j] = p;
		else r[(*e)[j]].next = p;
		(*e)[j] = p; 
	}
}

void collect(slCell r[], arrType *f, arrType *e)
{
	// 本算法按 keys[i] 自小到大的将 f[0..RADIX-1] 所指各个子表链接在一起。

	int j, t;
	for (j = 0; !(*f)[j]; j++);

	r[0].next = (*f)[j]; t = (*e)[j];

	while (j < (RADIX - 1))
	{
		for (j++; j < RADIX - 1 && !(*f)[j]; j++);

		if ((*f)[j])
		{
			r[t].next = (*f)[j]; 
			t = (*e)[j];
		}
	} 

	r[t].next = 0;
}

void radix_sort(slList *l)
{
	int i;
	arrType f, e;

	for (i = 0; i < l->recNum; i++) l->r[i].next = i+1;
	l->r[l->recNum].next = 0;
	
	for (i = 0; i < l->keyNum; i++)
	{
		distribute(l->r, i, &f, &e);
		collect(l->r, &f, &e);
	}

	// 调整 r 中的记录，按顺序排列
	int p = l->r[0].next;
	for (i = 1; i <= l->recNum; i++)
	{
		while (p < i) p = l->r[p].next;

		if (p != i) {
			int pp = l->r[p].next;
			slCell tmp = l->r[i];
			l->r[i] = l->r[p];
			l->r[p] = tmp;
			l->r[i].next = p;
			p = pp;
		} else {
			p = l->r[p].next;
		}
	}
}

int main(int n, const char **args)
{
	if (n < 3) 
	{
		fprintf(stderr, "usage: %s keynum numbers...\n", args[0]);
		exit(1);
	}

	slList l;

	l.keyNum = atoi(args[1]);
	if (l.keyNum <= 0 || l.keyNum > MAXNUM_KEY)
	{
		fprintf(stderr, "invalid keynum\n");
		exit(1);
	}

	l.recNum = n - 2;

	for (int i = 1; i <= l.recNum; i++)
	{
		const char *numStr = args[i+1];
		int len = strlen(numStr);
		if (len > l.keyNum) {
			fprintf(stderr, "%s keynum overflow\n", numStr);
			exit(1);
		}
		
		int j, k;
		for (j = 0, k = len - 1; j < l.keyNum && k >= 0; j++, k--)
		{
			if (!isdigit(numStr[k])) {
				fprintf(stderr, "%s is invalid\n", numStr);
				exit(1);
			}

			l.r[i].keys[j] = numStr[k];
		}

		while (j < l.keyNum) l.r[i].keys[j++] = '0';

		l.r[i].content = numStr;
	}

	radix_sort(&l);

	for (int i = 1; i <= l.recNum; i++)
	{
		printf("%s ", l.r[i].content);		
	}
	printf("\n");
}