#include <algorithm>
#include <utility>
#include <iostream>
#include <vector>

#define DEBUG 1

#if DEBUG
#define PRINT(x) cerr << x
#else
#define PRINT(x)
#endif

using namespace std;

vector<vector<pair<int, int>>> found;


//이 함수는 두 개의 문자열 A와 B, 두 개의 정수 인덱스 i와 j, 그리고 정수 쌍 subsequence를 인자로 받고
//하나의 정수를 반환한다. 이 함수 바깥에 2차원 정수 쌍 벡터 found를 전역 변수로 선언한다.
int LCS_BruteForce(string A, string B, int i, int j,
	vector<pair<int, int>> subsequence)
{
	//A와 B는 바교할 두 문자열이고, i와 j는 A와 B 문자열의 인덱스를 나타낸다. 그리고 subsequence는 공통 부분 시퀀스를
	// 구성하는 인덱스 쌍을 저장하며, 출력을 위해 found 벡터에 추가된다. 이미 앞에서 의사 코드를 만들었으므로 의사 코드를
	// 주석으로 넣어두고, 그 아래에 실제 C++코드를 작성하면 쉽게 함수를 완성할 수 있다.

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

	 //두 문자가 같지 않았을 경우에 재귀가 두 갈래로 나뉘는데, 두 갈래 중에서 큰 subsequence 사이즈가 반환되야 한다.
	 //LCS는 최장 부분 공통 시퀀스이기 때문이다.
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
	//재귀를 두 옵션으로 진행하다 보면 연속적으로 중복된 문자열 시퀀스가 저장될 때가 있다. 따라서 중복을 제거해준다.
	//unique를 통해서 중복된 vector<pait<int,int>>를 found 끝으로 보낸다. unique는 끝으로 보내진 중복된 요소의 시작 iterator를
	//반환한다. 따라서 erase로 반환된 반복자부터 끝까지 지워버린다.
	//found.erase(unique(found.begin(), found.end()), found.end());

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

int main()
{
	string A, B;
	cin >> A >> B;

	int LCS = LCS_BruteForce(A, B, 0, 0, {});
	cout << A << "와 " << B << "의 최장 공통 부분 시퀀스 길이: " << LCS << endl;

#if DEBUG
	PrintSubsequences(A, B);
#endif
}

/*
ABCX ACYXB
*/