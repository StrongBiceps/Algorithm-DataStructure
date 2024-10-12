#include<iostream>
#include <vector>
#include <queue>
using namespace std;

int traverse_count{ 0 };

// A BTree node
class BTreeNode
{
	int* keys; // 키 배열
	int t;	 // 최소 차수 ( 키 개수를 정의함.)
	BTreeNode** C; // 자식 노드들에 대한 포인터
	int n;	 // 노드가 보유한 현재 키 개수 ( 모든 노드는 최대 2^t - 1 개의 키를 가질 수 있다, 또한 최소 t-1 개의 키를 가져야 한다.)
	bool leaf; // 리프 노드 여부
public:
	BTreeNode(int _t, bool _leaf); // 생성자

	// 현재 노드를 루트로 갖는 서브 트리에 새로운 키를 삽입하는 함수,
	// 이 함수가 호출되었을 때, 노드는 가득 차지 않아야 한다.
	void insertNonFull(int k);

	// 현재 노드의 자식인 y 노드를 split하는 함수
	// i는 자식 배열인 C[]의 인덱스이다.
	// 이 함수가 호출될 때, y는 반드시 가득 차 있어야 한다.
	void splitChild(int i, BTreeNode* y);

	// 현재 노드를 루트로 갖는 서브 트리를 순회하는 함수
	void traverse();

	// 현재 노드를 루트로 갖는 서브 트리에서 k를 검색하는 함수
	// 만약 k가 존재하지 않으면, NULL을 반환.
	BTreeNode* search(int k);

	// 현재 노드를 루트로 갖는 서브 트리에서 k를 삭제하는 함수
	void remove(int k);

	// k보다 같거나 큰 가장 처음 키의 인덱스를 반환하는 함수
	int findKey(int k);

	// 외부 노드인 현재 노드에서 idx 인덱스를 갖는 키를 제거하는 함수
	void removeFromLeaf(int idx);

	// 내부 노드인 현재 노드에서 idx 인덱스를 갖는 키를 제거하는 함수
	void removeFromNonLeaf(int idx);

	// 현재 노드의 idx 번째 키의 전임자 키를 반환하는 함수
	int getPred(int idx);

	// 현재 노드의 idx 번째 키의 후속자 키를 반환하는 함수
	int getSucc(int idx);

	// 현재 노드의 idx 번째 자식 노드가 t-1개의 키보다 적게 가지고 있다면 해당 노드의 키를 채우는 함수
	void fill(int idx);

	// 키를 C[idx-1] 노드에게 빌려와서 C[idx] 노드에 삽입하는 함수
	void borrowFromPrev(int idx);

	// 키를 C[idx+1] 노드에게 빌려와서 C[idx] 노드에 삽입하는 함수
	void borrowFromNext(int idx);

	// C[idx+1] 노드와 C[idx] 노드를 병합하는 함수
	void merge(int idx);

	// 현재 노드의 키를 출력.
	void		printKeys()
	{
		for (size_t i = 0; i < n; ++i)
		{
			cout << keys[i] << " " << endl;
		}
	}

	// 자식 노드 포인터 배열을 반환.
	BTreeNode** GetC()
	{
		return C;
	}

	// B Tree 클래스에서 B Tree Node 클래스의 Private 멤버에 접근할 수 있게 하기 위함.
	friend class BTree;
};

// B Tree 클래스
class BTree
{
	BTreeNode* root; // 루트 노드를 가리키는 포인터
	int t; // 최소 차수
public:
	// 생성자
	BTree(int _t)
	{
		root = NULL;
		t = _t;
	}

	// 트리를 순회하는 함수
	void traverse()
	{
		if (root != NULL)
			root->traverse();
	}

	// 트리에서 k를 검색하는 함수
	BTreeNode* search(int k)
	{
		return (root == NULL) ? NULL : root->search(k);
	}

	// 트리에 새로운 키 k를 삽입하는 함수
	void insert(int k);

	// Root를 반환하는 함수
	BTreeNode* GetRoot()
	{
		return root;
	}

	// B Tree에서 키 k를 삭제하는 함수
	void remove(int k);
};

