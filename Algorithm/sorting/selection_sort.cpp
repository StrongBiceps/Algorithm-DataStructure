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
