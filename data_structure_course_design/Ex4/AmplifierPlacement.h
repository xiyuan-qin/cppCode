#ifndef AMPLIFIER_PLACEMENT_H
#define AMPLIFIER_PLACEMENT_H

#include "WeightedDAG.h"
#include <chrono>
#include <random>
#include <utility>

// 性能测试函数
void compareAlgorithmPerformance(int minNodes, int maxNodes, int step, double d, int testsPerSize);

// 创建随机DAG用于测试
WeightedDAG generateRandomDAG(int nodeCount, double edgeProbability, int minWeight, int maxWeight);

// 输出性能测试结果
void outputPerformanceResults(const vector<pair<int, pair<double, double>>>& results);

#endif // AMPLIFIER_PLACEMENT_H