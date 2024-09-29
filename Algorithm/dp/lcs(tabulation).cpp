#include <vector>
#include <algorithm>
#include <utility>
#include <time.h>
#include <iomanip>
#include <iostream>
#include <string>

/*
* ABC와 AXB를 이용하여 재귀를 작성한다.


i = 0
	for j to B.size
	기저 - DP[0][j] = 0

i = 1
	for j to B.size

	j = 0
	기저 - DP[1][0] = 0

	j = 1
	같음 - DP[1][1] = DP[0][0]+1 - 1

	j = 2
	다름 - DP[1][2] = DP[1][1] - 1 중복

	j = 3
	다름 - DP[1][3] = DP[1][2] - 1 중복

i = 2
	for j to B.size

	j = 0
	기저 - DP[2][0] = 0

	j = 1
	다름 - DP[2][1] = DP[1][1] - 1 중복

	j = 2
	다름 - DP[2][2] = DP[2][1] - 1 중복

	j = 3
	같음 - DP[2][3] = DP[1][2] + 1 - 2 중복

i = 3
	for j to B.size

	j = 0
	기저 - DP[3][0] = 0

	j = 1
	나머지 다 다름 ...

	모든 경우의 수에 대해 LCS 길이를 DP에 저장하고 마지막으로 Reconstruct 함수를 호출하여 최장 공통 부분 시퀀스를 출력한다.
*/
#define DEBUG 0

#if DEBUG
#define PRINT(x) cerr << x
#else
#define PRINT(x)
#endif

using namespace std;

vector<vector<pair<int, int>>> found;

int LCS_BruteForce(string A, string B, int i, int j,
	vector<pair<int, int>> subsequence)
{
	// 만약 i가 A의 길이보다 커지거나 또는 j가 B의 길이보다 커지면:
	if (i >= A.size() || j >= B.size())
	{
		found.push_back(subsequence);

		// 재귀를 종료하고 부분 시퀀스의 길이를 반환합니다.
		return subsequence.size();
	}

	// 만약 A[i] = B[j] 이면:
	if (A[i] == B[j])
	{
		// 부분 시퀀스 길이를 1만큼 증가합니다.
		subsequence.push_back({ i, j });

		// i와 j를 각각 1씩 증가합니다.
		return LCS_BruteForce(A, B, i + 1, j + 1, subsequence);
	}

	/* 그렇지 않으면:

	   옵션 1) (i + 1)번째와 j번째 문자에 대해 검사를 진행합니다.
	   옵션 2) i번째와 (j + 1)번째 문자에 대해 검사를 진행합니다.

	   이 상태의 LCS는 옵션 1 및 옵션 2의 최댓값과 같습니다.
	 */

	return max(LCS_BruteForce(A, B, i + 1, j, subsequence),
		LCS_BruteForce(A, B, i, j + 1, subsequence));
}

void PrintSubsequences(string A, string B)
{
	// 람다 함수를 이용한 정렬
	sort(found.begin(), found.end(), [](auto a, auto b)
		{
			// 부분 시퀀스 길이를 기준으로 정렬
			if (a.size() != b.size())
			{
				return a.size() < b.size();
			}

	// 두 부분 시퀀스 길이가 같다면 인덱스의 사전 순서로 정렬
	return a < b;
		});

	// 중복 제거
	found.erase(unique(found.begin(), found.end()), found.end());

	int previousSize = 0;

	for (auto subsequence : found)
	{
		if (subsequence.size() != previousSize)
		{
			previousSize = subsequence.size();
			PRINT("SIZE = " << previousSize << endl);
		}

		// 밑줄을 이용하여 문자의 자리를 표현
		string a_seq(A.size(), '_');
		string b_seq(B.size(), '_');

		for (auto pair : subsequence)
		{
			// 빈칸을 부분 문자열 문자로 채우기
			a_seq[pair.first] = A[pair.first];
			b_seq[pair.second] = B[pair.second];
		}

		PRINT("\t" << a_seq << " " << b_seq << endl);
	}
}

