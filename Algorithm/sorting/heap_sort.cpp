function heapSort(arr) :
    n = length(arr)

    // 1. 힙을 구성한다.
    for i from n / 2 - 1 down to 0 :
        heapify(arr, n, i)

        // 2. 힙에서 요소를 하나씩 제거하여 정렬한다.
        for i from n - 1 down to 1 :
            swap(arr[0], arr[i])  // 최대값을 배열의 끝으로 이동
            heapify(arr, i, 0)     // 힙을 다시 구성
        end for
    end for
end function

// 힙을 정렬하는 알고리즘 함수인 hipify 함수는 O(log n) 시간 복잡도를 가진다.
// 힙에서 최대,최소 값을 제거하고 정렬하는 연산이 n번 일어나기 때문에 O(n log n) 시간 복잡도를 가진다.