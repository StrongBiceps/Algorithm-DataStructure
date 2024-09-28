#include <iostream>
#include <vector>
using namespace std;

template<typename T>
vector<T> merge(vector<T>& arr1, vector<T>& arr2)
{
	vector<T> merged;

	auto iter1 = arr1.begin();
	auto iter2 = arr2.begin();

	//�� ������ ���ҵ��� ���ϸ� ���� �ͺ��� merged�� push
	while (iter1 != arr1.end() && iter2 != arr2.end())
	{
		if (*iter1 < *iter2)
		{
			merged.emplace_back(*iter1);
			++iter1;
		}
		else
		{
			merged.emplace_back(*iter2);
			++iter2;
		}
	}

	//�� ������ ���Ұ� �����ִ� �� �ִٸ� ���� ���ҵ��� merged�� push
	if (iter1 != arr1.end())
	{
		for (; iter1 != arr1.end(); ++iter1)
			merged.emplace_back(*iter1);
	}
	else
	{
		for (; iter2 != arr2.end(); ++iter2)
			merged.emplace_back(*iter2);
	}

	return merged;
}

template<typename T>
vector<T> merge_sort(vector<T> arr)
{
	if (arr.size() > 1)
	{
		auto mid = size_t(arr.size() / 2);
		//begin���� begin()+mid �ٷ� �� ���ұ��� �����Ѵ�.
		auto left_half = merge_sort<T>(vector<T>(arr.begin(), arr.begin() + mid));
		//begin()+mid���� end()�ٷ� ������ �����Ѵ�.
		auto right_half = merge_sort<T>(vector<T>(arr.begin() + mid, arr.end()));

		return merge<T>(left_half, right_half);
	}
	return arr;
}

template<typename T>
void print_vector(vector<T> arr)
{
	for (const auto& i : arr)
		cout << i << " ";
	cout << endl;
}

void run_merge_sort_test()
{
	vector<int> S1{ 45, 1, 3, 1, 2, 3, 45, 5, 1, 2, 44, 5, 7 };
	vector<float> S2{ 45.6f, 1.0f, 3.8f, 1.01f, 2.2f, 3.9f, 45.3f, 5.5f, 1.0f, 2.0f, 44.0f, 5.0f, 7.0f };
	vector<double> S3{ 45.6, 1.0, 3.8, 1.01, 2.2, 3.9, 45.3, 5.5, 1.0, 2.0, 44.0, 5.0, 7.0 };
	vector<char> C{ 'b', 'z', 'a', 'e', 'f', 't', 'q', 'u', 'y' };

	cout << "���ĵ��� ���� �Է� ����" << endl;
	print_vector<int>(S1);
	print_vector<float>(S2);
	print_vector<double>(S3);
	print_vector<char>(C);

	auto sorted_S1 = merge_sort<int>(S1);
	auto sorted_S2 = merge_sort<float>(S2);
	auto sorted_S3 = merge_sort<double>(S3);
	auto sorted_C = merge_sort<char>(C);

	cout << "���� ���Ŀ� ���� ���ĵ� ����" << endl;
	print_vector<int>(sorted_S1);
	print_vector<float>(sorted_S2);
	print_vector<double>(sorted_S3);
	print_vector<char>(sorted_C);
}

int main()
{
	run_merge_sort_test();
	return 0;
}

/*
sudo code

function merge(arr1, arr2):
	merged = empty vector  // ���յ� ����� ������ ����

	iter1 = start of arr1  // arr1�� ���� �ݺ���
	iter2 = start of arr2  // arr2�� ���� �ݺ���

	// �� ������ ���ҵ��� ���Ͽ� ���� �ͺ��� merged�� �߰�
	while iter1 is not at end of arr1 and iter2 is not at end of arr2:
		if value at iter1 < value at iter2:
			add value at iter1 to merged
			increment iter1
		else:
			add value at iter2 to merged
			increment iter2

	// �����ִ� ���ҵ��� merged�� �߰�
	if iter1 is not at end of arr1:
		for each remaining element in arr1 from iter1 to end:
			add value at iter1 to merged
			increment iter1
	else:
		for each remaining element in arr2 from iter2 to end:
			add value at iter2 to merged
			increment iter2

	return merged  // ���յ� ��� ��ȯ

function merge_sort(arr):
	if size of arr > 1:
		mid = size of arr / 2  // �߰� �ε��� ���
		// ���� ������ ��������� ����
		left_half = merge_sort(subarray of arr from start to mid)
		// ������ ������ ��������� ����
		right_half = merge_sort(subarray of arr from mid to end)

		return merge(left_half, right_half)  // �� ������ �����Ͽ� ��ȯ
	return arr  // ũ�Ⱑ 1 ������ ��� ���� �迭 ��ȯ

*/