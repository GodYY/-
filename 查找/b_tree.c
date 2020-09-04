#define m 3							// B-树的阶

typedef struct BTNode {
	int 			keynum;			// 结点关键字个数，不会超过 m-1
	struct BTNode 	*parent;		// 双亲结点
	KeyType 		key[m+1];		// 关键字向量，0号单元未用
	struct BTNode 	*ptr[m+1];		// 子树指针向量
	Record 			*recptr[m+1];	// 记录指针向量，0号单元未用
} BTNode, *BTree;					// 

typedef struct {
	BTNode	*pt;					// 指向找到的结点
	int 	i;						// 1..m-1，在节点中的关键字序号
	int 	tag;					// 1:查找成功，0:查找失败
} Result;

Result btree_search(BTree t, KeyType k) {
	struct BTNode *p, q;
	int i;

	p = t; q = NULL; i = 0;

	while (p) 
	{
		i = btree_search_key(p, k);				// 查找关键字

		if (i > 0 && p->key[i] == k) break;		// 匹配成功
		else { q = p; p = p->ptr[i]; }			// 下跳分支继续查找
	}

	if (!p) return {p, i, 1};					// 查找成功，返回结点和关键字序号
	else return {q, i, 0};						// 查找失败，返回 k 的插入位置信息
}

// 采用二分查找在结点中查找关键字，返回关键字序号
int btree_search_key(struct BTNode *p, KeyType k)
{
	int low, high, mid;

	low = 1, high = p->keynum;

	while (low <= high) {
		mid = (low + high) / 2;

		if (k < p->key[mid]) high = mid - 1;
		else if (k == p->key[mid]) return mid;
		else low = mid + 1;
	}

	return high;
}

// 在 B-树 t 上结点 q 的 key[i] 与 key[i+1] 之间插入关键字 k。
// 若引起结点过大，则沿双亲链进行必要的结点分裂调整。
int btree_insert(BTree &t, KeyType k, BTree q, int i)
{
	KeyType x;
	struct BTNode *ap;
	bool finished;
	int s;

	x = k; ap = NULL; finished = false;

	while (q && !finished)
	{
		btnode_insert(q, i, x, ap);				// 将 x 和 ap 分别插入到 q->key[i+1] 和 q->ptr[i+1]
		if (q->keynum < m) finished = true;		// 插入完成
		else
		{
			s = ceil(m/2); split(q, s, ap); x = q->key[s];
			// 将 q->key[s+1..m], q->ptr[s..m] 和 q->recptr[s+1..m] 移入新结点 ap
			q = q->parent;
			if (q) i = btree_search_key(q, x);	// 在双亲结点中查找 x 的插入位置
		}
	}

	if (!finished) 								// t 是空树或者根结点已分裂为结点 q 和 ap
		newroot(t, q, x, ap);					// 生成含信息 (1,t,x,ap) 的新根结点 t，原 t 和 ap 为子树指针
	return 1;
}

// 将关键字 k 插入到 p->key[i+1], 将 ap 插入到 p->ptr[i+1]
void btnode_insert(struct BTNode *p, int i, KeyType k, struct BTNode *ap)
{
	int j;

	for (j = p->keynum; j > i; --j)
	{
		p->key[j+1] = p->key[j];
		p->ptr[j+1] = p->ptr[j];
	}

	p->key[i+1] = k;
	p->ptr[i+1] = ap;
	p->keynum += 1;
}

// 将 p 分裂为左（1, i-1)，右（i+1,m) 两个分支
void btree_split(BTree p, int i, BTree *ap)
{
	int j, k;

	j = 0;
	ap->ptr[j++] = p->ptr[i];

	for (k = i+1; k <= p->keynum; ++k, ++j)
	{
		ap->key[j] = p->key[k];
		ap->ptr[j] = p->ptr[k]
	}

	ap->keynum = p->keynum - i;
	ap->parent = p->parent;

	p->keynum = i-1;
}

void btree_new(BTree &t, BTree l, KeyType k, BTree r)
{
	t = (BTree) malloc(sizeof(struct BTNode));

	t->parent = NULL;
	t->keynum = 1;
	t->key[1] = k;
	t->ptr[0] = l; t->ptr[1] = r;
}