const int UNKNOWN = -1;

int LCS_Memoization(string A, string B, int i, int j,
	vector<vector<int>>& memo)
{
	// 기저 조건 - 빈 문자열에 대해서는 0을 반환
	if (i == 0 || j == 0)
	{
		return 0;
	}

	// 두 문자열의 부분 문자열에 대해 결과가 저장되어 있으면 반환
	// Have we found a result for the prefixes of the two strings?
	if (memo[i - 1][j - 1] != UNKNOWN)
	{
		return memo[i - 1][j - 1];
	}

	// A와 B의 두 부분 문자열에서 맨 마지막 문자가 같은지 확인
	if (A[i - 1] == B[j - 1])
	{
		// 이 경우 A와 B의 부분 문자열에서 구한 LCS 길이는 각 부분 문자열에서 
		// 마지막 한 문자를 제외한 문자열로부터 구한 LCS 길이에 1을 더한 것과 같음

		memo[i - 1][j - 1] = 1 + LCS_Memoization(A, B, i - 1, j - 1, memo);

		// 테이블에 저장된 결과를 반환
		return memo[i - 1][j - 1];
	}

	// A와 B의 두 부분 문자열에서 맨 마지막 문자가 같지 않다면
	// A의 부분 문자열에서 마지막 문자를 제외한 것과 B의 부분 문자열에서 구한 LCS 길이,
	// B의 부분 문자열에서 마지막 문자를 제외한 것과 A의 부분 문자열에서 구한 LCS 길이 중
	// 최댓값을 선택하여 지정

	memo[i - 1][j - 1] = max(LCS_Memoization(A, B, i - 1, j, memo),
		LCS_Memoization(A, B, i, j - 1, memo));

	return memo[i - 1][j - 1];
}

//최장 공통 부분 시퀀스를 출력하고 싶다면 어떻게 해야 할까? 물론 전수 조사 방식은 최장 공통 부분 시퀀스 자체를 출
//력할 수 있지만, 너무 비효율적이다. 다행히도 앞서 구한 DP 테이블에 대해 백트래킹을 수행함으로써 최장 공통 부분
//시퀀스를 쉽게 구할 수 있다.

//다음은 백트래킹 의사 코드이다.

/*
* 만약 i = 0 또는 j = 0:
*		빈 문자열을 반환한다.
*
* 만약 A의 부분 문자열과 B의 부분 문자열 맨 마지막 문자가 같다면:
*		두 부분 문자열에서 한 문자씩 뺀 부분 문자열에서 LCS를 구하고, 공통 문자를 뒤에 추가한다.
* 그렇지 않으면:
*		만약 DP(i-1,j) 값이 DP(i,j-1)보다 크다면:
*				A의 부분 문자열에서 한 문자를 제외한 부분 문자열과 B의 부분 문자열의 LCS를 구한다.
*		그렇지 않으면:
*				B의 부분 문자열에서 한 문자를 제외한 부분 문자열과 A의 부분 문자열의 LCS를 구한다.
*/
string ReconstructLCS(vector<vector<int>> DP, string A, string B, int i, int j)
{
	/*
	* 만약 ABC와 AXB를 입력으로 사용한다면,
	* 마지막 문자가 다르고 DP[3][2]보다 DP[2][3]이 더 크므로 ReconstructLCS(2,3) 호출
	* 두 문자가 같으므로 ReconstructLCS(1,2) 호출
	*
	* DP[0][2]는 기저 조건이므로 Reconstruct(1,1) 호출
	* 두 문자는 같으므로 Reconstruct(0,0)호출 -> 재귀 종료
	*/
	if (i == 0 || j == 0)
	{
		return "";
	}

	if (A[i - 1] == B[j - 1])
	{
		return ReconstructLCS(DP, A, B, i - 1, j - 1) + A[i - 1];
	}
	else if (DP[i - 1][j] > DP[i][j - 1])
	{
		return ReconstructLCS(DP, A, B, i - 1, j);
	}
	else
	{
		return ReconstructLCS(DP, A, B, i, j - 1);
	}
}

