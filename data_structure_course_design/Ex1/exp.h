#ifndef EXP_H  
#define EXP_H

#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <chrono>
#include <algorithm> 
#include <cmath>
#include "SkipList.h" 

using namespace std;


bool binarySearch(const vector<int>& arr, int target) {
    int left = 0, right = arr.size() - 1;
    while (left <= right) {
        int mid = left + (right - left) / 2;
        if (arr[mid] == target) return true;
        if (arr[mid] < target) left = mid + 1;
        else right = mid - 1;
    }
    return false;
}

// 插入排序数组的函数
void insertSortedArray(vector<int>& arr, int value) {
    auto it = lower_bound(arr.begin(), arr.end(), value);
    arr.insert(it, value);
}

// 从排序数组中删除值
bool deleteSortedArray(vector<int>& arr, int value) {
    auto it = lower_bound(arr.begin(), arr.end(), value);
    if (it != arr.end() && *it == value) {
        arr.erase(it);
        return true;
    }
    return false;
}

// // 添加范围查询比较测试
// void testRangeQuery(SkipList& sk, vector<int>& arr) {
//     if (arr.empty()) return;
    
//     int rangeSize = max(1, (int)arr.size() / 10);  // 查询10%的数据
//     int start = rand() % (arr.size() - rangeSize + 1);
//     int minVal = arr[start];
//     int maxVal = arr[start + rangeSize - 1];
    
//     // 跳表范围查询
//     auto startSkip = chrono::high_resolution_clock::now();
//     vector<int> skipResult = sk.rangeQuery(minVal, maxVal);
//     auto endSkip = chrono::high_resolution_clock::now();
//     double timeSkip = chrono::duration_cast<chrono::microseconds>(endSkip - startSkip).count();
    
//     // 数组范围查询
//     auto startArray = chrono::high_resolution_clock::now();
//     vector<int> arrayResult;
//     auto lowerIt = lower_bound(arr.begin(), arr.end(), minVal);
//     auto upperIt = upper_bound(arr.begin(), arr.end(), maxVal);
//     arrayResult.insert(arrayResult.end(), lowerIt, upperIt);
//     auto endArray = chrono::high_resolution_clock::now();
//     double timeArray = chrono::duration_cast<chrono::microseconds>(endArray - startArray).count();
    
//     cout << "范围查询 [" << minVal << ", " << maxVal << "] (" << skipResult.size() << " 个元素):\n";
//     cout << "跳表: " << timeSkip << " μs | 数组: " << timeArray << " μs\n";
// }

