#include <iostream>
#include <vector>
#include <stack>

using namespace std;

//DFS를 수행하는 FillStack()함수는 네 개의 인자를 받는다. 정수형 node는 현재 정점 인덱스이고,
//부울형 벡터 visited는 기존에 방문한 정점을 기록한 벡터이다. 2차원 정수형 벡터 adj는 그래프를 나타내는
//인접 리스트이다. 마지막으로 정수형 스택 stack은 방문 순서에 따른 정점 인덱스를 기록하고 있다.
//visited,adj,stack 인자는 참조로 전달된다. DFS는 일반적인 방식으로 구현하되, 현재 정점 인덱스를 스택에 추가하는
//작업이 추가되어야 한다.
void FillStack(int node, vector<bool>& visited,
	vector<vector<int>>& adj, stack<int>& stack)
{
	//0->1->2->3->7->8->5->4->6 순서로 true로 만든다.
	visited[node] = true;

	for (auto next : adj[node])
	{
		if (!visited[next])
		{
			FillStack(next, visited, adj, stack);
		}
	}

	//8에서 3으로 가려는데 3은 visited true이므로 8에서 처음 for문을 벗어난다. 따라서 8이 스택에 가장 먼저
	//들어간다. 8의 코드가 끝났으므로 7로 가면 7도 더 이상의 자식이 없으므로 7이 그 다음에 들어간다.
	//다음으로 3도 자식이 없으므로 for문을 벗어나 스택에 push된다. 현재 스택에 8 7 3

	//그 다음으로 2의 자식 3이 끝났으므로 2의 다른 자식인 5로 들어간다. 5의 자식인 4로 들어가서 4의 자식인
	//2는 이미 방문되었으므로 4의 코드도 끝이 나서 4가 스택에 들어간다. 현재 스택 8 7 3 4
	//5의 다른 자식인 6으로 이동하여 6의 자식도 이미 끝났으므로 6이 스택에 들어간다. 8 7 3 4 6
	//이제 5의 모든 자식이 끝났으므로 5가 스택에 들어간다. 8 7 3 4 6 5 이제 2도 끝났고 그다음 1- >0 순으로 들어가서
	//최종 스택에 8 7 3 4 6 5 2 1 0 이 들어간다. 스택에 들어간 순서를 생각해보면 
	//8 7 3 (강한 연결 요소 1) 4 6 5 2(강한 연결 요소2) 1 (강한 연결 요소3) 0 (강한 연결 요소 4)순으로 들어갔다.
	cout << node << endl;
	stack.push(node);
}

//알고리즘 후반부에는 두 번째 DFS함수인 CollectConnectedComponents() 함수를 사용한다. 이 함수는 FillStack()함수와 비슷한 인자를
//받는다 다만 인자 Component가 정수 벡터 참조로 선언되어 있다. 이 벡터에 각각의 강한 연결 요소에 속하는 정점 인덱스가 저장된다.
//CollectConnectedComponents()함수에서 DFS 순회 코드는 FillStack()함수와 거의 비슷하며, 다만 스택에 정점을 추가하는 코드는 빠져있다.
//대신 함수 앞부분에 현재 정점 node를 벡터에 추가하는 코드가 추가된다.
void CollectConnectedComponents(int node, vector<bool>& visited,
	vector<vector<int>>& adj, vector<int>& component)
{
	//전치된 행렬의 의미는 모든 정점으로 갈 수 있던 0,1이 이제 거꾸로 0은 어떤 정점도 갈 수 없고,
	//1은 0만 갈 수 있게 되었다. 이 두 정점을 제외한 나머지 연결 요소들도 서로의 정점은 변함없이 방문할 수 있지만
	//도달할 수 있던 연결 요소는 도달할 수 없게 되었고, 도달할 수 없던 연결 요소는 도달할 수 있게 되었다. 
	//원래 그래프로 DFS를 한 순서를 스택에 저장하는데, 이 순서는 강한 연결 요소끼리 뭉친 형태이고,
	//전치 그래프로 DFS를 할 때에는 방향이 역전되어 도달할 수 있는 정점이 많은 연결 요소부터 차례로 끊겨서
	//connectedComponents배열에 저장된다.
	visited[node] = true;
	cout << "Coll:" << node << endl;
	component.push_back(node);

	for (auto next : adj[node])
	{
		if (!visited[next])
		{
			cout << "visit: " << next << endl;
			CollectConnectedComponents(next, visited, adj, component);
		}
	}
}

//새로운 함수 Transpose()를 추가한다. 이 함수는 인접 리스트로 표현된 그래프를 인자로 받고, 전치된 그래프를 반환한다.
vector<vector<int>> Transpose(int V, vector<vector<int>> adj)
{
	vector<vector<int>> transpose(V);

	for (int i = 0; i < V; i++)
	{
		for (auto next : adj[i])
		{
			transpose[next].push_back(i);
		}
	}

	return transpose;
}

