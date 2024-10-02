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