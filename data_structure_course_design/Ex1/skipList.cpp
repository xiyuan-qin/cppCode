#include <iostream>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <climits>
#include <chrono>
#include <algorithm>
#include <cmath>
#include <iomanip>
#include "SkipList.h"
#include "exp.h"

using namespace std;

// 初始化静态内存池
vector<SkipNode*> SkipNode::pool;

// SkipNode
// 重载节点的new和delete操作符，实现内存池
void* SkipNode::operator new(size_t size) {
    if (!pool.empty()) {
        SkipNode* node = pool.back();
        pool.pop_back();
        return node;
    }
    return ::operator new(size);
}

void SkipNode::operator delete(void* ptr) {// 删除实际上是将节点放回内存池
    if (pool.size() < 10000) { // 限制池大小，避免过度占用内存
        pool.push_back(static_cast<SkipNode*>(ptr));
    } else {
        ::operator delete(ptr);
    }
}

SkipNode::SkipNode(int key, int level) {
    this->key = key;
    this->next = vector<SkipNode*>(level, nullptr);
}

// SkipList
SkipList::SkipList(int maxLvl, float p) : maxLevel(maxLvl), currentLevel(1), probability(p) {
    header = new SkipNode(INT_MIN, maxLevel);
    srand(time(0));
}

SkipList::~SkipList() {
    SkipNode* node = header->next[0];
    while (node) {
        SkipNode* next = node->next[0];
        delete node;
        node = next;
    }
    delete header;
}

int SkipList::randomLevel() {
    int lvl = 1;
    while ((rand() / (float)RAND_MAX) < probability && lvl < maxLevel)
        lvl++;
    return lvl;
}

bool SkipList::search(int key) {
    SkipNode* current = header;
    for (int i = currentLevel - 1; i >= 0; i--) {
        while (current->next[i] && current->next[i]->key < key)// 如果下一个上层节点的值大了，进入下一层
            current = current->next[i];
    }
    current = current->next[0];
    return current != nullptr/*没找到*/ && current->key == key/*找到了*/;
}

void SkipList::insert(int key) {
    vector<SkipNode*> update(maxLevel, nullptr);
    SkipNode* current = header;

    for (int i = currentLevel - 1; i >= 0; i--) {
        while (current->next[i] && current->next[i]->key < key)
            current = current->next[i];
        update[i] = current;
    }

    current = current->next[0];// 找到插入的位置
    if (current && current->key == key) return;

    int newLevel = randomLevel();
    if (newLevel > currentLevel) {// 如果新节点的层数大于当前最大层数，按照最大层数更新update数组
        for (int i = currentLevel; i < newLevel; i++)
            update[i] = header;
        currentLevel = newLevel;
    }

    // update比插入的key要小
    SkipNode* newNode = new SkipNode(key, newLevel);
    for (int i = 0; i < newLevel; i++) {
        newNode->next[i] = update[i]->next[i];
        update[i]->next[i] = newNode;
    }
}

bool SkipList::erase(int key) {
    vector<SkipNode*> update(maxLevel, nullptr);
    SkipNode* current = header;

    // 优化查找路径 - 提前结束不必要的查找
    for (int i = currentLevel - 1; i >= 0; i--) {
        while (current->next[i] && current->next[i]->key < key)
            current = current->next[i];
        update[i] = current;
    }

    current = current->next[0];
    if (!current || current->key != key) return false;
    
    // 优化删除操作 - 只更新必要的层
    for (int i = 0; i < currentLevel && update[i]->next[i] == current; i++) {
        update[i]->next[i] = current->next[i];
    }

    // 使用内存池而不是直接释放
    delete current;
    
    // 只在必要时更新层数
    int topLevel = currentLevel - 1;
    while (topLevel > 0 && header->next[topLevel] == nullptr)
        topLevel--;
    currentLevel = topLevel + 1;

    return true;
}

int SkipList::deleteMin() {//返回最小值
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

int SkipList::deleteMax() {
    if (!header->next[0]) return INT_MIN;
    
    SkipNode* current = header;
    
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

void SkipList::rebuildSkipList(SkipList& sk) {
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

vector<int> SkipList::rangeQuery(int minKey, int maxKey) {// 查找范围内的元素
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

int SkipList::getCurrentLevel() const {
    return currentLevel;
}

int SkipList::getMaxLevel() const {
    return maxLevel;
}

// 主函数
int main() {
    //随机一个种子
    srand(time(nullptr) ^ (unsigned)chrono::system_clock::now().time_since_epoch().count());
    
    experiment();
    
    for (auto node : SkipNode::pool) {
        ::operator delete(node);
    }
    SkipNode::pool.clear();
    return 0;
}