//코사라주 알고리즘을 구현한 Kosaraju()함수를 정의한다. 이 함수는 정점 개수 V와 그래프를 표현하는 인접 리스트
//adj를 인자로 받는다. 그리고 입력 그래프에서 강한 연결 요소 정보를 담은 정수형 벡터의 벡터를 반환한다.
vector<vector<int>> Kosaraju(int V, vector<vector<int>> adj)
{
	//첫 번째 작업은 스택 컨테이너 stack과 방문 배열 visited를 선언하는 것이다. visited 배열의 모든 원소는 false로 초기화한다.
	//그리고 그래프 모든 정점에 대해 아직 방문하지 않은 정점이라면 DFS순회를 진행한다.
	vector<bool> visited(V, false);
	stack<int> stack;

	//순방향으로 순회했을 경우
	for (int i = 0; i < V; ++i)
	{
		if (!visited[i])
		{
			FillStack(i, visited, adj, stack);
		}
	}

#pragma region 다양한 순회 경우
	//역방향으로 순회했을 경우
	/*for (int i = V-1; i >= 0; --i)
	{
		if (!visited[i])
		{
			FillStack(i, visited, adj, stack);
		}
	}*/

	//강한 연결 요소 2,4,5,6 중 2부터 순회하고 다 끝난 뒤 0,1을 순회할 경우
	/*for (int i = 2; i <V; ++i)
	{
		if (!visited[i])
		{
			FillStack(i, visited, adj, stack);
		}
	}
	for (int i = 0; i <2 ; ++i)
	{
		if (!visited[i])
		{
			FillStack(i, visited, adj, stack);
		}
	}*/
	//위 모든 경우에서 가장 먼저 스택에 들어가는 연결 요소는 3,8,7이다 즉 다른 연결 요소로 이동할 수 없는
	//고립된 연결 요소가 가장 먼저 스택에 들어간다. 또한 스택에는 가장 먼저 들어가지만 역방향 그래프인 collect과정에서는
	//가장 나중에 스택에서 pop된다. 따라서 0  1  2456  873 순으로 강한 연결 요소로 묶이게 된다.

	//일단 다른 연결 요소로 이동할 수 없는 강한 연결 요소가 스택에 들어가는 이유는 간단하다. DFS방식을 사용하기 때문에
	//연결된 정점들을 따라 가다가 빠져나올 수 없는 강한 연결 요소로 들어가게 되면 그 연결 요소부터 스택에 들어가게 된다.
	//빠져나올 수 없는 경우에 for문을 벗어나기 때문이다. 하지만 고립된 강한 연결 요소가 2개 이상일 때, 모든 고립된 강한 연결 요소가
	//반드시 먼저 들어가는 것은 아니다. 단, 그 중에 하나는 반드시 가장 먼저 들어갈 것이다.

#pragma endregion

	//두 번째 DFS를 수행하기에 앞서 먼저 전치 그래프 transpose를 생성한다. 
	vector<vector<int>> transpose = Transpose(V, adj);

	//visited 배열을 다시 false로 초기화한다.
	fill(visited.begin(), visited.end(), false);

	//connectedComponents라는 이름의 정수형 벡터의 벡터를 정의한다. 이 벡터에 전치 그래프에서의 순회 결과,
	//즉 강한 연결 요소를 저장할 것이다. while반복문 안에서 스택으로부터 하나씩 꺼내고, 이 정점을 아직 방문
	//하지 않았다면 DFS순회를 진행한다. 이때 정수형 벡터 component를 선언하고, 이를 CollecComponent()
	//함수에 참조로 전달하여 값을 채운다. DFS 순회가 끝나면 component벡터를 connectComponents에 추가한다.
	//이 알고리즘은 스택이 빈 상태가 될 때까지 반복하고, 최종적으로 connectComponents를 반환한다.
	vector<vector<int>> connectedComponents;

	while (!stack.empty())
	{
		int node = stack.top();
		cout << "stack:" << node << endl;
		stack.pop();

		if (!visited[node])
		{
			vector<int> component;

			CollectConnectedComponents(node, visited, transpose, component);
			connectedComponents.push_back(component);
		}
	}

	return connectedComponents;
}

//main 함수에서 각각의 강한 연결 요소에 속하는 정점들의 번호를 화면에 출력한다.
int main()
{
	int V = 9;

	vector<vector<int>> adj =
		//index가 기준 정점이고 삽입된 배열이 연결된 정점들이다.
	{
		{ 1, 3 },
		{ 2, 4 },
		{ 3, 5 },
		{ 7 },
		{ 2 },
		{ 4, 6 },
		{ 7, 2 },
		{ 8 },
		{ 3 }
	};

	/*int V = 4;
	vector<vector<int>> adj =
	{
		{1},
		{2,3},
		{2},
		{3}
	};*/

	vector<vector<int>> connectedComponents = Kosaraju(V, adj);

	cout << "강한 연결 요소 개수: " << connectedComponents.size() << endl;

	for (int i = 0; i < connectedComponents.size(); i++)
	{
		cout << "[" << i + 1 << "] ";

		for (auto node : connectedComponents[i])
			cout << node << " ";

		cout << endl;
	}
}