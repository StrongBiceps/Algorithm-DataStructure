#include <vector>
#include <algorithm>
#include <time.h>
#include <iomanip>
#include <iostream>

#define DEBUG 0

#if DEBUG
#define PRINT(x) cerr << x
#else
#define PRINT(x)
#endif

using namespace std;

const int UNKNOWN = -1;

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

	// 원소를 추가하지 않고 재귀 호출
	GetAllSubsets(set, subset, index + 1, allSubsets);

	// 원소를 부분집합에 추가한 후 재귀 호출
	subset.push_back(set[index]);
	GetAllSubsets(set, subset, index + 1, allSubsets);
}

bool SubsetSum_BruteForce(vector<int> set, int target)
{
	vector<vector<vector<int>>> allSubsets(set.size() + 1);

	GetAllSubsets(set, {}, 0, allSubsets);

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

bool SubsetSum_Backtracking(vector<int> set, int sum, int i)
{
	// 만약 현재 부분집합의 합이 target과 같다면
	if (sum == 0)
	{
		return true;
	}

	// 만약 현재 부분집합의 합이 target보다 크거나, 집합의 끝에 도달했다면
	if (i == set.size() || set[i] > sum)
	{
		return false;
	}

	// Case 1: sum에서 set[i]을 빼서 재귀 호출 (i번째 원소를 부분집합에 추가)
	// Case 2: sum을 그대로 전달하여 재귀 호출 (i번째 원소를 부분집합에 추가하지 않음)

	return SubsetSum_Backtracking(set, sum - set[i], i + 1)
		|| SubsetSum_Backtracking(set, sum, i + 1);
}

bool SubsetSum_Memoization(vector<int>& set, int sum, int i,
	vector<vector<int>>& memo)
{
	// 만약 현재 부분집합의 합이 target과 같다면
	if (sum == 0)
	{
		return true;
	}

	// 만약 현재 부분집합의 합이 target보다 크거나, 집합의 끝에 도달했다면
	if (i == set.size() || set[i] > sum)
	{
		return false;
	}

	// 현재 상태가 캐시에 있는지 확인
	if (memo[i][sum] == UNKNOWN)
	{
		// 현재 상태에 대한 솔루션을 구하여 캐시에 저장
		bool append = SubsetSum_Memoization(set, sum - set[i], i + 1, memo);
		bool ignore = SubsetSum_Memoization(set, sum, i + 1, memo);

		memo[i][sum] = append || ignore;
	}

	// 캐시에 저장된 값을 반환
	return memo[i][sum];
}

//이 함수는 정수 벡터 set을 인자로 받고, 2차원 부울형 벡터를 반환한다,
vector<vector<bool>> SubsetSum_Tabulation(vector<int>& set)
{
	int maxSum = 0;

	for (int i = 0; i < set.size(); i++)
	{
		maxSum += set[i];
	}

	//DP라는 이름의 2차원 부울형 벡터를 선언하여 사용한다. dp의 첫 번째 차원 크기는 입력 집합의 크기와 같고, 두 번째 차원의 크기는
	//입력 집합의 모든 원소 합보다 1만큼 크게 설정한다. dp의 모든 원소는 false로 초기화하되, 부분집합의 합이 0인 기저 조건에 대해서
	//는 true로 설정한다.

	//첫 번째 차원은 인덱스를 의미하고 두 번째 차원은 부분집합의 합을 의미한다.
	vector<vector<bool>> DP(set.size() + 1, vector<bool>(maxSum + 1, 0));

	//각 차원의 기저 조건인 인덱스 0을 true로 설정한다.
	for (int i = 0; i < set.size(); i++)
	{
		DP[i][0] = true;
	}

	//DP 테이블 원소 값을 설정하기 위해 이중 for반복문을 사용한다.

	//3 7 9 13 16 22 46 47 55 85 92 98 106 333 367 577 807 1058 3059


	//i에서는 i-1 인덱스까지의 조합이 sum을 만들 수 있다면 i에서도 sum을 만들 수 있다.
	//if-else로 경우가 나뉘는 이유는 sum값이 set[i-1]보다 같거나 크다면 i-1 이전 단계에서 이미 sum값이
	//만들어질 수 있는 경우도 있지만, i-1까지 원소를 모두 합해도 sum값보다 작은 경우가 있다. 
	//원소 9 이하의 경우를 생각해보면 된다. 7까지 3,7을 만들 수는 있지만 10은 7을 합해야 만들 수 있다.
	//따라서 7을 뺐을 때 만들어질 수 있는가를 보는 것이다. 만약 만들어질 수 있다면 7을 더하면  7에서 10까지 만들 수 있다. 또한 9에서도 해당 값을 
	//만들 수 있게 된다. DP[i-1][sum]을 검토하는 것은 set[i-1]이 더해지지 않았을 경우만 검토하기 때문에 if-else로 나뉜다.

	//결과적으로, 잘 생각해보면 sum < set[i-1] 인 상황에서 set[i-1]은 sum에 반드시 더해지지 않은 값이므로
	//sum[i-1]이 더해지지 않았을 경우만 생각하면 된다.

	//만약 인덱스 i=set.size일 때는 마지막 원소까지의 합이 DP[set.size]에 저장된다.
	for (int i = 1; i <= set.size(); i++)
	{
		for (int sum = 1; sum <= maxSum; sum++)
		{
			//인덱스 i=1일 때는 Set[0] = 3 이므로 2까지 첫 번째 if문이 실행된다.
			if (sum < set[i - 1])
			{
				cout << "<" << i << ">" << endl;

				cout << "sum(" << sum << ") < set[" << i - 1 << "](" << set[i - 1] << ") called" << endl;

				//가장 초기에는 sum=1이고, i도 1이다.
				//DP[1][1]=DP[0][1]이 된다.
				//DP[1][1]=false가 된다. DP[0][1]은 false이다.

				//인덱스 i=1에서는
				//DP[1][1~MAX] = DP[0][MAX]가 된다.
				auto temp = DP[i][sum];

				DP[i][sum] = DP[i - 1][sum];

				cout << "DP[" << i << "][" << sum << "](" << temp << ") = DP[" << i - 1 << "][" << sum << "](" << DP[i - 1][sum] << ") : " << DP[i][sum] << endl;
				cout << endl;
			}
			else // sum >= set[i-1]
			{
				cout << "<" << i << ">" << endl;

				cout << "else called" << endl;
				//i=1일 때 sum이 3이 되면 해당 코드가 실행된다.
				//DP[1][3]=DP[0][3] || DP[0][0] 이 된다.

				//이 코드를 분석해보면, 인덱스 i에서 부분집합의 합 sum >= set[i]일 때
				//DP[i-1][sum] = true 즉 하나 이전 인덱스에서 해당 sum값을 만들 수 있다면
				//i에서도 sum을 만들 수 있다.

				//i가 가리키는 값이 9까지만 이 함수의 진행 상황을 기록해보자.
				//먼저 DP[1]에는 3이 true로 설정될 것이다.
				//i가 2가 되면 DP[2]에는 3 7 10이 true로 설정될 것이다.
				//10이 true로 설정되는 과정은 DP[1][10]은 불가능하므로 
				//DP[1][3]으로 넘어간다. 그런데 위에서 DP[1][3]은 true이므로 DP[2][10]은 true로 설정된다.
				//따라서 sum < set[i-1]일 때는 set[i-1]이 더해진 결과를 생각할 필요 없이 set[i-1]에서 sum이 만들어질 수 있는지를
				//확인해야 한다. 하지만 set[i-1] <= sum일 때는 set[i-1]이 더해진 결과도 생각을 해봐야 한다. 따라서 set[i-1]에서
				//sum에서 set[i-1]이 빠졌을 때 값이 만들어질 수 있다면 i에서는 set[i-1]을 더해서 sum을 만들 수 있게 된다.
				//그런데 만약 set[i-1]을 뺐을 때도 sum이 만들어질 수 없다면 이미 이전에서 sum-set[i-1]이 만들어질 수 없으므로
				//sum은 i에서 만들어질 수 없다. 반복이 증가하면서 DP[1]에 3이 true로 설정되고 DP[2]에 3,7,10이 true로 설정되므로
				//index i에서는 sum-set[i-1]까지의 모든 경우의 수가 true로 바뀌므로 i단계에서는 sum-set[i-1]이 만들어질 수 있는지만
				//확인하면 된다. i이전의 경우를 전부 다시 계산해서 sum-set[i-1]이 가능한지 체크하지 않아도 된다.

				//sum >= set[i-1]일 때는 이전 원소까지만 조합했을 경우와, 현재 원소를 포함하여 조합했을 경우를 비교한다.
				//만약 이전 원소까지 조합했을 때 가능하다면 현재 원소는 계산하지 않아도 된다. 하지만 이전 원소까지 조합이
				//안된다면 sum이 현재 원소까지 더해진 값인지를 확인해야 한다. 이 코드보다 나중에 작성한 0 - 1 배낭 문제를
				//동적 계획법으로 해결한 코드와의 로직이 거의 똑같다. 단, 배낭 문제는 가격을 따져야 한다는 차이만 있다.
				DP[i][sum] = DP[i - 1][sum]
					|| DP[i - 1][sum - set[i - 1]];
				cout << "DP[" << i << "][" << sum << "](" << DP[i - 1][sum] << ") OR "
					<< "DP[" << i - 1 << "][" << sum << " - " << set[i - 1] << "](" << DP[i - 1][sum - set[i - 1]] << ")" << endl;

				cout << "DP[" << i << "][" << sum << "] : " << DP[i][sum] << endl;
				cout << endl;
			}
		}
	}

	return DP;
}

vector<string> types =
{
	"BRUTE FORCE",
	"BACKTRACKING",
	"MEMOIZATION",
	"TABULATION"
};

void GetTime(clock_t& timer, string type)
{
	// 현재 시간에서 timer를 빼서 경과 시간을 계산
	timer = clock() - timer;

	// 화면에 경과 시간 출력
	cout << type << " 방법 경과 시간: ";
	cout << fixed << setprecision(5) << (float)timer / CLOCKS_PER_SEC << endl;

	timer = clock(); // timer를 현재 시간으로 초기화
}

int main()
{
	/*vector<int> set = { 16, 1058, 22, 13, 46, 55, 3, 92, 47, 7,
					   98, 367, 807, 106, 333, 85, 577, 9, 3059 };
	int target = 6800;*/

	vector<int> set = { 3, 7 ,9 };
	int target = 19;
	int tests = 4;

	sort(set.begin(), set.end());

	for (int i = 0; i < tests; i++)
	{
		bool found = false;

		clock_t timer = clock();

		switch (i)
		{
		case 0:
			found = SubsetSum_BruteForce(set, target);
			break;
		case 1:
			found = SubsetSum_Backtracking(set, target, 0);
			break;
		case 2:
		{
			// 메모이제이션 테이블 초기화
			vector<vector<int>> memo(set.size(), vector<int>(7000, UNKNOWN));

			found = SubsetSum_Memoization(set, target, 0, memo);
			break;
		}
		case 3:
		{
			int total = 0;
			for (auto number : set)
				total += number;

			vector<vector<bool>> DP = SubsetSum_Tabulation(set);
			found = DP[set.size()][target];

			vector<int> subsetSums;
			for (int sum = 1; sum <= total; sum++)
			{
				if (DP[set.size()][sum])
				{
					subsetSums.push_back(sum);
				}
			}

			cout << "다음과 같이 " << subsetSums.size() << "가지의 부분집합의 합이 가능합니다:" << endl;

			for (auto sum : subsetSums)
				cout << sum << " ";
			cout << endl;

			break;
		}
		}

		if (found)
		{
			cout << "원소 합이 " << target << "인 부분집합이 있습니다." << endl;
		}
		else
		{
			cout << "원소 합이 " << target << "인 부분집합이 없습니다." << endl;
		}

		GetTime(timer, types[i]);
		cout << endl;
	}
}