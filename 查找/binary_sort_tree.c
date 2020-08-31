typedef struct {
	KeyType 	key;		// 关键字项
	InfoType	otherinfo;	// 其他数据项
} ElemType;

typedef struct BSTNode {
	ElemType 		data;				// 数据域
	struct BSTNode 	*lchild, *rchild;	// 左右子树
} BSTNode, *BSTree;

// 查找目标结点, 平均查找长度与树的形态有关
BSTree BSTSearch(BSTree t, KeyType key)
{
	if ((!t) || key == t->data.key) return t;							// 树根为目标结点
	else if (key < t->data.key) return BSTSearch(t->lchild, key);		// 查找左子树
	else return BSTSearch(t->rchild, key);								// 查找右子树
}

// 插入结点
void BSTInsert(BSTree &t, ElemType e)
{
	if (!t)
	{
		// 树为空，将树根设置为插入结点

		struct BSTNode *s = (BSTNode*)malloc(sizeof(BSTNode));
		s->data = e;
		s->lchild = s->rchild = NULL;
		t = s;
	}
	else if (e.key < t->data.key)
		InsertBST(t->lchild, e);		// 插入左子树
	else if (e.key > t->data.key)
		InsertBST(t->rchild, e);		// 插入右子树
}

void BSTDelete(BSTree &t, KeyType key)
{
	struct BSTNode *p = t, *f = NULL, *q;

	while (p)
	{
		if (p->data.key == key) break;
		f = p;
		if (key < p->data.key) p = p->lchild;
		else p = p->rchild;
	}

	if (!p) return;										// 找不到结点直接返回

	if ((p->lchild) && (p->rchild))						// 被删除结点子树均不空
	{
		q = p; s = p->lchild;
		while (s->rchild)
		{
			q = s; s = s->rchild;						// 继续在待删除结点的左子树中寻找直接前驱
		}
		p->data = s->data;								// 将s移动到p
		if (q != p) q->rchild = s->lchild;				// 重接q的右子树
		else q->lchild = s->lchild;						// 重接q的左子树
		free(s);
		return		
	}
	else if (!p->rchild)
	{
		q = p; p = p->lchild;
	}
	else if (!p->lchild)
	{
		q = p; p = p->rchild;
	}

	if (!f) t = p;										// 被删除结点为根
	else if (q == f->lchild) f->lchild = p;				// 挂接到f的左子树
	else f->rchild = p;									// 挂接到f的右子树
	free(q);
}