BTreeNode::BTreeNode(int t1, bool leaf1)
{
	t = t1;
	leaf = leaf1;

	keys = new int[2 * t - 1];

	//최소 차수가 3인 B Tree는 6개의 자식을 가질 수 있다.
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

	// 현재 노드에 삭제하려는 키가 존재할 때
	if (idx < n && keys[idx] == k)
	{
		// 현재 노드가 리프 노드일 때, - removeFromLeaf 함수가 호출된다.
		// 현재 노드가 내부 노드라면, - removeFromNonLeaf 함수가 호출된다.

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

		//현재 노드의 가장 마지막 자식에 삭제할 키가 속해있는지 확인하는 flag
		bool flag = ((idx == n) ? true : false);

		// If the child where the key is supposed to exist has less that t keys,
		// we fill that child

		//삭제할 키가 속해있을 자식이 t보다 키가 적을 경우, 예를 들어 3차 트리인데 키의 개수가 2개보다 적을 경우
		//해당 자식을 fill해준다.
		//왜냐하면 C[idx]의 서브트리에서 원소가 하나 줄어들면, B 트리 속성에 부합하지 않기 때문에 미리 병합해준다.
		//예를 들어 C[idx]의 키가 1개이고 자식이 2개인데 C[idx]를 삭제하려면 자식들이 병합되어야 하고 병합하는 과정에서
		//C[idx] 노드가 사라지게 되기 때문이다. 따라서 미리 병합시켜 주는 것이다.
		if (C[idx]->n < t)
			fill(idx);

		// If the last child has been merged, it must have merged with the previous
		// child and so we recurse on the (idx-1)th child. Else, we recurse on the
		// (idx)th child which now has atleast t keys
		//만약 가장 마지막 자식이 병합되었을 때는 idx-1에서 삭제하는 것이 맞다.
		//또한 자식들이 병합되면 현재 노드의 키 수가 하나 줄어든다.
		if (flag && idx > n)
			C[idx - 1]->remove(k);
		else
			C[idx]->remove(k);
	}
	return;
}

// 단순히 외부 노드의 키를 삭제하는 함수이다.
// 아래 코드는 직관적으로 이해할 수 있을 것이다.
void BTreeNode::removeFromLeaf(int idx)
{
	// idx 인덱스를 idx+1 값으로 덮어 씌운다.
	for (int i = idx + 1; i < n; ++i)
		keys[i - 1] = keys[i];

	// 키 개수를 하나 감소시킨다.
	n--;

	return;
}

void BTreeNode::removeFromNonLeaf(int idx)
{
	int k = keys[idx];

	// C[idx]의 키가 최소 t개 이상 존재할 때, k를 대체할 전임자 키를 찾는다.
	// 그리고 해당 키와 삭제할 키를 교환하고 재귀적으로 C[idx]에서 삭제 함수를 호출한다.
	if (C[idx]->n >= t)
	{
		int pred = getPred(idx);
		keys[idx] = pred;
		C[idx]->remove(pred);
	}

	// 만약 대응하는 인덱스의 자식의 키 값이 너무 적다면, 다음 자식으로 넘어가서 가장 왼쪽 노드를 대체자로 바꿔주고 위 코드와 똑같이 삭제한다.
	else if (C[idx + 1]->n >= t)
	{
		int succ = getSucc(idx);
		keys[idx] = succ;
		C[idx + 1]->remove(succ);
	}

	// 만약 C[idx]와 C[idx+1] 모두 t보다 적은 키를 가지고 있다면, C[idx+1]과 k를 C[idx]에 병합한다.
	// 이제 C[idx]는 2*t-1 개의 키를 가질 것이다.
	// C[idx]에서 다시 k를 삭제한다.
	else
	{
		merge(idx);
		C[idx]->remove(k);
	}
	return;
}

