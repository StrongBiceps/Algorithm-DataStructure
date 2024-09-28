class segment_tree
{
public:
	//constructor
	segment_tree() {}
	segment_tree(const int& n, vector<int>& _arr)
	{
		tree_height = (size_t)ceil(log2(n));
		tree_size = (size_t)(1 << (tree_height + 1));

		tree.resize(tree_size);

		arr = _arr;

		init_tree(1, 0, n - 1);
	}

	//interface
public:
	int sum(int left, int right)
	{
		//1��° ���Һ��� 3��° ������ ���� ���Ѵٸ�, �ε����� ��ȯ�� �� 0~2�̱� ������ -1�� ���ش�.
		return sum_impl(1, 0, arr.size() - 1, left - 1, right - 1);
	}
	void update(int Index, int value)
	{
		int diff = value - arr[Index];

		update_impl(1, 0, arr.size() - 1, Index, diff);
	}

	//implementation
private:
	int init_tree(int node, int start, int end)
	{
		if (start == end)
			return tree[node] = arr[start];

		int mid = (start + end) / 2;
		int left_result = init_tree(node * 2, start, mid);
		int right_result = init_tree(node * 2 + 1, mid + 1, end);

		tree[node] = left_result + right_result;

		return tree[node];
	}
	int sum_impl(int node, int start, int end, int left, int right)
	{
		if (left > end || right < start)
			return 0;

		//0��°���� 5��° ���� ���Ѵٰ� ���� ��, ���� ����� ������ 1~3��° ����� �ϴ� ���� �����
		//���� ���� ���ԵǾ�� �ϹǷ� �׳� ���� ��� ���� �����Ѵ�.
		if (left <= start && end <= right)
			return tree[node];

		int mid = (start + end) / 2;
		int left_result = sum_impl(node * 2, start, mid, left, right);
		int right_result = sum_impl(node * 2 + 1, mid + 1, end, left, right);

		return left_result + right_result;
	}
	void update_impl(int node, int start, int end, int Index, int diff)
	{
		if (start > Index || end < Index)
			return;

		tree[node] += diff;

		if (start != end)
		{
			int mid = (start + end) / 2;
			update_impl(node * 2, start, mid, Index, diff);
			update_impl(node * 2 + 1, mid + 1, end, Index, diff);
		}
	}
private:
	vector<int> tree;
	vector<int> arr;
	size_t tree_size{ 0 };
	size_t tree_height{ 0 };
};