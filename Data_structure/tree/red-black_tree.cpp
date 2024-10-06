#include <iostream>
#include <queue>
using namespace std;

enum COLOR { RED, BLACK };

class Node
{
public:
	int val;
	COLOR color;
	Node* left, * right, * parent;

	Node(int val) : val(val)
	{
		parent = left = right = NULL;

		//노드가 생성되면 항상 빨간색 노드로 설정된다.
		color = RED;
	}

	// 삼촌 노드의 포인터를 반환한다.
	Node* uncle()
	{
		// 만약, 부모가 없거나 조부모가 없으면 삼촌도 없다.
		if (parent == NULL || parent->parent == NULL)
			return NULL;

		if (parent->isOnLeft())
			// 삼촌 노드는 조부모의 오른쪽 자식이다.
			return parent->parent->right;
		else
			// 삼촌 노드는 조부모의 왼쪽 자식이다.
			return parent->parent->left;
	}

	// 해당 노드가 부모 노드의 왼쪽 자식인지 확인한다.
	bool isOnLeft()
	{
		return this == parent->left;
	}

	// 형제 노드의 포인터를 반환한다.
	Node* sibling()
	{
		// 부모 노드가 없으면, 형제 노드도 없다.
		if (parent == NULL)
			return NULL;

		if (isOnLeft())
			return parent->right;

		return parent->left;
	}

	// 현재 노드를 아래로 내리고, 해당 위치에 새로운 노드를 삽입한다.
	void moveDown(Node* nParent)
	{
		if (parent != NULL)
		{
			if (isOnLeft())
			{
				parent->left = nParent;
			}
			else
			{
				parent->right = nParent;
			}
		}
		nParent->parent = parent;
		parent = nParent;
	}

	//빨간색 자식이 있는지 확인한다.
	bool hasRedChild()
	{
		return (left != NULL and left->color == RED) or
			(right != NULL and right->color == RED);
	}
};

class RBTree
{
	Node* root;

	// 왼쪽 회전.
	void leftRotate(Node* x)
	{
		// 새로운 부모 노드는 현재 노드의 오른쪽 노드가 될 것이다.
		Node* nParent = x->right;

		// 현재 노드가 root라면 root를 업데이트한다.
		if (x == root)
			root = nParent;

		//x의 높이를 한 칸 내리고, 해당 위치에 새로운 부모 노드를 삽입한다.
		x->moveDown(nParent);

		// 새로운 부모 노드의 왼쪽 자식을 현재 노드의 오른쪽 자식에 삽입한다.
		x->right = nParent->left;

		// 만약 새로운 부모 노드의 왼쪽 자식이 NULL이 아니라면, 부모를 현재 노드로 연결해준다.
		if (nParent->left != NULL)
			nParent->left->parent = x;

		// 새로운 부모 노드의 왼쪽 자식을 현재 노드로 설정한다.
		nParent->left = x;
	}

	void rightRotate(Node* x)
	{
		Node* nParent = x->left;

		if (x == root)
			root = nParent;

		x->moveDown(nParent);

		x->left = nParent->right;

		if (nParent->right != NULL)
			nParent->right->parent = x;

		nParent->right = x;
	}

	void swapColors(Node* x1, Node* x2)
	{
		COLOR temp;
		temp = x1->color;
		x1->color = x2->color;
		x2->color = temp;
	}

	void swapValues(Node* u, Node* v)
	{
		int temp;
		temp = u->val;
		u->val = v->val;
		v->val = temp;
	}

