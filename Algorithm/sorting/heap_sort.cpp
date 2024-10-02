function heapSort(arr) :
    n = length(arr)

    // 1. ���� �����Ѵ�.
    for i from n / 2 - 1 down to 0 :
        heapify(arr, n, i)

        // 2. ������ ��Ҹ� �ϳ��� �����Ͽ� �����Ѵ�.
        for i from n - 1 down to 1 :
            swap(arr[0], arr[i])  // �ִ밪�� �迭�� ������ �̵�
            heapify(arr, i, 0)     // ���� �ٽ� ����
        end for
    end for
end function

// ���� �����ϴ� �˰��� �Լ��� hipify �Լ��� O(log n) �ð� ���⵵�� ������.
// ������ �ִ�,�ּ� ���� �����ϰ� �����ϴ� ������ n�� �Ͼ�� ������ O(n log n) �ð� ���⵵�� ������.