#include <iostream>
#include <vector>
using namespace std;

vector<int> countSort(vector<int>& inputArray)
{
    int N = inputArray.size();

    // �Է� �迭���� ���� ū ���� ã�´�.
    int M = 0;

    for (int i = 0; i < N; i++)
        M = max(M, inputArray[i]);

    // Counting �迭�� �����Ѵ�.
    vector<int> countArray(M + 1, 0);

    //Bloom Filter ������� �Է� �迭�� ���� ������ ����.

    for (int i = 0; i < N; i++)
        countArray[inputArray[i]]++;

    //Counting �迭�� ���� ������Ų��.
    for (int i = 1; i <= M; i++)
        countArray[i] += countArray[i - 1];

    //���ĵ� �迭�� ������ �����̳ʸ� ����.
    vector<int> outputArray(N);

    for (int i = N - 1; i >= 0; i--)
    {
        outputArray[countArray[inputArray[i]] - 1]
            = inputArray[i];

        countArray[inputArray[i]]--;
    }

    return outputArray;
}

int main()
{
    // �Է� �迭
    vector<int> inputArray = { 4, 3, 12, 1, 5, 5, 3, 9 };

    // ��� �迭
    vector<int> outputArray = countSort(inputArray);

    for (int i = 0; i < inputArray.size(); i++)
        cout << outputArray[i] << " ";

    return 0;
}