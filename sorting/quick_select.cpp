#include <algorithm>
#include <iostream>
#include <random>
#include <vector>
using namespace std;

template <typename T>
T quickselect_pos(std::vector<T>& data, size_t start, size_t end, size_t k);

template <typename T>
size_t sort_and_get_median(std::vector<T>& data, size_t start, size_t end)
{
    std::sort(data.begin() + start, data.begin() + end + 1);
    return (start + end) / 2;
}

template <typename T>
size_t choose_pivot(std::vector<T>& data, size_t start, size_t end)
{
    if (end - start < 5)
    {
        return sort_and_get_median(data, start, end);
    }

    size_t current = start;

    for (size_t i = start; i <= end; i += 5)
    {
        size_t med_pos = sort_and_get_median(data, i, std::min(i + 4, end));

        std::iter_swap(data.begin() + current, data.begin() + med_pos);
        current++;
    }

    return quickselect_pos(data, start, current - 1, (current + start - 1) / 2);
}

template <typename T>
size_t partition(std::vector<T>& data, size_t start, size_t end, size_t pivot_pos)
{

    std::iter_swap(data.begin() + end, data.begin() + pivot_pos);

    const auto& pivot = data[end];
    size_t current_small_loc = start;


    for (size_t i = start; i <= end; i++)
    {
        if (data[i] <= pivot)
        {
            std::swap(data[current_small_loc++], data[i]);
        }
    }

    return current_small_loc - 1;
}

//k��° ��ġ�� ��ȯ�ϴ� �Լ�
template <typename T>
T quickselect_pos(std::vector<T>& data, size_t start, size_t end, size_t k)
{
    if (start == end)
    {
        return start;
    }

    size_t pivot_pos = choose_pivot(data, start, end);

    pivot_pos = partition(data, start, end, pivot_pos);

    if (pivot_pos == k)
    {
        return pivot_pos;
    }
    else if (pivot_pos < k)
    {
        return quickselect_pos(data, pivot_pos + 1, end, k);
    }
    else
    {
        return quickselect_pos(data, start, pivot_pos - 1, k);
    }
}

//k��° ���� ��ȯ�ϴ� �Լ�
template <typename T>
T quickselect(std::vector<T>& data, size_t start, size_t end, size_t k) {
    if (start == end)
    {
        return data[start];
    }

    size_t pivot_pos = choose_pivot(data, start, end);

    pivot_pos = partition(data, start, end, pivot_pos);

    if (pivot_pos == k)
    {
        return data[pivot_pos];
    }
    else if (pivot_pos < k)
    {
        return quickselect(data, pivot_pos + 1, end, k);
    }
    else
    {
        return quickselect(data, start, pivot_pos - 1, k);
    }
}

int main() {
    vector<int> S1{ 45, 1, 3, 1, 2, 3, 45, 5, 1, 2, 44, 5, 7 };

    for (register int i = 0; i < 13; ++i)
    {
        cout << quickselect(S1, 0, 12, i) << " ";
    }

    return 0;
}

/*
sudo code

function sort_and_get_median(data, start, end):
    sort(data[start] to data[end])  // start���� end���� ����
    return (start + end) / 2  // �߰� �ε��� ��ȯ

function choose_pivot(data, start, end):
    if (end - start < 5):
        return sort_and_get_median(data, start, end)  // 5�� �̸��� ��� �߰��� ��ȯ

    current = start
    for i from start to end step 5:
        med_pos = sort_and_get_median(data, i, min(i + 4, end))  // 5���� �׷�ȭ�Ͽ� �߰��� ã��
        swap(data[current], data[med_pos])  // �߰����� ���� ��ġ�� �̵�
        current = current + 1

    return quickselect_pos(data, start, current - 1, (current + start - 1) / 2)  // �߰����� ��ġ ��ȯ

function partition(data, start, end, pivot_pos):
    swap(data[end], data[pivot_pos])  // �ǹ��� ������ ��ġ�� �̵�
    pivot = data[end]
    current_small_loc = start

    for i from start to end:
        if (data[i] <= pivot):
            swap(data[current_small_loc], data[i])  // ���� ���Ҹ� �������� �̵�
            current_small_loc = current_small_loc + 1

    return current_small_loc - 1  // �ǹ��� ���� ��ġ ��ȯ

function quickselect_pos(data, start, end, k):
    if (start == end):
        return start  // ���۰� ���� ������ �ش� ��ġ ��ȯ

    pivot_pos = choose_pivot(data, start, end)  // �ǹ� ����
    pivot_pos = partition(data, start, end, pivot_pos)  // ��Ƽ�� ����

    if (pivot_pos == k):
        return pivot_pos  // k��° ��ġ�� ��ġ�ϸ� ��ȯ
    else if (pivot_pos < k):
        return quickselect_pos(data, pivot_pos + 1, end, k)  // k�� �ǹ� �����ʿ� ���� ��
    else:
        return quickselect_pos(data, start, pivot_pos - 1, k)  // k�� �ǹ� ���ʿ� ���� ��

function quickselect(data, start, end, k):
    if (start == end):
        return data[start]  // ���۰� ���� ������ �ش� �� ��ȯ

    pivot_pos = choose_pivot(data, start, end)  // �ǹ� ����
    pivot_pos = partition(data, start, end, pivot_pos)  // ��Ƽ�� ����

    if (pivot_pos == k):
        return data[pivot_pos]  // k��° ��ġ�� ��ġ�ϸ� ��ȯ
    else if (pivot_pos < k):
        return quickselect(data, pivot_pos + 1, end, k)  // k�� �ǹ� �����ʿ� ���� ��
    else:
        return quickselect(data, start, pivot_pos - 1, k)  // k�� �ǹ� ���ʿ� ���� ��

*/