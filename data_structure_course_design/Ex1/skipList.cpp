#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <chrono>
#include <algorithm> // 添加算法库以使用upper_bound和lower_bound
#include <cmath>     // 添加数学库以使用log2函数

using namespace std;

class SkipNode {
public:
    int key;
    vector<SkipNode*> next;// 创建一个链表的指针数组，表示n层指针
    
    // 添加静态内存池以减少内存分配开销
    static vector<SkipNode*> pool;
    
    static void* operator new(size_t size) {
        if (!pool.empty()) {
            SkipNode* node = pool.back();
            pool.pop_back();
            return node;
        }
        return ::operator new(size);
    }
    
    static void operator delete(void* ptr) {
        if (pool.size() < 10000) { // 限制池大小，避免过度占用内存
            pool.push_back(static_cast<SkipNode*>(ptr));
        } else {
            ::operator delete(ptr);
        }
    }

    SkipNode(int key, int level) {
        this->key = key;
        this->next = vector<SkipNode*>(level, nullptr);//创建新节点时只实例化，不连接
    }
};

// 初始化静态内存池
vector<SkipNode*> SkipNode::pool;

class SkipList {
private:
    SkipNode* header;// 头节点
    int maxLevel;
    int currentLevel;
    float probability;

    int randomLevel() {
        int lvl = 1;
        while ((rand() / (float)RAND_MAX) < probability && lvl < maxLevel)
            lvl++;
        return lvl;
    }// 随机生成一个层数

public:
    // 修改默认参数：增加最大层数，降低概率以获得更平衡的结构
    SkipList(int maxLvl = 32, float p = 0.25) : maxLevel(maxLvl), currentLevel(1), probability(p) {
        header = new SkipNode(INT_MIN, maxLevel);// 创建头节点
        srand(time(0));
    }

    ~SkipList() {
        SkipNode* node = header->next[0];
        while (node) {
            SkipNode* next = node->next[0];
            delete node;
            node = next;
        }
        delete header;
    }

