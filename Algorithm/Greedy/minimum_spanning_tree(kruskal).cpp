#include <iostream>
#include <vector>
#include <algorithm>
#include <queue>
#include <map>
using namespace std;

//이 클래스에서 ID는 1부터 8까지 생성된다. 하지만 vector의 index는 0~7까지이다.
//따라서 -1을 계산해서 인덱스로 넣어서 유효한 범위를 보장한다.
//원래 코드는 9를 넣는 것이 직관적이지 않아서 8을 넣기 위해 바꾼 것이다.

class Node
{
public:
	unsigned id;
	unsigned rank;
	unsigned parent;

	Node(unsigned _id) :id(_id), rank(0), parent(_id) {}

	bool operator!=(const Node& n) const
	{
		//id가 같은지를 비교한다.
		return this->id != n.id;
	}
};

//서로 다른 원소들이 같은 집합에 속해있는지 확인할 수 있는 구조
class SimpleDisjointSet
{
private:
	//가장 처음에 추가되는 노드의 자료형으로, 트리를 구성한다.
	vector<Node> nodes;
public:
	SimpleDisjointSet(const unsigned& N)
	{
		nodes.reserve(N);
	}
	void make_set(const unsigned& x)
	{
		nodes.emplace_back(x);
	}
	//서로 부모가 같은 지를 확인하는 연산이다.
	unsigned find(const unsigned& x)
	{
		auto node_it = find_if(nodes.begin(), nodes.end(),
			[x](auto n) { return n.id == x; });
		unsigned node_id = (*node_it).id;

		while (node_id != nodes[node_id - 1].parent)
		{
			node_id = nodes[node_id - 1].parent;
		}
		return node_id;
	}
	//union 연산이다. 
	void union_set(unsigned x, unsigned y)
	{
		auto root_x = find(x);
		auto root_y = find(y);

		//만약 x와 y가 같은 트리에 있다면 그대로 종료
		if (root_x == root_y)
			return;

		//작은 랭크의 트리를 큰 랭크의 트리 쪽으로 병합
		//처음에 매개 변수를 전달받을 때 둘 중 누가 받아들이는 노드인지는 정하지 않고
		//둘 중 랭크가 큰 것이 루트가 된다. 아래 쪽 코드가 x의 parent를 y의 parent로 바꾸기 때문에
		//여기서 swap을 해서 더 큰 랭크의 parent가 대입되게 한다.
		//만약 root_x보다 root_y의 랭크가 더 크다면 그냥 아래 코드처럼 대입하면 된다.

		//union by rank 연산이다. 항상 작은 트리를 큰 트리로 병합한다.
		if (nodes[root_x - 1].rank > nodes[root_y - 1].rank)
			swap(root_x, root_y);

		//아래 경우는 둘의 랭크가 같거나 root_y의 랭크가 더 큰 경우이다.
		//root_x가 아래로 들어가는 노드이고, root_y가 받아들이는 루트 노드이다.
		nodes[root_x - 1].parent = nodes[root_y - 1].parent;

		//자식이 하나 더 늘었으므로 랭크를 추가한다.
		nodes[root_y - 1].rank++;
	}
};

//디스조인트 셋 자료구조를 구현했으니 이제 그래프 자료 구조를 구현한다. 
//Edge list를 이용하여 구현한다.
//에지 구조체는 템플릿으로 구현했기 때문에 <연산자와 >연산자를 지원하는 모든 데이터 타입을 에지 가중치로 사용할 수 있다.
//에지는 단순히 선을 저장하는 것이 아니라 연결된 정점 정보와 가중치까지 저장한다.
template<typename T>
struct Edge
{
public:
	//main에서 정점 edge_map에 저장될 때 src는 정점을 의미하고 dst는 src와 연결된 정점이다.
	//둘 다 정점을 의미한다.
	//에지 양쪽 중 한 쪽을 의미하고
	unsigned src;
	//다른 한 쪽을 의미한다
	unsigned dst;
	T weight;

	//Edge 객체 비교는 가중치를 이용 -내림차순 비교
	bool operator<(const Edge<T>& e) const
	{
		return this->weight < e.weight;
	}
	bool operator>(const Edge<T>& e) const
	{
		return this->weight > e.weight;
	}
};

