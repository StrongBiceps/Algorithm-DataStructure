#include<iostream>
#include <vector>
#include <queue>
using namespace std;

int traverse_count{ 0 };

// B Tree�� ����� Ű�� �ڽ��� ���ĵ� ������ �����ϱ� ������, Ű ���� + 1���� �ڽ� ������ ������.

// BTree node
class BTreeNode
{
	int* keys; // Ű �迭
	int t;	 // �ּ� ���� ( Ű ������ ������.)
	BTreeNode** C; // �ڽ� ���鿡 ���� ������
	int n;	 // ��尡 ������ ���� Ű ���� ( ��� ���� �ִ� 2^t - 1 ���� Ű�� ���� �� �ִ�, ���� �ּ� t-1 ���� Ű�� ������ �Ѵ�.)
	bool leaf; // ���� ��� ����
public:
	BTreeNode(int _t, bool _leaf); // ������

	// ���� ��带 ��Ʈ�� ���� ���� Ʈ���� ���ο� Ű�� �����ϴ� �Լ�,
	// �� �Լ��� ȣ��Ǿ��� ��, ���� ���� ���� �ʾƾ� �Ѵ�.
	void insertNonFull(int k);

	// ���� ����� �ڽ��� y ��带 split�ϴ� �Լ�
	// i�� �ڽ� �迭�� C[]�� �ε����̴�.
	// �� �Լ��� ȣ��� ��, y�� �ݵ�� ���� �� �־�� �Ѵ�.
	void splitChild(int i, BTreeNode* y);

	// ���� ��带 ��Ʈ�� ���� ���� Ʈ���� ��ȸ�ϴ� �Լ�
	void traverse();

	// ���� ��带 ��Ʈ�� ���� ���� Ʈ������ k�� �˻��ϴ� �Լ�
	// ���� k�� �������� ������, NULL�� ��ȯ.
	BTreeNode* search(int k);

	// ���� ��带 ��Ʈ�� ���� ���� Ʈ������ k�� �����ϴ� �Լ�
	void remove(int k);

	// k���� ���ų� ū ���� ó�� Ű�� �ε����� ��ȯ�ϴ� �Լ�
	int findKey(int k);

	// �ܺ� ����� ���� ��忡�� idx �ε����� ���� Ű�� �����ϴ� �Լ�
	void removeFromLeaf(int idx);

	// ���� ����� ���� ��忡�� idx �ε����� ���� Ű�� �����ϴ� �Լ�
	void removeFromNonLeaf(int idx);

	// ���� ����� idx ��° Ű�� ������ Ű�� ��ȯ�ϴ� �Լ�
	int getPred(int idx);

	// ���� ����� idx ��° Ű�� �ļ��� Ű�� ��ȯ�ϴ� �Լ�
	int getSucc(int idx);

	// ���� ����� idx ��° �ڽ� ��尡 t-1���� Ű���� ���� ������ �ִٸ� �ش� ����� Ű�� ä��� �Լ�
	void fill(int idx);

	// Ű�� C[idx-1] ��忡�� �����ͼ� C[idx] ��忡 �����ϴ� �Լ�
	void borrowFromPrev(int idx);

	// Ű�� C[idx+1] ��忡�� �����ͼ� C[idx] ��忡 �����ϴ� �Լ�
	void borrowFromNext(int idx);

	// C[idx+1] ���� C[idx] ��带 �����ϴ� �Լ�
	void merge(int idx);

	// ���� ����� Ű�� ���.
	void		printKeys()
	{
		for (size_t i = 0; i < n; ++i)
		{
			cout << keys[i] << " " << endl;
		}
	}

	// �ڽ� ��� ������ �迭�� ��ȯ.
	BTreeNode** GetC()
	{
		return C;
	}

	// B Tree Ŭ�������� B Tree Node Ŭ������ Private ����� ������ �� �ְ� �ϱ� ����.
	friend class BTree;
};

// B Tree Ŭ����
class BTree
{
	BTreeNode* root; // ��Ʈ ��带 ����Ű�� ������
	int t; // �ּ� ����
public:
	// ������
	BTree(int _t)
	{
		root = NULL;
		t = _t;
	}

