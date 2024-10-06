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

		//��尡 �����Ǹ� �׻� ������ ���� �����ȴ�.
		color = RED;
	}

	// ���� ����� �����͸� ��ȯ�Ѵ�.
	Node* uncle()
	{
		// ����, �θ� ���ų� ���θ� ������ ���̵� ����.
		if (parent == NULL || parent->parent == NULL)
			return NULL;

		if (parent->isOnLeft())
			// ���� ���� ���θ��� ������ �ڽ��̴�.
			return parent->parent->right;
		else
			// ���� ���� ���θ��� ���� �ڽ��̴�.
			return parent->parent->left;
	}

	// �ش� ��尡 �θ� ����� ���� �ڽ����� Ȯ���Ѵ�.
	bool isOnLeft()
	{
		return this == parent->left;
	}

	// ���� ����� �����͸� ��ȯ�Ѵ�.
	Node* sibling()
	{
		// �θ� ��尡 ������, ���� ��嵵 ����.
		if (parent == NULL)
			return NULL;

		if (isOnLeft())
			return parent->right;

		return parent->left;
	}

	// ���� ��带 �Ʒ��� ������, �ش� ��ġ�� ���ο� ��带 �����Ѵ�.
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

	//������ �ڽ��� �ִ��� Ȯ���Ѵ�.
	bool hasRedChild()
	{
		return (left != NULL and left->color == RED) or
			(right != NULL and right->color == RED);
	}
};

class RBTree
{
	Node* root;

