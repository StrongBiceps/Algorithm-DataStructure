#include <iostream>
#include <vector>
#include <string>
using namespace std;

template<typename T>
class node
{
public:
    //Initializer_list를 사용하여 생성하려면 기본 생성자가 존재하지 않아야 한다.
    ~node()
    {
        if (data)
        {
            delete data;
            data = nullptr;
        }
    }
    bool isDummynode()
    {
        return isDummy;
    }
    void setNext(node* ptr)
    {
        if (ptr != nullptr)
            next = ptr;
    }
    void setPrev(node* ptr)
    {
        if (ptr != nullptr)
            prev = ptr;
    }
    node* getNext()
    {
        return next;
    }
    node* getPrev()
    {
        return prev;
    }
    const T& getData()
    {
        return *data;
    }

    //Initializer_list를 사용하여 생성하려면 멤버가 public이어야 한다.
public:
    T* data{ nullptr };
    node* next{ nullptr };
    node* prev{ nullptr };
    bool isDummy{ false };
};

template<typename T>
class doubly_list
{
public:
    doubly_list()
    {
        head = new node<T>{ nullptr,nullptr,nullptr,false };
        tail = new node<T>{ nullptr,nullptr,nullptr,false };

        head->setNext(tail);
        tail->setPrev(head);
    }
    ~doubly_list()
    {
        Release();
    }
    void insert(const T& val)
    {
        if (empty())
        {
            auto newnode = new node<T>{ new T(val),nullptr,nullptr,false };
            head->setNext(newnode);
            newnode->setPrev(head);
            tail->setPrev(newnode);
            newnode->setNext(tail);
            cursor = newnode;
            ++Size;
        }
        else
        {
            auto newnode = new node<T>{ new T(val),nullptr,nullptr,false };
            newnode->setNext(cursor->getNext());
            newnode->setPrev(cursor);

            cursor->getNext()->setPrev(newnode);
            cursor->setNext(newnode);
            cursor = newnode;

            ++Size;
        }
    }
    void erase()
    {
        if (cursor != head && cursor != tail)
        {
            cursor->getPrev()->setNext(cursor->getNext());
            cursor->getNext()->setPrev(cursor->getPrev());
            cursor = cursor->getPrev();
            --Size;
        }
    }
    void shift(const bool& dir)
    {
        if (dir)
        {
            if (cursor->getNext() != tail)
            {
                cursor = cursor->getNext();
            }
        }
        else
        {
            if (cursor != head)
            {
                cursor = cursor->getPrev();
            }
        }
    }
    void traverse()
    {
        node<T>* cur{ head };
        cur = cur->getNext();

        while (cur != tail)
        {
            cout << cur->getData();
            cur = cur->getNext();
        }
    }
    size_t getSize()
    {
        return Size;
    }
    bool empty()
    {
        return Size == 0;
    }
    void Release()
    {
        while (cursor != tail)
        {
            auto toDelete = cursor->next;
            delete cursor;
            cursor = toDelete;
        }

        delete cursor;
        cursor = nullptr;
    }
private:
    node<T>* head{ nullptr };
    node<T>* tail{ nullptr };
    node<T>* cursor{ nullptr };
    size_t Size{ 0 };
};