//그래프 자료 구조를 구현한 graph 클래스를 정의한다. graph클래스는 << 연산자를 이용하여 그래프 정보를 출력할 수 있다.
template<typename T>
class Graph
{
public:
	//N개의 정점으로 구성된 그래프
	Graph(unsigned N) :V(N) {}
	//그래프 정점 개수 반환
	auto vertices() const { return V; }
	//전체 에지 리스트 반환
	auto& edges() const { return edge_list; }
	//정점 v에서 나가는 모든 에지를 반환
	//나가는 에지를 반환하려면 에지의 src
	auto edges(unsigned v) const
	{
		vector<Edge<T>> edges_from_v;
		//main에서 Edge를 저장하는 방식을 보면 i.first(기준 정점 = src) j.first(연결된 정점 = dst) j.second(가중치)
		//이므로 Edge중에 src가 1인 Edge들은 1에서부터 연결된 정점을 의미한다. 따라서 src가 v인 모든 Edge들을 반환한다.
		for (const auto& e : edge_list)
		{
			if (e.src == v)
			{
				edges_from_v.emplace_back(e);
			}
		}
		return edges_from_v;
	}
	void add_edge(Edge<T>&& e)
	{
		//에지 양 끝 정점 ID가 유효한지 검사
		//e.src와 e.dst는 e와 연결된 정점을 의미하는데, 전체 개수인 V보다 큰 정점은 없기 때문에
		//유효한 범위인지 확인하는 작업
		if (e.src >= 1 && e.src <= V && e.dst >= 1 && e.dst <= V)
		{
			edge_list.emplace_back(e);
		}
	}
	//표준 출력 스트림 지원
	template<typename U>
	friend ostream& operator<<(ostream& os, const Graph<U>& G);

private:
	unsigned V; //정점 개수
	vector<Edge<T>> edge_list;
};

template<typename U>
ostream& operator<<(ostream& os, const Graph<U>& G)
{
	//1~8까지 반복하기 위해 <=를 사용한다. 원래 코드에서처럼 9를 넣으려면 <를 사용하면 되는데
	//원래 8개의 정점인데 9를 넣는 것은 직관적이지 않으므로 내가 바꿨다.
	for (unsigned i = 1; i <= G.vertices(); ++i)
	{
		os << i << ":\t";
		auto edges = G.edges(i);
		for (auto& e : edges)
		{
			os << "{" << e.dst << ": " << e.weight << "},";
		}
		os << endl;
	}
	return os;
}

//크루스칼 최소 신장 트리 알고리즘을 구현한다.
template<typename T>
Graph<T> minimum_spanning_tree(const Graph<T>& G)
{
	//에지 가중치를 이용한 최소 힙 구성
	//Edge<T>를 저장하고, vector로 구성되어 있으며, greater를 사용하여 최소 힙을 구성한다.
	//less를 사용하면 최대 힙이 된다.
	priority_queue<Edge<T>, vector<Edge<T>>, greater<Edge<T>>> edge_min_heap;

	for (auto& e : G.edges())
	{
		edge_min_heap.push(e);
	}

	//정점 개수에 해당하는 크기의 디스조인트-셋 자료 구조 생성 및 초기화
	auto N = G.vertices();
	SimpleDisjointSet dset(N);
	//ID를 1부터 생성하기 위해 1부터 N까지 반복을 돌린다.
	for (unsigned i = 1; i <= N; ++i)
	{
		dset.make_set(i);
	}

	//디스조인트 셋 자료구조를 이용하여 최소 신장 트리 구하기
	Graph<T> MST(N);
	while (!edge_min_heap.empty())
	{
		//최소 힙에서 최소 가중치 에지를 추출
		auto e = edge_min_heap.top();
		edge_min_heap.pop();

		//선택한 에지가 사이클을 생성하지 않으면 해당 에지를 MST에 추가
		if (dset.find(e.src) != dset.find(e.dst))
		{
			MST.add_edge(Edge<T>{e.src, e.dst, e.weight});
			dset.union_set(e.src, e.dst);
		}
	}
	return MST;
}

int main()
{
	using T = unsigned;

	//그래프 객체 생성
	Graph<T> G(8);

	map<unsigned, vector<pair<unsigned, T>>> edge_map;
	//edge_map의 인덱스는 정점을 의미한다.
	//pair의 unsigned는 정점을 의미한다 .T는 가중치를 의미한다.
	//edge_map[1]의 {5,3}을 예로 들면
	//Edge(1,5,3)이 될 것이다. 이 의미는 정점 1을 기준으로 연결된 정점은 5와 그 가중치는 3이라는 얘기다.
	edge_map[1] = { {2,2},{5,3} };
	edge_map[2] = { {1,2},{5,5},{4,1} };
	edge_map[3] = { {4,2},{7,3} };
	edge_map[4] = { {2,1},{3,2},{5,2},{6,4},{8,5} };
	edge_map[5] = { {1,3},{2,5},{4,2},{8,3} };
	edge_map[6] = { {4,4},{7,4},{8,1} };
	edge_map[7] = { {3,3},{6,4} };
	edge_map[8] = { {4,5},{5,3},{6,1} };

	for (auto& i : edge_map)
	{
		for (auto& j : i.second)
		{
			G.add_edge(Edge<T>{i.first, j.first, j.second});
		}
	}
	cout << "[입력 그래프]" << endl;
	cout << G << endl;

	Graph<T> MST = minimum_spanning_tree(G);
	cout << "[최소 신장 트리]" << endl;
	cout << MST;
}