	// Ʈ���� ��ȸ�ϴ� �Լ�
	void traverse()
	{
		if (root != NULL)
			root->traverse();
	}

	// Ʈ������ k�� �˻��ϴ� �Լ�
	BTreeNode* search(int k)
	{
		return (root == NULL) ? NULL : root->search(k);
	}

	// Ʈ���� ���ο� Ű k�� �����ϴ� �Լ�
	void insert(int k);

	// Root�� ��ȯ�ϴ� �Լ�
	BTreeNode* GetRoot()
	{
		return root;
	}

	// B Tree���� Ű k�� �����ϴ� �Լ�
	void remove(int k);
};

BTreeNode::BTreeNode(int t1, bool leaf1)
{
	t = t1;
	leaf = leaf1;

	keys = new int[2 * t - 1];

	//�ּ� ������ 3�� B Tree�� 6���� �ڽ��� ���� �� �ִ�.
	C = new BTreeNode * [2 * t];

	n = 0;
}

void BTreeNode::traverse()
{
	for (int i = 0; i < traverse_count; ++i)
	{
		cout << '\t';
	}

	for (int i = 0; i < n; i++)
	{
		cout << " " << keys[i];
	}

	cout << endl;

	if (leaf == false)
	{
		++traverse_count;
	}

	for (int i = 0; i <= n; ++i)
	{
		if (leaf == false)
			C[i]->traverse();
	}

	if (leaf == false)
	{
		--traverse_count;
	}


	cout << endl;
}

int BTreeNode::findKey(int k)
{
	int idx = 0;
	while (idx < n && keys[idx] < k)
		++idx;
	return idx;
}

BTreeNode* BTreeNode::search(int k)
{
	int i = 0;
	while (i < n && k > keys[i])
		i++;

	if (keys[i] == k)
		return this;

	if (leaf == true)
		return NULL;

	return C[i]->search(k);
}

void BTreeNode::remove(int k)
{
	int idx = findKey(k);

	// ���� ��忡 �����Ϸ��� Ű�� ������ ��
	if (idx < n && keys[idx] == k)
	{
		// ���� ��尡 ���� ����� ��, - removeFromLeaf �Լ��� ȣ��ȴ�.
		// ���� ��尡 ���� �����, - removeFromNonLeaf �Լ��� ȣ��ȴ�.

		if (leaf)
			removeFromLeaf(idx);
		else
			removeFromNonLeaf(idx);
	}
	else
	{
		// ���� �� ��尡 ���� �����, k�� �������� �ʴ´�.
		if (leaf)
		{
			cout << "The key " << k << " is does not exist in the tree\n";
			return;
		}

		//���� ����� ���� ������ �ڽĿ� ������ Ű�� �����ִ��� Ȯ���ϴ� flag
		bool flag = ((idx == n) ? true : false);

		// ���� C[idx]�� Ű�� t���� �۴ٸ�, Ű�� ä���ش�.
		if (C[idx]->n < t)
			fill(idx);

		// ���� ���� ������ �ڽ��� ���յǾ��� ���� idx-1���� �����ϴ� ���� �´�.
		// ���� �ڽĵ��� ���յǸ� ���� ����� Ű ���� �ϳ� �پ���.
		if (flag && idx > n)
			C[idx - 1]->remove(k);
		else
			C[idx]->remove(k);
	}
	return;
}

// �ܼ��� �ܺ� ����� Ű�� �����ϴ� �Լ��̴�.
// �Ʒ� �ڵ�� ���������� ������ �� ���� ���̴�.
void BTreeNode::removeFromLeaf(int idx)
{
	// idx �ε����� idx+1 ������ ���� �����.
	for (int i = idx + 1; i < n; ++i)
		keys[i - 1] = keys[i];

	// Ű ������ �ϳ� ���ҽ�Ų��.
	n--;

	return;
}

