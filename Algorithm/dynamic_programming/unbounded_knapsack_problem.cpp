#include <vector>
#include <iostream>
#include <algorithm>

/*
*아이템 개수
3

무게
8 4 10

가격
10 20 11

가방 최대 무게
18


for(int w = 0; w <= 18; w++)
	for(int i = 0;i<items;++i)


<w = 0일 때>

무게 0을 구성할 수 있는 가격은 없다.
따라서 안쪽 for문은 모두 생략된다.
w = 3까지의 반복은 모두 생략된다.

<w = 4일 때>

*i = 0일 때

첫 번째 아이템의 무게는 8이므로 if문에 걸리지 않는다.

*i = 1일 때

DP[4] = max(DP[4],DP[0]+20)
	  = max(0,20)

무게 4를 구성할 수 있는 경우는 두 번째 아이템만을 넣는 경우이다. 이후 반복은 모두 생략된다.

<w = 8일 때>

*i = 1일 때

DP[8] = max(DP[8],DP[0]+10)
	  = max(0,10)

*i = 2일 

DP[8] = max(DP[8],DP[4]+20)

이 처럼 무게 8을 구성할 수 있는 가격은 각 무게에서 모든 아이템에 대해서 고려한다.
i = 1일 때보다 i = 2일 때(물건 두 개를 중복해서 넣었을 때)가 더 가격이 높다.

0 - 1 문제일 때는 중복이 허용되지 않기 때문에 현재 아이템을 추가할지 말지 두 경우로 나누어서 생각한다.
하지만 unbounded문제일 경우는 아이템을 중복해서 넣을 수 있기 때문에 차원을 하나만 설정하고 각 무게에 대해서
모든 경우의 수를 조합하여 가능한 무게를 테이블에 업데이트한다. 따라서 현재 물건을 넣을지 말지에 대해서 생각할
필요는 없다.

중복을 허용하기 때문에, 차원은 하나만 존재하면 된다. 0-1 배낭 문제는 아이템이 중복이 허용되지 않기 때문에, 내부 for문을
weight에 대해서 진행하고, 무한 개수 배낭 문제는 아이템의 중복 제한이 없기 때문에 아이템의 모든 조합을 탐색해야 한다. 따라서
내부 for문을 아이템으로 진행한다.
*/
using namespace std;

int UnboundedKnapsack(int items, int capacity, vector<int> values,
	vector<int> weight)
{
	//DP테이블을 1차원 정수형 벡터로 생성한다. 이 벡터의 크기는 (capacity + 1)로 지정하고, 모든 원소는 0으로 초기화한다.
	vector<int> DP(capacity + 1, 0);

	//0-1 배낭 문제와 마찬가지로 상태 로직은 이중 for반복문을 사용한다. 그러나 이번에는 바깥쪽 반복문을 0부터 capacity까지 
	//증가하고, 안쪽 반복문은 물건의 인덱스를 사용한다. 이제 상태를 저장하는 방법을 결정해야 한다. 선택된 물건에 의해 용량을 
	//초과하면 안된다는 점을 기억해야 한다. DP테이블에서 무게를 0부터 capacity까지 표현할 수 있으므로, w와 weight[i]의 차이가 음수가 아닌지
	//확인해야 한다. 따라서 다음과 같이 하나의 if문을 사용하여 로직을 구현할 수 있다.
	for (int w = 0; w <= capacity; w++)
	{
		for (int i = 0; i < items; i++)
		{
			if (weight[i] <= w)
			{
				DP[w] = max(DP[w], DP[w - weight[i]] + values[i]);
			}
		}
	}

	return DP[capacity];
}

int main()
{
	int items, capacity;
	cin >> items >> capacity;

	vector<int> values(items), weight(items);

	for (auto& v : values) cin >> v;
	for (auto& w : weight) cin >> w;

	int result = UnboundedKnapsack(items, capacity, values, weight);

	cout << "배낭에 채울 수 있는 물건들의 최고 가격: " << result << endl;
}
