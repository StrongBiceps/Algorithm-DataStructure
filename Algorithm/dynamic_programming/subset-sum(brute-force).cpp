#include <iostream>
#include <vector>
#include <algorithm>

//부가적인 기능으로 DEBUG라는 전처리기 상수와 PRINT라는 매크로 함수를 정의하고, DEBUG가 0이 아닌 경우에만 stderr 출력을 수행하도록 설정한다.
#define DEBUG 1

#if DEBUG
#define PRINT(x) cerr << x
#else
#define PRINT(x)
#endif

using namespace std;

//집합의 모든 부분집합을 구하는 GetAllSubsets()함수를 정의한다. 이 함수는 두 개의 정수 벡터 Set과 Subset, 정수 index,
//그리고 모든 부분집합을 저장할 3차원 정수 벡터 allSubsets를 인자로 받는다. 이때 allSubsets는 참조로 전달한다. 이 함수는
//재귀 호출을 이용하여 set의 모든 부분집합을 생성한다.
void GetAllSubsets(vector<int> set, vector<int> subset,
	int index, vector<vector<vector<int>>>& allSubsets)
{
	// 집합 set의 끝에 도달한 경우
	if (index == set.size())
	{
		// 부분집합 크기를 인덱스로 사용하여 부분집합을 allSubsets에 추가
		allSubsets[subset.size()].push_back(subset);
		return;
	}

	//현재 인덱스를 부분집합에 추가할지 말지를 결정하는 논리를 재귀로 표현한 것이다.

	// 원소를 추가하지 않고 재귀 호출
	GetAllSubsets(set, subset, index + 1, allSubsets);

	// 원소를 부분집합에 추가한 후 재귀 호출
	subset.push_back(set[index]);
	GetAllSubsets(set, subset, index + 1, allSubsets);
}

//이 함수는 입력 집합을 표현하는 정수 벡터 set과 부분집합의 합을 나타내는 정수 target을 인자로 받고,bool 값을 반환한다. 
//이 함수의 앞부분에서 전체 부분집합을 저장할 allSubsets를 선언하고 GetAllSubsets()함수를 호출한다.
bool SubsetSum_BruteForce(vector<int> set, int target)
{
	//set.size()+1을 하는 이유는 set집합과 같은 크기의 부분 집합이 하나 존재하기 때문에
	//해당 인덱스를 사용해야 하기 때문이다.
	vector<vector<vector<int>>> allSubsets(set.size() + 1);

	GetAllSubsets(set, {}, 0, allSubsets);

	//각 부분집합의 원소 합을 target과 비교하고, 만약 일치하면 true를 반환한다. 만약 부분집합의 합이 target과 같은 경우가 발생하지 않으면
	//false를 반환한다.
	for (int size = 0; size <= set.size(); size++)
	{
		PRINT("부분집합의 원소 개수: " << size << endl);

		for (auto subset : allSubsets[size])
		{
			PRINT("\t{ ");

			int sum = 0;
			for (auto number : subset)
			{
				sum += number;
				PRINT(number << " ");
			}

			PRINT("} = " << sum << endl);

			if (sum == target)
				return true;
		}
	}

	return false;
}

int main()
{
	vector<int> set = { 13, 79, 45, 29 };
	int target = 58;

	bool found = SubsetSum_BruteForce(set, target);

	if (found)
	{
		cout << "원소 합이 " << target << "인 부분집합이 있습니다." << endl;
	}
	else
	{
		cout << "원소 합이 " << target << "인 부분집합이 없습니다." << endl;
	}
}