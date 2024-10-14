#include <iostream>
#include <vector>

void bubbleSort(std::vector<int>& arr) 
{
    int n = arr.size();
    bool swapped;

    // n-1 ȸ �ݺ�
    for (int i = 0; i < n - 1; ++i) 
    {
        swapped = false;

        // ������ ��� �� �� ��ȯ
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1]) 
            {
                std::swap(arr[j], arr[j + 1]);
                swapped = true; // ��ȯ�� �߻������� ���
            }
        }

        // �� �̻� ��ȯ�� ������ ���� �Ϸ�
        if (!swapped) 
        {
            break;
        }
    }
}

void printArray(const std::vector<int>& arr)
{
    for (int num : arr) {
        std::cout << num << " ";
    }
    std::cout << std::endl;
}

int main() 
{
    std::vector<int> arr = { 64, 34, 25, 12, 22, 11, 90 };

    std::cout << "���� �� �迭: ";
    printArray(arr);

    bubbleSort(arr);

    std::cout << "���� �� �迭: ";
    printArray(arr);

    return 0;
}
