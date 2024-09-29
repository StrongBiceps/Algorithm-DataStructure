#include <iostream>
#include <climits>
#include <vector>

using namespace std;

// 벨만-포드 알고리즘으로 S를 추가한 그래프의 거리 값을 업데이트한다.
// ->각 에지의 가중치를 변형식을 이용하여 변형한다.
// ->다익스트라 알고리즘으로 가중치가 양의 정수 값이된 그래프의 각 정점의 최단 거리를 계산
// ->계산된 거리 값에 다시 역으로 변형식을 적용하여 원래 거리 값으로 변환
// 즉 음수 가중치가 있는 그래프를 다익스트라 알고리즘을 사용하여 거리 값을 구할 수 있게 한 
// 알고리즘이다.

// 정점 S를 추가하는 이유는 가중치가 0인 에지를 각 정점에 연결함으로써 거리의 값을 모두 0이거나 음수로 바꿔주고,
// 또한 S를 모든 그래프에 연결했으므로 모든 경로가 S로부터 시작하고, 이동 경로상에서 각 정점의 거리 값이 일정한 관계를
// 유지한다. 이로 인해서 변형식을 적용했을 때 거리 값이 서로 상쇄된다. 

// 다익스트라 알고리즘이 A->D 경로를 다음과 같이 탐색할 경우, 수식은 다음과 같다.
// w(AB) = w(AB) + d[s,A] - d[s,B]
// w(BC) = w(BC) + d[s,B] - d[s,C]
// ...
// 위 수식을 보면 d[s,B]가 상쇄되는 것을 확인할 수 있다. 따라서 더미 정점 s를 추가하는 이유는 위와 같이
// 망원 속성을 사용하여 A -> D 경로의 중간 경로와 관계 없이 w(AB)+w(BC)+...+w(DE)+d[s,A]-d[s,E] 이 같이 식이 상쇄할 수 있다는 것이다.
// 또한 각 에지의 가중치를 양수로 만들 수 있으므로 다익스트라를 활용하기 위해서 더미 정점 s를 추가한다.
// w(AB)+w(BC)+...+w(DE)+d[s,A]-d[s,E] 이 같이 식이 상쇄될 수 있다면, 다시 가중치를 조정하기 전으로 되돌리기 쉽다.
// d[s,A]는 빼주고 d[s,E]는 더해주면 다시 원래 경로 식이 복원된다.

//	w(uv) = w(uv) + d[s,u] - d[s,v] 에서 w(uv) + d[s,u] >= d[s,v] 인 이유는
//  더미 정점 s를 추가한 후, s를 시작점으로 하고 벨만 포드 알고리즘을 사용하여 모든 정점에 대한 최단 거리를 업데이트한다.
//	결론적으로, s -> v 의 최단 거리인 d[s,v]가 s -> u의 최단 거리인 d[s,u]에 w(uv)를 더한 값보다 클 수가 없다.
//	s -> v의 최단 거리는 이미 계산이 끝난 상황인데, w(uv) + d[s,u] 보다 거리 값이 크게 나올 수 없다.
//  이러한 수학적 논리를 사용하여 모든 에지의 가중치를 양수로 바꿔주고, 다익스트라 알고리즘을 사용하여 최단 경로를 구한다.

struct Edge
{
	int src;
	int dst;
	int weight;
};

const int UNKNOWN = INT_MAX;

bool HasNegativeCycle(const vector<Edge>& edges, vector<int> distance)
{
	for (auto& e : edges)
	{
		if (distance[e.src] == UNKNOWN)
			continue;

		if (distance[e.dst] > distance[e.src] + e.weight)
			return true;
	}

	return false;
}

//에지 리스트 edges와 정점 개수 V를 인자로 받고, 시작 정점 번호 Start는 사용하지 않는다.
vector<int> BellmanFord(vector<Edge> edges, int V)
{
	//더미 정점 S를 추가한다. 더미 정점 S를 추가하므로 거리 배열은(V+1)크기로 준비하고, S정점과 나머지 정점 사이에
	//가중치가 0인 에지를 추가한다.
	vector<int> distance(V + 1, UNKNOWN);

	int s = V;
	for (int i = 0; i < V; i++)
	{
		edges.push_back(Edge{ s, i, 0 });
	}

	distance[s] = 0;

	// 정점 개수가 V + 1개 이므로 V번 반복
	for (int i = 0; i < V; i++)
	{
		for (auto& e : edges)
		{
			// 에지의 시작 정점의 거리 값이 UNKNOWN이면 스킵
			if (distance[e.src] == UNKNOWN)
				continue;

			// 인접한 정점의 거리 값이 새로운 경로에 의한 거리 값보다 크면
			// 거리 값을 업데이트함.
			// 벨만-포드 알고리즘에서 반환되는 모든 거리 값은
			//distance[e.dst] <= distance[e.src] + e.weight을 만족함
			if (distance[e.dst] > distance[e.src] + e.weight)
			{
				distance[e.dst] = distance[e.src] + e.weight;
			}
		}
	}

	// 음수 가중치 사이클이 있는 지 검사
	if (HasNegativeCycle(edges, distance))
	{
		cout << "음수 가중치 사이클 발견!" << endl;
		return {};
	}

	return distance;
}

