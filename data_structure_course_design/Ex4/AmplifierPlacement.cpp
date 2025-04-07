#include "AmplifierPlacement.h"
#include <iostream>
#include <fstream>
#include <iomanip>
#include <sstream>

// GraphGenerator 类实现
WeightedDAG GraphGenerator::generateRandomDAG(int nodeCount, double edgeProbability, 
                                            int minWeight, int maxWeight,
                                            std::mt19937* gen) {
    // 创建随机数生成器
    std::random_device rd;
    std::mt19937 defaultGen(rd());
    std::mt19937& generator = gen ? *gen : defaultGen;
    
    std::uniform_real_distribution<> edgeProb(0.0, 1.0);
    std::uniform_int_distribution<> weightDist(minWeight, maxWeight);
    
    // 创建图实例
    WeightedDAG graph(nodeCount);
    
    // 确保图是连通的，每个节点至少有一条入边（除了源点）
    for (int i = 1; i < nodeCount; i++) {
        int from = std::uniform_int_distribution<>(0, i - 1)(generator);
        int weight = weightDist(generator);
        graph.addEdge(from, i, weight);
    }
    
    // 添加额外的随机边，保持DAG性质
    for (int i = 0; i < nodeCount; i++) {
        for (int j = i + 1; j < nodeCount; j++) {
            if (edgeProb(generator) < edgeProbability) {
                int weight = weightDist(generator);
                graph.addEdge(i, j, weight);
            }
        }
    }
    
    return graph;
}

WeightedDAG GraphGenerator::generateExampleGraph() {
    WeightedDAG graph(6);
    
    // 添加边
    graph.addEdge(0, 1, 2);
    graph.addEdge(0, 2, 4);
    graph.addEdge(1, 3, 3);
    graph.addEdge(2, 3, 1);
    graph.addEdge(2, 4, 2);
    graph.addEdge(3, 5, 5);
    graph.addEdge(4, 5, 3);
    
    return graph;
}

// Visualizer 类实现
void Visualizer::visualizeResult(const WeightedDAG& graph, 
                                const std::vector<bool>& hasAmplifier, 
                                const std::string& filename) {
    std::ofstream dotFile(filename);
    if (!dotFile.is_open()) {
        std::cerr << "无法创建DOT文件: " << filename << std::endl;
        return;
    }
    
    dotFile << "digraph AmplifierPlacement {\n";
    
    // 生成节点
    for (int i = 0; i < graph.getNodeCount(); i++) {
        dotFile << "  " << i << " [";
        // 为放大器节点添加不同样式
        if (i < hasAmplifier.size() && hasAmplifier[i]) {
            dotFile << "style=filled, color=lightblue, ";
        }
        dotFile << "label=\"" << i << "\"";
        dotFile << "];\n";
    }
    
    // 生成边
    const auto& adjList = graph.getAdjList();
    for (int i = 0; i < adjList.size(); i++) {
        for (const auto& edge : adjList[i]) {
            dotFile << "  " << i << " -> " << edge.first 
                    << " [label=\"" << edge.second << "\"];\n";
        }
    }
    
    dotFile << "}\n";
    dotFile.close();
    
    std::cout << "可视化结果已保存至: " << filename << std::endl;
}

void Visualizer::visualizeSolution(const WeightedDAG& graph, 
                                  int sourceNode, 
                                  double maxDistance,
                                  const std::string& filename) {
    // 创建一个可修改的图副本
    WeightedDAG graphCopy = graph;
    
    // 获取贪心和动态规划算法的放大器位置
    std::vector<bool> greedyLocations;
    std::vector<bool> dpLocations;
    
    int greedyAmplifiers = graphCopy.minimumAmplifiersGreedy(sourceNode, maxDistance, &greedyLocations);
    int dpAmplifiers = graphCopy.minimumAmplifiersDP(sourceNode, maxDistance, &dpLocations);
    
    // 使用放大器数量更少的结果
    std::vector<bool>& finalLocations = (greedyAmplifiers <= dpAmplifiers) ? greedyLocations : dpLocations;
    int finalCount = std::min(greedyAmplifiers, dpAmplifiers);
    
    // 可视化结果
    visualizeResult(graphCopy, finalLocations, filename);
    
    std::cout << "图中放置了 " << finalCount << " 个放大器，可视化结果已保存至: " << filename << std::endl;
    std::cout << "贪心算法: " << greedyAmplifiers << " 个放大器，动态规划: " << dpAmplifiers << " 个放大器" << std::endl;
}

