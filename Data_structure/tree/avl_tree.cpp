//AVL Tree�� ����/������ ���ؼ� O(log N) �ð� ���⵵�� ������.

// AVL Tree�� ���ʰ� ������ ���� Ʈ���� ���� ���̰� 1�� �����ȴٴ� Ư¡���� ���ؼ�, O(log N)�� ���� �˻��� �����Ѵ�.
// Red Black Tree�� AVL Tree�� ���ؼ� ���̰� Ŭ �� ������, ������ O(log N)�� �ð� ���⵵�� ������.

// AVL Tree�� ���԰� ���� �������� ����� ȸ���� �߻��� �� �����Ƿ�, ����/������ ������ �� �ִ�.
// Red Black Tree�� ���� �� ���� �� ȸ�� ���� ����, �̷��� ������ �� ������ ����� �� �ִ�.

// AVL Tree�� ����/������ ���ؼ� �˻� ������ ���� ��쿡 �����ϴ�.
// Red Black Tree�� ����/������ ����� �� �����ϴ�.

#include <iostream>
using namespace std;

//AVL Tree ���
class Node
{
public:
    int key;
    Node* left;
    Node* right;
    int height;
};

int height(Node* N)
{
    if (N == NULL)
        return 0;
    return N->height;
}

int max(int a, int b)
{
    return (a > b) ? a : b;
}

Node* newNode(int key)
{
    Node* node = new Node();
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  //��� ���� �ܺ� ���� ���Եȴ�.

    return(node);
}

//y�� ���� �ڽ��� x�� ������ �ڽ��� �ִٸ� y���ٴ� �����Ƿ� y�� ���� �ڽĿ� �־���� �Ѵ�.
Node* rightRotate(Node* y)
{
    Node* x = y->left;
    Node* T2 = x->right;

    //Right Rotation�� �����Ѵ�.
    x->right = y;
    y->left = T2;

    //y,x�� ���̸� �����Ѵ�. 
    y->height = max(height(y->left),
        height(y->right)) + 1;
    x->height = max(height(x->left),
        height(x->right)) + 1;

    //y�� ��Ʈ�� �� ���� ���� Ʈ���� ���ο� ��Ʈ ��带 ��ȯ�Ѵ�. 
    return x;
}

//x�� y(x�� �ڽ�) �� ��� ������ ȸ���� �ϴµ� y�� ���� �ڽ��� �ִٸ� ȸ�� �� x���ٴ� Ŭ ���̹Ƿ� x�� ������ �ڽ����� �־���� �Ѵ�.
Node* leftRotate(Node* x)
{
    Node* y = x->right;
    Node* T2 = y->left;

    //Left Rotation�� �����Ѵ�.
    y->left = x;
    x->right = T2;

    //y,x�� ���̸� �����Ѵ�.
    x->height = max(height(x->left),
        height(x->right)) + 1;
    y->height = max(height(y->left),
        height(y->right)) + 1;

    // Return new root  
    return y;
}

//�ش� ����� ��/�� ����Ʈ�� ������ ���� ��ȯ�Ѵ�.
int getBalance(Node* N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node* insert(Node* node, int key)
{
    //�Ϲ����� BST���� ��带 �����ϴ� �ڵ��̴�.
    if (node == NULL)
        return(newNode(key));

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // ���� Ű�� BST���� ������ �ʴ´�.
        return node;

    //��� ���� ��,�� ���(���� ���)�� ���̸� �����Ѵ�.
    node->height = 1 + max(height(node->left),
        height(node->right));

    //�� ���(���� ���)�� �뷱���� ���������� üũ�Ѵ�.
    int balance = getBalance(node);

    //���� �뷱���� �������ٸ�, 4���� ��찡 �ִ�.

    //�Ʒ� �ڵ���� ��͸� �Ž��� �ö󰡸鼭 �뷱���� ������ ���� Ʈ���� ã��, ���°� � ������ �Ǻ��ϴ� �ڵ��̴�.

    // Left Left Case  
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Right Right Case  
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Left Right Case  
    if (balance > 1 && key > node->left->key)
    {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Right Left Case  
    if (balance < -1 && key < node->right->key)
    {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    /* ���� Ʈ���� ������ ������ ��Ʈ ��尡 �� ��Ϳ��� ��ȯ�ȴ�. */
    return node;
}

Node* minValueNode(Node* node)
{
    Node* current = node;

    /* ���� ���ʿ� �ִ� �ܺ� ��带 ��ȯ�Ѵ�. */
    while (current->left != NULL)
        current = current->left;

    return current;
}

Node* deleteNode(Node* root, int key)
{
    // STEP 1: �Ϲ����� BST ���� ������ �����Ѵ�.
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);

    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else
    {
        //���� �ڽ��� �ϳ��� �����Ѵٸ�,
        if ((root->left == NULL) ||
            (root->right == NULL))
        {
            Node* temp = root->left ?
                root->left :
                root->right;

            //�ڽ��� ���� Case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else //�ڽ��� �ϳ��� Case
                *root = *temp;
            free(temp);
        }
        else
        {
            //�ڽ��� �� ���� Case -> ������ ���� Ʈ������ ���� ���� ��带 Ž��.
            Node* temp = minValueNode(root->right);

            //������ ����Ʈ������ ���� ���� ����� ���� ��Ʈ ���� ����.
            root->key = temp->key;

            //Ž���� ��带 ������ ���� Ʈ������ ����
            root->right = deleteNode(root->right,
                temp->key);
        }
    }

    //���� ���� �� ��� ������ �ϳ� ���̾��ٸ�, nullptr�� ��ȯ
    if (root == NULL)
        return root;

    // STEP 2: ���� ����� ���̸� ����
    root->height = 1 + max(height(root->left),
        height(root->right));

    //���� ����� ������ Ȯ��
    int balance = getBalance(root);

    // Left Left Case 
    if (balance > 1 &&
        getBalance(root->left) >= 0)
        return rightRotate(root);

    // Left Right Case 
    if (balance > 1 &&
        getBalance(root->left) < 0)
    {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Right Right Case 
    if (balance < -1 &&
        getBalance(root->right) <= 0)
        return leftRotate(root);

    // Right Left Case 
    if (balance < -1 &&
        getBalance(root->right) > 0)
    {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

void preOrder(Node* root)
{
    if (root != NULL)
    {
        cout << root->key << " ";
        preOrder(root->left);
        preOrder(root->right);
    }
}

int main()
{
    Node* root = NULL;

    root = insert(root, 9);
    root = insert(root, 5);
    root = insert(root, 10);
    root = insert(root, 0);
    root = insert(root, 6);
    root = insert(root, 11);
    root = insert(root, -1);
    root = insert(root, 1);
    root = insert(root, 2);

    cout << "Preorder traversal of the "
        "constructed AVL tree is \n";
    preOrder(root);

    root = deleteNode(root, 10);

    cout << "\nPreorder traversal after"
        << " deletion of 10 \n";
    preOrder(root);

    return 0;
}