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

	//Edge ��ü �񱳴� ����ġ�� �̿� -�������� ��
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
	unsigned V; //���� ����
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

//�ؽ� ���� �̿��Ͽ� �׷��� �÷����� ����� ������ �����Ѵ�.
unordered_map<unsigned, string> color_map =
{
	{1,"Red"},
	{2,"Blue"},
	{3,"Green"},
	{4,"Yellow"},
	{5,"Black"},
	{6,"White"}
};

//�׷��� �÷��� �˰������� �����Ѵ�.

template<typename T>
auto greedy_coloring(const Graph<T>& G)
{
	auto size = G.vertices();

	vector<unsigned> assigned_colors(size, 0);

	//1�� �������� ���ʴ�� �˻��Ѵ�.
	for (unsigned i = 1; i < size; ++i)
	{
		//���� i���� ������� �������� ������ ���� ����Ʈ�� ��ȯ�ȴ�.
		auto outgoing_edges = G.edges(i);

		//1��° ������ ������ �ִ� �������� ���� ����
		set<unsigned> neighbours;

		for (auto& e : outgoing_edges)
		{
			//���⼭ e�� i�� �������� ������ �������̴�. ���� �� �������� src�� ���� i�� �ǹ��ϰ�
			//dst�� i�� ����� ������ �ǹ��Ѵ�. ���� dst�� �ǹ��ϴ� ���� �����Ѵ�.
			//ó������ 0�� ���Եȴ�.
			neighbours.insert(assigned_colors[e.dst]);
		}

		//������ ������ ĥ������ ���� ���� �߿��� ���� ���� ������ ������ ����
		auto smallest = 1;

		//�ֺ��� ������ ������ ������ neighbours���� ĥ������ ���� ������ �˻��Ѵ�.
		for (; smallest <= color_map.size(); ++smallest)
		{
			//�� �ݺ����� �ٸ� ����� �˻��Ͽ� ���� �� break�Ѵ�.
			if (neighbours.find(smallest) == neighbours.end())
				break;
		}
		//for���� ��� �������� �ε���i�� ĥ������ ���� ���� �߿��� ���� ID�� ���� ���� ����
		//ĥ�Ѵ�.
		assigned_colors[i] = smallest;
	}
	//���� ������ ���� ������ assigned_colors�� ��ȯ�Ѵ�.
	return assigned_colors;
}

//�׷��� �÷��� ����� ȭ�鿡 ����ϱ� ���� �Լ��� �߰��Ѵ�.
template<typename T>
void print_colors(vector<T>& colors)
{
	for (auto i = 1; i < colors.size(); ++i)
	{
		cout << i << ": " << color_map[colors[i]] << endl;
	}
}

//main�Լ����� �׷����� �����ϰ�, �׷��� �÷��� �˰����� ���� �Լ��� ȣ���Ѵ�. �׷���
//�÷��������� ���� ����ġ�� ������� �ʱ� ������ �׷��� ��ü G�� ��� ���� ����ġ�� 0���� �����Ѵ�.
int main()
{
	using T = unsigned;

	//���� 1���� �ǹ̰� �ֱ� ������ 9�� ��������� ���� 8���� ����� �� �ִ�.
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

	cout << "[�Է� �׷���]" << endl;
	cout << G << endl;

	auto colors = greedy_coloring<T>(G);
	cout << "[�׷��� �÷���]" << endl;
	print_colors(colors);

	return 0;
}