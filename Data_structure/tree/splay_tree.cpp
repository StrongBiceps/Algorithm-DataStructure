#include <iostream>
using namespace std;

struct node {
    int key;
    node* left, * right;
};

struct node* newNode(int key)
{
    node* _node = new node();
    _node->key = key;
    _node->left = _node->right = NULL;
    return _node;
}

struct node* rightRotate(struct node* x)
{
    struct node* y = x->left;
    x->left = y->right;
    y->right = x;
    return y;
}

struct node* leftRotate(struct node* x)
{
    struct node* y = x->right;
    x->right = y->left;
    y->left = x;
    return y;
}

//Key�� �ش��ϴ� ��带 Root�� ���� �ø���, Root�� ��ȯ�Ѵ�.
//���� Key�� �ش��ϴ� ��尡 ������, Tree�� ��ȸ�ϸ� ���� �������� �湮�� ��带 Root�� ���� �ø��� ��ȯ�Ѵ�.
struct node* splay(struct node* root, int key)
{
    if (root == NULL || root->key == key)
        return root;

    if (root->key > key)
    {

        if (root->left == NULL)
            return root;

        //Step 1 : ù ��° ȸ��

        // Zig-Zig (Left Left)
        if (root->left->key > key)
        {
            root->left->left = splay(root->left->left, key);

            root = rightRotate(root);
        }
        else if (root->left->key < key) // Zig-Zag (Left Right)
        {
            root->left->right
                = splay(root->left->right, key);

            if (root->left->right != NULL)
                root->left = leftRotate(root->left);
        }

        //Step 2 : �� ��° ȸ��

        //���� ���� ��尡 ã�� �����, ù ��° ȸ���� �������� �ʰ�, �ٷ� �� ��° ȸ���� �����Ѵ�.
        return (root->left == NULL) ? root
            : rightRotate(root);
    }
    else
    {
        if (root->right == NULL)
            return root;

        // Zag-Zig (Right Left)
        if (root->right->key > key)
        {
            root->right->left
                = splay(root->right->left, key);

            if (root->right->left != NULL)
                root->right = rightRotate(root->right);
        }
        else if (root->right->key < key) // Zag-Zag (Right Right)
        {
            root->right->right
                = splay(root->right->right, key);
            root = leftRotate(root);
        }

        return (root->right == NULL) ? root
            : leftRotate(root);
    }
}

Node* insert(Node* root, int key)
{
    if (root == nullptr)
        return newNode(key);

    root = splay(root, key);

    if (root->key == key)
        return root;

    Node* node = newNode(key);

    //splay ������ ������ root�� key�� ���� ����� ���� ���� ���̴�.
    //�� ��������, root�� left�� �̹� key���� ���� ���̰�, root�� right�� key���� Ŭ ���̴�.
    //splay �ùķ��̼��� Ȯ���غ���, Ȯ���� �� �ִ�. ���� �Ʒ��� ���� ���ο� root�� �ڽ��� �����ص� �����ϴ�.
    if (root->key > key) 
    {
        node->right = root;
        node->left = root->left;
        root->left = nullptr;
    }
    else 
    {
        node->left = root;
        node->right = root->right;
        root->right = nullptr;
    }
    return node;
}

/*
* Splay Tree������ �ļ� ��� Ž�� ������ BST������ �ļ� ��� Ž�� ������ �ٸ� ���� :
*
* Splay Tree -> ���� ���� Ʈ���� ���� ū ��带 Splay�Ѵ�. (root�� ��ġ�� ������)
* BST -> ������ ���� Ʈ���� ���� ���� ��带 Splay�Ѵ�. (root�� ��ġ�� �ִ�)
*
* splay tree�� root�� ��ġ�� ����� ��带 root�� �÷���, Splay ������ ���̰�, �� root�� �ٻ��� ���� ��带 splay�Ͽ� ���� ���꿡���� �����ϰ� �ۿ��ϵ��� �Ѵ�.
*
*/

//���� ��带 Root�� Splay�� ��, Root ��带 �����Ѵ�.
//�׸��� �ļ� ��带 �ٽ� Root�� Splay�Ѵ�.
struct node* delete_key(struct node* root, int key)
{
    struct node* temp;
    if (!root)
        return NULL;

    //���� ��带 Splay�Ѵ�.
    root = splay(root, key);

    //���� ���� Ű�� �������� �ʴ´ٸ�, Splay�� Root�� ��ȯ�Ѵ�.
    if (key != root->key)
        return root;

    //���� ���� �ڽ��� �������� �ʴ´ٸ�, ������ �ڽ��� ���ο� Root�� �����Ѵ�.
    if (!root->left) {
        temp = root;
        root = root->right;
    }

    //���� ���� �ڽ��� �����Ѵٸ�,
    else {
        temp = root;

        //������� �����ϸ�, root->key == key�̴�.
        //���� �ļ� ���� root�� ��ü�ϱ� ����, ���� ���� Ʈ������ key�� Splay�Ѵ�.
        //key�� ���� node�� �̹� root ���� Splay�� ���� �̹Ƿ�, ���� ���� Ʈ������ key�� Splay�ϸ�
        //���� ���� Ʈ������ ���� ū ��尡 root�� Splay�� ���̴�.
        root = splay(root->left, key);

        //Splay �������� Right Rotation�� �̷�������Ƿ�, ���ο� root�� ������ ���� Ʈ���� ���� root�� ������ ���� Ʈ���� ��ü�Ѵ�.
        root->right = temp->right;
    }

    free(temp);

    return root;
}

void preOrder(struct node* root)
{
    if (root != NULL) {
        cout << root->key;
        preOrder(root->left);
        preOrder(root->right);
    }
}

int main()
{
    // Splay Tree Formation
    struct node* root = newNode(6);
    root->left = newNode(1);
    root->right = newNode(9);
    root->left->right = newNode(4);
    root->left->right->left = newNode(2);
    root->right->left = newNode(7);

    int key = 4;

    root = delete_key(root, key);

    cout << "Preorder traversal of the modified Splay tree "
        "is \n";
    preOrder(root);
    return 0;
}