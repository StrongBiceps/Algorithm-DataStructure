#include<iostream>
#include <vector>
#include <queue>
using namespace std;

int traverse_count{ 0 };

// A BTree node
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

// Function to search key k in subtree rooted with this node
BTreeNode* BTreeNode::search(int k)
{
	// Find the first key greater than or equal to k
	int i = 0;
	while (i < n && k > keys[i])
		i++;

	// If the found key is equal to k, return this node
	if (keys[i] == k)
		return this;

	// If key is not found here and this is a leaf node
	if (leaf == true)
		return NULL;

	// Go to the appropriate child
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
		// If this node is a leaf node, then the key is not present in tree
		if (leaf)
		{
			cout << "The key " << k << " is does not exist in the tree\n";
			return;
		}

		// The key to be removed is present in the sub-tree rooted with this node
		// The flag indicates whether the key is present in the sub-tree rooted
		// with the last child of this node

		//���� ����� ���� ������ �ڽĿ� ������ Ű�� �����ִ��� Ȯ���ϴ� flag
		bool flag = ((idx == n) ? true : false);

		// If the child where the key is supposed to exist has less that t keys,
		// we fill that child

		//������ Ű�� �������� �ڽ��� t���� Ű�� ���� ���, ���� ��� 3�� Ʈ���ε� Ű�� ������ 2������ ���� ���
		//�ش� �ڽ��� fill���ش�.
		//�ֳ��ϸ� C[idx]�� ����Ʈ������ ���Ұ� �ϳ� �پ���, B Ʈ�� �Ӽ��� �������� �ʱ� ������ �̸� �������ش�.
		//���� ��� C[idx]�� Ű�� 1���̰� �ڽ��� 2���ε� C[idx]�� �����Ϸ��� �ڽĵ��� ���յǾ�� �ϰ� �����ϴ� ��������
		//C[idx] ��尡 ������� �Ǳ� �����̴�. ���� �̸� ���ս��� �ִ� ���̴�.
		if (C[idx]->n < t)
			fill(idx);

		// If the last child has been merged, it must have merged with the previous
		// child and so we recurse on the (idx-1)th child. Else, we recurse on the
		// (idx)th child which now has atleast t keys
		//���� ���� ������ �ڽ��� ���յǾ��� ���� idx-1���� �����ϴ� ���� �´�.
		//���� �ڽĵ��� ���յǸ� ���� ����� Ű ���� �ϳ� �پ���.
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

// A function to fill child C[idx] which has less than t-1 keys
void BTreeNode::fill(int idx)
{

	// If the previous child(C[idx-1]) has more than t-1 keys, borrow a key
	// from that child
	//���ʿ� �ִ� ������ Ű�� ����� ��� Ű�� �����´�.
	if (idx != 0 && C[idx - 1]->n >= t)
		borrowFromPrev(idx);

	// If the next child(C[idx+1]) has more than t-1 keys, borrow a key
	// from that child
	else if (idx != n && C[idx + 1]->n >= t)
		borrowFromNext(idx);

	// Merge C[idx] with its sibling
	// If C[idx] is the last child, merge it with its previous sibling
	// Otherwise merge it with its next sibling
	else
	{
		if (idx != n)
			merge(idx);
		else
			merge(idx - 1);
	}
	return;
}

// A function to borrow a key from C[idx-1] and insert it
// into C[idx]
void BTreeNode::borrowFromPrev(int idx)
{

	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx - 1];

	// The last key from C[idx-1] goes up to the parent and key[idx-1]
	// from parent is inserted as the first key in C[idx]. Thus, the  loses
	// sibling one key and child gains one key

	// Moving all key in C[idx] one step ahead
	for (int i = child->n - 1; i >= 0; --i)
		child->keys[i + 1] = child->keys[i];

	// If C[idx] is not a leaf, move all its child pointers one step ahead
	if (!child->leaf)
	{
		for (int i = child->n; i >= 0; --i)
			child->C[i + 1] = child->C[i];
	}

	// Setting child's first key equal to keys[idx-1] from the current node
	// �θ��� ���� ������ Ű�� child�� ���� ���� Ű�� �����Ѵ�.
	child->keys[0] = keys[idx - 1];

	// Moving sibling's last child as C[idx]'s first child
	//�������Լ� ���Ҹ� ������ �� ������ �ڽ� �ϳ��� �����ؾ߸� �Ѵ�. ���� �Ʒ� �ڵ带 �����Ѵ�.
	//child�� ���� ���� ���ҿ� ������ ���� ������ ���Ҹ� �����Ƿ� ������ ��� �� ��Ģ�� ��Ȯ�ϴ�.
	if (!child->leaf)
		child->C[0] = sibling->C[sibling->n];

	// Moving the key from the sibling to the parent
	// This reduces the number of keys in the sibling
	//��������� ���� ������ Ű�� �θ� ����� ���� ������ ���� �����Ѵ�.
	keys[idx - 1] = sibling->keys[sibling->n - 1];

	child->n += 1;
	sibling->n -= 1;

	return;
}

// A function to borrow a key from the C[idx+1] and place
// it in C[idx]
void BTreeNode::borrowFromNext(int idx)
{

	BTreeNode* child = C[idx];
	BTreeNode* sibling = C[idx + 1];

	// keys[idx] is inserted as the last key in C[idx]
	//child�� ���� ������ Ű�� �θ� ����� Ű�� �ٲ۴�.
	child->keys[(child->n)] = keys[idx];

	// Sibling's first child is inserted as the last child
	// into C[idx]
	//�������Լ� ���Ҹ� �����Ա� ������ �ڽĵ� �ϳ� �����;� �Ѵ�. child�� ���Ұ� �þ����Ƿ� �ڽĵ� �þ�� �ϰ�,
	//������ ���Ұ� �پ����Ƿ� �ڽĵ� �پ�� �Ѵ�.
	if (!(child->leaf))
		child->C[(child->n) + 1] = sibling->C[0];

	//The first key from sibling is inserted into keys[idx]
	//Ű�� ������ ������ ù ��° Ű�� �ٲ۴�.
	keys[idx] = sibling->keys[0];

	// Moving all keys in sibling one step behind
	for (int i = 1; i < sibling->n; ++i)
		sibling->keys[i - 1] = sibling->keys[i];

	// Moving the child pointers one step behind
	//���� ���� �ڽ��� �پ����Ƿ� �Ű��ش�.
	if (!sibling->leaf)
	{
		for (int i = 1; i <= sibling->n; ++i)
			sibling->C[i - 1] = sibling->C[i];
	}

	// Increasing and decreasing the key count of C[idx] and C[idx+1]
	// respectively
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

	// If the root node has 0 keys, make its first child as the new root
	//  if it has a child, otherwise set root as NULL
	if (root->n == 0)
	{
		BTreeNode* tmp = root;
		if (root->leaf)
			root = NULL;
		else
			root = root->C[0];

		// Free the old root
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