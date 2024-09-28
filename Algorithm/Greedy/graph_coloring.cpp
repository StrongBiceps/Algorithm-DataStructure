#include <string>
#include <vector>
#include <iostream>
#include <set>
#include <map>
#include <unordered_map>
using namespace std;

template<typename T>
struct Edge
{
public:
	unsigned src;
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

template<typename T>
class Graph
{
public:
	Graph(unsigned N) :V(N) {}
	auto vertices() const { return V; }
	auto& edges() const { return edge_list; }
	auto edges(unsigned v) const
	{
		vector<Edge<T>> edges_from_v;
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
		if (e.src >= 1 && e.src <= V && e.dst >= 1 && e.dst <= V)
		{
			edge_list.emplace_back(e);
		}
	}
	template<typename U>
	friend ostream& operator<<(ostream& os, const Graph<U>& G);
private:
	unsigned V; //정점 개수
	vector<Edge<T>> edge_list;
};

template<typename U>
ostream& operator<<(ostream& os, const Graph<U>& G)
{
	for (unsigned i = 1; i < G.vertices(); ++i)
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

//해시 맵을 이용하여 그래프 컬러링에 사용할 색상을 정의한다.
unordered_map<unsigned, string> color_map =
{
	{1,"Red"},
	{2,"Blue"},
	{3,"Green"},
	{4,"Yellow"},
	{5,"Black"},
	{6,"White"}
};

//그래프 컬러링 알고리즘을 정의한다.

template<typename T>
auto greedy_coloring(const Graph<T>& G)
{
	auto size = G.vertices();

	vector<unsigned> assigned_colors(size, 0);

	//1번 정점부터 차례대로 검사한다.
	for (unsigned i = 1; i < size; ++i)
	{
		//정점 i에서 뻗어나가는 에지들의 모음인 에지 리스트가 반환된다.
		auto outgoing_edges = G.edges(i);

		//1번째 정점과 인접해 있는 정점들의 현재 색상
		set<unsigned> neighbours;

		for (auto& e : outgoing_edges)
		{
			//여기서 e는 i번 정점에서 나가는 엣지들이다. 따라서 이 엣지들의 src는 정점 i를 의미하고
			//dst가 i와 연결된 정점을 의미한다. 따라서 dst가 의미하는 색을 저장한다.
			//처음에는 0이 삽입된다.
			neighbours.insert(assigned_colors[e.dst]);
		}

		//인접한 정점에 칠해지지 않은 색상 중에서 가장 작은 숫자의 색상을 선택
		auto smallest = 1;

		//주변의 정점의 색깔을 저장한 neighbours에서 칠해지지 않은 색상을 검사한다.
		for (; smallest <= color_map.size(); ++smallest)
		{
			//매 반복마다 다른 색깔로 검사하여 없을 때 break한다.
			if (neighbours.find(smallest) == neighbours.end())
				break;
		}
		//for문을 벗어난 시점에서 인덱스i는 칠해지지 않은 색상 중에서 가장 ID가 가장 작은 색을
		//칠한다.
		assigned_colors[i] = smallest;
	}
	//각각 정점의 색을 저장한 assigned_colors를 반환한다.
	return assigned_colors;
}

//그래프 컬러링 결과를 화면에 출력하기 위한 함수를 추가한다.
template<typename T>
void print_colors(vector<T>& colors)
{
	for (auto i = 1; i < colors.size(); ++i)
	{
		cout << i << ": " << color_map[colors[i]] << endl;
	}
}

//main함수에서 그래프를 정의하고, 그래프 컬러링 알고리즘 구현 함수를 호출한다. 그래프
//컬러링에서는 에지 가중치를 사용하지 않기 때문에 그래프 객체 G의 모든 에지 가중치에 0으로 설정한다.
int main()
{
	using T = unsigned;

	//정점 1부터 의미가 있기 때문에 9로 설정해줘야 정점 8까지 취급할 수 있다.
	Graph<T> G(9);

	map<unsigned, vector<pair<unsigned, T>>> edge_map;
	edge_map[1] = { {2,0},{5,0} };
	edge_map[2] = { {1,0},{5,0},{4,0} };
	edge_map[3] = { {4,0},{7,0} };
	edge_map[4] = { {2,0},{3,0},{5,0},{6,0},{8,0} };
	edge_map[5] = { {1,0},{2,0},{4,0},{8,0} };
	edge_map[6] = { {4,0},{7,0},{8,0} };
	edge_map[7] = { {3,0},{6,0} };
	edge_map[8] = { {4,0},{5,0},{6,0} };

	for (auto& i : edge_map)
		for (auto& j : i.second)
			G.add_edge(Edge<T>{i.first, j.first, j.second});

	cout << "[입력 그래프]" << endl;
	cout << G << endl;

	auto colors = greedy_coloring<T>(G);
	cout << "[그래프 컬러링]" << endl;
	print_colors(colors);

	return 0;
}