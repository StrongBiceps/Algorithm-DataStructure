#include <iostream>
#include <vector>
using namespace std;

using uint = unsigned int;

class hash_map
{
private:
	vector<int> data;
public:
	hash_map(size_t n)
	{
		//������ n���� ���Ҹ� -1�� �ʱ�ȭ�Ѵ�.
		data = vector<int>(n, -1);
	}
	void insert(uint value)
	{
		int n = data.size();
		//modulo operation�� �̿��Ͽ� �ؽ� ���̺��� ����.
		data[value % n] = value;
		cout << value << "�� �����߽��ϴ�." << endl;
	}
	bool find(uint value)
	{
		int n = data.size();
		return (data[value % n] == value);
	}
	void erase(uint value)
	{
		int n = data.size();
		if (data[value % n] == value)
		{
			data[value % n] = -1;
			cout << value << "�� �����߽��ϴ�." << endl;
		}
	}
};