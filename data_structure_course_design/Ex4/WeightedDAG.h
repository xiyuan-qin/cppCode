#ifndef WEIGHTED_DAG_H
#define WEIGHTED_DAG_H

#include<iostream>
#include<map>
#include<set>
#include<vector>
#include<queue>
#include<stack>
#include<climits>

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


// 有向加权图类
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

    // 方法一：贪心算法解决设置信号放大器问题
    int minimumAmplifiersGreedy(int sourceNode, double d);

    // 方法二：动态规划解决设置信号放大器问题
    int minimumAmplifiersDP(int sourceNode, double d);
    
    // 辅助方法：计算所有节点对之间的最短距离
    vector<vector<double>> calculateAllPairsShortestPaths();
    
    // 获取节点数
    int getNodeCount() const { return n; }
    
    // 获取邻接表
    const vector<vector<pair<int, int>>>& getAdjList() const { return adj; }
};

#endif // WEIGHTED_DAG_H
