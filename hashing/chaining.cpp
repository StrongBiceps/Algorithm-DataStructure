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
		cout << value << "를 삽입했습니다." << endl;
	}
	//std::find를 통해서 value를 찾아야 하므로 modulo 코드보다는 룩업 작업이 느리다.
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
			cout << value << "를 삭제했습니다." << endl;
		}
	}
};
