#include <iostream>
#include <vector>
using namespace std;

vector<int> countSort(vector<int>& inputArray)
{
    int N = inputArray.size();

    // 입력 배열에서 가장 큰 값을 찾는다.
    int M = 0;

    for (int i = 0; i < N; i++)
        M = max(M, inputArray[i]);

    // Counting 배열을 설정한다.
    vector<int> countArray(M + 1, 0);

    //Bloom Filter 방식으로 입력 배열의 원소 개수를 센다.

    for (int i = 0; i < N; i++)
        countArray[inputArray[i]]++;

    //Counting 배열의 값을 누적시킨다.
    for (int i = 1; i <= M; i++)
        countArray[i] += countArray[i - 1];

    //정렬된 배열을 저장할 컨테이너를 선언.
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
    // 입력 배열
    vector<int> inputArray = { 4, 3, 12, 1, 5, 5, 3, 9 };

    // 출력 배열
    vector<int> outputArray = countSort(inputArray);

    for (int i = 0; i < inputArray.size(); i++)
        cout << outputArray[i] << " ";

    return 0;
}