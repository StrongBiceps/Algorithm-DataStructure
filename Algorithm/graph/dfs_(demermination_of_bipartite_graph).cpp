#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <stack>
#include <string>

using namespace std;

template <typename T>
struct Edge
{
	unsigned src;
	unsigned dst;
	T weight;
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

template <typename U>
ostream& operator<< (ostream& os, const Graph<U>& G)
{
	for (unsigned i = 1; i < G.vertices(); i++)
	{
		os << i << ":\t";

		auto edges = G.edges(i);
		for (auto& e : edges)
			os << "{" << e.dst << ": " << e.weight << "}, ";

		os << endl;
	}

	return os;
}

template <typename T>
auto create_bipartite_reference_graph()
{
	Graph<T> G(10);

	map<unsigned, vector<pair<unsigned, T>>> edge_map;
	edge_map[1] = { {2, 0} };
	edge_map[2] = { {1, 0}, {3, 0} , {8, 0} };
	edge_map[3] = { {2, 0}, {4, 0} };
	edge_map[4] = { {3, 0}, {6, 0} };
	edge_map[5] = { {7, 0}, {9, 0} };
	edge_map[6] = { {4, 0} };
	edge_map[7] = { {5, 0} };
	edge_map[8] = { {2, 0}, {9, 0} };
	edge_map[9] = { {5, 0}, {8, 0} };

	for (auto& i : edge_map)
		for (auto& j : i.second)
			G.add_edge(Edge<T>{ i.first, j.first, j.second });

	return G;
}

template <typename T>
auto bipartite_check(const Graph<T>& G)
{
	stack<unsigned> stack;
	set<unsigned> visited;	// 방문한 정점
	stack.push(1);  		// 1번 정점부터 시작

	enum class colors { NONE, BLACK, RED };
	colors current_color{ colors::BLACK }; // 다음 정점에 칠할 색상

	vector<colors> vertex_colors(G.vertices(), colors::NONE);

	while (!stack.empty())
	{
		auto current_vertex = stack.top();
		stack.pop();

		// 현재 정점을 이전에 방문하지 않았다면
		if (visited.find(current_vertex) == visited.end())
		{
			visited.insert(current_vertex);
			//색을 저장하는 벡터에 현재 정점 ID에 해당하는 인덱스에 색을 저장한다.
			vertex_colors[current_vertex] = current_color;

			//매번 방문할 때마다 다른 색을 칠하기 위한 코드
			if (current_color == colors::RED)
			{
				cout << current_vertex << "번 정점: 빨간색" << endl;
				current_color = colors::BLACK;
			}
			else
			{
				cout << current_vertex << "번 정점: 검정색" << endl;
				current_color = colors::RED;
			}

			// 인접한 정점 중에서 방문하지 않은 정점이 있다면 스택에 추가
			for (auto e : G.edges(current_vertex))
				if (visited.find(e.dst) == visited.end())
					stack.push(e.dst);
		}
		// 현재 정점이 이미 방문한 정점이고, 
		// 현재 칠할 색상과 같은 색상이 칠해져 있다면 이분 그래프가 아님.
		else if (vertex_colors[current_vertex] != colors::NONE &&
			vertex_colors[current_vertex] != current_color)
			return false;
	}

	// 모든 정점에 색상이 칠해졌으면 이분 그래프가 맞음
	return true;
}

int main()
{
	using T = unsigned;

	// 그래프 객체 생성
	auto BG = create_bipartite_reference_graph<T>();
	cout << "[입력 그래프]" << endl;
	cout << BG << endl;

	if (bipartite_check<T>(BG))
		cout << endl << "이분 그래프가 맞습니다." << endl;
	else
		cout << endl << "이분 그래프가 아닙니다." << endl;
}