void BTreeNode::removeFromNonLeaf(int idx)
{
	int k = keys[idx];

	// C[idx]�� Ű�� �ּ� t�� �̻� ������ ��, k�� ��ü�� ������ Ű�� ã�´�.
	// �׸��� �ش� Ű�� ������ Ű�� ��ȯ�ϰ� ��������� C[idx]���� ���� �Լ��� ȣ���Ѵ�.
	if (C[idx]->n >= t)
	{
		int pred = getPred(idx);
		keys[idx] = pred;
		C[idx]->remove(pred);
	}

	// ���� �����ϴ� �ε����� �ڽ��� Ű ���� �ʹ� ���ٸ�, ���� �ڽ����� �Ѿ�� ���� ���� ��带 ��ü�ڷ� �ٲ��ְ� �� �ڵ�� �Ȱ��� �����Ѵ�.
	else if (C[idx + 1]->n >= t)
	{
		int succ = getSucc(idx);
		keys[idx] = succ;
		C[idx + 1]->remove(succ);
	}

	// ���� C[idx]�� C[idx+1] ��� t���� ���� Ű�� ������ �ִٸ�, C[idx+1]�� k�� C[idx]�� �����Ѵ�.
	// ���� C[idx]�� 2*t-1 ���� Ű�� ���� ���̴�.
	// C[idx]���� �ٽ� k�� �����Ѵ�.
	else
	{
		merge(idx);
		C[idx]->remove(k);
	}
	return;
}

int BTreeNode::getPred(int idx)
{
	// �ܺ� ��带 ���� �� ���� ��������, �ܺ� ��忡���� ���� �����ʿ� �ִ� Ű�� ��ȯ�Ѵ�.
	BTreeNode* cur = C[idx];

	while (!cur->leaf)
		cur = cur->C[cur->n];

	return cur->keys[cur->n - 1];
}

int BTreeNode::getSucc(int idx)
{

	// Keep moving the left most node starting from C[idx+1] until we reach a leaf
	BTreeNode* cur = C[idx + 1];
	while (!cur->leaf)
		cur = cur->C[0];

	// Return the first key of the leaf
	return cur->keys[0];
}

void BTreeNode::fill(int idx)
{
	// ���ʿ� �ִ� �ڽ��� Ű�� t�� �̻��̶��, Ű�� �����´�.
	if (idx != 0 && C[idx - 1]->n >= t)
		borrowFromPrev(idx);

	// ������ �ִ� �ڽ��� Ű�� t�� �̻��̶��, Ű�� �����´�.
	else if (idx != n && C[idx + 1]->n >= t)
		borrowFromNext(idx);

	// C[idx]�� ���� ���� �����Ѵ�. ���� C[idx]�� ������ �����, ���� ���� �����Ѵ�.
	else
	{
		if (idx != n)
			merge(idx);
		else
			merge(idx - 1);
	}
	return;
}

void BTreeNode::borrowFromPrev(int idx)
{
	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx - 1];

	// C[idx-1]�� ������ Ű�� �θ�� �ö󰡰�, key[idx-1]Ű�� C[idx]�� ù ��°�� ��������.
	// �׷��Ƿ�, ���� ���� Ű �ϳ��� �Ұ�, child ���� Ű �ϳ��� ��´�.

	// C[idx]�� ��� Ű�� ���������� �� ĭ�� �̵���Ų��.
	for (int i = child->n - 1; i >= 0; --i)
		child->keys[i + 1] = child->keys[i];

	// ���� C[idx]�� ���� ��尡 �ƴ϶��, �ڽĵ��� ���������� �� ĭ�� �̵���Ų��.
	// B Tree�� ����� �ڽ� ����, Ű + 1�̴�. ������ B Tree�� split�ϰ� Ű�� �θ�� �� ���� �ø��� ������ ���� ������ �� �ִ�.
	// ���� C�迭�� out of range�� ������ ����.
	if (!child->leaf)
	{
		for (int i = child->n; i >= 0; --i)
			child->C[i + 1] = child->C[i];
	}

	// �θ� Ű�� �ڽ����� ������. �� �� child�� ��� Ű���� ���� �θ��� keys[idx-1]�� ������ ��� �񱳰� ��Ȯ�ϴ�.
	child->keys[0] = keys[idx - 1];

	// Moving sibling's last child as C[idx]'s first child
	// ����� Ű�� �ϳ� �پ���, Ű ���� + 1�� �ڽ� ���� �����ؾ� �ϹǷ� �ڽĵ� �ϳ� �پ���� �Ѵ�.
	// ���� Ű�� �������� ������ ����, Ű�� �ϳ� �þ����Ƿ� �ڽĵ� �ϳ� �þ�� �Ѵ�.
	// ���� ������ �ڽ��� �����;� ��� �񱳰� �´�.
	if (!child->leaf)
		child->C[0] = sibling->C[sibling->n];

	// ��������� ���� ������ Ű�� �θ� ���� �ø���.
	keys[idx - 1] = sibling->keys[sibling->n - 1];

	child->n += 1;
	sibling->n -= 1;

	return;
}

