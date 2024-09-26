#include <iostream>

const int MX{ 100005 };

int dat[MX]{ 0 };
int pos{ 0 };

using namespace std;

void push(const int& data)
{
    dat[pos] = data;
    ++pos;
}

int empty()
{
    if (pos == 0)
        return 1;
    else
        return 0;
}

int pop()
{
    int data{ 0 };

    if (!empty())
    {
        data = dat[pos - 1];
        --pos;
    }
    else
        data = -1;

    return data;
}

int top()
{
    int data{ 0 };

    if (!empty())
    {
        data = dat[pos - 1];
    }
    else
        data = -1;

    return data;
}

int size()
{
    return pos;
}