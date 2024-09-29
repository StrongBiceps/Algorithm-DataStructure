#include <iostream>
#include <climits>
#include <vector>

using namespace std;

// ����-���� �˰��������� S�� �߰��� �׷����� �Ÿ� ���� ������Ʈ�Ѵ�.
// ->�� ������ ����ġ�� �������� �̿��Ͽ� �����Ѵ�.
// ->���ͽ�Ʈ�� �˰��������� ����ġ�� ���� ���� ���̵� �׷����� �� ������ �ִ� �Ÿ��� ���
// ->���� �Ÿ� ���� �ٽ� ������ �������� �����Ͽ� ���� �Ÿ� ������ ��ȯ
// �� ���� ����ġ�� �ִ� �׷����� ���ͽ�Ʈ�� �˰������� ����Ͽ� �Ÿ� ���� ���� �� �ְ� �� 
// �˰������̴�.

// ���� S�� �߰��ϴ� ������ ����ġ�� 0�� ������ �� ������ ���������ν� �Ÿ��� ���� ��� 0�̰ų� ������ �ٲ��ְ�,
// ���� S�� ��� �׷����� ���������Ƿ� ��� ��ΰ� S�κ��� �����ϰ�, �̵� ��λ󿡼� �� ������ �Ÿ� ���� ������ ���踦
// �����Ѵ�. �̷� ���ؼ� �������� �������� �� �Ÿ� ���� ���� ���ȴ�. 

// ���ͽ�Ʈ�� �˰������� A->D ��θ� ������ ���� Ž���� ���, ������ ������ ����.
// w(AB) = w(AB) + d[s,A] - d[s,B]
// w(BC) = w(BC) + d[s,B] - d[s,C]
// ...
// �� ������ ���� d[s,B]�� ���Ǵ� ���� Ȯ���� �� �ִ�. ���� ���� ���� s�� �߰��ϴ� ������ ���� ����
// ���� �Ӽ��� ����Ͽ� A -> D ����� �߰� ��ο� ���� ���� w(AB)+w(BC)+...+w(DE)+d[s,A]-d[s,E] �� ���� ���� ����� �� �ִٴ� ���̴�.
// ���� �� ������ ����ġ�� ����� ���� �� �����Ƿ� ���ͽ�Ʈ�� Ȱ���ϱ� ���ؼ� ���� ���� s�� �߰��Ѵ�.
// w(AB)+w(BC)+...+w(DE)+d[s,A]-d[s,E] �� ���� ���� ���� �� �ִٸ�, �ٽ� ����ġ�� �����ϱ� ������ �ǵ����� ����.
// d[s,A]�� ���ְ� d[s,E]�� �����ָ� �ٽ� ���� ��� ���� �����ȴ�.

//	w(uv) = w(uv) + d[s,u] - d[s,v] ���� w(uv) + d[s,u] >= d[s,v] �� ������
//  ���� ���� s�� �߰��� ��, s�� ���������� �ϰ� ���� ���� �˰������� ����Ͽ� ��� ������ ���� �ִ� �Ÿ��� ������Ʈ�Ѵ�.
//	���������, s -> v �� �ִ� �Ÿ��� d[s,v]�� s -> u�� �ִ� �Ÿ��� d[s,u]�� w(uv)�� ���� ������ Ŭ ���� ����.
//	s -> v�� �ִ� �Ÿ��� �̹� ����� ���� ��Ȳ�ε�, w(uv) + d[s,u] ���� �Ÿ� ���� ũ�� ���� �� ����.
//  �̷��� ������ ������ ����Ͽ� ��� ������ ����ġ�� ����� �ٲ��ְ�, ���ͽ�Ʈ�� �˰������� ����Ͽ� �ִ� ��θ� ���Ѵ�.

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

//���� ����Ʈ edges�� ���� ���� V�� ���ڷ� �ް�, ���� ���� ��ȣ Start�� ������� �ʴ´�.
vector<int> BellmanFord(vector<Edge> edges, int V)
{
	//���� ���� S�� �߰��Ѵ�. ���� ���� S�� �߰��ϹǷ� �Ÿ� �迭��(V+1)ũ��� �غ��ϰ�, S������ ������ ���� ���̿�
	//����ġ�� 0�� ������ �߰��Ѵ�.
	vector<int> distance(V + 1, UNKNOWN);

	int s = V;
	for (int i = 0; i < V; i++)
	{
		edges.push_back(Edge{ s, i, 0 });
	}

	distance[s] = 0;

	// ���� ������ V + 1�� �̹Ƿ� V�� �ݺ�
	for (int i = 0; i < V; i++)
	{
		for (auto& e : edges)
		{
			// ������ ���� ������ �Ÿ� ���� UNKNOWN�̸� ��ŵ
			if (distance[e.src] == UNKNOWN)
				continue;

			// ������ ������ �Ÿ� ���� ���ο� ��ο� ���� �Ÿ� ������ ũ��
			// �Ÿ� ���� ������Ʈ��.
			// ����-���� �˰����򿡼� ��ȯ�Ǵ� ��� �Ÿ� ����
			//distance[e.dst] <= distance[e.src] + e.weight�� ������
			if (distance[e.dst] > distance[e.src] + e.weight)
			{
				distance[e.dst] = distance[e.src] + e.weight;
			}
		}
	}

	// ���� ����ġ ����Ŭ�� �ִ� �� �˻�
	if (HasNegativeCycle(edges, distance))
	{
		cout << "���� ����ġ ����Ŭ �߰�!" << endl;
		return {};
	}

	return distance;
}

