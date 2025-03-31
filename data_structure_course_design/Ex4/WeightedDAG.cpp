#include "WeightedDAG.h"
#include <algorithm>
#include <limits>

// 方法一：贪心算法解决设置信号放大器问题
int WeightedDAG::minimumAmplifiersGreedy(int sourceNode, double d) {
    // 1. 获取拓扑排序
    vector<int> topoOrder = topologicalSort();
    if (topoOrder.empty()) return -1; // 图不是DAG
    
    // 2. 初始化累计距离数组，所有节点初始为无穷大
    vector<double> distance(n, numeric_limits<double>::infinity());
    distance[sourceNode] = 0; // 源点距离为0
    
    // 3. 初始化放大器计数
    int amplifierCount = 0;
    
    // 4. 记录放大器放置的位置
    vector<bool> hasAmplifier(n, false);
    
    // 5. 按拓扑排序顺序处理每个节点
    for (int node : topoOrder) {
        // 如果该节点不可达，跳过
        if (distance[node] == numeric_limits<double>::infinity())
            continue;
            
        // 检查所有出边
        for (const auto& edge : adj[node]) {
            int nextNode = edge.first;
            double weight = edge.second;
            
            // 如果当前节点到下一个节点的距离会超过d
            if (distance[node] + weight > d) {
                // 需要在当前节点放置放大器
                if (!hasAmplifier[node]) {
                    hasAmplifier[node] = true;
                    amplifierCount++;
                    // 重置当前节点的累计距离为0
                    distance[node] = 0;
                }
            }
            
            // 更新下一个节点的距离
            // 如果当前节点有放大器，下一节点距离就是边的权重
            // 否则累加当前节点的距离
            distance[nextNode] = min(distance[nextNode], 
                                   hasAmplifier[node] ? weight : distance[node] + weight);
        }
    }
    
    return amplifierCount;
}

// 辅助方法：计算所有节点对之间的最短距离
vector<vector<double>> WeightedDAG::calculateAllPairsShortestPaths() {
    // 初始化距离矩阵，所有位置为无穷大
    vector<vector<double>> dist(n, vector<double>(n, numeric_limits<double>::infinity()));
    
    // 自己到自己的距离为0
    for (int i = 0; i < n; i++) {
        dist[i][i] = 0;
    }
    
    // 初始化直接连接的边
    for (int i = 0; i < n; i++) {
        for (const auto& edge : adj[i]) {
            dist[i][edge.first] = edge.second;
        }
    }
    
    // Floyd-Warshall算法
    for (int k = 0; k < n; k++) {
        for (int i = 0; i < n; i++) {
            for (int j = 0; j < n; j++) {
                if (dist[i][k] != numeric_limits<double>::infinity() && 
                    dist[k][j] != numeric_limits<double>::infinity()) {
                    dist[i][j] = min(dist[i][j], dist[i][k] + dist[k][j]);
                }
            }
        }
    }
    
    return dist;
}

// 方法二：动态规划解决设置信号放大器问题
int WeightedDAG::minimumAmplifiersDP(int sourceNode, double d) {
    // 1. 获取拓扑排序
    vector<int> topoOrder = topologicalSort();
    if (topoOrder.empty()) return -1; // 图不是DAG
    
    // 2. 计算所有节点对的最短距离
    vector<vector<double>> shortestPaths = calculateAllPairsShortestPaths();
    
    // 3. 初始化dp数组
    // dp[i][j]: 表示从源点到节点i，最后一个放大器放在节点j时，最少需要的放大器数量
    vector<vector<int>> dp(n, vector<int>(n, INT_MAX));
    
    // 4. 初始条件：源点不放置放大器
    dp[sourceNode][sourceNode] = 0;
    
    // 5. 按拓扑顺序处理每个节点
    for (int i = 0; i < topoOrder.size(); i++) {
        int currentNode = topoOrder[i];
        
        // 如果当前节点是源点，继续处理下一个节点
        if (currentNode == sourceNode) continue;
        
        // 考虑所有可能的前驱节点和最后放大器位置
        for (int j = 0; j < i; j++) {
            int prevNode = topoOrder[j];
            
            // 检查prevNode是否是currentNode的前驱
            bool isDirectPredecessor = false;
            for (const auto& edge : adj[prevNode]) {
                if (edge.first == currentNode) {
                    isDirectPredecessor = true;
                    break;
                }
            }
            
            if (!isDirectPredecessor) continue;
            
            // 对于每个可能的最后放大器位置
            for (int lastAmp = 0; lastAmp < n; lastAmp++) {
                // 如果到达prevNode的方案无效，跳过
                if (dp[prevNode][lastAmp] == INT_MAX) continue;
                
                // 情况1：在currentNode放置放大器
                if (dp[prevNode][lastAmp] != INT_MAX) {
                    dp[currentNode][currentNode] = min(dp[currentNode][currentNode],
                                                    dp[prevNode][lastAmp] + 1);
                }
                
                // 情况2：不在currentNode放置放大器
                // 需要检查从最后一个放大器到currentNode的距离是否超过d
                if (shortestPaths[lastAmp][currentNode] <= d) {
                    dp[currentNode][lastAmp] = min(dp[currentNode][lastAmp],
                                                 dp[prevNode][lastAmp]);
                }
            }
        }
    }
    
    // 6. 找出从源点到每个节点的最小放大器数量
    int minAmplifiers = INT_MAX;
    for (int j = 0; j < n; j++) {
        for (int i = 0; i < n; i++) {
            if (dp[i][j] != INT_MAX) {
                minAmplifiers = min(minAmplifiers, dp[i][j]);
            }
        }
    }
    
    return minAmplifiers == INT_MAX ? -1 : minAmplifiers;
}