int BTreeNode::getPred(int idx)
{
	// 외부 노드를 만날 때 까지 내려가고, 외부 노드에서의 가장 오른쪽에 있는 키를 반환한다.
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
	//왼쪽에 있는 형제가 키가 충분할 경우 키를 빌려온다.
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
	// 부모의 가장 오른쪽 키로 child의 가장 왼쪽 키를 설정한다.
	child->keys[0] = keys[idx - 1];

	// Moving sibling's last child as C[idx]'s first child
	//형제에게서 원소를 빌렸을 때 형제는 자식 하나가 감소해야만 한다. 따라서 아래 코드를 실행한다.
	//child의 가장 왼쪽 원소에 형제의 가장 오른쪽 원소를 넣으므로 원소의 대소 비교 원칙은 정확하다.
	if (!child->leaf)
		child->C[0] = sibling->C[sibling->n];

	// Moving the key from the sibling to the parent
	// This reduces the number of keys in the sibling
	//형제노드의 가장 마지막 키를 부모 노드의 가장 오른쪽 노드로 설정한다.
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
	//child의 가장 오른쪽 키를 부모 노드의 키로 바꾼다.
	child->keys[(child->n)] = keys[idx];

	// Sibling's first child is inserted as the last child
	// into C[idx]
	//형제에게서 원소를 빌려왔기 때문에 자식도 하나 가져와야 한다. child는 원소가 늘었으므로 자식도 늘어야 하고,
	//형제는 원소가 줄었으므로 자식도 줄어야 한다.
	if (!(child->leaf))
		child->C[(child->n) + 1] = sibling->C[0];

	//The first key from sibling is inserted into keys[idx]
	//키는 오른쪽 형제의 첫 번째 키로 바꾼다.
	keys[idx] = sibling->keys[0];

	// Moving all keys in sibling one step behind
	for (int i = 1; i < sibling->n; ++i)
		sibling->keys[i - 1] = sibling->keys[i];

	// Moving the child pointers one step behind
	//가장 왼쪽 자식이 줄었으므로 옮겨준다.
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

	// 현재 노드의 idx번째 키를 child 노드의 가장 오른쪽 키로 삽입한다. (병합 시 child와 sibling은 t-1개의 키를 가진다)
	child->keys[t - 1] = keys[idx];

	// C[idx+1]의 키를 C[idx]의 오른쪽으로 옮긴다.
	for (int i = 0; i < sibling->n; ++i)
		child->keys[i + t] = sibling->keys[i];

	//위 코드들은 왼쪽 자식 - 부모 - 오른쪽 자식 으로 이루어진 노드를 생성하는 과정이다.

	// C[idx+1]의 자식을 C[idx]의 자식으로 옮긴다. 병합할 자식들을 옮겨준다.
	if (!child->leaf)
	{
		for (int i = 0; i <= sibling->n; ++i)
			child->C[i + t] = sibling->C[i];
	}

	// keys[idx]를 C[idx]로 옮겼으므로 빈 자리를 채운다.
	for (int i = idx + 1; i < n; ++i)
		keys[i - 1] = keys[i];

	// C[idx+1] 이후의 자식들을 왼쪽으로 한 칸씩 밀어서 C[idx]를 지운다.
	for (int i = idx + 2; i <= n; ++i)
		C[i - 1] = C[i];

	// 자식 노드와 현재 노드의 키 개수를 업데이트한다.
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
	// 트리가 비었을 때,
	if (root == NULL)
	{
		// 루트를 위해서 메모리를 할당한다.
		root = new BTreeNode(t, true);
		root->keys[0] = k; // 키 삽입
		root->n = 1; // 루트 노드의 키 개수를 업데이트
	}
	else // 만약 트리가 비어있지 않다면,
	{
		// 루트 노드의 키가 가득 찼으면 분할 연산을 진행한다. (트리의 높이가 커짐)
		if (root->n == 2 * t - 1)
		{
			// 새로운 루트를 위해서 메모리를 할당한다.
			BTreeNode* s = new BTreeNode(t, false);

			// 새로운 루트 s의 첫 번째 자식을 기존의 root로 만든다.
			s->C[0] = root;

			// 기존의 root를 분할하고, 1개의 키를 root로 넘긴다.
			s->splitChild(0, root);

			// 새로운 루트는 이제 2개의 키를 가진다. 두 자식 중에서 어떤 자식이 새로운 키를 가질지 판단한다.
			int i = 0;
			// 새로운 루트의 키 보다 새로운 키가 크다면, 오른쪽 자식에 삽입한다.
			if (s->keys[0] < k)
				i++;
			s->C[i]->insertNonFull(k);

			// 루트를 변경한다.
			root = s;
		}
		else // 루트가 가득 차지 않았다면, 
			root->insertNonFull(k);
	}
}

