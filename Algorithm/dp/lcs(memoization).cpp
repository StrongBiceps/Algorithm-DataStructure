#include <vector>
#include <algorithm>
#include <utility>
#include <time.h>
#include <iomanip>
#include <iostream>
#include <string>

/*재귀 트리
* ABC와 AXB를 대상으로 재귀를 작성한다.

LCS(3,3)
	memo[2][2] = max[(LCS,2,3),(LCS,3,2)] - 2
	LCS(2,3)
		memo[1][2] = 1+ LCS(1,2) - 2
		LCS(1,2)
			memo[0][1] = max[(LCS,0,2),(LCS,1,1)] - 1
			LCS(0,2) return 0
			LCS(1,1)
				memo[0][0] =1+ LCS(0,0) return 0 -1
	LCS(3,2)
		memo[2][1] = max[LCS(2,2),LCS(3,1)] - 1
		LCS(2,2)
			memo[1][1] = max[LCS(1,2),LCS(2,1)] - 1
			LCS(1,2)
				memo[0][1] = max[LCS(1,1),LCS(0,2)] -1
				LCS(1,1)
					memo[0][0] = 중복 return 1
				LCS(0,2) return 0
			LCS(2,1)
				memo[1][0] = max{LCS(1,1),LCS(2,0)] -1
				LCS(2,0) return 0
				LCS(1,1) 중복 return 1
		LCS(3,1)
			memo[2][0] = max[LCS(2,1),LCS(3,0)] - 1
			LCS(2,1) - 중복 return 1
			LCS(3,0) return 0

최종 결과 2

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

//이 함수의 인자 구성은 LCS_BruteForce()와 거의 같고, 다만 마지막 인자 subsequence를 2차원 정수 벡터 참조형
//memo로 변경했다. 또한 i와 j는 문자열 A와 B 각각의 size이다.

//하향식 방법을 통해서 각 문자열의 끝부분부터 같은지 여부를 검토한다. 같다면 이전 문자열까지의 공통 부분 개수에 +1을
//한 결과가 된다. 같지 않다면 각 문자열에 각각 1을 뺀 경우를 번갈아서 재귀로 검토한 뒤 두 값중 큰 값이 공통 부분 시퀀스의 값이 된다.
//중간 과정에서 중복된 부분에 대한 재귀는 캐시에 의해서 제거된다.
int LCS_Memoization(string A, string B, int i, int j,
	vector<vector<int>>& memo)
{
	//함수 코드 구성은 BruteForce와 거의 유사하다. 다만 두 문자열의 부분 문자열을 재귀적으로 탐색하고, 각 단계에서
	//memo 테이블에 결과를 저장한다.

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

	//max함수는 두 값이 같다면 둘 중 하나를 반환한다.
	memo[i - 1][j - 1] = max(LCS_Memoization(A, B, i - 1, j, memo),
		LCS_Memoization(A, B, i, j - 1, memo));

	return memo[i - 1][j - 1];
}

vector<string> types =
{
	"BRUTE FORCE",
	"MEMOIZATION",
	"TABULATION"
};

//성능 비교를 위해 GetTime함수를 정의한다.
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

	int tests = 2;


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
		}

		cout << A << "와 " << B << "의 최장 공통 부분 시퀀스 길이: " << LCS << endl;

		GetTime(timer, types[i]);
		cout << endl;
	}
}

/*
123456 QWERTY
= 0

ACBEBC ABCBC
= 4

AZYBYXCXW ZZAYYBXXXCWW
= 6

ABCABDBEFBA ABCBEFBEAB
= 8

ABZCYDABAZADAEA YABAZADBBEAAECYACAZ
= 10
*/