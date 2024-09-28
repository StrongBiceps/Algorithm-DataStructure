#include <iostream>
#include <vector>
using namespace std;

class MaxHeap 
{
public:
	MaxHeap(int maxSize);

	void MaxHeapify(int);

	int parent(int i)
	{
		return (i - 1) / 2;
	}

	int lChild(int i)
	{
		return (2 * i + 1);
	}

	int rChild(int i)
	{
		return (2 * i + 2);
	}

	int removeMax();

	void increaseKey(int i, int newVal);

	int getMax()
	{
		return arr[0];
	}

	int curSize()
	{
		return heapSize;
	}

	void deleteKey(int i);

	void insertKey(int x);
private:
	vector<int> arr;
	int maxSize;
	int heapSize;
};

MaxHeap::MaxHeap(int totSize)
{
	heapSize = 0;
	maxSize = totSize;
	arr.resize(totSize);
}

void MaxHeap::insertKey(int x)
{
	if (heapSize == maxSize)
	{
		cout << "\nOverflow: Could not insertKey\n";
		return;
	}

	heapSize++;
	int i = heapSize - 1;
	arr[i] = x;

	while (i != 0 && arr[parent(i)] < arr[i])
	{
		swap(arr[i], arr[parent(i)]);
		i = parent(i);
	}
}

void MaxHeap::increaseKey(int i, int newVal)
{
	arr[i] = newVal;

	while (i != 0 && arr[parent(i)] < arr[i]) 
	{
		swap(arr[i], arr[parent(i)]);
		i = parent(i);
	}
}

int MaxHeap::removeMax()
{
	if (heapSize <= 0)
		return INT_MIN;

	if (heapSize == 1) 
	{
		heapSize--;
		return arr[0];
	}

	int root = arr[0];
	arr[0] = arr[heapSize - 1];
	heapSize--;

	//root의 두 자식이 서브 트리 중에서 가장 큰 값들이다.
	//이유는 insert 과정에서 큰 값들이 위로 올라가기 때문이다.
	//가장 마지막 원소를 root로 보낸 후, MaxHeapify 함수를 통해 원소들의
	//대소를 비교하여 정렬한다.
	MaxHeapify(0);

	return root;
}

void MaxHeap::deleteKey(int i)
{
	increaseKey(i, INT_MAX);
	removeMax();
}

void MaxHeap::MaxHeapify(int i)
{
	int l = lChild(i);
	int r = rChild(i);
	int largest = i;
	if (l < heapSize && arr[l] > arr[i])
		largest = l;
	if (r < heapSize && arr[r] > arr[largest])
		largest = r;
	if (largest != i)
	{
		swap(arr[i], arr[largest]);
		MaxHeapify(largest);
	}
}