void BTreeNode::insertNonFull(int k)
{
	// 현재 노드의 가장 큰 원소의 인덱스로 i를 설정한다.
	int i = n - 1;

	// 만약 현재 노드가 외부 노드라면,
	if (leaf == true)
	{
		// 다음 루프는 두 가지 기능을 수행한다.
		// a) 새로운 키가 삽입되어야 할 위치를 찾는다.
		// b) 새로운 키보다 값이 큰 키들을 뒤로 한 칸씩 밀어준다.

		// 삽입 정렬 방식으로 키를 삽입한다.
		while (i >= 0 && keys[i] > k)
		{
			keys[i + 1] = keys[i];
			i--;
		}

		// 찾은 위치에 새로운 키를 삽입하고 키 개수를 증가시킨다.
		keys[i + 1] = k;
		n = n + 1;
	}
	else // 만약 현재 노드가 내부 노드라면,
	{
		// 자식들 중에서 어떤 노드가 새로운 키를 가질지 판단한다.
		while (i >= 0 && keys[i] > k)
			i--;

		// 해당 자식이 가득 찼는지 판단한다.
		if (C[i + 1]->n == 2 * t - 1)
		{
			// 자식이 가득 찼다면, 분할한다.
			splitChild(i + 1, C[i + 1]);

			// 분할 후, C[i+1]은 두 개의 노드로 분할되어 C[i + 1]의 중간 위치 키는 현재 노드로 올라올 것이다.

			// 분할한 후에 삽입할 키가 자식들 중에서 어느 자식에 들어가야 하는지 인덱스 i를 조정하는 코드이다.
			if (keys[i + 1] < k)
				i++;
		}
		C[i + 1]->insertNonFull(k);
	}
}

void BTreeNode::splitChild(int i, BTreeNode* y)
{
	// y의 키 중에서 t-1개를 소유할 새로운 노드를 만든다.
	BTreeNode* z = new BTreeNode(y->t, y->leaf);

	// t = 3일 때는 노드를 분할하면 분할된 노드는 2개의 키를 가지게 된다.
	z->n = t - 1;

	// 노드의 키를 2등분하여 분할하기 위한 코드
	// y의 뒷 부분 키 t-1개를 z에 할당한다.
	for (int j = 0; j < t - 1; j++)
		z->keys[j] = y->keys[j + t];

	// y의 뒷 부분 3개의 자식을 z의 앞 부분 3개의 자식으로 설정한다.
	// 만약 t가 3이면, 3,4,5 번째 자식을 z의 0,1,2 번째 자식으로 설정한다.
	if (y->leaf == false)
	{
		for (int j = 0; j < t; j++)
			z->C[j] = y->C[j + t];
	}

	// 분할한 후 해당 노드의 원소 수를 줄인다.
	// y가 분할되었다면 5개 원소 중에서 1개가 부모로 올라가고 나머지 2개씩 y,z가 된다.
	y->n = t - 1;

	// 현재 노드가 새로운 자식을 가질 것이기 때문에, 새로운 노드를 위한 공간을 확보한다.
	for (int j = n; j >= i + 1; j--)
		C[j + 1] = C[j];

	// y를 분할하여 생성된 z 노드를 i + 1 인덱스에 연결한다.
	// 삽입 정렬에서 자리를 밀어주는 것과 동일하다.
	C[i + 1] = z;

	// 새로운 키의 위치를 확보하기 위해서 새로운 키보다 큰 키들을 뒤로 한 칸씩 밀어준다.

	// 만약 현재 노드의 키가 2개이고, i가 0이라면,
	// 0,1 번째 키가 1,2번째 위치로 이동하고, 0번째 위치에 새로운 키가 삽입된다.
	// 키가 2개이고, i가 1이라면, 중간 위치에 삽입될 것이다.
	// 0번째 자식이 분할된다면, 현재 노드의 키 중에서 가장 작은 값으로 들어가야 하므로 자리를 확보해주는 것이다.
	for (int j = n - 1; j >= i; j--)
		keys[j + 1] = keys[j];

	// y의 키들의 중간 위치에 있는 키를 현재 노드로 옮긴다.
	keys[i] = y->keys[t - 1];

	// 현재 노드의 키 개수를 증가시킨다.
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