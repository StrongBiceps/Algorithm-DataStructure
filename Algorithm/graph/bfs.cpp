#include <iostream>
#include <string>
#include <set>
#include <map>
#include <queue>
#include <vector>
using namespace std;

template<typename T>
class Edge
{
public:
	unsigned src;
	unsigned dst;
	T Weight;
};

template <typename T>
class Graph
{
public:
	Graph(unsigned N) : V(N) {}
	auto vertices() const { return V; }
	auto& edges() const { return edge_list; }
	auto edges(unsigned v) const
	{
		vector<Edge<T>> edges_from_v;
		for (auto& e : edge_list)
		{
			if (e.src == v)
				edges_from_v.emplace_back(e);
		}

		return edges_from_v;
	}

	void add_edge(Edge<T>&& e)
	{
		if (e.src >= 1 && e.src <= V && e.dst >= 1 && e.dst <= V)
			edge_list.emplace_back(e);
	}

	template <typename U>
	friend ostream& operator<< (ostream& os, const Graph<U>& G);

private:
	unsigned V;	
	vector<Edge<T>> edge_list;
};

template <typename T>
ostream& operator<< (ostream& os, const Graph<T>& G)
{
	for (unsigned i = 1; i < G.vertices(); i++)
	{
		os << i << ":\t";

		auto edges = G.edges(i);
		for (auto& e : edges)
			os << "{" << e.dst << ": " << 0 << "}, ";

		os << endl;
	}

	return os;
}

//그래프를 생성하고 반환하는 함수를 작성한다.
template <typename T>
auto create_reference_graph()
{
	Graph<T> G(9);

	map<unsigned, vector<pair<unsigned, T>>> edge_map;
	edge_map[1] = { {2, 0}, {5, 0} };
	edge_map[2] = { {1, 0}, {5, 0}, {4, 0} };
	edge_map[3] = { {4, 0}, {7, 0} };
	edge_map[4] = { {2, 0}, {3, 0}, {5, 0}, {6, 0}, {8, 0} };
	edge_map[5] = { {1, 0}, {2, 0}, {4, 0}, {8, 0} };
	edge_map[6] = { {4, 0}, {7, 0}, {8, 0} };
	edge_map[7] = { {3, 0}, {6, 0} };
	edge_map[8] = { {4, 0}, {5, 0}, {6, 0} };

	for (auto& i : edge_map)
		for (auto& j : i.second)
			G.add_edge(Edge<T>{ i.first, j.first, j.second });

	return G;
}

//너비 우선 탐색 알고리즘을 다음과 같이 구현한다.
template<typename T>
auto breadth_first_search(const Graph<T>& G, unsigned start)
{
	queue<unsigned> queue;
	//빠른 탐색을 위해서 선형 자료구조가 아닌 set을 사용한다.
	set<unsigned> visited; //방문한 정점
	vector<unsigned> visit_order; //방문 순서
	queue.push(start);

	while (!queue.empty())
	{
		auto current_vertex = queue.front();
		queue.pop();

		//현재 정점을 이전에 방문하지 않았다면
		if (visited.find(current_vertex) == visited.end())
		{
			visited.insert(current_vertex);
			visit_order.push_back(current_vertex);

			for (auto& e : G.edges(current_vertex))
			{
				//인접한 정점 중에서 방문하지 않은 정점이 있다면 큐에 추가
				if (visited.find(e.dst) == visited.end())
				{
					queue.push(e.dst);
				}
			}
		}
	}
	return visit_order;
}

//BFS알고리즘을 실행한다. 시작 정점은 임의로 1번으로 설정한다.
int main()
{
	using T = unsigned;

	//그래프 객체 생성
	auto G = create_reference_graph<T>();
	cout << "[입력 그래프]" << endl;
	cout << G << endl;

	auto bfs_visit_order = breadth_first_search(G, 1);
	for (const auto& v : bfs_visit_order)
		cout << v << endl;
}
