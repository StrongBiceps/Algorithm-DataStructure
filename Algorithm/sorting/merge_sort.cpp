#include <iostream>
#include <vector>
using namespace std;

template<typename T>
vector<T> merge(vector<T>& arr1, vector<T>& arr2)
{
	vector<T> merged;

	auto iter1 = arr1.begin();
	auto iter2 = arr2.begin();

	//두 벡터의 원소들을 비교하며 작은 것부터 merged에 push
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

	//두 벡터중 원소가 남아있는 게 있다면 남은 원소들을 merged에 push
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
		//begin부터 begin()+mid 바로 전 원소까지 전달한다.
		auto left_half = merge_sort<T>(vector<T>(arr.begin(), arr.begin() + mid));
		//begin()+mid부터 end()바로 전까지 전달한다.
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

	cout << "정렬되지 않은 입력 벡터" << endl;
	print_vector<int>(S1);
	print_vector<float>(S2);
	print_vector<double>(S3);
	print_vector<char>(C);

	auto sorted_S1 = merge_sort<int>(S1);
	auto sorted_S2 = merge_sort<float>(S2);
	auto sorted_S3 = merge_sort<double>(S3);
	auto sorted_C = merge_sort<char>(C);

	cout << "병합 정렬에 의해 정렬된 벡터" << endl;
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
	merged = empty vector  // 병합된 결과를 저장할 벡터

	iter1 = start of arr1  // arr1의 시작 반복자
	iter2 = start of arr2  // arr2의 시작 반복자

	// 두 벡터의 원소들을 비교하여 작은 것부터 merged에 추가
	while iter1 is not at end of arr1 and iter2 is not at end of arr2:
		if value at iter1 < value at iter2:
			add value at iter1 to merged
			increment iter1
		else:
			add value at iter2 to merged
			increment iter2

	// 남아있는 원소들을 merged에 추가
	if iter1 is not at end of arr1:
		for each remaining element in arr1 from iter1 to end:
			add value at iter1 to merged
			increment iter1
	else:
		for each remaining element in arr2 from iter2 to end:
			add value at iter2 to merged
			increment iter2

	return merged  // 병합된 결과 반환

function merge_sort(arr):
	if size of arr > 1:
		mid = size of arr / 2  // 중간 인덱스 계산
		// 왼쪽 절반을 재귀적으로 정렬
		left_half = merge_sort(subarray of arr from start to mid)
		// 오른쪽 절반을 재귀적으로 정렬
		right_half = merge_sort(subarray of arr from mid to end)

		return merge(left_half, right_half)  // 두 절반을 병합하여 반환
	return arr  // 크기가 1 이하인 경우 원래 배열 반환

*/