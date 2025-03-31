#include "WeightedDAG.h"
#include "AmplifierPlacement.h"
#include <iostream>

// 简单测试示例
void testWithSimpleGraph() {
    // 创建一个简单的有向无环图
    WeightedDAG graph(6);
    
    // 添加边
    graph.addEdge(0, 1, 2);  // 源点到1的距离为2
    graph.addEdge(0, 2, 4);  // 源点到2的距离为4
    graph.addEdge(1, 3, 3);  // 1到3的距离为3
    graph.addEdge(2, 3, 1);  // 2到3的距离为1
    graph.addEdge(2, 4, 2);  // 2到4的距离为2
    graph.addEdge(3, 5, 5);  // 3到5的距离为5
    graph.addEdge(4, 5, 3);  // 4到5的距离为3
    
    std::cout << "测试简单图:\n";
    
    // 不同d值的测试
    double dValues[] = {3.0, 4.0, 5.0, 6.0};
    for (double d : dValues) {
        int greedyResult = graph.minimumAmplifiersGreedy(0, d);
        int dpResult = graph.minimumAmplifiersDP(0, d);
        
        std::cout << "d = " << d << ":\n";
        std::cout << "  贪心算法: " << greedyResult << " 个放大器\n";
        std::cout << "  动态规划: " << dpResult << " 个放大器\n";
    }
}

int main() {
    // 简单图测试
    testWithSimpleGraph();
    
    std::cout << "\n性能比较测试开始...\n";
    
    // 性能比较（小规模）
    compareAlgorithmPerformance(10, 100, 10, 5.0, 5);
    
    // 性能比较（大规模，可选）
    // compareAlgorithmPerformance(100, 500, 50, 5.0, 3);
    
    return 0;
}