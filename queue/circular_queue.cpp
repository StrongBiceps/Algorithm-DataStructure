#include <iostream>
#include <vector>
#include <string>
using namespace std;

template<typename T>
class Cir_Q
{
public:
    Cir_Q() {}
    Cir_Q(const int& q_size)
    {
        data.resize(q_size);
        size = q_size;
    }
    void enqueue(const T& val)
    {
        if (!isFull())
        {
            data[tail] = val;
            tail = (tail + 1) % size;
        }
    }
    T dequeue()
    {
        if (!empty())
        {
            T val = data[head];
            head = (head + 1) % size;

            return val;
        }
        else
        {
            return -1;
        }
    }
    size_t getSize()
    {
        return (tail - head + size) % size;
    }
    T getFront()
    {
        return data[head];
    }
    T getRear()
    {
        return data[(tail - 1 + size) % size];
    }
    bool empty()
    {
        return head == tail;
    }
    bool  isFull()
    {
        return ((tail + 1) % size) == head;
    }
private:
    vector<T> data;
    size_t size;
    int head{ 0 };
    int tail{ 0 };
};
