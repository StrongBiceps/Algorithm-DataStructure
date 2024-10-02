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