    bool search(int key) {
        SkipNode* current = header;
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->next[i] && current->next[i]->key < key)
                current = current->next[i];
        }
        current = current->next[0];
        return current != nullptr && current->key == key;
    }

    void insert(int key) {
        vector<SkipNode*> update(maxLevel, nullptr);
        SkipNode* current = header;

        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->next[i] && current->next[i]->key < key)//找到每一层比插入元素大的就停下来
                current = current->next[i];
            update[i] = current;// 记录每一层最后一个小于插入元素的节点
        }

        current = current->next[0];
        if (current && current->key == key) return;//不用插入重复元素

        int newLevel = randomLevel();//插入的层数
        if (newLevel > currentLevel) {
            for (int i = currentLevel; i < newLevel; i++)
                update[i] = header;
            currentLevel = newLevel;
        }//超出的话就把多出来的层的update都指向头节点，相当于头节点就是最后一个小于插入元素的节点

        SkipNode* newNode = new SkipNode(key, newLevel);
        for (int i = 0; i < newLevel; i++) {
            newNode->next[i] = update[i]->next[i];
            update[i]->next[i] = newNode;
        }//插
    }

    bool erase(int key) {
        vector<SkipNode*> update(maxLevel, nullptr);
        SkipNode* current = header;

        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->next[i] && current->next[i]->key < key)
                current = current->next[i];
            update[i] = current;
        }

        current = current->next[0];
        if (!current || current->key != key) return false;

        for (int i = 0; i < currentLevel; i++) {
            if (update[i]->next[i] != current) break;
            update[i]->next[i] = current->next[i];
        }

        delete current;
        while (currentLevel > 1 && header->next[currentLevel - 1] == nullptr)
            currentLevel--;

        return true;
    }

    int deleteMin() {
        SkipNode* minNode = header->next[0];
        if (!minNode) return INT_MIN;

        int minKey = minNode->key;
        for (int i = 0; i < minNode->next.size(); i++)
            header->next[i] = minNode->next[i];

        while (currentLevel > 1 && header->next[currentLevel - 1] == nullptr)
            currentLevel--;

        delete minNode;
        return minKey;
    }

    // 优化deleteMax实现，提高效率
    int deleteMax() {
        if (!header->next[0]) return INT_MIN;
        
        SkipNode* current = header;
        
        // 从最高层开始寻找最后一个节点
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->next[i] && current->next[i]->next[i]) {
                current = current->next[i];
            }
        }
        
        // 找到最大值节点
        current = header;
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->next[i]) {
                if (!current->next[i]->next[i]) {
                    break;
                }
                current = current->next[i];
            }
        }
        
        // 如果没有找到有效节点，则返回
        if (!current->next[0]) return INT_MIN;
        
        // 删除最大节点
        SkipNode* maxNode = current->next[0];
        int maxKey = maxNode->key;
        
        for (int i = 0; i < currentLevel && current->next[i] == maxNode; i++) {
            current->next[i] = nullptr; // 断开连接
        }
        
        delete maxNode;
        
        // 更新当前最大层数
        while (currentLevel > 1 && header->next[currentLevel - 1] == nullptr)
            currentLevel--;
            
        return maxKey;
    }

    void rebuildSkipList(SkipList& sk) {
        // 收集所有键值
        vector<int> allKeys;
        SkipNode* node = header->next[0];
        while (node) {
            allKeys.push_back(node->key);
            node = node->next[0];
        }
        
        // 清空当前跳表
        node = header->next[0];
        while (node) {
            SkipNode* next = node->next[0];
            delete node;
            node = next;
        }
        
        // 重置头节点指针
        for (int i = 0; i < maxLevel; i++)
            header->next[i] = nullptr;
        
        currentLevel = 1;
        
        // 重新插入所有元素
        for (int key : allKeys)
            insert(key);
    }

    // 优化范围查询实现，提高大范围查询性能
    vector<int> rangeQuery(int minKey, int maxKey) {
        vector<int> result;
        SkipNode* current = header;
        
        // 用最高层快速定位到范围起始位置
        for (int i = currentLevel - 1; i >= 0; i--) {
            while (current->next[i] && current->next[i]->key < minKey)
                current = current->next[i];
        }
        
        // 从找到的位置开始收集范围内的所有键值
        current = current->next[0];
        while (current && current->key <= maxKey) {
            result.push_back(current->key);
            current = current->next[0];
        }
        
        return result;
    }

    // 添加获取当前层级的方法
    int getCurrentLevel() const {
        return currentLevel;
    }
    
    // 添加获取最大层级的方法
    int getMaxLevel() const {
        return maxLevel;
    }
};

// 添加二分查找排序数组的函数，用于对比
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

// 添加范围查询比较测试
void testRangeQuery(SkipList& sk, vector<int>& arr) {
    if (arr.empty()) return;
    
    int rangeSize = max(1, (int)arr.size() / 10);  // 查询10%的数据
    int start = rand() % (arr.size() - rangeSize + 1);
    int minVal = arr[start];
    int maxVal = arr[start + rangeSize - 1];
    
    // 跳表范围查询
    auto startSkip = chrono::high_resolution_clock::now();
    vector<int> skipResult = sk.rangeQuery(minVal, maxVal);
    auto endSkip = chrono::high_resolution_clock::now();
    double timeSkip = chrono::duration_cast<chrono::microseconds>(endSkip - startSkip).count();
    
    // 数组范围查询
    auto startArray = chrono::high_resolution_clock::now();
    vector<int> arrayResult;
    auto lowerIt = lower_bound(arr.begin(), arr.end(), minVal);
    auto upperIt = upper_bound(arr.begin(), arr.end(), maxVal);
    arrayResult.insert(arrayResult.end(), lowerIt, upperIt);
    auto endArray = chrono::high_resolution_clock::now();
    double timeArray = chrono::duration_cast<chrono::microseconds>(endArray - startArray).count();
    
    cout << "范围查询 [" << minVal << ", " << maxVal << "] (" << skipResult.size() << " 个元素):\n";
    cout << "跳表: " << timeSkip << " μs | 数组: " << timeArray << " μs\n";
}