	// Red - Red 위반을 해결한다.
	void fixRedRed(Node* x)
	{
		// 만약 x가 root라면 색을 빨간색으로 바꾼다.
		if (x == root)
		{
			x->color = BLACK;
			return;
		}

		// 부모, 삼촌, 조부모 지역 변수를 초기화한다.
		Node* parent = x->parent, * grandparent = parent->parent,
			* uncle = x->uncle();

		// 부모의 색깔이 검정색이 아니면,
		if (parent->color != BLACK)
		{
			//만약 삼촌이 존재하고, 색깔이 빨간색이라면,
			if (uncle != NULL && uncle->color == RED)
			{
				//삼촌과 부모의 색깔을 검정색으로 다시 칠하고, 조부모를 빨간색으로 칠한 뒤 재귀 호출한다.
				parent->color = BLACK;
				uncle->color = BLACK;
				grandparent->color = RED;
				fixRedRed(grandparent);
			}
			else
			{
				// 삼촌이 존재하지 않거나, 검정색이라면 LR, LL, RL, RR 회전을 실행한다.
				if (parent->isOnLeft())
				{
					if (x->isOnLeft())
					{
						// LL Case 첫 번째 회전
						swapColors(parent, grandparent);
					}
					else
					{
						// LR Case 첫 번째 회전
						leftRotate(parent);
						swapColors(x, grandparent);
					}
					// 두 번째 회전
					rightRotate(grandparent);
				}
				else
				{
					if (x->isOnLeft())
					{
						// RL Case 첫 번째 회전
						rightRotate(parent);
						swapColors(x, grandparent);
					}
					else
					{
						// RR case 첫 번째 회전
						swapColors(parent, grandparent);
					}

					// 두 번째 회전
					leftRotate(grandparent);
				}
			}
		}
	}

	// 후속 노드 반환 함수.
	// 왼쪽 서브 트리의 가장 큰 값을 반환.
	// !중요 -> 오른쪽 서브 트리의 가장 작은 값을 반환하는 코드와 트리 구조가 차이가 있음.
	//			현재 코드는 왼쪽 서브 트리의 가장 오른쪽 노드를 반환.
	Node* successor(Node* x)
	{
		Node* temp = x;

		while (temp->right != NULL)
			temp = temp->right;

		return temp;
	}

	// BST에서 노드 삭제 시 후속 노드를 구하는 함수
	Node* BSTreplace(Node* x)
	{
		// 노드가 2개의 자식이 존재할 때,
		if (x->left != NULL and x->right != NULL)
			return successor(x->left);

		// 노드가 리프 노드일 때,
		if (x->left == NULL and x->right == NULL)
			return NULL;

		// 노드의 자식이 하나일 때,
		if (x->left != NULL)
			return x->left;
		else
			return x->right;
	}

	void deleteNode(Node* v)
	{
		//후속 노드를 반환한다.
		Node* u = BSTreplace(v);

		// u와 v가 모두 검정색이라면, true 아니면 false
		bool uvBlack = ((u == NULL || u->color == BLACK) && (v->color == BLACK));

		//삭제할 노드의 부모 노드
		Node* parent = v->parent;

		// 후속 노드가 없다면, v는 리프 노드이다.
		if (u == NULL)
		{
			if (v == root)
			{
				// v가 root라면, root를 삭제한다.
				root = NULL;
			}
			else
			{
				if (uvBlack)
				{
					// 후속 노드가 없고, v가 검은색일 경우 더블 블랙이다.
					// 후속 노드가 없고, 삭제할 노드가 검은색이라면 해당 서브 트리의 검은색 노드 개수가 줄어들어서
					// 더블 블랙이라고 칭하고 그 문제를 해결한다.
					fixDoubleBlack(v);
				}
				else
				{
					// v가 빨간색일 때,
					if (v->sibling() != NULL)
						// 형제 노드가 존재할 때, 빨간색으로 바꾼다.
						// 형제 노드가 검은색이라면, 검은색 노드 개수 규칙을 위반하므로 빨간색으로 바꾼다.
					{
						v->sibling()->color = RED;
					}
				}

				// v 노드를 트리에서 삭제한다.
				if (v->isOnLeft())
				{
					parent->left = NULL;
				}
				else
				{
					parent->right = NULL;
				}
			}
			delete v;
			return;
		}

		// 후속 노드가 존재하는 경우,
		// v의 자식이 하나만 존재한다면,
		if (v->left == NULL or v->right == NULL)
		{
			// v가 root라면,
			if (v == root)
			{
				// v와 u의 값을 바꾸고, u 노드를 삭제한다.
				v->val = u->val;
				v->left = v->right = NULL;
				delete u;
			}
			else
			{
				// v를 트리에서 분리하고, u를 v자리로 올린다.
				if (v->isOnLeft())
				{
					parent->left = u;
				}
				else
				{
					parent->right = u;
				}

				delete v;
				u->parent = parent;

				if (uvBlack)
				{
					// 후속 노드 u와 v가 검은색일 경우 더블 블랙이다.
					// 삭제할 노드와 후속 노드 모두 검은색이라면, 해당 서브 트리의 검은색 노드 개수가 하나 줄어들어서
					// 더블 블랙이라고 칭하고 그 문제를 해결한다.
					fixDoubleBlack(u);
				}
				else
				{
					// 둘 중 하나가 Red라면, 어떠한 경우에서든 Black의 개수가 맞아야 하므로 u를 Black으로 칠한다.
					u->color = BLACK;
				}
			}
			return;
		}

		// v가 2개의 자식이 존재한다면, 두 노드의 값을 바꾸고 재귀 호출한다.
		swapValues(u, v);
		deleteNode(u);
	}

