#include <vector>
#include <iostream>
#include <climits>

using namespace std;

class Edge
{
public:
	int src;
	int dst;
	int weight;
};


const int UNKNOWN = INT_MAX;


vector<int> BellmanFord(vector<Edge> edges, int V, int start)
{
	vector<int> distance(V, UNKNOWN);
	distance[start] = 0;

	for (int i = 0; i < V - 1; ++i)
	{
		cout << "outter" << endl;

		for (auto& e : edges)
		{
			if (distance[e.src] == UNKNOWN)
			{
				cout << "continue" << endl;
				continue;
			}

			if (distance[e.dst] > distance[e.src] + e.weight)
			{
				cout << "update" << endl;
				distance[e.dst] = distance[e.src] + e.weight;
			}
		}
	}

	//V-1번의 반복이 완료되었음에도 거리가 업데이트되는 경우가 발생한다면 사이클인 것이다.
	for (auto& e : edges)
	{
		if (distance[e.src] == UNKNOWN)
			continue;

		if (distance[e.dst] > distance[e.src] + e.weight)
		{
			cout << "음수 가중치 사이클 발견" << endl;
			return {};
		}
	}

	return distance;
}

int main()
{
	int V = 6;              // 정점 개수
	vector<Edge> edges;     // 에지 포인터의 벡터

	vector<vector<int>> edge_map{ // {시작 정점, 목표 정점, 가중치}
		{0, 1, 3},
		{1, 3, -8},
		{2, 1, 3},
		{2, 5, 5},
		{3, 2, 3},
		{2, 4, 2},
		{4, 5, -1},
		{5, 1, 8}
	};

	for (auto& e : edge_map)
	{
		edges.emplace_back(Edge{ e[0], e[1], e[2] });
	}

	int start = 0;
	vector<int> distance = BellmanFord(edges, V, start);

	if (!distance.empty())
	{
		cout << "[" << start << "번 정점으로부터 최소 거리]" << endl;

		for (int i = 0; i < distance.size(); i++)
		{
			if (distance[i] == UNKNOWN)
				cout << i << "번 정점: 방문하지 않음!" << endl;
			else
				cout << i << "번 정점: " << distance[i] << endl;
		}
	}
}