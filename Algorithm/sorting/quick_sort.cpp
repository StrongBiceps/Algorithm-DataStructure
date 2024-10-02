//퀵 정렬은 평균,최선의 경우 O(n log n)의 시간 복잡도를 갖는다.
//하지만 피벗을 최소,최댓값으로 고르는 최악의 경우 O(n^2)의 시간 복잡도를 갖는다.

// 1.퀵 정렬은 최악의 경우를 제외하고 병합 정렬, 힙 정렬과 동일한 시간 복잡도를 가져 다른 정렬 알고리즘보다 빠르다.
// 2.하지만 최악의 경우 O(n^2)까지 걸리기 때문에 불안정하다.
// 3.퀵 정렬은 추가적인 공간을 요구하지 않는다.

#include <iostream>
#include <vector>
using namespace std;

template<typename T>
auto partition(typename vector<T>::iterator begin, typename vector<T>::iterator end)
{
	//세 개의 반복자를 생성한다.
	//하나는 피벗을 가리키고, 나머지 둘은 벡터의 시작과 마지막 원소를 가리킨다.
	auto pivot_val = *begin;
	auto left_iter = begin + 1;
	auto right_iter = end;

	//피벗을 중심으로 피벗보다 작거나 같은 원소들은 피벗의 왼쪽으로 이동시키고,
	//피벗보다 큰 원소들은 피벗의 오른쪽으로 이동시킨다.
	while (true)
	{	
		//벡터의 첫 번째 원소부터 시작하여 피벗보다 큰 원소를 찾는다.
		while (*left_iter <= pivot_val && distance(left_iter, right_iter) > 0)
			++left_iter;

		//벡터의 마지막 원소부터 시작하여 역순으로 피벗보다 작은 원소를 찾는다.
		while (*right_iter > pivot_val && distance(left_iter, right_iter) > 0)
			--right_iter;

		//만약 left_iter와 right_iter가 같다면, 교환할 원소가 없음을 의미한다.
		//그렇지 않으면 left_iter와 right_iter가 가리키는 원소를 서로 교환한다.
		if (left_iter == right_iter)
		{
			break;
		}
		else
		{
			iter_swap(left_iter, right_iter);
		}
	}

	if (pivot_val > *right_iter)
	{
		iter_swap(begin, right_iter);
	}

	return right_iter;
}

template<typename T>
void quick_sort(typename vector<T>::iterator begin, typename vector<T>::iterator last)
{
	//만약 벡터에 하나 이상의 원소가 있다면 분할 작업을 수행한다.
	if (distance(begin, last) >= 1)
	{
		auto partition_iter = partition<T>(begin, last);

		//분할 작업에 의해 생성된 벡터를 재귀적으로 정렬
		quick_sort<T>(begin, partition_iter - 1);
		quick_sort<T>(partition_iter, last);
	}
}

template<typename T>
void print_vector(vector<T> arr)
{
	for (const auto& i : arr)
	{
		cout << i << " ";
	}
	cout << endl;
}

void run_quick_sort_test()
{
	vector<int> S1{45, 1, 3, 1, 2, 3, 45, 5, 1, 2, 44, 5, 7};
	vector<float> S2{45.6f, 1.0f, 3.8f, 1.01f, 2.2f, 3.9f, 45.3f, 5.5f, 1.0f, 2.0f, 44.0f, 5.0f, 7.0f};
	vector<double> S3{45.6, 1.0, 3.8, 1.01, 2.2, 3.9, 45.3, 5.5, 1.0, 2.0, 44.0, 5.0, 7.0 };
	vector<char> C{'b', 'z', 'a', 'e', 'f', 't', 'q', 'u', 'y'};

	cout << "정렬되지 않은 입력 벡터" << endl;
	print_vector<int>(S1);
	print_vector<float>(S2);
	print_vector<double>(S3);
	print_vector<char>(C);

	//arr.end()는 맨 마지막 원소 다음을 가리키므로 end()-1을 전달한다.
	quick_sort<int>(S1.begin(), S1.end() - 1);
	quick_sort<float>(S2.begin(), S2.end() - 1);
	quick_sort<double>(S3.begin(), S3.end() - 1);
	quick_sort<char>(C.begin(), C.end() - 1);

	cout << "퀵 정렬에 의해 정렬된 벡터" << endl;
	print_vector<int>(S1);
	print_vector<float>(S2);
	print_vector<double>(S3);
	print_vector<char>(C);
	cout << endl;

	vector<int> temp{5, 4, 3, 2, 1};
	quick_sort<int>(temp.begin(), temp.end() - 1);
	print_vector<int>(temp);

}

int main()
{
	run_quick_sort_test();

	return 0;
}

/*
sudo code

function partition(begin, end):
	pivot_val = value at begin  // 피벗 값 설정
	left_iter = begin + 1  // 왼쪽 반복자
	right_iter = end  // 오른쪽 반복자

	while true:
		// 피벗보다 큰 원소를 찾기 위해 왼쪽으로 탐색
		while value at left_iter <= pivot_val and distance(left_iter, right_iter) > 0:
			increment left_iter

		// 피벗보다 작은 원소를 찾기 위해 오른쪽으로 탐색
		while value at right_iter > pivot_val and distance(left_iter, right_iter) > 0:
			decrement right_iter

		if left_iter == right_iter:
			break  // 교환할 원소가 없으면 종료
		else:
			swap values at left_iter and right_iter  // 원소 교환

	if pivot_val > value at right_iter:
		swap values at begin and right_iter  // 피벗 위치 교환

	return right_iter  // 분할된 피벗의 위치 반환

function quick_sort(begin, last):
	if distance(begin, last) >= 1:  // 벡터에 하나 이상의 원소가 있으면
		partition_iter = partition(begin, last)  // 분할 작업 수행

		// 분할된 벡터를 재귀적으로 정렬
		quick_sort(begin, partition_iter - 1)  // 왼쪽 부분 정렬
		quick_sort(partition_iter, last)  // 오른쪽 부분 정렬

*/
