typedef struct {
	KeyType 	key;					// 关键字项
	InfoType	otherinfo;				// 其他数据项
} ElemType;

typedef struct BSTNode {
	ElemType		data;				// 数据项
	int 			bf;					// 平衡因子
	struct BSTNode 	*lchild, *rchild;	// 左右子树
} BSTNode, *BSTree;

// 右旋
void r_rotate(BSTree &p) 
{
	struct BSTNode *lc;
	lc = p->lchild;						// lc 指向 p 的左子树
	p->lchild = lc->rchild;				// lc 右子树挂接为 p 的左子树
	lc->rchild = p; p = lc;				// p 挂机位 lc 的右子树, 更新 p
}

// 左旋
void l_rotate(BSTree &p) {
	struct BSTNode *rc;
	rc = p->rchild;						// rc 指向 p 的右子树
	p->rchild = rc->lchild;				// lc 左子树挂接为 p 的右子树
	rc->lchild = p;	p = rc;				// p 挂机位 lc 的左子树, 更新 p
}

#define LH +1							// 左高
#define EH 0							// 等高
#define RH -1							// 右高

int bbt_insert(BSTree &t, ElemType e, bool &taller)
{
	if (!t) 
	{
		// 树为空，直接作为树根
		t = (BSTree)malloc(sizeof(BSTNode)); t->data = e;
		t->lchild = t->rchild = NULL; T->bf = EH; taller = true;
	} 
	else 
	{
		if (e.key == t->data.key)
		{
			// 关键字与树根相同，插入失败
			taller = false; return 0;
		}
		else if (e.key < t->data.key)
		{
			// 关键字小于树根，插入左子树
			if (!bbt_insert(t->lchild, e, taller)) return 0;
			if (taller) 
			{
				switch(t->bf)
				{
				case LH:
					// 插入前左子树比右子树高，左平衡处理
					// 处理后树高度不变
					left_balance(T); taller = false; break;
					break;

				case EH:
					// 插入前左、右子树等高，根左倾，树变高
					t->bf = LH; taller = true; break;

				case RH:
					// 插入前右子树比左子树高，根等高，树高不变
					t->bf = EH; taller = false; break;
				}
			}
		} 
		else 
		{
			// 关键字大于树根，插入右子树
			if (!bbt_insert(t->rchild, e, taller)) return 0;
			if (taller) 
			{
				switch(t->bf)
				{
				case LH:
					// 插入前左子树比右子树高，根等高，树高不变
					t->bf = EH; taller = false; break;

				case EH:
					// 插入前左、右子树等高，根右倾，树变高	
					t->bf = RH; taller = true; break;

				case RH:
					// 插入前右子树比左子树高，右平衡处理
					// 处理后树高不变
					right_balance(t); taller = false; break;
				}
			}
		}
	}

	return 1;
}

// 左分支平衡，左分支比右分支高 H(L) = H(R) + 2
void left_balance(BSTree &t)
{
	struct BSTNode *lc, *rd; 

	lc = t->lchild;

	switch (lc->bf)
	{
	case LH:
		// 新增结点在左分支的左分支上，右旋
		t->bf = lc->bf = EH; r_rotate(t); break;

	case RH:
		// 新增结点在左分支的右分支上，先左旋再右旋
		rd = lc->rchild;

		// 调整平衡因子
		switch (rd->bf)
		{
		case LH:
			// 新增结点在左分支的右分支的左分支上
			// 平衡后原树根右倾，原左分支等高
			t->bf = RH; lc->bf = EH; break;

		case EH:
			// 左分支原本无分支，新增结点成为左分支的右分支
			// 平衡后原树根和左分支均等高
			t->bf = lc->bf = EH; break;

		case RH:
			// 新增结点在左分支的右分支的右分支上
			// 平衡后原树根等高，原左分支左倾
			t->bf = EH; lc->bf = LH; break;
		}

		// rd 作为平衡后的树根，等高
		rd->bf = EH;
		// 先左旋再右旋
		l_rotate(t->lchild);
		r_rotate(t);
		break;
	}
}

// 右分支平衡，右分支比左分支高 H(R) = H(L) + 2
// 所有逻辑均与 left_balance 对称
void right_balance(BSTree &t)
{
	struct BSTNode *rc, *ld; 

	rc = t->rchild;

	switch (rc->bf)
	{
	case LH:
		// 新增结点在右分支的左分支上，逻辑与在左分支的右分支上对称
		ld = rc->lchild;

		switch (ld->bf)
		{
		case LH:
			// 新增结点在右分支的左分支的左分支上，逻辑与在左分支的右分支的右分支上对称
			// 平衡后原树根等高，原右分支右倾
			t->bf = EH; rc->bf = RH; break;

		case EH:
			// 右分支原本无分支，新增结点成为右分支的左分支，与 left_balance 对称
			// 平衡后原树根和右分支均等高
			t->bf = rc->bf = EH; break;

		case RH:
			// 新增结点在右分支的左分支的右分支上，逻辑与在左分支的右分支的左分支上对称
			// 平衡后原树根左倾，原右分支等高
			t->bf = LH; rc->bf = EH; break;
		}

		// 右分支的左分支根结点成为新的树根，等高
		ld->bf = EH;
		// 先右旋再左旋
		r_rotate(t->rchild);
		l_rotate(t);
		break;

	case RH:
		// 新增结点在右分支的右分支上，左旋，逻辑与在左分支的左分支上对称
		t->bf = rc->bf = EH; l_rotate(t); break;
	}
}