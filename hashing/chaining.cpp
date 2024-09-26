#include <iostream>
#include <vector>
#include <list>
#include <algorithm>
using namespace std;

using uint = unsigned int;

class hash_map
{
private:
	vector<list<int>> data;
public:
	hash_map(size_t n)
	{
		data.resize(n);
	}
	void insert(uint value)
	{
		int n = static_cast<int>(data.size());
		data[value % n].push_back(value);
		cout << value << "�� �����߽��ϴ�." << endl;
	}
	//std::find�� ���ؼ� value�� ã�ƾ� �ϹǷ� modulo �ڵ庸�ٴ� ��� �۾��� ������.
	bool find_hash(uint value)
	{
		int n = static_cast<int>(data.size());
		auto& entries = data[value % n];
		return find(entries.begin(), entries.end(), value) != entries.end();
	}
	void erase(uint value)
	{
		int n = static_cast<int>(data.size());
		auto& entries = data[value % n];
		auto iter = find(entries.begin(), entries.end(), value);

		if (iter != entries.end())
		{
			entries.erase(iter);
			cout << value << "�� �����߽��ϴ�." << endl;
		}
	}
};
