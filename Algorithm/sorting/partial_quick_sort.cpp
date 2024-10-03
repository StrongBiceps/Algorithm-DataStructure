#include <iostream>
#include <vector>
#include <chrono>
#include <random>
#include <algorithm>

template <typename T>
auto partition(typename std::vector<T>::iterator begin,
	typename std::vector<T>::iterator end)
{
	auto pivot_val = *begin;
	auto left_iter = begin + 1;
	auto right_iter = end;

	while (true)
	{
		while (*left_iter <= pivot_val && std::distance(left_iter, right_iter) > 0)
			left_iter++;

		while (*right_iter > pivot_val && std::distance(left_iter, right_iter) > 0)
			right_iter--;

		if (left_iter == right_iter)
			break;
		else
			std::iter_swap(left_iter, right_iter);
	}

	if (pivot_val > *right_iter)
		std::iter_swap(begin, right_iter);

	return right_iter;
}

template <typename T>
void partial_quick_sort(typename std::vector<T>::iterator begin,
	typename std::vector<T>::iterator last,
	size_t k)
{
	if (std::distance(begin, last) >= 1)
	{
		auto partition_iter = partition<T>(begin, last);
		partial_quick_sort<T>(begin, partition_iter - 1, k);

		// 만약 마지막 피벗 위치가 k보다 작으면 오른쪽 부분 벡터도 정렬
		//오른쪽 부분 벡터를 정렬할 때 k를 그대로 전달하면 또 다시 k개를 정렬하는 셈이 된다. 따라서
		//k에서 정렬한 원소만큼 빼줘야 나머지 개수를 정렬할 것이다. 해당 코드는 partial_quick_sort2함수에서 정의했다.
		if (std::distance(begin, partition_iter) < k)
			partial_quick_sort<T>(partition_iter, last, k);
	}
}

template <typename T>
void partial_quick_sort2(typename std::vector<T>::iterator begin,
	typename std::vector<T>::iterator last,
	size_t k)
{
	if (std::distance(begin, last) >= 1)
	{
		auto partition_iter = partition<T>(begin, last);

		partial_quick_sort2<T>(begin, partition_iter - 1, k);

		// 만약 마지막 피벗 위치가 k보다 작으면 오른쪽 부분 벡터도 정렬
		//만약 k개의 원소를 정확히 정렬한다는 생각으로 partition_iter + (k-distance) 만큼만 정렬한다는 생각을 하면 안된다.
		//예를 들어 다음과 같이 생각했을 경우이다.
		//5 1 3 9 7 6에서 k = 4라고 할 때, 왼쪽 부분 벡터가 정렬된 상태이고 오른쪽 부분 벡터를 정렬하려 할 때
		//1 3 5 9 ,  7 6   이렇게 3번째 까지 정렬되었으므로 오른쪽은 하나만 정렬하면 되는 거 아닌가 라고 생각할 수 있는데
		//이 생각은 잘못된 생각이다. 오른쪽 부분 시퀀스 전체를 보고 partition을 해야 6이 제자리에 올 수 있기 때문이다.
		//만약 위 생각처럼 코드를 짜면 k = 4 까지 정렬된 시퀀스는 1 3 5 9 가 될 것이다. 이것은 잘못된 결과이다.
		if (std::distance(begin, partition_iter) < k)
			partial_quick_sort2<T>(partition_iter, last, k - std::distance(begin, partition_iter));
	}
}

template <typename T>
void quick_sort(typename std::vector<T>::iterator begin,
	typename std::vector<T>::iterator last)
{
	if (std::distance(begin, last) >= 1)
	{
		auto partition_iter = partition<T>(begin, last);

		quick_sort<T>(begin, partition_iter - 1);
		quick_sort<T>(partition_iter, last);
	}
}

template <typename T>
void print_vector(std::vector<T> arr)
{
	for (auto i : arr)
		std::cout << i << " ";

	std::cout << std::endl;
}

