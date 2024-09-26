class graph
{
private:
	vector < vector<pair<int, int>>> data;
public:
	graph(int n)
	{
		data = vector<vector<pair<int, int>>>(n, vector<pair<int, int>>());
	}
	void addEdge(const city& c1, const city& c2, int dis)
	{
		auto n1 = static_cast<int>(c1);
		auto n2 = static_cast<int>(c2);
		data[n1].push_back(pair<int, int>(n2, dis));
		data[n2].push_back(pair<int, int>(n1, dis));
	}

	void removeEdge(const city& c1, const city& c2)
	{
		auto n1 = static_cast<int>(c1);
		auto n2 = static_cast<int>(c2);
		auto it_remove = remove_if(data[n1].begin(), data[n1].end(), [&](const auto& pair) {
			return pair.first == n2;
			});
		data[n1].erase(it_remove, data[n1].end());

		auto it_remove2 = remove_if(data[n2].begin(), data[n2].end(), [&](const auto& pair) {
			return pair.first == n1;
			});

		data[n2].erase(it_remove2, data[n2].end());
	}
	void printall()
	{
		for (register int i = 0; i < data.size(); ++i)
		{
			for (register int j = 0; j < data[i].size(); ++j)
			{
				cout << convert_to_string(i) << ":" << convert_to_string(data[i][j].first) << " " << data[i][j].second << endl;
			}
			if (data[i].size() != 0)
				cout << data[i].size() << endl;
		}
	}
};