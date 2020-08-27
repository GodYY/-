#ifndef __LOSERTREE_H__
#define __LOSERTREE_H__

typedef struct 
{
	int		*itNodes;	// 内部节点，数量为 k，树的 k-1 个非终端节点加上用来标示胜者的树根的父节点。
	KeyType *exNodes;	// 外部节点，数量为 k+1，标示对应归并段中最小的关键字。exNodes[k] 存储 MINKEY。
	int 	k;			// 标示为 k 路归并。
} LoserTree, *PLoserTree;

PLoserTree
LoserTree_Create(int k);

int
LoserTree_Init(PLoserTree t, KeyType exKeys[], KeyType minKey);

void
LoserTree_Destroy(PLoserTree plt);

int
LoserTree_Update(PLoserTree t, KeyType key);

#endif