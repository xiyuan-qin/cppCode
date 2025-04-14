#include "WeightedDAG.h"
#include <algorithm>
#include <limits>
#include <queue>
#include <set>

using namespace std;

WeightedDAG::WeightedDAG(int nodeCount) : nodeCount_(nodeCount) {
    adjacencyList_.resize(nodeCount);
}

void WeightedDAG::addEdge(int from, int to, int weight) {
    if (from < 0 || from >= nodeCount_ || to < 0 || to >= nodeCount_) {
        throw out_of_range("节点索引超出范围");
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

// 拓扑排序
vector<int> WeightedDAG::topologicalSort() const {
    vector<int> inDegree(nodeCount_, 0);
    
    // 计算每个节点的入度
    for (int i = 0; i < nodeCount_; i++) {
        for (const auto& edge : adjacencyList_[i]) {
            inDegree[edge.first]++;
        }
    }
    
    // 使用队列进行拓扑排序
    queue<int> q;
    for (int i = 0; i < nodeCount_; i++) {
        if (inDegree[i] == 0) q.push(i);
    }
    
    vector<int> result;
    while (!q.empty()) {
        int node = q.front(); q.pop();
        result.push_back(node);
        
        for (const auto& edge : adjacencyList_[node]) {
            if (--inDegree[edge.first] == 0) {
                q.push(edge.first);
            }
        }
    }
    
    return result.size() == nodeCount_ ? result : vector<int>();
}


// 贪心算法
int WeightedDAG::minimumAmplifiersGreedy(int sourceNode, double maxDistance, 
                                        vector<bool>* amplifierLocations) {
    if (sourceNode < 0 || sourceNode >= nodeCount_) {
        return -1;
    }
    
    // 初始化放大器位置向量
    if (amplifierLocations) {
        amplifierLocations->assign(nodeCount_, false);
    }
    
    // 获取拓扑排序
    vector<int> topoOrder = topologicalSort();
    {
    if (topoOrder.empty()) return -1; // 图不是DAG
    }
    
    // 初始化距离数组和前驱节点数组
    vector<double> distance(nodeCount_, numeric_limits<double>::infinity());
    vector<int> parent(nodeCount_, -1);
    distance[sourceNode] = 0;
    
    // 使用拓扑顺序计算最短路径
    for (int node : topoOrder) {
        if (distance[node] == numeric_limits<double>::infinity()) 
            continue;
            
        for (const auto& edge : adjacencyList_[node]) {
            int nextNode = edge.first;
            double weight = edge.second;
            
            if (distance[node] + weight < distance[nextNode]) {
                distance[nextNode] = distance[node] + weight;
                parent[nextNode] = node;
            }
        }
    }
    
    // 贪心放置放大器
    int amplifierCount = 0;
    vector<bool> hasAmplifier(nodeCount_, false);
    
    // 从每个叶子节点回溯到源点，放置放大器
    for (int node = 0; node < nodeCount_; node++) {
        if (distance[node] != numeric_limits<double>::infinity() && 
            (adjacencyList_[node].empty() || node == nodeCount_ - 1)) { // 是叶子节点或最后一个节点
            
            // 从叶子节点回溯到源点
            double currentDistance = 0;
            int currentNode = node;
            
            while (currentNode != sourceNode) {
                int prevNode = parent[currentNode];
                if (prevNode == -1) break; // 防止断路
                
                double edgeWeight = 0;
                for (const auto& edge : adjacencyList_[prevNode]) {
                    if (edge.first == currentNode) {
                        edgeWeight = edge.second;
                        break;
                    }
                }
                
                // 检查添加这条边是否超过最大距离
                if (currentDistance + edgeWeight > maxDistance) {
                    // 需要在当前节点放置放大器
                    if (!hasAmplifier[currentNode]) {
                        hasAmplifier[currentNode] = true;
                        amplifierCount++;
                    }
                    currentDistance = 0;
                } else {
                    currentDistance += edgeWeight;
                }
                
                currentNode = prevNode;
            }
        }
    }
    
    // 如果提供了放大器位置数组，保存结果
    if (amplifierLocations) {
        *amplifierLocations = hasAmplifier;
    }
    
    return amplifierCount;
}


// 动态规划算法
int WeightedDAG::minimumAmplifiersDP(int sourceNode, double maxDistance, 
                                    vector<bool>* amplifierLocations) {
    if (sourceNode < 0 || sourceNode >= nodeCount_) {
        return -1;
    }
    
    // 获取拓扑排序
    vector<int> topoOrder = topologicalSort();
    if (topoOrder.empty()) return -1; // 图不是DAG
    
    // dp[i]表示从源点到节点i所需的最少放大器数量
    vector<int> dp(nodeCount_, INT_MAX);
    dp[sourceNode] = 0;
    
    // 记录当前累计距离
    vector<double> distance(nodeCount_, numeric_limits<double>::infinity());
    distance[sourceNode] = 0;
    
    // 存储放大器放置的决策
    vector<bool> hasAmplifier(nodeCount_, false);
    vector<int> parent(nodeCount_, -1);  // 记录最优路径的父节点
    
    // 按拓扑顺序处理节点
    for (int node : topoOrder) {
        /**
         *  按照拓扑顺序处理每个节点
         *  对于每个节点，考虑所有出边
            对每条边(node, nextNode)，考虑两种情况：
            a) 不需要放大器：如果distance[node] + weight <= maxDistance 
            b) 需要放大器：在node处放置放大器，重置距离
            每次从一个点到另一个点到时候，如果加上到边的权重超过了最大距离
            就需要在当前节点放置放大器，同时重置到下一个节点的距离
            不需要放大器的情况，直接更新到下一个节点的最小放大器数量，同时增加距离。
         */
        if (dp[node] == INT_MAX) continue;
        
        for (const auto& edge : adjacencyList_[node]) {
            int nextNode = edge.first;
            double weight = edge.second;
            
            // 不需要额外放大器的情况
            if (distance[node] + weight <= maxDistance) {
                if (dp[node] < dp[nextNode] || 
                    (dp[node] == dp[nextNode] && distance[node] + weight < distance[nextNode])) {
                    dp[nextNode] = dp[node];
                    // 更新距离为当前节点到下一个节点的距离
                    distance[nextNode] = distance[node] + weight;
                    parent[nextNode] = node;
                }
            } 
            // 需要放置放大器的情况
            else {
                int newAmplifiers = dp[node] + 1;
                if (newAmplifiers < dp[nextNode] || 
                    (newAmplifiers == dp[nextNode] && weight < distance[nextNode])) {
                    // 更新放大器数量
                    dp[nextNode] = newAmplifiers;
                    distance[nextNode] = weight;// 重置距离
                    // 更新父节点
                    parent[nextNode] = node;
                    hasAmplifier[node] = true;
                }
            }
        }
    }
    
    // 计算放大器位置
    /**
     * 找出所有的叶子节点
     * 通过比较到达节点的累积距离和预期累积距离，判断是否在途中放置了放大器
     * 如果发现差异，标记放大器的位置
     */
    if (amplifierLocations) {
        amplifierLocations->assign(nodeCount_, false);
        
        // 找出叶子节点（拓扑排序的最后几个节点）
        set<int> leafNodes;
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
            maxAmplifiers = max(maxAmplifiers, dp[i]);
        }
    }
    
    return maxAmplifiers;
}