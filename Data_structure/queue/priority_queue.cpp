//Heap을 사용한 우선 순위 큐 구현.

#include <iostream>
#include <vector>
using namespace std;

class priority_queue
{
public:
	priority_queue(int maxSize);

	void Heapify(int);

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

	int removeTop();

	void updateKey(int i, int newVal, int newPriority);

	int getTop()
	{
		return arr[0];
	}

	int curSize()
	{
		return heapSize;
	}

	void deleteKey(int i);

	void insertKey(int val, int pr);
private:
	vector<int> arr;
	vector<int> priority;
	int maxSize;
	int heapSize;
};

priority_queue::priority_queue(int totSize)
{
	heapSize = 0;
	maxSize = totSize;
	arr.resize(totSize);
	priority.resize(totSize);
}

void priority_queue::insertKey(int val, int pr)
{
	if (heapSize == maxSize)
	{
		cout << "\nOverflow: Could not insertKey\n";
		return;
	}

	heapSize++;
	int i = heapSize - 1;
	arr[i] = val;
	priority[i] = pr;

	while (i != 0 && priority[parent(i)] < priority[i])
	{
		swap(arr[i], arr[parent(i)]);
		swap(priority[i], priority[parent(i)]);
		i = parent(i);
	}
}

void priority_queue::updateKey(int i, int newVal, int newPriority)
{
	arr[i] = newVal;
	priority[i] = newPriority;

	while (i != 0 && priority[parent(i)] < priority[i])
	{
		swap(arr[i], arr[parent(i)]);
		i = parent(i);
	}
}

int priority_queue::removeTop()
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
	priority[0] = priority[heapSize - 1];
	heapSize--;

	Heapify(0);

	return root;
}

void priority_queue::deleteKey(int i)
{
	updateKey(i, INT_MAX, INT_MAX);
	removeTop();
}

void priority_queue::Heapify(int i)
{
	int l = lChild(i);
	int r = rChild(i);
	int largest = i;

	if (l < heapSize && priority[l] > priority[i])
		largest = l;
	if (r < heapSize && priority[r] > priority[largest])
		largest = r;

	if (largest != i)
	{
		swap(arr[i], arr[largest]);
		swap(priority[i], priority[largest]);
		Heapify(largest);
	}
}

int main(void)
{
	priority_queue pr_queue(5);

	pr_queue.insertKey(1, 5);
	pr_queue.insertKey(2, 2);
	pr_queue.insertKey(3, 3);
	pr_queue.insertKey(4, 4);
	pr_queue.insertKey(5, 1);

	for (int i = 0; i < 5; ++i)
	{
		cout << pr_queue.removeTop();
	}

	return 0;
}