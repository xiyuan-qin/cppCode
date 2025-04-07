#include "WeightedDAG.h"
#include <algorithm>
#include <limits>
#include <queue>
#include <set>

WeightedDAG::WeightedDAG(int nodeCount) : nodeCount_(nodeCount) {
    adjacencyList_.resize(nodeCount);
}

void WeightedDAG::addEdge(int from, int to, int weight) {
    if (from < 0 || from >= nodeCount_ || to < 0 || to >= nodeCount_) {
        throw std::out_of_range("节点索引超出范围");
    }
    adjacencyList_[from].push_back({to, weight});
}

int WeightedDAG::getEdgeWeight(int from, int to) const {
    if (from < 0 || from >= nodeCount_ || to < 0 || to >= nodeCount_) {
        return -1;
    }
    
    for (const auto& edge : adjacencyList_[from]) {
        if (edge.first == to) {
            return edge.second;
        }
    }
    return -1;
}

std::vector<int> WeightedDAG::topologicalSort() const {
    std::vector<int> inDegree(nodeCount_, 0);
    
    // 计算每个节点的入度
    for (int i = 0; i < nodeCount_; i++) {
        for (const auto& edge : adjacencyList_[i]) {
            inDegree[edge.first]++;
        }
    }
    
    // 使用队列进行拓扑排序
    std::queue<int> q;
    for (int i = 0; i < nodeCount_; i++) {
        if (inDegree[i] == 0) q.push(i);
    }
    
    std::vector<int> result;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        result.push_back(node);
        
        for (const auto& edge : adjacencyList_[node]) {
            if (--inDegree[edge.first] == 0) {
                q.push(edge.first);
            }
        }
    }
    
    return result.size() == nodeCount_ ? result : std::vector<int>();
}

int WeightedDAG::minimumAmplifiersGreedy(int sourceNode, double maxDistance, 
                                        std::vector<bool>* amplifierLocations) {
    if (sourceNode < 0 || sourceNode >= nodeCount_) {
        return -1;
    }
    
    // 初始化放大器位置向量（如果提供）
    if (amplifierLocations) {
        amplifierLocations->assign(nodeCount_, false);
    }
    
    // 获取拓扑排序
    std::vector<int> topoOrder = topologicalSort();
    if (topoOrder.empty()) return -1; // 图不是DAG
    
    // 初始化距离数组
    std::vector<double> distance(nodeCount_, std::numeric_limits<double>::infinity());
    distance[sourceNode] = 0;
    
    // 放大器计数
    int amplifierCount = 0;
    std::vector<bool> hasAmplifier(nodeCount_, false);
    
    // 按拓扑顺序处理节点
    for (int node : topoOrder) {
        if (distance[node] == std::numeric_limits<double>::infinity()) 
            continue;
            
        for (const auto& edge : adjacencyList_[node]) {
            int nextNode = edge.first;
            double weight = edge.second;
            
            // 如果需要放置放大器
            if (distance[node] + weight > maxDistance && !hasAmplifier[node]) {
                hasAmplifier[node] = true;
                amplifierCount++;
                distance[node] = 0; // 重置累计距离
            }
            
            // 更新下一节点的距离
            distance[nextNode] = std::min(distance[nextNode], distance[node] + weight);
        }
    }
    
    // 如果提供了放大器位置数组，保存结果
    if (amplifierLocations) {
        *amplifierLocations = hasAmplifier;
    }
    
    return amplifierCount;
}

int WeightedDAG::minimumAmplifiersDP(int sourceNode, double maxDistance, 
                                    std::vector<bool>* amplifierLocations) {
    if (sourceNode < 0 || sourceNode >= nodeCount_) {
        return -1;
    }
    
    // 获取拓扑排序
    std::vector<int> topoOrder = topologicalSort();
    if (topoOrder.empty()) return -1; // 图不是DAG
    
    // dp[i]表示从源点到节点i所需的最少放大器数量
    std::vector<int> dp(nodeCount_, INT_MAX);
    dp[sourceNode] = 0;
    
    // 记录当前累计距离
    std::vector<double> distance(nodeCount_, std::numeric_limits<double>::infinity());
    distance[sourceNode] = 0;
    
    // 存储放大器放置的决策
    std::vector<bool> hasAmplifier(nodeCount_, false);
    std::vector<int> parent(nodeCount_, -1);  // 记录最优路径的父节点
    
    // 按拓扑顺序处理节点
    for (int node : topoOrder) {
        if (dp[node] == INT_MAX) continue;
        
        for (const auto& edge : adjacencyList_[node]) {
            int nextNode = edge.first;
            double weight = edge.second;
            
            // 不需要额外放大器的情况
            if (distance[node] + weight <= maxDistance) {
                if (dp[node] < dp[nextNode] || 
                    (dp[node] == dp[nextNode] && distance[node] + weight < distance[nextNode])) {
                    dp[nextNode] = dp[node];
                    distance[nextNode] = distance[node] + weight;
                    parent[nextNode] = node;
                }
            } 
            // 需要放置放大器的情况
            else {
                int newAmplifiers = dp[node] + 1;
                if (newAmplifiers < dp[nextNode] || 
                    (newAmplifiers == dp[nextNode] && weight < distance[nextNode])) {
                    dp[nextNode] = newAmplifiers;
                    distance[nextNode] = weight;
                    parent[nextNode] = node;
                    hasAmplifier[node] = true;
                }
            }
        }
    }
    
    // 如果请求放大器位置，则构建放大器位置向量
    if (amplifierLocations) {
        amplifierLocations->assign(nodeCount_, false);
        
        // 找出叶子节点（拓扑排序的最后几个节点）
        std::set<int> leafNodes;
        for (int i = 0; i < nodeCount_; i++) {
            if (adjacencyList_[i].empty() && dp[i] != INT_MAX) {
                leafNodes.insert(i);
            }
        }
        
        // 从叶子节点回溯，标记放大器位置
        for (int leaf : leafNodes) {
            int current = leaf;
            while (current != -1 && current != sourceNode) {
                int prev = parent[current];
                if (prev != -1 && distance[current] != distance[prev] + getEdgeWeight(prev, current)) {
                    (*amplifierLocations)[prev] = true;
                }
                current = prev;
            }
        }
    }
    
    // 计算最少放大器数量
    int maxAmplifiers = 0;
    for (int i = 0; i < nodeCount_; i++) {
        if (dp[i] != INT_MAX) {
            maxAmplifiers = std::max(maxAmplifiers, dp[i]);
        }
    }
    
    return maxAmplifiers;
}