int GetMinDistance(vector<int>& distance, vector<bool>& visited)
{
	int minDistance = UNKNOWN;
	int minIndex = -1;

	//ó�� ȣ��Ǿ��� ���� start�� �Ÿ��� 0�̹Ƿ� �ٸ� ������ ���ؼ���
	//if���� �������� �ʰ� minDistance�� 0�� �ǰ� minIndex�� start�� index�� �ȴ�.
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

//���ͽ�Ʈ�� �˰����� ������ �ܼ��� for���� ����ؼ� ������ �� �ִ�. ���ͽ�Ʈ�� �˰�������
//�� �ܰ踶�� �ּ� �Ÿ� ���� ���� ������ ã�ƾ� �Ѵ�. �������� �켱���� ť�� ���������, ���⼭��
//�ܼ��� GetMinDistance�Լ��� ����Ͽ� �ּ� �Ÿ� ������ ã���� �Ѵ�.
vector<int> Dijkstra(vector<Edge> edges, int V, int start)
{
	vector<int> distance(V, UNKNOWN);
	vector<bool> visited(V, false);

	distance[start] = 0;

	for (int i = 0; i < V - 1; i++)
	{
		// �湮���� ���� ���� �߿��� �ּ� �Ÿ� ������ ã��
		// ó������ ���� ������ Index�� ��ȯ��
		// �� ��° �ݺ������� ���� �ݺ������� curr�� ���� �������� �Ÿ�
		// ���� ������Ʈ�� �����̴�. ������ ������ UNKNOWN�̹Ƿ� 
		// �ּ� �Ÿ� ������ ���� curr�� ���� ������ �ϳ��� ��ȯ�� ���̴�.
		// �׵��� �Ÿ� ���� ������ UNKNOWN���ٴ� �۱� �����̴�.
		int curr = GetMinDistance(distance, visited);

		//curr������ �湮 ó���Ѵ�.
		visited[curr] = true;

		for (auto& e : edges)
		{
			//curr������ ������ ������ ����
			if (e.src != curr)
				continue;

			// �̹� �湮������ ����
			// ���� ����� �ٸ� ���� ���⿡ �ִ�. 
			// ���ͽ�Ʈ��� �̹� �湮�� ������ ���ؼ��� �Ÿ��� ������Ʈ���� �ʴ´�.
			// ������ ���� ����ġ�� ���� ������ �湮�� ������ ���ؼ��� �����ص� �����ϴ�.
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
	// ���� ������ �߰��� �׷������� �ִ� �Ÿ��� ���
	vector<int> h = BellmanFord(edges, V);

	if (h.empty())
		return;

	for (auto& e : edges)
	{
		e.weight += (h[e.src] - h[e.dst]);
	}

	// ��� ������ ������ �ִ� �Ÿ��� ����
	vector<vector<int>> shortest(V);

	//������ ����ġ�� ����� ���¿��� ���ͽ�Ʈ�� �˰������� ȣ���Ͽ� �� ������ �Ÿ� ���� �ٽ� �����Ѵ�.
	for (int i = 0; i < V; i++)
	{
		shortest[i] = Dijkstra(edges, V, i);
	}

	// ����ġ ��ȯ ������ ������ �����Ͽ� �ִ� �Ÿ��� ���
	// ������ ���� shortest�� ���� �ִ� �Ÿ��� �ƴϴ�. ������ ����ġ ��ȯ ������ ����Ͽ� ��� 
	// ���� ����ġ�� ����� ��ȯ�߱� �����̴�. ���� ����ġ ��ȯ ������ ������ �����Ͽ� ���� �ִ� �Ÿ� ���� �ٽ� ����ؾ� �Ѵ�.
	for (int i = 0; i < V; i++)
	{
		cout << i << ":\n";

		for (int j = 0; j < V; j++)
		{
			if (shortest[i][j] != UNKNOWN)
			{
				//w(AB)+w(BC)+...+w(DE)+d[s,A]-d[s,E]�̹Ƿ� d[s,A]�� ���ְ� d[s,E]�� �����ָ� �ٽ�
				//����ġ���� ���� ������ �ٲ��.
				shortest[i][j] += h[j] - h[i];

				cout << "\t" << j << ": " << shortest[i][j] << endl;
			}
		}
	}
}

int main()
{
	int V = 5;              // ���� ����
	vector<Edge> edges;     // ���� �������� ����

	vector<vector<int>> edge_map{ // {���� ����, ��ǥ ����, ����ġ}
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