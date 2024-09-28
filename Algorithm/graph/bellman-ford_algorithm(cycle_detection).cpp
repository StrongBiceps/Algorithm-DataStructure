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

	//V-1���� �ݺ��� �Ϸ�Ǿ������� �Ÿ��� ������Ʈ�Ǵ� ��찡 �߻��Ѵٸ� ����Ŭ�� ���̴�.
	for (auto& e : edges)
	{
		if (distance[e.src] == UNKNOWN)
			continue;

		if (distance[e.dst] > distance[e.src] + e.weight)
		{
			cout << "���� ����ġ ����Ŭ �߰�" << endl;
			return {};
		}
	}

	return distance;
}

int main()
{
	int V = 6;              // ���� ����
	vector<Edge> edges;     // ���� �������� ����

	vector<vector<int>> edge_map{ // {���� ����, ��ǥ ����, ����ġ}
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
		cout << "[" << start << "�� �������κ��� �ּ� �Ÿ�]" << endl;

		for (int i = 0; i < distance.size(); i++)
		{
			if (distance[i] == UNKNOWN)
				cout << i << "�� ����: �湮���� ����!" << endl;
			else
				cout << i << "�� ����: " << distance[i] << endl;
		}
	}
}