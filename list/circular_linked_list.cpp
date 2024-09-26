#include <iostream>

//checking memory leak
#define _CRTDBG_MAP_ALLOC
#include <stdlib.h>
#include <crtdbg.h>

using namespace std;

template<typename T>
class node
{
public:
    ~node()
    {
        if (data)
        {
            delete data;
            data = nullptr;
        }
    }
    void setNext(node* ptr)
    {
        next = ptr;
    }
    void setPrev(node* ptr)
    {
        prev = ptr;
    }
    void setData(const T& val)
    {
        if (data)
            *data = val;
    }
    T getData()
    {
        return *data;
    }
    node* getNext()
    {
        return next;
    }
    node* getPrev()
    {
        return prev;
    }
    bool isDummyNode()
    {
        return isDummy;
    }

public:
    T* data{ nullptr };
    node* next{ nullptr };
    node* prev{ nullptr };
    bool  isDummy{ false };
};

template<typename T>
class cir_list
{
public:
    cir_list()
    {
        head = new node<T>{ nullptr,nullptr,nullptr,true };
        head->setPrev(head);
        head->setNext(head);
        cursor = head;
    }
    ~cir_list() = default;
    void insert(const T& val)
    {
        if (empty())
        {
            node<T>* newNode = new node<T>{ new T(val),nullptr,nullptr,false };
            newNode->setPrev(head);
            newNode->setNext(head);
            head->setNext(newNode);
            head->setPrev(newNode);

            head = newNode;
            cursor = head;

            ++size;
        }
        else
        {
            node<T>* newNode = new node<T>{ new T(val),nullptr,nullptr,false };
            newNode->setPrev(cursor->getPrev());
            newNode->setNext(cursor);
            cursor->getPrev()->setNext(newNode);
            cursor->setPrev(newNode);

            if (cursor == head)
            {
                head = newNode;
            }
            cursor = newNode;

            ++size;
        }
    }
    void erase()
    {
        if (!empty())
        {
            cursor->getPrev()->setNext(cursor->getNext());
            cursor->getNext()->setPrev(cursor->getPrev());

            if (cursor == head)
            {
                head = cursor->getNext();
            }

            auto Temp = cursor;

            cursor = cursor->getNext();

            if (cursor->isDummyNode() == true)
                cursor = cursor->getNext();

            delete Temp;

            --size;
        }
    }
    void shift(const int& val)
    {
        for (int i = 0; i < val; ++i)
        {
            cursor = cursor->getNext();
            if (cursor->isDummyNode())
                cursor = cursor->getNext();
        }
    }
    T getCursorData()
    {
        return cursor->getData();
    }
    bool empty()
    {
        return size == 0;
    }
    size_t getSize()
    {
        return size;
    }
    void release()
    {
        while (!empty())
        {
            erase();
        }

        if (head)
        {
            delete head;
        }

        head = nullptr;
        cursor = nullptr;
    }
private:
    size_t size{ 0 };
    node<T>* head{ nullptr };
    node<T>* cursor{ nullptr };
};