	// ���� ȸ��.
	void leftRotate(Node* x)
	{
		// ���ο� �θ� ���� ���� ����� ������ ��尡 �� ���̴�.
		Node* nParent = x->right;

		// ���� ��尡 root��� root�� ������Ʈ�Ѵ�.
		if (x == root)
			root = nParent;

		//x�� ���̸� �� ĭ ������, �ش� ��ġ�� ���ο� �θ� ��带 �����Ѵ�.
		x->moveDown(nParent);

		// ���ο� �θ� ����� ���� �ڽ��� ���� ����� ������ �ڽĿ� �����Ѵ�.
		x->right = nParent->left;

		// ���� ���ο� �θ� ����� ���� �ڽ��� NULL�� �ƴ϶��, �θ� ���� ���� �������ش�.
		if (nParent->left != NULL)
			nParent->left->parent = x;

		// ���ο� �θ� ����� ���� �ڽ��� ���� ���� �����Ѵ�.
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

	// Red - Red ������ �ذ��Ѵ�.
	void fixRedRed(Node* x)
	{
		// ���� x�� root��� ���� ���������� �ٲ۴�.
		if (x == root)
		{
			x->color = BLACK;
			return;
		}

		// �θ�, ����, ���θ� ���� ������ �ʱ�ȭ�Ѵ�.
		Node* parent = x->parent, * grandparent = parent->parent,
			* uncle = x->uncle();

		// �θ��� ������ �������� �ƴϸ�,
		if (parent->color != BLACK)
		{
			//���� ������ �����ϰ�, ������ �������̶��,
			if (uncle != NULL && uncle->color == RED)
			{
				//���̰� �θ��� ������ ���������� �ٽ� ĥ�ϰ�, ���θ� ���������� ĥ�� �� ��� ȣ���Ѵ�.
				parent->color = BLACK;
				uncle->color = BLACK;
				grandparent->color = RED;
				fixRedRed(grandparent);
			}
			else
			{
				// ������ �������� �ʰų�, �������̶�� LR, LL, RL, RR ȸ���� �����Ѵ�.
				if (parent->isOnLeft())
				{
					if (x->isOnLeft())
					{
						// LL Case ù ��° ȸ��
						swapColors(parent, grandparent);
					}
					else
					{
						// LR Case ù ��° ȸ��
						leftRotate(parent);
						swapColors(x, grandparent);
					}
					// �� ��° ȸ��
					rightRotate(grandparent);
				}
				else
				{
					if (x->isOnLeft())
					{
						// RL Case ù ��° ȸ��
						rightRotate(parent);
						swapColors(x, grandparent);
					}
					else
					{
						// RR case ù ��° ȸ��
						swapColors(parent, grandparent);
					}

					// �� ��° ȸ��
					leftRotate(grandparent);
				}
			}
		}
	}

	// �ļ� ��� ��ȯ �Լ�.
	// ���� ���� Ʈ���� ���� ū ���� ��ȯ.
	// !�߿� -> ������ ���� Ʈ���� ���� ���� ���� ��ȯ�ϴ� �ڵ�� Ʈ�� ������ ���̰� ����.
	//			���� �ڵ�� ���� ���� Ʈ���� ���� ������ ��带 ��ȯ.
	Node* successor(Node* x)
	{
		Node* temp = x;

		while (temp->right != NULL)
			temp = temp->right;

		return temp;
	}

	// BST���� ��� ���� �� �ļ� ��带 ���ϴ� �Լ�
	Node* BSTreplace(Node* x)
	{
		// ��尡 2���� �ڽ��� ������ ��,
		if (x->left != NULL and x->right != NULL)
			return successor(x->left);

		// ��尡 ���� ����� ��,
		if (x->left == NULL and x->right == NULL)
			return NULL;

		// ����� �ڽ��� �ϳ��� ��,
		if (x->left != NULL)
			return x->left;
		else
			return x->right;
	}

	void deleteNode(Node* v)
	{
		//�ļ� ��带 ��ȯ�Ѵ�.
		Node* u = BSTreplace(v);

		// u�� v�� ��� �������̶��, true �ƴϸ� false
		bool uvBlack = ((u == NULL || u->color == BLACK) && (v->color == BLACK));

		//������ ����� �θ� ���
		Node* parent = v->parent;

		// �ļ� ��尡 ���ٸ�, v�� ���� ����̴�.
		if (u == NULL)
		{
			if (v == root)
			{
				// v�� root���, root�� �����Ѵ�.
				root = NULL;
			}
			else
			{
				if (uvBlack)
				{
					// �ļ� ��尡 ����, v�� �������� ��� ���� ���̴�.
					// �ļ� ��尡 ����, ������ ��尡 �������̶�� �ش� ���� Ʈ���� ������ ��� ������ �پ��
					// ���� ���̶�� Ī�ϰ� �� ������ �ذ��Ѵ�.
					fixDoubleBlack(v);
				}
				else
				{
					// v�� �������� ��,
					if (v->sibling() != NULL)
						// ���� ��尡 ������ ��, ���������� �ٲ۴�.
						// ���� ��尡 �������̶��, ������ ��� ���� ��Ģ�� �����ϹǷ� ���������� �ٲ۴�.
					{
						v->sibling()->color = RED;
					}
				}

				// v ��带 Ʈ������ �����Ѵ�.
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

		// �ļ� ��尡 �����ϴ� ���,
		// v�� �ڽ��� �ϳ��� �����Ѵٸ�,
		if (v->left == NULL or v->right == NULL)
		{
			// v�� root���,
			if (v == root)
			{
				// v�� u�� ���� �ٲٰ�, u ��带 �����Ѵ�.
				v->val = u->val;
				v->left = v->right = NULL;
				delete u;
			}
			else
			{
				// v�� Ʈ������ �и��ϰ�, u�� v�ڸ��� �ø���.
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
					// �ļ� ��� u�� v�� �������� ��� ���� ���̴�.
					// ������ ���� �ļ� ��� ��� �������̶��, �ش� ���� Ʈ���� ������ ��� ������ �ϳ� �پ��
					// ���� ���̶�� Ī�ϰ� �� ������ �ذ��Ѵ�.
					fixDoubleBlack(u);
				}
				else
				{
					// �� �� �ϳ��� Red���, ��� ��쿡���� Black�� ������ �¾ƾ� �ϹǷ� u�� Black���� ĥ�Ѵ�.
					u->color = BLACK;
				}
			}
			return;
		}

		// v�� 2���� �ڽ��� �����Ѵٸ�, �� ����� ���� �ٲٰ� ��� ȣ���Ѵ�.
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
			// ���� ��尡 ���ٸ�, �θ�� �ö󰡼� ���� ���� �ذ��Ѵ�.
			fixDoubleBlack(parent);
		}
		else
		{
			if (sibling->color == RED)
			{
				//������ �������̶�� ȸ���� �����ؾ� �Ѵ�.
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
				// ���� ��尡 �������� ���,
				if (sibling->hasRedChild())
				{
					// ���� ������ �ڽ��� ������, ������ �ڽ��� ���ų� �������� ���
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
							// �θ� ��带 ������ ���� Ʈ���� ���� ���� ������ ����� �ֱ� ���ؼ� RL ȸ���� �Ѵ�.
							// LR ȸ���� ���������̴�. ���� ���� Ʈ���� ���� ū ���� �θ�� ����� �༭ BST�� ������ �����Ѵ�.
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

							// ���� ���� Ʈ������ ������ ��� �ϳ��� �����Ǿ����Ƿ�, ������ ��带 �������� �����ֱ� ���� left rotation�� �����Ѵ�.
							// ���� ������ ���� Ʈ���� ������ ��带 �ϳ� �����Ƿ� �ڽ��� ���������� ĥ�ؼ� �ϳ� �����Ѵ�.
							// ������ Case�鵵 �̷��� ������ ȸ���� �����Ѵ�.
							sibling->right->color = sibling->color;
							sibling->color = parent->color;
							leftRotate(parent);
						}
					}
					parent->color = BLACK;
				}
				else
				{
					// 2���� ������ �ڽ� ��尡 �ִ� ���,
					sibling->color = RED;

					//�θ� ������ �������̶��, ���� ��尡 ���� �������� �θ�� �����ϸ� ������,
					//�������̶�� ������ ������ �ϳ� �پ�� �ٽ� �θ� ������ ���� ���� ���� �ذ��Ѵ�.
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

	// �־��� ��带 �˻��Ѵ�.
	// ��尡 �߰ߵǸ� ��ȯ�Ѵ�. (������ ���ȴ�.)
	// ��尡 �߰ߵ��� �ʴ´ٸ�, ��ȸ�ϴ� �������� �ֱٿ� �湮�� ��带 ��ȯ. (���Կ� ���ȴ�.)
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

	// Ʈ���� ��带 �����Ѵ�.
	void insert(int n)
	{
		Node* newNode = new Node(n);
		if (root == NULL)
		{
			// root�� null�̶��, root�� �����Ѵ�.
			newNode->color = BLACK;
			root = newNode;
		}
		else
		{
			Node* temp = search(n);

			if (temp->val == n)
			{
				// ���� �ߺ� ��尡 �����Ѵٸ�, ������ �����Ѵ�.
				return;
			}

			// ���� ��尡 �߰ߵ��� �ʾҴٸ�, search�Լ��� ���ο� ��尡 ���Ե�
			// ��ġ�� ��ȯ�� ���̴�.

			newNode->parent = temp;

			if (n < temp->val)
				temp->left = newNode;
			else
				temp->right = newNode;

			// Red - Red ������ �����Ѵٸ�, ��ģ��.
			fixRedRed(newNode);
		}
	}

	// �־��� ���� ���� ��带 �����ϴ� �Լ�
	void deleteByVal(int n)
	{
		if (root == NULL)
			// Ʈ���� ����� ��,
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