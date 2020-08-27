#include <stdlib.h>
#include <stdio.h>

#include "losertree.h"

#define MINKEY (-(1<<31))
#define MAXKEY ((1<<31) - 1)

typedef struct {
	KeyType *keys;
	int		len;
	int		cur;
} Segment;

void
k_merge(Segment segs[], int n)
{
	PLoserTree 	plt;
	KeyType 	*keys;
	int 		i;

	plt = LoserTree_Create(n);

	keys = (KeyType *)malloc(sizeof(KeyType) * n);

	for (i = 0; i < n; ++i)
	{
		keys[i] = segs[i].keys[segs[i].cur];
	}

	i = LoserTree_Init(plt, keys, MINKEY);

	while (segs[i].cur < segs[i].len)
	{
		printf("%d:%d\n", i, segs[i].keys[segs[i].cur++]);

		if (segs[i].cur < segs[i].len)
		{
			i = LoserTree_Update(plt, segs[i].keys[segs[i].cur]);
		}
		else
		{
			i = LoserTree_Update(plt, MAXKEY);
		}
	}
}

int
main(int n, const char **args)
{
	const int k = 3;

	Segment segs[k];

	segs[0].len = 3; segs[0].cur = 0;
	segs[0].keys = (KeyType *)malloc(sizeof(KeyType) * 3);
	segs[0].keys[0] = 1; segs[0].keys[1] = 4; segs[0].keys[2] = 7;

	segs[1].len = 3; segs[1].cur = 0;
	segs[1].keys = (KeyType *)malloc(sizeof(KeyType) * 3);
	segs[1].keys[0] = 2; segs[1].keys[1] = 5; segs[1].keys[2] = 8;

	segs[2].len = 3; segs[2].cur = 0;
	segs[2].keys = (KeyType *)malloc(sizeof(KeyType) * 3);
	segs[2].keys[0] = 3; segs[2].keys[1] = 6; segs[2].keys[2] = 9;

	k_merge(segs, k);

	return 0;
}