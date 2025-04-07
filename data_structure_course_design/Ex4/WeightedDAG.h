#ifndef WEIGHTED_DAG_H
#define WEIGHTED_DAG_H

#include <iostream>
#include <map>
#include <set>
#include <vector>
#include <queue>
#include <stack>
#include <climits>
#include <functional>

class WeightedDAG {
public:
    // 构造函数
    explicit WeightedDAG(int nodeCount);
    
    // 添加有向边
    void addEdge(int from, int to, int weight);
    
    // 拓扑排序
    std::vector<int> topologicalSort() const;
    
    // 计算最少放大器数 - 贪心算法
    int minimumAmplifiersGreedy(int sourceNode, double maxDistance, 
                               std::vector<bool>* amplifierLocations = nullptr);
    
    // 计算最少放大器数 - 动态规划
    int minimumAmplifiersDP(int sourceNode, double maxDistance, 
                           std::vector<bool>* amplifierLocations = nullptr);
    
    // 获取节点数
    int getNodeCount() const { return nodeCount_; }
    
    // 获取邻接表
    const std::vector<std::vector<std::pair<int, int>>>& getAdjList() const { return adjacencyList_; }
    
    // 获取两个节点间的边权重，不存在则返回-1
    int getEdgeWeight(int from, int to) const;

private:
    int nodeCount_;                                    // 节点数量
    std::vector<std::vector<std::pair<int, int>>> adjacencyList_;  // 邻接表
};

#endif // WEIGHTED_DAG_H
