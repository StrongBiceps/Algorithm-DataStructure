#include <iostream>
#include <vector>
using namespace std;

template<typename T>
class Cir_Deque
{
public:
    Cir_Deque(const int& deq_size)
    {
        size = deq_size;
        data.resize(deq_size);
        head = size / 2;
        tail = size / 2;
    }
    void enqueue_front(const T& val)
    {
        if (!isFull())
        {
            head = (head - 1 + size) % size;
            data[head] = val;
        }
    }
    void enqueue_rear(const T& val)
    {
        if (!isFull())
        {
            data[tail] = val;
            tail = (tail + 1) % size;
        }
    }
    T dequeue_front()
    {
        if (!empty())
        {
            T returnValue{ data[head] };
            head = (head + 1) % size;
            return returnValue;
        }
    }
    T dequeue_rear()
    {
        if (!empty())
        {
            tail = (tail - 1 + size) % size;
            return data[tail];
        }
    }
    bool isFull()
    {
        return (tail + 1) % size == head;
    }
    bool empty()
    {
        return (head == tail);
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
private:
    size_t size{ 0 };
    vector<int>    data;
    int head{ 0 };
    int tail{ 0 };
};