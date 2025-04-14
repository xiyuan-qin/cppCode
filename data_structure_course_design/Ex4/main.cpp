#include "WeightedDAG.h"
#include "AmplifierPlacement.h"
#include <iostream>
#include <fstream>
#include <filesystem>

using namespace std;

// 简单测试示例
void testWithSimpleGraph() {
    // 使用工厂方法创建示例图
    WeightedDAG graph = GraphGenerator::generateExampleGraph();
    
    cout << "测试简单图:\n";
    
    // 不同d值的测试
    double dValues[] = {3.0, 4.0, 5.0, 6.0};
    for (double d : dValues) {
        vector<bool> greedyAmplifiers;
        vector<bool> dpAmplifiers;
        
        int greedyResult = graph.minimumAmplifiersGreedy(0, d, &greedyAmplifiers);
        int dpResult = graph.minimumAmplifiersDP(0, d, &dpAmplifiers);
        
        cout << "d = " << d << ":\n";
        cout << "  贪心算法: " << greedyResult << " 个放大器\n";
        cout << "  动态规划: " << dpResult << " 个放大器\n";
        
        // 检查两种算法的放大器位置
        int greedyCount = 0, dpCount = 0;
        for (bool has : greedyAmplifiers) if (has) greedyCount++;
        for (bool has : dpAmplifiers) if (has) dpCount++;
        
        cout << "  贪心放大器位置数: " << greedyCount << ", 动态规划放大器位置数: " << dpCount << "\n";
        
        // 记录放大器位置
        cout << "  贪心放大器位置: ";
        for (int i = 0; i < greedyAmplifiers.size(); i++) {
            if (greedyAmplifiers[i]) cout << i << " ";
        }
        cout << "\n  动态规划放大器位置: ";
        for (int i = 0; i < dpAmplifiers.size(); i++) {
            if (dpAmplifiers[i]) cout << i << " ";
        }
        cout << endl;
    }
    
    // 确保data目录存在
    filesystem::create_directory("data");
    
    // 为示例图生成可视化结果
    double d = 4.0;
    
    // 使用贪心算法和动态规划算法获取放大器位置
    vector<bool> greedyAmplifiers = AmplifierPlacement::getAmplifierPlacementGreedy(graph, 0, d);
    vector<bool> dpAmplifiers = AmplifierPlacement::getAmplifierPlacementDP(graph, 0, d);
    
    // 生成可视化结果
    Visualizer::visualizeResult(graph, greedyAmplifiers, "data/amplifier_placement_greedy.dot");
    Visualizer::visualizeResult(graph, dpAmplifiers, "data/amplifier_placement_dp.dot");
    
    cout << "示例图可视化结果已保存至data目录\n";
}

// 详细解释问题和解决方案
void explainProblem() {
    cout << "\n====== 汽油传送网络中的放大器放置问题 ======\n\n";
    cout << "问题描述:\n";
    cout << "在汽油传送网络中，压力的损失是所走距离的函数。为了保证网络正常运转，\n";
    cout << "必须保证最小压力Pmin。为了维持这个最小压力，我们可以在网络中的一些或\n";
    cout << "全部顶点放置压力放大器将压力恢复至最大可允许的量级Pmax。\n\n";
    cout << "参数d表示汽油在压力由Pmax降为Pmin时所走的距离。我们的目标是放置最少\n";
    cout << "数量的放大器，使得在遇到一个放大器之前汽油所走的距离不超过d。\n\n";
    cout << "算法策略:\n";
    cout << "1. 贪心算法: 从每个叶子节点回溯至源点，当累积距离超过d时放置放大器\n";
    cout << "2. 动态规划: 使用状态转移方程，计算到达每个节点的最少放大器数量\n\n";
    cout << "以下将进行简单示例测试和性能比较测试...\n";
}

// 保存图数据供外部渲染
void saveGraphData(int nodeCount) {
    mt19937 gen(42); // 固定种子
    WeightedDAG graph = GraphGenerator::generateRandomDAG(nodeCount, 0.3, 1, 10, &gen);
    
    // 确保data目录存在
    filesystem::create_directory("data");
    
    // 保存图结构数据
    ofstream graphData("data/graph_structure.txt");
    graphData << nodeCount << "\n"; // 节点数量
    
    // 保存边信息
    for (int i = 0; i < nodeCount; i++) {
        for (const auto& edge : graph.getAdjList()[i]) {
            graphData << i << " " << edge.first << " " << edge.second << "\n";
        }
    }
    graphData.close();
    
    // 使用不同的d值计算放大器位置
    ofstream resultsFile("data/amplifier_results.txt");
    
    // 测试不同的d值
    double dValues[] = {3.0, 4.0, 5.0, 6.0, 7.0};
    resultsFile << "d值,贪心算法放大器数,动态规划放大器数\n";
    
    for (double d : dValues) {
        int greedyCount = graph.minimumAmplifiersGreedy(0, d);
        int dpCount = graph.minimumAmplifiersDP(0, d);
        resultsFile << d << "," << greedyCount << "," << dpCount << "\n";
    }
    
    resultsFile.close();
    
    cout << "图数据已保存至data/graph_structure.txt和data/amplifier_results.txt\n";
    
    // 生成d=5.0的可视化结果
    AmplifierPlacement::visualizeSolutionForGraph(nodeCount, 42, 5.0);
}

int main() {
    // 解释问题
    explainProblem();
    
    // 简单图测试
    testWithSimpleGraph();
    
    cout << "\n性能比较测试开始...\n";
    
    // 执行性能测试，使用更合理的参数范围
    // 节点数范围：10-100，步长10，参数d=5.0，每种规模测试5次
    auto results = PerformanceTester::compareAlgorithms(10, 100, 10, 5.0, 5);
    
    // 输出性能测试结果
    PerformanceTester::outputResults(results);
    
    // 生成具有50个节点的图数据，供外部工具可视化
    saveGraphData(50);
    
    // 为特定图生成可视化结果
    cout << "\n为特定图生成可视化结果...\n";
    AmplifierPlacement::visualizeSolutionForGraph(15, 1, 5.0);
    
    // 控制台输出更多信息
    cout << "\n说明: DOT文件可以使用Graphviz等工具可视化查看\n";
    cout << "命令示例: dot -Tpng data/graph_15_test_1.dot -o data/example.png\n";
    
    return 0;
}