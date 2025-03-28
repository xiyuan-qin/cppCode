#include<iostream>
#include<map>
#include<set>
#include<vector>
#include<queue>
#include<stack>

using namespace std;

// 新增辅助类以处理二维索引
class AdjacencyList {
private:
    const vector<pair<int, int>>& edges;

public:
    AdjacencyList(const vector<pair<int, int>>& edges) : edges(edges) {}

    // 重载[]操作符根据目标节点返回边的权重，如果边不存在则返回0
    int operator[](int destNode) const {
        for (const auto& edge : edges) {
            if (edge.first == destNode) {
                return edge.second;  // 返回边的权重
            }
        }
        return 0;  // 如果边不存在，返回0表示没有连接
    }
};

class WeightedDAG {
private:
    int n; // 节点数量
    vector<vector<pair<int, int>>> adj; // 邻接表
    vector<string> nodeNames; // 可选：节点名称
    
public:
    WeightedDAG(int n) : n(n) {
        adj.resize(n);
        nodeNames.resize(n);
    }
    
    void addEdge(int from, int to, int weight) {
        adj[from].push_back({to, weight});
    }
    
    // 修改[]运算符返回AdjacencyList对象
    AdjacencyList operator[](int index) const {
        if (index < 0 || index >= n) {
            throw out_of_range("节点索引超出范围");
        }
        return AdjacencyList(adj[index]);
    }
    
    // 拓扑排序（DAG的典型操作）
    vector<int> topologicalSort() {
        vector<int> inDegree(n, 0);
        for (int i = 0; i < n; i++) {
            for (auto& edge : adj[i]) {
                inDegree[edge.first]++;
            }
        }
        
        queue<int> q;
        for (int i = 0; i < n; i++) {
            if (inDegree[i] == 0) q.push(i);
        }
        
        vector<int> result;
        while (!q.empty()) {
            int node = q.front(); q.pop();
            result.push_back(node);
            
            for (auto& edge : adj[node]) {
                if (--inDegree[edge.first] == 0) {
                    q.push(edge.first);
                }
            }
        }
        
        return result.size() == n ? result : vector<int>();
    }
};

int main(){
    WeightedDAG g(6);
    g.addEdge(0, 1, 5);
    g.addEdge(0, 2, 3);
    g.addEdge(1, 3, 6);
    g.addEdge(1, 2, 2);
    g.addEdge(2, 4, 4);
    g.addEdge(2, 5, 2);
    g.addEdge(3, 4, 1);
    g.addEdge(4, 5, 2);

    cout << g[1][3]<< endl;
    cout << g[3][1]<< endl;
    
    vector<int> result = g.topologicalSort();
    if (result.empty()) {
        cout << "图中存在环，无法进行拓扑排序" << endl;
    } else {
        cout << "拓扑排序结果：";
        for (int i = 0; i < result.size(); i++) {
            cout << result[i] << " ";
        }
        cout << endl;
    }
    return 0;
}