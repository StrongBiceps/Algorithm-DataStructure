function insertionSort(array A)
	for i from 2 to length[A] do
		key = A[i]
		j = i - 1
		while j > 0 and A[j] > key do
			[j + 1] = A[j]
			j = j - 1
		end while
		A[j + 1] = key
	end for
end function

// 삽입 정렬은 시퀀스가 이미 정렬되어 있을 때, 즉 최선의 경우에 O(n)의 시간 복잡도를 보이고
// 최악의 경우에는 O(n^2)이다.

void insertionSort(std::vector<int>& arr)
{
    int n = arr.size();
    for (int i = 1; i < n; i++)
    {
        int key = arr[i];
        int j = i - 1;

        // 현재 키 값보다 큰 요소들을 한 칸씩 오른쪽으로 이동
        while (j >= 0 && arr[j] > key) 
	{
            arr[j + 1] = arr[j];
            j--;
        }
        // 키 값을 적절한 위치에 삽입
        arr[j + 1] = key;
    }
}
