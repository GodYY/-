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

// 在 B-树 t 上删除结点 p 的，第 i 个关键字
int btree_delete(BTree &t, struct BTNode *p, int i)
{
	struct BTNode *q;
	int s, j;

	// 用右邻分支中最左下的关键字替代被删除关键字
	for (q = p->ptr[i]; q->ptr[0]; q = q->ptr[0]);

	if (q) {
		// 复制关键字
		p->key[i] = q->key[1];
		// 调整为删除最下层结点中的关键字
		p = q;
		i = 1;
	}

	s = ceil(m/2);

	// 删除关键字
	for (; i < p->keynum; ++i)
			p->key[i] = p->key[i+1];
	p->keynum -= 1;

	while (p->keynum < s - 1 && p != t)
	{
		q = p->parent;

		// 在根结点中定位结点位置
		for (i = 0; i <= q->keynum && p != q->ptr[i]; ++i);

		if (i == 0) 
		{
			// p 为最左边的孩子

			if (q->ptr[1]->keynum >= s)
			{
				// 将 q->ptr[1] 的最小关键字上移至根结点，将 q->key[1] 下移至 p。
				// 并将 q->ptr[1] 中最左侧的分支转移至 p 中。
				btree_moveleft(q, 1);
			}
			else 
			{
				// 合并 p、q->key[i+1]、q->ptr[i+1]。
				btree_merge(q, 1);
			}
		}
		else if (i == q->keynum)
		{
			// p 为最右边的孩子

			if (q-ptr[i-1]->keynum >= s)
			{
				// 将 q->ptr[i-1] 的最大关键字上移至根结点，将 q->key[i] 下移至 p。
				// 并将 q->ptr[i-1] 中最右侧分支转移至 p 中。
				btree_moveright(q, i);
			}
			else
			{
				// 合并 p、q->key[i]、q->ptr[i-1]。
				btree_merge(q, i);
			}
		}
		else 
		{
			// 结点不处于两端

			if (q->ptr[i-1]->keynum >= s)
			{
				// 将 q->ptr[i-1] 的最大关键字上移至根结点，将 q->key[i] 下移至 p。
				// 并将 q->ptr[i-1] 中最右侧分支转移至 p 中。
				btree_moveright(q, i);
			}
			else if (q->ptr[i+1]->keynum >= s)
			{
				// 将 q->ptr[i+1] 的最小关键字上移至根结点，将 q->key[i+1] 下移至 p。
				// 并将 q->ptr[i+1] 中最左侧的分支转移至 p 中。
				btree_moveleft(q, i+1);
			}
			else
			{
				// 合并 p、q->key[i]、q->ptr[i-1]。
				btree_merge(q, i);
			}
		}

		p = q;
	}

	if (t->keynum == 0)
	{
		// 根结点被合并到没有关键字，整棵树下降1层。
		// 因为合并是向左兄弟合并的关系，所以 ptr[0]
		// 要么是合并后的分支，要么为空。
		q = t; t = t->ptr[0]; free(q);
	}
}

// 将 t->ptr[i] 的最小关键字上移至 t，并将 t->key[i] 下移至 t->ptr[i-1]
// ，将 t->ptr[i]->ptr[0]，转移至 t->ptr[i-1] 中。
void btree_moveleft(BTree t, int i)
{
	BTree p;

	p = t->ptr[i-1];

	// 根结点关键字下移至左兄弟结点
	p->keynum++;
	p->key[p->keynum] = t->key[i];
	p->ptr[p->keynum] = t->ptr[i]->ptr[0];

	// 右兄弟最小关键字上移至根结点
	p = t->ptr[i];
	t->key[i] = p->key[1];
	p->ptr[0] = p->ptr[1];
	for (i = 1; i < p->keynum; ++i)
	{
		p->key[i] = p->key[i+1];
		p->ptr[i] = p->ptr[i+1];
	}
	p->keynum--;
}

// 将 t->ptr[i] 的最小关键字上移至 t，并将 t->key[i] 下移至 t->ptr[i-1]
// ，将 t->ptr[i]->ptr[0]，转移至 t->ptr[i-1] 中。
void btree_moveright(BTree t, int i)
{
	int j;
	BTree p;

	// 关键字指针右移，为从上层来的关键字和右移过来的分支留出空间。
	p = t->ptr[i];
	p->keynum++;
	for (j = p->keynum; j > 1; --j)
	{
		p->key[j] = p->key[j-1];
		p->ptr[j] = p->ptr[j-1];
	}
	p->ptr[1] = p->ptr[0];

	// 根结点关键字下移至右兄弟
	p->key[1] = t->key[i];

	// 将左兄弟结点的最后一个关键字上移至根结点。
	p = t->ptr[i-1];
	t->key[i] = p->key[i];
	t->ptr[0] = p->ptr[p->keynum];
	p->keynun--;
}

// 合并 t->key[i], t->ptr[i] 中所有信息到 t->ptr[i-1] 中。
void btree_merge(BTree t, int i)
{
	int j;
	BTree l, r;

	l = t->ptr[i-1];
	r = t->ptr[i];

	// 根结点关键字下移至左兄弟
	l->keynum++;
	l->key[l->keynum] = t->key[i];
	l->ptr[l->keynum] = r->ptr[0];

	// 右兄弟终端信息转移到左兄弟中
	for (j = 1; j <= r->keynum; ++j)
	{
		l->keynum++;
		l->key[l->keynum] = r->key[j];
		l->ptr[l->keynum] = r->ptr[j];
	}

	// 删除根结点中的关键字
	for (j = i; j < t->keynum; ++j)
	{
		t->key[j] = t->key[j+1];
		t->ptr[j] = t->ptr[j+1];
	}
	t->keynum--;

	free(r);
}

