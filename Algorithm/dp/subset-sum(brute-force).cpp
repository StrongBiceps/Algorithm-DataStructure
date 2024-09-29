#include <iostream>
#include <vector>
#include <algorithm>

//�ΰ����� ������� DEBUG��� ��ó���� ����� PRINT��� ��ũ�� �Լ��� �����ϰ�, DEBUG�� 0�� �ƴ� ��쿡�� stderr ����� �����ϵ��� �����Ѵ�.
#define DEBUG 1

#if DEBUG
#define PRINT(x) cerr << x
#else
#define PRINT(x)
#endif

using namespace std;

//������ ��� �κ������� ���ϴ� GetAllSubsets()�Լ��� �����Ѵ�. �� �Լ��� �� ���� ���� ���� Set�� Subset, ���� index,
//�׸��� ��� �κ������� ������ 3���� ���� ���� allSubsets�� ���ڷ� �޴´�. �̶� allSubsets�� ������ �����Ѵ�. �� �Լ���
//��� ȣ���� �̿��Ͽ� set�� ��� �κ������� �����Ѵ�.
void GetAllSubsets(vector<int> set, vector<int> subset,
	int index, vector<vector<vector<int>>>& allSubsets)
{
	// ���� set�� ���� ������ ���
	if (index == set.size())
	{
		// �κ����� ũ�⸦ �ε����� ����Ͽ� �κ������� allSubsets�� �߰�
		allSubsets[subset.size()].push_back(subset);
		return;
	}

	//���� �ε����� �κ����տ� �߰����� ������ �����ϴ� ���� ��ͷ� ǥ���� ���̴�.

	// ���Ҹ� �߰����� �ʰ� ��� ȣ��
	GetAllSubsets(set, subset, index + 1, allSubsets);

	// ���Ҹ� �κ����տ� �߰��� �� ��� ȣ��
	subset.push_back(set[index]);
	GetAllSubsets(set, subset, index + 1, allSubsets);
}

//�� �Լ��� �Է� ������ ǥ���ϴ� ���� ���� set�� �κ������� ���� ��Ÿ���� ���� target�� ���ڷ� �ް�,bool ���� ��ȯ�Ѵ�. 
//�� �Լ��� �պκп��� ��ü �κ������� ������ allSubsets�� �����ϰ� GetAllSubsets()�Լ��� ȣ���Ѵ�.
bool SubsetSum_BruteForce(vector<int> set, int target)
{
	//set.size()+1�� �ϴ� ������ set���հ� ���� ũ���� �κ� ������ �ϳ� �����ϱ� ������
	//�ش� �ε����� ����ؾ� �ϱ� �����̴�.
	vector<vector<vector<int>>> allSubsets(set.size() + 1);

	GetAllSubsets(set, {}, 0, allSubsets);

	//�� �κ������� ���� ���� target�� ���ϰ�, ���� ��ġ�ϸ� true�� ��ȯ�Ѵ�. ���� �κ������� ���� target�� ���� ��찡 �߻����� ������
	//false�� ��ȯ�Ѵ�.
	for (int size = 0; size <= set.size(); size++)
	{
		PRINT("�κ������� ���� ����: " << size << endl);

		for (auto subset : allSubsets[size])
		{
			PRINT("\t{ ");

			int sum = 0;
			for (auto number : subset)
			{
				sum += number;
				PRINT(number << " ");
			}

			PRINT("} = " << sum << endl);

			if (sum == target)
				return true;
		}
	}

	return false;
}

int main()
{
	vector<int> set = { 13, 79, 45, 29 };
	int target = 58;

	bool found = SubsetSum_BruteForce(set, target);

	if (found)
	{
		cout << "���� ���� " << target << "�� �κ������� �ֽ��ϴ�." << endl;
	}
	else
	{
		cout << "���� ���� " << target << "�� �κ������� �����ϴ�." << endl;
	}
}