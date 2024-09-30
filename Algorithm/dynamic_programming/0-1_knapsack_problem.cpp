#include <iostream>
#include <vector>
#include <algorithm>

using namespace std;

/*
* 아이템 개수
3

무게
8 4 10

가격
10 20 11

가방 최대 무게
18


<i = 1 일 때>

cw = 8
cv = 10

*totalw = 7 까지는 totalw < currentw 이고
DP[0][0~7] = 0이므로

DP[1][0~7] = DP[i-1][0~7] = 0

-지금까지의 아이템으로는 만들 수 없는 무게들이다.


*total = 8일 때

DP[1][8] = max(DP[0][8],DP[0][0]+cv)
	 = 10

-첫 번째 아이템을 추가했을 경우이다.


*total = 9 이후부터는 구성할 수 없는 무게지만 가격은 설정된다.

DP[1][9~18] = max(DP[0][9~18],DP[0][1~10]+cv)
전부 10으로 설정된다.

-이 무게들은 첫 번째 아이템만 고려했을 경우에는 첫 번째 아이템만으로 채워지는 무게들이다.
즉 가방에 가능한 물품을 전부 담았지만 채우지 못하는 무게들은 가격을 가능한 물품들을 최대로 채웠을 경우의
가격으로 업데이트 되는 의미이다.

<i = 2일 때>

cw = 4
cv = 20

*totalw = 3까지는 totalw < currentw 이고
DP[1][0~3] = 0이므로

DP[2][0~3] = DP[1][0~3]
	   = 0

-두 번째 아이템까지 고려했을 경우에도 만들 수 없는 무게들이다.


*totalw=4 일 때

DP[2][4] = max(DP[1][4],DP[1][0]+cv)
	=20

-두 번째 아이템까지 고려했을 경우 두 번째 아이템을 추가하지 않았을 경우와 추가했을 경우를 비교한다.


*totalw=5~7 까지는 구성할 수 없는 무게이고
DP[1][5~7] = 0이다. 또 DP[1][5~7 - 4 = 1~3]+cv -> DP[1][1~3]+cv이므로 테이블이 20으로 업데이트될 것이다.

DP[2][5~7] = 20

*totalw=8 일 때

DP[2][8] = max(DP[1][8],DP[1][4]+cv)
	=20


*totalw=9~11 까지는 구성할 수 없는 무게이다.
DP[2][9~11] = max(DP[1][9~11],DP[1][5~7]+cv)

DP[2][9~11] =max(10,20)
	   =20


*totalw=12 일 때

DP[2][12] = max(DP[1][12],DP[1][8]+cv)
	  = 30


*totalw = 13부터는 구성할 수 없는 무게이다.

DP[2][13~18] = max(DP[1][13~18],DP[1][9~14]+cv)
		=max(10,30)

DP[2][13~18] = 30


<i = 3일 때>

cw = 10
cv = 11

*totalw = 9까지는 DP[2][0~9]가 적용될 것이다.

DP[3][0~9] = DP[2][0~9]

DP[3][4] = DP[2][4]
	 = 20

DP[3][8] = DP[2][8]
	 = 20


*totalw = 10일 때

DP[3][10] = max(DP[2][10],DP[2][0]+cv)
	  = 20


*totalw = 11 ~ 13

DP[3][11~13] = max(DP[2][11~13],DP[2][1~3]+cv)

DP[3][11] = max(DP[2][11],DP[2][1]+cv)
	  = max(20,11)
	  = 20

DP[3][12] = max(DP[2][12],DP[2][2]+cv)
	  = max(30,11)
	  = 30

DP[3][13] = max(DP[2][13],DP[2][3]+cv)
	  = max(30,11)
	  = 30

DP[3][14] = max(DP[2][14],DP[2][4]+cv)
	  = max(30,31)
	  = 31

DP[3][15] ~ DP[3][18] 까지는 31로 설정된다.


위 코드를 분석해보면 각 아이템을 고려했을 경우에서 모든 무게에 대해서 테이블에 업데이트한다. 물론 만들 수
없는 무게이거나 가방에 담았지만 채우지 못하는 무게에 대해서는 0과 현재까지의 최대 무게로 업데이트된다.
또 각 무게에 대해서 현재 아이템보다 큰 total 무게에 대해서는 현재 아이템을 추가한 경우와 추가하지 않은
경우를 비교하여 큰 값을 저장한다. 때로는 이전까지의 물품들로 구성한 가격이 현재 물품을 추가한 가격보다
클 경우가 있다.

이전 물품들로 이룬 가방 무게 == 이전 물품과 현재 물품을 더해서 이룬 가방 무게
둘 중에서 어떤 것이 더 비싼 가방인지 체크하는 것이다.

만약 현재 무게가 total 무게보다 크다면 현재 아이템은 total 무게에 포함될 수 없다. 따라서 현재
아이템을 추가하는 경우는 생각할 수 없다. 그러므로 결국 이전 아이템까지 조합된 가격이 될 것이다.

DP[i]에는 모든 무게에 대하여 이전 아이템과 i까지 포함했을 경우에 대한 모든 경우의 가격이 다 저장되어 있다.
따라서 index가 하나 작은 DP값을 가져와서 대입하는 것이다.

이 로직은 SubsetSum using Tabulation 코드와 완전히 똑같다. else문에서
DP를 업데이트하는 방식만 다를 뿐이다.
*/