// PerformanceTester 类实现
std::vector<PerformanceResult> PerformanceTester::compareAlgorithms(
    int minNodes, int maxNodes, int step, double maxDistance, int testsPerSize) {
    
    std::vector<PerformanceResult> results;
    
    std::cout << "\n===== 算法性能测试 =====\n";
    std::cout << "节点数范围: " << minNodes << "-" << maxNodes << ", 步长: " << step << "\n";
    std::cout << "每种规模测试次数: " << testsPerSize << ", 信号衰减阈值 d: " << maxDistance << "\n\n";
    
    for (int nodeCount = minNodes; nodeCount <= maxNodes; nodeCount += step) {
        PerformanceResult result;
        result.nodeCount = nodeCount;
        result.greedyTime = 0;
        result.dpTime = 0;
        result.greedyAmplifiers = 0;
        result.dpAmplifiers = 0;
        
        for (int test = 0; test < testsPerSize; test++) {
            // 创建随机DAG
            WeightedDAG graph = GraphGenerator::generateRandomDAG(nodeCount, 0.3, 1, 10);
            
            // 测试贪心算法
            auto startGreedy = std::chrono::high_resolution_clock::now();
            int greedyResult = graph.minimumAmplifiersGreedy(0, maxDistance);
            auto endGreedy = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> greedyDuration = endGreedy - startGreedy;
            
            // 测试动态规划算法
            auto startDP = std::chrono::high_resolution_clock::now();
            int dpResult = graph.minimumAmplifiersDP(0, maxDistance);
            auto endDP = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> dpDuration = endDP - startDP;
            
            // 累加时间和结果
            result.greedyTime += greedyDuration.count();
            result.dpTime += dpDuration.count();
            result.greedyAmplifiers += greedyResult;
            result.dpAmplifiers += dpResult;
            
            // 验证两种算法结果是否一致
            if (greedyResult != dpResult) {
                std::cout << "警告: 节点数=" << nodeCount 
                          << " 的测试 #" << test
                          << " - 贪心: " << greedyResult 
                          << ", 动态规划: " << dpResult << std::endl;
            }
        }
        
        // 计算平均值
        result.greedyTime /= testsPerSize;
        result.dpTime /= testsPerSize;
        result.greedyAmplifiers /= testsPerSize;
        result.dpAmplifiers /= testsPerSize;
        
        results.push_back(result);
        
        std::cout << "节点数: " << nodeCount 
                  << ", 贪心平均: " << result.greedyTime << "ms"
                  << ", 动态规划平均: " << result.dpTime << "ms" 
                  << ", 平均放大器数: " << result.greedyAmplifiers << std::endl;
    }
    
    return results;
}

void PerformanceTester::outputResults(const std::vector<PerformanceResult>& results) {
    // 输出到CSV文件
    std::ofstream csvFile("performance_results.csv");
    csvFile << "NodeCount,GreedyTime(ms),DPTime(ms),GreedyAmplifiers,DPAmplifiers" << std::endl;
    
    for (const auto& result : results) {
        csvFile << result.nodeCount << "," 
                << result.greedyTime << "," 
                << result.dpTime << ","
                << result.greedyAmplifiers << ","
                << result.dpAmplifiers << std::endl;
    }
    
    csvFile.close();
    
    // 输出到控制台表格
    std::cout << "\n===== 性能测试结果 =====\n";
    std::cout << std::setw(10) << "节点数" 
              << std::setw(15) << "贪心 (ms)" 
              << std::setw(15) << "动态规划 (ms)" 
              << std::setw(15) << "速度比(DP/贪心)" << std::endl;
    
    for (const auto& result : results) {
        double ratio = result.dpTime / result.greedyTime;
        std::cout << std::setw(10) << result.nodeCount 
                  << std::setw(15) << std::fixed << std::setprecision(2) << result.greedyTime 
                  << std::setw(15) << std::fixed << std::setprecision(2) << result.dpTime 
                  << std::setw(15) << std::fixed << std::setprecision(2) << ratio << std::endl;
    }
}

// AmplifierPlacement 类实现
std::vector<bool> AmplifierPlacement::getAmplifierPlacementGreedy(
    WeightedDAG& graph, int sourceNode, double maxDistance) {
    
    std::vector<bool> amplifierLocations;
    graph.minimumAmplifiersGreedy(sourceNode, maxDistance, &amplifierLocations);
    return amplifierLocations;
}

std::vector<bool> AmplifierPlacement::getAmplifierPlacementDP(
    WeightedDAG& graph, int sourceNode, double maxDistance) {
    
    std::vector<bool> amplifierLocations;
    graph.minimumAmplifiersDP(sourceNode, maxDistance, &amplifierLocations);
    return amplifierLocations;
}

void AmplifierPlacement::visualizeSolutionForGraph(int nodeCount, int testNumber, double maxDistance) {
    // 创建一个固定种子的随机生成器
    std::mt19937 gen(testNumber);
    
    // 创建图
    WeightedDAG graph = GraphGenerator::generateRandomDAG(
        nodeCount, 0.3, 1, 10, &gen);
    
    // 生成文件名
    std::string filename = "graph_" + std::to_string(nodeCount) + 
                         "_test_" + std::to_string(testNumber) + ".dot";
    
    // 可视化解决方案
    Visualizer::visualizeSolution(graph, 0, maxDistance, filename);
}