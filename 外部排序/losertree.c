#include <stdlib.h>
#include <stdio.h>
#include "losertree.h"

int 
LoserTree_Adjust(PLoserTree t, int i)
{
	int p;
	p = (i + t->k) / 2;
	while(p > 0) {
		if (t->exNodes[i] > t->exNodes[t->itNodes[p]]) {
			int tmp = t->itNodes[p];
			t->itNodes[p] = i;
			i = tmp;
		}

		p = p / 2;
	}

	t->itNodes[0] = i;

	return i;
}

PLoserTree
LoserTree_Create(int k)
{
	PLoserTree plt;

	plt = (PLoserTree) malloc(sizeof(LoserTree));

	plt->itNodes = (int *) malloc(sizeof(int) * k);

	plt->exNodes = (KeyType *) malloc(sizeof(KeyType) * (k+1));

	plt->k = k;

	return plt;
}

int
LoserTree_Init(PLoserTree t, KeyType exKeys[], KeyType minKey)
{
	int i;
	t->exNodes[t->k] = minKey;
	for (i = 0; i < t->k; ++i)
	{
		t->itNodes[i] = t->k;
		t->exNodes[i] = exKeys[i];
	}

	for (i = t->k-1; i >= 0; --i)
		LoserTree_Adjust(t, i);

	return t->itNodes[0];
}

int
LoserTree_Update(PLoserTree t, KeyType key)
{
	// printf("--- %d %d\n", t->itNodes[0], key);

	t->exNodes[t->itNodes[0]] = key;
	return LoserTree_Adjust(t, t->itNodes[0]);
}