void BTreeNode::borrowFromNext(int idx)
{
	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx + 1];

	// child�� �θ� ����� keys[idx]�� �����Ѵ�.
	// �θ� ����� keys[idx]���� sibling�� ���� ���� Ű�� ���Ե� ���̴�.
	child->keys[(child->n)] = keys[idx];

	//�������Լ� ���Ҹ� �����Ա� ������ �ڽĵ� �ϳ� �����;� �Ѵ�. child�� ���Ұ� �þ����Ƿ� �ڽĵ� �þ�� �ϰ�,
	//������ ���Ұ� �پ����Ƿ� �ڽĵ� �پ�� �Ѵ�.
	if (!(child->leaf))
		child->C[(child->n) + 1] = sibling->C[0];

	//Ű�� ������ ������ ù ��° Ű�� �ٲ۴�.
	keys[idx] = sibling->keys[0];

	// ��� Ű�� �������� �� ĭ�� �о �� ĭ�� �����ش�.
	for (int i = 1; i < sibling->n; ++i)
		sibling->keys[i - 1] = sibling->keys[i];

	// ���� ���� �ڽ��� �پ����Ƿ� �Ű��ش�.
	if (!sibling->leaf)
	{
		for (int i = 1; i <= sibling->n; ++i)
			sibling->C[i - 1] = sibling->C[i];
	}

	child->n += 1;
	sibling->n -= 1;

	return;
}

void BTreeNode::merge(int idx)
{
	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx + 1];

	// ���� ����� idx��° Ű�� child ����� ���� ������ Ű�� �����Ѵ�. (���� �� child�� sibling�� t-1���� Ű�� ������)
	child->keys[t - 1] = keys[idx];

	// C[idx+1]�� Ű�� C[idx]�� ���������� �ű��.
	for (int i = 0; i < sibling->n; ++i)
		child->keys[i + t] = sibling->keys[i];

	//�� �ڵ���� ���� �ڽ� - �θ� - ������ �ڽ� ���� �̷���� ��带 �����ϴ� �����̴�.

	// C[idx+1]�� �ڽ��� C[idx]�� �ڽ����� �ű��. ������ �ڽĵ��� �Ű��ش�.
	if (!child->leaf)
	{
		for (int i = 0; i <= sibling->n; ++i)
			child->C[i + t] = sibling->C[i];
	}

	// keys[idx]�� C[idx]�� �Ű����Ƿ� �� �ڸ��� ä���.
	for (int i = idx + 1; i < n; ++i)
		keys[i - 1] = keys[i];

	// C[idx+1] ������ �ڽĵ��� �������� �� ĭ�� �о C[idx]�� �����.
	for (int i = idx + 2; i <= n; ++i)
		C[i - 1] = C[i];

	// �ڽ� ���� ���� ����� Ű ������ ������Ʈ�Ѵ�.
	child->n += sibling->n + 1;
	n--;

	delete(sibling);
	return;
}