	void fixDoubleBlack(Node* x)
	{
		if (x == root)
			return;

		Node* sibling = x->sibling();
		Node* parent = x->parent;

		if (sibling == NULL)
		{
			// 형제 노드가 없다면, 부모로 올라가서 더블 블랙을 해결한다.
			fixDoubleBlack(parent);
		}
		else
		{
			if (sibling->color == RED)
			{
				//형제가 빨간색이라면 회전을 수행해야 한다.
				parent->color = RED;
				sibling->color = BLACK;
				if (sibling->isOnLeft())
				{
					// L Case
					rightRotate(parent);
				}
				else
				{
					// R Case
					leftRotate(parent);
				}
				fixDoubleBlack(x);
			}
			else
			{
				// 형제 노드가 검은색일 경우,
				if (sibling->hasRedChild())
				{
					// 형제 노드왼쪽 자식이 빨간색, 오른쪽 자식이 없거나 검은색일 경우
					if (sibling->left != NULL && sibling->left->color == RED && (sibling->right == NULL || sibling->left->color != RED))
					{
						if (sibling->isOnLeft())
						{
							// LL Case
							sibling->left->color = sibling->color;
							sibling->color = parent->color;
							rightRotate(parent);
						}
						else
						{
							// RL Case
							// 부모 노드를 오른쪽 서브 트리의 가장 작은 값으로 만들어 주기 위해서 RL 회전을 한다.
							// LR 회전도 마찬가지이다. 왼쪽 서브 트리의 가장 큰 값을 부모로 만들어 줘서 BST의 성질을 유지한다.
							sibling->left->color = parent->color;
							rightRotate(sibling);
							leftRotate(parent);
						}
					}
					else
					{
						if (sibling->isOnLeft())
						{
							// LR Case
							sibling->right->color = parent->color;
							leftRotate(sibling);
							rightRotate(parent);
						}
						else
						{
							// RR Case

							// 왼쪽 서브 트리에서 검은색 노드 하나가 삭제되었으므로, 검은색 노드를 왼쪽으로 보내주기 위해 left rotation을 수행한다.
							// 또한 오른쪽 서브 트리는 검은색 노드를 하나 잃으므로 자식을 검은색으로 칠해서 하나 보충한다.
							// 나머지 Case들도 이러한 이유로 회전을 실행한다.
							sibling->right->color = sibling->color;
							sibling->color = parent->color;
							leftRotate(parent);
						}
					}
					parent->color = BLACK;
				}
				else
				{
					// 2개의 검은색 자식 노드가 있는 경우,
					sibling->color = RED;

					//부모 색깔이 빨간색이라면, 형제 노드가 잃은 검은색을 부모로 전파하면 되지만,
					//검은색이라면 검은색 개수가 하나 줄어들어서 다시 부모 쪽으로 가서 더블 블랙을 해결한다.
					if (parent->color == BLACK)
						fixDoubleBlack(parent);
					else
						parent->color = BLACK;
				}
			}
		}
	}