void experiment() {

    // 实验参数
    const int MAX_LEVEL = 32;        // 增加最大层数以提高性能
    const int INIT_SIZE = 100000;    // 增大初始数据量以更好展示性能差异
    const int OPS = 200000;          // 增加操作次数以获得更稳定的结果
    const int REBUILD_THRESHOLD = 10000; // 增加重建阈值
    const int REPORT_INTERVAL = 10000;  // 调整报告间隔
    
    cout << "\n┌───────────────────────────────────────────────────────┐" << endl;
    cout << "│          跳表与排序数组性能对比实验                   │" << endl;
    cout << "└───────────────────────────────────────────────────────┘\n" << endl;
    

    // ==================初始化=========================================================
    
    
    cout << "初始化数据中... (数据量: " << INIT_SIZE << ")\n" << endl;
    
    // 初始化跳表和排序数组
    SkipList sk(MAX_LEVEL);// 初始化跳表
    vector<int> sortedArray;
    
    // 预分配空间以减少重新分配
    sortedArray.reserve(INIT_SIZE + OPS/2);
    
    // 生成随机初始数据
    vector<int> randomValues(INIT_SIZE);
    for (int i = 0; i < INIT_SIZE; i++) {
        randomValues[i] = rand() % 10000000;
    }
    
    cout << "\n>>> 插入初始数据...\n" << endl;
    // 分别测量初始化时间
    //===================跳表初始化=========================
    auto startSkipInit = chrono::high_resolution_clock::now();// 跳表初始化的计时开始
    for (int val : randomValues) {
        sk.insert(val);
    }
    auto endSkipInit = chrono::high_resolution_clock::now();// 结束
    double skipInitTime = chrono::duration_cast<chrono::milliseconds>(endSkipInit - startSkipInit).count();
    

    //===================数组初始化=========================
    auto startArrayInit = chrono::high_resolution_clock::now();// 数组初始化的计时开始
    for (int val : randomValues) {
        insertSortedArray(sortedArray, val);
    }
    auto endArrayInit = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startArrayInit).count();
    
    cout << "\n┌───────────────────────────────────────────────────────┐" << endl;
    cout << "│          初始化完成                                   │" << endl;
    cout << "├───────────────────────────────────────────────────────┤" << endl;
    cout << "│  跳表初始化: " << setw(8) << skipInitTime << " ms" << setw(22) << "│" << endl;
    cout << "│  数组初始化: " << setw(8) << endArrayInit << " ms" << setw(22) << "│" << endl;
    cout << "│  速度比例  : " << setw(8) << (endArrayInit > 0 ? skipInitTime / endArrayInit : 0) << "x" << setw(22) << "│" << endl;
    cout << "└───────────────────────────────────────────────────────┘\n" << endl;
    


    // ==================性能测试=========================================================
    cout << ">>> 开始性能测试... (预计操作数: " << OPS << ")\n" << endl;
    
    // 用于记录各操作的累积时间
    vector<double> insertTimesSkip, searchTimesSkip, deleteTimesSkip;
    vector<double> insertTimesArray, searchTimesArray, deleteTimesArray;
    
    double totalInsertSkip = 0, totalSearchSkip = 0, totalDeleteSkip = 0;
    double totalInsertArray = 0, totalSearchArray = 0, totalDeleteArray = 0;
    
    int opCount = 0;
    bool needRebuild = false;

    // 操作序列
    for (int i = 0; i < OPS; i++) {
        int op = rand() % 3;  // 0=插入，1=查找，2=删除
        // 在计时开始前生成随机键值，避免随机数生成影响测量
        int key = rand() % 10000000;
        opCount++;


        // 分别对跳表和数组执行相同操作并计时

        //===============跳表操作计时=========================
        auto startSkip = chrono::high_resolution_clock::now();
        switch(op) {
            case 0: sk.insert(key); break;
            case 1: sk.search(key); break;
            case 2: {
                if (!sk.search(key)) {
                    // 如果元素不存在，先插入再删除，确保删除真正发生
                    sk.insert(key);
                }
                sk.erase(key);
                break;
            }
        }
        auto endSkip = chrono::high_resolution_clock::now();
        double timeSkip = chrono::duration_cast<chrono::nanoseconds>(endSkip - startSkip).count();

        //===============数组操作计时=========================
        auto startArray = chrono::high_resolution_clock::now();
        switch(op) {
            case 0: insertSortedArray(sortedArray, key); break;
            case 1: binarySearch(sortedArray, key); break;
            case 2: {
                if (!binarySearch(sortedArray, key)) {
                    // 如果元素不存在，先插入再删除，确保删除真正发生
                    insertSortedArray(sortedArray, key);
                }
                deleteSortedArray(sortedArray, key);
                break;
            }
        }
        auto endArray = chrono::high_resolution_clock::now();
        double timeArray = chrono::duration_cast<chrono::nanoseconds>(endArray - startArray).count();

        // 总运行时间
        if (op == 0) {
            totalInsertSkip += timeSkip;
            totalInsertArray += timeArray;
            insertTimesSkip.push_back(timeSkip);
            insertTimesArray.push_back(timeArray);
        } else if (op == 1) {
            totalSearchSkip += timeSkip;
            totalSearchArray += timeArray;
            searchTimesSkip.push_back(timeSkip);
            searchTimesArray.push_back(timeArray);
        } else {
            totalDeleteSkip += timeSkip;
            totalDeleteArray += timeArray;
            deleteTimesSkip.push_back(timeSkip);
            deleteTimesArray.push_back(timeArray);
        }

        // 每REPORT_INTERVAL次操作输出一次平均时间
        if ((i + 1) % REPORT_INTERVAL == 0) {
            // 计算平均时间
            double avgInsertSkip = insertTimesSkip.empty() ? 0 : totalInsertSkip / insertTimesSkip.size();
            double avgSearchSkip = searchTimesSkip.empty() ? 0 : totalSearchSkip / searchTimesSkip.size();
            double avgDeleteSkip = deleteTimesSkip.empty() ? 0 : totalDeleteSkip / deleteTimesSkip.size();
            
            double avgInsertArray = insertTimesArray.empty() ? 0 : totalInsertArray / insertTimesArray.size();
            double avgSearchArray = searchTimesArray.empty() ? 0 : totalSearchArray / searchTimesArray.size();
            double avgDeleteArray = deleteTimesArray.empty() ? 0 : totalDeleteArray / deleteTimesArray.size();
            
            cout << "\n┌───────────────────────────────────────────────────────┐" << endl;
            cout << "│          进度报告: " << setw(6) << (i + 1) << "/" << setw(6) << OPS << " 操作完成         │" << endl;
            cout << "└───────────────────────────────────────────────────────┘\n" << endl;
            
            cout << "跳表平均时间 (ns):" << endl;
            cout << "  插入: " << avgInsertSkip << endl;
            cout << "  查找: " << avgSearchSkip << endl;
            cout << "  删除: " << avgDeleteSkip << endl << endl;
            
            cout << "排序数组平均时间 (ns):" << endl;
            cout << "  插入: " << avgInsertArray << endl;
            cout << "  查找: " << avgSearchArray << endl;
            cout << "  删除: " << avgDeleteArray << endl << endl;
            
            cout << "性能对比 (跳表时间/数组时间):" << endl;
            if (avgInsertArray > 0) {
                double ratio = avgInsertSkip / avgInsertArray;
                cout << "  插入: " << ratio << "x " 
                     << (ratio < 1 ? "(跳表更快)" : "(数组更快)") << endl;
            }
            
            if (avgSearchArray > 0) {
                double ratio = avgSearchSkip / avgSearchArray;
                cout << "  查找: " << ratio << "x " 
                     << (ratio < 1 ? "(跳表更快)" : "(数组更快)") << endl;
            }
            
            if (avgDeleteArray > 0) {
                double ratio = avgDeleteSkip / avgDeleteArray;
                cout << "  删除: " << ratio << "x " 
                     << (ratio < 1 ? "(跳表更快)" : "(数组更快)") << endl;
            }
            
            cout << "─────────────────────────────────────────────────────────\n";
        }

        // 重建
        if (opCount >= REBUILD_THRESHOLD) {
            double expectedLevel = log2(sortedArray.size()) / 2;// 理论最优层数
            bool levelImbalance = expectedLevel > sk.getCurrentLevel() * 1.5; 
            bool performanceDrop = (!insertTimesSkip.empty() && 
                                   insertTimesSkip.back() > 3.0 * (totalInsertSkip / insertTimesSkip.size()));
                
            if (levelImbalance || performanceDrop) {
                needRebuild = true;
                
                // 输出详细的重建原因
                cout << "\n>>> 跳表重建触发原因分析:" << endl;
                cout << "  当前操作次数: " << i << endl;
                cout << "  当前元素数量: " << sortedArray.size() << endl;
                cout << "  当前跳表层数: " << sk.getCurrentLevel() << endl;
                cout << "  理论最优层数: " << expectedLevel << endl;
                
                if (levelImbalance) {
                    cout << "  [重建原因] 层级不平衡: 理论层数(" << expectedLevel 
                         << ")超过当前层数(" << sk.getCurrentLevel() << ")的1.5倍" << endl;
                }
                
                if (performanceDrop) {
                    cout << "  [重建原因] 性能下降: 最近一次插入时间(" << insertTimesSkip.back() 
                         << "ns)超过平均插入时间(" << (totalInsertSkip / insertTimesSkip.size()) 
                         << "ns)的3倍" << endl;
                }
                cout << endl;
            }
        }

        // 执行重建
        if (needRebuild) {
            cout << "\n>>> 在 " << i << " 次操作后执行跳表重建\n" << endl;
            auto startRebuild = chrono::high_resolution_clock::now();
            
            // 打印重建前的状态
            cout << "  重建前状态:" << endl;
            cout << "    跳表层数: " << sk.getCurrentLevel() << endl;
            cout << "    元素数量: " << sortedArray.size() << endl;
            
            // 调用重建函数
            sk.rebuildSkipList(sk);
            
            auto endRebuild = chrono::high_resolution_clock::now();
            double rebuildTime = chrono::duration_cast<chrono::milliseconds>(endRebuild - startRebuild).count();
            
            // 打印重建后的状态
            cout << "  重建后状态:" << endl;
            cout << "    跳表层数: " << sk.getCurrentLevel() << endl;
            cout << "    用时: " << rebuildTime << " ms\n" << endl;
            
            opCount = 0;
            needRebuild = false;
        }
    }

    // 最终结果统计
    cout << "\n┌───────────────────────────────────────────────────────┐" << endl;
    cout << "│                最终性能统计                           │" << endl;
    cout << "└───────────────────────────────────────────────────────┘\n" << endl;
    
    double avgInsertSkip = insertTimesSkip.empty() ? 0 : totalInsertSkip / insertTimesSkip.size();
    double avgSearchSkip = searchTimesSkip.empty() ? 0 : totalSearchSkip / searchTimesSkip.size();
    double avgDeleteSkip = deleteTimesSkip.empty() ? 0 : totalDeleteSkip / deleteTimesSkip.size();
    
    double avgInsertArray = insertTimesArray.empty() ? 0 : totalInsertArray / insertTimesArray.size();
    double avgSearchArray = searchTimesArray.empty() ? 0 : totalSearchArray / searchTimesArray.size();
    double avgDeleteArray = deleteTimesArray.empty() ? 0 : totalDeleteArray / deleteTimesArray.size();
    
    cout << fixed << setprecision(2);  // 设置浮点数精度为2位
    
    cout << "跳表平均操作时间 (ns):" << endl;
    cout << "  插入: " << avgInsertSkip << endl;
    cout << "  查找: " << avgSearchSkip << endl;
    cout << "  删除: " << avgDeleteSkip << endl << endl;
    
    cout << "排序数组平均操作时间 (ns):" << endl;
    cout << "  插入: " << avgInsertArray << endl;
    cout << "  查找: " << avgSearchArray << endl;
    cout << "  删除: " << avgDeleteArray << endl << endl;
    
    cout << "性能对比结果 (时间比率):\n" << endl;
    
    // 插入操作
    cout << "  插入操作: ";
    if (avgInsertArray > 0) {
        double ratio = avgInsertSkip / avgInsertArray;
        if (ratio < 1)
            cout << "跳表快 " << (1/ratio) << " 倍" << endl;
        else
            cout << "数组快 " << ratio << " 倍" << endl;
    } else {
        cout << "无法比较" << endl;
    }
    
    // 查找操作
    cout << "  查找操作: ";
    if (avgSearchArray > 0) {
        double ratio = avgSearchSkip / avgSearchArray;
        if (ratio < 1)
            cout << "跳表快 " << (1/ratio) << " 倍" << endl;
        else
            cout << "数组快 " << ratio << " 倍" << endl;
    } else {
        cout << "无法比较" << endl;
    }
    
    // 删除操作
    cout << "  删除操作: ";
    if (avgDeleteArray > 0) {
        double ratio = avgDeleteSkip / avgDeleteArray;
        if (ratio < 1)
            cout << "跳表快 " << (1/ratio) << " 倍" << endl;
        else
            cout << "数组快 " << ratio << " 倍" << endl;
    } else {
        cout << "无法比较" << endl;
    }
    
    // 特殊操作测试
    cout << "\n┌───────────────────────────────────────────────────────┐" << endl;
    cout << "│                特殊操作性能测试                       │" << endl;
    cout << "└───────────────────────────────────────────────────────┘\n" << endl;
    
    // 删除最小元素测试
    auto startMinSkip = chrono::high_resolution_clock::now();
    int minKey = sk.deleteMin();
    auto endMinSkip = chrono::high_resolution_clock::now();
    double timeMinSkip = chrono::duration_cast<chrono::nanoseconds>(endMinSkip - startMinSkip).count();
    
    // 数组删除最小元素
    auto startMinArray = chrono::high_resolution_clock::now();
    int minValueArray = INT_MAX;
    if (!sortedArray.empty()) {
        minValueArray = sortedArray[0];
        sortedArray.erase(sortedArray.begin());
    }
    auto endMinArray = chrono::high_resolution_clock::now();
    double timeMinArray = chrono::duration_cast<chrono::nanoseconds>(endMinArray - startMinArray).count();
    
    cout << "删除最小元素:\n" << endl;
    cout << "  跳表: " << timeMinSkip << " ns (" << minKey << ")" << endl;
    cout << "  数组: " << timeMinArray << " ns (" << minValueArray << ")" << endl;
    
    if (timeMinArray > 0) {
        double ratio = timeMinSkip / timeMinArray;
        cout << "  性能比较: ";
        if (ratio < 1)
            cout << "跳表快 " << (1/ratio) << " 倍\n" << endl;
        else
            cout << "数组快 " << ratio << " 倍\n" << endl;
    }
    
    // 删除最大元素测试
    auto startMaxSkip = chrono::high_resolution_clock::now();
    int maxKey = sk.deleteMax();
    auto endMaxSkip = chrono::high_resolution_clock::now();
    double timeMaxSkip = chrono::duration_cast<chrono::nanoseconds>(endMaxSkip - startMaxSkip).count();
    
    // 数组删除最大元素
    auto startMaxArray = chrono::high_resolution_clock::now();
    int maxValueArray = INT_MIN;
    if (!sortedArray.empty()) {
        maxValueArray = sortedArray.back();
        sortedArray.pop_back();
    }
    auto endMaxArray = chrono::high_resolution_clock::now();
    double timeMaxArray = chrono::duration_cast<chrono::nanoseconds>(endMaxArray - startMaxArray).count();
    
    cout << "删除最大元素:\n" << endl;
    cout << "  跳表: " << timeMaxSkip << " ns (" << maxKey << ")" << endl;
    cout << "  数组: " << timeMaxArray << " ns (" << maxValueArray << ")" << endl;
    
    if (timeMaxArray > 0) {
        double ratio = timeMaxSkip / timeMaxArray;
        cout << "  性能比较: ";
        if (ratio < 1)
            cout << "跳表快 " << (1/ratio) << " 倍\n" << endl;
        else
            cout << "数组快 " << ratio << " 倍\n" << endl;
    }
    
    // 理论时间复杂度分析
    cout << "\n┌───────────────────────────────────────────────────────┐" << endl;
    cout << "│                理论时间复杂度分析                     │" << endl;
    cout << "└───────────────────────────────────────────────────────┘\n" << endl;
    
    cout << "  ┌───────────┬────────────────┬────────────────┐" << endl;
    cout << "  │   操作    │     跳表       │    排序数组    │" << endl;
    cout << "  ├───────────┼────────────────┼────────────────┤" << endl;
    cout << "  │   查找    │   O(log n)     │   O(log n)     │" << endl;
    cout << "  │   插入    │   O(log n)     │   O(n)         │" << endl;
    cout << "  │   删除    │   O(log n)     │   O(n)         │" << endl;
    cout << "  │ 删除最小  │   O(1)         │   O(n)         │" << endl;
    cout << "  │ 删除最大  │   O(log n)     │   O(1)         │" << endl;
    cout << "  │ 范围查询  │ O(log n + k)   │ O(log n + k)   │" << endl;
    cout << "  └───────────┴────────────────┴────────────────┘" << endl;
    cout << "    注: n 为元素总数，k 为范围内元素数量\n" << endl;
    
    // 添加更多理论分析和性能说明
    cout << "跳表性能分析:\n" << endl;
    cout << "  1. 查找和插入: 跳表O(log n)性能接近理想平衡树" << endl;
    cout << "  2. 随机访问: 跳表查找比平衡树慢约30%，但实现更简单" << endl;
    cout << "  3. 空间占用: 跳表平均每个节点额外使用1/(1-p)个指针" << endl;
    cout << "  4. 删除操作: 跳表删除只需O(log n)时间，不需要重平衡" << endl;
    cout << "  5. 范围查询: 跳表范围查询性能优秀，适合此类应用\n" << endl;
    
    cout << "跳表的优势应用场景:\n" << endl;
    cout << "  1. 频繁插入和删除的动态数据集" << endl;
    cout << "  2. 内存数据库索引" << endl;
    cout << "  3. 需要快速范围查询的应用" << endl;
    cout << "  4. 对删除最小元素有O(1)需求的优先队列实现\n" << endl;
    
    cout << "┌───────────────────────────────────────────────────────┐" << endl;
    cout << "│                实验结束                               │" << endl;
    cout << "└───────────────────────────────────────────────────────┘" << endl;
}

void experiment();  // 添加实验函数的声明

#endif // EXP_H  // 修改结束宏定义以匹配新名称