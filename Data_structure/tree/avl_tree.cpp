//AVL Tree는 삽입/삭제에 대해서 O(log N) 시간 복잡도를 가진다.

// AVL Tree는 왼쪽과 오른쪽 서브 트리의 높이 차이가 1로 유지된다는 특징으로 인해서, O(log N)의 빠른 검색을 보장한다.
// Red Black Tree는 AVL Tree에 비해서 높이가 클 수 있지만, 여전히 O(log N)의 시간 복잡도를 가진다.

// AVL Tree는 삽입과 삭제 과정에서 빈번한 회전이 발생할 수 있으므로, 삽입/삭제가 느려질 수 있다.
// Red Black Tree는 삽입 및 삭제 시 회전 수가 적어, 이러한 연산이 더 빠르게 진행될 수 있다.

// AVL Tree는 삽입/삭제에 비해서 검색 연산이 많은 경우에 유리하다.
// Red Black Tree는 삽입/삭제가 빈번할 때 유용하다.

#include <iostream>
using namespace std;

//AVL Tree 노드
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
    node->height = 1;  //모든 노드는 외부 노드로 삽입된다.

    return(node);
}

//y의 왼쪽 자식인 x가 오른쪽 자식이 있다면 y보다는 작으므로 y의 왼쪽 자식에 넣어줘야 한다.
Node* rightRotate(Node* y)
{
    Node* x = y->left;
    Node* T2 = x->right;

    //Right Rotation을 수행한다.
    x->right = y;
    y->left = T2;

    //y,x의 높이를 수정한다. 
    y->height = max(height(y->left),
        height(y->right)) + 1;
    x->height = max(height(x->left),
        height(x->right)) + 1;

    //y를 루트로 한 현재 서브 트리의 새로운 루트 노드를 반환한다. 
    return x;
}

//x와 y(x의 자식) 두 노드 가지고 회전을 하는데 y가 왼쪽 자식이 있다면 회전 후 x보다는 클 것이므로 x의 오른쪽 자식으로 넣어줘야 한다.
Node* leftRotate(Node* x)
{
    Node* y = x->right;
    Node* T2 = y->left;

    //Left Rotation을 수행한다.
    y->left = x;
    x->right = T2;

    //y,x의 높이를 수정한다.
    x->height = max(height(x->left),
        height(x->right)) + 1;
    y->height = max(height(y->left),
        height(y->right)) + 1;

    // Return new root  
    return y;
}

//해당 노드의 좌/우 서브트리 높이의 차를 반환한다.
int getBalance(Node* N)
{
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

Node* insert(Node* node, int key)
{
    //일반적인 BST에서 노드를 삽입하는 코드이다.
    if (node == NULL)
        return(newNode(key));

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else // 같은 키는 BST에서 허용되지 않는다.
        return node;

    //노드 삽입 후,이 노드(조상 노드)의 높이를 갱신한다.
    node->height = 1 + max(height(node->left),
        height(node->right));

    //이 노드(조상 노드)가 밸런스가 무너졌는지 체크한다.
    int balance = getBalance(node);

    //만약 밸런스가 무너졌다면, 4가지 경우가 있다.

    //아래 코드들은 재귀를 거슬러 올라가면서 밸런스가 무너진 서브 트리를 찾고, 형태가 어떤 것인지 판별하는 코드이다.

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

    /* 서브 트리의 균형을 조정한 루트 노드가 각 재귀에서 반환된다. */
    return node;
}

Node* minValueNode(Node* node)
{
    Node* current = node;

    /* 가장 왼쪽에 있는 외부 노드를 반환한다. */
    while (current->left != NULL)
        current = current->left;

    return current;
}

Node* deleteNode(Node* root, int key)
{
    // STEP 1: 일반적인 BST 삭제 연산을 실행한다.
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);

    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else
    {
        //만약 자식이 하나만 존재한다면,
        if ((root->left == NULL) ||
            (root->right == NULL))
        {
            Node* temp = root->left ?
                root->left :
                root->right;

            //자식이 없는 Case
            if (temp == NULL)
            {
                temp = root;
                root = NULL;
            }
            else //자식이 하나인 Case
                *root = *temp;
            free(temp);
        }
        else
        {
            //자식이 두 개인 Case -> 오른쪽 서브 트리에서 가장 작은 노드를 탐색.
            Node* temp = minValueNode(root->right);

            //오른쪽 서브트리에서 가장 작은 노드의 값을 루트 노드로 복사.
            root->key = temp->key;

            //탐색한 노드를 오른쪽 서브 트리에서 삭제
            root->right = deleteNode(root->right,
                temp->key);
        }
    }

    //만약 삭제 전 노드 개수가 하나 뿐이었다면, nullptr을 반환
    if (root == NULL)
        return root;

    // STEP 2: 현재 노드의 높이를 갱신
    root->height = 1 + max(height(root->left),
        height(root->right));

    //현재 노드의 균형을 확인
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