//LCS함수에서는 먼저 DP테이블을 정의한다. 이 테이블은 2차원 정수 벡터로 표현할 수 있다. 이때 첫 번째 차원 크기는 문자열
//A의 길이보다 1크게 지정하고, 두 번째 차원 크기는 문자열 B보다 1크게 지정한다.
string LCS_Tabulation(string A, string B)
{
	vector<vector<int>> DP(A.size() + 1, vector<int>(B.size() + 1));

	for (int i = 0; i <= A.size(); i++)
	{
		for (int j = 0; j <= B.size(); j++)
		{
			//부분집합의 합 문제와는 달리 기저 조건은 for반복문 전에 처리하지 않고 매번
			//반복문 안에서 처리한다. A또는 B문자열이 비어 있는 경우가 기저 조건이고, 이는 i=0
			//또는 j=0인 경우이기 때문이다. 기저 조건을 처리하는 소스 코드는 다음과 같다.
			if (i == 0 || j == 0)
			{
				DP[i][j] = 0;
			}
			//두 문자가 같을 경우에는 LCS 길이는 맨 마지막 문자를 제외한 두 부분 문자열의 LCS 길이에 1을 더한
			//결과이다.
			else if (A[i - 1] == B[j - 1])
			{
				DP[i][j] = DP[i - 1][j - 1] + 1;
			}
			//마지막으로 두 부분 문자열의 마지막 문자가 같지 않은 경우를 처리해야 한다. 이 경우 LCS길이는
			//A의 부분 문자열에서 마지막 문자를 제외한 것과 B의 부분 문자열에서 구한 LCS길이, 그리고 B의 부분
			//문자열에서 마지막 문자를 제외한 것과 A의 부분 문자열에서 구한 LCS길이 둘 중의 최댓값이다. 타뷸레이션
			//기법을 적용할 경우 특정 상태의 LCS길이는 한 행 위의 LCS길이와 한 열 위쪽의 LCS 길이의 최댓값과 같다.
			else
			{
				DP[i][j] = max(DP[i - 1][j], DP[i][j - 1]);
			}
		}
	}

	string lcs = ReconstructLCS(DP, A, B, A.size(), B.size());

	return lcs;
}

vector<string> types =
{
	"BRUTE FORCE",
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
	string A, B;
	cin >> A >> B;

	int tests = 3;


	for (int i = 0; i < tests; i++)
	{
		int LCS;

		clock_t timer = clock();

		switch (i)
		{
		case 0:
		{
			LCS = LCS_BruteForce(A, B, 0, 0, {});

#if DEBUG
			PrintSubsequences(A, B);
#endif
			break;
		}
		case 1:
		{
			vector<vector<int>> memo(A.size(), vector<int>(B.size(), UNKNOWN));
			LCS = LCS_Memoization(A, B, A.size(), B.size(), memo);

			break;
		}
		case 2:
		{
			string lcs = LCS_Tabulation(A, B);

			LCS = lcs.size();

			cout << A << "와 " << B << "의 최장 공통 부분 시퀀스: " << lcs << endl;
			break;
		}
		}

		cout << A << "와 " << B << "의 최장 공통 부분 시퀀스 길이: " << LCS << endl;

		GetTime(timer, types[i]);
		cout << endl;
	}
}

/*
A1B2C3D4E ABCDE
5
ABCDE

ABZCYDABAZADAEA YABAZADBBEAAECYACAZ
10
YABAZADAEA

QWJEHFBEMVJEIIFJEEVFBEHFJXAJXBE BVBQHEJEJCHEEHXBNEBCHHEHHFEHSBE
14
QHEJEJEEBEHFBE

AAA12AAA3AA4AA56AA7AAA8 AA1AA2AAA3A4A5A6AA7A89AAA
19
AA12AAA3A4A56AA7AAA

 */