int GetMinDistance(vector<int>& distance, vector<bool>& visited)
{
	int minDistance = UNKNOWN;
	int minIndex = -1;

	//처음 호출되었을 때는 start만 거리가 0이므로 다른 정점에 대해서는
	//if문을 수행하지 않고 minDistance는 0이 되고 minIndex는 start의 index가 된다.
	for (int i = 0; i < distance.size(); i++)
	{
		if (!visited[i] && distance[i] <= minDistance)
		{
			minDistance = distance[i];
			minIndex = i;
		}
	}

	return minIndex;
}

//다익스트라 알고리즘 구현은 단순히 for문을 사용해서 구현할 수 있다. 다익스트라 알고리즘은
//매 단계마다 최소 거리 값을 갖는 정점을 찾아야 한다. 이전에는 우선순위 큐를 사용했지만, 여기서는
//단순히 GetMinDistance함수를 사용하여 최소 거리 정점을 찾도록 한다.
vector<int> Dijkstra(vector<Edge> edges, int V, int start)
{
	vector<int> distance(V, UNKNOWN);
	vector<bool> visited(V, false);

	distance[start] = 0;

	for (int i = 0; i < V - 1; i++)
	{
		// 방문하지 않은 정점 중에서 최소 거리 정점을 찾음
		// 처음에는 시작 정점의 Index가 반환됨
		// 두 번째 반복에서는 이전 반복에서의 curr의 인접 정점들의 거리
		// 값이 업데이트된 상태이다. 나머지 정점은 UNKNOWN이므로 
		// 최소 거리 정점도 이전 curr의 인접 정점중 하나가 반환될 것이다.
		// 그들의 거리 값은 무조건 UNKNOWN보다는 작기 때문이다.
		int curr = GetMinDistance(distance, visited);

		//curr정점을 방문 처리한다.
		visited[curr] = true;

		for (auto& e : edges)
		{
			//curr정점의 인접한 정점만 고려
			if (e.src != curr)
				continue;

			// 이미 방문했으면 무시
			// 벨만 포드와 다른 점이 여기에 있다. 
			// 다익스트라는 이미 방문한 정점에 대해서는 거리를 업데이트하지 않는다.
			// 하지만 음수 가중치가 없기 때문에 방문한 정점에 대해서는 무시해도 무방하다.
			if (visited[e.dst])
				continue;

			if (distance[curr] != UNKNOWN &&
				distance[e.dst] > distance[curr] + e.weight)
			{
				distance[e.dst] = distance[curr] + e.weight;
			}
		}
	}

	return distance;
}

void Johnson(vector<Edge> edges, int V)
{
	// 더미 정점을 추가한 그래프에서 최단 거리를 계산
	vector<int> h = BellmanFord(edges, V);

	if (h.empty())
		return;

	for (auto& e : edges)
	{
		e.weight += (h[e.src] - h[e.dst]);
	}

	// 모든 정점들 사이의 최단 거리를 저장
	vector<vector<int>> shortest(V);

	//에지의 가중치가 변경된 상태에서 다익스트라 알고리즘을 호출하여 각 정점의 거리 값을 다시 갱신한다.
	for (int i = 0; i < V; i++)
	{
		shortest[i] = Dijkstra(edges, V, i);
	}

	// 가중치 변환 수식을 역으로 적용하여 최단 거리를 출력
	// 위에서 구한 shortest는 최종 최단 거리는 아니다. 이유는 가중치 변환 수식을 사용하여 모든 
	// 에지 가중치를 양수로 변환했기 때문이다. 따라서 가중치 변환 수식을 역으로 적용하여 실제 최단 거리 값을 다시 계산해야 한다.
	for (int i = 0; i < V; i++)
	{
		cout << i << ":\n";

		for (int j = 0; j < V; j++)
		{
			if (shortest[i][j] != UNKNOWN)
			{
				//w(AB)+w(BC)+...+w(DE)+d[s,A]-d[s,E]이므로 d[s,A]는 빼주고 d[s,E]는 더해주면 다시
				//가중치들의 합의 식으로 바뀐다.
				shortest[i][j] += h[j] - h[i];

				cout << "\t" << j << ": " << shortest[i][j] << endl;
			}
		}
	}
}

int main()
{
	int V = 5;              // 정점 개수
	vector<Edge> edges;     // 에지 포인터의 벡터

	vector<vector<int>> edge_map{ // {시작 정점, 목표 정점, 가중치}
		{0, 1, -7},
		{1, 2, -2},
		{2, 0, 10},
		{0, 3, -5},
		{0, 4, 2},
		{3, 4, 4}
	};

	for (auto& e : edge_map)
	{
		edges.emplace_back(Edge{ e[0], e[1], e[2] });
	}

	Johnson(edges, V);
}