template <typename T>
auto generate_random_vector(T size)
{
	std::vector<T> V;
	V.reserve(size)

	std::random_device rd;
	std::mt19937 rand(rd());

	std::uniform_int_distribution<std::mt19937::result_type> uniform_dist(1, size);

	for (T i = 0; i < size; i++)
		V.push_back(uniform_dist(rand));

	return std::move(V);
}

template <typename T>
void test_partial_quicksort(size_t size, size_t k)
{
	auto random_vec = generate_random_vector<T>(size);

	auto random_vec_copy(random_vec);

	std::cout << "입력 벡터: " << std::endl;
	print_vector<T>(random_vec);

	// 부분 퀵 정렬 알고리즘 수행 시간 측정
	auto begin1 = std::chrono::steady_clock::now();
	partial_quick_sort<T>(random_vec.begin(), random_vec.end() - 1, k);
	auto end1 = std::chrono::steady_clock::now();
	auto diff1 = std::chrono::duration_cast<std::chrono::microseconds>(end1 - begin1);

	std::cout << std::endl;
	std::cout << "부분 퀵 정렬 수행 시간: " << diff1.count() << "us" << std::endl;

	std::cout << "(처음 " << k << "개 원소만) 부분 정렬된 벡터: ";
	print_vector<T>(random_vec);

	//부분 퀵 정렬 2 실행 결과
	auto begin2 = std::chrono::steady_clock::now();
	partial_quick_sort2<T>(random_vec_copy.begin(), random_vec_copy.end() - 1, k);
	auto end2 = std::chrono::steady_clock::now();
	auto diff2 = std::chrono::duration_cast<std::chrono::microseconds>(end2 - begin2);

	std::cout << std::endl;
	std::cout << "부분 퀵 정렬2 수행 시간: " << diff2.count() << "us" << std::endl;

	std::cout << "부분 정렬된 벡터: ";
	print_vector<T>(random_vec_copy);
}

int main()
{
	test_partial_quicksort<unsigned>(100, 10);

	return 0;
}

/*
* 아래 결과를 살펴보면, 부분 퀵 정렬2 버전이 더 정확한 개수가 정렬된 것을 확인할 수 있다. 오른쪽 부분 벡터를 정렬할 때
* k를 그대로 전달하면 또 k개만큼 정렬하게 되므로 잘못된 코드이다.
*
* 입력 벡터:
86 80 51 94 66 84 43 40 59 87 26 3 94 80 44 2 85 37 52 63 48 4 69 56 16 86 46 23 91 48 2 79 42 89 98 15 2 23 18 17 37 66 99 82 88 26 21 99 49 55 94 31 62 14 26 33 52 30 56 39 85 36 71 55 21 19 22 91 10 74 48 56 11 57 28 43 92 33 65 89 23 57 66 23 96 93 14 18 15 82 53 97 77 96 81 38 68 49 100 69

부분 퀵 정렬 수행 시간: 368us
(처음 10개 원소만) 부분 정렬된 벡터: 2 2 2 3 4 10 11 14 14 15 15 16 17 18 18 19 21 21 22 23 23 23 23 26 26 28 26 30 38 48 46 56 42 48 48 52 37 56 44 57 37 55 53 36 39 49 40 43 49 55 43 31 56 33 51 33 52 57 62 82 85 82 71 66 77 81 79 86 69 74 63 85 80 68 59 84 66 69 65 66 80 86 89 92 96 93 91 94 99 88 99 97 98 96 89 91 94 87 100 94

부분 퀵 정렬2 수행 시간: 252us
부분 정렬된 벡터: 2 2 2 3 4 10 11 14 14 15 15 18 17 22 18 23 23 16 21 21 19 23 23 26 26 28 26 30 38 48 46 56 42 48 48 52 37 56 44 57 37 55 53 36 39 49 40 43 49 55 43 31 56 33 51 33 52 57 62 82 85 82 71 66 77 81 79 86 69 74 63 85 80 68 59 84 66 69 65 66 80 86 89 92 96 93 91 94 99 88 99 97 98 96 89 91 94 87 100 94
*/