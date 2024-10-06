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

//Key에 해당하는 노드를 Root로 끌어 올리고, Root를 반환한다.
//만약 Key에 해당하는 노드가 없으면, Tree를 순회하며 가장 마지막에 방문한 노드를 Root로 끌어 올리고 반환한다.
struct node* splay(struct node* root, int key)
{
    if (root == NULL || root->key == key)
        return root;

    if (root->key > key)
    {

        if (root->left == NULL)
            return root;

        //Step 1 : 첫 번째 회전

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

        //Step 2 : 두 번째 회전

        //만약 손자 노드가 찾는 노드라면, 첫 번째 회전은 진행하지 않고, 바로 두 번째 회전을 진행한다.
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

    //splay 과정이 끝나면 root는 key와 가장 가까운 값을 가질 것이다.
    //이 과정에서, root의 left는 이미 key보다 작을 것이고, root의 right는 key보다 클 것이다.
    //splay 시뮬레이션을 확인해보면, 확인할 수 있다. 따라서 아래와 같이 새로운 root의 자식을 변경해도 무방하다.
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
* Splay Tree에서의 후속 노드 탐색 과정과 BST에서의 후속 노드 탐색 과정이 다른 이유 :
*
* Splay Tree -> 왼쪽 서브 트리의 가장 큰 노드를 Splay한다. (root의 위치와 가깝다)
* BST -> 오른쪽 서브 트리의 가장 작은 노드를 Splay한다. (root의 위치와 멀다)
*
* splay tree는 root의 위치와 가까운 노드를 root로 올려서, Splay 연산을 줄이고, 또 root와 근사한 값의 노드를 splay하여 다음 연산에서도 유리하게 작용하도록 한다.
*
*/

//지울 노드를 Root로 Splay한 후, Root 노드를 삭제한다.
//그리고 후속 노드를 다시 Root로 Splay한다.
struct node* delete_key(struct node* root, int key)
{
    struct node* temp;
    if (!root)
        return NULL;

    //지울 노드를 Splay한다.
    root = splay(root, key);

    //만약 지울 키가 존재하지 않는다면, Splay된 Root를 반환한다.
    if (key != root->key)
        return root;

    //만약 왼쪽 자식이 존재하지 않는다면, 오른쪽 자식을 새로운 Root로 설정한다.
    if (!root->left) {
        temp = root;
        root = root->right;
    }

    //만약 왼쪽 자식이 존재한다면,
    else {
        temp = root;

        //여기까지 도달하면, root->key == key이다.
        //따라서 후속 노드로 root를 교체하기 위해, 왼쪽 서브 트리에서 key를 Splay한다.
        //key를 가진 node는 이미 root 노드로 Splay된 상태 이므로, 왼쪽 서브 트리에서 key를 Splay하면
        //왼쪽 서브 트리에서 가장 큰 노드가 root로 Splay될 것이다.
        root = splay(root->left, key);

        //Splay 과정에서 Right Rotation이 이루어졌으므로, 새로운 root의 오른쪽 서브 트리를 이전 root의 오른쪽 서브 트리로 교체한다.
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