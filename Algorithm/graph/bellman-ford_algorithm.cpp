#include <vector>
#include <iostream>
#include <climits>

//벨만 포드 알고리즘은 모든 에지에 대해서 최대 V-1번 거리를 업데이트하기 때문에 매 반복마다 더 짧은 경로를 찾을 수 있다.
//따라서 음수 가중치가 있는 그래프에서 적합하며 다익스트라의 단점을 보완할 수 있다. 음수 가중치가 없는 그래프에서는
//다익스트라 알고리즘을 사용하는 것이 효율적이다. V-1번의 검토는 양수 가중치 그래프에서는 불필요한 반복이기 때문이다.
using namespace std;

class Edge
{
public:
	int src;
	int dst;
	int weight;
};

const int UNKNOWN = INT_MAX;

//이제 벨만 포드 알고리즘을 구현한 BellmanFord() 함수를 구현한다. 이 함수는 그래프를 표현하는 에지 리스트
//edges, 정점 개수 V, 출발 정점 번호 Start를 인자로 받고, 거리 값 배열을 반환한다. 이 함수 내부에서는 V개 크기의
//거리 값 배열 distance를 사용하고, 이 배열의 초깃값은 UNKNOWN으로 지정한다. 다만 출발 정점에 대해서는 distance값은
//0으로 설정한다.

vector<int> BellmanFord(vector<Edge> edges, int V, int start)
{
	vector<int> distance(V, UNKNOWN);
	distance[start] = 0;

	//알고리즘의 대부분은 다음 단계에서 수행한다. 이 단계에서는 (V-1)번 반복되는 루프를 정의하고, 각 루프에서는
	//전체 에지를 검토하면서 각 정점의 거리 값을 업데이트한다. 구체적으로 설명하면 각각의 에지 중에서 시작 정점의 
	//거리 값이 UNKNOWN이 아닌 경우에 대해서만 거리 값을 업데이트하며, 이때(시작 정점 거리 값+에지 가중치) 계산 결과가
	//목적지 정점의 거리 값보다 작으면 목적지 정점의 거리 값을 (시작 정점 거리 값+ 에지 가중치)로 교체한다.

	//(V-1)번 반복
	for (int i = 0; i < V - 1; ++i)
	{
		//전체 에지에 대해 반복
		for (auto& e : edges)
		{
			//에지의 시작 정점이 UNKNOWN값이면 스킵
			//즉 한 번이라도 거리가 업데이트 된 정점일 때에만 다시 거리 업데이트를 진행한다.
			//거리가 무한인 정점에 대해서도 거리를 바로 업데이트 해버리면 의도치 않은 상황이 발생한다.
			//예를들어 1->2->3->4<-5 의 그래프가 있다고 하자. 3->4까지는 정상적으로 업데이트가 이루어진다.
			//만약 5(무한대)->4의 거리가 업데이트 되어 버리면 1->5->4가 가능하다는 상황이 된다. 하지만
			//1에서 바로 5로 갈 수는 없다. 즉 거리가 한 번이라도 업데이트 된 상태라는 것은 해당 정점까지
			//경로가 이어져 있고, 거리가 계산된 상태라는 것이다.
			//위 경우에서 i = 5 일 때 5->4의 거리가 업데이트된다.
			//i = 5일 때가 아니라 start가 5일 때에 거리가 업데이트될 것이다.

			//즉 다른 정점에 의해서 해당 정점의 거리가 업데이트된 상태일 때만 거리를 업데이트할 수 있다.
			//5번 정점은 시작 정점일 때를 제외하고는 다른 정점이 도달할 수 없으므로 5번을 거치는 경로는 거리가 업데이트되지 않는다.
			if (distance[e.src] == UNKNOWN)
			{
				continue;
			}
			//에지의 시작 정점의 거리 값이 새로운 경로에 의한 거리 값보다 크면
			//거리 값을 업데이트함
			if (distance[e.dst] > distance[e.src] + e.weight)
			{
				distance[e.dst] = distance[e.src] + e.weight;
			}
		}
	}
	return distance;
}

int main()
{
	int V = 5;              // 정점 개수
	vector<Edge> edges;     // 에지 포인터의 벡터

	vector<vector<int>> edge_map{ // {시작 정점, 목표 정점, 가중치}
		{0, 1, 3},
		{1, 2, 5},
		{1, 3, 10},
		{3, 2, -7},
		{2, 4, 2}
	};

	for (auto& e : edge_map)
	{
		edges.emplace_back(Edge{ e[0], e[1], e[2] });
	}

	int start = 0;
	vector<int> distance = BellmanFord(edges, V, start);

	cout << "[" << start << "번 정점으로부터 최소 거리]" << endl;

	for (int i = 0; i < distance.size(); i++)
	{
		if (distance[i] == UNKNOWN)
			cout << i << "번 정점: 방문하지 않음!" << endl;
		else
			cout << i << "번 정점: " << distance[i] << endl;
	}
}