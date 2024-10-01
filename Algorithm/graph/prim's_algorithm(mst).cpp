#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <queue>
#include <limits>
#include <string>

// 시작 정점에서의 거리가 중요한 것이 아닌, 현재 판단하고 있는 정점과 연결된 정점들과의 거리만이 중요하기 때문에,
// 거리 값을 에지의 weight만 사용해서 판단한다.

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

//에지의 가중치가 정의된 그래프를 반환한다.
template <typename T>
auto create_reference_graph()
{
	Graph<T> G(9);

	map<unsigned, vector<pair<unsigned, T>>> edge_map;
	edge_map[1] = { {2, 2}, {5, 3} };
	edge_map[2] = { {1, 2}, {5, 5}, {4, 1} };
	edge_map[3] = { {4, 2}, {7, 3} };
	edge_map[4] = { {2, 1}, {3, 2}, {5, 2}, {6, 4}, {8, 5} };
	edge_map[5] = { {1, 3}, {2, 5}, {4, 2}, {8, 3} };
	edge_map[6] = { {4, 4}, {7, 4}, {8, 1} };
	edge_map[7] = { {3, 3}, {6, 4} };
	edge_map[8] = { {4, 5}, {5, 3}, {6, 1} };

	for (auto& i : edge_map)
		for (auto& j : i.second)
			G.add_edge(Edge<T>{ i.first, j.first, j.second });

	return G;
}

//정점에 경계로부터 거리 정보를 저장하기 위해 사용할 Lable 구조체를 정의한다.
//Lable 객체 비교는 거리 값을 이용하도록 비교 연산자를 오버로딩한다.
template <typename T>
struct Label
{
	unsigned ID;
	T distance;

	// Label 객체 비교는 거리(distance) 값을 이용
	inline bool operator> (const Label<T>& l) const
	{
		return this->distance > l.distance;
	}
};

template <typename T>
auto prim_MST(const Graph<T>& G, unsigned src)
{
	// 최소 힙
	//거리가 가장 작은 정점이 Top에 위치한다.
	//Label<T> 자료형을 vector<Label<T>>컨테이너에 저장하고, 객체 비교에는 greater<Lable<T>> 함수 객체를 사용한다.
	priority_queue<Label<T>, vector<Label<T>>, greater<Label<T>>> heap;

	// 모든 정점에서 거리 값을 최대로 설정
	vector<T> distance(G.vertices(), numeric_limits<T>::max());

	set<unsigned> visited;	// 방문한 정점
	vector<unsigned> MST;	// 최소 신장 트리

	//시작 정점을 heap에 넣는다.
	heap.emplace(Label<T>{src, 0});

	while (!heap.empty())
	{
		//거리 값이 가장 작은 정점이 선택된다.
		auto current_vertex = heap.top();
		heap.pop();

		//만약 현재 정점을 방문했다면 아무런 작업을 하지 않는다.
		//그러나 현재 정점을 이전에 방문하지 않았다면 다음 작업을 수행한다.
		//이전에 방문했던 정점에 대해서는 에지를 추가하는 작업을 하지 않으므로 사이클이 생성될 수 없다.
		if (visited.find(current_vertex.ID) == visited.end())
		{
			//방문한 정점을 최소 신장 트리에 집어 넣는다.
			MST.push_back(current_vertex.ID);

			//현재 정점에서 뻗어 나가는 에지의 리스트를 활용하여 인접 정점들의 거리를 다시 설정한다.
			for (auto& e : G.edges(current_vertex.ID))
			{
				auto neighbor = e.dst;
				auto new_distance = e.weight;

				// 인접한 정점의 거리 값이 새로운 경로에 의한 거리 값보다 크면
				// 힙에 추가하고, 거리 값을 업데이트함.
				if (new_distance < distance[neighbor])
				{
					heap.emplace(Label<T>{neighbor, new_distance});
					distance[neighbor] = new_distance;
				}
			}

			//방문한 정점에 현재 정점을 추가한다.
			visited.insert(current_vertex.ID);
		}
	}

	for (const auto& i : distance)
	{
		cout << i << " ";
	}
	cout << endl;
	return MST;
}

int main()
{
	using T = unsigned;

	// 그래프 객체 생성
	auto G = create_reference_graph<T>();
	cout << "[입력 그래프]" << endl;
	cout << G << endl;

	auto MST = prim_MST<T>(G, 1);

	cout << "[최소 신장 트리]" << endl;
	for (auto v : MST)
		cout << v << endl;
	cout << endl;
}
