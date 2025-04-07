#ifndef AMPLIFIER_PLACEMENT_H
#define AMPLIFIER_PLACEMENT_H

#include "WeightedDAG.h"
#include <chrono>
#include <random>
#include <utility>
#include <string>
#include <vector>

// 定义性能测试结果结构体
struct PerformanceResult {
    int nodeCount;
    double greedyTime;
    double dpTime;
    int greedyAmplifiers;
    int dpAmplifiers;
};

// 定义图生成器类
class GraphGenerator {
public:
    // 生成随机DAG
    static WeightedDAG generateRandomDAG(int nodeCount, double edgeProbability, 
                                        int minWeight, int maxWeight, 
                                        std::mt19937* gen = nullptr);
                                        
    // 生成示例图
    static WeightedDAG generateExampleGraph();
};

// 定义可视化工具类
class Visualizer {
public:
    // 将结果保存为DOT文件
    static void visualizeResult(const WeightedDAG& graph, 
                               const std::vector<bool>& hasAmplifier, 
                               const std::string& filename);
                               
    // 生成可视化解决方案
    static void visualizeSolution(const WeightedDAG& graph, 
                                 int sourceNode, 
                                 double maxDistance,
                                 const std::string& filename);
};

// 定义性能测试类
class PerformanceTester {
public:
    // 执行性能测试
    static std::vector<PerformanceResult> compareAlgorithms(
        int minNodes, int maxNodes, int step, double maxDistance, int testsPerSize);
        
    // 输出性能测试结果
    static void outputResults(const std::vector<PerformanceResult>& results);
};

// 定义主算法工具类
class AmplifierPlacement {
public:
    // 获取放大器位置（使用贪心算法）
    static std::vector<bool> getAmplifierPlacementGreedy(
        WeightedDAG& graph, int sourceNode, double maxDistance);
        
    // 获取放大器位置（使用动态规划）
    static std::vector<bool> getAmplifierPlacementDP(
        WeightedDAG& graph, int sourceNode, double maxDistance);
        
    // 为特定图生成和可视化解决方案
    static void visualizeSolutionForGraph(int nodeCount, int testNumber, double maxDistance);
};

#endif // AMPLIFIER_PLACEMENT_H