void BTree::remove(int k)
{
	if (!root)
	{
		cout << "The tree is empty\n";
		return;
	}

	root->remove(k);

	// ��Ʈ ����� Ű�� 0�����,
	// ��Ʈ ����� �ڽ��� �����Ѵٸ�, �ڽ��� ���� �ø���, �ƴϸ� ��Ʈ ��带 NULL�� �����Ѵ�.
	if (root->n == 0)
	{
		BTreeNode* tmp = root;
		if (root->leaf)
			root = NULL;
		else
			root = root->C[0];

		delete tmp;
	}
	return;
}

void BTree::insert(int k)
{
	// Ʈ���� ����� ��,
	if (root == NULL)
	{
		// ��Ʈ�� ���ؼ� �޸𸮸� �Ҵ��Ѵ�.
		root = new BTreeNode(t, true);
		root->keys[0] = k; // Ű ����
		root->n = 1; // ��Ʈ ����� Ű ������ ������Ʈ
	}
	else // ���� Ʈ���� ������� �ʴٸ�,
	{
		// ��Ʈ ����� Ű�� ���� á���� ���� ������ �����Ѵ�. (Ʈ���� ���̰� Ŀ��)
		if (root->n == 2 * t - 1)
		{
			// ���ο� ��Ʈ�� ���ؼ� �޸𸮸� �Ҵ��Ѵ�.
			BTreeNode* s = new BTreeNode(t, false);

			// ���ο� ��Ʈ s�� ù ��° �ڽ��� ������ root�� �����.
			s->C[0] = root;

			// ������ root�� �����ϰ�, 1���� Ű�� root�� �ѱ��.
			s->splitChild(0, root);

			// ���ο� ��Ʈ�� ���� 2���� Ű�� ������. �� �ڽ� �߿��� � �ڽ��� ���ο� Ű�� ������ �Ǵ��Ѵ�.
			int i = 0;
			// ���ο� ��Ʈ�� Ű ���� ���ο� Ű�� ũ�ٸ�, ������ �ڽĿ� �����Ѵ�.
			if (s->keys[0] < k)
				i++;
			s->C[i]->insertNonFull(k);

			// ��Ʈ�� �����Ѵ�.
			root = s;
		}
		else // ��Ʈ�� ���� ���� �ʾҴٸ�, 
			root->insertNonFull(k);
	}
}

void BTreeNode::insertNonFull(int k)
{
	// ���� ����� ���� ū ������ �ε����� i�� �����Ѵ�.
	int i = n - 1;

	// ���� ���� ��尡 �ܺ� �����,
	if (leaf == true)
	{
		// ���� ������ �� ���� ����� �����Ѵ�.
		// a) ���ο� Ű�� ���ԵǾ�� �� ��ġ�� ã�´�.
		// b) ���ο� Ű���� ���� ū Ű���� �ڷ� �� ĭ�� �о��ش�.

		// ���� ���� ������� Ű�� �����Ѵ�.
		while (i >= 0 && keys[i] > k)
		{
			keys[i + 1] = keys[i];
			i--;
		}

		// ã�� ��ġ�� ���ο� Ű�� �����ϰ� Ű ������ ������Ų��.
		keys[i + 1] = k;
		n = n + 1;
	}
	else // ���� ���� ��尡 ���� �����,
	{
		// �ڽĵ� �߿��� � ��尡 ���ο� Ű�� ������ �Ǵ��Ѵ�.
		while (i >= 0 && keys[i] > k)
			i--;

		// �ش� �ڽ��� ���� á���� �Ǵ��Ѵ�.
		if (C[i + 1]->n == 2 * t - 1)
		{
			// �ڽ��� ���� á�ٸ�, �����Ѵ�.
			splitChild(i + 1, C[i + 1]);

			// ���� ��, C[i+1]�� �� ���� ���� ���ҵǾ� C[i + 1]�� �߰� ��ġ Ű�� ���� ���� �ö�� ���̴�.

			// ������ �Ŀ� ������ Ű�� �ڽĵ� �߿��� ��� �ڽĿ� ���� �ϴ��� �ε��� i�� �����ϴ� �ڵ��̴�.
			if (keys[i + 1] < k)
				i++;
		}
		C[i + 1]->insertNonFull(k);
	}
}

