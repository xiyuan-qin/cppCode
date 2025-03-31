#include "AmplifierPlacement.h"
#include <iostream>
#include <fstream>
#include <iomanip>

// 创建随机DAG用于测试
WeightedDAG generateRandomDAG(int nodeCount, double edgeProbability, int minWeight, int maxWeight) {
    WeightedDAG graph(nodeCount);
    
    // 使用随机数引擎
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<> edgeProb(0.0, 1.0);
    std::uniform_int_distribution<> weightDist(minWeight, maxWeight);
    
    // 确保图是连通的，每个节点至少有一条入边（除了源点）
    for (int i = 1; i < nodeCount; i++) {
        int from = std::uniform_int_distribution<>(0, i - 1)(gen);
        int weight = weightDist(gen);
        graph.addEdge(from, i, weight);
    }
    
    // 添加额外的随机边，但保持DAG性质
    for (int i = 0; i < nodeCount; i++) {
        for (int j = i + 1; j < nodeCount; j++) {
            if (edgeProb(gen) < edgeProbability) {
                int weight = weightDist(gen);
                graph.addEdge(i, j, weight);
            }
        }
    }
    
    return graph;
}

// 性能测试函数
void compareAlgorithmPerformance(int minNodes, int maxNodes, int step, double d, int testsPerSize) {
    vector<pair<int, pair<double, double>>> results; // 存储节点数和两种算法的平均时间
    
    for (int nodeCount = minNodes; nodeCount <= maxNodes; nodeCount += step) {
        double totalGreedyTime = 0.0;
        double totalDPTime = 0.0;
        
        for (int test = 0; test < testsPerSize; test++) {
            // 创建随机DAG
            WeightedDAG graph = generateRandomDAG(nodeCount, 0.3, 1, 10);
            
            // 测试贪心算法
            auto startGreedy = std::chrono::high_resolution_clock::now();
            int greedyResult = graph.minimumAmplifiersGreedy(0, d);
            auto endGreedy = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> greedyDuration = endGreedy - startGreedy;
            
            // 测试动态规划算法
            auto startDP = std::chrono::high_resolution_clock::now();
            int dpResult = graph.minimumAmplifiersDP(0, d);
            auto endDP = std::chrono::high_resolution_clock::now();
            std::chrono::duration<double, std::milli> dpDuration = endDP - startDP;
            
            // 累加时间
            totalGreedyTime += greedyDuration.count();
            totalDPTime += dpDuration.count();
            
            // 验证两种算法结果是否一致
            if (greedyResult != dpResult) {
                std::cout << "Warning: Different results for nodeCount=" << nodeCount 
                          << " - Greedy: " << greedyResult 
                          << ", DP: " << dpResult << std::endl;
            }
        }
        
        // 计算平均时间
        double avgGreedyTime = totalGreedyTime / testsPerSize;
        double avgDPTime = totalDPTime / testsPerSize;
        
        // 存储结果
        results.push_back({nodeCount, {avgGreedyTime, avgDPTime}});
        
        std::cout << "Nodes: " << nodeCount 
                  << ", Greedy avg: " << avgGreedyTime << "ms"
                  << ", DP avg: " << avgDPTime << "ms" << std::endl;
    }
    
    // 输出结果
    outputPerformanceResults(results);
}

// 输出性能测试结果
void outputPerformanceResults(const vector<pair<int, pair<double, double>>>& results) {
    // 输出到CSV文件
    std::ofstream csvFile("performance_results.csv");
    csvFile << "NodeCount,GreedyTime(ms),DPTime(ms)" << std::endl;
    
    for (const auto& result : results) {
        csvFile << result.first << "," 
                << result.second.first << "," 
                << result.second.second << std::endl;
    }
    
    csvFile.close();
    
    // 输出到控制台表格
    std::cout << "\n===== Performance Results =====\n";
    std::cout << std::setw(10) << "Nodes" 
              << std::setw(15) << "Greedy (ms)" 
              << std::setw(15) << "DP (ms)" 
              << std::setw(15) << "Ratio (DP/Greedy)" << std::endl;
    
    for (const auto& result : results) {
        double ratio = result.second.second / result.second.first;
        std::cout << std::setw(10) << result.first 
                  << std::setw(15) << std::fixed << std::setprecision(2) << result.second.first 
                  << std::setw(15) << std::fixed << std::setprecision(2) << result.second.second 
                  << std::setw(15) << std::fixed << std::setprecision(2) << ratio << std::endl;
    }
}