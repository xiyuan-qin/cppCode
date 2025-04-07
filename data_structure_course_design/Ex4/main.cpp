#include "WeightedDAG.h"
#include "AmplifierPlacement.h"
#include <iostream>

// 简单测试示例
void testWithSimpleGraph() {
    // 使用工厂方法创建示例图
    WeightedDAG graph = GraphGenerator::generateExampleGraph();
    
    std::cout << "测试简单图:\n";
    
    // 不同d值的测试
    double dValues[] = {3.0, 4.0, 5.0, 6.0};
    for (double d : dValues) {
        std::vector<bool> greedyAmplifiers;
        std::vector<bool> dpAmplifiers;
        
        int greedyResult = graph.minimumAmplifiersGreedy(0, d, &greedyAmplifiers);
        int dpResult = graph.minimumAmplifiersDP(0, d, &dpAmplifiers);
        
        std::cout << "d = " << d << ":\n";
        std::cout << "  贪心算法: " << greedyResult << " 个放大器\n";
        std::cout << "  动态规划: " << dpResult << " 个放大器\n";
        
        // 检查两种算法的放大器位置
        int greedyCount = 0, dpCount = 0;
        for (bool has : greedyAmplifiers) if (has) greedyCount++;
        for (bool has : dpAmplifiers) if (has) dpCount++;
        
        std::cout << "  贪心放大器位置数: " << greedyCount << ", 动态规划放大器位置数: " << dpCount << "\n";
    }
    
    // 为示例图生成可视化结果
    double d = 4.0;
    
    // 使用贪心算法获取放大器位置
    std::vector<bool> hasAmplifier = AmplifierPlacement::getAmplifierPlacementGreedy(graph, 0, d);
    
    // 生成可视化结果
    Visualizer::visualizeResult(graph, hasAmplifier, "amplifier_placement_example.dot");
    
    std::cout << "示例图可视化结果已保存至: amplifier_placement_example.dot\n";
}

int main() {
    // 简单图测试
    testWithSimpleGraph();
    
    std::cout << "\n性能比较测试开始...\n";
    
    // 执行性能测试
    auto results = PerformanceTester::compareAlgorithms(10, 100, 10, 5.0, 5);
    
    // 输出性能测试结果
    PerformanceTester::outputResults(results);
    
    // 为特定图生成可视化结果
    std::cout << "\n为特定图生成可视化结果...\n";
    AmplifierPlacement::visualizeSolutionForGraph(15, 1, 5.0);
    
    // 控制台输出更多信息
    std::cout << "\n说明: DOT文件可以使用Graphviz等工具可视化查看\n";
    std::cout << "命令示例: dot -Tpng graph_15_test_1.dot -o example.png\n";
    
    return 0;
}