	void levelOrder(Node* x)
	{
		if (x == NULL)
			return;

		queue<Node*> q;
		Node* curr;

		q.push(x);

		while (!q.empty())
		{
			curr = q.front();
			q.pop();

			cout << curr->val << " ";

			if (curr->left != NULL)
				q.push(curr->left);
			if (curr->right != NULL)
				q.push(curr->right);
		}
	}

	void inorder(Node* x)
	{
		if (x == NULL)
			return;
		inorder(x->left);
		cout << x->val << " ";
		inorder(x->right);
	}

public:
	RBTree()
	{
		root = NULL;
	}

	Node* getRoot()
	{
		return root;
	}

	// 주어진 노드를 검색한다.
	// 노드가 발견되면 반환한다. (삭제에 사용된다.)
	// 노드가 발견되지 않는다면, 순회하는 과정에서 최근에 방문한 노드를 반환. (삽입에 사용된다.)
	Node* search(int n)
	{
		Node* temp = root;
		while (temp != NULL)
		{
			if (n < temp->val)
			{
				if (temp->left == NULL)
					break;
				else
					temp = temp->left;
			}
			else if (n == temp->val)
			{
				break;
			}
			else
			{
				if (temp->right == NULL)
					break;
				else
					temp = temp->right;
			}
		}

		return temp;
	}

	// 트리에 노드를 삽입한다.
	void insert(int n)
	{
		Node* newNode = new Node(n);
		if (root == NULL)
		{
			// root가 null이라면, root를 설정한다.
			newNode->color = BLACK;
			root = newNode;
		}
		else
		{
			Node* temp = search(n);

			if (temp->val == n)
			{
				// 만약 중복 노드가 존재한다면, 삽입을 중지한다.
				return;
			}

			// 만약 노드가 발견되지 않았다면, search함수는 새로운 노드가 삽입될
			// 위치를 반환할 것이다.

			newNode->parent = temp;

			if (n < temp->val)
				temp->left = newNode;
			else
				temp->right = newNode;

			// Red - Red 위반이 존재한다면, 고친다.
			fixRedRed(newNode);
		}
	}

	// 주어진 값을 가진 노드를 제거하는 함수
	void deleteByVal(int n)
	{
		if (root == NULL)
			// 트리가 비었을 때,
			return;

		Node* v = search(n);

		if (v->val != n)
		{
			cout << "No node found to delete with value:" << n << endl;
			return;
		}

		deleteNode(v);
	}

	void printInOrder()
	{
		cout << "Inorder: " << endl;
		if (root == NULL)
			cout << "Tree is empty" << endl;
		else
			inorder(root);
		cout << endl;
	}

	void printLevelOrder()
	{
		cout << "Level order: " << endl;
		if (root == NULL)
			cout << "Tree is empty" << endl;
		else
			levelOrder(root);
		cout << endl;
	}
};

int main() {
	RBTree tree;

	tree.insert(7);
	tree.insert(3);
	tree.insert(18);
	tree.insert(10);
	tree.insert(22);
	tree.insert(8);
	tree.insert(11);
	tree.insert(26);
	tree.insert(2);
	tree.insert(6);
	tree.insert(13);

	tree.printInOrder();
	tree.printLevelOrder();

	cout << endl << "Deleting 18, 11, 3, 10, 22" << endl;

	tree.deleteByVal(18);
	tree.deleteByVal(11);
	tree.deleteByVal(3);
	tree.deleteByVal(10);
	tree.deleteByVal(22);

	tree.printInOrder();
	tree.printLevelOrder();

	return 0;
}