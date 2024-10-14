#include <iostream>
#include <vector>

void bubbleSort(std::vector<int>& arr) 
{
    int n = arr.size();
    bool swapped;

    // n-1 회 반복
    for (int i = 0; i < n - 1; ++i) 
    {
        swapped = false;

        // 인접한 요소 비교 및 교환
        for (int j = 0; j < n - i - 1; ++j)
        {
            if (arr[j] > arr[j + 1]) 
            {
                std::swap(arr[j], arr[j + 1]);
                swapped = true; // 교환이 발생했음을 기록
            }
        }

        // 더 이상 교환이 없으면 정렬 완료
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

    std::cout << "정렬 전 배열: ";
    printArray(arr);

    bubbleSort(arr);

    std::cout << "정렬 후 배열: ";
    printArray(arr);

    return 0;
}
