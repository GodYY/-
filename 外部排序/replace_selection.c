#include <stdlib.h>
#include <stdio.h>

#define MAXKEY 2147483647
#define w 6

typedef int LoserTree[w];

typedef int KeyType;

typedef union 
{
	KeyType key;
	int 	value;
} RcdType;

static RcdType RUNEND_SYMBOL = {0};

typedef struct 
{
	RcdType rec;
	KeyType key;
	int 	rnum;
} RcdNode, WorkArea[w];

void construct_loser(LoserTree ls, WorkArea wa, FILE *fi);

void select_minimax(LoserTree ls, WorkArea wa, int q);

void get_run(LoserTree ls, WorkArea wa, int rc, int *rmax, FILE *fi, FILE *fo);

void replace_selection(LoserTree ls, WorkArea wa, FILE *fi, FILE *fo)
{
	int rc, rmax;

	construct_loser(ls, wa, fi);

	rc = rmax = 1;

	while (rc <= rmax) {
		get_run(ls, wa, rc, &rmax, fi, fo);
		fwrite(&RUNEND_SYMBOL, sizeof(RcdType), 1, fo);
		rc = wa[ls[0]].rnum;
	}
}

// 获得一个初始归并段
void get_run(LoserTree ls, WorkArea wa, int rc, int *rmax, FILE *fi, FILE *fo)
{
	KeyType minimax;
	int q;

	while (wa[ls[0]].rnum == rc) {
		q = ls[0];
		minimax = wa[q].key;

		fwrite(&wa[q].rec, sizeof(RcdType), 1, fo);

		if (feof(fi)) {
			wa[q].rnum = *rmax + 1;
			wa[q].key = MAXKEY;
		} else {
			fread(&wa[q].rec, sizeof(RcdType), 1, fi);
			wa[q].key = wa[q].rec.key;
			if (wa[q].key < minimax) {
				*rmax = rc + 1;
				wa[q].rnum = *rmax;
			} else {
				wa[q].rnum = rc;
			}
		}

		select_minimax(ls, wa, q);
	}
}

void select_minimax(LoserTree ls, WorkArea wa, int q)
{
	for (int t = (w+q) / 2, p=ls[t]; t > 0; t=t/2, p=ls[t])
	{
		if (wa[p].rnum < wa[q].rnum || wa[p].rnum == wa[q].rnum && wa[p].key < wa[q].key)
		{
			ls[t] = q;
			q = p;
		}
	}
	ls[0] = q;
}

void construct_loser(LoserTree ls, WorkArea wa, FILE *fi)
{
	int i;

	for (i = 0; i < w; ++i)
	{
		wa[i].rnum = wa[i].key = ls[i] = 0;
	}

	for (i = w - 1; i >= 0; --i)
	{
		fread(&wa[i].rec, sizeof(RcdType), 1, fi);
		wa[i].key = wa[i].rec.key;
		wa[i].rnum = 1;
		select_minimax(ls, wa, i);
	}
}

int
main(int n, const char **args)
{
	return 0;
}