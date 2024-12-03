function selectionSort(array A)
	for i from 1 to length[A] - 1 do
		minIndex = i
		for j from i + 1 to length[A] do
			if A[j] < A[minIndex] then
				minIndex = j
			end if
		end for

		if minIndex != i then
			swap(A[i], A[minIndex])
		end if
	end for
end function

// 선택 정렬은 최선, 최악의 경우 모두 O(n^2)의 시간 복잡도를 보인다.

void selectionSort(std::vector<int>& arr) 
{
    int n = arr.size();
    for (int i = 0; i < n - 1; i++) 
    {
        // 현재 위치에서 가장 작은 요소의 인덱스를 찾음
        int minIndex = i;
        for (int j = i + 1; j < n; j++) 
	{
            if (arr[j] < arr[minIndex]) 
	    {
                minIndex = j;
            }
        }
        // 현재 위치의 요소와 가장 작은 요소를 교환
        std::swap(arr[i], arr[minIndex]);
    }
}