void BTreeNode::splitChild(int i, BTreeNode* y)
{
	// y�� Ű �߿��� t-1���� ������ ���ο� ��带 �����.
	BTreeNode* z = new BTreeNode(y->t, y->leaf);

	// t = 3�� ���� ��带 �����ϸ� ���ҵ� ���� 2���� Ű�� ������ �ȴ�.
	z->n = t - 1;

	// ����� Ű�� 2����Ͽ� �����ϱ� ���� �ڵ�
	// y�� �� �κ� Ű t-1���� z�� �Ҵ��Ѵ�.
	for (int j = 0; j < t - 1; j++)
		z->keys[j] = y->keys[j + t];

	// y�� �� �κ� 3���� �ڽ��� z�� �� �κ� 3���� �ڽ����� �����Ѵ�.
	// ���� t�� 3�̸�, 3,4,5 ��° �ڽ��� z�� 0,1,2 ��° �ڽ����� �����Ѵ�.
	if (y->leaf == false)
	{
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j + t];
	}

	// ������ �� �ش� ����� ���� ���� ���δ�.
	// y�� ���ҵǾ��ٸ� 5�� ���� �߿��� 1���� �θ�� �ö󰡰� ������ 2���� y,z�� �ȴ�.
	y->n = t - 1;

	// ���� ��尡 ���ο� �ڽ��� ���� ���̱� ������, ���ο� ��带 ���� ������ Ȯ���Ѵ�.
	for (int j = n; j >= i + 1; j--)
		C[j + 1] = C[j];

	// y�� �����Ͽ� ������ z ��带 i + 1 �ε����� �����Ѵ�.
	// ���� ���Ŀ��� �ڸ��� �о��ִ� �Ͱ� �����ϴ�.
	C[i + 1] = z;

	// ���ο� Ű�� ��ġ�� Ȯ���ϱ� ���ؼ� ���ο� Ű���� ū Ű���� �ڷ� �� ĭ�� �о��ش�.

	// ���� ���� ����� Ű�� 2���̰�, i�� 0�̶��,
	// 0,1 ��° Ű�� 1,2��° ��ġ�� �̵��ϰ�, 0��° ��ġ�� ���ο� Ű�� ���Եȴ�.
	// Ű�� 2���̰�, i�� 1�̶��, �߰� ��ġ�� ���Ե� ���̴�.
	// 0��° �ڽ��� ���ҵȴٸ�, ���� ����� Ű �߿��� ���� ���� ������ ���� �ϹǷ� �ڸ��� Ȯ�����ִ� ���̴�.
	for (int j = n - 1; j >= i; j--)
		keys[j + 1] = keys[j];

	// y�� Ű���� �߰� ��ġ�� �ִ� Ű�� ���� ���� �ű��.
	keys[i] = y->keys[t - 1];

	// ���� ����� Ű ������ ������Ų��.
	n = n + 1;
}

int main()
{
	//BTree t(3);
	/*t.insert(10);
	t.insert(20);
	t.insert(5);
	t.insert(6);
	t.insert(12);
	t.insert(30);
	t.insert(7);
	t.insert(17);*/

	//5 6 10 12 20
	BTree t(3);

	t.insert(1);
	t.insert(2);
	t.insert(3);
	t.insert(4);
	t.insert(5);
	t.insert(6);
	t.insert(7);
	t.insert(8);
	t.insert(9);
	t.insert(10);
	t.insert(11);
	t.insert(12);
	t.insert(13);
	t.insert(14);
	t.insert(15);
	t.insert(16);
	t.insert(17);
	t.insert(18);
	t.insert(19);
	t.insert(20);
	t.insert(21);

	cout << "Traversal of tree constructed is\n";
	t.traverse();
	cout << endl;

	traverse_count = 0;

	t.remove(9);
	cout << "Traversal of tree after removing 9\n";
	t.traverse();
	cout << endl;

	traverse_count = 0;

	t.remove(10);
	cout << "Traversal of tree after removing 10\n";
	t.traverse();
	cout << endl;

	traverse_count = 0;

	return 0;
}