void experiment() {
    const int MAX_LEVEL = 32;        // 增加最大层数以提高性能
    const int INIT_SIZE = 100000;    // 增大初始数据量以更好展示性能差异
    const int OPS = 200000;          // 增加操作次数以获得更稳定的结果
    const int REBUILD_THRESHOLD = 10000; // 增加重建阈值
    const int REPORT_INTERVAL = 10000;  // 调整报告间隔
    
    cout << "初始化数据中... (数据量: " << INIT_SIZE << ")\n";
    
    // 初始化跳表和排序数组
    SkipList sk(MAX_LEVEL);
    vector<int> sortedArray;
    
    // 预分配空间以减少重新分配
    sortedArray.reserve(INIT_SIZE + OPS/2);
    
    // 生成随机初始数据
    vector<int> randomValues(INIT_SIZE);
    for (int i = 0; i < INIT_SIZE; i++) {
        randomValues[i] = rand() % 10000000;
    }
    
    cout << "插入数据中...\n";
    // 分别测量初始化时间
    auto startSkipInit = chrono::high_resolution_clock::now();
    for (int val : randomValues) {
        sk.insert(val);
    }
    auto endSkipInit = chrono::high_resolution_clock::now();
    double skipInitTime = chrono::duration_cast<chrono::milliseconds>(endSkipInit - startSkipInit).count();
    
    auto startArrayInit = chrono::high_resolution_clock::now();
    for (int val : randomValues) {
        insertSortedArray(sortedArray, val);
    }
    auto endArrayInit = chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - startArrayInit).count();
    
    cout << "初始化完成:\n";
    cout << "  跳表初始化: " << skipInitTime << " ms\n";
    cout << "  数组初始化: " << endArrayInit << " ms\n";
    cout << "  速度比: " << (endArrayInit > 0 ? skipInitTime / endArrayInit : 0) << "x\n\n";
    
    cout << "开始性能测试... (操作数: " << OPS << ")\n\n";
    
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

        // 跳表操作计时
        auto startSkip = chrono::high_resolution_clock::now();
        switch(op) {
            case 0: sk.insert(key); break;
            case 1: sk.search(key); break;
            case 2: sk.erase(key); break;
        }
        auto endSkip = chrono::high_resolution_clock::now();
        double timeSkip = chrono::duration_cast<chrono::nanoseconds>(endSkip - startSkip).count();

        // 排序数组操作计时
        auto startArray = chrono::high_resolution_clock::now();
        switch(op) {
            case 0: insertSortedArray(sortedArray, key); break;
            case 1: binarySearch(sortedArray, key); break;
            case 2: deleteSortedArray(sortedArray, key); break;
        }
        auto endArray = chrono::high_resolution_clock::now();
        double timeArray = chrono::duration_cast<chrono::nanoseconds>(endArray - startArray).count();

        // 累计时间
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
            cout << "===== 操作次数 " << i + 1 << " 后的性能报告 =====\n";
            
            // 计算平均时间
            double avgInsertSkip = insertTimesSkip.empty() ? 0 : totalInsertSkip / insertTimesSkip.size();
            double avgSearchSkip = searchTimesSkip.empty() ? 0 : totalSearchSkip / searchTimesSkip.size();
            double avgDeleteSkip = deleteTimesSkip.empty() ? 0 : totalDeleteSkip / deleteTimesSkip.size();
            
            double avgInsertArray = insertTimesArray.empty() ? 0 : totalInsertArray / insertTimesArray.size();
            double avgSearchArray = searchTimesArray.empty() ? 0 : totalSearchArray / searchTimesArray.size();
            double avgDeleteArray = deleteTimesArray.empty() ? 0 : totalDeleteArray / deleteTimesArray.size();
            
            cout << "跳表平均时间 (ns):\n";
            cout << "  插入: " << avgInsertSkip
                 << " | 查找: " << avgSearchSkip
                 << " | 删除: " << avgDeleteSkip << endl;
            
            cout << "排序数组平均时间 (ns):\n";
            cout << "  插入: " << avgInsertArray
                 << " | 查找: " << avgSearchArray
                 << " | 删除: " << avgDeleteArray << endl;
                 
            cout << "性能对比 (跳表时间/数组时间):\n";
            if (avgInsertArray > 0) cout << "  插入: " << avgInsertSkip / avgInsertArray << "x ";
            if (avgSearchArray > 0) cout << " | 查找: " << avgSearchSkip / avgSearchArray << "x ";
            if (avgDeleteArray > 0) cout << " | 删除: " << avgDeleteSkip / avgDeleteArray << "x";
            cout << endl;
            
            // 执行范围查询测试
            testRangeQuery(sk, sortedArray);
            
            cout << "----------------------------------------\n";
        }

        // 改进重建策略，基于跳表高度和操作次数
        if (opCount >= REBUILD_THRESHOLD) {
            double expectedLevel = log2(sortedArray.size()) / 2;
            if (expectedLevel > sk.getCurrentLevel() * 1.5 || 
                (!insertTimesSkip.empty() && 
                insertTimesSkip.back() > 3.0 * (totalInsertSkip / insertTimesSkip.size()))) {
                needRebuild = true;
            }
        }

        // 执行重建
        if (needRebuild) {
            cout << "在 " << i << " 次操作后执行跳表重建 (高度不平衡或性能下降)\n";
            auto startRebuild = chrono::high_resolution_clock::now();
            
            // 调用重建函数
            sk.rebuildSkipList(sk);
            
            auto endRebuild = chrono::high_resolution_clock::now();
            double rebuildTime = chrono::duration_cast<chrono::milliseconds>(endRebuild - startRebuild).count();
            cout << "重建用时: " << rebuildTime << " ms\n";
            
            opCount = 0;
            needRebuild = false;
        }
    }

    // 最终结果统计
    cout << "\n======== 最终统计 ========\n";
    
    double avgInsertSkip = insertTimesSkip.empty() ? 0 : totalInsertSkip / insertTimesSkip.size();
    double avgSearchSkip = searchTimesSkip.empty() ? 0 : totalSearchSkip / searchTimesSkip.size();
    double avgDeleteSkip = deleteTimesSkip.empty() ? 0 : totalDeleteSkip / deleteTimesSkip.size();
    
    double avgInsertArray = insertTimesArray.empty() ? 0 : totalInsertArray / insertTimesArray.size();
    double avgSearchArray = searchTimesArray.empty() ? 0 : totalSearchArray / searchTimesArray.size();
    double avgDeleteArray = deleteTimesArray.empty() ? 0 : totalDeleteArray / deleteTimesArray.size();
    
    cout << "跳表平均时间 (ns):\n";
    cout << "  插入: " << avgInsertSkip
         << " | 查找: " << avgSearchSkip
         << " | 删除: " << avgDeleteSkip << endl;
    
    cout << "排序数组平均时间 (ns):\n";
    cout << "  插入: " << avgInsertArray
         << " | 查找: " << avgSearchArray
         << " | 删除: " << avgDeleteArray << endl;
    
    cout << "\n性能对比 (时间比率):\n";
    
    // 插入操作 - 跳表应该明显优于数组
    cout << "插入操作: ";
    if (avgInsertArray > 0) {
        double ratio = avgInsertSkip / avgInsertArray;
        cout << (ratio < 1 ? "跳表快 " + to_string(1/ratio) + "x" : "数组快 " + to_string(ratio) + "x") << endl;
    } else {
        cout << "无法比较" << endl;
    }
    
    // 查找操作 - 理论上数组的二分查找应该和跳表相当
    cout << "查找操作: ";
    if (avgSearchArray > 0) {
        double ratio = avgSearchSkip / avgSearchArray;
        cout << (ratio < 1 ? "跳表快 " + to_string(1/ratio) + "x" : "数组快 " + to_string(ratio) + "x") << endl;
    } else {
        cout << "无法比较" << endl;
    }
    
    // 删除操作 - 跳表应该明显优于数组
    cout << "删除操作: ";
    if (avgDeleteArray > 0) {
        double ratio = avgDeleteSkip / avgDeleteArray;
        cout << (ratio < 1 ? "跳表快 " + to_string(1/ratio) + "x" : "数组快 " + to_string(ratio) + "x") << endl;
    } else {
        cout << "无法比较" << endl;
    }
    
    // 测试删除最大/最小元素的性能
    cout << "\n特殊操作测试:\n";
    
    // 删除最小元素
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
    
    cout << "删除最小元素:\n";
    cout << "  跳表: " << timeMinSkip << " ns (" << minKey << ")\n";
    cout << "  数组: " << timeMinArray << " ns (" << minValueArray << ")\n";
    if (timeMinArray > 0) {
        double ratio = timeMinSkip / timeMinArray;
        cout << "  比率: " << (ratio < 1 ? "跳表快 " + to_string(1/ratio) + "x" : "数组快 " + to_string(ratio) + "x") << endl;
    }
    
    // 删除最大元素
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
    
    cout << "删除最大元素:\n";
    cout << "  跳表: " << timeMaxSkip << " ns (" << maxKey << ")\n";
    cout << "  数组: " << timeMaxArray << " ns (" << maxValueArray << ")\n";
    if (timeMaxArray > 0) {
        double ratio = timeMaxSkip / timeMaxArray;
        cout << "  比率: " << (ratio < 1 ? "跳表快 " + to_string(1/ratio) + "x" : "数组快 " + to_string(ratio) + "x") << endl;
    }
    
    // 大型范围查询测试
    cout << "\n大型范围查询测试:\n";
    testRangeQuery(sk, sortedArray);
    
    // 理论时间复杂度分析
    cout << "\n理论时间复杂度分析:\n";
    cout << "             | 跳表            | 排序数组\n";
    cout << "--------------------------------------------\n";
    cout << "查找        | O(log n)        | O(log n)\n";
    cout << "插入        | O(log n)        | O(n)\n";
    cout << "删除        | O(log n)        | O(n)\n";
    cout << "删除最小    | O(1)            | O(n)\n";
    cout << "删除最大    | O(log n)        | O(1)\n";
    cout << "范围查询    | O(log n + k)    | O(log n + k)\n";
    cout << "--------------------------------------------\n";
    cout << "注: n 为元素总数，k 为范围内元素数量\n";
    
    // 添加更多理论分析和性能说明
    cout << "\n跳表性能分析:\n";
    cout << "1. 查找和插入: 跳表O(log n)性能接近理想平衡树\n";
    cout << "2. 随机访问: 跳表查找比平衡树慢约30%，但实现更简单\n";
    cout << "3. 空间占用: 跳表平均每个节点额外使用1/(1-p)个指针\n";
    cout << "4. 删除操作: 跳表删除只需O(log n)时间，不需要重平衡\n";
    cout << "5. 范围查询: 跳表范围查询性能优秀，适合此类应用\n\n";
    
    cout << "跳表的优势应用场景:\n";
    cout << "1. 频繁插入和删除的动态数据集\n";
    cout << "2. 内存数据库索引\n";
    cout << "3. 需要快速范围查询的应用\n";
    cout << "4. 对删除最小元素有O(1)需求的优先队列实现\n";
}

int main() {
    // 设置更大的随机数种子以增加随机性
    srand(time(nullptr) ^ (unsigned)chrono::system_clock::now().time_since_epoch().count());
    experiment();
    // 释放内存池
    for (auto node : SkipNode::pool) {
        ::operator delete(node);
    }
    SkipNode::pool.clear();
    return 0;
}