int Knapsack_01(int items, int capacity, vector<int> values,
	vector<int> weight)
{
	vector<vector<int>> DP(items + 1, vector<int>(capacity + 1, 0));

	for (int i = 1; i <= items; i++)
	{
		int currentWeight = weight[i - 1];
		int currentValue = values[i - 1];

		for (int totalWeight = 1; totalWeight <= capacity; totalWeight++)
		{
			//total_weight가 currentweight보다 작다는 것은 total_weight는 currentweight가 더해진 무게가 아니라는 것이다.
			//즉 현재 아이템의 무게가 7인데 total_weoght가 5일 때는 이전 아이템까지 고려했을 때의 경우밖에 없다는 것이다.
			//만약 total_weight가 current_weight보다 클 경우는 현재 아이템을 추가한 경우와 추가하지 않았을 경우 무게가 
			//어떤 것이 더 큰지를 결정해야 한다.
			if (totalWeight < currentWeight)
			{
				DP[i][totalWeight] = DP[i - 1][totalWeight];
			}
			else
			{
				//DP[i-1][totalWeight]는 현재 아이템을 추가하지 않고 이전 아이템까지의 경우에서 totalweight일 때의 가격이다.
				//부분집합의 합 처럼 set[i-1]을 더하는 것과 더하지 않는 것으로 나뉘듯이 현재 아이템을 추가했을 때와 추가하지
				//않았을 때 어떤 경우의 가격이 더 큰 지를 판단한 뒤 현재 DP값을 업데이트한다.
				DP[i][totalWeight] = max(DP[i - 1][totalWeight],
					DP[i - 1][totalWeight - currentWeight] + currentValue);
			}
		}
	}

	return DP[items][capacity];
}

int main()
{
	//두 개의 정수형 변수 items와 capacity를 선언하고, 표준 입력으로 값을 지정한다. items는 전체 물건
	//개수를 나타내고, capacity는 배낭의 제한 용량을 나타낸다. 그리고 value와 weight 벡터에 각 물건의 가격과
	//무게 정보를 저장한다.
	int items, capacity;
	cin >> items >> capacity;

	vector<int> values(items), weight(items);

	for (auto& v : values) cin >> v;
	for (auto& w : weight) cin >> w;

	int result = Knapsack_01(items, capacity, values, weight);

	cout << "배낭에 채울 수 있는 물건들의 최고 가격: " << result << endl;
}

/*
	8 66
	20 4 89 12 5 50 8 13
	5 23 9 72 16 14 32 4
	returns 180.
*/